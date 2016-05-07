#ifndef __MOUVEMENT__
#define __MOUVEMENT__

void avancer(int angle, float vitesse);
void reculer(int angle, float vitesse);
int differentielVitesse(float vitesse1);
void asservissement(float vitesse);
boolean distanceSecuriteRespectee(float vitesse);

#endif
