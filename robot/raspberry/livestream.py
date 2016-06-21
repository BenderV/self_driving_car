import socket
import struct
import os
import pickle
import cv2

class LiveStreamClient(object):
    """docstring for LiveStreamClient"""
    def __init__(self):
        super(LiveStreamClient, self).__init__()
        self.data = ''
        self.stopped = False
        self.HOST = os.environ.get('CAPTURE_HOST', 'localhost')  # RPI_IP
        self.PORT = os.environ.get('COMMAND_PORT', 8089)

    def start(self):
        self.socket_client = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        print('Socket created')

        self.socket_client.connect((self.HOST, self.PORT))
        print('Socket connect')

        self.connection = self.socket_client.makefile('wb')
        return self

    def send(self, frame):
        ret, jpeg = cv2.imencode('.jpg', frame)
        data = pickle.dumps(jpeg) ### new code
        self.socket_client.sendall(struct.pack('<L', len(data))+data) ### new code
        data = pickle.dumps('test-test--test')
        self.socket_client.sendall(struct.pack('<L', len(data))+data) ### new code

    def read(self):
        return self.data

    def stop(self):
        self.stopped = True
        self.connection.close()
        self.socket_client.close()
