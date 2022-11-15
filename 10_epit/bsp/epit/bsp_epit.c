/**
 * @file    : bsp_epit.c
 * @author  : KeeneChen
 * @date    : 2022.11.15-10:29:13
 * @details : bsp_epit
 */

#include "bsp_epit.h"
#include "MCIMX6Y2.h"
#include "bsp_int.h"
#include "bsp_led.h"
#include <stdlib.h>

void epit1_init(int frac, int value)
{
    if (frac > 4095)
        frac = 4095;

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
    EPIT1->CR = (1 << 24 | frac << 4 | 1 << 3 | 1 << 2 | 1 << 1);

    EPIT1->LR   = value; // 倒计数值
    EPIT1->CMPR = 0;     // 比较寄存器,当计数器值和此寄存器值相等的话就会产生中断

    /* 使能GIC中对应的中断 */
    GIC_EnableIRQ(EPIT1_IRQn);

    /* 注册中断服务函数 */
    system_register_irqhandler(EPIT1_IRQn, (system_irq_handler_t)epit1_irqhandler, NULL);

    /* 使能EPIT1 */
    EPIT1->CR |= (1 << 0);
}

void epit1_irqhandler(unsigned int gicc_iar, void* user_param)
{
    static uint8_t state = OFF;

    state = !state;
    if (EPIT1->SR & (1 << 0)) { // 比较中断标志位是否发生中断
        led_switch(LED0, state);
    }
    EPIT1->SR |= (1 << 0); // 清除中断标志位
}
