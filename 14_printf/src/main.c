/**
 * @file    : bsp_uart.c
 * @author  : KeeneChen
 * @date    : 2022.11.19-13:31:48
 * @details : printf 移植
 */

#include "bsp.h"
#include "stdio.h"

int main(void)
{
    int_init();   // 初始化中断
    clk_init();   // 初始化时钟
    clk_enable(); // 使能所有的时钟
    led_init();   // 初始化led
    delay_init(); // 初始化GPT1定时器
    uart_init();  // 初始化UART1

    unsigned char state = OFF;

    int a = 0, b = 0;

    while (1) {
        printf("输入两个整数，使用空格隔开:");
        scanf("%d %d", &a, &b);
        printf("\r\n数据%d + %d = %d\r\n\r\n", a, b, a + b);

        state = !state;
        led_switch(LED0, state);
    }

    return 0;
}