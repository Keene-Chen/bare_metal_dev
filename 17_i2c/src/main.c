/**
 * @file    : main.c
 * @author  : KeeneChen
 * @date    : 2022.11.24-17:02:18
 * @details : I2C驱动
 */

/* 外部模块(优先引用) */
#include "stdio.h"

/* 内部模块 */
#include "bsp.h"

int main(void)
{
    int_init();   // 初始化中断
    clk_init();   // 初始化时钟
    clk_enable(); // 使能所有的时钟
    led_init();   // 初始化led
    delay_init(); // 初始化GPT1定时器
    uart_init();  // 初始化UART1
    lcd_init();   // 初始化LCD

    unsigned short ir, als, ps;
    unsigned char state  = OFF;
    tftlcd_dev.forecolor = LCD_RED;
    lcd_show_string(30, 50, 200, 16, 16, (char*)"ALPHA-IMX6U IIC TEST");
    lcd_show_string(30, 70, 200, 16, 16, (char*)"AP3216C TEST");
    lcd_show_string(30, 90, 200, 16, 16, (char*)"KeeneChen");
    lcd_show_string(30, 110, 200, 16, 16, (char*)"2022.11.25-13:53:53");

    while (ap3216c_init()) /* 检测不到AP3216C */
    {
        lcd_show_string(30, 130, 200, 16, 16, (char*)"AP3216C Check Failed!");
        delay_ms(500);
        lcd_show_string(30, 130, 200, 16, 16, (char*)"Please Check!        ");
        delay_ms(500);
    }

    lcd_show_string(30, 130, 200, 16, 16, (char*)"AP3216C Ready!");
    lcd_show_string(30, 160, 200, 16, 16, (char*)" IR:");
    lcd_show_string(30, 180, 200, 16, 16, (char*)" PS:");
    lcd_show_string(30, 200, 200, 16, 16, (char*)"ALS:");
    tftlcd_dev.forecolor = LCD_BLUE;
    while (1) {
        ap3216c_read_data(&ir, &ps, &als);      // 读取数据
        lcd_show_num(30 + 32, 160, ir, 5, 16);  // 显示IR数据
        lcd_show_num(30 + 32, 180, ps, 5, 16);  // 显示PS数据
        lcd_show_num(30 + 32, 200, als, 5, 16); // 显示ALS数据
        delay_ms(120);
        state = !state;
        led_switch(LED0, state);
    }

    return 0;
}