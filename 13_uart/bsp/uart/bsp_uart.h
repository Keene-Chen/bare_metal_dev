/**
 * @file    : bsp_uart.c
 * @author  : KeeneChen
 * @date    : 2022.11.18-20:14:35
 * @details : UART串口通信实验
 */
#ifndef __BSP_UART_H__
#define __BSP_UART_H__
#include "imx6ull.h"

/**
 * @brief  串口初始化
 * @return void
 */
void uart_init(void);

/**
 * @brief  串口 IO 初始化
 * @return void
 */
void uart_io_init(void);

/**
 * @brief  串口软复位
 * @param  base UART1,UART2...
 * @return void
 */
void uart_soft_reset(UART_Type* base);

/**
 * @brief  波特率计算公式,可以用此函数计算出指定串口对应的UFCR,UBIR和UBMR这三个寄存器的值
 * @param  base        要计算的串口
 * @param  baudrate    要使用的波特率
 * @param  srcclock_hz 串口时钟源频率,单位Hz
 * @return void
 */
void uart_set_baudrate(UART_Type* base, unsigned int baudrate, unsigned int srcclock_hz);

/**
 * @brief  串口使能
 * @param  base UART1,UART2...
 * @return void
 */
void uart_enable(UART_Type* base);

/**
 * @brief  串口失能
 * @param  base UART1,UART2...
 * @return void
 */
void uart_disable(UART_Type* base);

/**
 * @brief  串口输出一个字符
 * @param  c 待输出字符
 * @return void
 */
void putc(unsigned char c);

/**
 * @brief  串口发送一个字符串
 * @param  ste 待发送字符串
 * @return void
 */
void puts(char* str);

/**
 * @brief  串口接收一个字符
 * @return unsigned char 接收的一个字符
 */
unsigned char getc(void);

/**
 * @brief  防止编译器报错
 * @return void
 */
void raise(int sig_nr);

#endif // __BSP_UART_H__