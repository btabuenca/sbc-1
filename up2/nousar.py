import smbus
import time

dir = 5a
bus= smbus.SMBus(1) # cambiar el 1 si no funciona

bus.write_i2c_block_data(dir, 0x24, [0x06])

time.sleep(0.5)

data= bus.read_i2c_block_data(dir, 0x00, 6)

temp = data[0] * 256 + data[1]
cTemp = -45 + (175 * temp / 65535.0)
humedad = 100 *(data[3] * 256 + data[4]) / 65535.0

print " Temperatura: %.2f C" %cTemp
print " Humedad relativa : %.2f %%RH" %humedad
