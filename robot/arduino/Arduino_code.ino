<<<<<<< HEAD
//#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <FlexiTimer2.h>
#include "main.h"


float setSpeed = 0;
float kp = KP;
float ki = KI;
float kd = KD;
volatile speed_t currentSpeeds;
boolean flagAsservissement = false;
mode_t Mode = INACTIVE_MODE;
state_t State = STOPPING_STATE;


void setup()
{
    Wire.begin();
    while (!Serial)
    {
        // Wait for serial connection
    }
    FlexiTimer2::set(DT, asservissement); // tous les "dt" ms le programme calcule la frequence et l'affiche
    FlexiTimer2::start();
}

void loop()
{
    #ifdef PID_CONFIG_MODE
    #
    #endif // PID_CONFIG_MODE
    updateDataFromRPi();

    /****************FLAGS HANDLING*************/


    /** Asservissement
    For feedback regulation (asservissement) of the speed, after the ISR has been called
    */
    if (flagAsservissement)
    {
        error1 = SetSpeed1 - currentSpeeds.leftWheel;
        error2 = SetSpeed2 - currentSpeeds.rightWheel;
        dErr1 = (error1 - previousError1)/DT;
        dErr1 = (error2 - previousError2)/DT;
        errInt1 += error1/DT;
        errInt2 += error2/DT;
        newSpeed1 = (int)(kp*error1 + ki*errInt1 + kd*dErr1);   // value adapted to register, not real value of speed in turns/min
        newSpeed2 = (int)(kp*error1 + ki*errInt1 + kd*dErr1);
        previousError1 = error1;
        previousError2 = error2;
        if(abs(newSpeed1) > VITESSE_MAX)
        {
            if(newSpeed1 > 0)
            {
                newSpeed1 = VITESSE_MAX;
            }
            else
            {
                newSpeed1 = -VITESSE_MAX;
            }
        }
        if(abs(newSpeed2) > VITESSE_MAX)
        {
            if(newSpeed2 > 0)
            {
                newSpeed2 = VITESSE_MAX;
            }
            else
            {
                newSpeed2 = -VITESSE_MAX;
            }
        }
#ifdef PID_CONFIG_MODE
        void sendPIDConfigDataToRPi();
#endif // PID_CONFIG_MODE
    }

//    // Lecture du port s¨¦rie dans le cas o¨´ la consigne, kp, ki ou kd auraient chang¨¦
//    if(Serial.available())
//    {
//        valeurAChanger = Serial.parseInt();
//        switch (valeurAChanger)
//        {
//            case 1:
//                consigne = Serial.parseFloat();
//                break;
//            case 2:
//                kp = Serial.parseFloat());
//                break;
//            case 3:
//                ki = Serial.parseFloat());
//                break;
//            case 4:
//                kd = Serial.parseFloat());
//                break;
//        }
//    }
}

//void calibrage() {
//  FlexiTimer2::stop();
//  temps_actuel = millis();
//  Serial.print(
//}
=======
//#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <FlexiTimer2.h>
#include "main.h"


int setSpeed = 0;
float kp = KP;
float ki = KI;
float kd = KD;
volatile speed_t currentSpeeds;
boolean flagAsservissement = false;
mode_t Mode = INACTIVE_MODE;
state_t State = STOPPING_STATE;


void setup()
{
    Wire.begin();
    while (!Serial)
    {
        // Wait for serial connection
    }
    FlexiTimer2::set(DT, asservissement); // tous les "dt" ms le programme calcule la frequence et l'affiche
    FlexiTimer2::start();
}

void loop()
{
    updateDataFromRPi();

    /****************FLAGS HANDLING*************/


    /** Asservissement
    For feedback regulation (asservissement) of the speed, after the ISR has been called
    */
    if (flagAsservissement)
    {
        error1 = SetSpeed1 - currentSpeeds.leftWheel;
        error2 = SetSpeed2 - currentSpeeds.rightWheel;
        dErr1 = (error1 - previousError1)/DT;
        dErr1 = (error2 - previousError2)/DT;
        errInt1 += error1/DT;
        errInt2 += error2/DT;
        newSpeed1 = (int)(kp*error1 + ki*errInt1 + kd*dErr1);
        newSpeed2 = (int)(kp*error1 + ki*errInt1 + kd*dErr1);
        previousError1 = error1;
        previousError2 = error2;
        if(abs(newSpeed1) > VITESSE_MAX)
        {
            if(newSpeed1 > 0)
            {
                newSpeed1 = VITESSE_MAX;
            }
            else
            {
                newSpeed1 = -VITESSE_MAX;
            }
        }
        if(abs(newSpeed2) > VITESSE_MAX)
        {
            if(newSpeed2 > 0)
            {
                newSpeed2 = VITESSE_MAX;
            }
            else
            {
                newSpeed2 = -VITESSE_MAX;
            }
        }
#ifdef PID_CONFIG_MODE
        void sendSpeedToRPi(currentSpeeds);
#endif // PID_CONFIG_MODE
    }

//    // Lecture du port s¨¦rie dans le cas o¨´ la consigne, kp, ki ou kd auraient chang¨¦
//    if(Serial.available())
//    {
//        valeurAChanger = Serial.parseInt();
//        switch (valeurAChanger)
//        {
//            case 1:
//                consigne = Serial.parseFloat();
//                break;
//            case 2:
//                kp = Serial.parseFloat());
//                break;
//            case 3:
//                ki = Serial.parseFloat());
//                break;
//            case 4:
//                kd = Serial.parseFloat());
//                break;
//        }
//    }
}

//void calibrage() {
//  FlexiTimer2::stop();
//  temps_actuel = millis();
//  Serial.print(
//}
>>>>>>> ae5b5afd93b7b3d030e80e9e672bd130ac63944d
