import socket
import sys
import cv2
import pickle
import numpy as np
import struct


HOST='localhost'
PORT=9089

s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
print('Socket created')

s.bind((HOST, PORT))
print('Socket bind complete')

s.listen(10)
print('Socket now listening')

conn, addr = s.accept() # Accept the connection once (for starter)
print('Connected with ' + addr[0] + ':' + str(addr[1]))

data = ""

while True:
    data = conn.recv(1024)
    if not data:
        break

    reply = 'got...' + data
    print(data)
    conn.send(data)

conn.close()
s.close()