/**
 * @file    : led.c
 * @author  : KeeneChen
 * @date    : 2022.11.03-11:05:42
 * @details : 1
 */

#include "led.h"

/**
 * @brief  使能I.MX6U所有外设时钟
 * @return void
 */
void clk_enable(void)
{
    CCM_CCGR0 = 0xFFFFFFFF;
    CCM_CCGR1 = 0xFFFFFFFF;
    CCM_CCGR2 = 0xFFFFFFFF;
    CCM_CCGR3 = 0xFFFFFFFF;
    CCM_CCGR4 = 0xFFFFFFFF;
    CCM_CCGR5 = 0xFFFFFFFF;
    CCM_CCGR6 = 0xFFFFFFFF;
}

/**
 * @brief  初始化LED对应的GPIO
 * @return void
 */
void led_init(void)
{
    // 1.初始化IO复用
    SW_MUX_GPIO1_IO03 = 0x5; // 复用为GPIO1_IO03

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
    SW_PAD_GPIO1_IO03 = 0x10B0;

    // 3.初始化GPIO
    GPIO1_GDIR = 0x0000008; // GPIO1_IO03设置为输出

    // 4.设置GPIO1_IO03输出低电平，打开LED0
    GPIO1_DR = 0x0;
}

/**
 * @brief  打开LED灯
 * @return void
 */
void led_on(void)
{
    // 将GPIO1_DR的bit3清零
    GPIO1_DR &= ~(1 << 3);
}

/**
 * @brief  关闭LED灯
 * @return void
 */
void led_off(void)
{
    // 将GPIO1_DR的bit3置1
    GPIO1_DR |= (1 << 3);
}

/**
 * @brief  短时间延时函数
 * @param  n 要延时循环次数(空操作循环次数，模式延时)
 * @return void
 */
static void delay_short(volatile unsigned int n)
{
    while (n--)
        ;
}

/**
 * @brief  延时函数,在396Mhz的主频下,延时时间大约为1ms
 * @param  n 要延时的ms数
 * @return void
 */
void delay(volatile unsigned int n)
{
    while (n--) {
        delay_short(0x7FF);
    }
}

int main(void)
{
    clk_enable(); // 使能所有的时钟
    led_init(); // 初始化led

    while (1) {
        led_off(); // 关闭LED
        delay(500); // 延时大约500ms

        led_on(); // 打开LED
        delay(500); // 延时大约500ms
    }

    return 0;
}