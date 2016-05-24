import socket
import sys
import cv2
import pickle
import numpy as np
import struct ## new

cap = cv2.VideoCapture(0)

HOST = 'localhost'
PORT = 8089

clientsocket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
clientsocket.connect((HOST, PORT))

while True:
    ret, frame = cap.read()
    frame = cv2.resize(frame, (720, 480)) # resize every photo
    data = pickle.dumps(frame)
    clientsocket.sendall(struct.pack("L", len(data)) + data)