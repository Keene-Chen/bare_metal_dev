/**
 * @file    : bsp_gpio.h
 * @author  : KeeneChen
 * @date    : 2022.11.09-16:18:52
 * @details : 通用GPIO头文件
 */

#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__

#include "cc.h"
#include "fsl_iomuxc.h"

// 定义枚举和GPIO配置结构体
typedef enum {
    GPIO_DIGITAL_INPUT  = 0U,
    GPIO_DIGITAL_OUTPUT = 1U,
} gpio_pin_direction_um;

// 描述中断触发类型
typedef enum {
    kGPIO_NoIntMode              = 0U, // 无中断功能
    kGPIO_IntLowLevel            = 1U, // 低电平触发
    kGPIO_IntHighLevel           = 2U, // 高电平触发
    kGPIO_IntRisingEdge          = 3U, // 上升沿触发
    kGPIO_IntFallingEdge         = 4U, // 下降沿触发
    kGPIO_IntRisingOrFallingEdge = 5U, // 上升沿和下降沿都触发
} gpio_interrupt_mode_um;

typedef struct {
    gpio_pin_direction_um direction;       // 引脚方向
    uint8_t output_logic;                  // 逻辑输出
    gpio_interrupt_mode_um interrupt_mode; // 中断方式
} gpio_pin_conf_t;

/**
 * @brief  GPIO初始化
 * @param  base GPIO组
 * @param  pin  GPIO组下具体引脚
 * @return void
 */
void gpio_init(GPIO_Type* base, int pin, gpio_pin_conf_t* config);

/**
 * @brief  控制GPIO高低电平
 * @param  base  GPIO组
 * @param  pin   GPIO组下具体引脚
 * @param  value 写入引脚的值
 * @return void
 */
void gpio_pin_write(GPIO_Type* base, int pin, int value);

/**
 * @brief  读取GPIO电平
 * @param  base GPIO组
 * @param  pin  GPIO组下具体引脚
 * @return int  引脚电平
 */
int gpio_pin_read(GPIO_Type* base, int pin);

/**
 * @brief  使能指定IO中断
 * @param  base GPIO组
 * @param  pin  GPIO组下具体引脚
 * @return void
 */
void gpio_interrupt_enable(GPIO_Type* base, int pin);

/**
 * @brief  禁止指定IO中断
 * @param  base GPIO组
 * @param  pin  GPIO组下具体引脚
 * @return void
 */
void gpio_interrupt_disable(GPIO_Type* base, int pin);

/**
 * @brief  清除指定IO中断
 * @param  base GPIO组
 * @param  pin  GPIO组下具体引脚
 * @return void
 */
void gpio_interrupt_clear_flags(GPIO_Type* base, int pin);

/**
 * @brief  GPIO中断初始化函数
 * @param  base         GPIO组
 * @param  pin          GPIO组下具体引脚
 * @param  pin_int_mode 中断类型
 * @return void
 */
void gpio_interrupt_config(GPIO_Type* base, int pin,
                           gpio_interrupt_mode_um pin_int_mode);

#endif // __BSP_GPIO_H__