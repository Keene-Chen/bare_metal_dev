/**
 * @file    : main.c
 * @author  : KeeneChen
 * @date    : 2022.11.15-15:54:56
 * @details : 定时器按键消抖实验
 */

#include "bsp.h"

int main(void)
{
    int_init();        // 初始化中断
    clk_init();        // 初始化时钟
    clk_enable();      // 使能所有的时钟
    led_init();        // 初始化led
    beep_init();       // 初始化蜂鸣器
    key_filter_init(); // 初始化定时器

    unsigned char state = OFF;

    while (1) {
        state = !state;
        led_switch(LED0, state);
        delay(500);
    }

    return 0;
}