/**
 * @file    : bsp_delay.h
 * @author  : KeeneChen
 * @date    : 2022.11.15-22:03:15
 * @details : 高精度定时器
 */

#include "bsp.h"
#include "bsp_delay.h"
#include "bsp_led.h"

int main(void)
{
    int_init();   // 初始化中断
    clk_init();   // 初始化时钟
    clk_enable(); // 使能所有的时钟
    led_init();   // 初始化led
    delay_init(); // 初始化GPT1定时器

    unsigned char state = OFF;

    while (1) {
        state = !state;
        led_switch(LED0, state);
        delay_ms(500);
    }

    return 0;
}