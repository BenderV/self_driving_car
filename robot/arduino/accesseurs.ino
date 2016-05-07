#include "main.h"


/**
  Gives the distance of closest object as seen by the ultrasonic module at the adress 'adresse'

  @param adresse    SRF08 module address
  @return range     return the distance in centimeters
*/
int getRangeSRF(int adresse)
{
    int range = 0;

    Wire.beginTransmission(adresse);
    Wire.write((byte)SRF08_CMD_REG);
    Wire.write(0x51); //Pour avoir des cm
    Wire.endTransmission();

    delay(70); // On attend que les capteurs prennent des valeurs (tres important)

    Wire.beginTransmission(adresse);
    Wire.write(SRF08_RANGE_REG);
    Wire.endTransmission();

    Wire.requestFrom(adresse, 2);               // Request 2 bytes from SRF module
    while(Wire.available() < 2);                    // Wait for data to arrive
    byte highByte = Wire.read();                 // Get high byte
    byte lowByte = Wire.read();                 // Get low byte

    range = (highByte << 8) + lowByte;              // Put them together

    return(range);                                  // Returns Range
}

int getLightSRF(int adresse)                                    // Function to get light reading
{
    Wire.beginTransmission(adresse);
    Wire.write(SRF08_LIGHT_REG);                           // Call register to get light reading
    Wire.endTransmission();

    Wire.requestFrom(adresse, 1);               // Request 1 byte
    while(Wire.available() < 0);                    // While byte available
    int lightRead = Wire.read();                 // Get light reading

    return(lightRead);                              // Returns lightRead

}

int getSoftSRF(int adresse)                                     // Function to get software revision
{

    Wire.beginTransmission(adresse);             // Begin communication with the SRF module
    Wire.write((byte)SRF08_CMD_REG);                             // Sends the command bit, when this bit is read it returns the software revision
    Wire.endTransmission();

    Wire.requestFrom(adresse, 1);               // Request 1 byte
    while(Wire.available() < 0);                    // While byte available
    int software = Wire.read();                 // Get byte

    return(software);

}

double getVoltageRD01()
{
    //Avoir le voltage de la batterie

    Wire.beginTransmission(RD01_ADDR);
    Wire.write((byte)RD01_VOLTAGE_REG);
    Wire.endTransmission();

    Wire.requestFrom(adresse, 1);
    while(Wire.available() < 0);
    int voltage = Wire.read();

    return voltage/10.0;
}

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
    Wire.write((byte)RD01_RESET_ENCODERS);
    Wire.endTransmission();
}

int isAreaClean()
{
    if (getRangeSRF(SRF08_4_ADDR) < SRF08_DISTANCE
            ||  getRangeSRF(SRF08_6_ADDR) < SRF08_DISTANCE)
        return ARRIERE;
    if (getRangeSRF(SRF08_2_ADDR) < SRF08_DISTANCE
            ||  getRangeSRF(SRF08_8_ADDR) < SRF08_DISTANCE
            ||  getRangeSRF(SRF08_0_ADDR) > 35 )//le capteur dans le vide
        return AVANT;
    return OK;
}

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
    float currentSpeeds.leftWheel = (float)(counts/COUNTS_PER_TURN)/(float)(timeDiff1/60000) // in turns/min
    float currentSpeeds.rightWheel = (float)(counts/COUNTS_PER_TURN)/(float)(timeDiff2/60000)
}
