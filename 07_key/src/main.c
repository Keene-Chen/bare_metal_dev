/**
 * @file    : main.c
 * @author  : KeeneChen
 * @date    : 2022.11.05-18:59:43
 * @details : 按键输入
 */

#include "bsp_beep.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include "fsl_iomuxc.h"

int main(void)
{
    clk_enable(); // 使能所有的时钟
    led_init();   // 初始化led
    beep_init();  // 初始化蜂鸣器
    key_init();   // 初始化按键输入

    int i = 0, key_val = 0, led_state = 0, beep_state = 0;

    while (1) {
        // 按键控制蜂鸣器
        key_val = key_getvalue();
        if (key_val) {
            switch ((key_val)) {
            case KEY0_VALUE:
                beep_state = !beep_state;
                beep_switch(beep_state);
                break;
            default:
                break;
            }
        }

        // 每隔500ms切换LED状态
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