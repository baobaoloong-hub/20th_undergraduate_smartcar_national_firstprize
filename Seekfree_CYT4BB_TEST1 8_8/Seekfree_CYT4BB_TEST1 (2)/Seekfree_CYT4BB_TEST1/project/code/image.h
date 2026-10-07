#ifndef CODE_IMAG_H_
#define CODE_IMAG_H_
#include "zf_common_headfile.h"
#define image_h 60//图像高度
#define image_w 94//图像宽度

#define white_pixel 255
#define black_pixel 0

#define linecow 60 

extern   uint8 Left_Line[MT9V03X_H]; //左边线数组
extern   uint8 Right_Line[MT9V03X_H];//右边线数组
extern   uint8 Mid_Line[MT9V03X_H];  //中线数组



uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row);
extern uint8 image_thereshold;
extern uint8 original_image[image_h][image_w];  //原始数据
extern uint8 bin_image[image_h][image_w];//图像数组  二值化
extern int err_count;
extern float err;
extern int16 offset;
extern float offset_circle1;
extern float offset_circle2;
extern float otsu_min;
extern float otsu_max;
extern volatile int Search_Stop_Line;     //搜索截止行,只记录长度，想要坐标需要用视野高度减去该值
extern int botton_leader_lost;
void image_process(void);
void Get_image(uint8(*mt9v03x_image)[image_w]);
uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row);
void turn_to_bin(void);
void Get_Threshold(int threshold,uint8_t *im);
void image_filter(uint8(*bin_image)[image_w]);//形态学滤波，简单来说就是膨胀和腐蚀的思想
void Longest_White_Column(void);//最长白列巡线
int camera_err(void);
void ips200_show(void);
int my_abs(int value);
int my_min(int x, int y);
int my_max(int x, int y);
void search_line();
void angle90_detect();
void ips200_showgyro(void);

extern int track_wide[image_h];//赛宽数组
extern int l_border[image_h];//左线数组
extern int r_border[image_h];//右线数组
extern int center_line[image_h];//中线数组
extern int l_founder[image_w];//左引导线数组
extern int r_founder[image_w];//右引导线数组
extern int qiguai;
extern int yuansu;//1为左，2为右
extern int turn_flag;
extern int hightest ;
extern int tingche_test;
extern int tingche_state;
extern int yuanhuan_count;
extern int Left_Leader_count;
extern int Right_Leader_count;
extern int Left_Leader_lost;
extern int Right_Leader_lost;
extern int Left_Leader_count;
extern int Right_Leader_count;
extern int Left_Leader_lost;
extern int Right_Leader_lost;
extern int Both_Lost_count;
extern int Top_Leader_lost;
extern int Top_Leader_count;
extern int circle_wide_flag ;//环岛标志位，用于入环选取偏差
extern int circle_flag;
extern int huandao_state;
extern int start_mideline;
extern int err_get[image_h];
extern int L_leader_states[image_w];
extern int R_leader_states[image_w];
//寻找起始点，从左往右，从下到上遍历
//直线
extern int error_flag;

extern int wide_num[image_h];//每一行的赛宽数量






#endif /* CODE_IMAG_H_ */