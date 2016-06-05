#ifndef __STRUCTURES__
#define __STRUCTURES__



  /***MODES DEFINITION***/

typedef enum { INACTIVE_MODE, AUTONOMOUS_MODE, CONTROL_MODE } mode_t;
typedef enum { STOPPING_STATE, MOVING_STATE, PARKING_STATE } state_t;


  /***STRUCTURES DEFINITION***/

typedef struct speed
{
    int leftWheel;
    int rightWheel;
} speed_t;


//#ifdef PID_CONFIG_MODE
//    typedef struct donneesPID
//    {
//        float kp;
//        float ki;
//        float kd;
//    } PID_t;
//#endif // PID_CONFIG_MODE


#endif
