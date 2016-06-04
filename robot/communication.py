import serial

PID_CONFIG_MODE = True

setSpeed = 0
kp = 0.1
ki = 0.1
kd = 0


port = serial.Serial("/dev/ttyAMA0", baudrate=115200, timeout=3.0)


if PID_CONFIG_MODE == True:
    def sendData(setSpeed, kp, ki , kd):
        stringToSend = str(setSpeed) + ',' + str(kp) + ',' + str(ki) + ',' + str(kd) + '\n'
        stringToSend.encode('ascii')
        port.write(stringToSend.encode('ascii'))

    def receiveData():
        while lastCharacter != ord('\n'):
            lastCaracter = port.read();
            data += lastCharacter;
        listOfNumbers = data.split(',')
        realSpeedLeft = float(listOfNumbers[0])
        realSpeedRight = float(listOfNumbers[1])
        return realSpeedLeft, realSpeedRight

if PID_CONFIG_MODE == False:
    def sendData(speed, angle, security, powerDown):
        stringToSend = str(speed) + ',' + str(angle) + ',' + str(security) + ',' + str(powerDown) + '\n'
        stringToSend.encode('ascii')
        port.write(stringToSend.encode('ascii'))

    def receiveData(angle, security, usdChanged, usd1, usd2, usd3, usd4, usd5):     """" usd# = ultrasound device nbr # distance in centimeters """
        while lastCharacter != ord('\n'):
            lastCaracter = port.read();
            data += lastCharacter;
        listOfNumbers = data.split(',')
        angle = float(listOfNumbers[0])
        security = float(listOfNumbers[1])
        usdChanged = float(listOfNumbers[2])
        us1 = float(listOfNumbers[3])
        us2 = float(listOfNumbers[4])
        us3 = float(listOfNumbers[5])
        us4 = float(listOfNumbers[6])
        us5 = float(listOfNumbers[7])
        return realSpeedLeft, realSpeedRight



