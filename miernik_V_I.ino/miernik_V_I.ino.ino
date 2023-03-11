// SD card section
#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;

// sensors section
#define Voltage_pin     A0
#define Voltage_pin_led 7
#define Current_pin     A1
#define Current_pin_led 6
#define voltage_ref     5.00
#define DataUpdate_Led  8
#define Rref            984.0

void logger(double v, double c);


void setup() {
  pinMode(Voltage_pin_led, OUTPUT);
  pinMode(Current_pin_led, OUTPUT);
  pinMode(DataUpdate_Led, OUTPUT);

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    for (int i; i < 10; i++) {
      digitalWrite(DataUpdate_Led, !digitalRead(DataUpdate_Led));
    }
    while (1);
  }
  Serial.println("card initialized.");
}

unsigned long curr_time = millis();
double voltage_adc, voltage, voltage_adc_i, voltage_i, sensor_voltage, current;

void loop() {
  if (millis() - curr_time == 1000u || curr_time == millis())
  {
    curr_time = millis();
    voltage_adc = (analogRead(Voltage_pin) * voltage_ref) / 1024.0;
    voltage = voltage_adc;

    digitalWrite(Voltage_pin_led, !digitalRead(Voltage_pin_led));

    voltage_adc_i = (analogRead(Current_pin) * voltage_ref) / 1024.0;
    voltage_i = voltage_adc_i;
    sensor_voltage = (double)voltage - (double)voltage_i; 
    current = (double)voltage_adc_i / (double)Rref;

    digitalWrite(Current_pin_led, !digitalRead(Current_pin_led));
    
    logger(sensor_voltage, current);
  }
}

void logger(double v, double c)
{                  
  File datalog = SD.open("test.txt", FILE_WRITE);       

  if (datalog)
  {         
    String str = "";
    str += String("Voltage: ");
    str += String(v, 4);
    str += String("V Current: ");
    str += String(c, 4);
    str += String("A Total power: ");
    str += String(v * c, 4);
    str += String("W.\r\n");
    datalog.print(str);
    datalog.close();
    digitalWrite(DataUpdate_Led, !digitalRead(DataUpdate_Led));
  }
  else{
    Serial.println("SD card problem!");
  }                       
}
