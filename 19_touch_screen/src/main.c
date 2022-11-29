/**
 * @file    : main.c
 * @author  : KeeneChen
 * @date    : 2022.11.28-15:50:01
 * @details : ft5426触摸屏驱动
 */

/* 外部模块(优先引用) */
#include "stdio.h"

/* 内部模块 */
#include "bsp.h"

/**
 * @brief  使能I.MX6U的硬件NEON和FPU
 * @return void
 */
void imx6ul_hardfpu_enable(void)
{
    uint32_t cpacr;
    uint32_t fpexc;

    /* 使能NEON和FPU */
    cpacr = __get_CPACR();
    cpacr = (cpacr & ~(CPACR_ASEDIS_Msk | CPACR_D32DIS_Msk)) | (3UL << CPACR_cp10_Pos)
            | (3UL << CPACR_cp11_Pos);
    __set_CPACR(cpacr);
    fpexc = __get_FPEXC();
    fpexc |= 0x40000000UL;
    __set_FPEXC(fpexc);
}

static void touch_draw_line(unsigned short x, unsigned short y)
{
    lcd_draw_point(x, y, LCD_RED);
    if (x < tftlcd_dev.width - 1) {
        lcd_draw_point(x + 1, y, LCD_RED);
        lcd_draw_point(x + 2, y, LCD_RED);
    }
    if (y < tftlcd_dev.height - 1) {
        lcd_draw_point(x, y + 1, LCD_RED);
        lcd_draw_point(x, y + 2, LCD_RED);
    }
    if ((x < tftlcd_dev.width - 1) && (y < tftlcd_dev.height - 1)) {
        lcd_draw_point(x + 1, y + 1, LCD_RED);
        lcd_draw_point(x + 2, y + 2, LCD_RED);
    }
}

int main(void)
{
    imx6ul_hardfpu_enable(); // 使能I.MX6U的硬件浮点
    int_init();              // 初始化中断
    clk_init();              // 初始化系统时钟
    clk_enable();            // 使能所有的时钟
    delay_init();            // 初始化延时
    led_init();              // 初始化led
    uart_init();             // 初始化串口，波特率115200
    lcd_init();              // 初始化LCD
    ft5426_init();           // 初始化ft5426

    tftlcd_dev.forecolor = LCD_RED;
    lcd_show_string(50, 10, 400, 24, 24, (char*)"ALPHA-IMX6U Touch TEST");
    lcd_show_string(50, 40, 200, 16, 16, (char*)"FT5426 TEST");
    lcd_show_string(50, 60, 200, 16, 16, (char*)"Keene Chen");
    lcd_show_string(50, 80, 200, 16, 16, (char*)"2022.11.28-16:53:57");

#if 1
    lcd_show_string(50, 110, 400, 16, 16, (char*)"TP Num	:");
    lcd_show_string(50, 130, 200, 16, 16, (char*)"Point0 X:");
    lcd_show_string(50, 150, 200, 16, 16, (char*)"Point0 Y:");
    lcd_show_string(50, 170, 200, 16, 16, (char*)"Point1 X:");
    lcd_show_string(50, 190, 200, 16, 16, (char*)"Point1 Y:");
    lcd_show_string(50, 210, 200, 16, 16, (char*)"Point2 X:");
    lcd_show_string(50, 230, 200, 16, 16, (char*)"Point2 Y:");
    lcd_show_string(50, 250, 200, 16, 16, (char*)"Point3 X:");
    lcd_show_string(50, 270, 200, 16, 16, (char*)"Point3 Y:");
    lcd_show_string(50, 290, 200, 16, 16, (char*)"Point4 X:");
    lcd_show_string(50, 310, 200, 16, 16, (char*)"Point4 Y:");
#endif

    unsigned char state = OFF;
    int count           = 0;
    while (1) {
#if 1
        lcd_show_num(50 + 72, 110, ft5426_dev.point_num, 1, 16);
        lcd_show_num(50 + 72, 130, ft5426_dev.x[0], 5, 16);
        lcd_show_num(50 + 72, 150, ft5426_dev.y[0], 5, 16);
        lcd_show_num(50 + 72, 170, ft5426_dev.x[1], 5, 16);
        lcd_show_num(50 + 72, 190, ft5426_dev.y[1], 5, 16);
        lcd_show_num(50 + 72, 210, ft5426_dev.x[2], 5, 16);
        lcd_show_num(50 + 72, 230, ft5426_dev.y[2], 5, 16);
        lcd_show_num(50 + 72, 250, ft5426_dev.x[3], 5, 16);
        lcd_show_num(50 + 72, 270, ft5426_dev.y[3], 5, 16);
        lcd_show_num(50 + 72, 290, ft5426_dev.x[4], 5, 16);
        lcd_show_num(50 + 72, 310, ft5426_dev.y[4], 5, 16);
        touch_draw_line(ft5426_dev.x[0], ft5426_dev.y[0]);
#endif
        delay_ms(10);
        count++;

        if (count == 50) {
            count = 0;
            state = !state;
            led_switch(LED0, state);
        }
    }

    return 0;
}
