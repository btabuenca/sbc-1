
#import DemoLed as DM
#!/usr/bin/python3
import spidev 
from time import sleep
import os
import mraa
import time
# Use pin 7 by default
pin_rele = 26
# Export the GPIO pin for use
pin_rele = mraa.Gpio(pin_rele,owner=False,raw=True)
# Configure the pin direction
pin_rele.dir(mraa.DIR_OUT)

def releOFF():
	pin_rele.write(0)
