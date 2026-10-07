
#include "zf_common_headfile.h"

float aim_G=0;
// **************************** 代码区域 ****************************


int main(void)
{
    clock_init(SYSTEM_CLOCK_250M); 	// 时钟配置及系统初始化<务必保留>
    debug_init();                       // 调试串口信息初始化
    flash_init();
    flash_PID_1_get();
    flash_environment_get();
    pid_init();
    enviro_state_init();
    mt9v03x_init();
    motor8874_init();

    // ips200_init(IPS200_TYPE_SPI);
    // ips114_init();


    uart_date_init();//串口1

    imu660ra_init();
    IMU_gyro_Offset_Init();

    menu_init();


    encoder_init();//5ms

    key_init(1);
    pit_init(PIT_CH0,1000);
    timer_init(TC_TIME2_CH0, TIMER_US);
    // pid_init();

    while(true)
    {
       pwm_set_duty(PWM_R_FUYA,fuya_speed);
       gpio_set_level(DIR_FUYA,1);
        // key_scanner();

        // if(key_get_state(KEY_4)==KEY_SHORT_PRESS)mode=1;
        // if(key_get_state(KEY_3)==KEY_SHORT_PRESS)mode=2;
        // if(key_get_state(KEY_1)==KEY_SHORT_PRESS)
        // {
        //     mode=3;       
        //     i_Track=0;
        //     T_N=0;
        //     total_distance=0;
        // }
        // if(key_get_state(KEY_2)==KEY_SHORT_PRESS){T_N=0;        turn_flag=0;        }

        // if(key_get_state(sw_2)==KEY_LONG_PRESS)remote_control();
        // if(key_get_state(sw_2)==KEY_LONG_PRESS)remote_control();

        // if(mode==1){}//xunji
        // if(mode==2){Auto_Collect();}
        // timer_start(TC_TIME2_CH0);
        if(mt9v03x_finish_flag==1)
        {
            image_process();

            Pid_Position(0,offset,&camera);
            // Pid_Position(0,offset*abs(offset)/7000,&KP2_err);
            
            // Pid_Position(0,IMU_Data.gyro_z,&BMI270_G);
                        // Pid_Position(offset_circle,IMU_Data.gyro_z,&BMI270_G);

            Pid_Position(camera.Out+KP2_err.Out,IMU_Data.gyro_z,&BMI270_G);
            // Pid_Position(0,offset+BMI270_G.Out,&camera);
            // ins_track();
        }

        key_scanner();
        choose_menu();

        // timer_stop(TC_TIME2_CH0);
        // printf("Timer count is %dus.\r\n",timer_get (TC_TIME2_CH0));
        // timer_clear(TC_TIME2_CH0);

    //  printf("%f,%f\r\n",offset_circle,IMU_Data.gyro_z);
// printf("%d,%d\r\n",encoder_data_quad[1],vol);//左
     

    }
}


// **************************** 代码区域 ****************************
