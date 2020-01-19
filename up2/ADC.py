#import DemoLed as DM
#!/usr/bin/python3
import spidev 
from time import sleep
import os
import mraa
import time

# Use pin 7 by default
pin_r = 24
pin_g = 23
pin_b = 22

# Use pin 7 by default
pin_rele = 26

# Export the GPIO pin for use
pin_r = mraa.Gpio(pin_r,owner=False,raw=True)
pin_g = mraa.Gpio(pin_g,owner=False,raw=True)
pin_b = mraa.Gpio(pin_b,owner=False,raw=True)

# Export the GPIO pin for use
pin_rele = mraa.Gpio(pin_rele,owner=False,raw=True)

pin_r.dir(mraa.DIR_OUT)
pin_g.dir(mraa.DIR_OUT)
pin_b.dir(mraa.DIR_OUT)

# Configure the pin direction
pin_rele.dir(mraa.DIR_OUT)
# Small delay to allow udev rules to execute (necessary only on up)
time.sleep(1)

# Configure the pin direction
pin_r.dir(mraa.DIR_OUT)
pin_g.dir(mraa.DIR_OUT)
pin_b.dir(mraa.DIR_OUT)

def led(r, g, b):
	if(r==1):
		pin_r.write(1)
	#	print("rojo")	
	if(g==1):
		pin_g.write(1)
	#	print("verde")
	if(b==1):
		pin_b.write(1)
	#	print("azul")
	time.sleep(1)

	pin_r.write(0)
	pin_g.write(0)
	pin_b.write(0)

	time.sleep(1)
	

spi = spidev.SpiDev()
spi.open(1,0)

luz = 0
detectar_agua = 1
nivel_agua = 2
nivel_peso = 3
espera = 0.5

def rele():
	pin_rele.write(1)	

	time.sleep(3)

	pin_rele.write(0)


def leer(canal):
	#print(spi.xfer([1, (8+canal) << 4,0]))
	rawData = spi.xfer([1,(8+canal) << 4,0])
	processedData = ((rawData[1]&3) << 8)+ rawData[2]

	return processedData

while True:

	rele()

	datos_luz = leer(luz)
	datos_luz = 1024 - datos_luz 
#	print("luz") 
	if datos_luz < 300:
		led(1,0,0) #Enciende rojo si hay luz
		sleep(espera)
	
	datos_detectar_agua= leer(detectar_agua)
	datos_detectar_agua = 1024 - datos_detectar_agua
	if datos_detectar_agua < 400:
		led(0, 1, 0)#Enciende verde si hay agura
	sleep(espera)
	primer_nivel = leer(nivel_agua)
	
	resultado = (100*((int(nivel_agua)))/1024)
	resultado = int(resultado)
	print(resultado)
	if(resultado>0):
		print(str(resultado) + '%')
	if(resultado > 15):
		led(0, 0, 1) 

