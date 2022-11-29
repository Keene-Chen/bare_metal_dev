/**
 * @file    : bsp_beep.c
 * @author  : KeeneChen
 * @date    : 2022.11.05-10:47:20
 * @details : 蜂鸣器驱动模块
 */

#include "bsp_beep.h"
#include "bsp_gpio.h"
#include "fsl_iomuxc.h"

void beep_init(void)
{
    // 1.初始化IO复用
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0);

    // 2.配置GPIO5_IO01 IO属性
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0x10B0);

    // 3.配置GPIO5_IO01为输出
    gpio_pin_conf_t beep_config;
    beep_config.direction    = GPIO_DIGITAL_OUTPUT;
    beep_config.output_logic = 1;
    gpio_init(GPIO5, 1, &beep_config);
}

void beep_switch(int status)
{
    if (status == ON)
        gpio_pin_write(GPIO5, 1, 0); // 打开蜂鸣器
    else if (status == OFF)
        gpio_pin_write(GPIO5, 1, 1); // 关闭蜂鸣器
}