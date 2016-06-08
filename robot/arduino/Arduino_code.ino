//#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <FlexiTimer2.h>
#include "main.h"


float setSpeed = 0; // Set speed for "leading" wheel, the other wheel is regulated through value "angle"
float setSpeedLeft = 0;
float setSpeedRight = 0;
float realSpeedL = 0;
float realSpeedR = 0;
float kp = KP;
float ki = KI;
float kd = KD;
float PrevKi = 0;
int angle = 0;
int newSpeedL = 0;
int newSpeedR = 0;
int usdDistance1 = 0;
int usdDistance2 = 0;
int usdDistance3 = 0;
int usdDistance4 = 0;
int usdDistance5 = 0;
//volatile speed_t currentSpeeds;
boolean flagSpeedRegulation = false;
//boolean angleChanged = false;
boolean securityModeChanged = false;
boolean safetyChanged = false;
boolean powerDownModeChanged = false;
boolean usdChanged = false;
boolean security = true;
boolean powerDown = true;
boolean safe = true;
mode_t Mode = INACTIVE_MODE;
state_t State = STOPPING_STATE;


void setup()
{
    Wire.begin();
    while (!Serial)
    {
        // Wait for serial connection
    }

    /**
    Set MD25 mode to Mode 1 for motors speed control (see documentation)
    */
    Wire.beginTransmission(RD01_ADDR);
    Wire.write((byte)RD01_MODE_REG);
    Wire.write((byte)RD01_MODE_1);
    Wire.endTransmission();

    #ifndef MD25_REGULATION_MODE
    /**
    Disable MD25 automatic regulation
    */
    Wire.beginTransmission(RD01_ADDR);
    Wire.write((byte)RD01_CMD_REG);
    Wire.write((byte)RD01_CMD_DIS_SPEED_REG);
    Wire.endTransmission();
    #endif // MD25_REGULATION_MODE

    #ifdef MD25_REGULATION_MODE
    /**
    Enable MD25 automatic regulation
    */
    Wire.beginTransmission(RD01_ADDR);
    Wire.write((byte)RD01_CMD_REG);
    Wire.write((byte)RD01_CMD_EN_SPEED_REG);
    Wire.endTransmission();
    #endif // MD25_REGULATION_MODE

    FlexiTimer2::set(DT, speedRegulation); // call speedRegulation every DT ms
    FlexiTimer2::start();
}

void loop()
{
    #ifndef PID_CONFIG_MODE
    static usd_t usd[5];

    static usd[0].address = SRF08_6_ADDR;   // usd front left
    static usd[1].address = SRF08_4_ADDR;   // usd front right
    static usd[2].address = SRF08_8_ADDR;   // usd back left
    static usd[3].address = SRF08_2_ADDR;   // usd back right
    static usd[4].address = SRF08_0_ADDR;   // usd back center

    if (usd[0].usdRangeInitiated == false)  // start ranging and run rest of the loop without dealying for data collection
    {
        for (int i = 0; i<5; i++)
        {
            getRangeSRF(usd[i].address);
        }
    }
    else if (usd[4].lastTime >= 65) // if 65 ms have passed since ranging command, ranging is done and data can be collected
    {
        for (int i = 0; i<5; i++)
        {
            getRangeSRF(usd[i].address);
        }

        usdDistance1 = usd[0].range;
        usdDistance2 = usd[1].range;
        usdDistance3 = usd[2].range;
        usdDistance4 = usd[3].range;
        usdDistance5 = usd[4].range;
        usdChanged = true;

//        if (security)
//        {
//            testSecurityDistance();
//        }

    }

    /**
    If one arduino parameter or more have been changed, the parameters are updated to the RPi
    */
    if (usdChanged || safetyChanged || powerDownModeChanged)
    {
        sendDataToRPi();
        usdChanged = false;
        safetyChanged = false;
        powerDownModeChanged = false;
    }
    #endif // PID_CONFIG_MODE

    /**
    Receive datas from RPi if data available on Serial (for RPi configuration or control of robot according to control mode)
    */
    #ifdef PID_CONFIG_MODE
    getPIDConfigDataFromRPi();
    #else
    getDataFromRPi();

//    if (!safe && security)
//    {
//        isAreaClean();
//        safe = true;
//    }
//    if (!safe)
//    {
//        setSpeed = 0;
//    }

    #endif // PID_CONFIG_MODE

    /****************FLAGS HANDLING*************/


    /** SpeedRegulation
    For feedback regulation (speedRegulation) of the speed, after the ISR has been called
    */
    if (flagSpeedRegulation)
    {
        speedDifferential();

        /**
        If not using MD25 regulation mode, regulation is needed
        */
        #ifndef MD25_REGULATION_MODE
        errorL = setSpeedLeft - realSpeedL;
        errorR = setSpeedRight - realSpeedR;
        dErrL = (errorL - previousErrorL)/DT;
        dErrR = (errorR - previousErrorR)/DT;
        errIntL += errorL/DT;
        errIntR += errorR/DT;

        // To prevent an accumulation of integral error when speed is to high to be reached
        if (errIntL*ki > MAX_SPEED)
        {
            errIntL = (float)MAX_SPEED/ki;
        }
        if (errIntR*ki > MAX_SPEED)
        {
            errIntR = (float)MAX_SPEED/ki;
        }

        newSpeedL = (int)(kp*errorL + ki*errIntL + kd*dErrL);   // value adapted to register (max is MAX_SPEED), not real value of speed in turns/min
        newSpeedR = (int)(kp*errorL + ki*errIntR + kd*dErrR);
        previousErrorL = errorL;
        previousErrorR = errorR;

        // To prevent outpasssing the maximum/minimum speed
        if(abs(newSpeedL) > MAX_SPEED)
        {
            if(newSpeedL > 0)
            {
                newSpeedL = MAX_SPEED;
            }
            else
            {
                newSpeedL = -MAX_SPEED;
            }
        }
        if(abs(newSpeedR) > MAX_SPEED)
        {
            if(newSpeedR > 0)
            {
                newSpeedR = MAX_SPEED;
            }
            else
            {
                newSpeedR = -MAX_SPEED;
            }
        }
        #endif // MD25_REGULATION_MODE

        /**
        If using MD25 regulation, no regulation made
        */
        #ifdef MD25_REGULATION_MODE
        newSpeedL = (int)(setSpeedLeft/MAX_SPEED_RPM*127);
        newSpeedR = (int)(setSpeedRight/MAX_SPEED_RPM*127);
        #endif // MD25_REGULATION_MODE

        setMotorsSpeed();

        /**
        If in PID configuration mode, the speeds are sent to the RPi for visualisation and evaluation of the PID parameters
        */
        #ifdef PID_CONFIG_MODE
        void sendPIDConfigDataToRPi();
        #endif // PID_CONFIG_MODE
    }
}
