/**
 * @file    : led.c
 * @author  : KeeneChen
 * @date    : 2022.11.03-19:56:16
 * @details : led
 */

#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "fsl_iomuxc.h"

int main(void)
{
    clk_enable(); // 使能所有的时钟
    led_init();   // 初始化led

    while (1) {
        led_switch(LED0, ON);
        delay(200); // 延时大约500ms

        led_switch(LED0, OFF);
        delay(200); // 延时大约500ms
    }

    return 0;
}