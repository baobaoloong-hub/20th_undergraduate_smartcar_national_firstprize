/*---------------------------------
*   File name: Posture.c
*
*	Author: LaZuli
*
*	Created time: 2024��4��10��
---------------------------------*/
#include "Posture.h"

gyro_param_t Gyro_Offset;//��������Ʈ�ṹ��
IMU_param_t  IMU_Data;   //��ȥ��Ʈ���ݽṹ��
extKalman_t Kalman1;
extKalman_t Kalman2;
extKalman_t Zero;
float gyro_Offset_flag = 0;
float Daty_Z = 0;
float Daty_X = 0;
float Daty_Y = 0;
float T_M = 0;
float T_N = 0;

/*
 * @brief ��Ԫ����̬����
 * NULL
 * */

/*
 * @brief һ�׻����˲���̬����
 * */
float Pitch_Max = 0;
float Pitch_Min = 0;
float Roll_Max = 0;
float Roll_Min = 0;
float Max_Delta_Pitch = 0;
float Max_Delta_Roll = 0;

float roll_zhen = 0;

First_Complement first_complement;

void First_complement(){

    float k = 0.85;
    static float Pitch_Temp = 0;
    static float Roll_Temp = 0;
    static float Delta_Roll = 0;
    static char Pitch_Flag = 0;
    static char Roll_Flag = 0;
//    static float Integral_Pitch = 0;
//    static float Integral_Roll = 0;
/*----------------------------------------�ǶȻ�ȡ----------------------------------------*/

    Pitch_Temp = (atan(IMU_Data.acc_y/IMU_Data.acc_z))*180/3.1415926535f;
    Roll_Temp = (atan(IMU_Data.acc_x/IMU_Data.acc_z))*180/3.1415926535f;

    Pitch_Temp = Pitch_Temp + IMU_Data.gyro_x * 0.005f;
    Roll_Temp = Roll_Temp + IMU_Data.gyro_y * 0.005f;

//    Integral_Pitch += RAD_TO_ANGLE(IMU_Data.gyro_x) * 0.005f;
//    Integral_Roll += RAD_TO_ANGLE(IMU_Data.gyro_y) * 0.005f;

    first_complement.angle.pitch = k * Pitch_Temp + (1 - k) * (first_complement.angle.pitch + IMU_Data.gyro_x * 0.005f);
    first_complement.angle.roll = k * Roll_Temp + (1 - k) * (first_complement.angle.roll + IMU_Data.gyro_y * 0.005f);

//    first_complement.angle.pitch = k * Pitch_Temp + (1 - k) * Integral_Pitch;
//    first_complement.angle.roll = k * Roll_Temp + (1 - k) * Integral_Roll;

    if(Delta_Roll < 0.1 && Delta_Roll > -0.1){
        first_complement.angle.roll = first_complement.angle.roll - Delta_Roll;
    }
//    else if(Delta_Roll > 1 || i > 10){
//        first_complement.angle.roll = 0.5 * first_complement.angle.roll;
//    }
    first_complement.angle.roll = KalmanFilter(&Kalman1,first_complement.angle.roll);//�������˲�
    first_complement.angle.pitch = KalmanFilter(&Kalman2,first_complement.angle.pitch);//�������˲�
//    first_complement.angle.pitch = firstOrderFilter(first_complement.angle.pitch);
//    first_complement.angle.roll = firstOrderFilter(first_complement.angle.roll);

///*------------------ͻ���˲�-----------------*/

//static float roll_last = 0;
//
//if(My_abs(first_complement.angle.roll-roll_last)>0.1)roll_zhen=roll_last;
//else roll_zhen=first_complement.angle.roll;
//roll_last=first_complement.angle.roll;



///*--------------------------------------------*/

/*----------------------------------------�ǶȻ�ȡ----------------------------------------*/

    if(Pitch_Flag == 0 ){
        Pitch_Min = first_complement.angle.roll;
        Pitch_Max = first_complement.angle.roll;

        Pitch_Flag = 1;
    }

    if(Roll_Flag == 0 ){
        Roll_Min = first_complement.angle.pitch;
        Roll_Max = first_complement.angle.pitch;

        Roll_Flag = 1;
    }

    if(My_abs(Pitch_Min) > My_abs(first_complement.angle.roll)){
        Pitch_Min = My_abs(first_complement.angle.roll);
    }
    else if(My_abs(Pitch_Max) < My_abs(first_complement.angle.roll)){
        Pitch_Max = My_abs(first_complement.angle.roll);
    }

    if(My_abs(Roll_Min) > My_abs(first_complement.angle.pitch)){
        Roll_Min = My_abs(first_complement.angle.pitch);
    }
    else if(My_abs(Roll_Max) < My_abs(first_complement.angle.pitch)){
        Roll_Max = My_abs(first_complement.angle.pitch);
    }
    Max_Delta_Pitch = Pitch_Max - Pitch_Min;
    Max_Delta_Roll = Roll_Max - Roll_Min;
}


/**
 * @brief ��������Ư��ʼ��
 * ͨ���ɼ�һ���������ֵ�������������ƫ��ֵ��
 * ���� �����Ƕ�ȡ������ - ��Ʈֵ������ȥ������?������
 */
float IMU_gyro_Offset_Init()
{
    Gyro_Offset.Xdata = 0;
    Gyro_Offset.Ydata = 0;
    Gyro_Offset.Zdata = 0;
    for (uint16_t i = 0; i < 100; i++)
    {
//        Gyro_Offset.Xdata += imu660ra_gyro_x;
//        Gyro_Offset.Ydata += imu660ra_gyro_y;
//        Gyro_Offset.Zdata += imu660ra_gyro_z;

        Gyro_Offset.Xdata += imu660ra_gyro_x;
        Gyro_Offset.Ydata += imu660ra_gyro_y;
        Gyro_Offset.Zdata += imu660ra_gyro_z;
        system_delay_ms(5);   // ���? 1Khz
    }

    Gyro_Offset.Xdata /= 1000;
    Gyro_Offset.Ydata /= 1000;
    Gyro_Offset.Zdata /= 1000;

    return gyro_Offset_flag=1;
}

void IMU_GetValues()//���ɼ�����ֵת��Ϊʵ������ֵ, ���������ǽ���ȥ��Ư����
{
//2000dps:IMU660--16.4
//2000dps:IMU660--14.3
        imu660ra_get_gyro();
        imu660ra_get_acc();
    //! �����ǽ��ٶȱ���ת��Ϊ�����ƽ��ٶ�: deg/s -> rad/s
//    IMU_Data.gyro_x = ((float) imu660ra_gyro_x - Gyro_Offset.Xdata) * PI / 180 / 16.4f;
//    IMU_Data.gyro_y = ((float) imu660ra_gyro_y - Gyro_Offset.Ydata) * PI / 180 / 16.4f;
//    IMU_Data.gyro_z = ((float) imu660ra_gyro_z - Gyro_Offset.Zdata) * PI / 180 / 16.4f;

        IMU_Data.gyro_x = ((float) imu660ra_gyro_x - Gyro_Offset.Xdata) * PI / 180 / 16.4f;
        IMU_Data.gyro_y = ((float) imu660ra_gyro_y - Gyro_Offset.Ydata) * PI / 180 / 16.4f;
        IMU_Data.gyro_z = ((float) imu660ra_gyro_z - Gyro_Offset.Zdata) * PI / 180 / 16.4f;

        IMU_Data.acc_x = (((float)imu660ra_acc_x) * 0.3f) + IMU_Data.acc_x * 0.7f;
        IMU_Data.acc_y = (((float)imu660ra_acc_y) * 0.3f) + IMU_Data.acc_y * 0.7f;
        IMU_Data.acc_z = (((float)imu660ra_acc_z) * 0.3f) + IMU_Data.acc_z * 0.7f;


}

void IMU_YAW_integral()//�Խ��ٶȽ��л���
{
//        IMU_GetValues();

//        Daty_X-=RAD_TO_ANGLE(IMU_Data.gyro_x*0.005);
//        Daty_Y-=RAD_TO_ANGLE(IMU_Data.gyro_y*0.005);
    //    if(IMU_Data.gyro_z<0.0045&&IMU_Data.gyro_z>-0.0045)
        if(IMU_Data.gyro_z<0.015&&IMU_Data.gyro_z>-0.015)//�˲�
        {
            Daty_Z-=0;

        }
//        else if(IMU_Data.gyro_z < - 5|| IMU_Data.gyro_z > 5){
//            Daty_Z-=0;
//        }
        else
        {
            IMU_Handle_180();//�滮Ϊ0-180��0-(-180)   Daty_Z
            IMU_Handle_360();//�滮Ϊ0-360��0-(-360)   T_M
            IMU_Handle_0();  //�滮Ϊ0-�������?0-������  T_N
         }

        if(IMU_Data.gyro_x<0.015&&IMU_Data.gyro_x>-0.015)//�˲�
        {
            Daty_X-=0;

        }
        if(IMU_Data.gyro_y<0.015&&IMU_Data.gyro_y>-0.015)//�˲�
        {
            Daty_Y-=0;

        }



}

void IMU_Handle_180()
{
    Daty_Z-=RAD_TO_ANGLE(IMU_Data.gyro_z*0.005);//(���ֹ���)��������ʱ��Ϊ��,���ڸ�Ϊ˳ʱ��Ϊ��

    if((Daty_Z>0&&Daty_Z<=180)  ||   (Daty_Z<0&&Daty_Z>=(-180)))//˳ʱ��
    {
        Daty_Z= +Daty_Z;
    }
    else if(Daty_Z>180 && Daty_Z<=360)
    {
        Daty_Z-=360;
    }
    else if(Daty_Z<(-180) && Daty_Z>=(-360))
    {
        Daty_Z+=360;
    }

}


char Round = 0;

void IMU_Handle_360()
{
    if(Round == 0){
        T_M -= RAD_TO_ANGLE(IMU_Data.gyro_z*0.005);
        if(T_M > 360){
            T_M = 360;
            Round = 1;
        }
        else if(T_M < -360){
            T_M = -360;
            Round = 1;
        }
    }
    if(Round == 1){
        if(T_M <= 360 && T_M >= -360){
            T_M += RAD_TO_ANGLE(IMU_Data.gyro_z*0.005);
            if(T_M > 360){
                T_M = 360;
                Round = 0;
            }
        }
        else if(T_M >= -360){
            T_M -= RAD_TO_ANGLE(IMU_Data.gyro_z*0.005);
            if(T_M < -360){

                T_M = -360;
                Round = 0;
            }
        }
//        if(T_M < 0){
//            T_M = 0;
//            Round = 0;
//        }
    }
}
void IMU_Handle_0()
{
    T_N-=RAD_TO_ANGLE(IMU_Data.gyro_z*0.005);//(���ֹ���)��������ʱ��Ϊ��,���ڸ�Ϊ˳ʱ��Ϊ��

}


float fast_sqrt(float num) {
    float halfx = 0.5f * num;
    float y = num;
    long i = *(long*)&y;
    i = 0x5f375a86 - (i >> 1);

    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));
    y = y * (1.5f - (halfx * y * y));
    return y;
    // float y = sqrtf(num);
    // return y;
}

float My_abs(float x) {
    // ����x�ľ���ֵ������Ԫ�����?
    return x < 0 ? -x : x;
}
/**
  * @name   kalmanCreate
  * @brief  ����һ���������˲���
  * @param  p:  �˲���
  *         T_Q:ϵͳ����Э����
  *         T_R:��������Э����
  *
  * @retval none
  * @attention R�̶���QԽ�󣬴���Խ���β���ֵ��Q�������ֻ�ò����?
  *             ��֮��QԽС����Խ����ģ��Ԥ��ֵ��QΪ������ֻ��ģ��Ԥ��
  */

void KalmanCreate(extKalman_t *p,float T_Q,float T_R)
{
    p->X_last = (float)0;
    p->P_last = 0;
    p->Q = T_Q;
    p->R = T_R;
    p->A = 1;
    p->B = 0;
    p->H = 1;
    p->X_mid = p->X_last;
}

/**
  * @name   KalmanFilter
  * @brief  �������˲���
  * @param  p:  �˲���
  *         dat:���˲�����
  * @retval �˲��������?
  * @attention Z(k)��ϵͳ����,������ֵ   X(k|k)�ǿ������˲�����?,���������?
  *            A=1 B=0 H=1 I=1  W(K)  V(k)�Ǹ�˹������,�����ڲ���ֵ����,���Բ��ù�
  *            �����ǿ�������5�����Ĺ�ʽ
  *            һ��H'��Ϊ������,����Ϊת�þ���
  */

float KalmanFilter(extKalman_t* p,float dat)
{
    p->X_mid =p->A*p->X_last;                     //�ٶȶ�Ӧ��ʽ(1)    x(k|k-1) = A*X(k-1|k-1)+B*U(k)+W(K)
    p->P_mid = p->A*p->P_last+p->Q;               //�ٶȶ�Ӧ��ʽ(2)    p(k|k-1) = A*p(k-1|k-1)*A'+Q
    p->kg = p->P_mid/(p->P_mid+p->R);             //�ٶȶ�Ӧ��ʽ(4)    kg(k) = p(k|k-1)*H'/(H*p(k|k-1)*H'+R)
    p->X_now = p->X_mid+p->kg*(dat-p->X_mid);     //�ٶȶ�Ӧ��ʽ(3)    x(k|k) = X(k|k-1)+kg(k)*(Z(k)-H*X(k|k-1))
    p->P_now = (1-p->kg)*p->P_mid;                //�ٶȶ�Ӧ��ʽ(5)    p(k|k) = (I-kg(k)*H)*P(k|k-1)
    p->P_last = p->P_now;                         //״̬����
    p->X_last = p->X_now;
    return p->X_now;                              //���Ԥ����x(k|k)
}
