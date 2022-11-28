/**
 * @file    : bsp_lcdapi.c
 * @author  : KeeneChen
 * @date    : 2022.11.22-19:07:24
 * @details : LCD API函数
 */

#include "imx6ull.h"

#include "bsp_lcd_api.h"
#include "font.h"

void lcd_draw_line(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; // 计算坐标增量
    delta_y = y2 - y1;
    uRow    = x1;
    uCol    = y1;
    if (delta_x > 0) // 设置单步方向
        incx = 1;
    else if (delta_x == 0) // 垂直线
        incx = 0;
    else {
        incx    = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0) // 水平线
        incy = 0;
    else {
        incy    = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y) // 选取基本增量坐标轴
        distance = delta_x;
    else
        distance = delta_y;
    for (t = 0; t <= distance + 1; t++) // 画线输出
    {

        lcd_draw_point(uRow, uCol, tftlcd_dev.forecolor); // 画点
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance) {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance) {
            yerr -= distance;
            uCol += incy;
        }
    }
}

void lcd_draw_rectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{
    lcd_draw_line(x1, y1, x2, y1);
    lcd_draw_line(x1, y1, x1, y2);
    lcd_draw_line(x1, y2, x2, y2);
    lcd_draw_line(x2, y1, x2, y2);
}

void lcd_draw_circle(unsigned short x0, unsigned short y0, unsigned char r)
{
    int mx = x0, my = y0;
    int x = 0, y = r;

    int d = 1 - r;
    while (y > x) // y>x即第一象限的第1区八分圆
    {
        lcd_draw_point(x + mx, y + my, tftlcd_dev.forecolor);
        lcd_draw_point(y + mx, x + my, tftlcd_dev.forecolor);
        lcd_draw_point(-x + mx, y + my, tftlcd_dev.forecolor);
        lcd_draw_point(-y + mx, x + my, tftlcd_dev.forecolor);

        lcd_draw_point(-x + mx, -y + my, tftlcd_dev.forecolor);
        lcd_draw_point(-y + mx, -x + my, tftlcd_dev.forecolor);
        lcd_draw_point(x + mx, -y + my, tftlcd_dev.forecolor);
        lcd_draw_point(y + mx, -x + my, tftlcd_dev.forecolor);
        if (d < 0) {
            d = d + 2 * x + 3;
        }
        else {
            d = d + 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

void lcd_show_char(unsigned short x, unsigned short y, unsigned char num, unsigned char size, unsigned char mode)
{
    unsigned char temp, t1, t;
    unsigned short y0   = y;
    unsigned char csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); // 得到字体一个字符对应点阵集所占的字节数
    num                 = num - ' ';                                      // 得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
    for (t = 0; t < csize; t++) {
        if (size == 12)
            temp = asc2_1206[num][t]; // 调用1206字体
        else if (size == 16)
            temp = asc2_1608[num][t]; // 调用1608字体
        else if (size == 24)
            temp = asc2_2412[num][t]; // 调用2412字体
        else if (size == 32)
            temp = asc2_3216[num][t]; // 调用3216字体
        else
            return; // 没有的字库
        for (t1 = 0; t1 < 8; t1++) {
            if (temp & 0x80)
                lcd_draw_point(x, y, tftlcd_dev.forecolor);
            else if (mode == 0)
                lcd_draw_point(x, y, tftlcd_dev.backcolor);
            temp <<= 1;
            y++;
            if (y >= tftlcd_dev.height)
                return; // 超区域了
            if ((y - y0) == size) {
                y = y0;
                x++;
                if (x >= tftlcd_dev.width)
                    return; // 超区域了
                break;
            }
        }
    }
}

unsigned int lcd_pow(unsigned char m, unsigned char n)
{
    unsigned int result = 1;
    while (n--)
        result *= m;
    return result;
}

void lcd_show_num(unsigned short x, unsigned short y, unsigned int num, unsigned char len, unsigned char size)
{
    unsigned char t, temp;
    unsigned char enshow = 0;
    for (t = 0; t < len; t++) {
        temp = (num / lcd_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                lcd_show_char(x + (size / 2) * t, y, ' ', size, 0);
                continue;
            }
            else
                enshow = 1;
        }
        lcd_show_char(x + (size / 2) * t, y, temp + '0', size, 0);
    }
}

void lcd_show_xnum(unsigned short x, unsigned short y, unsigned int num, unsigned char len, unsigned char size, unsigned char mode)
{
    unsigned char t, temp;
    unsigned char enshow = 0;
    for (t = 0; t < len; t++) {
        temp = (num / lcd_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                if (mode & 0X80)
                    lcd_show_char(x + (size / 2) * t, y, '0', size, mode & 0X01);
                else
                    lcd_show_char(x + (size / 2) * t, y, ' ', size, mode & 0X01);
                continue;
            }
            else
                enshow = 1;
        }
        lcd_show_char(x + (size / 2) * t, y, temp + '0', size, mode & 0X01);
    }
}

void lcd_show_string(unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned char size, char* p)
{
    unsigned char x0 = x;
    width += x;
    height += y;
    while ((*p <= '~') && (*p >= ' ')) // 判断是不是非法字符!
    {
        if (x >= width) {
            x = x0;
            y += size;
        }
        if (y >= height)
            break; // 退出
        lcd_show_char(x, y, *p, size, 0);
        x += size / 2;
        p++;
    }
}
