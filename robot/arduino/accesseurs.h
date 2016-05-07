#ifndef __ACCESSEURS__
#define __ACCESSEURS__


/**SRF08**/
int getRangeSRF(int adresse);
int getLightSRF(int adresse);
int getSoftSRF(int adresse);

/**Motors**/
void initialisation
double getVoltageRD01();
long getEncoder1Counts();
long getEncoder2Counts();
void zeroEncoders();

/**Raspberry Pi**/
void updateDataFromRPi();
#ifdef PID_CONFIG_MODE
    void sendSpeedToRPi(speed_t currentSpeeds);
#endif // PID_CONFIG_MODE

/**General**/
int isAreaClean();
void getCurrentSpeeds();


#endif
