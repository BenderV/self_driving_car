import socket
import sys
import cv2
import pickle
import numpy as np
import struct
import csv
### SAVING PARAMS


HOST = 'localhost'
PORT = 8089

s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
print('Socket created')

s.bind((HOST, PORT))
print('Socket bind complete')

s.listen(10)
print('Socket now listening')

conn, addr = s.accept()

data = ""
payload_size = struct.calcsize("L") # ?

count = 0

with open('./records/records.csv', 'a+') as csvfile:
    writer = csv.writer(csvfile, delimiter=',')

    while True:
        while len(data) < payload_size:
            data += conn.recv(4096)
        packed_msg_size = data[:payload_size]
        data = data[payload_size:]
        msg_size = struct.unpack("L", packed_msg_size)[0]
        while len(data) < msg_size:
            data += conn.recv(4096)
        frame_data = data[:msg_size]
        data = data[msg_size:]
        frame=pickle.loads(frame_data)

        # write the frame
        name = "./records/frames/frame-{0}.jpg".format(count)
        cv2.imwrite(name, frame)
        writer.writerow([name, 'test', 1])
        count += 1

        # Display the resulting frame
        cv2.imshow('video', frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

cv2.destroyAllWindows()
conn.close()
s.close()