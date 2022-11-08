/**
 * @file    : bsp_delay.c
 * @author  : KeeneChen
 * @date    : 2022.11.05-10:16:38
 * @details : 延时模块
 */

#include "bsp_delay.h"

void delay_short(volatile unsigned int n)
{
    while (n--)
        ;
}

void delay(volatile unsigned int n)
{
    while (n--) {
        delay_short(0x7FF);
    }
}