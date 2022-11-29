/**
 * @file    : bsp_spi.c
 * @author  : KeeneChen
 * @date    : 2022.11.27-12:07:22
 * @details : SPI驱动
 */

#include "imx6ull.h"

#include "bsp_spi.h"

void spi_init(ECSPI_Type* base)
{
    /**
     * 1.配置CONREG寄存器
     * bit [0]    : 1    使能ECSPI
     * bit [3]    : 1    当向TXFIFO写入数据以后立即开启SPI突发
     * bit [7:4]  : 0001
     * SPI通道0主模式,根据实际情况选择,开发板上的ICM-20608接在SS0上,所以设置通道0为主模式 bit
     * [19:18]:	00 	 选中通道0(其实不需要,因为片选信号我们自己控制) bit [31:20]:	0x7
     * 突发长度为8个bit
     */
    base->CONREG = 0;                                           // 先清除控制寄存器
    base->CONREG |= (1 << 0) | (1 << 3) | (1 << 4) | (7 << 20); // 配置CONREG寄存器

    /**
     * 2.ECSPI通道0设置,即设置CONFIGREG寄存器
     * bit0:	0 通道0 PHA为0
     * bit4:	0 通道0 SCLK高电平有效
     * bit8: 	0 通道0片选信号 当SMC为1的时候此位无效
     * bit12：	0 通道0 POL为0
     * bit16：	0 通道0 数据线空闲时高电平
     * bit20:	0 通道0 时钟线空闲时低电平
     */
    base->CONFIGREG = 0; // 设置通道寄存器

    /**
     * 3.ECSPI通道0设置,设置采样周期
     * bit[14:0] :	0x2000  采样等待周期,比如当SPI时钟为10MHz的时候
     *  		    0x2000就等于1/10000 * 0x2000 = 0.8192ms,也就是连续
     *          	读取数据的时候每次之间间隔0.8ms
     * bit15	 :  0  采样时钟源为SPI CLK
     * bit[21:16]:  0  片选延时,可设置为0~63
     */
    base->PERIODREG = 0x2000; // 设置采样周期寄存器

    /**
     * 4.ECSPI的SPI时钟配置,SPI的时钟源来源于pll3_sw_clk/8=480/8=60MHz
     * 通过设置CONREG寄存器的PER_DIVIDER(bit[11:8])和POST_DIVEDER(bit[15:12])来
     * 对SPI时钟源分频,获取到我们想要的SPI时钟：
     * SPI CLK = (SourceCLK / PER_DIVIDER) / (2^POST_DIVEDER)
     * 比如我们现在要设置SPI时钟为6MHz,那么PER_DIVEIDER和POST_DEIVIDER设置如下：
     * PER_DIVIDER = 0x9
     * POST_DIVIDER = 0x0
     * SPI CLK = 60000000/(0x9 + 1) = 60000000=6MHz
     */
    base->CONREG &= ~((0xF << 12) | (0xF << 8)); // 清除PER_DIVDER和POST_DIVEDER以前的设置
    base->CONREG |= (0x9 << 12);                 // 设置SPI CLK = 6MHz
}

unsigned char spich0_read_write_byte(ECSPI_Type* base, unsigned char tx_data)
{
    uint32_t spi_rx_data = 0;
    uint32_t spi_tx_data = tx_data;

    /* 选择通道0 */
    base->CONREG &= ~(3 << 18);
    base->CONREG |= (0 << 18);

    while ((base->STATREG & (1 << 0)) == 0)
        ; /* 等待发送FIFO为空 */
    base->TXDATA = spi_tx_data;

    while ((base->STATREG & (1 << 3)) == 0)
        ; /* 等待接收FIFO有数据 */
    spi_rx_data = base->RXDATA;
    
    return spi_rx_data;
}