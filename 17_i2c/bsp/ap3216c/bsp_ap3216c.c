/**
 * @file    : bsp_ap3216c.c
 * @author  : KeeneChen
 * @date    : 2022.11.25-13:25:18
 * @details : AP3216C驱动
 */

#include "imx6ull.h"
#include "stdio.h"

#include "bsp_ap3216c.h"
#include "bsp_delay.h"
#include "bsp_i2c.h"

unsigned char ap3216c_init(void)
{
    unsigned char data = 0;

    /**
     * 1.IO初始化，配置I2C IO属性
     * I2C1_SCL -> UART4_TXD
     * I2C1_SDA -> UART4_RXD
     */
    IOMUXC_SetPinMux(IOMUXC_UART4_TX_DATA_I2C1_SCL, 1);
    IOMUXC_SetPinMux(IOMUXC_UART4_RX_DATA_I2C1_SDA, 1);

    /**
     * bit [16]   : 0   HYS关闭
     * bit [15:14]: 1   默认47K上拉
     * bit [13]   : 1   pull功能
     * bit [12]   : 1   pull/keeper使能
     * bit [11]   : 0   关闭开路输出
     * bit [7:6]  : 10  速度100Mhz
     * bit [5:3]  : 110 驱动能力为R0/6
     * bit [0]    : 1   高转换率
     */
    IOMUXC_SetPinConfig(IOMUXC_UART4_TX_DATA_I2C1_SCL, 0x70B0);
    IOMUXC_SetPinConfig(IOMUXC_UART4_RX_DATA_I2C1_SDA, 0X70B0);

    i2c_init(I2C1); // 初始化I2C1

    /* 2.初始化AP3216C */
    ap3216c_write_onebyte(AP3216C_ADDR, AP3216C_SYSTEMCONG, 0X04); // 复位AP3216C
    delay_ms(50);                                                  // AP33216C复位至少10ms
    ap3216c_write_onebyte(AP3216C_ADDR, AP3216C_SYSTEMCONG, 0X03); // 开启ALSPS+IR
    data = ap3216c_read_onebyte(AP3216C_ADDR, AP3216C_SYSTEMCONG); // 读取刚刚写进去的0X03

    if (data == 0X03)
        return 0; // AP3216C正常
    else
        return 1; // AP3216C失败
}

unsigned char ap3216c_write_onebyte(unsigned char addr, unsigned char reg, unsigned char data)
{
    unsigned char status    = 0;
    unsigned char writedata = data;
    i2c_transfer_t master_xfer;

    /* 配置I2C xfer结构体 */
    master_xfer.slave_addr    = addr;       // 设备地址
    master_xfer.direction     = kI2C_Write; // 写入数据
    master_xfer.sub_addr      = reg;        // 要写入的寄存器地址
    master_xfer.sub_addr_size = 1;          // 地址长度一个字节
    master_xfer.data          = &writedata; // 要写入的数据
    master_xfer.data_size     = 1;          // 写入数据长度1个字节

    if (i2c_master_transfer(I2C1, &master_xfer))
        status = 1;

    return status;
}

unsigned char ap3216c_read_onebyte(unsigned char addr, unsigned char reg)
{
    unsigned char val = 0;

    i2c_transfer_t master_xfer;
    master_xfer.slave_addr    = addr;      // 设备地址
    master_xfer.direction     = kI2C_Read; // 读取数据
    master_xfer.sub_addr      = reg;       // 要读取的寄存器地址
    master_xfer.sub_addr_size = 1;         // 地址长度一个字节
    master_xfer.data          = &val;      // 接收数据缓冲区
    master_xfer.data_size     = 1;         // 读取数据长度1个字节
    i2c_master_transfer(I2C1, &master_xfer);

    return val;
}

void ap3216c_read_data(unsigned short* ir, unsigned short* ps, unsigned short* als)
{
    unsigned char buf[6];
    unsigned char i;

    /* 循环读取所有传感器数据 */
    for (i = 0; i < 6; i++) {
        buf[i] = ap3216c_read_onebyte(AP3216C_ADDR, AP3216C_IRDATALOW + i);
    }

    if (buf[0] & 0X80) /* IR_OF位为1,则数据无效 */
        *ir = 0;
    else /* 读取IR传感器的数据   		*/
        *ir = ((unsigned short)buf[1] << 2) | (buf[0] & 0X03);

    *als = ((unsigned short)buf[3] << 8) | buf[2]; /* 读取ALS传感器的数据 */

    if (buf[4] & 0x40) /* IR_OF位为1,则数据无效 			*/
        *ps = 0;
    else /* 读取PS传感器的数据    */
        *ps = ((unsigned short)(buf[5] & 0X3F) << 4) | (buf[4] & 0X0F);
}
