/**
 * @file    : bsp_uart.c
 * @author  : KeeneChen
 * @date    : 2022.11.21-15:59:01
 * @details : LCD驱动
 */

/* 外部模块(优先引用) */
#include "stdio.h"

/* 内部模块 */
#include "bsp.h"
#include "bsp_delay.h"
#include "bsp_lcd.h"
#include "bsp_lcd_api.h"

/* 背景颜色索引 */
unsigned int backcolor[] = { LCD_WHITE,     LCD_GRAY,      LCD_BLACK,      LCD_BROWN,      LCD_ORANGE,      LCD_TRANSPARENT,  LCD_BLUE,        LCD_GREEN,         LCD_RED,
                             LCD_CYAN,      LCD_MAGENTA,   LCD_YELLOW,     LCD_LIGHT_BLUE, LCD_LIGHT_GREEN, LCD_LIGHT_RED,    LCD_LIGHT_CYAN,  LCD_LIGHT_MAGENTA, LCD_LIGHT_YELLOW,
                             LCD_LIGHT_GRAY, LCD_DARK_BLUE, LCD_DARK_GREEN, LCD_DARK_RED,   LCD_DARK_CYAN,   LCD_DARK_MAGENTA, LCD_DARK_YELLOW, LCD_DARK_GRAY };

int main(void)
{
    int_init();   // 初始化中断
    clk_init();   // 初始化时钟
    clk_enable(); // 使能所有的时钟
    led_init();   // 初始化led
    delay_init(); // 初始化GPT1定时器
    uart_init();  // 初始化UART1
    lcd_init();   // 初始化LCD

    unsigned char state = OFF, index = 0;
    tftlcd_dev.forecolor = LCD_RED;
    tftlcd_dev.backcolor = LCD_WHITE;
    while (1) {
        lcd_clear(backcolor[index % 26]);
        // delay_ms(1);
        lcd_show_string(10, 40, 260, 32, 32, (char*)"MINI I.MX6ULL");
        lcd_show_string(10, 80, 240, 24, 24, (char*)"RGBLCD TEST");
        lcd_show_string(10, 110, 240, 16, 16, (char*)"Keene Chen");
        lcd_show_string(10, 130, 240, 12, 12, (char*)"2022.11.22-19:37:02");
        index++;

        state = !state;
        led_switch(LED0, state);
        delay_ms(3000);
    }

    return 0;
}