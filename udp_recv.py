import random
import socket
import time
UDP_IP = '10.25.104.165'
UDP_PORT = 4321
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#server_socket.bind((UDP_IP, UDP_PORT))
buffer_size = 1000
server_socket.bind(('',UDP_PORT))

#server_socket.connect((UDP_IP,UDP_PORT

while True:
    data = server_socket.recvfrom(buffer_size)
    data_str = data[0]
    if data_str[0]=='\xff':
        data_ord = [(x+y) for x,y in zip(data_str[1::2],data_str[2::2])]
        print [int(''.join('%02x' % ord(c) for c in xx),16) for xx in data_ord]
    if data_str[0]=='\xfe':
        data_ord = [(x+y) for x,y in zip(data_str[1::2],data_str[2::2])]
        print [int(''.join('%02x' % ord(c) for c in xx),16) for xx in data_ord]


    #rand = random.randint(0, 10)
    #message, address = server_socket.recvfrom(1024)
    #message = message.upper()
    #if rand >= 4:
    #    server_socket.sendto(message, address)
