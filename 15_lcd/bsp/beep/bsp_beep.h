/**
 * @file    : bsp_beep.h
 * @author  : KeeneChen
 * @date    : 2022.11.05-10:46:58
 * @details : 蜂鸣器驱动模块
 */

#ifndef __BSP_BEEP_H__
#define __BSP_BEEP_H__

/**
 * @brief  蜂鸣器初始化
 * @return void
 */
void beep_init(void);

/**
 * @brief  蜂鸣器控制
 * @param  status 0:关闭 1:开启
 * @return void
 */
void beep_switch(int status);

#endif // __BSP_BEEP_H__