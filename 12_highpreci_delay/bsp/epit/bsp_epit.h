/**
 * @file    : bsp_epit.h
 * @author  : KeeneChen
 * @date    : 2022.11.15-10:29:20
 * @details : bsp_epit
 */

#ifndef __BSP_EPIT_H__
#define __BSP_EPIT_H__

/**
 * @brief  EPIT定时器初始化
 * @param  frac  分频值,范围为0~4095,分别对应1~4096分频
 * @param  value 倒计时值
 * @return void
 */
void epit1_init(int frac, int value);

/**
 * @brief  EPIT定时器中断服务函数
 * @param  gicc_iar   未使用
 * @param  user_param 未使用
 * @return void
 */
void epit1_irqhandler(unsigned int gicc_iar, void* user_param);

#endif // __BSP_EPIT_H__