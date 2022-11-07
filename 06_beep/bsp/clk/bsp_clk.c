/**
 * @file    : bsp_clk.c
 * @author  : KeeneChen
 * @date    : 2022.11.05-10:15:20
 * @details : 时钟驱动模块
 */

#include "bsp_clk.h"
#include "fsl_iomuxc.h"

void clk_enable(void)
{
    CCM->CCGR0 = 0xFFFFFFFF;
    CCM->CCGR1 = 0xFFFFFFFF;
    CCM->CCGR2 = 0xFFFFFFFF;
    CCM->CCGR3 = 0xFFFFFFFF;
    CCM->CCGR4 = 0xFFFFFFFF;
    CCM->CCGR5 = 0xFFFFFFFF;
    CCM->CCGR6 = 0xFFFFFFFF;
}