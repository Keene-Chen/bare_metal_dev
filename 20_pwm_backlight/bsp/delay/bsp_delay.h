/**
 * @file    : bsp_delay.h
 * @author  : KeeneChen
 * @date    : 2022.11.15-22:03:15
 * @details : 高精度定时器
 */

#ifndef __BSP_DELAY_H__
#define __BSP_DELAY_H__

/**
 * @brief  GPT1定时器初始化
 * @return void
 */
void delay_init(void);

/**
 * @brief  GPT1定时器延时us
 * @return void
 */
void delay_us(unsigned int us);

/**
 * @brief  GPT1定时器延时ms
 * @return void
 */
void delay_ms(unsigned int ms);

// void gpt1_irqhandler(unsigned int gicc_iar, void* param);

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