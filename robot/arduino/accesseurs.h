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
<<<<<<< HEAD
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
=======
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
>>>>>>> ae5b5afd93b7b3d030e80e9e672bd130ac63944d



#endif
