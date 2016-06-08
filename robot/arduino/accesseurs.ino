#include "main.h"


/**
  Gives the distance of closest object as seen by the ultrasonic module at the adress 'adresse'

  @param adresse    SRF08 module address
  @return range     return the distance in centimeters
*/
usd_t getRangeSRF(int address, usd_t usd)
{
    if (usd.usdRangeInitiated == false)
    {
        Wire.beginTransmission(usd.address);
        Wire.write((byte)SRF08_CMD_REG);
        Wire.write(SRF08_CMD_CM); // to get centimeters
        Wire.endTransmission();

        usd.lastTime = millis();
        usd.usdRangeInitiated = true;
    }
    else
    {
        Wire.beginTransmission(usd.address);
        Wire.write((byte)SRF08_ECHO1_HSB_REG);
        Wire.endTransmission();

        Wire.requestFrom(usd.address, 2);               // Request 2 bytes from SRF module
        while(Wire.available() < 2);                    // Wait for data to arrive
        byte highByte = Wire.read();                 // Get high byte
        byte lowByte = Wire.read();                 // Get low byte

        usd.range = (highByte << 8) + lowByte;              // Put them together
        usd.usdRangeInitiated = false;
    }

    return(usd);                                  // Returns Range
}
//
//int getLightSRF(int adress)                                    // Function to get light reading
//{
//    Wire.beginTransmission(adresse);
//    Wire.write(SRF08_LIGHT_REG);                           // Call register to get light reading
//    Wire.endTransmission();
//
//    Wire.requestFrom(adresse, 1);               // Request 1 byte
//    while(Wire.available() < 0);                    // While byte available
//    int lightRead = Wire.read();                 // Get light reading
//
//    return(lightRead);                              // Returns lightRead
//
//}
//
//int getSoftSRF(adresse)                                     // Function to get software revision
//{
//
//    Wire.beginTransmission(adresse);             // Begin communication with the SRF module
//    Wire.write((byte)SRF08_CMD_REG);                             // Sends the command bit, when this bit is read it returns the software revision
//    Wire.endTransmission();
//
//    Wire.requestFrom(adresse, 1);               // Request 1 byte
//    while(Wire.available() < 0);                    // While byte available
//    int software = Wire.read();                 // Get byte
//
//    return(software);
//
//}
//
//double getVoltageRD01()
//{
//    //Avoir le voltage de la batterie
//
//    Wire.beginTransmission(RD01_ADDR);
//    Wire.write((byte)RD01_VOLTAGE_REG);
//    Wire.endTransmission();
//
//    Wire.requestFrom(adresse, 1);
//    while(Wire.available() < 0);
//    int voltage = Wire.read();
//
//    return voltage/10.0;
//}

long getEncoder1Counts()
{
    Wire.beginTransmission(RD01_ADDR);
    Wire.write((byte)RD01_ENCODER1_REG);
    Wire.endTransmission();
    Wire.requestFrom(RD01_ADDR, 4);
    while (Wire.available() < 4);
    long counts = Wire.read();
    counts <<= 8;
    counts += Wire.read();
    counts <<= 8;
    counts += Wire.read();
    counts <<= 8;
    counts  +=Wire.read();
    return counts;
}

long getEncoder2Counts()
{
    Wire.beginTransmission(RD01_ADDR);
    Wire.write((byte)RD01_ENCODER2_REG);
    Wire.endTransmission();
    Wire.requestFrom(RD01_ADDR, 4);
    while (Wire.available() < 4);
    long counts = Wire.read();
    counts <<= 8;
    counts += Wire.read();
    counts <<= 8;
    counts += Wire.read();
    counts <<= 8;
    counts  +=Wire.read();
    return counts;
}

void zeroEncoders()
{
    Wire.beginTransmission(RD01_ADDR);
    Wire.write((byte)RD01_CMD_REG);
    Wire.write((byte)RD01_CMD_RESET_ENCODERS);
    Wire.endTransmission();
}

void setMotorsSpeed()
{
    Wire.beginTransmission(RD01_ADDR);
    Wire.write((byte)RD01_RIGHT_WHEEL_REG);
    Wire.write((byte)newSpeedR);
    Wire.endTransmission();

    Wire.beginTransmission(RD01_ADDR);
    Wire.write((byte)RD01_LEFT_WHEEL_REG);
    Wire.write((byte)newSpeedL);
    Wire.endTransmission();
}

//int isAreaClean()
//{
//    if (getRangeSRF(SRF08_4_ADDR) < SRF08_DISTANCE
//            ||  getRangeSRF(SRF08_6_ADDR) < SRF08_DISTANCE)
//        return ARRIERE;
//    if (getRangeSRF(SRF08_2_ADDR) < SRF08_DISTANCE
//            ||  getRangeSRF(SRF08_8_ADDR) < SRF08_DISTANCE
//            ||  getRangeSRF(SRF08_0_ADDR) > 35 )//le capteur dans le vide
//        return AVANT;
//    return OK;
//}

/**
    Gives the actual speed of the wheels in turns/min using coders counts.
    /!\ Each time this function is called, encoders are put to zero (=> if called too often may affect precision)
*/
void getCurrentSpeeds()
{
    long counts1 = getEncoder1Counts();
    long counts2 = getEncoder2Counts();
    long currentTime = millis();
    static long previousTime1 = millis();
    static long previousTime2 = millis();
    zeroEncoders();
    long timeDiff1 = previousTime1 - currentTime;
    long timeDiff2 = previousTime2 - currentTime;
    previousTime1 = currentTime;
    previousTime2 = currentTime;
    realSpeedR = (counts1/COUNTS_PER_TURN)/(float)(timeDiff1/60000); // in turns/min
    realSpeedL = (counts2/COUNTS_PER_TURN)/(float)(timeDiff2/60000);
}


void getPIDConfigDataFromRPi()
{
    int lastIndexOfComma = 0;

    if (Serial.available() > 0)
    {
        String dataFromSerial = Serial.readStringUntil('\n');   // Get the full string from serial
        dataFromSerial.trim();  // Supress all the white spaces (just in case)

        String bufferStringOne = dataFromSerial.substring(lastIndexOfComma, dataFromSerial.indexOf(',', lastIndexOfComma));   // Take the first value separed from the others by a comma
        lastIndexOfComma = dataFromSerial.indexOf(',', lastIndexOfComma) + 1; // Starting the next reading from the character after the comma
        String bufferStringTwo = dataFromSerial.substring(lastIndexOfComma, dataFromSerial.indexOf(',', lastIndexOfComma));
        lastIndexOfComma = dataFromSerial.indexOf(',', lastIndexOfComma) + 1;
        String bufferStringThree = dataFromSerial.substring(lastIndexOfComma, dataFromSerial.indexOf(',', lastIndexOfComma));
        lastIndexOfComma = dataFromSerial.indexOf(',', lastIndexOfComma) + 1;
        String bufferStringFour = dataFromSerial.substring(lastIndexOfComma, dataFromSerial.indexOf(',', lastIndexOfComma));
        lastIndexOfComma = dataFromSerial.indexOf(',', lastIndexOfComma) + 1;

        setSpeed = bufferStringOne.toFloat();
        kp = bufferStringTwo.toFloat();
        ki = bufferStringThree.toFloat();
        kd = bufferStringFour.toFloat();
    }
}

void sendPIDConfigDataToRPi()
{
    Serial.print(realSpeedL, 2);
    Serial.print(',');
    Serial.println(realSpeedR, 2);
}

void getDataFromRPi()
{
    int lastIndexOfComma = 0;

    if (Serial.available() > 0)
    {
        String dataFromSerial = Serial.readStringUntil('\n');   // Get the full string from serial
        dataFromSerial.trim();  // Supress all the white spaces (just in case)

        String bufferStringOne = dataFromSerial.substring(lastIndexOfComma, dataFromSerial.indexOf(',', lastIndexOfComma));   // Take the first value separed from the others by a comma
        lastIndexOfComma = dataFromSerial.indexOf(',', lastIndexOfComma) + 1; // Starting the next reading from the character after the comma
        String bufferStringTwo = dataFromSerial.substring(lastIndexOfComma, dataFromSerial.indexOf(',', lastIndexOfComma));
        lastIndexOfComma = dataFromSerial.indexOf(',', lastIndexOfComma) + 1;
        String bufferStringThree = dataFromSerial.substring(lastIndexOfComma, dataFromSerial.indexOf(',', lastIndexOfComma));
        lastIndexOfComma = dataFromSerial.indexOf(',', lastIndexOfComma) + 1;
        String bufferStringFour = dataFromSerial.substring(lastIndexOfComma, dataFromSerial.indexOf(',', lastIndexOfComma));
        lastIndexOfComma = dataFromSerial.indexOf(',', lastIndexOfComma) + 1;

        setSpeed = bufferStringOne.toFloat();
        angle = bufferStringTwo.toInt();
        security = bufferStringThree.equals("true");
        powerDown = bufferStringFour.equals("true");
    }
}

void sendDataToRPi()
{
    Serial.print(int(safe));
    Serial.print(',');
    Serial.println(int(powerDown));
    Serial.print(',');
    Serial.println(int(usdChanged));
    Serial.print(',');
    Serial.println(usdDistance1);
    Serial.print(',');
    Serial.println(usdDistance2);
    Serial.print(',');
    Serial.println(usdDistance3);
    Serial.print(',');
    Serial.println(usdDistance4);
    Serial.print(',');
    Serial.println(usdDistance5);
}

//void testSecurityDistance()
//{
//    float linearSpeed = abs(setSpeed)*5/60 // speed of wheel converted in linear speed in cm/s (wheel diameter = 10 cm)
//
//    float securityDistance = linearSpeed*REACTION_DELAY + K_SECURITY*pow(linearSpeed,3);
//
//    if (setSpeed >=0)
//    {
//        if (usdDistance1 <= securityDistance || usdDistance2 <= securityDistance)
//        {
//            safe = false;
//            safetyChanged = true;
//        }
//    }
//    if (setSpeed < 0)
//    {
//        if (usdDistance3 <= securityDistance || usdDistance4 <= securityDistance || usdDistance5 <= securityDistance)
//        {
//            safe = false;
//            safetyChanged = true;
//        }
//    }
//}
