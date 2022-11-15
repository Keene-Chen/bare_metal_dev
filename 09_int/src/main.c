/**
 * @file    : main.c
 * @author  : KeeneChen
 * @date    : 2022.11.09-19:01:37
 * @details : 按键中断实验
 */

#include "bsp.h"

int main(void)
{
    int_init();   // 初始化中断
    clk_init();   // 初始化时钟
    clk_enable(); // 使能所有的时钟
    led_init();   // 初始化led
    beep_init();  // 初始化蜂鸣器
    key_init();   // 初始化按键输入
    exit_init();  // 初始化按键中断

    uint8_t state = OFF;

    while (1) {
        state = !state;
        led_switch(LED0, state);
        delay(500);
    }

    return 0;
}