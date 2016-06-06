#ifndef __CONSTANTES__
#define __CONSTANTES__


#define PID_CONFIG_MODE 1   // comment to supress the possibility of modifying the PID values
#define MD25_REGULATION_MODE 1  // comment to disable automatic regulation of the speed by MD25 card


  /***I/O***/


  /***CONSTANTS***/
#define MAX_SPEED           127 // Maximum speed on a scale from 0 to -128 or 0 to 127
#define MAX_SPEED_RPM       200 // Maximum supposed speed (from motors doc) in rpm
#define DELAY_LOOP          50
#define COUNTS_PER_TURN     360.0   // Counts per turn defined by the encoders
#define DT                  200.0   // Time step between each call to "speedRegulation" and all calculations included
#define KP                  0.1
#define KI                  0.01
#define KD                  0
#define SRF08_DISTANCE      10 // value of distance for which an area is "clean"

/*
I2C device found at address 0x00 !
I2C device found at address 0x58 ! MOTEUR
I2C device found at address 0x70 ! Capteur 0
I2C device found at address 0x71 ! Capteur 2
I2C device found at address 0x72 ! Capteur 4
I2C device found at address 0x73 ! Capteur 6
I2C device found at address 0x74 ! Capteur 8

Pour les capteurs :
cable maron = GND
cable jaune = SCL
cable orange clair = SDA
cable rose = +5V
*/

  /***MOTORS & ENCODERS***/
// Module address
#define RD01_ADDR 0x58
// Read only registers
#define RD01_ENCODER1_REG 0x02
#define RD01_ENCODER2_REG 0x06
#define RD01_VOLTAGE_REG 0x0A
#define RD01_MOTOR1_CURRENT_REG 0x0B
#define RD01_MOTOR2_CURRENT_REG 0x0C
// Read & Write registers
#define RD01_RIGHT_WHEEL_REG 0x00
#define RD01_LEFT_WHEEL_REG 0x01
#define RD01_ACC_RATE_REG 0x0E
#define RD01_MODE_REG 0x0F
#define RD01_CMD_REG 0x10
// Possible values of the register MODE
#define RD01_MODE_0 0x00  // 0 (full reverse) 128 (stop) 255 (full forward) for both motors independently [DEFAULT]
#define RD01_MODE_1 0x01  // -128 (full reverse) 0 (stop) 127 (full forward) for both motors independently
#define RD01_MODE_2 0x02  // 0 (full reverse) 128 (stop) 255 (full forward) with speed2 being the turn value
#define RD01_MODE_3 0x03  // -128 (full reverse) 0 (stop) 127 (full forward) for both motors with speed2 being the turn value
// Possible values of the register CMD
#define RD01_CMD_RESET_ENCODERS 0x20
#define RD01_CMD_DIS_SPEED_REG 0x30 // Disable speed regulation by the card
#define RD01_CMD_EN_SPEED_REG 0x31  // Enable speed regulation by the card [DEFAULT]
#define RD01_CMD_DIS_TIMEOUT 0x32 // Disable motors timeout when no communication
#define RD01_CMD_EN_TIMEOUT 0x33  // Enable motors timeout when no communication [DEFAULT]


  /***ULTASONIC CAPTORS***/
// Ultrasonic device modules addresses
#define SRF08_ALL_ADDR 0x00
#define SRF08_0_ADDR 0x70
#define SRF08_2_ADDR 0x71
#define SRF08_4_ADDR 0x72
#define SRF08_6_ADDR 0x73
#define SRF08_8_ADDR 0x74
// Read only registers
#define SRF08_ECHO1_HSB_REG 0x02
#define SRF08_ECHO1_LSB_REG 0x03
#define SRF08_ECHO2_HSB_REG 0x04
#define SRF08_ECHO2_LSB_REG 0x05
#define SRF08_ECHO3_HSB_REG 0x06
#define SRF08_ECHO3_LSB_REG 0x07
#define SRF08_ECHO4_HSB_REG 0x08
#define SRF08_ECHO4_LSB_REG 0x09
#define SRF08_ECHO5_HSB_REG 0x0A
#define SRF08_ECHO5_LSB_REG 0x0B
#define SRF08_ECHO6_HSB_REG 0x0C
#define SRF08_ECHO6_LSB_REG 0x0D
#define SRF08_ECHO7_HSB_REG 0x0E
#define SRF08_ECHO7_LSB_REG 0x0F
#define SRF08_ECHO8_HSB_REG 0x10
#define SRF08_ECHO8_LSB_REG 0x11
#define SRF08_ECHO9_HSB_REG 0x12
#define SRF08_ECHO9_LSB_REG 0x13
#define SRF08_ECHO10_HSB_REG 0x14
#define SRF08_ECHO10_LSB_REG 0x15
#define SRF08_ECHO11_HSB_REG 0x16
#define SRF08_ECHO11_LSB_REG 0x17
#define SRF08_ECHO12_HSB_REG 0x18
#define SRF08_ECHO12_LSB_REG 0x19
#define SRF08_ECHO13_HSB_REG 0x1A
#define SRF08_ECHO13_LSB_REG 0x1B
#define SRF08_ECHO14_HSB_REG 0x1C
#define SRF08_ECHO14_LSB_REG 0x1D
#define SRF08_ECHO15_HSB_REG 0x1E
#define SRF08_ECHO15_LSB_REG 0x1F
#define SRF08_ECHO16_HSB_REG 0x20
#define SRF08_ECHO16_LSB_REG 0x21
#define SRF08_ECHO17_HSB_REG 0x22
#define SRF08_ECHO17_LSB_REG 0x23
// Read & Write registers
#define SRF08_CMD_REG 0x00  // write = start ranging of the module
#define SRF08_LIGHT_REG 0x01  // read = light sensor
// Possible values for CMD register
#define SRF08_CMD_CM 0x51 // Ask for a range with values returned in centimeters
#define SRF08_CMD_MS 0x52 // Ask for a range with values returned in milliseconds


//  /***LIGHTING***/
//#define LED_PHARE_ARRIERE 2
//#define LED_PHARE_STOP    3
//#define LED_PHARE_AVANT   4


#endif
