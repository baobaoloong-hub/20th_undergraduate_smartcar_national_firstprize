#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "zf_common_headfile.h"


// #define DIR_R1              (P05_2)
// #define PWM_R1              (TCPWM_CH12_P05_)
// #define DIR_L1              (P10_2)
// #define PWM_L1              (TCPWM_CH31_P10_3)

#define DIR_R1              (P05_3)
#define PWM_R1              (TCPWM_CH11_P05_2)
#define DIR_L1              (P10_2)
#define PWM_L1              (TCPWM_CH31_P10_3)
#define PWM_R_FUYA              (TCPWM_CH28_P10_0)
#define DIR_FUYA              (P10_1)


//encoder
#define PIT0                             (PIT_CH0 )                             // ʹ�õ������жϱ��?                
                                                                                
#define ENCODER_QUAD1                    (TC_CH58_ENCODER)                      // ��������ӿ�?  
#define ENCODER_QUAD1_PHASE_A            (TC_CH58_ENCODER_CH1_P17_3)            // PHASE_A ��Ӧ������                 
#define ENCODER_QUAD1_PHASE_B            (TC_CH58_ENCODER_CH2_P17_4)            // PHASE_B ��Ӧ������                   
                                                                                
#define ENCODER_QUAD2                    (TC_CH27_ENCODER)                      // �ұ������ӿ�
#define ENCODER_QUAD2_PHASE_A            (TC_CH27_ENCODER_CH1_P19_2)            // PHASE_A ��Ӧ������
#define ENCODER_QUAD2_PHASE_B            (TC_CH27_ENCODER_CH2_P19_3)            // PHASE_B ��Ӧ������


// #define DIR_R1             (P00_3)
// #define PWM_R1              (TCPWM_CH14_P00_2)
// #define DIR_L1              (P18_7)
// #define PWM_L1              (TCPWM_CH51_P18_6)

void motor8874_init(void);
void Motor_run(int16 motor1, int16 motor2);   //��    ��
void encoder_get(void);
void encoder_get_ins(void);
// void encoder_init(void);
// void encoder_exam(void);
// void encoder_get();     //�жϻ�ȡ�ٶ�

// void speed_loop_init();



#endif /* CODE_MOTOR_CTRL_H_ */

 