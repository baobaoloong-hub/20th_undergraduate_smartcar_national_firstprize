
#include "image.h"

int16 offset;
float offset_circle1;
float offset_circle2;
float otsu_min;
float otsu_max;
int16 count_ref=0;

void image_process(void)
{
//    Get_image(mt9v03x_image);
    mt9v03x_finish_flag=0;
    count_ref++;
    turn_to_bin();
    image_filter(bin_image);//滤波
    search_line();
    offset=camera_err();
    angle90_detect();

}

void ips200_show(void)
{
            ips200_show_gray_image(100, 0, (const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
           ips200_show_gray_image(0, 0,(const uint8 *)bin_image, image_w, image_h, image_w, image_h,0);
            // ips200_draw_line(54,40,14,120,RGB565_YELLOW);
            // ips200_draw_line(134,40,174,120,RGB565_YELLOW);
            for(uint8 i=(MT9V03X_H -2); i>0; i--)
            {

                ips200_draw_point(Left_Line[i], i , RGB565_BLUE);//显示起点 显示右边线c++;
                ips200_draw_point(Right_Line[i], i, RGB565_GREEN);//显示起点 显示左边线
                ips200_draw_point(Mid_Line[i], i, RGB565_RED);//显示起点 显示中线

            }
            // ips200_show_int(0,8*16,offset,3);
            // ips200_show_int(50,8*16,Both_Lost_count,3);

            // ips200_show_int(20, 16*9, total_distance, 5);
            // ips200_show_int(80, 16*9, mode, 5);

            ips200_show_int(0,10*16,encoder_data_quad[0],3);
            ips200_show_int(50,10*16,encoder_data_quad[1],3); 
            
            // ips200_show_int(0,10*16,L_leader_states[4],3);
            // ips200_show_int(30,10*16,L_leader_states[5],3);
            // ips200_show_int(60,10*16,L_leader_states[6],3);
            // ips200_show_int(90,10*16,L_leader_states[7],3);
                        
            // ips200_show_int(0,12 *16,l_founder[L_leader_states[4]],3);
            // ips200_show_int(30,12 *16,l_founder[L_leader_states[4]],3);
            // ips200_show_int(60,12 *16,l_founder[L_leader_states[4]],3);
            // ips200_show_int(90,12 *16,l_founder[L_leader_states[4]],3);

            // ips200_show_int(0,14*16,R_leader_states[4],3);
            // ips200_show_int(30,14*16,R_leader_states[5],3);
            // ips200_show_int(60,14*16,R_leader_states[6],3);
            // ips200_show_int(90,14*16,R_leader_states[7],3);

            // ips200_show_int(0,16 *16,l_founder[R_leader_states[4]],3);
            // ips200_show_int(30,16 *16,l_founder[R_leader_states[4]],3);
            // ips200_show_int(60,16 *16,l_founder[R_leader_states[4]],3);
            // ips200_show_int(90,16 *16,l_founder[R_leader_states[4]],3);




            // ips200_show_int(0,12*16,Left_Leader_count,3);
            // ips200_show_int(50,12*16,Right_Leader_count,3);
            
            // ips200_show_int(0,14*16,Left_Leader_lost,3);
            // ips200_show_int(50,14*16,Right_Leader_lost,3);
            
            // ips200_show_int(0,16*16,turn_flag,3);
            // ips200_show_int(50,16*16,image_thereshold,3);
            
            // ips200_show_int(0,18*16,circle_wide_flag,3);
            //  ips200_show_int(50, 18*16, T_N, 5);



            ips200_show_int(0,8*16,offset,5);
            ips200_show_int(50,8*16,yuanhuan_count,3);

            ips200_show_int(20, 16*9, total_distance, 8);
            ips200_show_int(200, 16*9, mode, 2);

            // ips200_show_int(0,10*16,track_wide[50],3);
            // ips200_show_int(50,10*16,tingche_test,3);
            // ips200_show_int(100,10*16,tingche_state,3);
            
            ips200_show_int(0,12*16,Left_Leader_lost,3);
            ips200_show_int(50,12*16,Right_Leader_lost,3);
            
            ips200_show_int(0,14*16,Left_Leader_count,3);
            ips200_show_int(50,14*16,Right_Leader_count,3);
            
            ips200_show_int(0,16*16,turn_flag,3);
            ips200_show_int(50,16*16,circle_flag,3);
            ips200_show_int(100,16*16,image_thereshold,3);
            
            ips200_show_int(0,18*16,tingche_state,3);
            ips200_show_int(50,18*16,circle_wide_flag,3);
            ips200_show_int(100, 18*16, T_N, 5);





        if(key_get_state(KEY_1)==KEY_SHORT_PRESS)mode=1;
        if(key_get_state(KEY_2)==KEY_SHORT_PRESS)mode=2;
        if(key_get_state(KEY_3)==KEY_SHORT_PRESS)
        {
            T_N=0;        
            turn_flag=0;   
            circle_flag=0;
            tingche_state=0;
        }


//            ips200_show_int(0,12*16,Mid_Line[MT9V03X_H-Search_Stop_Line],3);
            

}



int my_abs(int value)
{
    if (value >= 0) return value;
    else return -value;
}
int my_min(int x, int y)
{
    if (x >= y)             return 1;
    else if (x < y)       return 0;
    else return 2;
}
int my_max(int x, int y)
{
    if (x >= y)             return 0;
    else if (x < y)       return 1;
    else return 2;
}


uint8 original_image[image_h][image_w];
uint8 image_thereshold;//图像分割阈值
void Get_image(uint8(*mt9v03x_image)[image_w])
{
#define use_num    1 //1就是不压缩，2就是压缩一倍
    uint8 i = 0, j = 0, row = 0, line = 0;
    for (i = 0; i < image_h; i += use_num)          //
    {
        for (j = 0; j <image_w; j += use_num)     //
        {
            original_image[row][line] = mt9v03x_image[i][j];//这里的参数填写你的摄像头采集到的图像
            line++;
        }
        line = 0;
        row++;
    }
}

uint8 bin_image[image_h][image_w];//图像数组


void turn_to_bin(void)
{
  uint8 i,j;
  if(count_ref>=20)
  {
    count_ref=0;
  }
  image_thereshold = otsuThreshold(mt9v03x_image[0], image_w, image_h);
if(image_thereshold<otsu_min)image_thereshold=otsu_min;
if(image_thereshold>otsu_max)image_thereshold=otsu_max;
  for(i = 0;i<image_h;i++)
  {
      for(j = 0;j<image_w;j++)
      {
          if(mt9v03x_image[i][j]>image_thereshold)bin_image[i][j] = white_pixel;
         else bin_image[i][j] = black_pixel;
     }
  }
//    Get_Threshold(image_thereshold,original_image[0]); // /70
}

uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row)
{
#define GrayScale 256
    uint16 Image_Width  = col;
    uint16 Image_Height = row;
    int X; uint16 Y;
    uint8* data = image;
    int HistGram[GrayScale] = {0};

    uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32 PixelIntegralFore = 0;
    int32 PixelFore = 0;
    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // 类间方差;
    uint8 MinValue=0, MaxValue=0;
    uint8 Threshold = 0;


    for (Y = 0; Y <Image_Height; Y++) //Y<Image_Height改为Y =Image_Height；以便进行 行二值化
    {
        //Y=Image_Height;
        for (X = 0; X < Image_Width; X++)
        {
        HistGram[(int)data[Y*Image_Width + X]]++; //统计每个灰度值的个数信息
        }
    }




    for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistGram[MaxValue] == 0; MaxValue--) ; //获取最大灰度的值

    if (MaxValue == MinValue)
    {
        return MaxValue;          // 图像中只有一个颜色
    }
    if (MinValue + 1 == MaxValue)
    {
        return MinValue;      // 图像中只有二个颜色
    }

    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        Amount += HistGram[Y];        //  像素总数
    }

    PixelIntegral = 0;
    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        PixelIntegral += HistGram[Y] * Y;//灰度值总数
    }
    SigmaB = -1;
    for (Y = MinValue; Y < MaxValue; Y++)
    {
          PixelBack = PixelBack + HistGram[Y];    //前景像素点数
          PixelFore = Amount - PixelBack;         //背景像素点数
          OmegaBack = (double)PixelBack / Amount;//前景像素百分比
          OmegaFore = (double)PixelFore / Amount;//背景像素百分比
          PixelIntegralBack += HistGram[Y] * Y;  //前景灰度值
          PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
          MicroBack = (double)PixelIntegralBack / PixelBack;//前景灰度百分比
          MicroFore = (double)PixelIntegralFore / PixelFore;//背景灰度百分比
          Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//g
          if (Sigma > SigmaB)//遍历最大的类间方差g
          {
              SigmaB = Sigma;
              Threshold = (uint8)Y;
          }
    }
   return Threshold;
}

#define threshold_max   255*5//此参数可根据自己的需求调节
#define threshold_min   255*2//此参数可根据自己的需求调节
void image_filter(uint8(*bin_image)[image_w])//形态学滤波，简单来说就是膨胀和腐蚀的思想
{
    uint16 i, j;
    uint32 num = 0;


    for (i = 1; i < image_h - 1; i++)
    {
        for (j = 1; j < (image_w - 1); j++)
        {
            //统计八个方向的像素值
            num =
                bin_image[i - 1][j - 1] + bin_image[i - 1][j] + bin_image[i - 1][j + 1]
                + bin_image[i][j - 1] + bin_image[i][j + 1]
                + bin_image[i + 1][j - 1] + bin_image[i + 1][j] + bin_image[i + 1][j + 1];


            if (num >= threshold_max && bin_image[i][j] == 0)
            {

                bin_image[i][j] = 255;//白  可以搞成宏定义，方便更改

            }
            if (num <= threshold_min && bin_image[i][j] == 255)
            {

                bin_image[i][j] = 0;//黑

            }

        }
    }

}



/*-------------------------------------------------------------------------------------------------------------------
  @brief     左右搜线找标志位
  @param     null
  @return    null
  Sample    search_line()
  @note      最长白列巡线，寻找初始边界，丢线，最长白列等基础元素，后续读取这些变量来进行赛道识别
-------------------------------------------------------------------------------------------------------------------*/
uint8 Left_Line[MT9V03X_H]; //左边线数组
uint8 Right_Line[MT9V03X_H];//右边线数组
uint8 Mid_Line[MT9V03X_H];  //中线数组
int track_wide[image_h];//赛宽数组（完整赛宽，遇虚线重置）
int l_border[image_h];//左线数组
int r_border[image_h];//右线数组
int l_founder[image_w];//左引导线数组
int r_founder[image_w];//右引导线数组
int center_line[image_h];//中线数组
int err_get_flag[image_h];
int err_get[image_h];
int qiguai;
int tingche_state=0;  //停车状态（判断圈数）
int yuanhuan_count=0; 
int error_flag=0; 
int error_flag_dir=0;
int yuansu;//1为左，2为右
int Both_Lost_count=0; //总丢线行数
int Both_Lost_start=0;
int Left_Leader_count=0; //左引导线数（左不丢线数）
int Right_Leader_count=0; //右引导线数（右不丢线数）
int Top_Leader_lost=0;    //顶部丢线数
int botton_leader_lost=0;//底部丢线数
int Top_Leader_count=0; //顶部引导线数（顶部不丢线数）
int Left_Leader_lost=0; //左丢线数
int Right_Leader_lost=0; //右丢线数
int tingche_test=0;      //停车检测标志位
//寻找起始点，从左往右，从下到上遍历
//直线
int start_flag = 0;//最下方起始点标志位

int left_h_find = 0;//每一行的左边起始点标志位
int wide_num[image_h];//每一行的赛宽数量
int circle_wide_flag = 0;//判断环岛时需要得到的两行赛宽数

int turn_flag = 0;       //元素标志位，不止环岛
                        //0：所有积分清零（T_N陀螺仪角度积分清零，total_distance编码器距离积分清零）
                        //1：停车标志位，为1时停车
                        //2：进环标志位，在2时开启编码器距离积分，积分距离超过给定值时进环
                        //3：左环岛进环标志位，进入3时固定左环岛误差
                        //4：右环岛进环标志位，进入4时固定右环岛误差


int circle_flag = 0;//圆环判断标志位
                    //先通过这个标志位表示判断到环岛，进入turn_flag=3/4，之后利用距离积分固定路径入环
int huandao_state = 0;//环岛妆台，1表示入环，2表示环岛内，3表示出环
int hightest = 0;//最高检测到的白线行，不好用，会误判，之后写了好多无用的代码去消除这个误判还是不行
int elemin = 0;//image_w/2
int make_flag = 0;
int Mid_Line_remenber=0;
int start_mideline=0;/*
                    白线赛道起始点，从底部开始判断十行避免虚线的误判，并且后续左右引导线以及丢线全是根据中线起始点来进行判断，
                    所以需要根据需要来进行限幅判断，不能让中线起始点出现在太过图像的左右两边
                    */
int L_leader_states[image_w];    //为了另一种进环方式准备的，写好了没用，各有各的好处，正常圆环百分百进，虚线圆环容易出问题
int R_leader_states[image_w];

void search_line()
{

    error_flag_dir=0;
    elemin = image_w / 2;
    circle_wide_flag = 0;
    Left_Leader_count=0;
    Right_Leader_count=0;
    Left_Leader_lost=0;
    Right_Leader_lost=0;
    Top_Leader_lost=0;
    botton_leader_lost=0;
    Top_Leader_count=0;
    start_flag = 0;
    error_flag=0;
    start_mideline=0;
    hightest = 0;
    tingche_test=0;
    float k_1=0;
    float k_2=0;
    float k_used=0;
    int lostline_flag=0;
    int not_lostline_count=0;
    Mid_Line_remenber = 0;
    int buxian_flag=0;
    Both_Lost_count=0;
    for (int i = 0; i <=MT9V03X_H-1; i++)//数据清零
    {
        Left_Line[i] = 0;
        Right_Line[i] = 0;
        l_border[i]=0;
        r_border[i]=0;
        track_wide[i]=0;
        err_get[i]=0;
        err_get_flag[i]=0;
        center_line[i]=image_w / 2;



    }
    for (int j = 0; j <=MT9V03X_W-1;    j++)//数据清零
    {
        l_founder[j]=0;
        r_founder[j]=0;
        L_leader_states[j]=0;
        R_leader_states[j]=0;


    }
/**********************************************************  第一遍全图像扫线从左往右找赛道边界      *********************************************************************/
    for(int i = image_h - 1; i > 0; i--)//遍历列
    {
        left_h_find = 0;//初始清零
        wide_num[i] = 0;//初始化
        make_flag = 0;
        /*  遍历图像找出中线并且找到左右引导线判断元素  */
        for (int j = 1; j < image_w - 1; j++)//遍历行
        {

/******************** 在左右扫线的时候直接将上下扫线所需要的点记录下来，直接省去整图再遍历一遍的时间 **********************/
            if (bin_image[i][j] == black_pixel && bin_image[i-1][j] == white_pixel && l_founder[j]==0)//由黑到白的跳变点
            {

                  l_founder[j]=i;                  
            //   if(i<image_h-10 && i>10 && j>94)
            //   {
            //       r_founder[j]=i;
            //       Right_Leader_count+=1;
            //   }
            }


/****************************************               正常找寻赛道左右边界           *************************************/
            if (bin_image[i][j] == black_pixel && bin_image[i][j + 1] == white_pixel)//由黑到白的跳变点
            {
                l_border[i] = j;
                left_h_find = 1;//左边起始点已经找到（没用到）
            if (start_flag == 0 && i < image_h - 10)start_flag = 1;//已经找到最下方起始点,并且已经合成了一部分中线
            }
            else if (bin_image[i][j] == white_pixel && bin_image[i][j + 1] == black_pixel)//由白到黑的跳变点
            {
                r_border[i] = j;
                //  if(left_h_find==0)l_border[i]=4;//给右边线进行赋值
                // track_wide[i] += my_abs(r_border[i] - l_border[i]);//记录赛宽（第一版非拉线直角用，赛宽累加用于应对虚线直角）
                // if(wide_num==0 && l_border[i]<60)
                // {
                //     err_get_flag[i]=1;
                // }
                // if(wide_num>=1 && r_border[i]>120)
                // {
                //     err_get_flag[i]=image_w - 1;
                // }
                wide_num[i]++;//此行的赛宽数量加一
            }
            if (l_border[i] == 0 && r_border[i] == 0)//如果左右边线均未找到则强制赋值
            {
              
                l_border[i] = 1;
                r_border[i] = image_w - 1;
                
            }

            if (wide_num[i] ==2  && start_flag == 0 && wide_num[i] != make_flag)//图像下方一定位置采用与elemin做参照
            {
                if (my_min(my_abs(center_line[i] - elemin), my_abs((l_border[i] + r_border[i]) / 2 - elemin)) == 0)
                //在还未确定是否是赛宽时（即start_flag=0时），若一行出现两个赛宽则取和图像中点相比偏差比较小的（离图像中点近的）
                
                {
                   make_flag = wide_num[i]; //找到几行赛宽make_flag就等于几
                }
                else 
                {
                  center_line[i] = (l_border[i] + r_border[i]) / 2;   //生成中线
                  track_wide[i]=my_abs(r_border[i] - l_border[i]);    //生成这一段赛宽
                  make_flag = wide_num[i];                            //每次生成一行赛宽必加
                }
            }
            else if (wide_num[i] == 2 && start_flag == 1 && wide_num[i] != make_flag)
            //正常情况下与上一个点做参照（若一行出现两个赛宽则取和和上一个点相比偏差比较小的（离上一个点生成的中点近的））
            {
                if (my_min(my_abs(center_line[i] - center_line[i + 2]), my_abs((l_border[i] + r_border[i]) / 2 - center_line[i + 2])) == 0)//若一行出现两个赛宽则取偏差比较小的
                {
                    center_line[i] = center_line[i];
                    make_flag = wide_num[i];
                }   
                else 
                {
                  center_line[i] = (l_border[i] + r_border[i]) / 2;
                  track_wide[i]=my_abs(r_border[i] - l_border[i]);
                  make_flag = wide_num[i];
                }
            }


            else if (wide_num[i]!=make_flag)
            {
                center_line[i] = (l_border[i] + r_border[i]) /2;//左右边线合成中线
                track_wide[i]=my_abs(r_border[i] - l_border[i]);
                make_flag = wide_num[i];
            }
            else if (j > image_w - 3 && wide_num[i] == 0)
            {
                center_line[i] = (l_border[i] + r_border[i]) /2;//左右边线合成中线
                track_wide[i]=my_abs(r_border[i] - l_border[i]);
                make_flag = 1;
            }
            
         }
            Left_Line [i] = l_border[i];       //左边线线数组
            Right_Line[i] = r_border[i];      //右边线线数组
            Mid_Line[i] = center_line[i];     //中线数组


/*虚线补线 （没用上，还是因为遇到白点容易乱拉线并且影响直角拉线，因为是想了好几天的东西所以舍不得删）*/
        // if(abs(Mid_Line[i]-Mid_Line[i-1])>10 && Mid_Line_remenber==0 && !(l_border[i] == 1 && r_border[i] == image_w - 1) && !(l_border[i-1] == 1 && r_border[i-1] == image_w - 1))
        // {
        //     Mid_Line_remenber=Mid_Line[i-1];

        // }
        

        if(l_border[i] == 1 && r_border[i] == image_w - 1 )
        {
            track_wide[i]=0;       //没有扫到左右边界就没有赛宽
 //           if(Mid_Line_remenber==0)
 //           {
            if(i<20 && i>0)
            {
                Top_Leader_lost+=1;  //没扫到线的情况出现在图像上方就是顶部丢线，具体取多少行看自身要求

            }
            if(i>40 && i<60)
            {
                botton_leader_lost+=1; //出现在图像下方就是底部丢线，具体取多少行看自身要求
            }
                // if(lostline_flag == 0 && !(l_border[i-1] == 1 && r_border[i-1] == image_w - 1))
                // {
                //     lostline_flag=i;
                //     Both_Lost_count=0;
                // }
                // if(lostline_flag != 0)
                // {
                    Both_Lost_count+=1; //总丢线数，只要丢线就累加

                // }
                not_lostline_count=0;//没用上，虚线补线时用的

//            }
            // else
            // {
            //     lostline_flag=Mid_Line_remenber;  //没用上
            //     Mid_Line_re menber=0;
            // }

        }

        else
        {
/**********  虚线补线，将上一段赛道的顶部与下一段赛道的底部中线拉线相连（没用上，容易误判乱拉，正常虚线好用，感觉对于全图扫线来说没必要）   ***********/
            // if(lostline_flag!=0 && lostline_flag<55 && start_mideline!=0 )
            // {
            //     not_lostline_count+=1;
            //     if(not_lostline_count==3);
            //     {
            //         buxian_flag=1;
            //         k_1=(Mid_Line[lostline_flag+1]-Mid_Line[i])/(float)(lostline_flag+1-i);
            //         if(circle_flag==3 && k_1<0 )
            //         {
            //             buxian_flag=0;
            //         }
            //         if(circle_flag==4 && k_1>0 )
            //         {
            //             buxian_flag=0;
            //         }
            //         if(buxian_flag==1)
            //         {
            //             for(int m = lostline_flag; m >=i-1; m--)
            //             {
            //                 Mid_Line[m]=(int)(k_1*(m-i)+Mid_Line[i]);
            //                 if(Mid_Line[m]>image_w)
            //                 {
            //                     Mid_Line[m]=image_w;
            //                 }
            //                 if(Mid_Line[m]<0)
            //                 {
            //                     Mid_Line[m]=0;
            //                 }
            //             }
            //         }

                    
            //     }

            // }
            k_1=0;
            lostline_flag=0;
            if(start_mideline==0)
            {
                start_mideline=i;
            }
            if(start_mideline!=0)
            {
                if(Mid_Line[start_mideline]>(image_w/2+40) || Mid_Line[start_mideline]<(image_w/2-40))
                {
                    Mid_Line[start_mideline]=image_w/2;
                }

            }
        }
        /* */
        
        if(track_wide[i]>30)
        {
            tingche_test+=1;    //赛宽过大时累加，判断停车线，只有在直角和停车线时会出现大赛宽，停车线出现的行数比直角多得多，累加多一点判断就行，基本不误判
        }
/*识别到圆环处理中线误差，整体中线偏向圆环方向来进环，整体中线误差远离圆环方向来出环*/       
        err_get[i]=Mid_Line[i];
        if (wide_num[i] >= 2)circle_wide_flag += 1; 
        if (wide_num[i] != 0 && center_line[i]!=(image_w/2) && i>=40 )hightest = i;//记录最高点
//        if (wide_num[i] == 2 && circle_wide_flag == 0)circle_wide_flag = 1;//疑似环岛//环岛特征为两个赛宽变成一个赛宽再往上又是俩个赛宽
//        if (wide_num[i] == 1 && circle_wide_flag == 1)circle_wide_flag = 2;
//        if (wide_num[i] == 2 && circle_wide_flag == 2) circle_wide_flag = 3; 
    }


/*******************************************************  二次遍历图像寻找左右引导线（所需要的点已经在第一次全图遍历的时候记录在了l_founder[]这个数组里）      *******************************************************/
for (int j = 5; j < image_w - 5; j++)//因为在第一次遍历时已经记录好点了，现在仅需遍历一遍数组，将所需要的点进行左右引导线的判断即可
{
        if ( j<Mid_Line[start_mideline])      //如果引导线所在的点在中线起始点数组所记录的中点左边即为左引导线，相应的，如果数组里记录的为0即为丢线，记录入左丢线数组
        {
            if(l_founder[j] ==0)
            {
                Left_Leader_lost+=1;
            }
            else
            {
                L_leader_states[Left_Leader_count]=j;             
                Left_Leader_count+=1;
            }
                
        }
         if ( j>Mid_Line[start_mideline])   //如果引导线所在的点在中线起始点数组所记录的中点右边即为右引导线，相应的，如果数组里记录的为0即为丢线，记录入右丢线数组
         {
            if(l_founder[j] ==0)
            {
                Right_Leader_lost+=1;
            }
            else
            {
                R_leader_states[Right_Leader_count]=j;
                Right_Leader_count+=1;
            }
                
         } 

    
 }

/*
    for(int i = 58; i > 0; i--)
    {

        if(track_wide[i]>15 && Both_Lost_count>5 && Left_Leader_lost>20 && Right_Leader_lost<25)
        {
            error_flag=i;
            error_flag_dir=1;
        }
        if(track_wide[i]>15 && Both_Lost_count>5 && Left_Leader_lost<25 && Right_Leader_lost>20)
        {
            error_flag=i;
            error_flag_dir=0;
        }

    }
*/        
/*
                    k_1=(Mid_Line[lostline_flag+1]-Mid_Line[i])/(float)(lostline_flag+1-i);
                    for(int m = lostline_flag; m >=i-1; m--)
                    {
                        Mid_Line[m]=(int)(k_1*(m-i)+Mid_Line[i]);
                        if(Mid_Line[m]>image_w)
                        {
                            Mid_Line[m]=image_w;
                        }
                        if(Mid_Line[m]<0)
                        {
                            Mid_Line[m]=0;
                        }
                    }
*/
    // if(error_flag!=0 && error_flag_dir==1 && turn_flag==0)
    if(  Left_Leader_count<20 && Right_Leader_count>18 && Top_Leader_lost >= 5)                //右转直角弯判断拉线
    {
        k_2=(94-Mid_Line[start_mideline])/(float)(5-start_mideline);                //计算斜率
        for(int m = start_mideline; m >=5; m--)                             //根据斜率通过y=kx+b来挨个点赋值（简单的拉线函数，可以自己封装一下，有点懒没封装）
        {
            Mid_Line[m]=(int)(k_2*(m-start_mideline)+Mid_Line[start_mideline]);
            if(Mid_Line[m]>image_w)
            {
                Mid_Line[m]=image_w;
            }
            if(Mid_Line[m]<0)
            {
                Mid_Line[m]=0;
            }
            err_get[m]=Mid_Line[m];                //因为误差的获取已经在上面的代码里做过了，所以这里的拉线完需要再获取一下更改完的误差
        }
        // for(int i = 0; i <=error_flag; i++)
        // {
            
        //     err_get[i]=1.5*image_w;
        // }

    }

    // if(error_flag!=0 && error_flag_dir==0 && turn_flag==0)
    if(  Left_Leader_count>18 && Right_Leader_count<20 && Top_Leader_lost >= 5)                 //左转直角判断拉线（与右转相同）
    {
        k_2=(0-Mid_Line[start_mideline])/(float)(5-start_mideline);
        for(int m = start_mideline; m >=5; m--)
        {
            Mid_Line[m]=(int)(k_2*(m-start_mideline)+Mid_Line[start_mideline]);
            if(Mid_Line[m]>image_w)
            {
                Mid_Line[m]=image_w;
            }
            if(Mid_Line[m]<0)
            {
                Mid_Line[m]=0;
            }
            err_get[m]=Mid_Line[m];
        }
        // for(int i = 0; i <=error_flag; i++)
        // {
        //     err_get[i]=-0.5*image_w;
        // }
    }








/*************************** 另一种圆环进环方式，拉线进环，也研究了很久，最终没用上，舍不得删就保留下来了，可以自己研究研究，感觉这个弄好了比固定误差要方便 ***********************/
    // if(turn_flag==3)
    // {
    //     if(huandao_state==1 && l_founder[L_leader_states[1]]<50)
    //     {
    //         k_2=(L_leader_states[1]-Mid_Line[start_mideline])/(float)(l_founder[L_leader_states[1]]-start_mideline);
    //         for(int m = start_mideline; m >=l_founder[L_leader_states[1]]; m--)
    //         {
    //             Mid_Line[m]=(int)(k_2*(m-start_mideline)+Mid_Line[start_mideline]);
    //             if(Mid_Line[m]>image_w)
    //             {
    //                 Mid_Line[m]=image_w;
    //             }
    //             if(Mid_Line[m]<0)
    //             {
    //                 Mid_Line[m]=0;
    //             }
    //             err_get[m]=Mid_Line[m];
    //         }

    //     }

    // }



    // if(turn_flag==4 )
    // {
    //     if(huandao_state==1 && l_founder[R_leader_states[Right_Leader_count-1]]<50)
    //     {
    //         k_2=(R_leader_states[Right_Leader_count-1]-Mid_Line[start_mideline])/(float)(l_founder[R_leader_states[Right_Leader_count-1]]-start_mideline);
    //         for(int m = start_mideline; m >=l_founder[R_leader_states[Right_Leader_count-1]]; m--)
    //         {
    //             Mid_Line[m]=(int)(k_2*(m-start_mideline)+Mid_Line[start_mideline]);
    //             if(Mid_Line[m]>image_w)
    //             {
    //                 Mid_Line[m]=image_w;
    //             }
    //             if(Mid_Line[m]<0) 
    //             {
    //                 Mid_Line[m]=0;
    //             }
    //             err_get[m]=Mid_Line[m];
    //         }

    //     }

    // }
 

    /*遍历一遍数据寻找赛宽大于50的直角误差*/

                       //右丢线累加   
}


/*************************  元素检测函数 ******************///（一开始把直角当元素跑用来判断直角，后面懒得改名了就一直用这个，里面放的是所有元素的判断）
//停车，圆环，断路都在里面
void angle90_detect()        
{
/*停车判断*/
//停车判断的基本思路（所需要的标志位）
//停车检测赛宽数（大于30的赛宽数）大于某个值
//左右都有引导线，要大于某个值，但是加起来又不能太大，不可能充满图像左右两边
//通过编码器积分距离来增加表示的标志位的情况，判断到停车往前走一点，此时的图像就判断不到停车线了，这时候就进下一个状态，一圈回来又判断到了再重复
     if(tingche_test>=10 && Left_Leader_count>10 && Right_Leader_count>10 && tingche_state==0 && (Left_Leader_count+Right_Leader_count)<60)
     {
         tingche_state=1;                //开机检测到停车线
         turn_flag=1;
         current_replay_page=4;          //惯导读取的flash页标志位清零，详见ins.c
         yuanhuan_count=0;
        
     }
     if(tingche_state==1 && !(tingche_test>=10 && Left_Leader_count>10 && Right_Leader_count>10 && (Left_Leader_count+Right_Leader_count)<60) && total_distance>6000)
     {
         tingche_state=2;                //第一圈行驶中
         turn_flag=0;
         
     }
     if(tingche_test>=10 && Left_Leader_count>10 && Right_Leader_count>10 && tingche_state==2 && (Left_Leader_count+Right_Leader_count)<60)
     {
         tingche_state=3;                 //第二圈开始检测到停车线
         turn_flag=1;
         current_replay_page=4;           //惯导读取的flash页标志位清零，详见ins.c
         yuanhuan_count=0;
     }
     if(tingche_state==3 && !(tingche_test>=10 && Left_Leader_count>10 && Right_Leader_count>10 && (Left_Leader_count+Right_Leader_count)<60) && total_distance>6000)
     {
         tingche_state=4;                 //第二圈行驶中
         turn_flag=0;
     }
     if(tingche_test>=10 && Left_Leader_count>10 && Right_Leader_count>10 && tingche_state==4 && (Left_Leader_count+Right_Leader_count)<60)
     {
         tingche_state=5;                  //执行停车指令
         turn_flag=1;
         current_replay_page = 4;    //惯导读取的flash页标志位清零，详见ins.c
         yuanhuan_count=0;
    }



    // if(turn_flag==0 && Left_Leader_lost>40 && Right_Leader_lost<20 && Both_Lost_count>30)     //右转直角弯判断
    // {
    //     turn_flag=1;
    // }

    // if(turn_flag==0 && Left_Leader_lost<20 && Right_Leader_lost>40 && Both_Lost_count>30)     //左转直角弯判断
    // {
    //     turn_flag=2;
    // }


//(int)((image_w-Mid_Line[start_mideline])*2/3.0)


/**************************************************************************  圆环判断 **************************************************************************/
/*
圆环判断的基本思路：左丢线，右不丢线，左没有引导线，右有引导线，图像上方（总Both_Lost_count）不丢线（有待商榷，碰到虚线会出问题），圆环两行赛宽数（circle_wide_flag）大于某个值，即为右圆环，左圆环与右圆环同思路



*/
/**********************************************  左圆环判断 *****************************************************/
    if(turn_flag==0 && Left_Leader_lost<17 && Right_Leader_lost>20 && Right_Leader_count<5 &&Left_Leader_count>10 && Both_Lost_count<10 && circle_wide_flag>20 && circle_flag!=4)
    {
        circle_flag=3;
        turn_flag=2;
        // if(l_founder[L_leader_states[Left_Leader_count-4]]>45)
        // {
        //     turn_flag=3;   
        //     T_N=0; 
        //     total_distance=0;


        // }
                                                                             //左圆环判断
    }




/**********************************************  右圆环判断 *****************************************************/
    if(turn_flag==0 && Left_Leader_lost>20 && Right_Leader_lost<17 && Left_Leader_count<5 && Right_Leader_count>10 && Both_Lost_count<10 && circle_wide_flag>20 && circle_flag!=3)
    {
        circle_flag=4;
        turn_flag=2;
        // if((l_founder[R_leader_states[4]]>45))
        // {
        //     turn_flag=4;    
        //     T_N=0; 
        //     total_distance=0;   
        // }
                                                                            //右圆环判断
    }






//断路判断，简单的丢线判断
    if(Both_Lost_count>50 && mode!=3 && botton_leader_lost>15 && record_mode!=1 && huandao_state==0 && mode==1)
    {
        mode=3;
        turn_flag=5;

        // if(mode!=3){
        // i_Track=0;
        // T_N=0;
        // total_distance=0;}
        // if(mode!=2)mode=3;
    }
    
    if(turn_flag==3 && T_N>-320 && yuanhuan_count==0)
    {
        offset=offset_circle1;
        // vol_now=max_vol;
    }
    if(turn_flag==3 && T_N>-320 && yuanhuan_count==1)
    {
        offset=offset_circle2;
        // vol_now=max_vol;
    }
    if(turn_flag==4 && T_N<320 && yuanhuan_count==0)
    {
        offset=-offset_circle1;
        // vol_now=max_vol;
    }
    if(turn_flag==4 && T_N<320 && yuanhuan_count==1)
    {
        offset=-offset_circle2;
        // vol_now=max_vol;
    }
    if(yuanhuan_count==0 && huandao_state==4)
    {
        yuanhuan_count=1;
        // vol_now=vol;
    }

    if(yuanhuan_count==1 && huandao_state==3)
    {
        yuanhuan_count=2;

    }

    if(yuanhuan_count==2 && huandao_state==2)
    {
        yuanhuan_count=3;
    }
    // if(yuanhuan_count==3 && current_replay_page==4)
    // {
    //     // vol_now=200;
    //     // BMI270_G. Kp=20;
    //     // BMI270_G. Kd=15.6;
    // }

    if(Both_Lost_count<50&&turn_flag==5 && botton_leader_lost<5 && mode==3)//出断路需要底部丢线数少，图像底部是赛道才能避免误判串道
    {
        turn_flag=0;
        mode=1;
        replay_stop=1;
        // vol_now=pid_number[0];
        // BMI270_G. Kp=pid_number[4];
        // BMI270_G. Kd=pid_number[6];
    }

}



float err_quanzhong[60]={
    2.08, 2.06, 2.04, 2.02, 2.0, 
    1.98, 1.96, 1.94, 1.92, 1.9, 
    1.88, 1.86, 1.84, 1.82, 1.8, 
    1.78, 1.76, 1.74, 1.72, 1.7, 
    1.68, 1.66, 1.64, 1.62, 1.6, 
    1.58, 1.56, 1.54, 1.52, 1.5, 
    1.48, 1.46, 1.44, 1.42, 1.4, 
    1.38, 1.36, 1.34, 1.32, 1.3, 
    1.28, 1.26, 1.24, 1.22, 1.2, 
    1.18, 1.16, 1.14, 1.12, 1.1, 
    1.08, 1.06, 1.04, 1.02, 1.0,
    1.00, 1.00, 1.00, 1.00, 1.0, 
    
};

int err_count;
float err;
int camera_err()
{
    err=0;
    err_count=0;
    for(int i=10;i<45;i++)
    {
        err+=(err_get[i]-image_w/2);
        if(err_get[i]==image_w/2)
        {
            err_count+=1;
        }
    }
    if(err_count>15 || turn_flag!=0)
    {
        for(int i=45;i<58;i++)
        {
            err+=(err_get[i]-image_w/2);
        }
        // err+=((err_get[46]+err_get[47]+err_get[48]+err_get[49]+err_get[50]+err_get[51]+err_get[52]+err_get[53]+err_get[54]+err_get[55]+err_get[56]+err_get[57]+err_get[58])-(image_w/2)*13);
        err/=10;
    }

    else
    {
        err/=10;
    }

    return (int)err;
}



