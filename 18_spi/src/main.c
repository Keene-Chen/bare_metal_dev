/**
 * @file    : main.c
 * @author  : KeeneChen
 * @date    : 2022.11.27-12:05:30
 * @details : SPI驱动
 */

/* 外部模块(优先引用) */
#include "stdio.h"

/* 内部模块 */
#include "bsp.h"

/*
 * @description	: 指定的位置显示整数数据
 * @param - x	: X轴位置
 * @param - y 	: Y轴位置
 * @param - size: 字体大小
 * @param - num : 要显示的数据
 * @return 		: 无
 */
void integer_display(unsigned short x, unsigned short y, unsigned char size, signed int num)
{
    char buf[200];

    lcd_fill(x, y, x + 50, y + size, tftlcd_dev.backcolor);

    memset(buf, 0, sizeof(buf));
    if (num < 0)
        sprintf(buf, "-%d", -num);
    else
        sprintf(buf, "%d", num);
    lcd_show_string(x, y, 50, size, size, buf);
}

/*
 * @description	: 指定的位置显示小数数据,比如5123，显示为51.23
 * @param - x	: X轴位置
 * @param - y 	: Y轴位置
 * @param - size: 字体大小
 * @param - num : 要显示的数据，实际小数扩大100倍，
 * @return 		: 无
 */
void decimals_display(unsigned short x, unsigned short y, unsigned char size, signed int num)
{
    signed int integ; /* 整数部分 */
    signed int fract; /* 小数部分 */
    signed int uncomptemp = num;
    char buf[200];

    if (num < 0)
        uncomptemp = -uncomptemp;
    integ = uncomptemp / 100;
    fract = uncomptemp % 100;

    memset(buf, 0, sizeof(buf));
    if (num < 0)
        sprintf(buf, "-%d.%d", integ, fract);
    else
        sprintf(buf, "%d.%d", integ, fract);
    lcd_fill(x, y, x + 60, y + size, tftlcd_dev.backcolor);
    lcd_show_string(x, y, 60, size, size, buf);
}

/*
 * @description	: 使能I.MX6U的硬件NEON和FPU
 * @param 		: 无
 * @return 		: 无
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

/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
    unsigned char state = OFF;

    imx6ul_hardfpu_enable(); /* 使能I.MX6U的硬件浮点 			*/
    int_init();              /* 初始化中断(一定要最先调用！) */
    clk_init();              /* 初始化系统时钟 					*/
    delay_init();            /* 初始化延时 					*/
    clk_enable();            /* 使能所有的时钟 					*/
    led_init();              /* 初始化led 					*/
    beep_init();             /* 初始化beep	 				*/
    uart_init();             /* 初始化串口，波特率115200 */
    lcd_init();              /* 初始化LCD 					*/

    tftlcd_dev.forecolor = LCD_RED;
    lcd_show_string(50, 10, 400, 24, 24, (char*)"ALPHA-IMX6U SPI TEST");
    lcd_show_string(50, 40, 200, 16, 16, (char*)"ICM20608 TEST");
    lcd_show_string(50, 60, 200, 16, 16, (char*)"ATOM@ALIENTEK");
    lcd_show_string(50, 80, 200, 16, 16, (char*)"2019/3/27");

    while (icm20608_init()) /* 初始化ICM20608	 			*/
    {
        lcd_show_string(50, 100, 200, 16, 16, (char*)"ICM20608 Check Failed!");
        delay_ms(500);
        lcd_show_string(50, 100, 200, 16, 16, (char*)"Please Check!        ");
        delay_ms(500);
    }

    lcd_show_string(50, 100, 200, 16, 16, (char*)"ICM20608 Ready");

    lcd_show_string(50, 130, 200, 16, 16, (char*)"accel x:");
    lcd_show_string(50, 150, 200, 16, 16, (char*)"accel y:");
    lcd_show_string(50, 170, 200, 16, 16, (char*)"accel z:");
    lcd_show_string(50, 190, 200, 16, 16, (char*)"gyro  x:");
    lcd_show_string(50, 210, 200, 16, 16, (char*)"gyro  y:");
    lcd_show_string(50, 230, 200, 16, 16, (char*)"gyro  z:");
    lcd_show_string(50, 250, 200, 16, 16, (char*)"temp   :");

    lcd_show_string(50 + 181, 130, 200, 16, 16, (char*)"g");
    lcd_show_string(50 + 181, 150, 200, 16, 16, (char*)"g");
    lcd_show_string(50 + 181, 170, 200, 16, 16, (char*)"g");
    lcd_show_string(50 + 181, 190, 200, 16, 16, (char*)"o/s");
    lcd_show_string(50 + 181, 210, 200, 16, 16, (char*)"o/s");
    lcd_show_string(50 + 181, 230, 200, 16, 16, (char*)"o/s");
    lcd_show_string(50 + 181, 250, 200, 16, 16, (char*)"C");

    tftlcd_dev.forecolor = LCD_BLUE;

    while (1) {
        icm20608_getdata();
        integer_display(50 + 70, 130, 16, icm20608_dev.accel_x_adc);
        integer_display(50 + 70, 150, 16, icm20608_dev.accel_y_adc);
        integer_display(50 + 70, 170, 16, icm20608_dev.accel_z_adc);
        integer_display(50 + 70, 190, 16, icm20608_dev.gyro_x_adc);
        integer_display(50 + 70, 210, 16, icm20608_dev.gyro_y_adc);
        integer_display(50 + 70, 230, 16, icm20608_dev.gyro_z_adc);
        integer_display(50 + 70, 250, 16, icm20608_dev.temp_adc);

        decimals_display(50 + 70 + 50, 130, 16, icm20608_dev.accel_x_act);
        decimals_display(50 + 70 + 50, 150, 16, icm20608_dev.accel_y_act);
        decimals_display(50 + 70 + 50, 170, 16, icm20608_dev.accel_z_act);
        decimals_display(50 + 70 + 50, 190, 16, icm20608_dev.gyro_x_act);
        decimals_display(50 + 70 + 50, 210, 16, icm20608_dev.gyro_y_act);
        decimals_display(50 + 70 + 50, 230, 16, icm20608_dev.gyro_z_act);
        decimals_display(50 + 70 + 50, 250, 16, icm20608_dev.temp_act);

#if 0		
		printf("accel x = %d\r\n",icm20608_dev.accel_x_adc);
		printf("accel y = %d\r\n",icm20608_dev.accel_y_adc);
		printf("accel z = %d\r\n",icm20608_dev.accel_z_adc);
		printf("gyrp  x = %d\r\n",icm20608_dev.gyro_x_adc);
		printf("gyro  y = %d\r\n",icm20608_dev.gyro_y_adc);
		printf("gyro  z = %d\r\n",icm20608_dev.gyro_z_adc);
		printf("temp    = %d\r\n",icm20608_dev.temp_adc);
#endif
        delay_ms(120);
        state = !state;
        led_switch(LED0, state);
    }
    return 0;
}
