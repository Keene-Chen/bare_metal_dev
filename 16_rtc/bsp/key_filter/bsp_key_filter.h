/**
 * @file    : bsp_key_filter.h
 * @author  : KeeneChen
 * @date    : 2022.11.15-15:57:42
 * @details : 定时器按键消抖实验
 */

#ifndef __BSP_KEY_FILTER_H__
#define __BSP_KEY_FILTER_H__

/**
 * @brief  按键初始化
 * @return void
 */
void key_filter_init(void);

/**
 * @brief  定时器初始化用于消抖,默认关闭定时器
 * @param  value 加载寄存器,倒计数值
 * @return void
 */
void timer_filter_init(unsigned int value);

/**
 * @brief  关闭定时器
 * @return void
 */
void timer_filter_stop(void);

/**
 * @brief  重启定时器
 * @param  value 加载寄存器,倒计数值
 * @return void
 */
void timer_filter_restart(unsigned int value);

/**
 * @brief  定时器中断处理函数
 * @return void
 */
void timer_filter_irqhandler(void);

/**
 * @brief  GPIO中断处理函数
 * @return void
 */
void gpio1_16_31_irqhandler(void);

#endif // __BSP_KEY_FILTER_H__