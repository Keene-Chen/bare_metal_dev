/**
 * @file    : bsp_led.h
 * @author  : KeeneChen
 * @date    : 2022.11.05-10:17:39
 * @details : LED模块
 */

#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#define LED0 0 
#define LED1 1 
#define LED2 2 

/**
 * @brief  初始化LED对应的GPIO
 * @return void
 */
void led_init(void);

/**
 * @brief  打开LED灯
 * @return void
 */
void led_on(void);

/**
 * @brief  关闭LED灯
 * @return void
 */
void led_off(void);

/**
 * @brief  控制LED灯
 * @return void
 */
void led_switch(int led,int status);

#endif // __BSP_LED_H__