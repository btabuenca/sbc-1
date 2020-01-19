import spidev 
from time import sleep
import os

spi = spidev.SpiDev()
spi.open(1,0)

luz = 0
agua = 1
nivel = 2

espera = 1

def leer(canal):
	rawData = spi.xfer([1,(8+canal) << 4,0])
	processedData = ((rawData[1]&3) << 8)+ rawData[2]

	return processedData

while True:
	datos_luz = leer(luz)
#	print (datos)
	if datos_luz > 700:
		print ("Encender led")
	sleep(espera)
	
	datos_agua= leer(agua)
#	print(datos_agua)
	if datos_agua < 400:
		print("Agua detectada")
	
	datos_nivel = leer(nivel)
	print(datos_nivel)
sleep(espera)
