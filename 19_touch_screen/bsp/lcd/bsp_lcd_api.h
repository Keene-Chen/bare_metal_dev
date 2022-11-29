/**
 * @file    : bsp_lcd_api.h
 * @author  : KeeneChen
 * @date    : 2022.11.22-19:07:51
 * @details : LCD API函数文件
 */

#ifndef __BSP_LCD_API_H__
#define __BSP_LCD_API_H__

#include "bsp_lcd.h"

/**
 * @brief 画线函数
 * @param x1 线起始点坐标X轴
 * @param y1 线起始点坐标Y轴
 * @param x2 线终止点坐标X轴
 * @param y2 线终止点坐标Y轴
 * @return void
 */
void lcd_draw_line(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);

/**
 * @brief 画矩形函数
 * @param x1 矩形坐上角坐标X轴
 * @param y1 矩形坐上角坐标Y轴
 * @param x2 矩形右下角坐标X轴
 * @param y2 矩形右下角坐标Y轴
 * @return void
 */
void lcd_draw_rectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);

/**
 * @brief 在指定位置画一个指定大小的圆
 * @param x0 圆心坐标X轴
 * @param y0 圆心坐标Y轴
 * @param y2 圆形半径
 * @return void
 */
void lcd_draw_circle(unsigned short x0, unsigned short y0, unsigned char r);

/**
 * @brief	在指定位置显示一个字符
 * @param x	   起始坐标X轴
 * @param y	   起始坐标Y轴
 * @param num  显示字符
 * @param size 字体大小, 可选12/16/24/32
 * @param mode 叠加方式(1)还是非叠加方式(0)
 * @return void
 */
void lcd_showchar(unsigned short x, unsigned short y, unsigned char num, unsigned char size, unsigned char mode);

/**
 * @brief 计算m的n次方
 * @param m 要计算的值
 * @param n n次方
 * @return unsigned int m^n次方.
 */
unsigned int lcd_pow(unsigned char m, unsigned char n);

/**
 * @brief	 显示指定的数字，高位为0的话不显示
 * @param x	   起始坐标点X轴。
 * @param y	   起始坐标点Y轴。
 * @param num  数值(0~999999999)。
 * @param len  数字位数。
 * @param size 字体大小
 * @return void
 */
void lcd_show_num(unsigned short x, unsigned short y, unsigned int num, unsigned char len, unsigned char size);

/**
 * @brief 显示指定的数字，高位为0,还是显示
 * @param x	   起始坐标点X轴。
 * @param y	   起始坐标点Y轴。
 * @param num  数值(0~999999999)。
 * @param len  数字位数。
 * @param size 字体大小
 * @param mode [7]  : 0,不填充;1,填充0.
 *             [6:1]: 保留
 *             [0]  : 0,非叠加显示;1,叠加显示.
 * @return void
 */
void lcd_show_xnum(unsigned short x, unsigned short y, unsigned int num, unsigned char len, unsigned char size, unsigned char mode);

/**
 * @brief 显示一串字符串
 * @param x	     起始坐标点X轴。
 * @param y	     起始坐标点Y轴。
 * @param width  字符串显示区域长度
 * @param height 字符串显示区域高度
 * @param size   字体大小
 * @param p	     要显示的字符串首地址
 * @return void
 */
void lcd_show_string(unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned char size, char* p);

#endif // __BSP_LCD_API_H__
