import curses
import atexit
import RPi.GPIO as GPIO
import time
import picamera
import io
import datetime, time
from functools import wraps

def save_state():
    state = {}
    pins = [11, 13, 15, 16]
    state['pins'] = [GPIO.input(i) for i in pins]
    state['time'] = time.time()
    return state

def record_state(tag_name):
    def record_state_decorator(func):
        print(save_state())
        @wraps(func)
        return func()
    return record_state_decorator

#import record

def turn_left():
    motors('LEFT', 'BACKWARD', 100)
    motors('RIGHT', 'FORWARD', 100)

def turn_right():
    motors('LEFT', 'FORWARD', 100)
    motors('RIGHT', 'BACKWARD', 100)

def move_forward():
    motors('LEFT', 'FORWARD', 100)
    motors('RIGHT', 'FORWARD', 100)

def move_backward():
    motors('LEFT', 'BACKWARD', 100)
    motors('RIGHT', 'BACKWARD', 100)

def stop():
    motors('LEFT', 'STOP')
    motors('RIGHT', 'STOP')

@record_state # commands, input and output?
def motors(side, direction, speed=0):
    """Control the motors. The speed is however fixed for now"""
    if side == 'LEFT':
        pin_a = 11
        pin_b = 13
    else: # RIGHT
        pin_a = 16
        pin_b = 15

    # set direction
    if direction == 'FORWARD':
        GPIO.output(pin_a, GPIO.HIGH)
        GPIO.output(pin_b, GPIO.LOW)
    elif direction == 'BACKWARD':
        GPIO.output(pin_a, GPIO.LOW)
        GPIO.output(pin_b, GPIO.HIGH)
    else: # RELEASE
        GPIO.output(pin_a, GPIO.LOW)
        GPIO.output(pin_b, GPIO.LOW) 


def drive_manual():
    """Manually drive the car"""
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
            print('Left Arrow Key pressed')
            turn_left()
        elif event == curses.KEY_RIGHT:
            print('Right Arrow Key pressed')
            turn_right()
        elif event == curses.KEY_UP:
            print('Up Arrow Key pressed')
            move_forward()
        elif event == curses.KEY_DOWN:
            print('Down Arrow Key pressed')
            move_backward()
        elif event == 32:
            print('Space Arrow Key pressed')
            stop()
        else:
            print(event)
            stop()
            break

def drive_ai():
    pass

def setup():
    GPIO.setmode(GPIO.BOARD) ## Use board pin numbering
    GPIO.setup(11, GPIO.OUT)
    GPIO.setup(13, GPIO.OUT)
    GPIO.setup(15, GPIO.OUT)
    GPIO.setup(16, GPIO.OUT)

def exit():
    GPIO.cleanup()

def main():
    setup()
    drive_manual()

if __name__ == '__main__':
    atexit.register(exit)
    main()