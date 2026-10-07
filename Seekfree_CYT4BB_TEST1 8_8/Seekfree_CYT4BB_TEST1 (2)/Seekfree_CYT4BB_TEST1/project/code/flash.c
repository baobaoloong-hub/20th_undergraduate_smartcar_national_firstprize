/*
 * flash.c
 *
 *  Created on: 2024年12月30日
 *      Author: 林林林
 */

#include "zf_common_headfile.h"

#define FLASH_SECTION_INDEX (0)     // 存储数据用的扇区
#define road_memery_page_INDEX (11) // 路径存点专用扇区

#define PID_1_page_INDEX_1 (1) // pid1专用扇区
#define PID_2_page_INDEX_2 (2) // pid2专用扇区
#define PID_3_page_INDEX_3 (3) // pid3专用扇区

#define environment_page_INDEX_3 (20) // envrio_state专用扇区
// #define Y_memery_page_INDEX_2 (2) // y存点专用扇区
// #define Y_memery_page_INDEX_4 (4) // y存点专用扇区
// #define Y_memery_page_INDEX_6 (6) // y存点专用扇区

// #define X_memery_page_INDEX_7 (7)   // 新增X轴存点专用扇区
// #define X_memery_page_INDEX_9 (9)   // 新增X轴存点专用扇区
// #define X_memery_page_INDEX_13 (13) // 新增X轴存点专用扇区

// #define Y_memery_page_INDEX_8 (8)   // 新增Y轴存点专用扇区
// #define Y_memery_page_INDEX_10 (10) // 新增Y轴存点专用扇区
// #define Y_memery_page_INDEX_12 (12) // 新增Y轴存点专用扇区

uint8_t flash_flag = 0;      // 0为初始状态，1为开始存，2为开始取，3为存完标志，4为取完标志；
uint8_t flash_flag_Plus = 0; // 0为初始状态，1为开始存，2为开始取，3为存完标志，4为取完标志；
// 存pid_1数据
void flash_PID_1_store(void)
{
    // 第一步清除缓冲区
    flash_buffer_clear();
    // 第二步把数据存到缓冲区
    for (size_t i = 0; i < FLASH_PAGE_LENGTH - 1; i++)
    {

        flash_union_buffer[i].float_type = pid_number[i]; // 存储pid数据
    }
    // 第三步判断FLASH里有没有数据，有就把FLASH数据擦除/把数据存到缓冲区
    if (flash_check(FLASH_SECTION_INDEX, PID_1_page_INDEX_1))
    { // 判断是否有数据
        flash_erase_page(FLASH_SECTION_INDEX, PID_1_page_INDEX_1);
    } // 擦除这一页
    // 第四步存到FLASH指定扇区
    flash_write_page_from_buffer(FLASH_SECTION_INDEX, PID_1_page_INDEX_1, FLASH_PAGE_LENGTH - 1);
    // 存取过程结束 —— 成功存取
    flash_flag = 3;
}




void flash_PID_2_store(void)
{
    // 第一步清除缓冲区
    flash_buffer_clear();
    // 第二步把数据存到缓冲区
    for (size_t i = 0; i < FLASH_PAGE_LENGTH - 1; i++)
    {

        flash_union_buffer[i].float_type = pid_number[i]; // 存储偏航角
    }

    // 第三步判断FLASH里有没有数据，有就把FLASH数据擦除/把数据存到缓冲区
    if (flash_check(FLASH_SECTION_INDEX, PID_2_page_INDEX_2))
    { // 判断是否有数据
        flash_erase_page(FLASH_SECTION_INDEX, PID_2_page_INDEX_2);
    } // 擦除这一页
    // 第四步存到FLASH指定扇区
    flash_write_page_from_buffer(FLASH_SECTION_INDEX, PID_2_page_INDEX_2, FLASH_PAGE_LENGTH - 1);
    // 存取过程结束 —— 成功存取
    flash_flag = 3;
}





void flash_PID_3_store(void)
{
    // 第一步清除缓冲区
    flash_buffer_clear();
    // 第二步把数据存到缓冲区
    for (size_t i = 0; i < FLASH_PAGE_LENGTH - 1; i++)
    {

        flash_union_buffer[i].float_type = pid_number[i]; // 存储偏航角
    }

    // 第三步判断FLASH里有没有数据，有就把FLASH数据擦除/把数据存到缓冲区
    if (flash_check(FLASH_SECTION_INDEX, PID_3_page_INDEX_3))
    { // 判断是否有数据
        flash_erase_page(FLASH_SECTION_INDEX, PID_3_page_INDEX_3);
    } // 擦除这一页
    // 第四步存到FLASH指定扇区
    flash_write_page_from_buffer(FLASH_SECTION_INDEX, PID_3_page_INDEX_3, FLASH_PAGE_LENGTH - 1);
    // 存取过程结束 —— 成功存取
    flash_flag = 3;
}

void flash_environment_store(void)
{
    // 第一步清除缓冲区
    flash_buffer_clear();
    // 第二步把数据存到缓冲区
    for (size_t i = 0; i < FLASH_PAGE_LENGTH - 1; i++)
    {

        flash_union_buffer[i].float_type = enviro_state[i]; // 存储偏航角
    }

    // 第三步判断FLASH里有没有数据，有就把FLASH数据擦除/把数据存到缓冲区
    if (flash_check(FLASH_SECTION_INDEX, environment_page_INDEX_3))
    { // 判断是否有数据
        flash_erase_page(FLASH_SECTION_INDEX, environment_page_INDEX_3);
    } // 擦除这一页
    // 第四步存到FLASH指定扇区
    flash_write_page_from_buffer(FLASH_SECTION_INDEX, environment_page_INDEX_3, FLASH_PAGE_LENGTH - 1);
    // 存取过程结束 —— 成功存取
    flash_flag = 3;
}










// 取pid1数据
void flash_PID_1_get(void)
{
    if (flash_check(FLASH_SECTION_INDEX, PID_1_page_INDEX_1)) // 检查flash里面是否存了数据
    {
        flash_read_page_to_buffer(FLASH_SECTION_INDEX, PID_1_page_INDEX_1, FLASH_PAGE_LENGTH - 1);
        for (size_t i = 0; i < FLASH_PAGE_LENGTH - 1; i++)
        {
            pid_number[i] = flash_union_buffer[i].float_type; // 取出历史偏航角
        }
    }
    flash_flag = 4;
}


// 取pid2数据
void flash_PID_2_get(void)
{
    if (flash_check(FLASH_SECTION_INDEX, PID_2_page_INDEX_2)) // 检查flash里面是否存了数据
    {
        flash_read_page_to_buffer(FLASH_SECTION_INDEX, PID_2_page_INDEX_2, FLASH_PAGE_LENGTH - 1);
        for (size_t i = 0; i < FLASH_PAGE_LENGTH - 1; i++)
        {
            pid_number[i] = flash_union_buffer[i].float_type; // 取出历史偏航角
        }
    }
    flash_flag = 4;
}


// 取pid3数据
void flash_PID_3_get(void)
{
    if (flash_check(FLASH_SECTION_INDEX, PID_3_page_INDEX_3)) // 检查flash里面是否存了数据
    {
        flash_read_page_to_buffer(FLASH_SECTION_INDEX, PID_3_page_INDEX_3, FLASH_PAGE_LENGTH - 1);
        for (size_t i = 0; i < FLASH_PAGE_LENGTH - 1; i++)
        {
            pid_number[i] = flash_union_buffer[i].float_type; // 取出历史偏航角
        }
    }
    flash_flag = 4;
}


void flash_environment_get(void)
{
    if (flash_check(FLASH_SECTION_INDEX, environment_page_INDEX_3)) // 检查flash里面是否存了数据
    {
        flash_read_page_to_buffer(FLASH_SECTION_INDEX, environment_page_INDEX_3, FLASH_PAGE_LENGTH - 1);
        for (size_t i = 0; i < FLASH_PAGE_LENGTH - 1; i++)
        {
            enviro_state[i] = flash_union_buffer[i].float_type; // 取出历史偏航角
        }
    }
    mt9v03x_init();
    flash_flag = 4;

}












// void flash_road_memery_store_Plus(void)
// {

//     /*存x轴数据分三个扇区依次存*/

//     // 第一步清除缓冲区
//     flash_buffer_clear();
//     // 第二步把数据存到缓冲区
//     for (size_t i = 0; i < FLASH_PAGE_LENGTH; i++)
//     {
//         flash_union_buffer[i].float_type = X_Memery_Plus[i];
//     }

//     // 第三步判断FLASH里有没有数据，有就把FLASH数据擦除/把数据存到缓冲区
//     if (flash_check(FLASH_SECTION_INDEX, X_memery_page_INDEX_1))
//     { // 判断是否有数据
//         flash_erase_page(FLASH_SECTION_INDEX, X_memery_page_INDEX_1);
//     } // 擦除这一页
//     // 第四步存到FLASH指定扇区
//     flash_write_page_from_buffer(FLASH_SECTION_INDEX, X_memery_page_INDEX_1, FLASH_PAGE_LENGTH);

//     // 第一步清除缓冲区
//     flash_buffer_clear();
//     // 第二步把数据存到缓冲区
//     for (size_t i = FLASH_PAGE_LENGTH, j = 0; i < 2 * FLASH_PAGE_LENGTH; i++, j++)
//     {
//         flash_union_buffer[j].float_type = X_Memery_Plus[i];
//     }

//     // 第三步判断FLASH里有没有数据，有就把FLASH数据擦除/把数据存到缓冲区
//     if (flash_check(FLASH_SECTION_INDEX, X_memery_page_INDEX_3))
//     { // 判断是否有数据
//         flash_erase_page(FLASH_SECTION_INDEX, X_memery_page_INDEX_3);
//     } // 擦除这一页
//     // 第四步存到FLASH指定扇区
//     flash_write_page_from_buffer(FLASH_SECTION_INDEX, X_memery_page_INDEX_3, FLASH_PAGE_LENGTH);

//     // 第一步清除缓冲区
//     flash_buffer_clear();
//     // 第二步把数据存到缓冲区
//     for (size_t i = 2 * FLASH_PAGE_LENGTH, j = 0; i < 3 * FLASH_PAGE_LENGTH; i++, j++)
//     {
//         flash_union_buffer[j].float_type = X_Memery_Plus[i];
//     }

//     // 第三步判断FLASH里有没有数据，有就把FLASH数据擦除/把数据存到缓冲区
//     if (flash_check(FLASH_SECTION_INDEX, X_memery_page_INDEX_5))
//     { // 判断是否有数据
//         flash_erase_page(FLASH_SECTION_INDEX, X_memery_page_INDEX_5);
//     } // 擦除这一页
//     // 第四步存到FLASH指定扇区
//     flash_write_page_from_buffer(FLASH_SECTION_INDEX, X_memery_page_INDEX_5, FLASH_PAGE_LENGTH);

//     // 新增X轴数据存储，扇区 7
//     flash_buffer_clear();
//     for (size_t i = 3 * FLASH_PAGE_LENGTH, j = 0; i < 4 * FLASH_PAGE_LENGTH; i++, j++)
//     {
//         flash_union_buffer[j].float_type = X_Memery_Plus[i];
//     }
//     if (flash_check(FLASH_SECTION_INDEX, X_memery_page_INDEX_7))
//     {
//         flash_erase_page(FLASH_SECTION_INDEX, X_memery_page_INDEX_7);
//     }
//     flash_write_page_from_buffer(FLASH_SECTION_INDEX, X_memery_page_INDEX_7, FLASH_PAGE_LENGTH);

//     // 新增X轴数据存储，扇区 9
//     flash_buffer_clear();
//     for (size_t i = 4 * FLASH_PAGE_LENGTH, j = 0; i < 5 * FLASH_PAGE_LENGTH; i++, j++)
//     {
//         flash_union_buffer[j].float_type = X_Memery_Plus[i];
//     }
//     if (flash_check(FLASH_SECTION_INDEX, X_memery_page_INDEX_9))
//     {
//         flash_erase_page(FLASH_SECTION_INDEX, X_memery_page_INDEX_9);
//     }
//     flash_write_page_from_buffer(FLASH_SECTION_INDEX, X_memery_page_INDEX_9, FLASH_PAGE_LENGTH);

//     // 新增X轴数据存储，扇区 13
//     flash_buffer_clear();
//     for (size_t i = 5 * FLASH_PAGE_LENGTH, j = 0; i < 6 * FLASH_PAGE_LENGTH; i++, j++)
//     {
//         flash_union_buffer[j].float_type = X_Memery_Plus[i];
//     }
//     // 在最后一页的最后一个位置存储NUM_L_Plus，用于记录实际数据长度即路径终点
//     flash_union_buffer[FLASH_PAGE_LENGTH - 1].uint16_type = NUM_L_Plus;
//     if (flash_check(FLASH_SECTION_INDEX, X_memery_page_INDEX_13))
//     {
//         flash_erase_page(FLASH_SECTION_INDEX, X_memery_page_INDEX_13);
//     }
//     flash_write_page_from_buffer(FLASH_SECTION_INDEX, X_memery_page_INDEX_13, FLASH_PAGE_LENGTH);

//     /*存y轴数据  分三个扇区依次存*/

//     // 第一步清除缓冲区
//     flash_buffer_clear();
//     // 第二步把数据存到缓冲区
//     for (size_t i = 0; i < FLASH_PAGE_LENGTH; i++)
//     {
//         flash_union_buffer[i].float_type = Y_Memery_Plus[i];
//     }

//     // 第三步判断FLASH里有没有数据，有就把FLASH数据擦除/把数据存到缓冲区
//     if (flash_check(FLASH_SECTION_INDEX, Y_memery_page_INDEX_2))
//     { // 判断是否有数据
//         flash_erase_page(FLASH_SECTION_INDEX, Y_memery_page_INDEX_2);
//     } // 擦除这一页
//     // 第四步存到FLASH指定扇区
//     flash_write_page_from_buffer(FLASH_SECTION_INDEX, Y_memery_page_INDEX_2, FLASH_PAGE_LENGTH);

//     // 第一步清除缓冲区
//     flash_buffer_clear();
//     // 第二步把数据存到缓冲区
//     for (size_t i = FLASH_PAGE_LENGTH, j = 0; i < 2 * FLASH_PAGE_LENGTH; i++, j++)
//     {
//         flash_union_buffer[j].float_type = Y_Memery_Plus[i];
//     }

//     // 第三步判断FLASH里有没有数据，有就把FLASH数据擦除/把数据存到缓冲区
//     if (flash_check(FLASH_SECTION_INDEX, Y_memery_page_INDEX_4))
//     { // 判断是否有数据
//         flash_erase_page(FLASH_SECTION_INDEX, Y_memery_page_INDEX_4);
//     } // 擦除这一页
//     // 第四步存到FLASH指定扇区
//     flash_write_page_from_buffer(FLASH_SECTION_INDEX, Y_memery_page_INDEX_4, FLASH_PAGE_LENGTH);

//     // 第一步清除缓冲区
//     flash_buffer_clear();
//     // 第二步把数据存到缓冲区
//     for (size_t i = 2 * FLASH_PAGE_LENGTH, j = 0; i < 3 * FLASH_PAGE_LENGTH; i++, j++)
//     {
//         flash_union_buffer[j].float_type = Y_Memery_Plus[i];
//     }

//     // 第三步判断FLASH里有没有数据，有就把FLASH数据擦除/把数据存到缓冲区
//     if (flash_check(FLASH_SECTION_INDEX, Y_memery_page_INDEX_6))
//     { // 判断是否有数据
//         flash_erase_page(FLASH_SECTION_INDEX, Y_memery_page_INDEX_6);
//     } // 擦除这一页
//     // 第四步存到FLASH指定扇区
//     flash_write_page_from_buffer(FLASH_SECTION_INDEX, Y_memery_page_INDEX_6, FLASH_PAGE_LENGTH);

//     // 新增Y轴数据存储，扇区 8
//     flash_buffer_clear();
//     for (size_t i = 3 * FLASH_PAGE_LENGTH, j = 0; i < 4 * FLASH_PAGE_LENGTH; i++, j++)
//     {
//         flash_union_buffer[j].float_type = Y_Memery_Plus[i];
//     }
//     if (flash_check(FLASH_SECTION_INDEX, Y_memery_page_INDEX_8))
//     {
//         flash_erase_page(FLASH_SECTION_INDEX, Y_memery_page_INDEX_8);
//     }
//     flash_write_page_from_buffer(FLASH_SECTION_INDEX, Y_memery_page_INDEX_8, FLASH_PAGE_LENGTH);

//     // 新增Y轴数据存储，扇区 10
//     flash_buffer_clear();
//     for (size_t i = 4 * FLASH_PAGE_LENGTH, j = 0; i < 5 * FLASH_PAGE_LENGTH; i++, j++)
//     {
//         flash_union_buffer[j].float_type = Y_Memery_Plus[i];
//     }
//     if (flash_check(FLASH_SECTION_INDEX, Y_memery_page_INDEX_10))
//     {
//         flash_erase_page(FLASH_SECTION_INDEX, Y_memery_page_INDEX_10);
//     }
//     flash_write_page_from_buffer(FLASH_SECTION_INDEX, Y_memery_page_INDEX_10, FLASH_PAGE_LENGTH);

//     // 新增Y轴数据存储，扇区 12
//     flash_buffer_clear();
//     for (size_t i = 5 * FLASH_PAGE_LENGTH, j = 0; i < 6 * FLASH_PAGE_LENGTH; i++, j++)
//     {
//         flash_union_buffer[j].float_type = Y_Memery_Plus[i];
//     }
//     if (flash_check(FLASH_SECTION_INDEX, Y_memery_page_INDEX_12))
//     {
//         flash_erase_page(FLASH_SECTION_INDEX, Y_memery_page_INDEX_12);
//     }
//     flash_write_page_from_buffer(FLASH_SECTION_INDEX, Y_memery_page_INDEX_12, FLASH_PAGE_LENGTH);

//     // 存取过程结束 —— 成功存取
//     flash_flag_Plus = 3;
// }

// void flash_road_memery_get_Plus(void)
// {

//     /*取x轴数据,分三个扇区取*/
//     if (flash_check(FLASH_SECTION_INDEX, X_memery_page_INDEX_1)) // 检查flash里面是否存了数据
//     {
//         flash_read_page_to_buffer(FLASH_SECTION_INDEX, X_memery_page_INDEX_1, FLASH_PAGE_LENGTH);
//         for (size_t i = 0; i < FLASH_PAGE_LENGTH; i++)
//         {
//             X_Memery_Store_Plus[i] = flash_union_buffer[i].float_type; //
//         }
//     }

//     /*取x轴数据*/
//     if (flash_check(FLASH_SECTION_INDEX, X_memery_page_INDEX_3)) // 检查flash里面是否存了数据
//     {
//         flash_read_page_to_buffer(FLASH_SECTION_INDEX, X_memery_page_INDEX_3, FLASH_PAGE_LENGTH);
//         for (size_t i = 0, j = FLASH_PAGE_LENGTH; i < FLASH_PAGE_LENGTH; i++, j++)
//         {
//             X_Memery_Store_Plus[j] = flash_union_buffer[i].float_type; //
//         }
//     }

//     /*取x轴数据*/
//     if (flash_check(FLASH_SECTION_INDEX, X_memery_page_INDEX_5)) // 检查flash里面是否存了数据
//     {
//         flash_read_page_to_buffer(FLASH_SECTION_INDEX, X_memery_page_INDEX_5, FLASH_PAGE_LENGTH);
//         for (size_t i = 0, j = 2 * FLASH_PAGE_LENGTH; i < FLASH_PAGE_LENGTH; i++, j++)
//         {
//             X_Memery_Store_Plus[j] = flash_union_buffer[i].float_type; //
//         }
//     }

//     // 新增X轴数据读取，扇区 7
//     if (flash_check(FLASH_SECTION_INDEX, X_memery_page_INDEX_7))
//     {
//         flash_read_page_to_buffer(FLASH_SECTION_INDEX, X_memery_page_INDEX_7, FLASH_PAGE_LENGTH);
//         for (size_t i = 0, j = 3 * FLASH_PAGE_LENGTH; i < FLASH_PAGE_LENGTH; i++, j++)
//         {
//             X_Memery_Store_Plus[j] = flash_union_buffer[i].float_type;
//         }
//     }

//     // 新增X轴数据读取，扇区 9
//     if (flash_check(FLASH_SECTION_INDEX, X_memery_page_INDEX_9))
//     {
//         flash_read_page_to_buffer(FLASH_SECTION_INDEX, X_memery_page_INDEX_9, FLASH_PAGE_LENGTH);
//         for (size_t i = 0, j = 4 * FLASH_PAGE_LENGTH; i < FLASH_PAGE_LENGTH; i++, j++)
//         {
//             X_Memery_Store_Plus[j] = flash_union_buffer[i].float_type;
//         }
//     }

//     // 新增X轴数据读取，扇区 13
//     if (flash_check(FLASH_SECTION_INDEX, X_memery_page_INDEX_13))
//     {
//         flash_read_page_to_buffer(FLASH_SECTION_INDEX, X_memery_page_INDEX_13, FLASH_PAGE_LENGTH);
//         for (size_t i = 0, j = 5 * FLASH_PAGE_LENGTH; i < FLASH_PAGE_LENGTH; i++, j++)
//         {
//             X_Memery_Store_Plus[j] = flash_union_buffer[i].float_type;
//         }
//         // 从最后一页读取路径终点值
//         road_destination = flash_union_buffer[FLASH_PAGE_LENGTH - 1].uint16_type;
//     }

//     /*取y轴数据,分三个扇区取*/
//     if (flash_check(FLASH_SECTION_INDEX, Y_memery_page_INDEX_2)) // 检查flash里面是否存了数据
//     {
//         flash_read_page_to_buffer(FLASH_SECTION_INDEX, Y_memery_page_INDEX_2, FLASH_PAGE_LENGTH);
//         for (size_t i = 0; i < FLASH_PAGE_LENGTH; i++)
//         {
//             Y_Memery_Store_Plus[i] = flash_union_buffer[i].float_type; //
//         }
//     }

//     /*取y轴数据*/
//     if (flash_check(FLASH_SECTION_INDEX, Y_memery_page_INDEX_4)) // 检查flash里面是否存了数据
//     {
//         flash_read_page_to_buffer(FLASH_SECTION_INDEX, Y_memery_page_INDEX_4, FLASH_PAGE_LENGTH);
//         for (size_t i = 0, j = FLASH_PAGE_LENGTH; i < FLASH_PAGE_LENGTH; i++, j++)
//         {
//             Y_Memery_Store_Plus[j] = flash_union_buffer[i].float_type; //
//         }
//     }

//     /*取y轴数据*/
//     if (flash_check(FLASH_SECTION_INDEX, Y_memery_page_INDEX_6)) // 检查flash里面是否存了数据
//     {
//         flash_read_page_to_buffer(FLASH_SECTION_INDEX, Y_memery_page_INDEX_6, FLASH_PAGE_LENGTH);
//         for (size_t i = 0, j = 2 * FLASH_PAGE_LENGTH; i < FLASH_PAGE_LENGTH; i++, j++)
//         {
//             Y_Memery_Store_Plus[j] = flash_union_buffer[i].float_type; //
//         }
//     }

//     // 新增Y轴数据读取，扇区 8
//     if (flash_check(FLASH_SECTION_INDEX, Y_memery_page_INDEX_8))
//     {
//         flash_read_page_to_buffer(FLASH_SECTION_INDEX, Y_memery_page_INDEX_8, FLASH_PAGE_LENGTH);
//         for (size_t i = 0, j = 3 * FLASH_PAGE_LENGTH; i < FLASH_PAGE_LENGTH; i++, j++)
//         {
//             Y_Memery_Store_Plus[j] = flash_union_buffer[i].float_type;
//         }
//     }

//     // 新增Y轴数据读取，扇区 10
//     if (flash_check(FLASH_SECTION_INDEX, Y_memery_page_INDEX_10))
//     {
//         flash_read_page_to_buffer(FLASH_SECTION_INDEX, Y_memery_page_INDEX_10, FLASH_PAGE_LENGTH);
//         for (size_t i = 0, j = 4 * FLASH_PAGE_LENGTH; i < FLASH_PAGE_LENGTH; i++, j++)
//         {
//             Y_Memery_Store_Plus[j] = flash_union_buffer[i].float_type;
//         }
//     }

//     // 新增Y轴数据读取，扇区 12
//     if (flash_check(FLASH_SECTION_INDEX, Y_memery_page_INDEX_12))
//     {
//         flash_read_page_to_buffer(FLASH_SECTION_INDEX, Y_memery_page_INDEX_12, FLASH_PAGE_LENGTH);
//         for (size_t i = 0, j = 5 * FLASH_PAGE_LENGTH; i < FLASH_PAGE_LENGTH; i++, j++)
//         {
//             Y_Memery_Store_Plus[j] = flash_union_buffer[i].float_type;
//         }
//     }

//     flash_flag_Plus = 4;
// }