#include "navigation.h"

int16_t    aim_distance;
float    aim_yaw=0;
int32    total_distance=0;
int32 left_total=0;

uint8_t mode_rem=0;

point_data point[10000];

uint16_t i_cb=0;

void Auto_Collect(){

    
    point[i_cb] .data_distance= total_distance;//point()->gps.Latitude;
    point[i_cb] .yaw= T_N;//point()->gps.Longitude;
    // Encoder_Total = 0;
    i_cb++;
    
    }

    // uint16_t count_divert=0;
    // if(mode_rem==0){}
    // point[count_divert].data_distance=total_distance;
    // point[count_divert].yaw=T_N;
    // count_divert++;

uint16_t i_Track=0;

void start_track(){
///*------------------Âæ?Ëø?(ÈùíÊò•Áâ?)-----------------*/
    if((point[i_Track].data_distance - total_distance )<100){
                aim_yaw = point[i_Track].yaw;//‰ΩøÂΩìÂâçË?íÂ∫¶‰æùÊ?°Âèò‰∏∫ÁÇπ‰ΩçÁöÑËßíÂ∫¶ÂÄ?
                if(i_Track<=i_cb)i_Track ++;
                }
        
    ///*--------------------------------------------*/
    }



void UI_navigation_col(){
    ips200_show_string(0, 16*1,"break_count:");ips200_show_int(150, 16*1, break_count, 4);
    ips200_show_string(0, 16*2,"record_encoder:");ips200_show_int(150,16*2,record_encoder,4);
    ips200_show_string(0, 16*3,"mode:");ips200_show_int(150, 16*3, mode, 4);
    ips200_show_string(0, 16*4,"T_N:");ips200_show_int(150, 16*4, T_N, 4);//replay_encoder
    ips200_show_string(0, 16*5,"replay_encoder:");ips200_show_int(150, 16*5, replay_encoder, 4);
    ips200_show_string(0, 16*6,"replay_mode:");ips200_show_int(150, 16*6, replay_mode, 4);
    ips200_show_string(0, 16*7,"target_yaw:");ips200_show_int(150, 16*7, target_yaw, 4);
    ips200_show_string(0, 16*8,"current_replay_page:");ips200_show_int(200, 16*8, current_replay_page, 4);
    if(key0_short)
    {
        mode=0;
        record_mode=1;
        
    }
    if(key1_short)
    {
        record_stop=1;
    }
    if(key5_short)
    {

        mode=3;
        // ips200_show_float(0,16*4,target_yaw,3,4);

    }
    // if(key1_short){mode++;if(mode>=4)mode=0;}
    
    // if(key3_short){total_distance=0;T_N=0;i_Track=0;}

    // ips200_show_int(0, 16*0, mode, 4);
    // ips200_show_int(0, 16*1, total_distance, 7);
    // ips200_show_float(50, 16*1, T_N, 4,2);
    // if(mode==2)
    // {
    //     if(key2_short){
    //     point[i_cb] .data_distance= left_total;//point()->gps.Latitude;
    //     point[i_cb] .yaw= T_N;//point()->gps.Longitude;
    //     // Encoder_Total = 0;
    //     i_cb++;}

    //     ips200_show_int(50, 16*0, i_cb, 4);
        
    //     ips200_show_int(0, 16*9, point[i_cb-1] .yaw, 4);
    //     ips200_show_int(50, 16*9, point[i_cb-1] .data_distance, 7);
    // }
    // if(mode==3)
    // {
    //     // vol=100;
    //     start_track();
    //     // aim_yaw=0;
    //     Divert_loop(); 

    //     Pid_Incre(vol-Divert.Out,encoder_data_quad[1],&speed_l);
    //     Pid_Incre(vol+Divert.Out,encoder_data_quad[0],&speed_r);

    //     Motor_run(speed_r.Out,speed_l.Out);


    //     ips200_show_int(50, 16*0, i_Track, 4);
        
    //     ips200_show_int(0, 16*9, point[i_Track-1] .yaw, 4);
    //     ips200_show_int(50, 16*9, point[i_Track-1] .data_distance, 4);


    // }

}
