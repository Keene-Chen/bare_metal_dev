/**
 * @file    : ft5426.c
 * @author  : KeeneChen
 * @date    : 2022.11.28-15:53:29
 * @details : ft5426触摸屏驱动
 */

#include "imx6ull.h"
#include "stdio.h"

#include "bsp_delay.h"
#include "bsp_exti.h"
#include "bsp_ft5426.h"
#include "bsp_gpio.h"
#include "bsp_i2c.h"
#include "bsp_int.h"

ft5426_dev_t ft5426_dev;

void ft5426_init(void)
{
    /* 1.IO初始化 */
    IOMUXC_SetPinMux(IOMUXC_UART5_TX_DATA_I2C2_SCL, 1);
    IOMUXC_SetPinMux(IOMUXC_UART5_RX_DATA_I2C2_SDA, 1);
    IOMUXC_SetPinConfig(IOMUXC_UART5_TX_DATA_I2C2_SCL, 0x70B0);
    IOMUXC_SetPinConfig(IOMUXC_UART5_RX_DATA_I2C2_SDA, 0X70B0);

    /* 初始化CT_INT引脚使能中断 */
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO09_GPIO1_IO09, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO09_GPIO1_IO09, 0XF080);
    gpio_pin_conf_t ctint_config;
    ctint_config.direction      = GPIO_DIGITAL_INPUT;
    ctint_config.interrupt_mode = kGPIO_IntRisingOrFallingEdge; // 跳变沿触发
    ctint_config.output_logic   = 1;
    gpio_init(GPIO1, 9, &ctint_config);

    GIC_EnableIRQ(GPIO1_Combined_0_15_IRQn); // 使能GIC中对应的中断
    system_register_irqhandler(GPIO1_Combined_0_15_IRQn,
                               (system_irq_handler_t)gpio1_io09_irqhandler,
                               NULL); // 注册中断服务函数
    gpio_interrupt_enable(GPIO1, 9);  // 使能GPIO1_IO18的中断功能

    /* 初始化CT_RST引脚 */
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER9_GPIO5_IO09, 0);
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER9_GPIO5_IO09, 0X10B0);
    gpio_pin_conf_t ctrst_config;
    ctrst_config.direction    = GPIO_DIGITAL_OUTPUT;
    ctrst_config.output_logic = 1;
    gpio_init(GPIO5, 9, &ctrst_config);

    gpio_pin_write(GPIO5, 9, 0); // 复位ft5426
    delay_ms(50);
    gpio_pin_write(GPIO5, 9, 1); // 停止复位
    delay_ms(50);

    /* 2.I2C2接口初始化 */
    i2c_init(I2C2);

    /* 3.ft5426初始化 */
    unsigned char reg_value[2];

    ft5426_read_len(FT5426_ADDR, FT5426_IDGLIB_VERSION, 2, reg_value);
    printf("Touch Frimware Version:%#X\r\n", ((unsigned short)reg_value[0] << 8) | reg_value[1]);

    ft5426_write_byte(FT5426_ADDR, FT5426_DEVICE_MODE, 0); // 进入正常模式
    ft5426_write_byte(FT5426_ADDR, FT5426_IDG_MODE, 1);    // FT5426中断模式

    ft5426_dev.init_falg = FT5426_INIT_FINISHED; // 标记FT5426初始化完成
    ft5426_dev.int_flag  = 0;
}

void gpio1_io09_irqhandler(void)
{
    // 判断是否初始化完成
    if (ft5426_dev.init_falg == FT5426_INIT_FINISHED) {
        ft5426_read_tpcoord();
    }

    gpio_interrupt_clear_flags(GPIO1, 18); // 清除中断标志位
}

unsigned char ft5426_write_byte(unsigned char addr, unsigned char reg, unsigned char data)
{
    unsigned char status    = 0;
    unsigned char writedata = data;

    /* 配置I2C xfer结构体 */
    i2c_transfer_t master_xfer = {
        .slave_addr    = addr,       // 设备地址
        .direction     = kI2C_Write, // 写入数据
        .sub_addr      = reg,        // 要写入的寄存器地址
        .sub_addr_size = 1,          // 地址长度一个字节
        .data          = &writedata, // 要写入的数据
        .data_size     = 1           // 写入数据长度1个字节
    };

    if (i2c_master_transfer(I2C2, &master_xfer))
        status = 1;

    return status;
}

unsigned char ft5426_read_byte(unsigned char addr, unsigned char reg)
{
    unsigned char val = 0;

    i2c_transfer_t master_xfer;
    master_xfer.slave_addr    = addr;      // 设备地址
    master_xfer.direction     = kI2C_Read; // 读取数据
    master_xfer.sub_addr      = reg;       // 要读取的寄存器地址
    master_xfer.sub_addr_size = 1;         // 地址长度一个字节
    master_xfer.data          = &val;      // 接收数据缓冲区
    master_xfer.data_size     = 1;         // 读取数据长度1个字节
    i2c_master_transfer(I2C2, &master_xfer);

    return val;
}

void ft5426_read_len(unsigned char addr, unsigned char reg, unsigned char len, unsigned char* buf)
{
    i2c_transfer_t master_xfer = { addr, kI2C_Read, reg, 1, buf, len };
    i2c_master_transfer(I2C2, &master_xfer);
}

void ft5426_read_tpnum(void)
{
    ft5426_dev.point_num = ft5426_read_byte(FT5426_ADDR, FT5426_TD_STATUS);
}

void ft5426_read_tpcoord(void)
{
    unsigned char point_buf[FT5426_XYCOORDREG_NUM];

    ft5426_read_tpnum();
    ft5426_read_len(FT5426_ADDR, FT5426_TOUCH1_XH, FT5426_XYCOORDREG_NUM, point_buf);

    unsigned char type = 0;
    for (int i = 0; i < ft5426_dev.point_num; i++) {
        unsigned char* buf = &point_buf[i * 6];
        /**
         * 以第一个触摸点为例，寄存器TOUCH1_XH(地址0X03),各位描述如下：
         * bit7:6  Event flag  0:按下 1:释放 2：接触 3：没有事件
         * bit5:4  保留
         * bit3:0  X轴触摸点的11~8位。
         */
        ft5426_dev.x[i] = ((buf[2] << 8) | buf[3]) & 0x0fff;
        ft5426_dev.y[i] = ((buf[0] << 8) | buf[1]) & 0x0fff;

        type = buf[0] >> 6; // 获取触摸类型

        /**
         * 以第一个触摸点为例，寄存器TOUCH1_YH(地址0X05),各位描述如下：
         * bit7:4  Touch ID  触摸ID，表示是哪个触摸点
         * bit3:0  Y轴触摸点的11~8位。
         */
        // id = (buf[2] >> 4) & 0x0f;

        if (type == FT5426_TOUCH_EVENT_DOWN || type == FT5426_TOUCH_EVENT_ON) // 按下
        {
        }
        else { // 释放
        }
    }
}