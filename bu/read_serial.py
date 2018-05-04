import serial
import time
import binascii
import matplotlib.pyplot as plt
ser = serial.Serial('/dev/ttyACM2', 9600)
counter = 0
measurement = []
#plt.ion()
#fig = plt.figure()



counter = counter +1
data = ser.read()[:]
rx=binascii.hexlify(bytearray(data))
print rx
if rx == 'ff':
	measurement = []
	for i in range(0,500):

		data = ser.read()[:]
		rx=binascii.hexlify(bytearray(data))
		high = rx
		data = ser.read()[:]
		rx=binascii.hexlify(bytearray(data))
		low = rx
		measurement.append(int(high+low,16))
		plt.clf()

	plt.plot(measurement)
	plt.show()

	fig.canvas.draw()
	fig.canvas.flush_events()
