import socket
import sys
import os
import curses
from threading import Thread


class RemoteControlServer(object):
    """docstring for Curses_control"""
    def __init__(self):
        super(RemoteControl, self).__init__()
        self.data = ''
        self.stopped = False
        self.HOST = os.environ.get('COMMAND_HOST', 'localhost')
        self.PORT = os.environ.get('COMMAND_PORT', 9089)

    def start(self):
        self.socket_server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        print('Socket created')

        self.socket_server.bind((self.HOST, self.PORT))
        print('Socket bind complete')

        self.socket_server.listen(10)
        print('Socket now listening')

        self.conn, self.addr = self.socket_server.accept() # Accept the connection once (for starter)
        print('Connected with ' + self.addr[0] + ':' + str(self.addr[1]))

        Thread(target=self.update, args=()).start()
        return self

    def update(self):
        while True:
            try:
                self.data = self.conn.recv(1024)
                self.conn.send(self.data)
                print(self.data)
                if self.data == 27:
                    self.stop()
                    return
            except socket.error as e:
                print(e)
                self.stop()
                return
            else:
                if len(self.data) == 0:
                    print 'orderly shutdown on server end'
                    self.stop()
                else:
                    print(self.data)

    def read(self):
        return self.data

    def stop(self):
        self.stopped = True

        self.conn.close()
        self.socket_server.close()


class CursesControl(object):
    """docstring for Curses_control"""
    def __init__(self):
        super(CursesControl, self).__init__()
        # self.screen.nodelay()
        self.event = 'unload'
        self.stopped = False

    def start(self):
        self.screen = curses.initscr()
        Thread(target=self.update, args=()).start()
        return self

    def update(self):
        while True:
            try:
                curses.noecho()
                curses.curs_set(0)
                self.screen.keypad(1)
                self.screen.addstr("Press a key, " + str(self.event))
                self.event = self.screen.getch()
            finally:
                curses.endwin()
            if self.stopped or self.event == 27:
                return

    def read(self):
        if self.event == curses.KEY_LEFT:
            command = 'left'
        elif self.event == curses.KEY_RIGHT:
            command = 'right'
        elif self.event == curses.KEY_UP:
            command = 'up'
        elif self.event == curses.KEY_DOWN:
            command = 'down'
        elif self.event == 32: # SPACE
            command = 'stop'
        elif self.event == 27: # ESC key
            command = 'quit'
        elif self.event == ord('p'): # P key
            command = 'auto_logic_based'
        elif self.event == ord('o'): # O key
            command = 'stream'
        elif self.event == ord('m'): # O key
            command = 'auto_neural_network'
        else:
            command = '?'
        return command

    def stop(self):
        self.stopped = True
