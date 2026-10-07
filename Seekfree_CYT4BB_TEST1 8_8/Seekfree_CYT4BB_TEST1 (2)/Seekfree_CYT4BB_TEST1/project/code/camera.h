// #ifndef _CAMERA_H_
// #define _CAMERA_H_

// #include "zf_common_headfile.h"

// typedef struct coord_info{
//     // uint16 index;//结构体数组,索引隐去
//     uint8 x;
//     uint8 y;
//     uint8 dir;
//     struct coord_info* next;

// }coord;


// extern coord L[10000];//左边边线,最多100个点
// extern coord R[10000];

// extern uint8 image_thereshold;
// extern uint8 bin_image[MT9V03X_H][MT9V03X_W];//图像数组


// //////////////////////////////////////////////////////////////////////////////////////
// void turn_to_bin(void);
// uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row);
// void image_filter(uint8(*bin_image)[MT9V03X_W]);


// //////////////////////////////////////////////////////////////////////////////////////
// extern uint16_t num_dir[9];


// void balinyu();
// void longest_line();

// #endif