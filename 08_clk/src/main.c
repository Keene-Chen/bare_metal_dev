/**
 * @file    : main.c
 * @author  : KeeneChen
 * @date    : 2022.11.07-20:24:22
 * @details : 系统主频与时钟配置
 */

#include "bsp_beep.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_key.h"
#include "bsp_led.h"

int main(void)
{
    clk_init();   // 初始化时钟
    clk_enable(); // 使能所有的时钟
    led_init();   // 初始化led
    beep_init();  // 初始化蜂鸣器
    key_init();   // 初始化按键输入

    int i = 0, key_val = 0, led_state = OFF, beep_state = OFF;

    while (1) {
        // 按键控制蜂鸣器
        key_val = key_getvalue();
        if (key_val) {
            switch (key_val) {
            case KEY0_VALUE:
                beep_state = !beep_state;
                beep_switch(beep_state);
                break;
            }
        }

        // 每隔500ms切换LED状态,由于主频切换到792MHz是原来的两倍,所以延时也快了两倍
        i++;
        if (i == 50) {
            i         = 0;
            led_state = !led_state;
            led_switch(LED0, led_state);
        }
        delay(10);
    }

    return 0;
}