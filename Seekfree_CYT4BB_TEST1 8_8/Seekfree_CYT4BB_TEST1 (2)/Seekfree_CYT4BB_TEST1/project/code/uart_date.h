#ifndef _UART_DATE_H_
#define _UART_DATE_H_
 
#include "zf_common_headfile.h"

extern int8 left_aim;
extern int8 right_aim;

//** 串口初始化,必须调用 */
void uart_date_init();

/*/////////////////////////////////////////////////////
    发送区域
*//////////////////////////////////////////////////////

//** 发送数据包 */
void uart_send();
void send_data();
/*/////////////////////////////////////////////////////
    接收区域
*//////////////////////////////////////////////////////

//** 串口接收中断处理函数,放在中断函数中*/
void uart_rx_interrupt_handler ();


//** 将字符串转换为浮点数 */
double string_to_float (const uint8 fifo_get_data[]);
float unpack_int16_from_uint8(const uint8_t* buffer);
void pack_int16_to_uint8(int16 fdata, uint8* buffer) ;
    float unpack_float_from_uint8(const uint8_t* buffer) ;
void pack_float_to_uint8(float fdata, uint8_t* buffer) ;






// // 鍋囪?惧浘鍍忓ぇ灏忎负 width x height
// #define WIDTH 188
// #define HEIGHT 120
// void send_gray_image(unsigned char image[HEIGHT][WIDTH]) ;




// 定义联合体
union FloatConverter {
    float f;        // 4字节的float类型
    uint8_t bytes[4];  // 4个字节的数组
};

// 字节数组转float
float bytesToFloat(uint8_t* bytes);

// float转字节数组
void floatToBytes(float value, uint8_t* bytes);

// 定义short转换联合体
union ShortConverter {
    int16_t s;      // 2字节的short类型
    uint8_t bytes[2];  // 2个字节的数组
};

// 字节数组转short
int16_t bytesToShort(uint8_t* bytes);

// short转字节数组
void shortToBytes(int16_t value, uint8_t* bytes);

// VOFA+相关函数
void vofa_send_float(float data);
void vofa_send_float_array(float* data, uint8_t length);
void vofa_send_end(void);

#endif