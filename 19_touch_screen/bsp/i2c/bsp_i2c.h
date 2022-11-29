/**
 * @file    : bsp_i2c.h
 * @author  : KeeneChen
 * @date    : 2022.11.24-17:04:01
 * @details : I2C驱动
 */

#ifndef __BSP_I2C_H__
#define __BSP_I2C_H__

#include "imx6ull.h"

/* 相关宏定义 */
#define I2C_STATUS_OK              (0)
#define I2C_STATUS_BUSY            (1)
#define I2C_STATUS_IDLE            (2)
#define I2C_STATUS_NAK             (3)
#define I2C_STATUS_ARBITRATIONLOST (4)
#define I2C_STATUS_TIMEOUT         (5)
#define I2C_STATUS_ADDRNAK         (6)

/* I2C方向枚举类型 */
enum i2c_direction {
    kI2C_Write = 0x0, // 主机向从机写数据
    kI2C_Read  = 0x1, // 主机从从机读数据
};

/* 主机传输结构体 */
typedef struct i2c_transfer_s {
    unsigned char slave_addr;        // 7位从机地址
    enum i2c_direction direction;    // 传输方向
    unsigned int sub_addr;           // 寄存器地址
    unsigned char sub_addr_size;     // 寄存器地址长度
    unsigned char* volatile data;    // 数据缓冲区
    volatile unsigned int data_size; // 数据缓冲区长度
} i2c_transfer_t;

/**
 * @brief  I2C初始化,低速模式波特率100KHz
 * @param  base 待使用的I2C I2C_1/I2C_2
 * @return void
 */
void i2c_init(I2C_Type* base);

/**
 * @brief  I2C 发送开始信号
 * @param  base      待使用的I2C I2C_1/I2C_2
 * @param  address   设备地址
 * @param  direction I2C方向
 * @return unsigned char start位是否发送成功
 * @retval 0     成功
 * @retval other 出错
 */
unsigned char i2c_master_start(I2C_Type* base, unsigned char address, enum i2c_direction direction);

/**
 * @brief  I2C 发送重新开始信号
 * @param  base      待使用的I2C I2C_1/I2C_2
 * @param  address   设备地址
 * @param  direction I2C方向
 * @return unsigned char repeated_start位是否发送成功
 * @retval 0     成功
 * @retval other 出错
 */
unsigned char i2c_master_repeated_start(I2C_Type* base, unsigned char address,
                                        enum i2c_direction direction);

/**
 * @brief  I2C 发送停止信号
 * @param  base 待使用的I2C I2C_1/I2C_2
 * @return unsigned char stop位是否发送成功
 * @retval 0     成功
 * @retval other 出错
 */
unsigned char i2c_master_stop(I2C_Type* base);

/**
 * @brief  I2C 发送数据
 * @param  base 待使用的I2C I2C_1/I2C_2
 * @param  buf  待发送的数据
 * @param  size 待发送的数据大小
 * @return void
 */
void i2c_master_write(I2C_Type* base, const unsigned char* buf, unsigned int size);

/**
 * @brief  I2C 读取数据
 * @param  base 待使用的I2C I2C_1/I2C_2
 * @param  buf  待发送的数据
 * @param  size 待发送的数据大小
 * @return void
 */
void i2c_master_read(I2C_Type* base, unsigned char* buf, unsigned int size);

/**
 * @brief  I2C 检查并清除错误
 * @param  base   待使用的I2C I2C_1/I2C_2
 * @param  status 状态
 * @return unsigned char 状态结果
 */
unsigned char i2c_check_and_clear_error(I2C_Type* base, unsigned int status);

/**
 * @brief  I2C 数据传输,包括读和写
 * @param  base 待使用的I2C I2C_1/I2C_2
 * @param  xfer 传输结构体
 * @return unsigned char 传输是否发送成功
 * @retval 0     成功
 * @retval other 出错
 */
unsigned char i2c_master_transfer(I2C_Type* base, i2c_transfer_t* xfer);

#endif // __BSP_I2C_H__