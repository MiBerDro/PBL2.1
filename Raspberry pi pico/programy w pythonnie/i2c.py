from machine import Pin, I2C

i2c = machine.I2C(0, scl=Pin(1), sda=Pin(0))

device = hex(i2c.scan()[0])

print(device)