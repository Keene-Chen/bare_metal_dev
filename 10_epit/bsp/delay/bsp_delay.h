/**
 * @file    : bsp_delay.h
 * @author  : KeeneChen
 * @date    : 2022.11.05-10:16:59
 * @details : 延时模块
 */

#ifndef __BSP_DELAY_H__
#define __BSP_DELAY_H__

/**
 * @brief  短时间延时函数
 * @param  n 要延时循环次数(空操作循环次数，模式延时)
 * @return void
 */
void delay_short(volatile unsigned int n);

/**
 * @brief  延时函数,在396Mhz的主频下,延时时间大约为1ms
 * @param  n 要延时的ms数
 * @return void
 */
void delay(volatile unsigned int n);

#endif // __BSP_DELAY_H__