/*---------------------------------
*   File name: UI.h
*
*   Author: LaZuli
*
*   Created time: 2024年4月12日
---------------------------------*/
#ifndef CODE_UI_H_
#define CODE_UI_H_

#include "zf_common_headfile.h"

/*-----------结构体初始化-----------*/
typedef struct UI_page                 //定义页面结构体UI_page的别名为menu
{
    uint16 row;
    char date[20];

    void (*Fanction)(void);

    struct UI_page* sibling;
    struct UI_page* child;
} menu;
/*--------------------------------*/

/*-----------外部变量-----------*/
/*--------------------------------*/

/*-----------按键定义-----------*/
#define key0_short (key_get_state(KEY_1)==KEY_SHORT_PRESS)
#define key1_short (key_get_state(KEY_2)==KEY_SHORT_PRESS)
#define key2_short (key_get_state(KEY_3)==KEY_SHORT_PRESS)
#define key3_short (key_get_state(KEY_4)==KEY_SHORT_PRESS)
#define key4_short (key_get_state(KEY_5)==KEY_SHORT_PRESS)
#define key5_short (key_get_state(KEY_6)==KEY_SHORT_PRESS)

#define key1_long (key_get_state(KEY_1)==KEY_LONG_PRESS)
#define key2_long (key_get_state(KEY_2)==KEY_LONG_PRESS)

/*--------------------------------*/


/*-----------枚举数组初始化-----------*/

/*--------------------------------*/

/*-----------函数声明-----------*/
void menu_init(void);
void choose_menu(void);
void remote_control(void );
char navigation(void);
void navigation_path(void);
extern float flash_mode;
/*--------------------------------*/


#endif /* CODE_UI_H_ */
