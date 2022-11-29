/**
 * @file    : bsp_delay.c
 * @author  : KeeneChen
 * @date    : 2022.11.15-22:03:15
 * @details : 高精度定时器
 */

#include "bsp_delay.h"
#include "bsp_int.h"
#include "bsp_led.h"
#include "imx6ull.h"

void delay_init(void)
{
    /* 1.将GPT1_CR寄存器清零 */
    GPT1->CR = 0;

    /* 2.设置软复位 */
    GPT1->CR = (1 << 15);
    while ((GPT1->CR >> 15) & 0x01) // 等待复位完成
        ;

    /**
     * 3.GPT的CR寄存器,GPT通用设置
     * bit [22:20]:	000 输出比较1的输出功能关闭,也就是对应的引脚没反应
     * bit 9      : 0   Restart模式,当CNT等于OCR1的时候就产生中断
     * bit [8:6]  : 001 GPT时钟源选择ipg_clk=66Mhz
     * bit 1      : 1   重置计数器
     */
    GPT1->CR = (1 << 1 | 1 << 6);

    /**
     * 4.GPT的PR寄存器,GPT的分频设置
     * bit11:0  设置分频值,设置为0表示1分频,
     *          以此类推,最大可以设置为0XFFF,也就是最大4096分频
     */
    GPT1->PR = 65; // 65为66分频,GPT1时钟为66M/(65+1)=1MHz

    /**
     * 5.GPT的OCR1寄存器,设置为最大用于延时
     * 1Mhz每加一经过1us,0XFFFFFFFFus = 4294967296us = 4295s = 71.5min
     */
    GPT1->OCR[0] = 0xFFFFFFFF;

    /* 8.打开GPT1_CR寄存器 */
    GPT1->CR |= (1 << 0);

#if 0
    /**
     * 5.GPT的OCR1寄存器,GPT的输出比较1比较计数值,
     * 当GPT的计数值等于OCR1里面值时候,输出比较1就会发生中断
     * 这里定时500ms产生中断,因此就应该为1000000/2=500000;
     */
    GPT1->OCR[0] = 500000;

    /**
     * 6.GPT的IR寄存器,使能通道1的比较中断
     * bit0: 0 使能输出比较中断
     */
    GPT1->IR |= (1 << 0);

    /* 7.使能GIC里面相应的中断,并且注册中断处理函数 */
    GIC_EnableIRQ(GPT1_IRQn);                                                           // 使能GIC中对应的中断
    system_register_irqhandler(GPT1_IRQn, (system_irq_handler_t)gpt1_irqhandler, NULL); // 注册中断服务函数

    /* 8.打开GPT1_CR寄存器 */
    GPT1->CR |= (1 << 0);
#endif
}

#if 0
void gpt1_irqhandler(unsigned int gicc_iar, void* param)
{
    static unsigned char state = 0;

    state = !state;

    /**
     * GPT的SR寄存器，状态寄存器
     * bit2: 1 输出比较1发生中断
     */
    if (GPT1->SR & (1 << 0)) {
        led_switch(LED0, state);
    }

    GPT1->SR |= 1 << 0; /* 清除中断标志位 */
}
#endif

void delay_us(unsigned int us)
{
    unsigned long old_cnt = 0, new_cnt = 0, tcnt_val = 0;
    old_cnt = GPT1->CNT;
    // 定时器溢出处理,统计时间间隔
    while (1) {
        new_cnt = GPT1->CNT;
        if (new_cnt != old_cnt) {
            if (new_cnt > old_cnt)
                tcnt_val += new_cnt - old_cnt;
            else
                tcnt_val += 0xFFFFFFFF - old_cnt + new_cnt;
            old_cnt = new_cnt;
            if (tcnt_val >= us)
                break;
        }
    }
}

void delay_ms(unsigned int ms)
{
    for (int i = 0; i < ms; i++)
        delay_us(1000);
}

void delay_short(volatile unsigned int n)
{
    while (n--)
        ;
}

void delay(volatile unsigned int n)
{
    while (n--) {
        delay_short(0x7FF);
    }
}