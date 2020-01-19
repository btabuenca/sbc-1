mport smbus
import time

dire = 0x00

bus= smbus.SMBus(1) # /dev/ic2-1
bus.write_byte_data(dire,0x00, 0x00)

while true: 

	valor= bus.read_byte_data(dir)
	print(valor)
	time.sleep(1)

	



