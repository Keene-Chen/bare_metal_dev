/**
 * @file    : main.c
 * @author  : KeeneChen
 * @date    : 2022.11.03-19:56:16
 * @details : 蜂鸣器
 */

#include "bsp_beep.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "fsl_iomuxc.h"

int main(void)
{
    clk_enable(); // 使能所有的时钟
    led_init();   // 初始化led
    beep_init();  // 初始化蜂鸣器
    while (1) {
        led_switch(LED0, ON);
        beep_switch(ON);
        delay(500); // 延时大约500ms

        led_switch(LED0, OFF);
        beep_switch(OFF);
        delay(30000); // 延时大约500ms
    }

    return 0;
}