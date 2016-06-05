import serial

PID_CONFIG_MODE = True

# "/dev/ttyAMA0" to change with our serial port communicating with the Arduino
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
        stringToSend = str(speed) + ',' + str(angle) + ',' + str(int(security)) + ',' + str(int(powerDown)) + '\n'
        stringToSend.encode('ascii')
        port.write(stringToSend.encode('ascii'))

    def receiveData():     # usd# = ultrasound device nbr # distance in centimeters
        while lastCharacter != ord('\n'):
            lastCaracter = port.read();
            data += lastCharacter;
        listOfNumbers = data.split(',')
        if int(listOfNumbers[0]) == True:
            security = True
        else:
            security = False
        if int(listOfNumbers[1]) == True:
            powerDown = True
        else:
            powerDown = False
        if int(listOfNumbers[2]) == True:
            usdChanged = True
        else:
            usdChanged = False
        us1 = float(listOfNumbers[3])
        us2 = float(listOfNumbers[4])
        us3 = float(listOfNumbers[5])
        us4 = float(listOfNumbers[6])
        us5 = float(listOfNumbers[7])
        return security, usdChanged, usd1, usd2, usd3, usd4, usd5
