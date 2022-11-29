/**
 * @file    : bsp_backlight.c
 * @author  : KeeneChen
 * @date    : 2022.11.29-10:54:47
 * @details : pwm调节lcd背光
 */

#include "imx6ull.h"

#include "bsp_backlight.h"
#include "bsp_gpio.h"
#include "bsp_int.h"

backlight_dev_t backlight_dev;

void backlight_init(void)
{
    /* 1.IO初始化 */
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO08_PWM1_OUT, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO08_PWM1_OUT, 0xB090);

    /**
     * 2.初始化寄存器PWMCR
     * PWM时钟源为ipg_clk=66Mhz,设置66分频,时钟频率为1Mhz
     *
     * bit [27:26]: 01 当FIFO中空余位置大于等于2的时候FIFO空标志值位
     * bit [25]   : 0  停止模式下PWM不工作
     * bit [24]   : 0  休眠模式下PWM不工作
     * bit [23]   : 0  等待模式下PWM不工作
     * bit [22]   : 0  调试模式下PWM不工作
     * bit [21]   : 0  关闭字节交换
     * bit [20]   : 0  关闭半字数据交换
     * bit [19:18]: 00 PWM输出引脚在计数器重新计数的时候输出高电平
     *                 在计数器计数值达到比较值以后输出低电平
     * bit [17:16]: 01 PWM时钟源选择IPG CLK = 66MHz
     * bit [15:4] : 65 分频系数为65+1=66，PWM时钟源 = 66MHZ/66=1MHz
     * bit [3]	  : 0  PWM不复位
     * bit [2:1]  : 00 FIFO中的sample数据每个只能使用一次。
     * bit [0]    : 0  先关闭PWM，后面再使能
     */
    PWM1->PWMCR = 0;
    PWM1->PWMCR |= (1 << 26) | (1 << 16) | (65 << 4);
    pwm1_set_period_value(1000); // 设置PWM频率为1KHz

    /* 3.设置默认占空比 */
    backlight_dev.pwm_duty = 50;
    for (int i = 0; i < 4; i++)
        pwm1_set_duty(backlight_dev.pwm_duty);

    /* 4.使能FIFO空中断 */
    PWM1->PWMIR |= (1 << 0); // 使能PWM1的FIFO空中断
    system_register_irqhandler(PWM1_IRQn, (system_irq_handler_t)pwm1_irqhandler, NULL);
    GIC_EnableIRQ(PWM1_IRQn);
    PWM1->PWMSR = 0; // PWM中断状态寄存器清零

    /* 5.使能PWM */
    pwm1_enable();
}

void pwm1_set_sample_value(unsigned int value)
{
    PWM1->PWMSAR = (value & 0xFFFF);
}

void pwm1_set_period_value(unsigned int value)
{
    unsigned int reg = 0;
    if (value < 2)
        reg = 2;
    else
        reg = value - 2;
    PWM1->PWMPR = (reg & 0xFFFF); // 取低16位
}

void pwm1_set_duty(unsigned char duty)
{
    backlight_dev.pwm_duty = duty;
    unsigned short period  = PWM1->PWMPR + 2;
    unsigned short sample  = period * backlight_dev.pwm_duty / 100;
    pwm1_set_sample_value(sample);
}

void pwm1_irqhandler(void)
{
    if (PWM1->PWMSR & (1 << 3)) { // FIFO空中断
        pwm1_set_duty(backlight_dev.pwm_duty);
        PWM1->PWMSR |= (1 << 3); // 清零中断标志位
    }
}

void pwm1_enable(void)
{
    PWM1->PWMCR |= (1 << 0);
}