from functools import wraps
from contextlib import contextmanager
import time, json, sys, random
import cv2
from unqlite import UnQLite
import tensorflow as tf
import numpy as np
from ai import detection, preprocessing, deepq
import i2c
from controls import CursesControl, RemoteControlServer
from camera import PiVideoStream
from livestream import LiveStreamClient

db = UnQLite('records.udb')

@contextmanager
def timeit_context(name):
    startTime = time.time()
    yield
    elapsedTime = time.time() - startTime
    print('[{}] finished in {} ms'.format(name, int(elapsedTime * 1000)))


class AutonomousDriver(object):
    """docstring for AutonomousDriver"""
    def __init__(self):
        super(AutonomousDriver, self).__init__()
        self.LAST_OBSERV = []
        self.LAST_ACTION = None

    def start(self):
        session = tf.InteractiveSession()
        size_ = 10
        brain = deepq.MLP([size_,], [10, 10], 
                    [tf.tanh, tf.identity])
        optimizer = tf.train.RMSPropOptimizer(learning_rate = 0.01, decay=0.9)
        self.current_controller = deepq.DiscreteDeepQ(size_, 10, brain, optimizer, session,
                                       discount_rate=0.9, exploration_period=100, max_experience=10000, 
                                       store_every_nth=1, train_every_nth=4, target_network_update_rate=0.1)
        session.run(tf.initialize_all_variables())
        session.run(self.current_controller.target_network_update)
        return self

    def _train(self, reward, new_obs):
        self.current_controller.store(self.LAST_OBSERV, self.LAST_ACTION, reward, new_obs)
        if random.random() < 0.25:
            self.current_controller.training_step()

    def learn(self):
        """Learn on a given dataset"""
        # db.all()
        experiences = json.loads('[' + db['env'][1:].replace('\'', '\"') + ']')
        experiences = filter(lambda x: x['output']['command'] == 'auto_logic_based', experiences)
        
        for i, exp in enumerate(experiences):
            path = 'records/' + exp['img'] + '.jpg'
            img = cv2.imread(path)
            mask = preprocessing.get_mask_color(img, color='red')
            reward = preprocessing.get_reward(mask)
            points = np.array(list(preprocessing.get_path_points(mask)))
            if len(self.LAST_OBSERV) > 1:
                self._train(reward, points)
                print('learning', i, '/', len(experiences))
            self.LAST_OBSERV = points
            self.LAST_ACTION = 4 if experiences[0]['output']['turn'] == -5 else 5

    def load():
        """Load a model"""
        pass

    def action(self, command, img):
        mask = preprocessing.get_mask_color(img, color='red')
        reward = preprocessing.get_reward(mask)
        points = np.array(list(preprocessing.get_path_points(mask)))

        action = self.current_controller.action(points)

        if len(self.LAST_OBSERV) > 1:
            self._train(reward, points)
        self.LAST_OBSERV = points
        self.LAST_ACTION = action
      
        if action < 5:
            action = (9 - action)
            action = - int(action*1.5 - 5)
        else:
            action = int(action*1.5 - 5)
        return 5, action # speed, turn

class LogicBasedDriver(object):
    def __init__(self):
        super(LogicBasedDriver, self).__init__()

    def action(self, command, img, *args, **kwargs):
        speed, turn = 0, 0
        with timeit_context('Detections'):
            detections = detection.detections(img, detect=['stop'])
        mask = preprocessing.get_mask_color(img, color='red')
        mask = preprocessing.get_size_mask(mask, 0.5, 1) # get the bottom half 

        cx, cy, surface_size = preprocessing.get_mask_info(mask)
        screen_size = mask.shape[1]/2
        speed = 30
        K = 0.6
        turn = K * speed * (cx - screen_size)/screen_size
        """
        if 'stop' in detections:
            i2c.move_backward()
        """
        return speed, int(turn)

class HumanDriver(object):
    def __init__(self):
        super(HumanDriver, self).__init__()

    def action(self, command, *args):
        speed, turn = 0, 0

        if command == 'left':
            turn = -20
        elif command == 'right':
            turn = 20
        elif command == 'up':
            speed = 20
        elif command == 'down':
            speed = -20

        return speed, turn

def get_sensors(func):
    @wraps(func)
    def wrapper(self, *args, **kwargs):
        with timeit_context('Image'):
            img = self.camera.read()
        with timeit_context('Sensors'):
            sensors = i2c.get_sonars_input()
        return func(self, img=img, sensors=sensors, *args, **kwargs)
    return wrapper

def save_state(self, **states):
    # write the image
    filename = str(states['time']).replace('.', '_')
    # states['output'] = list(states['output'])
    
    with timeit_context('Img Write'):
        cv2.imwrite('./records/' + filename + '.jpg', states['img'])
    
    states['img'] = filename
    with timeit_context('Data Write'):
        # if len(db.get('env', '')) > 0:
        db.append('env', ',')
        db.append('env', states)
        if random.random() < 0.05:
            db.commit()

def record(func):
    @wraps(func)
    def wrapper(self, *args, **kwargs):
        output = func(self, *args, **kwargs)
        with timeit_context('Save states'):
            save_state(self, output=output, time=time.time(), *args, **kwargs)
        return output
    return wrapper


class Car(object):
    """docstring for Car"""
    def __init__(self, control):
        super(Car, self).__init__()
        self.live_stream = None
        self.camera = PiVideoStream(resolution=(320, 240), framerate=16)
        self.control = control
        self.driver = HumanDriver() 
    
    def exit(self):
        self.camera.stop()
        self.control.stop()
        if self.live_stream:
            self.live_stream.stop()
        print('exit')

    def start(self):
        i2c.setup(mode_motors=3)
        self.control.start()
        self.camera.start()

    @get_sensors
    @record # inputs (camera + sensor) and output
    def drive(self, img, sensors):
        if self.live_stream:
            self.live_stream.send(frame=img, sensors=sensors)

        command = self.control.read()

        if command == 'quit':
            self.exit()
            sys.exit(1)
        elif command == 'stream':
            try:
                if not self.live_stream:
                    self.live_stream = LiveStreamClient().start()
            except Exception as e:
                print('live_stream', e)
        elif command == 'stop':
            i2c.stop()
        
        if command == 'auto_logic_based':
            if not isinstance(self.driver, LogicBasedDriver):
                self.driver = LogicBasedDriver()
        elif command == 'auto_neural_network':
            if not isinstance(self.driver, AutonomousDriver):
                ai = AutonomousDriver().start()
                ai.learn()
                self.driver = ai # utonomousDriver().start()
        else:
            self.driver = HumanDriver()
            pass # human control

        speed, turn = self.driver.action(command, img)

        i2c.set_speed(speed)
        i2c.set_turn(turn)

        # CONSTRAINTS
        for sonar in i2c.SONARS:
            if sonar == i2c.I2C_SONAR_2:
                continue
            if sensors.get(str(sonar), [30])[0] < 20:
                i2c.stop()
        
        return {'command': command, 'speed': speed, 'turn': turn}


def main():
    if len(sys.argv) > 1 and sys.argv[1] == '--remote':
        control = RemoteControl()
    else:
        control = CursesControl()
    
    car = Car(control=control)
    car.start()
    while True:
        car.drive()


if __name__ == '__main__':
    main()