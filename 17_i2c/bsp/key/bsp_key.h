#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

#include "bsp_delay.h"
#include "fsl_iomuxc.h"

// 按键值枚举
enum key_value { KEY_NONE = 0, KEY0_VALUE };

/**
 * @brief  按键初始化
 * @return void
 */
void key_init(void);

/**
 * @brief  获取按键值
 * @return int   按键电平信号
 * @retval other 对应的按键值
 * @retval 0     没有按键按下
 */
int key_getvalue(void);

#endif // __BSP_KEY_H__