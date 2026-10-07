/*---------------------------------
*   File name: Posture.h
*	
*	Author: LaZuli
*
*	Created time: 2024年4月10日
---------------------------------*/
#ifndef CODE_POSTURE_H_
#define CODE_POSTURE_H_

#include "zf_common_headfile.h"

/*--------------------结构体初始化--------------------*/
typedef struct{
    float Xdata;   //零飘参数X
    float Ydata;   //零飘参数Y
    float Zdata;   //零飘参数Z
}gyro_param_t ;

typedef struct{
    float acc_x;   //x轴加速度
    float acc_y;   //y轴加速度
    float acc_z;   //z轴加速度

    float gyro_x;  //x轴角速度
    float gyro_y;  //y轴角速度
    float gyro_z;  //z轴角速度
}IMU_param_t ;

typedef struct IMU_Original{
        float ax;
        float ay;
        float az;
        float gx;
        float gy;
        float gz;
        float mx;
        float my;
        float mz;
}IMU_Original;

extern IMU_Original IMU_original;

typedef struct Angle{
        float pitch_temp;
        float roll_temp;
        float pitch;
        float roll;
        float yaw;
}Angle;

typedef struct First_Complement{
        Angle angle;
}First_Complement;

typedef struct extKalman_t{
    float X_last; //上一时刻的最优结果  X(k-|k-1)
    float X_mid;  //当前时刻的预测结果  X(k|k-1)
    float X_now;  //当前时刻的最优结果  X(k|k)
    float P_mid;  //当前时刻预测结果的协方差  P(k|k-1)
    float P_now;  //当前时刻最优结果的协方差  P(k|k)
    float P_last; //上一时刻最优结果的协方差  P(k-1|k-1)
    float kg;     //kalman增益
    float A;      //系统参数
    float B;
    float Q;
    float R;
    float H;
}extKalman_t;
/*--------------------结构体初始化--------------------*/

/*--------------------函数声明--------------------*/
void First_complement(void);
void low_pass_filter_init(void);
float low_pass_filter(float value);
float My_abs(float x);
float IMU_gyro_Offset_Init(void);
void IMU_GetValues(void);
void IMU_YAW_integral(void);
void IMU_Handle_180(void);
void IMU_Handle_360(void);
void IMU_Handle_0(void);
float fast_sqrt(float num);
//卡尔曼滤波
void KalmanCreate(extKalman_t *p,float T_Q,float T_R);
float KalmanFilter(extKalman_t* p,float dat);

/*--------------------函数声明--------------------*/

/*----------参数extern----------*/
extern First_Complement first_complement;
extern IMU_param_t  IMU_Data;
extern extKalman_t Kalman1;
extern extKalman_t Kalman2;
extern extKalman_t Zero;

extern float gyro_Offset_flag;
extern float Daty_Z;
extern float Daty_X;
extern float Daty_Y;
extern float T_M;
extern float T_N;
extern int   GL_IMU_Flag;
extern float Pitch_Max;
extern float Pitch_Min;
extern float Roll_Max;
extern float Roll_Min;
extern float Max_Delta_Pitch;
extern float Max_Delta_Roll;

extern float roll_zhen;


/*----------参数extern----------*/

#endif /* CODE_POSTURE_H_ */
