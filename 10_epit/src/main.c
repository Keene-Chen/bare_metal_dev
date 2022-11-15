/**
 * @file    : main.c
 * @author  : KeeneChen
 * @date    : 2022.11.15-10:54:55
 * @details : EPIT定时500ms点灯
 */

#include "bsp.h"

int main(void)
{
    int_init();                  // 初始化中断
    clk_init();                  // 初始化时钟
    clk_enable();                // 使能所有的时钟
    led_init();                  // 初始化led
    epit1_init(0, 66000000 / 2); // 初始化EPIT定时器,计数值为:66000000/2,定时周期为500ms

    while (1) {
        delay(500);
    }

    return 0;
}