#include "main.h"


void speedDifferential()
{
    if (angle >= 0)
    {
        setSpeedLeft = setSpeed;
        setSpeedRight = angle/128*setSpeed;
    }
    else
    {
        setSpeedRight = setSpeed;
        setSpeedLeft = angle/128*setSpeed;
    }
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
