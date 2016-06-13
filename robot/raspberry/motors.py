import curses
import atexit
import RPi.GPIO as GPIO
import time
import picamera
import io
import datetime, time
from functools import wraps
import smbus

DEVICE_BUS = 1
bus = smbus.SMBus(DEVICE_BUS)

I2C_MOTOR   = 0x58
I2C_SONAR_0 = 0x70
I2C_SONAR_2 = 0x71
I2C_SONAR_4 = 0x72
I2C_SONAR_6 = 0x73
I2C_SONAR_8 = 0x74


def save_state():
    state = {}
    # pins = [11, 13, 15, 16]
    # state['pins'] = [GPIO.input(i) for i in pins]
    state['time'] = time.time()
    return state

def record_state(tag_name):
    def record_state_decorator(func):
        #print(save_state())
        #@wraps(func)
        #return func()
	pass
    return record_state_decorator

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

# @record_state # commands, input and output?
def motors(side, direction, speed=0):
    """Control the motors. The speed is however fixed for now"""
    if side == 'LEFT':
        register_address = 0
    else: # RIGHT
        register_address = 1

    # set direction
    if direction == 'FORWARD':
        bus.write_byte_data(I2C_MOTOR, register_address, -speed)
    elif direction == 'BACKWARD':
        bus.write_byte_data(I2C_MOTOR, register_address, speed)
    else: # RELEASE
        bus.write_byte_data(I2C_MOTOR, register_address, 0)


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

def setup(mode_motors=1): # 1 (or 3
    bus.write_byte_data(I2C_MOTOR, 0x00, 0x01)
    pass

def exit():
    pass 

def main():
    setup()
    drive_manual()

if __name__ == '__main__':
    atexit.register(exit)
    main()
