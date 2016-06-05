#include "main.h"


void avancer(int angle, int vitesse)
{

}

void reculer(int angle, int vitesse)
{

}

int differentielVitesse(int vitesse1)
{

}

/**
    Function called when FlexiTimer2 is making an interrupt.
    /!\ Since it's an ISR, avoid doing too much things here.
        That's why there is the flag "flagSpeedRegulation", and the rest is done in loop()
    */
void speedRegulation()
{
    flagSpeedRegulation = true;
    getCurrentSpeeds();
}
