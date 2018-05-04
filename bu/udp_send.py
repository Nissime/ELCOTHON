
import random
import socket
import time
UDP_IP = '10.25.104.165'
UDP_PORT = 12345
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#server_socket.bind((UDP_IP, UDP_PORT))

while True:
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
    sock.sendto(bytes([49,49]), (UDP_IP, UDP_PORT))
    time.sleep(2)
    #rand = random.randint(0, 10)
    #message, address = server_socket.recvfrom(1024)
    #message = message.upper()
    #if rand >= 4:
    #    server_socket.sendto(message, address)
