/**
 * @file    : bsp_beep.c
 * @author  : KeeneChen
 * @date    : 2022.11.05-10:47:20
 * @details : 蜂鸣器驱动模块
 */

#include "bsp_beep.h"
#include "fsl_iomuxc.h"

void beep_init(void)
{
    // 1.初始化IO复用
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0);

    // 2.配置GPIO5_IO01 IO属性
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0x10B0);

    // 3.配置GPIO5_IO01为输出
    GPIO5->GDIR |= (1 << 1);

    // 4.输出高电平
    GPIO5->DR |= (1 << 1);
}

void beep_switch(int status)
{
    if (status == ON)
        GPIO5->DR &= ~(1 << 1); // 打开蜂鸣器
    else if (status == OFF)
        GPIO5->DR |= (1 << 1); // 关闭蜂鸣器
}