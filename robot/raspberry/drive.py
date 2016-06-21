from functools import wraps
from contextlib import contextmanager
import time, json, sys
import cv2
import controls
import motors
import detection
import i2c
import camera
import livestream
import atexit

@contextmanager
def timeit_context(name):
    startTime = time.time()
    yield
    elapsedTime = time.time() - startTime
    print('[{}] finished in {} ms'.format(name, int(elapsedTime * 1000)))

def save_state(**states):
    # write the image
    filename = str(states['time']).replace('.', '_')
    states['output'] = list(states['output'])
    cv2.imwrite('./records/' + filename + '.jpg', states['img'])
    states['img'] = filename
    with open('record.json') as f:
        data = json.load(f)

    data.append(states)

    with open('record.json', 'w') as f:
        json.dump(data, f)

    print(states.get('output', 'n'))
    pass 

def get_sensors(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        with timeit_context('Image'):  
            img = camera.vs.read()
        with timeit_context('Sensors'):
            sensors = i2c.get_sonars_input()
        return func(img=img, sensors=sensors, *args, **kwargs)
    return wrapper

def record(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        output = func(*args, **kwargs)
        save_state(output=output, time=time.time(), *args, **kwargs)
        return output
    return wrapper

def live_stream(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        if 'stream' in kwargs:
            kwargs['stream'].send(frame=kwargs['img'])
            del kwargs['stream']
        output = func(*args, **kwargs)
        return output
    return wrapper


def command_parse(command):
    if command == 'left':
        motors.turn_left()
    elif command == 'right':
        motors.turn_right()
    elif command == 'up':
        motors.move_forward()
    elif command == 'down':
        motors.move_backward()
    elif command == 'stop':
        motors.stop()
    elif command == 'record':
        pass # video.start()
    elif command == 'stop_record':
        pass # video.end()
    elif command == 'quit':
        return 'quit'
    else:
        print('stop motors')
        motors.stop()
        return -1

@get_sensors
@live_stream
@record # inputs (camera + sensor) and output
def drive(img, sensors):
    with timeit_context('Detections'):
        detections = detection.detections(img, detect=['stop'])
    #if 'stop' in detections:
    #    motors.move_backward()
    control = command_parse('stop') # controls.ct.read())
    if control == 'quit':
        controls.ct.stop()
        camera.vs.stop()
        print('exit')
        sys.exit(1)

    
    if sensors.get(i2c.I2C_SONAR_2, [0])[0] < 25:
        motors.stop()
    if sensors.get(i2c.I2C_SONAR_4, [0])[0] < 25:
        motors.stop()
    
    return detections


def main():
    motors.setup()
    # controls.ct.start()
    # stream = livestream.LiveStreamClient().start()
    while True:
        drive()


if __name__ == '__main__':
    main()