/*
 * flash.h
 *
 *  Created on: 2024年12月30日
 *      Author: 林林林
 */

#ifndef CODE_FLASH_H_
#define CODE_FLASH_H_

#include "zf_common_headfile.h"

void flash_PID_1_store(void);
void flash_PID_2_store(void);
void flash_PID_3_store(void);
void flash_environment_store(void);


void flash_PID_1_get(void);
void flash_PID_2_get(void);
void flash_PID_3_get(void);
void flash_environment_get(void);
// void flash_road_memery_get(void);   // 取路径数据

// void flash_road_memery_store_Plus(void);
// void flash_road_memery_get_Plus(void);

// extern uint8_t flash_flag;      // 0为初始状态，1为开始存，2为开始取，3为存完标志，4为取完标志；
// extern uint8_t flash_flag_Plus; // 0为初始状态，1为开始存，2为开始取，3为存完标志，4为取完标志；

// #define Y_memery_page_INDEX_6 (6) // y存点专用扇区

// #define X_memery_page_INDEX_7 (7)   // 新增X轴存点专用扇区
// #define X_memery_page_INDEX_9 (9)   // 新增X轴存点专用扇区
// #define X_memery_page_INDEX_13 (13) // 新增X轴存点专用扇区

// #define Y_memery_page_INDEX_8 (8)   // 新增Y轴存点专用扇区
// #define Y_memery_page_INDEX_10 (10) // 新增Y轴存点专用扇区
// #define Y_memery_page_INDEX_12 (12) // 新增Y轴存点专用扇区

#endif /* CODE_FLASH_H_ */
