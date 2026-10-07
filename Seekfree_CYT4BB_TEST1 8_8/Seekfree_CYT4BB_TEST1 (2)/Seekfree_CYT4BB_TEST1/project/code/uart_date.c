#include "uart_date.h"

#define UART_INDEX              (UART_1   )                           // 默认 UART_0
#define UART_BAUDRATE           (115200)                           // 默认 115200
#define UART_TX_PIN             (UART1_TX_P04_1  )                           // 默认 UART0_TX_P00_1
#define UART_RX_PIN             (UART1_RX_P04_0  )                           // 默认 UART0_RX_P00_0



#define UART_HEADER              (0xA5)                           // 发送包头
#define UART_TAIL                (0x5A)                           // 发送包尾

// #define UART_RECEVIER_UART_INDEX            (UART_2)              // 定义串口接收机使用的串口                               
// #define UART_RECEVIER_TX_PIN                (UART2_RX_P07_0)    // 遥控器接收机没有这个引脚，仅用于串口初始化时占位使用      
// #define UART_RECEVIER_RX_PIN                (UART2_TX_P07_1)    // 串口接收机的TX引脚 连接单片机的RX引脚                    
// #define UART_RECEVIER_COUNTER               (TC_TIME2_CH2)      // 定义遥控器接收间隔计数器通道                             
// int8 left_aim=0;
// int8 right_aim=0;

uint8 uart_get_data[64];                                                        // 串口接收数据缓冲区
uint8 fifo_get_data[64];                                                        // fifo 输出读出缓冲区

uint8  get_data = 0;                                                            // 接收数据变量
uint32 fifo_data_count = 0;                                                     // fifo 数据个数

fifo_struct uart_data_fifo;
fifo_struct uart_datapack1_fifo;

//** 串口初始化,必须调用 */
void uart_date_init()
{
fifo_init(&uart_data_fifo, FIFO_DATA_8BIT, uart_get_data, 64);              // 初始化 fifo 挂载缓冲区
uart_init(UART_INDEX, UART_BAUDRATE, UART_TX_PIN, UART_RX_PIN);             // 初始化串口
uart_rx_interrupt(UART_INDEX, 1);                                           // 开启 UART_INDEX 的接收中断
}
// uart_write_string(UART_INDEX, "UART Text.");                                // 输出测试信息
// uart_write_byte(UART_INDEX, '\r');                                          // 输出回车
// uart_write_byte(UART_INDEX, '\n');                                          // 输出换行

/*
    发送区域
*/

unsigned char Tx_buffer[64];  // 创建发送缓冲区

//** 发送数据包 */
uint8 TX_buf[7]={UART_HEADER,0,0,0,0,0,UART_TAIL};

void uart_send(){
    int i, j;

    uint8 left_buf[2]={0};
    uint8 right_buf[2]={0};

    // pack_int16_to_uint8(left_encoder,left_buf);
    // pack_int16_to_uint8(right_encoder,right_buf);

    TX_buf[1]=left_buf[0];
    TX_buf[2]=left_buf[1];
    TX_buf[3]=right_buf[0];
    TX_buf[4]=right_buf[1];
    TX_buf[5]=TX_buf[1]+TX_buf[2]+TX_buf[3]+TX_buf[4];
    // uart_write_string(UART_INDEX, TX_buf);                                // 输出测试信息
    for (i = 0; i < 7; i++) {
        uart_write_byte (UART_INDEX, TX_buf[i]);
    }

}




// // 数据打包相关全局变量
// unsigned char *valuepack_tx_buffer;    // 发送缓冲区指针
// unsigned short valuepack_tx_index;     // 发送缓冲区当前索引
// unsigned char valuepack_tx_bit_index;  // 当前字节中的位索引
// unsigned char valuepack_stage;         // 数据打包阶段标志

// /**
//  * @brief 开始数据打包过程
//  * @param buffer 用于存储打包数据的缓冲区
//  */
// void startValuePack(unsigned char *buffer)
// {
//     valuepack_tx_buffer = buffer;
//     valuepack_tx_index = 1;           // 从索引1开始，索引0用于存储包头
//     valuepack_tx_bit_index = 0;       // 位索引初始化为0
//     valuepack_tx_buffer[0] = UART_HEADER; // 写入包头标识
//     valuepack_stage = 0;              // 重置打包阶段
// }

// void ValuePacking()
// {
//     // 按小端序存储短整型数据
//     valuepack_tx_index=1;//encoder_l
//     valuepack_tx_buffer[valuepack_tx_index] = ((short)encoder_data_quad[0])  &0xff;
//     valuepack_tx_buffer[valuepack_tx_index+1] = ((short)encoder_data_quad[0])  >>8;
//     valuepack_tx_index=3;//encoder_r
//     valuepack_tx_buffer[valuepack_tx_index] = ((short)encoder_data_quad[0])  &0xff;
//     valuepack_tx_buffer[valuepack_tx_index+1] = ((short)encoder_data_quad[1])  >>8;
//     valuepack_tx_index=5;

// }

// /**
//  * @brief 结束数据打包过程
//  * @return 返回数据包的总长度
//  */
// unsigned short endValuePack()
// {
//     unsigned char sum=0;
//     // 计算校验和
//     for(int i=1;i<valuepack_tx_index;i++)
//     {
//         sum+=valuepack_tx_buffer[i];
//     }
//     valuepack_tx_buffer[valuepack_tx_index] = sum;        // 添加校验和
//     valuepack_tx_buffer[valuepack_tx_index+1] = UART_TAIL; // 添加包尾标识
//     return valuepack_tx_index+2;  // 返回总长度（包括校验和和包尾）
// }

// /**
//  * @brief 发送数据缓冲区
//  * @param p 要发送的数据缓冲区指针
//  * @param length 要发送的数据长度
//  */
// void sendBuffer(unsigned char *p, unsigned short length)
// {
//     for(int i=0;i<length;i++)
//     { 
//         uart_write_byte(UART_INDEX, *p++); 
//     }
// }

// void send_data(){
//     startValuePack(Tx_buffer);
//     ValuePacking();
//     endValuePack();
//     sendBuffer(Tx_buffer,);
// }


/*
    接收区域
*/
uint16_t vol=0;
float vol_now=0;
float circle_distance_enter1;
float circle_distance_enter2;
float circle_distance_out;
void uart_rx_interrupt_handler (void)
{
//    get_data = uart_read_byte(UART_INDEX);                                      // 读取数据流 while 循环中 避免 缓冲区溢出
    if(uart_query_byte(UART_INDEX, &get_data))                                  // 读取数据流 查询 如果 有数据 TRUE 否则 FALSE
    {
        fifo_write_buffer(&uart_data_fifo, &get_data, 1);                       // 写入数据到 fifo 中
    }


    if(get_data==0x5A){
        fifo_data_count = fifo_used(&uart_data_fifo);                           // 获取 fifo 中的数据
        if(fifo_data_count != 0)                                                // 如果数据不为空
        {
            fifo_read_buffer(&uart_data_fifo, fifo_get_data, &fifo_data_count, FIFO_READ_AND_CLEAN);    // 从 fifo 中读取数据并清空 fifo 缓冲区
            // uart_write_string(UART_INDEX, "\r\nUART get data:");                // 打印接收到的数据
            // uart_write_buffer(UART_INDEX, fifo_get_data, fifo_data_count);      // 打印接收到的数据
            // camera.Kp=fifo_get_data[1];//if(camera.Out>127)camera.Out=camera.Out-256;
            // camera.Kp*=0.1;

            // camera.Kd=fifo_get_data[2];if( camera.Kd>127) camera.Kd= camera.Kd-256;
            // camera.Kd*=0.01;

            // KP2_err.Kp=fifo_get_data[3];//if(KP2_err.Kp>127)KP2_err.Kp=KP2_err.Kp-256;
            // KP2_err.Kp*=0.001;

            // BMI270_G.Kp=fifo_get_data[4];if(BMI270_G.Kp>127)BMI270_G.Kp=BMI270_G.Kp-256;
            // BMI270_G.Kp*= 0.1;
            // BMI270_G.Kd=fifo_get_data[5];if(BMI270_G.Kd>127)BMI270_G.Kd=BMI270_G.Kd-256;
            // BMI270_G.Kd*= 0.1;

            // vol=10*fifo_get_data[6];

            union ShortConverter voll;
            voll.bytes[0] = fifo_get_data[1];
            voll.bytes[1] = fifo_get_data[2];
            vol= voll.s;
pid_number[0]=vol;
            union FloatConverter e_kp;
            e_kp.bytes[0] = fifo_get_data[3];
            e_kp.bytes[1] = fifo_get_data[4];
            e_kp.bytes[2] = fifo_get_data[5];
            e_kp.bytes[3] = fifo_get_data[6];
            camera.Kp= e_kp.f;
pid_number[1]=camera.Kp;
            union FloatConverter e_ki;
            e_ki.bytes[0] = fifo_get_data[7];
            e_ki.bytes[1] = fifo_get_data[8];
            e_ki.bytes[2] = fifo_get_data[9];
            e_ki.bytes[3] = fifo_get_data[10];
            camera.Ki= e_ki.f;
pid_number[2]=camera.Ki;

            union FloatConverter e_kd;
            e_kd.bytes[0] = fifo_get_data[11];
            e_kd.bytes[1] = fifo_get_data[12];
            e_kd.bytes[2] = fifo_get_data[13];
            e_kd.bytes[3] = fifo_get_data[14];
            camera.Kd= e_kd.f;
pid_number[3]=camera.Kd;
            //e2
            union FloatConverter e2_kp;
            e2_kp.bytes[0] = fifo_get_data[15];
            e2_kp.bytes[1] = fifo_get_data[16];
            e2_kp.bytes[2] = fifo_get_data[17];
            e2_kp.bytes[3] = fifo_get_data[18];
            circle_distance_enter1= e2_kp.f;
pid_number[14]=circle_distance_enter1;
            // union FloatConverter e2_ki;
            // e2_ki.bytes[0] = fifo_get_data[19];
            // e2_ki.bytes[1] = fifo_get_data[20];
            // e2_ki.bytes[2] = fifo_get_data[21];
            // e2_ki.bytes[3] = fifo_get_data[22];
            // KP2_err.Ki= e2_ki.f;

            // union FloatConverter e2_kd;
            // e2_kd.bytes[0] = fifo_get_data[23];
            // e2_kd.bytes[1] = fifo_get_data[24];
            // e2_kd.bytes[2] = fifo_get_data[25];
            // e2_kd.bytes[3] = fifo_get_data[26];
            // KP2_err.Kd= e2_kd.f;

            //G
            union FloatConverter G_kp;
            G_kp.bytes[0] = fifo_get_data[19];
            G_kp.bytes[1] = fifo_get_data[20];
            G_kp.bytes[2] = fifo_get_data[21];
            G_kp.bytes[3] = fifo_get_data[22];
            BMI270_G.Kp= G_kp.f;
pid_number[4]=BMI270_G.Kp;
            union FloatConverter G_ki;
            G_ki.bytes[0] = fifo_get_data[23];
            G_ki.bytes[1] = fifo_get_data[24];
            G_ki.bytes[2] = fifo_get_data[25];
            G_ki.bytes[3] = fifo_get_data[26];
            BMI270_G.Ki= G_ki.f;
pid_number[5]=BMI270_G.Ki;
            union FloatConverter G_kd;
            G_kd.bytes[0] = fifo_get_data[27];
            G_kd.bytes[1] = fifo_get_data[28];
            G_kd.bytes[2] = fifo_get_data[29];
            G_kd.bytes[3] = fifo_get_data[30];
            BMI270_G.Kd= G_kd.f;
pid_number[6]=BMI270_G.Kd;
            //l
            union FloatConverter l_kp;
            l_kp.bytes[0] = fifo_get_data[31];
            l_kp.bytes[1] = fifo_get_data[32];
            l_kp.bytes[2] = fifo_get_data[33];
            l_kp.bytes[3] = fifo_get_data[34];
            Divert. Kp= l_kp.f;
pid_number[7]=Divert. Kp;
            union FloatConverter l_ki;
            l_ki.bytes[0] = fifo_get_data[35];
            l_ki.bytes[1] = fifo_get_data[36];
            l_ki.bytes[2] = fifo_get_data[37];
            l_ki.bytes[3] = fifo_get_data[38];
            Divert. Ki= l_ki.f;
pid_number[8]=Divert. Ki;
            union FloatConverter l_kd;
            l_kd.bytes[0] = fifo_get_data[39];
            l_kd.bytes[1] = fifo_get_data[40];
            l_kd.bytes[2] = fifo_get_data[41];
            l_kd.bytes[3] = fifo_get_data[42];
            Divert. Kd= l_kd.f;
pid_number[9]=Divert. Kd;
            //r
            union FloatConverter r_kp;
            r_kp.bytes[0] = fifo_get_data[43];
            r_kp.bytes[1] = fifo_get_data[44];
            r_kp.bytes[2] = fifo_get_data[45];
            r_kp.bytes[3] = fifo_get_data[46];
            circle_distance_out= r_kp.f;
pid_number[10]=circle_distance_out;
            union FloatConverter r_ki;
            r_ki.bytes[0] = fifo_get_data[47];
            r_ki.bytes[1] = fifo_get_data[48];
            r_ki.bytes[2] = fifo_get_data[49];
            r_ki.bytes[3] = fifo_get_data[50];
            KP2_err.Kp= r_ki.f;
pid_number[11]=speed_r.Ki;
            union FloatConverter r_kd;
            r_kd.bytes[0] = fifo_get_data[51];
            r_kd.bytes[1] = fifo_get_data[52];
            r_kd.bytes[2] = fifo_get_data[53];
            r_kd.bytes[3] = fifo_get_data[54];
            // speed_r.Kd= r_kd.f;
pid_number[12]=speed_r.Kd;
            union FloatConverter G;
            G.bytes[0] = fifo_get_data[55];
            G.bytes[1] = fifo_get_data[56];
            G.bytes[2] = fifo_get_data[57];
            G.bytes[3] = fifo_get_data[58];
            offset_circle1= G.f;
pid_number[13]=offset_circle1;
            vol_now=vol;
// pid_init();
        }
        // ips200_show_float(80,14*16,left_aim,3,2);
    }


}



/////////////////////end



// /**
//  * @brief 向数据包中放入布尔值
//  * @param b 要打包的布尔值
//  */
// void putBool(unsigned char b)
// {
//     if(valuepack_stage<=1)
//     {
//         // 根据布尔值设置对应位
//         if(b)
//             valuepack_tx_buffer[valuepack_tx_index] |= 0x01<<valuepack_tx_bit_index;
//         else
//             valuepack_tx_buffer[valuepack_tx_index] &= ~(0x01<<valuepack_tx_bit_index);

//         valuepack_tx_bit_index++;
//         // 如果当前字节已满，移动到下一个字节
//         if(valuepack_tx_bit_index>=8)
//         {
//             valuepack_tx_bit_index = 0;
//             valuepack_tx_index++;
//         }
//         valuepack_stage = 1;
//     }
// }

// /**
//  * @brief 向数据包中放入字节数据
//  * @param b 要打包的字节数据
//  */
// void putByte(char b)
// {
//     if(valuepack_stage<=2)
//     {
//         // 确保字节对齐
//         if(valuepack_tx_bit_index!=0)
//         {    
//             valuepack_tx_index++;
//             valuepack_tx_bit_index = 0;
//         }
//         valuepack_tx_buffer[valuepack_tx_index] = b;
//         valuepack_tx_index++;
        
//         valuepack_stage = 2;
//     }
// }


// /**
//  * @brief 向数据包中放入整型数据
//  * @param i 要打包的整型数据
//  */
// void putInt(int i)
// {
//     if(valuepack_stage<=4)
//     {
//         // 确保字节对齐
//         if(valuepack_tx_bit_index!=0)
//         {    
//             valuepack_tx_index++;
//             valuepack_tx_bit_index = 0;
//         }
        
//         // 按小端序存储整型数据
//         valuepack_tx_buffer[valuepack_tx_index] = i&0xff;
//         valuepack_tx_buffer[valuepack_tx_index+1] = (i>>8)&0xff;
//         valuepack_tx_buffer[valuepack_tx_index+2] = (i>>16)&0xff;
//         valuepack_tx_buffer[valuepack_tx_index+3] = (i>>24)&0xff;
        
//         valuepack_tx_index +=4;
        
//         valuepack_stage = 4;
//     }
// }

// /**
//  * @brief 向数据包中放入浮点型数据
//  * @param f 要打包的浮点型数据
//  */
// void putFloat(float f)
// {
//     if(valuepack_stage<=5)
//     {
//         // 确保字节对齐
//         if(valuepack_tx_bit_index!=0)
//         {    
//             valuepack_tx_index++;
//             valuepack_tx_bit_index = 0;
//         }
        
//         // 将浮点数转换为整型后按小端序存储
//         int fi = *(int*)(&f);
//         valuepack_tx_buffer[valuepack_tx_index] = fi&0xff;
//         valuepack_tx_buffer[valuepack_tx_index+1] = (fi>>8)&0xff;
//         valuepack_tx_buffer[valuepack_tx_index+2] = (fi>>16)&0xff;
//         valuepack_tx_buffer[valuepack_tx_index+3] = (fi>>24)&0xff;
//         valuepack_tx_index +=4;
//         valuepack_stage = 5;
//     }
// }

// 定义联合体
// union FloatConverter {
//     float f;        // 4字节的float类型
//     uint8_t bytes[4];  // 4个字节的数组
// };

// 使用示例
float bytesToFloat(uint8_t* bytes) {
    union FloatConverter converter;
    
    // 将4个字节复制到联合体的bytes数组中
    converter.bytes[0] = bytes[0];
    converter.bytes[1] = bytes[1];
    converter.bytes[2] = bytes[2];
    converter.bytes[3] = bytes[3];
    
    // 直接返回float值
    return converter.f;
}

// 反向转换：float转字节
void floatToBytes(float value, uint8_t* bytes) {
    union FloatConverter converter;
    converter.f = value;
    
    // 将float的字节表示复制到输出数组
    bytes[0] = converter.bytes[0];
    bytes[1] = converter.bytes[1];
    bytes[2] = converter.bytes[2];
    bytes[3] = converter.bytes[3];
}

// 字节数组转short
int16_t bytesToShort(uint8_t* bytes) {
    union ShortConverter converter;
    
    // 将2个字节复制到联合体的bytes数组中
    converter.bytes[0] = bytes[0];
    converter.bytes[1] = bytes[1];
    
    // 直接返回short值
    return converter.s;
}

// short转字节数组
void shortToBytes(int16_t value, uint8_t* bytes) {
    union ShortConverter converter;
    converter.s = value;
    
    // 将short的字节表示复制到输出数组
    bytes[0] = converter.bytes[0];
    bytes[1] = converter.bytes[1];
}




