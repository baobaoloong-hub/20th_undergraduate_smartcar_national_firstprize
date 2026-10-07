#include "motor.h"
#include "PID.h"

#define speed_limit 9999
float fuya_speed=0;



void motor8874_init()
{

    gpio_init(DIR_R1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                           // GPIO ????????????? ??????????????????
    pwm_init(PWM_R1, 17000, 0);                                                 // PWM ??????????????? 17KHz ???????? 0
    gpio_init(DIR_L1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                           // GPIO ????????????? ??????????????????
    pwm_init(PWM_L1, 17000, 0);                                                 // PWM ??????????????? 17KHz ???????? 0
    gpio_init(DIR_FUYA, GPO, GPIO_HIGH, GPO_PUSH_PULL);                                             
    pwm_init(PWM_R_FUYA, 17000, 0);

}

void set_motor(pwm_channel_enum pwm_pin, pwm_channel_enum dir_pin, int16 speed) {
    if (speed >= 0) {
        pwm_set_duty(pwm_pin, speed);
        gpio_set_level(dir_pin, 0);
    } else {
        pwm_set_duty(pwm_pin, -speed);
        gpio_set_level(dir_pin, 1);
    }
}

void Motor_run(int16 motor1, int16 motor2) {
    if(motor1>speed_limit)motor1=speed_limit;
    if(motor1<-speed_limit)motor1=-speed_limit;
    if(motor2>speed_limit)motor2=speed_limit;
    if(motor2<-speed_limit)motor2=-speed_limit;
    
    set_motor(PWM_R1, DIR_R1, motor1);  // ????????????
    set_motor(PWM_L1, DIR_L1, -motor2);  // ????????????
}


// //encoder
// #define PIT0                             (PIT_CH0 )                             // ??????????????????                
                                                                                
// #define ENCODER_QUAD1                    (TC_CH58_ENCODER)                      // ???????????????  
// #define ENCODER_QUAD1_PHASE_A            (TC_CH58_ENCODER_CH1_P17_3)            // PHASE_A ?????????????                 
// #define ENCODER_QUAD1_PHASE_B            (TC_CH58_ENCODER_CH2_P17_4)            // PHASE_B ?????????????                   
                                                                                
// #define ENCODER_QUAD2                    (TC_CH27_ENCODER)                      // ??????????????
// #define ENCODER_QUAD2_PHASE_A            (TC_CH27_ENCODER_CH1_P19_2)            // PHASE_A ?????????????
// #define ENCODER_QUAD2_PHASE_B            (TC_CH27_ENCODER_CH2_P19_3)            // PHASE_B ?????????????

int32 encoder_data_quad[2] = {0};
int32 encoder_data_ins[2] = {0};

long int encoder_data[2] = {0};
long int encoder_data_last[2] = {0};

uint8 pit_state = 0;

void encoder_init(){

    encoder_quad_init(ENCODER_QUAD1, ENCODER_QUAD1_PHASE_A, ENCODER_QUAD1_PHASE_B);  // ????????????????????????????? ?????????????????
    encoder_quad_init(ENCODER_QUAD2, ENCODER_QUAD2_PHASE_A, ENCODER_QUAD2_PHASE_B);  // ????????????????????????????? ?????????????????

    pit_ms_init(PIT_CH1, 1);                                                     // ??????? PIT0 ??????????? 100ms ???

}

void encoder_get_ins(void)
{
        encoder_data_ins[0] = encoder_get_count(ENCODER_QUAD1);    //ÓÒ±àÂëÆ÷
        // encoder_clear_count(ENCODER_QUAD1);
        encoder_data_ins[1] = -encoder_get_count(ENCODER_QUAD2);   //×ó±àÂëÆ÷
        // encoder_clear_count(ENCODER_QUAD2);
}




void encoder_get(void){        //??????????
    
        encoder_data_quad[0] = encoder_get_count(ENCODER_QUAD1);    //ÓÒ±àÂëÆ÷
        encoder_clear_count(ENCODER_QUAD1);
        encoder_data_quad[1] = -encoder_get_count(ENCODER_QUAD2);   //×ó±àÂëÆ÷
        encoder_clear_count(ENCODER_QUAD2);
        total_distance+=(encoder_data_quad[0]+encoder_data_quad[1])/2;
        left_total+=encoder_data_quad[0];
        if(left_total<=0)left_total=0;

        // encoder_data[0] = encoder_get_count(ENCODER_QUAD1);    //ÓÒ±àÂëÆ÷
        // encoder_data[1] = -encoder_get_count(ENCODER_QUAD2);   //×ó±àÂëÆ÷

        // if (encoder_data[0]>=50000)
        // {
        //     encoder_clear_count(ENCODER_QUAD1);
        // }
        
        // total_distance+=(encoder_data_quad[0]+encoder_data_quad[1])/2;
        // left_total+=encoder_data_quad[0];
        // if(left_total<=0)left_total=0;
        


}

//?????

