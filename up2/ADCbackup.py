import spidev 
from time import sleep
import os

spi = spidev.SpiDev()
spi.open(1,0)

luz = 0
detectar_agua = 1
nivel_agua = 2

espera = 0.5

def leer(canal):
	rawData = spi.xfer([1,(8+canal) << 4,0])
	processedData = ((rawData[1]&3) << 8)+ rawData[2]

	return processedData

while True:
	datos_luz = leer(luz)
#	print (datos_luz)
	if datos_luz > 700:
		print ("Encender led")
	sleep(espera)
	
	datos_detectar_agua= leer(detectar_agua)
#	print(datos_detectar_agua)
	if datos_detectar_agua < 400:
		print("Agua detectada")
	sleep(espera)

	datos_nivel_agua = leer(nivel_agua)

	resultado = 100-((int(datos_nivel_agua)/10))
	resultado = int(resultado)

	if(resultado>0) :
		print(str(resultado) + '%')


