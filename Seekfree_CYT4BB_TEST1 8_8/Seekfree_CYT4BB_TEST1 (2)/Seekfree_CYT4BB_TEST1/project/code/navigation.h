#ifndef CODE_NAVIGATION_H_
#define CODE_NAVIGATION_H_

#include "zf_common_headfile.h"

typedef struct navigation
{
    uint32 data_distance;
    float yaw;

}point_data;

extern point_data point[10000];

extern PID Divert;

extern int16_t    aim_distance;
extern float    aim_yaw;
extern int32    total_distance;

extern uint8_t mode_rem;
extern uint16_t i_Track;
extern uint16_t i_cb;

void start_track();
void Auto_Collect();
void UI_navigation_col();


#endif

