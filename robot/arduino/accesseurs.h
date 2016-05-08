#ifndef __ACCESSEURS__
#define __ACCESSEURS__


/**SRF08**/
int getRangeSRF(int adresse);
int getLightSRF(int adresse);
int getSoftSRF(int adresse);

/**Motors**/
double getVoltageRD01();
long getEncoder1Counts();
long getEncoder2Counts();
void zeroEncoders();
void getCurrentSpeeds();

/**Raspberry Pi Communication**/
void getPIDConfigDataFromRPi();
void sendPIDConfigDataToRPi();
void getDataFromRPi();
void sendDataFromRPi();

/**General**/
int isAreaClean();
void intitializeSlaves();



#endif
