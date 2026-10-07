#ifndef _CONTROL_H_
#define _CONTROL_H_
 
#include "zf_common_headfile.h"

void speed_loop();
//
void Divert_loop();
//
void BMI270_G_loop();
void enviro_state_init();
extern float pid_number[512];
extern float enviro_state[512];
extern float MT9V03X_EXP_number;
// extern PID speed_l;
// extern PID speed_r;

// extern PID camera;
// extern PID BMI270_G;
// extern PID KP2_err;
// extern PID Divert;


#endif
