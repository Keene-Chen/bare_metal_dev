/**
 * @file    : bsp_clk.h
 * @author  : KeeneChen
 * @date    : 2022.11.07-20:25:47
 * @details : 时钟驱动模块
 */

#ifndef __BSP_CLK_H__
#define __BSP_CLK_H__

/**
 * @brief  使能I.MX6ULL所有外设时钟
 * @return void
 */
void clk_enable(void);

/**
 * @brief  初始化系统时钟,设置系统时钟为792Mhz,并且设置PLL2和PLL3各个PFD时钟,
 *         所有时钟频率均按照I.MX6ULL官方手册推荐的值.
 * @return void
 */
void clk_clk(void);

#endif // __BSP_CLK_H__