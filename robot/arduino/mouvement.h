#ifndef __MOUVEMENT__
#define __MOUVEMENT__

void avancer(int angle, int vitesse);
int differentielVitesse(int vitesse1);
void speedRegulation(int vitesse);
boolean distanceSecuriteRespectee(int vitesse);

#endif
