//#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <FlexiTimer2.h>
#include "main.h"


float setSpeed = 0; // Set speed for "leading" wheel, the other wheel is regulated through value "angle"
float setSpeedLeft = 0;
float setSpeedRight = 0;
float kp = KP;
float ki = KI;
float kd = KD;
float PrevKi = 0;
int angle = 0;
int newSpeedL = 0;
int newSpeedR = 0;
volatile speed_t currentSpeeds;
boolean flagSpeedRegulation = false;
boolean angleChanged = false;
boolean securityModeChanged = false;
boolean powerDownModeChanged = false;
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

    FlexiTimer2::set(DT, speedRegulation); // tous les "dt" ms le programme calcule la frequence et l'affiche
    FlexiTimer2::start();
}

void loop()
{
    #ifndef PID_CONFIG_MODE
    /**
    If not in PID configuration mode and if one arduino parameter or more have been changed, the parameters are updated to the RPi
    */
    if (angleChanged || securityModeChanged || powerDownModeChanged)
    {
        sendDataToRPi();
    }
    #endif // PID_CONFIG_MODE

    /**
    Receive datas from RPi if data available on Serial (for RPi configuration or control of robot according to control mode)
    */
    #ifdef PID_CONFIG_MODE
    getPIDConfigDataFromRPi();
    #else
    getDataFromRPi();
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
        errorL = setSpeedLeft - currentSpeeds.leftWheel;
        errorR = setSpeedRight - currentSpeeds.rightWheel;
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
