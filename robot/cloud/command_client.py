"""Remotely drive the car"""

import socket
import curses

HOST='localhost'
PORT=9089

s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect((HOST,PORT))


screen = curses.initscr()
while True:
    try:
        curses.noecho()
        curses.curs_set(0)
        screen.keypad(1)
        screen.addstr("Press a key")
        event = screen.getch()
    finally:
        curses.endwin()

    if event == curses.KEY_LEFT:
        print('left arrow key pressed')
        s.send('left')
    elif event == curses.KEY_RIGHT:
        print('right arrow key pressed')
        s.send('right')
    elif event == curses.KEY_UP:
        print('up arrow key pressed')
        s.send('up')
    elif event == curses.KEY_DOWN:
        print('down arrow key pressed')
        s.send('down')
    elif event == 32:
        print('space arrow key pressed')
        s.send('stop')
    else:
        print(event)
        break
    reply = s.recv(1024)
    print(reply)
