
import random
import socket
import time


#server_socket.bind((UDP_IP, UDP_PORT))

def snd_data(msg):

    UDP_IP = '10.25.104.165'
    UDP_PORT_snd = 12345
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
    sock.sendto(bytes([msg]), (UDP_IP, UDP_PORT_snd))
    time.sleep(2)
    #rand = random.randint(0, 10)
    #message, address = server_socket.recvfrom(1024)
    #message = message.upper()
    #if rand >= 4:
    #    server_socket.sendto(message, address)


#server_socket.bind((UDP_IP, UDP_PORT))
#server_socket.connect((UDP_IP,UDP_PORT
def rcv_data():
    while True:
        UDP_IP = '10.25.104.165'
        UDP_PORT_rcv = 4321
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        try:
            server_socket.bind(('',UDP_PORT_rcv))
        except:
            buffer_size = 1000
            data = server_socket.recvfrom(buffer_size)
            print data
    return data
