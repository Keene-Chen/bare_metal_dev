/**
 * @file    : bsp_spi.h
 * @author  : KeeneChen
 * @date    : 2022.11.27-12:07:10
 * @details : SPI驱动
 */

#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

#include "imx6ull.h"

/**
 * @brief  SPI初始化
 * @param  base 待初始化SPI
 * @return void
 */
void spi_init(ECSPI_Type* base);

/**
 * @brief  SPI通道0读写一字节数据
 * @param  base    待接收/发送的SPI
 * @param  tx_data 发送数据
 * @return unsigned char 读取到的数据
 */
unsigned char spich0_read_write_byte(ECSPI_Type* base, unsigned char tx_data);

#endif // __BSP_SPI_H__