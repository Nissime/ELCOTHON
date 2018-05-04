
import random
import socket
import time
import elecApl as ea
import matplotlib.pyplot as plt
import numpy as np
plt.ion()
fig = plt.figure()

#UDP_IP = '10.25.104.110'
UDP_IP = '10.25.104.165'



UDP_PORT = 4321
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#server_socket.bind((UDP_IP, UDP_PORT))
buffer_size = 1000
server_socket.bind(('',UDP_PORT))
n_samples = 40
ID_Valid = 0
#server_socket.connect((UDP_IP,UDP_PORT
measurements = []
xxxx, yyyy = np.meshgrid(np.linspace(-100, 1200, 500), np.linspace(-100, 1200, 500))

clearFig=False
while True:


	data = server_socket.recvfrom(buffer_size)
	data_str = data[0]
	if data_str[0]=='\xff' and not ID_Valid:
		print('ID Varified')
		data_ord = [(x+y) for x,y in zip(data_str[1::2],data_str[2::2])]
		ID =  [int(''.join('%02x' % ord(c) for c in xx),16) for xx in data_ord]
		ea1 = ea.elAp(34,n_samples)
		ID_Valid = 1
	if data_str[0]=='\xff' and ID_Valid:
		print('Got Data')
		data_ord = [(x+y) for x,y in zip(data_str[1::2],data_str[2::2])]
		measurements = [int(''.join('%02x' % ord(c) for c in xx),16) for xx in data_ord]
		clsRes=1
		if ea1.state=='train':
			ea1.addTrainData(measurements)
		else:
			clsRes = ea1.clsfy(measurements)
			if clearFig:
				plt.clf()
			else:
				Z = ea1.clf.decision_function(np.c_[xxxx.ravel(), yyyy.ravel()])
				Z = Z.reshape(xxxx.shape)
			clearFig=True
			#import pdb; pdb.set_trace()
			plt.title('Predictive Maintenance')
			x_display = [x for x, y in ea1.X_train]
			y_display = [y for x, y in ea1.X_train]
			plt.scatter(x_display,y_display,c='gold',edgecolors='k')
			plt.hold(True)

			plt.contour(xxxx, yyyy, Z, levels=[0], linewidths=4, colors='darkred')
			plt.grid(True)
			plt.scatter(max(measurements)-min(measurements), int(np.mean(measurements)),c='white',edgecolors='k')
			#import pdb; pdb.set_trace()
			#plt.plot(measurements)
			plt.hold(False)
			plt.xlim(-100,1200)
			plt.ylim(400,800)

			plt.show()
			#fig.savefig('foo3.png')
			fig.canvas.draw()
			fig.canvas.flush_events()
			#break
