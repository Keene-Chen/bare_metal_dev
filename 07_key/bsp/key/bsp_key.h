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
 * @return int
 * @retval int
 * @retval int
 */
int key_getvalue(void);

#endif // __BSP_KEY_H__