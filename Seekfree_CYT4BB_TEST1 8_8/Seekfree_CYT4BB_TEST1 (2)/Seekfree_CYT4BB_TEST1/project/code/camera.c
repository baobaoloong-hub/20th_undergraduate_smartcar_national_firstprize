// #include "camera.h"

// //uint8 mt9v03x_image[MT9V03X_H][MT9V03X_W];     

// //todo

// uint8_t longest_x=0;

// void longest_line(){
//     uint8_t x_buf;

//     for (int i=0;i>MT9V03X_W-1;i++)//列遍历
//     {
//         for(int j=MT9V03X_H-5;j<0;i--)//行遍历
//         {
//             if(((mt9v03x_image[j][i]-mt9v03x_image[j-2][i])/((mt9v03x_image[j][i]+mt9v03x_image[j-2][i])*1.00))>0.25)
//             {
//                 x_buf=i;              
//                 ips200_draw_point(x_buf, 115, RGB565_RED);//显示起点 显示中线
//                 break;
//             }
//         }
//     }

    

// }





// //     *************************
// //     *//-1, 1 //0, 1 //1, 1//*
// //     *//-1, 0 //     //1, 0//*
// //     *//-1,-1 //0,-1 //1,-1//*
// //     *************************
// uint8 weight_l[8][2]={
//     {0, 1},
//     {1, 1},
//     {1,0},
//     {1, -1},
//     {0,-1},
//     {-1,-1},
//     {-1,0},
//     {-1,1},

// };
// uint8 weight_r[8][2]={
//     {0, 1},
//     {-1,1},
//     {-1,0},
//     {-1,-1},
//     {0,-1},
//     {1,-1},
//     {1, 0},
//     {1, 1}
// };


// #define start_line 115
// #define start_l 1
// #define start_r MT9V03X_W-2

// #define num_edge 1000
// // uint8_t start_piont[2][2];//左右两个起始点的xy坐标
// // uint8_t L_edge[num_edge][2];//左边线
// // uint8_t R_edge[num_edge][2];//

// coord L[10000];//左边边线,最多100个点
// coord R[10000];


// #define start_dir 
// uint8_t found_y_or_n = 0;
// uint16_t err_count = 0;
// uint16_t num_dir[9]={0};

// void balinyu(){
//     //找起始点
//     //左
//     for (int i=start_l;i<MT9V03X_W-1;i++){
//         if(((bin_image[start_line][i+1]-bin_image[start_line][i])/((bin_image[start_line][i]+bin_image[start_line][i+1])*1.00))>0.25)
//         {
//             L[0].x=start_line;
//             L[0].y=i+1;
//             ips200_draw_point(L[0].y, L[0].x, RGB565_RED);//显示起点 显示中线
//             found_y_or_n = 1;
//             break;
//         }
//     }
//     //右
//     for (int i=start_r;i>0;i--){
//         if(((bin_image[start_line][i-1]-bin_image[start_line][i])/((bin_image[start_line][i]+bin_image[start_line][i-1])*1.00))>0.25)
//         {
//             R[0].x=start_line;
//             R[0].y=i-1;
//             ips200_draw_point(R[0].y, R[0].x, RGB565_RED);//显示起点 显示中线
//             found_y_or_n = 1;
//             break;
//         }
//     }
//     //找到起始点(种子),开始生长
//     // L_edge[0][0]=start_piont[0][0];
//     // R_edge[0][0]=start_piont[0][1];
//     //左,顺
//     uint16_t count_now=0,count_next=1;
//     uint8_t x_buf,y_buf,x_next,y_next;
//     uint8_t L_dir=2,R_dir=2;

// memset(num_dir,0,sizeof(num_dir));

// while(L[count_now].x>1&&L[count_now].y>1&&L[count_now].x<116&&L[count_now].y<187&&found_y_or_n == 1&&err_count<1000)
// {
//     for (int i = 0; i < 8; i++) 
//     {
        
//         L_dir=((count_now==0)?2: L[count_now - 1].dir);
//         x_buf=L[count_now].x+weight_l[(L_dir+4+i)%8][0];
//         y_buf=L[count_now].y+weight_l[(L_dir+4+i)%8][1];
//         x_next=L[count_now].x+weight_l[(L_dir+5+i)%8][0];
//         y_next=L[count_now].y+weight_l[(L_dir+5+i)%8][1];
//         if(((bin_image[x_next][y_next]-bin_image[x_buf][y_buf])/((bin_image[x_next][y_next]+bin_image[x_buf][y_buf])*1.00))>0.1)
//         {
//             // if(L[count_now].dir==(L_dir+4+i)%8){found_y_or_n=0;break;}

//             found_y_or_n = 1;
//             L_dir=(L_dir+5+i)%8;
//             num_dir[L_dir]++;
//             L[count_now + 1].dir = L_dir;
//             L[count_now + 1].x = L[count_now].x+weight_l[L_dir][0];
//             L[count_now + 1].y = L[count_now].y+weight_l[L_dir][1];
//             L[count_now + 1].next = &L[count_now + 2];
//             ips200_draw_point(L[count_now+1].y, L[count_now+1].x, RGB565_PURPLE); 
//             count_now++;break;

//         }
//         else found_y_or_n =0;
//         // err_count=0;
//         // count_now++;

//     }
//     err_count++;
// }

// err_count=0;
// found_y_or_n = 1;
// count_now=0;

// while(R[count_now].x>1&&R[count_now].y>1&&R[count_now].x<116&&R[count_now].y<187&&found_y_or_n == 1&&err_count<1000)
// {
    
//     for (int i = 0; i < 8; i++) 
//     {
//         R_dir=((count_now==0)?6: R[count_now - 1].dir);
//         x_buf=R[count_now].x+weight_r[(R_dir+4+i)%8][0];
//         y_buf=R[count_now].y+weight_r[(R_dir+4+i)%8][1];
//         x_next=R[count_now].x+weight_r[(R_dir+5+i)%8][0];
//         y_next=R[count_now].y+weight_r[(R_dir+5+(i))%8][1];
//         if(((bin_image[x_next][y_next]-bin_image[x_buf][y_buf])/((bin_image[x_next][y_next]+bin_image[x_buf][y_buf])*1.00))>0.1)
//         {
//             // if(R[count_now].dir==(R_dir+4+i)%8){found_y_or_n=0;break;}

//             found_y_or_n = 1;
//             R_dir=(R_dir+5+(i))%8;
//             num_dir[R_dir]++;
//             R[count_now + 1].dir = R_dir;
//             R[count_now + 1].x = R[count_now].x+weight_r[R_dir][0];
//             R[count_now + 1].y = R[count_now].y+weight_r[R_dir][1];
//             R[count_now + 1].next = &R[count_now + 2];
//             ips200_draw_point(R[count_now+1].y, R[count_now+1].x, RGB565_GREEN); 
//             count_now++;break;

//         }
//         // count_now++;
//         else found_y_or_n = 0;
//         // err_count=0;

//     }
//     err_count++;
// }


// }






// //////////////////////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////////////////////

// uint8 image_thereshold=150;
// uint8 bin_image[MT9V03X_H][MT9V03X_W];//图像数组
// void turn_to_bin(void)
// {
//   uint8 i,j;
//  image_thereshold = otsuThreshold(mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
//  if(image_thereshold<130)image_thereshold=130;

//   for(i = 0;i<MT9V03X_H;i++)
//   {
//       for(j = 0;j<MT9V03X_W;j++)
//       {
//           if(mt9v03x_image[i][j]>image_thereshold)bin_image[i][j] = 255;
//          else bin_image[i][j] = 0;
//      }
//   }
// //    Get_Threshold(image_thereshold,original_image[0]); // /70
// }

// uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row)
// {
// #define GrayScale 256
//     uint16 MT9V03X_Width  = col;
//     uint16 MT9V03X_Height = row;
//     int X; uint16 Y;
//     uint8* data = image;
//     int HistGram[GrayScale] = {0};

//     uint32 Amount = 0;
//     uint32 PixelBack = 0;
//     uint32 PixelIntegralBack = 0;
//     uint32 PixelIntegral = 0;
//     int32 PixelIntegralFore = 0;
//     int32 PixelFore = 0;
//     double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // 类间方差;
//     uint8 MinValue=0, MaxValue=0;
//     uint8 Threshold = 0;


//     for (Y = 0; Y <MT9V03X_Height; Y++) //Y<MT9V03X_Height改为Y =MT9V03X_Height；以便进行 行二值化
//     {
//         //Y=MT9V03X_Height;
//         for (X = 0; X < MT9V03X_Width; X++)
//         {
//         HistGram[(int)data[Y*MT9V03X_Width + X]]++; //统计每个灰度值的个数信息
//         }
//     }




//     for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
//     for (MaxValue = 255; MaxValue > MinValue && HistGram[MaxValue] == 0; MaxValue--) ; //获取最大灰度的值

//     if (MaxValue == MinValue)
//     {
//         return MaxValue;          // 图像中只有一个颜色
//     }
//     if (MinValue + 1 == MaxValue)
//     {
//         return MinValue;      // 图像中只有二个颜色
//     }

//     for (Y = MinValue; Y <= MaxValue; Y++)
//     {
//         Amount += HistGram[Y];        //  像素总数
//     }

//     PixelIntegral = 0;
//     for (Y = MinValue; Y <= MaxValue; Y++)
//     {
//         PixelIntegral += HistGram[Y] * Y;//灰度值总数
//     }
//     SigmaB = -1;
//     for (Y = MinValue; Y < MaxValue; Y++)
//     {
//           PixelBack = PixelBack + HistGram[Y];    //前景像素点数
//           PixelFore = Amount - PixelBack;         //背景像素点数
//           OmegaBack = (double)PixelBack / Amount;//前景像素百分比
//           OmegaFore = (double)PixelFore / Amount;//背景像素百分比
//           PixelIntegralBack += HistGram[Y] * Y;  //前景灰度值
//           PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
//           MicroBack = (double)PixelIntegralBack / PixelBack;//前景灰度百分比
//           MicroFore = (double)PixelIntegralFore / PixelFore;//背景灰度百分比
//           Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//g
//           if (Sigma > SigmaB)//遍历最大的类间方差g
//           {
//               SigmaB = Sigma;
//               Threshold = (uint8)Y;
//           }
//     }
//    return Threshold;
// }

// #define threshold_max   255*5//此参数可根据自己的需求调节
// #define threshold_min   255*2//此参数可根据自己的需求调节
// void image_filter(uint8(*bin_image)[MT9V03X_W])//形态学滤波，简单来说就是膨胀和腐蚀的思想
// {
//     uint16 i, j;
//     uint32 num = 0;


//     for (i = 1; i < MT9V03X_H - 1; i++)
//     {
//         for (j = 1; j < (MT9V03X_W - 1); j++)
//         {
//             //统计八个方向的像素值
//             num =
//                 bin_image[i - 1][j - 1] + bin_image[i - 1][j] + bin_image[i - 1][j + 1]
//                 + bin_image[i][j - 1] + bin_image[i][j + 1]
//                 + bin_image[i + 1][j - 1] + bin_image[i + 1][j] + bin_image[i + 1][j + 1];


//             if (num >= threshold_max && bin_image[i][j] == 0)
//             {

//                 bin_image[i][j] = 255;//白  可以搞成宏定义，方便更改

//             }
//             if (num <= threshold_min && bin_image[i][j] == 255)
//             {

//                 bin_image[i][j] = 0;//黑

//             }

//         }
//     }

// }

// //////////////////////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////////////////////



















