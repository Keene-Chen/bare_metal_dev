/**
 * @file    : bsp_ap3216c.h
 * @author  : KeeneChen
 * @date    : 2022.11.25-13:25:33
 * @details : AP3216C驱动
 */

#ifndef __BSP_AP3216C_H__
#define __BSP_AP3216C_H__

#define AP3216C_ADDR 0X1E // AP3216C器件地址

/* AP3316C寄存器 */
#define AP3216C_SYSTEMCONG  0x00 // 配置寄存器
#define AP3216C_INTSTATUS   0X01 // 中断状态寄存器
#define AP3216C_INTCLEAR    0X02 // 中断清除寄存器
#define AP3216C_IRDATALOW   0x0A // IR数据低字节
#define AP3216C_IRDATAHIGH  0x0B // IR数据高字节
#define AP3216C_ALSDATALOW  0x0C // ALS数据低字节
#define AP3216C_ALSDATAHIGH 0X0D // ALS数据高字节
#define AP3216C_PSDATALOW   0X0E // PS数据低字节
#define AP3216C_PSDATAHIGH  0X0F // PS数据高字节

/**
 * @brief  初始化AP3216C
 * @return unsigned char stop位是否发送成功
 * @retval 0     成功
 * @retval other 出错
 */
unsigned char ap3216c_init(void);

/**
 * @brief  向AP3216C写入数据
 * @param  addr 设备地址
 * @param  reg  要写入的寄存器
 * @param  data 要写入的数据
 * @return unsigned char 操作结果
 */
unsigned char ap3216c_read_onebyte(unsigned char addr, unsigned char reg);

/**
 * @brief  从AP3216C读取一个字节的数据
 * @param  addr 设备地址
 * @param  reg  要读取的寄存器
 * @return unsigned char 读取到的数据
 */
unsigned char ap3216c_write_onebyte(unsigned char addr, unsigned char reg, unsigned char data);

/**
 * @brief 读取AP3216C的数据，读取原始数据，包括ALS,PS和IR, 注意！
 *		  如果同时打开ALS,IR+PS的话两次数据读取的时间间隔要大于112.5ms
 * @param  ir ir数据
 * @param  ps ps数据
 * @param  ps als数据
 * @return void
 */
void ap3216c_read_data(unsigned short* ir, unsigned short* ps, unsigned short* als);

#endif // __BSP_AP3216C_H__
