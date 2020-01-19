#/usr/bin/python3

import mraa
import time

# Use pin 7 by default
pin_rele = 26

# Export the GPIO pin for use
pin_rele = mraa.Gpio(pin_rele,owner=False,raw=True)

pin_rele.dir(mraa.DIR_OUT)

# Small delay to allow udev rules to execute (necessary only on up)
time.sleep(1)

# Configure the pin direction
pin_rele.dir(mraa.DIR_OUT)

def rele():
	pin_rele.write(1)	

	time.sleep(3)

	pin_rele.write(0)

	time.sleep(1)
	
while True:
	
	rele()	

