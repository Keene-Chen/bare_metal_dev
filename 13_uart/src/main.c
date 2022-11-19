/**
 * @file    : bsp_uart.c
 * @author  : KeeneChen
 * @date    : 2022.11.18-20:40:38
 * @details : UART串口通信实验
 */

#include "bsp.h"

int main(void)
{
    int_init();   // 初始化中断
    clk_init();   // 初始化时钟
    clk_enable(); // 使能所有的时钟
    led_init();   // 初始化led
    delay_init(); // 初始化GPT1定时器
    uart_init();  // 初始化UART1

    unsigned char state = OFF;
    unsigned char ch    = 0;

    while (1) {
        // puts("请输入一个字符:");
        puts("Please enter a character:");
        ch = getc();
        putc(ch); // 回显功能
        puts("\r\n");

        // 显示输入的字符
        // puts("您输入的字符为:");
        puts("The character you entered is:");
        putc(ch);
        puts("\r\n\r\n");

        state = !state;
        led_switch(LED0, state);
    }

    return 0;
}