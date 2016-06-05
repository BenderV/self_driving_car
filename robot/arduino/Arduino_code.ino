//#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <FlexiTimer2.h>
#include "main.h"


float setSpeed = 0; // Set speed for "leading" wheel, the other wheel is regulated through value "angle"
float kp = KP;
float ki = KI;
float kd = KD;
float PrevKi = 0;
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
    Receive datas from RPi if data available on Serial (for RPi configuration or control of robot according to mode)
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
        error1 = SetSpeed1 - currentSpeeds.leftWheel;
        error2 = SetSpeed2 - currentSpeeds.rightWheel;
        dErr1 = (error1 - previousError1)/DT;
        dErr1 = (error2 - previousError2)/DT;
        errInt1 += error1/DT;
        errInt2 += error2/DT;
        // To prevent an accumulation of integral error when speed is to high to be reached
        if (errInt1*ki > MAX_SPEED)
        {
            errInt1 = (float)MAX_SPEED/ki;
        }
        if (errInt2*ki > MAX_SPEED)
        {
            errInt2 = (float)MAX_SPEED/ki;
        }
        newSpeed1 = (int)(kp*error1 + ki*errInt1 + kd*dErr1);   // value adapted to register (max is MAX_SPEED), not real value of speed in turns/min
        newSpeed2 = (int)(kp*error1 + ki*errInt1 + kd*dErr1);
        previousError1 = error1;
        previousError2 = error2;
        // To prevent outpasssing the maximum/minimum speed
        if(abs(newSpeed1) > MAX_SPEED)
        {
            if(newSpeed1 > 0)
            {
                newSpeed1 = MAX_SPEED;
            }
            else
            {
                newSpeed1 = -MAX_SPEED;
            }
        }
        if(abs(newSpeed2) > MAX_SPEED)
        {
            if(newSpeed2 > 0)
            {
                newSpeed2 = MAX_SPEED;
            }
            else
            {
                newSpeed2 = -MAX_SPEED;
            }
        }
        // If in PID configuration mode, the speeds are sent to the RPi for visualisation and evaluation of the PID parameters
#ifdef PID_CONFIG_MODE
        void sendPIDConfigDataToRPi();
#endif // PID_CONFIG_MODE
    }
}
