import io
import socket
import struct
import os
import cv2
import pickle
from threading import Thread
import curses
import time
from ai import detection, preprocessing

class RemoteControlClient(object):
    """docstring for RemoteControlClient"""
    def __init__(self):
        super(RemoteControlClient, self).__init__()
        self.data = ''
        self.stopped = False
        self.HOST = os.environ.get('RPI_IP', 'localhost')
        self.PORT = os.environ.get('COMMAND_PORT', 9089)

    def start(self):
        self.socket_client = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        print('Socket created')

        self.socket_client.connect((self.HOST, self.PORT))
        print('Socket connect')

        screen = curses.initscr()

        Thread(target=self.update, args=()).start()
        return self

    def update(self):
        while True:
            try:
                curses.noecho()
                curses.curs_set(0)
                screen.keypad(1)
                screen.addstr("Press a key")
                event = screen.getch()
            finally:
                curses.endwin()

            print(event)
            if event == curses.KEY_LEFT:
                socket_client.send('left')
            elif event == curses.KEY_RIGHT:
                socket_client.send('right')
            elif event == curses.KEY_UP:
                socket_client.send('up')
            elif event == curses.KEY_DOWN:
                socket_client.send('down')
            elif event == ord('r'):
                socket_client.send('record')
            elif event == ord('t'):
                socket_client.send('stop_record')
            elif event == ord('p'):
                socket_client.send('setup')
            elif event == 32: # space
                socket_client.send('stop')
            else:
                break

            reply = socket_client.recv(1024)
            print(reply)


    def read(self):
        return self.data

    def stop(self):
        self.stopped = True

        self.conn.close()
        self.socket_server.close()


class VisionStreamServer(object):
    """docstring for Curses_control"""
    def __init__(self):
        super(VisionStreamServer, self).__init__()
        self.frame = ''
        self.buffer = ''
        self.data = ''
        self.HOST = os.environ.get('LOCAL_IP', 'localhost')
        self.PORT = os.environ.get('CAPTURE_PORT', 8089)
        
    def start(self):
        self.socket_server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        print('Socket created')

        self.socket_server.bind((self.HOST, self.PORT))
        print('Socket bind complete')

        self.socket_server.listen(10)
        print('Socket now listening')

        self.connection, addr = self.socket_server.accept() # Accept the connection once (for starter)
        print('Connected with ' + addr[0] + ':' + str(addr[1]))

        self.payload_size = struct.calcsize('<L') 
        return self

    def read(self):
        try:
            while len(self.data) < self.payload_size:
                self.data += self.connection.recv(4096)
            packed_msg_size = self.data[:self.payload_size]
            self.data = self.data[self.payload_size:]
            msg_size = struct.unpack('<L', packed_msg_size)[0]

            while len(self.data) < msg_size:
                self.data += self.connection.recv(4096)
            frame_data = self.data[:msg_size]
            self.data = self.data[msg_size:]

            self.buffer = pickle.loads(frame_data)

            if len(frame_data) == 0:
                print('orderly shutdown on server end')
                self.stop()
                return

            while len(self.data) < self.payload_size:
                self.data += self.connection.recv(4096)
            packed_msg_size = self.data[:self.payload_size]
            self.data = self.data[self.payload_size:]
            msg_size = struct.unpack('<L', packed_msg_size)[0]
            while len(self.data) < msg_size:
                self.data += self.connection.recv(4096)
            other_data = self.data[:msg_size]
            self.data = self.data[msg_size:]
            print(pickle.loads(other_data))
            
            # self.callback(cv2.imdecode(self.buffer, cv2.CV_LOAD_IMAGE_COLOR))
            return cv2.imdecode(self.buffer, cv2.CV_LOAD_IMAGE_COLOR)
        except socket.error as e:
            print(e)
            self.stop()
            return
        
    def stop(self):
        self.connection.close()
        self.socket_server.close()

    def __exit__(self):
        self.stop()


def main():
    vss = VisionStreamServer().start()
    time.sleep(2)
    frame_times = []
    while True:
        frame = vss.read()
        frame_times.append(time.time())
        if len(frame_times) > 10:
            framerate = 10.0/(frame_times[-1] - frame_times[-10])
        else:
            framerate = 0

        detected = detection.detections(frame)
        frame = detection.draw_pattern(frame, detected['stop'],  style='roi',  color=(0, 0, 255))
        frame = detection.draw_pattern(frame, detected['lines'], style='line', color=(255, 0, 0))
        mask = preprocessing.track_line(frame)
        
        mask = cv2.cvtColor(mask, cv2.cv.CV_GRAY2RGB)
        dst = cv2.add(frame,mask)

        cv2.putText(dst,
                    str(framerate)[:3] + 'FPS',
                    (dst.shape[1]-100,dst.shape[0]-30),
                    cv2.FONT_HERSHEY_PLAIN,
                    1,
                    cv2.cv.CV_RGB(255,255,0),
                    thickness=1)

        # dst = cv2.Canny(frame, 50, 150, apertureSize = 3)
        cv2.imshow('Video', dst)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            cv2.destroyAllWindows()
            break


if __name__ == '__main__':
    main()
