/**
 * @file    : bsp_led.c
 * @author  : KeeneChen
 * @date    : 2022.11.05-10:17:20
 * @details : LED模块
 */

#include "bsp_led.h"
#include "bsp_clk.h"
#include "cc.h"
#include "fsl_iomuxc.h"

void led_init(void)
{
    // 1.初始化IO复用
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0); // 复用为GPIO1_IO03

    /**
     * 2.配置GPIO1_IO03的IO属性
     * [16]:0 HYS关闭
     * [15:14]: 00 默认下拉
     * [13]: 0 kepper功能
     * [12]: 1 pull/keeper使能
     * [11]: 0 关闭开路输出
     * [7:6]: 10 速度100Mhz
     * [5:3]: 110 R0/6驱动能力
     * [0]: 0 低转换率
     */
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0x10B0);

    // 3.初始化GPIO
    GPIO1->GDIR = 0x0000008; // GPIO1_IO03设置为输出

    // 4.设置GPIO1_IO03输出低电平,打开LED0
    GPIO1->DR &= ~(1 << 3);
}

void led_on(void)
{
    // 将GPIO1_DR的bit3清零
    GPIO1->DR &= ~(1 << 3);
}

void led_off(void)
{
    // 将GPIO1_DR的bit3置1
    GPIO1->DR |= (1 << 3);
}

void led_switch(int led, int status)
{
    switch (led) {
    case LED0:
        if (status == ON)
            GPIO1->DR &= ~(1 << 3);
        else if (status == OFF)
            GPIO1->DR |= (1 << 3);
        break;

    default:
        break;
    }
}