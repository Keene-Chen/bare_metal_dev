#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__

#include "cc.h"
#include "fsl_iomuxc.h"

// 定义枚举和GPIO配置结构体
typedef enum {
    GPIO_DIGITAL_INPUT  = 0U,
    GPIO_DIGITAL_OUTPUT = 1U,
} gpio_pin_direction_um;

typedef struct {
    gpio_pin_direction_um direction; // 引脚方向
    uint8_t output_logic;
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

#endif // __BSP_GPIO_H__