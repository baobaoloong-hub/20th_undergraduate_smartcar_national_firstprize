/*---------------------------------
*   File name: UI.c
*
*   Author: LaZuli
*
*   Created time: 2024年4月12日
---------------------------------*/
#include "UI.h"
/*--------------------------------*/
#define animation_time (20) //添加光标动画,越大越慢,没啥用
uint8_t mode;


//菜单定义
menu* father1;  //菜单一
menu* father2;
menu* father3;
menu* father4;
menu* Cursor;   //光标
menu* nowmenu;  //现菜单
menu* father5;

char display_on=1;
static bool key1_long_flag = false; // 添加key1长按标志位

static uint16 i=20;//光标起点
bool menu_flag=0;   //是否调用结构体中的指针函数,1为是
static int L_Y,L_X,R_Y,R_X;

//函数声明
void Basis_Factor_Check_1(void);
void menu_display(void);
void parameter_debug(void);
void remote_track(void);
void Value_change(float *Value,float Unit);
void Cam(void);
void navigation_path(void);
void nofun(void){}  //空函数
char navigation(void){return 1;}
void Value_change(float *Value,float Unit);  //调整值   单位值



//添加菜单
menu* Menu_create(uint8 row,char date[20],void (*Fanction)(void))//行数,菜单名,指针函数
{
    menu* node=(menu*)malloc (sizeof(menu));    //分配节点内存

    node->row=row;                  //填入行数
    strcpy(node->date, date);       //填入菜单名
    node->Fanction=Fanction;        //填入执行函数

    node->sibling=NULL;

    return node;
}

//添加兄弟菜单
void addsibling(menu *older, menu *youth)
{
    if (older->sibling == NULL)
    {
        older->sibling = youth;
    }
    else    //其他情况还没考虑
    {

    }
}

void tft_init(void) //屏幕初始化
{
    // tft180_init();
    // tft180_clear();
    // tft180_set_dir(TFT180_PORTAIT);     //屏幕方向
    // tft180_set_color(RGB565_WHITE, RGB565_BLACK);
    // tft180_full(RGB565_BLACK);

    
    ips200_init(IPS200_TYPE_SPI);


    ips200_set_color(RGB565_WHITE, RGB565_BLACK);
    ips200_full(RGB565_BLACK);
}

void menu_init()
{
    tft_init();

    Cursor = Menu_create(20,">>",&nofun);
    father1 = Menu_create(20,"1.Status",&Basis_Factor_Check_1);
    father2 = Menu_create(40,"2.Parameter",&parameter_debug);
    father3 = Menu_create(60,"3.Camera",&ips200_show);
    father4 = Menu_create(80,"4.Navigation",&UI_navigation_col);
    father5 = Menu_create(100,"5.remote_T",&remote_track);

    nowmenu=father1;

    addsibling(father1,father2);
    addsibling(father2,father3);
    addsibling(father3,father4);
    addsibling(father4,father5);
    addsibling(father5,father1);
//    addsibling(father4,father1);

    menu_display();
}

void menu_display() //主菜单显示
{
    // ips200_full(RGB565_WHITE);
    // ips200_set_font(ips200_8X16_FONT);


    ips200_show_string(20,father1->row,father1->date);
    ips200_show_string(20,father2->row,father2->date);
    ips200_show_string(20,father3->row,father3->date);
    ips200_show_string(20,father4->row,father4->date);
    ips200_show_string(20,father5->row,father5->date);
    ips200_show_string(0,Cursor->row,Cursor->date);
}
void choose_menu()
{
    if(key2_short&&menu_flag==0)//光标移动
    {
  
            nowmenu=nowmenu->sibling;
            if(menu_flag==1)ips200_full(RGB565_BLACK);
            // menu_flag=0;

        if(Cursor->row==100)    //光标回到第一个
        {
            ips200_show_string(0,i,"  ");
            Cursor->row =20;
            i=80;
            for(i;i>Cursor->row;i--)
            {
                ips200_show_string(0,i,Cursor->date);
                i+=(Cursor->row-i)/32;
                system_delay_ms(1);
            }
            i=20;
        }
        else
        {
            Cursor->row +=20;

            for(i;i<Cursor->row;i++)
            {
                 ips200_show_string(0,i,Cursor->date);
                 i+=(Cursor->row-i)/animation_time;
                 system_delay_ms(2);

                  
            }
        }
        menu_display();
        key1_long_flag = false; // 短按后重置长按标志位
    }

    if(key3_short && !key1_long_flag)   //确定,返回,调用结构体中的指针函数，并且保证每次只执行一次
    {
        menu_flag=!menu_flag;  // 每次长按切换一次状态
        ips200_full(RGB565_BLACK);
        if(menu_flag==0)menu_display();
        key1_long_flag = true; // 设置标志位，防止重复触发
    }
    else if(!key3_short) // 当按键松开时，重置标志位
    {
        key1_long_flag = false;
    }

    if(menu_flag==1)
        {
            nowmenu->Fanction();
        }
    else {}
}

/*-----------结构体中的指针函数(需提前声明)-----------*/

/*-------------------子菜单-------------------*/

/*------------------------------------------------
 * 函数简介     状态查看
 * 函数名称     Basis_Factor_Check_1(void )
 * 返回参数     void
 * 使用示例     NaN
 * ----------------------------------------------*/
void Basis_Factor_Check_1(void)
{
        if(key_get_state(KEY_1)==KEY_SHORT_PRESS)mode=1;
        if(key_get_state(KEY_2)==KEY_SHORT_PRESS)mode=2;
        ips200_show_int(200, 16*9, mode, 2);

}
/*------------------------------------------------
 * 函数简介     调参todo
 * 函数名称     parameter_debug(void )
 * 返回参数     void
 * 使用示例     NaN
 * ----------------------------------------------*/

void parameter_debug()
{
 static uint16 cursor_2 = 0;
    //
    if(key2_short)        //光标移动(向下)
    {
        system_delay_ms(12);
        
        system_delay_ms(12);
        ips200_full(RGB565_BLACK);
        cursor_2+=16;
        if(cursor_2 >(16*15-1)) cursor_2 = 0;
        ips200_show_string(0,cursor_2,">>");
        ips200_show_string(20,16*2,"offset_circle2");            ips200_show_int(180,16*2,(int)offset_circle2,4);
        ips200_show_string(20,16*3,"circle_enter2");            ips200_show_int(180,16*3,(int)circle_distance_enter2,5);
        ips200_show_string(20,16*4,"offset_circle1");            ips200_show_int(180,16*4,(int)offset_circle1,4);
        ips200_show_string(20,16*5,"circle_enter1");            ips200_show_int(180,16*5,(int)circle_distance_enter1,5);
        ips200_show_string(20,16*6,"circle_out");            ips200_show_int(180,16*6,(int)circle_distance_out,5);
        ips200_show_string(20,16*7,"vol_now");            ips200_show_int(180,16*7,(int)vol_now,5);
        ips200_show_string(20,16*8,"Divert. Kp");            ips200_show_float(180,16*8,Divert. Kp,2,3);
        ips200_show_string(20,16*9,"Divert. Kd");            ips200_show_float(180,16*9,Divert. Kd,2,3);
        ips200_show_string(20,16*10,"camera.Kp");            ips200_show_float(180,16*10,camera.Kp,2,3);
        ips200_show_string(20,16*11,"camera.Kd");            ips200_show_float(180,16*11,camera.Kd,2,3);
        ips200_show_string(20,16*12,"BMI270_G.Kp");            ips200_show_float(180,16*12,BMI270_G.Kp,2,3);
        ips200_show_string(20,16*13,"BMI270_G.Kd");            ips200_show_float(180,16*13,BMI270_G.Kd,2,3);
        // ips200_show_string(20,16*12,"circle_out");



    }

   switch(cursor_2)      //调整参数对应的光标,执行调整函数
   {
        case 0:remote_control();break;
        case 2*16:
        {
            Value_change(&offset_circle2,5);
            ips200_show_int(180,16*2,(int)offset_circle2,4);
           pid_number[15]=offset_circle2;
            break;
        }
        case 3*16:
        {
            Value_change(&circle_distance_enter2,1000);
            ips200_show_int(180,16*3,(int)circle_distance_enter2,5);
            pid_number[16]=circle_distance_enter2;
            break;
        }
        case 4*16:
        {
            Value_change(&offset_circle1,5);
            ips200_show_int(180,16*4,(int)offset_circle1,4);
            pid_number[13]=offset_circle1;
            break;
        }
        case 5*16:
        {
            Value_change(&circle_distance_enter1,1000);
            ips200_show_int(180,16*5,(int)circle_distance_enter1,5);
            pid_number[14]=circle_distance_enter1;
            break;
        }
        case 6*16:
        {
            Value_change(&circle_distance_out,5000);
            ips200_show_int(180,16*6,(int)circle_distance_out,5);
            pid_number[10]=circle_distance_out;
            break;
        }
        case 7*16:
        {
            Value_change(&vol_now,10);
            ips200_show_int(180,16*7,(int)vol_now,5);
            pid_number[0]=vol_now;
            break;
        }
        case 8*16:
        {
            Value_change(&Divert. Kp,0.1);
            ips200_show_float(180,16*8,Divert. Kp,2,3);
            pid_number[7]=Divert. Kp;
            break;
        }
        case 9*16:
        {
            Value_change(&Divert. Kd,1);
            ips200_show_float(180,16*9,Divert. Kd,2,3);
            pid_number[9]=Divert. Kd;
            break;
        }
        case 10*16:
        {
            Value_change(&camera.Kp,0.005);
            ips200_show_float(180,16*10,camera.Kp,2,3);
            pid_number[1]=camera.Kp;
            break;
        }
        case 11*16:
        {
            Value_change(&camera.Kd,0.005);
            ips200_show_float(180,16*11,camera.Kd,2,3);
            pid_number[3]=camera.Kd;
            break;
        }
        case 12*16:
        {
            Value_change(&BMI270_G.Kp,1);
            ips200_show_float(180,16*12,BMI270_G.Kp,2,3);
            pid_number[4]=BMI270_G.Kp;
            break;
        }
        case 13*16:
        {
            Value_change(&BMI270_G.Kd,1);
            ips200_show_float(180,16*13,BMI270_G.Kd,2,3);
            pid_number[6]=BMI270_G.Kd;
            break;
        }
        // case 7*16:Value_change(&A.Kp,0.1);break;

        default:break;
   }
        if(flash_mode==1)
    {

        ips200_show_string(0,16*1,"pid1");
        if(key4_short)
        {
            flash_PID_1_store();
            ips200_show_string(0,16*17,"flash_store_finish");
        }
        if(key5_short)
        {
            flash_PID_1_get();
            pid_init();
            ips200_show_int(0,16*18,(int)pid_number[0],4);
        }
    }

    if(flash_mode==2)
    {

        ips200_show_string(0,16*1,"pid2");
        if(key4_short)
        {
            flash_PID_2_store();
            ips200_show_string(0,16*17,"flash_store_finish");
        }
        if(key5_short)
        {
            flash_PID_2_get();
            pid_init();
            ips200_show_int(0,16*18,(int)pid_number[0],4);
        }
    }

    if(flash_mode==3)
    {

        ips200_show_string(0,16*1,"pid3");
        if(key4_short)
        {
            flash_PID_3_store();
            ips200_show_string(0,16*17,"flash_store_finish");
        }
        if(key5_short)
        {
            flash_PID_3_get();
            pid_init();
            ips200_show_int(0,16*18,(int)pid_number[0],4);
        }
    }
pid_init();
}
/*------------------------------------------------
 * 函数简介     Todo
 * 函数名称     remote_control(void )
 * 返回参数     void
 * 使用示例     NaN
 * ----------------------------------------------*/
void Cam()
{
    ips200_show();
}
/*------------------------------------------------
 * 函数简介     路径导航
 * 函数名称     remote_control(void )
 * 返回参数     void
 * 使用示例     NaN
 * ----------------------------------------------*/
void navigation_path(){
//     static int i=0;

// if(display_on==1){
//                 ips200_set_font(ips200_6X8_FONT);

//             //    ips200_show_float( 0, 0, point_data[1][0], 3, 4);
//                 ips200_show_float( 58, 0, Encoder_Total, 5, 1);

//                 ips200_show_float( 0, 24, point_data[2][0], 3, 4);
//                 ips200_show_float( 58, 24, point_data[2][1], 3, 4);

//                 ips200_show_string(15,144,"RR");
//                 ips200_show_float(60,120,Vec_Original,4,1);

//                 ips200_show_string(15,144,"R");
//                 ips200_show_float(60,144,Vec_Aim,4,1);

//                 ips200_show_float( 5, 48, T_N , 4, 3);
//                 ips200_show_float( 50, 48, Turn.Out , 4, 2);
//                 }

// //if(i<1){
// ////    Vec_Aim = Vec_Original;
// //    Delta_Yaw_Temp=0;
// //        Vec_Aim = 0 ;
// //
// //    Delta_Yaw=0;
// //    Delta_Yaw_Temp=0;
// //    i++;
// //}

// ///*------------------复位按键-------------------*/

// if(gpio_get_level(Key3) == 0)
//     {
         
        
         
//         Encoder_Total=0;
//         Vec_Aim = Vec_Original;
//         i_Track=0 ;
//         Delta_Yaw_Temp=0;
//         T_N=0;Delta_Yaw=0;
//     }
// ///*--------------------------------------------*/

// ///*---------------------循迹--------------------*/

// //    Track();

// ///*--------------------------------------------*/

// ///*------------------循迹(青春版)-----------------*/
//     if(point_data[i_Track][0] - Encoder_Total < 100){
//         Encoder_Total = 0;
//         Vec_Aim=Vec_Original;
//         Buzzer(1);
//         system_delay_ms(1);
//         Buzzer(0);
//         i_Track ++;
//         Delta_Yaw = point_data[i_Track][1];//使当前角度依次变为点位的角度值
//         }
// //    else Vec_Aim=Vec_Original;
// ///*--------------------------------------------*/

// ///*------------------速度控制--------------------*/

// Value_change(&Vec_Original,10);
// Speed_ctrl_loop();
// if(dl1b_distance_mm<700&&Vec_Aim>=550){if(Vec_Aim>550)Vec_Aim-=0.25;}

// ///*--------------------------------------------*/
// ///*-----------------循迹の函数--------------------*/

// //    Turn_Angle();
//     Passive_Car_Status();
//     Spontaneous_Car_Status();

// ///*--------------------------------------------*/

   }
/*------------------------------------------------
 * 函数简介     遥控打点
 * 函数名称     remote_track(void )
 * 返回参数     void
 * 使用示例     NaN
 * ----------------------------------------------*/

void remote_track ()    
{


 static uint16 cursor_3 = 4*16;
    //
    if(key2_short)        //光标移动(向下)
    {
        system_delay_ms(12);
        
        system_delay_ms(12);
        ips200_full(RGB565_PINK);
        cursor_3+=16;
        if(cursor_3 >(16*9-1)) cursor_3 = 4*16;
        ips200_show_string(0,cursor_3,">>");
        ips200_show_string(20,16*5,"fuya_speed");            ips200_show_int(180,16*5,(int)fuya_speed,4);
        ips200_show_string(20,16*6,"MT9V03X_EXP");            ips200_show_int(180,16*6,(int)MT9V03X_EXP_number,5);
        ips200_show_string(20,16*7,"otsu_min");            ips200_show_int(180,16*7,(int)otsu_min,5);
        ips200_show_string(20,16*8,"otsu_max");            ips200_show_int(180,16*8,(int)otsu_max,5);
        // ips200_show_string(20,16*8,"Divert. Kp");            ips200_show_float(180,16*8,Divert. Kp,2,3);
        // ips200_show_string(20,16*9,"Divert. Kd");            ips200_show_float(180,16*9,Divert. Kd,2,3);
        // ips200_show_string(20,16*10,"camera.Kp");            ips200_show_float(180,16*10,camera.Kp,2,3);
        // ips200_show_string(20,16*11,"camera.Kd");            ips200_show_float(180,16*11,camera.Kd,2,3);
        // ips200_show_string(20,16*12,"BMI270_G.Kp");            ips200_show_float(180,16*12,BMI270_G.Kp,2,3);
        // ips200_show_string(20,16*13,"BMI270_G.Kd");            ips200_show_float(180,16*13,BMI270_G.Kd,2,3);
        // ips200_show_string(20,16*12,"circle_out");



    }

   switch(cursor_3)      //调整参数对应的光标,执行调整函数
   {
        // case 0:remote_control();break;
        case 5*16:
        {
            // Value_change(&offset_circle,5);
            // ips200_show_int(180,16*4,(int)offset_circle,4);
            // pid_number[13]=offset_circle;

            Value_change(&fuya_speed,1000);
            ips200_show_int(180, 16*5, (int)fuya_speed, 5);
            if(fuya_speed<0)fuya_speed=0;
            if(fuya_speed>10000)fuya_speed=10000;
            enviro_state[0]=fuya_speed;
            break;
        }
        case 6*16:
        {
            Value_change(&MT9V03X_EXP_number,20);
            if(MT9V03X_EXP_number<0)
            {
                MT9V03X_EXP_number=0;
            }
            if(MT9V03X_EXP_number!=enviro_state[1])
            {
                mt9v03x_init();
            }
            ips200_show_int(180,16*6,(int)MT9V03X_EXP_number,5);
            enviro_state[1]=MT9V03X_EXP_number;
            break;
        }
        case 7*16:
        {
            Value_change(&otsu_min,1);
            ips200_show_int(180,16*7,(int)otsu_min,5);
            enviro_state[2]=otsu_min;
            break;
        }
        case 8*16:
        {
            Value_change(&otsu_max,1);
            ips200_show_int(180,16*8,(int)otsu_max,5);
            enviro_state[3]=otsu_max;
            break;
        }
        case 9*16:
        {
            // Value_change(&Divert. Kp,0.1);
            // ips200_show_float(180,16*8,Divert. Kp,2,3);
            // pid_number[7]=Divert. Kp;
            break;
        }
        case 10*16:
        {
            // Value_change(&Divert. Kd,1);
            // ips200_show_float(180,16*9,Divert. Kd,2,3);
            // pid_number[9]=Divert. Kd;
            break;
        }
        case 11*16:
        {
            // Value_change(&camera.Kp,0.005);
            // ips200_show_float(180,16*10,camera.Kp,2,3);
            // pid_number[1]=camera.Kp;
            break;
        }
        case 12*16:
        {
            // Value_change(&camera.Kd,0.005);
            // ips200_show_float(180,16*11,camera.Kd,2,3);
            // pid_number[3]=camera.Kd;
            break;
        }
        case 13*16:
        {
            // Value_change(&BMI270_G.Kp,1);
            // ips200_show_float(180,16*12,BMI270_G.Kp,2,3);
            // pid_number[4]=BMI270_G.Kp;
            break;
        }
        case 14*16:
        {
            // Value_change(&BMI270_G.Kd,1);
            // ips200_show_float(180,16*13,BMI270_G.Kd,2,3);
            // pid_number[6]=BMI270_G.Kd;
            break;
        }
        // case 7*16:Value_change(&A.Kp,0.1);break;

        default:break;
   }
    if(key4_short)
    {
    flash_environment_store();
    ips200_show_string(0,16*18,"flash_store_finish");
    }
    if(key5_short)
    {
    flash_environment_get();
    enviro_state_init();
    ips200_show_int(0,16*19,(int)enviro_state[1],4);
    }
    enviro_state_init();
    ips200_show_gray_image(0, 0, (const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
    ips200_show_gray_image(100, 0,(const uint8 *)bin_image, image_w, image_h, image_w, image_h,0);
}

/*-------------------功能函数-------------------*/

/*------------------------------------------------
 * 函数简介     遥控
 * 函数名称     remote_control(void )
 * 返回参数     void
 * 使用示例     NaN
 * ----------------------------------------------*/
float flash_mode=0;
void remote_control(void )
{
    ips200_show_int(0, 16*16, flash_mode, 4);
    Value_change(&flash_mode,1);
    // if(flash_mode==1)
    // {

    //     ips200_show_string(0,16*1,"pid1");
    //     if(key4_short)
    //     {
    //         flash_PID_1_store();
    //         ips200_show_string(0,16*2,"flash_store_finish");
    //     }
    //     if(key5_short)
    //     {
    //         flash_PID_1_get();
    //         pid_init();
    //         ips200_show_int(0,16*3,(int)pid_number[0],4);
    //     }
    // }

    // if(flash_mode==2)
    // {

    //     ips200_show_string(0,16*1,"pid2");
    //     if(key4_short)
    //     {
    //         flash_PID_2_store();
    //         ips200_show_string(0,16*2,"flash_store_finish");
    //     }
    //     if(key5_short)
    //     {
    //         flash_PID_2_get();
    //         pid_init();
    //         ips200_show_int(0,16*3,(int)pid_number[0],4);
    //     }
//     }

//     if(flash_mode==3)
//     {

//         ips200_show_string(0,16*1,"pid3");
//         if(key4_short)
//         {
//             flash_PID_3_store();
//             ips200_show_string(0,16*2,"flash_store_finish");
//         }
//         if(key5_short)
//         {
//             flash_PID_3_get();
//             pid_init();
//             ips200_show_int(0,16*3,(int)pid_number[0],4);
//         }
//     }
}
/*------------------------------------------------
 * 函数简介     按键调参
 * 函数名称     Value_change(float *Value,float Unit )
 * 返回参数     void
 * 使用示例     Value----需要改变的值
 *            Unit ----单位数值
 * ----------------------------------------------*/
void Value_change(float *Value,float Unit)  //调整值   单位值
{
    if(key0_short)    //值加一个单位
    {
        // ips200_clear();
        *Value=*Value+Unit;
    }
    if(key1_short)    //值减一个单位
    {
        // ips200_clear();
        *Value=*Value-Unit;
    }

}



/*--------------------------------*/


