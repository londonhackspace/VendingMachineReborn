#!/usr/bin/python
from time import sleep
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
import serial
import binascii
import array
GPIO.setup(17, GPIO.OUT)
GPIO.setup(27, GPIO.OUT)
GPIO.setup(22, GPIO.OUT)
GPIO.setup(5,  GPIO.OUT)
GPIO.output(17, 0)
GPIO.output(27, 0)
GPIO.output(22, 0)
GPIO.output(5, 0)
elements=[2, 0, 1, 229, 24]
inputlength = len(elements) * 2
lastcount = 0
ser = serial.Serial('/dev/ttyUSB0')
a_var=bytearray(elements)

while 1:
	ser.write(a_var)
	b_var = ser.read(21)
	hexly = binascii.hexlify(b_var)
	response = hexly[inputlength:len(hexly)]
	responsebyte = []
	decresponse = []

	for x in range (0, len(response) / 2):
		indexstarter = x*2
		responsebyte.append(response[indexstarter:indexstarter+2])
	for x in range (0, len(response) / 2):
		decresponse.append(int(responsebyte[x], 16))
	eventcounter = decresponse[4]
	events = decresponse[5:15]
	lastcount = eventcounter
	if eventcounter != lastcount:
		lastcount = eventcounter
		print("NEW EVENT")
		latestevent=events[0:2]
		print(latestevent)
		if latestevent[1] == 1:
			print("COIN ACCEPTED!!")
			if latestevent[0] == 1:
				print("1 GBP coin accepted, pulling up BCM pin 17")
				GPIO.output(17, 1)
				sleep(0.01)
				GPIO.output(17, 0)
			elif latestevent[0] == 2:
				print("2 GBP coin accepted, pulling up BCM pin 27")
				GPIO.output(27, 1)
				sleep(0.01)
				GPIO.output(27, 0)
			elif latestevent[0] == 3:
				print("50p coin accepted, pulling up BCM pin 22")
				GPIO.output(22, 1)
				sleep(0.01)
				GPIO.output(22, 0)
			elif latestevent[0] == 4:
				print("20p coin accepted, pulling up BCM pin 5")
				GPIO.output(5, 1)
				sleep(0.01)
				GPIO.output(5, 0)
			elif latestevent[0] == 5:
				print("coin 5???")
			else:
				print("ERROR AT COIN EVAL (COIN REJECT)")
		else:
			print("ERROR - COIN REJECTED")
