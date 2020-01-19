#!/usr/bin/python3

import mraa
import time
import spidev 
from time import sleep
import os

spi = spidev.SpiDev()
spi.open(1,0)

luz = 0

espera = 1

pin_rele = 26

pin_r = 22
pin_g = 27
pin_b = 23
# Export the GPIO pin for use
pin_rele = mraa.Gpio(pin_rele,owner=False,raw=True)

pin_r = mraa.Gpio(pin_r,owner=False,raw=True)
pin_g = mraa.Gpio(pin_g,owner=False,raw=True)
pin_b = mraa.Gpio(pin_b,owner=False,raw=True)

pin_rele.dir(mraa.DIR_OUT)

pin_r.dir(mraa.DIR_OUT)
pin_g.dir(mraa.DIR_OUT)
pin_b.dir(mraa.DIR_OUT)

time.sleep(1)

# Configure the pin direction
pin_rele.dir(mraa.DIR_OUT)

pin_r.dir(mraa.DIR_OUT)
pin_g.dir(mraa.DIR_OUT)
pin_b.dir(mraa.DIR_OUT)

def leer(canal):
	rawData = spi.xfer([1,(8+canal) << 4,0])
	processedData = ((rawData[1]&3) << 8)+ rawData[2]

	return processedData

# Loop
while True:

    datos= leer(luz)
    print (datos)
    if datos > 700:
        pin_r.write(1)
        print ("Encender led")
    else:
        pin_r.write(0)
    sleep(espera)
