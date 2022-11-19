/**
 * @file    : bsp_exti.h
 * @author  : KeeneChen
 * @date    : 2022.11.09-18:36:55
 * @details : 外部中断配置
 */

#ifndef __BSP_EXTI_H__
#define __BSP_EXTI_H__

#include "imx6ull.h"

/**
 * @brief  中断初始化
 * @return void
 */
void exit_init(void);

/**
 * @brief  中断处理函数
 * @return void
 */
void gpio1_io18_irqhandler(void);

#endif // __BSP_EXTI_H__