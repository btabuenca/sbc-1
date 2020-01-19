#!/usr/bin/python3

import mraa
import time

# Use pin 7 by default
pin_r = 22
pin_g = 27
pin_b = 23
# Export the GPIO pin for use
pin_r = mraa.Gpio(pin_r,owner=False,raw=True)
pin_g = mraa.Gpio(pin_g,owner=False,raw=True)
pin_b = mraa.Gpio(pin_b,owner=False,raw=True)

pin_r.dir(mraa.DIR_OUT)
pin_g.dir(mraa.DIR_OUT)
pin_b.dir(mraa.DIR_OUT)


# Small delay to allow udev rules to execute (necessary only on up)
time.sleep(1)

# Configure the pin direction
pin_r.dir(mraa.DIR_OUT)
pin_g.dir(mraa.DIR_OUT)
pin_b.dir(mraa.DIR_OUT)

# Loop
while True:
    # Turn the LED on and wait for 1 seconds
    pin_r.write(1)
    print("Encendido")
    time.sleep(1)
    # Turn the LED off and wait for 1 seconds
    pin_r.write(0)
    print("Apagado")
    time.sleep(1)

# Turn the LED on and wait for 1 seconds
#    pin_g.write(1)
#    print("Encendido")
#    time.sleep(1)
    # Turn the LED off and wait for 1 seconds
#    pin_g.write(0)
#    print("Apagado")
#    time.sleep(1)


# Turn the LED on and wait for 1 seconds
#    pin_b.write(1)
#    print("Encendido")
#    time.sleep(1)
    # Turn the LED off and wait for 1 seconds
#    pin_b.write(0)
#    print("Apagado")
#    time.sleep(1)

