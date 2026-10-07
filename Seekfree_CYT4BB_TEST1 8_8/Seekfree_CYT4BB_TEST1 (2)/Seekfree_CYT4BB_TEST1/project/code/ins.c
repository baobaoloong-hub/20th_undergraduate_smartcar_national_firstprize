#include "ins.h"
#include "pid.h"

#define  right_encoder (encoder_data_quad[0])
#define  left_encoder (encoder_data_quad[1])

float record_yaw[point_max];
uint8 record_mode = 0;//采点，赛道记忆模式
int16 record_encoder = 0;//赛道记忆时的编码器积分
uint32 record_index = 0;
uint8 record_stop = 0;
uint8 flash_page = 0;
uint8 replay_stop=0;
uint8 break_count = 0;//断路数

uint8 replay_mode = 0;//路径回放模式
uint32 replay_index = 0;
volatile uint8 current_replay_page = 4;
int16 replay_encoder = 0;
float target_yaw = 0;
uint8 stop_replay= 0;

void ins_track(void)
{  
  if(record_mode == 1) //赛道记忆模式
  {
    record_encoder += ((left_encoder + right_encoder)/2);
    if(record_encoder >vol_now-15) //一定距离采点
    {
      if(record_index < point_max-1)
      {
        record_index++;
        record_yaw[record_index] = T_N;
        // printf("%.1f\n\r", record_yaw[record_index]);
        record_encoder = 0;
      }
      else
      {
        record_stop = 1;//写满自动停止记录，也可手动按键控制
      }
    }
  }
  
  
  if(record_stop == 1) //记录完后将轨迹数据存入flash
  {
    record_stop = 0;//回到初始状态
    record_mode = 0;
    record_encoder=0;
    T_N = 0;
    
    flash_page = 4 + break_count; //存储页（456）
    flash_buffer_clear();//清空缓冲区
    
    for(int i=0;i<record_index;i++)
    {
      flash_union_buffer[i].float_type = record_yaw[i];  //将角度存入缓冲区
    }
    
    flash_union_buffer[point_max].uint32_type = record_index; //在最后一个位置存储点数
    
    if(flash_check(0 ,flash_page))
    {
      flash_erase_page(0,flash_page);
    }

    
    flash_write_page_from_buffer(0 ,flash_page,FLASH_PAGE_LENGTH-1);
    flash_buffer_clear();
    
    break_count++;
    record_index = 0;
  }
  
  
  //回放路径
  if(replay_mode == 1)
  {
    if(replay_index == 0)
    {
      T_N=0;
      circle_flag=0;
      huandao_state=0;
      flash_read_page_to_buffer(0,current_replay_page,FLASH_PAGE_LENGTH-1);          
      uint32_t stored_point = flash_union_buffer[point_max].uint32_type;      
      if(stored_point< point_max) record_index = stored_point;
      
      //printf("%d\n\r",record_index);      
      for(int i=0;i<record_index;i++)
      {
        record_yaw[i] = flash_union_buffer[i].float_type; //从缓冲区取出轨迹数据
      }
    }   
    replay_encoder += ((left_encoder + right_encoder)/2);
    if(replay_encoder >vol_now-15)
    {
      replay_encoder = 0;
      if(replay_index < record_index-1)
      {
        replay_index++;
        target_yaw = record_yaw[replay_index];
      }
      
    }
   
    if(replay_index >= record_index-1 )
    {
        replay_stop=1;
      //target_yaw= 0;
      // stop_replay= 0;
    }
    if(replay_stop==1)
    {
        current_replay_page++; //下一回放则进入下一断路区、
        if(current_replay_page > BREAK_PAGE)
        {
          current_replay_page = 4;
        }
        target_yaw= 0;
        replay_index= 0;
        record_index=0;
        replay_mode= 0;
        replay_stop=0;
        T_N = 0;
    }
  }  
}