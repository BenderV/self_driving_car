<<<<<<< HEAD
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
        That's why there is the flag "flagAsservissement", and the rest is done in loop()
    */
void asservissement()
{
    flagAsservissement = true;
    getCurrentSpeeds();
}
=======
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
        That's why there is the flag "flagAsservissement", and the rest is done in loop()
    */
void asservissement()
{
    flagAsservissement = true;
    getCurrentSpeeds();
}
>>>>>>> ae5b5afd93b7b3d030e80e9e672bd130ac63944d
