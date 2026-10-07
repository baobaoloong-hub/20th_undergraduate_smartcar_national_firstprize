#include"control.h"

PID speed_l;
PID speed_r;

PID camera;
PID BMI270_G;
PID KP2_err;
PID Divert;

float pid_number[512];
float enviro_state[512];
float MT9V03X_EXP_number;
void pid_init(){
    //*****/????????pid    PD
    camera. Kp=pid_number[1];
    camera. Ki=pid_number[2];
    camera. Kd=pid_number[3];
    camera. Err=0;
    camera. Err_last=0;
    camera. Err_l2st=0;  
    camera. Integral_lim=2000; 
    camera. Err_Int=0;  
    camera. P_out=0;
    camera. I_out=0;
    camera. D_out=0;
    camera. Out=0;
    camera. Out_lim=9999;  
    //
    BMI270_G. Kp=pid_number[4];
    BMI270_G. Ki=pid_number[5];
    BMI270_G. Kd=pid_number[6];
    BMI270_G. Err=0;
    BMI270_G. Err_last=0;
    BMI270_G. Err_l2st=0;  
    BMI270_G. Integral_lim=400;
    BMI270_G. Err_Int=0; 
    BMI270_G. P_out=0;
    BMI270_G. I_out=0;
    BMI270_G. D_out=0;
    BMI270_G. Out=0;
    BMI270_G. Out_lim=400;   
    //
    speed_l. Kp=-14;
    speed_l. Ki=-0.6;
    speed_l. Kd=0;
    speed_l. Err=0;
    speed_l. Err_last=0;
    speed_l. Err_l2st=0;  
    speed_l. Integral_lim=9999;
    speed_l. Err_Int=0;  
    speed_l. P_out=0;
    speed_l. I_out=0;
    speed_l. D_out=0;
    speed_l. Out=0;
    speed_l. Out_lim=8000;
    //******/????pid    PI
    speed_r. Kp=14;
    speed_r. Ki=0.6;
    speed_r. Kd=0;
    speed_r. Err=0;
    speed_r. Err_last=0;
    speed_r. Err_l2st=0;  
    speed_r. Integral_lim=9999; 
    speed_r. Err_Int=0;  
    speed_r. P_out=0;
    speed_r. I_out=0;
    speed_r. D_out=0;
    speed_r. Out=0;
    speed_r. Out_lim=8000;      
    
    /////////////////
    KP2_err. Kp=0.0000000;
    KP2_err. Ki=0;
    KP2_err. Kd=0;
    KP2_err. Err=0;
    KP2_err. Err_last=0;
    KP2_err. Err_l2st=0;  
    KP2_err. Integral_lim=2000; 
    KP2_err. Err_Int=0;  
    KP2_err. P_out=0;
    KP2_err. I_out=0;
    KP2_err. D_out=0;
    KP2_err. Out=0;
    KP2_err. Out_lim=9999;      
    /////////////////////////////
    Divert. Kp=pid_number[7];
    Divert. Ki=pid_number[8];
    Divert. Kd=pid_number[9];
    Divert. Err=0;
    Divert. Err_last=0;
    Divert. Err_l2st=0;  
    Divert. Integral_lim=2000;
    Divert. Err_Int=0; 
    Divert. P_out=0;
    Divert. I_out=0;
    Divert. D_out=0;
    Divert. Out=0;
    Divert. Out_lim=9999;    
    vol_now=pid_number[0];
    offset_circle1=pid_number[13];
    offset_circle2=pid_number[15];    
    circle_distance_enter1=pid_number[14];
    circle_distance_enter2=pid_number[16];
    circle_distance_out=pid_number[10];


}

void enviro_state_init()
{
    fuya_speed=enviro_state[0];
    MT9V03X_EXP_number=enviro_state[1];
    otsu_min=enviro_state[2];
    otsu_max=enviro_state[3];
}


void speed_loop(){
    Pid_Position(5,encoder_data_quad[0],&speed_l);
}
//
void Divert_loop(){
    Pid_Position(target_yaw,T_N,&Divert);
}
//
void BMI270_G_loop(){
    Pid_Position(0,IMU_Data.gyro_z,&BMI270_G);
}

