import smbus

DEVICE_BUS = 1
bus = smbus.SMBus(DEVICE_BUS)

I2C_MOTORS  = 0x58 
I2C_SONAR_0 = 0x70 # 112
I2C_SONAR_2 = 0x71 # 113
I2C_SONAR_4 = 0x72 # 114
I2C_SONAR_6 = 0x73 # 115
I2C_SONAR_8 = 0x74 # 116
SONARS = [I2C_SONAR_0, I2C_SONAR_2, I2C_SONAR_4, I2C_SONAR_6, I2C_SONAR_8]

def get_sonars_input(mode=0):
    method = 0x51
    if mode == 1: # ANN mode
        method = 0x54
    sonars_input = {}
    for sonar_addr in SONARS:
        while True:
            try:
                if mode == 1:
                    sonar_input = [bus.read_byte_data(sonar_addr, i) for i in range(4, 36)] # 15 ms
                else:
                    sonar_input = []
                    for i in range(2, 36, 2):
                        high_byte = bus.read_byte_data(sonar_addr, i)
                        low_byte  = bus.read_byte_data(sonar_addr, i+1)
                        res = high_byte * 256 + low_byte
                        sonar_input.append(res)
                        if res == 0:
                            break
                break
            except Exception as e:
                continue
        sonars_input[sonar_addr] = sonar_input
        bus.write_byte_data(sonar_addr, 0, 0x51) # ANN mode
    return sonars_input


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

def setup(mode_motors=1): # 1 or 3
    # bus.write_byte_data(I2C_MOTOR, 14, 10) # Acceleration rate
    bus.write_byte_data(I2C_MOTOR, 15, 1) # mode

def motors(side, direction, speed=0):
    if side == 'LEFT':
        register_address = 0
    else: # RIGHT
        register_address = 1

    speed = speed / 3
    # set direction
    try:
      if direction == 'FORWARD':
          bus.write_byte_data(I2C_MOTOR, register_address, speed)
      elif direction == 'BACKWARD':
          bus.write_byte_data(I2C_MOTOR, register_address, -speed)
      else: # RELEASE
          bus.write_byte_data(I2C_MOTOR, register_address, 0)
    except IOError as e:
        print(e)