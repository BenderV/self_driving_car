#ifndef __ACCESSEURS__
#define __ACCESSEURS__


/**SRF08**/
usd_t getRangeSRF(int address, usd_t usd);
int getLightSRF(int address);
int getSoftSRF(int address);

/**Motors**/
double getVoltageRD01();
long getEncoder1Counts();
long getEncoder2Counts();
void zeroEncoders();
void getCurrentSpeeds();
void setMotorsSpeed();

/**Raspberry Pi Communication**/
void getPIDConfigDataFromRPi();
void sendPIDConfigDataToRPi();
void getDataFromRPi();
void sendDataToRPi();

/**General**/
int isAreaClean();
void intitializeSlaves();
void zeroEncoders();
void getCurrentSpeeds();
void testSecurityDistance();


#endif
