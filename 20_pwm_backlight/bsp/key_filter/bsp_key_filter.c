/**
 * @file    : bsp_key_filter.c
 * @author  : KeeneChen
 * @date    : 2022.11.15-15:57:29
 * @details : 定时器按键消抖实验
 */

#include "bsp_key_filter.h"
#include "bsp_beep.h"
#include "bsp_exti.h"
#include "bsp_gpio.h"
#include "bsp_int.h"

void key_filter_init(void)
{
    // 1.设置IO复用
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0); // 复用为GPIO1_IO18
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);

    // 2.初始化GPIO为中断模式
    gpio_pin_conf_t key_config;
    key_config.direction      = GPIO_DIGITAL_INPUT;
    key_config.interrupt_mode = kGPIO_IntFallingEdge;
    key_config.output_logic   = 1;
    gpio_init(GPIO1, 18, &key_config);

    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn); // 使能GIC中对应的中断
    system_register_irqhandler(GPIO1_Combined_16_31_IRQn, (system_irq_handler_t)gpio1_16_31_irqhandler, NULL);
    gpio_interrupt_enable(GPIO1, 18); // 使能GPIO1_IO18的中断功能

    // 3.初始化定时器
    timer_filter_init(66000000 / 100);
}

void timer_filter_init(unsigned int value)
{
    /* 清零EPIT1_CR寄存器 */
    EPIT1->CR = 0;

    /**
     * CR寄存器:
     * bit [25:24]: 01 时钟源选择Peripheral clock=66MHz
     * bit [15:4]:  frac 分频值
     * bit 3: 1 当计数器到0的话从LR重新加载数值
     * bit 2: 1 比较中断使能
     * bit 1: 1 初始计数值来源于LR寄存器值
     * bit 0: 0 关闭EPIT1
     */
    EPIT1->CR = (1 << 24 | 1 << 3 | 1 << 2 | 1 << 1);

    EPIT1->LR   = value; // 倒计数值
    EPIT1->CMPR = 0;     // 比较寄存器,当计数器值和此寄存器值相等的话就会产生中断

    /* 使能GIC中对应的中断 */
    GIC_EnableIRQ(EPIT1_IRQn);

    /* 注册中断服务函数 */
    system_register_irqhandler(EPIT1_IRQn, (system_irq_handler_t)timer_filter_irqhandler, NULL);
}

void timer_filter_stop(void)
{
    EPIT1->CR &= ~(1 << 0);
}

void timer_filter_restart(unsigned int value)
{
    EPIT1->CR &= ~(1 << 0);
    EPIT1->LR = value;
    EPIT1->CR |= (1 << 0);
}

void timer_filter_irqhandler(void)
{
    static unsigned char state = OFF;

    if (EPIT1->SR & (1 << 0)) // 判断比较事件是否发生
    {
        timer_filter_stop();               // 关闭定时器
        if (gpio_pin_read(GPIO1, 18) == 0) // KEY0
        {
            state = !state;
            beep_switch(state); // 反转蜂鸣器
        }
    }

    EPIT1->SR |= (1 << 0); // 清除中断标志位
}

void gpio1_16_31_irqhandler(void)
{
    timer_filter_restart(66000000 / 100);  // 开启定时器
    gpio_interrupt_clear_flags(GPIO1, 18); // 清除中断标志位
}
