/**
 * @file    : ft5426.h
 * @author  : KeeneChen
 * @date    : 2022.11.28-15:53:11
 * @details : ft5426触摸屏驱动
 */

#ifndef __BSP_FT5426_H__
#define __BSP_FT5426_H__

/* 宏定义 */
#define FT5426_ADDR                 0X38 // FT5426设备地址
#define FT5426_DEVICE_MODE          0X00 // 模式寄存器
#define FT5426_IDGLIB_VERSION       0XA1 // 固件版本寄存器
#define FT5426_IDG_MODE             0XA4 // 中断模式
#define FT5426_TD_STATUS            0X02 // 触摸状态寄存器
#define FT5426_TOUCH1_XH            0X03 // 触摸点坐标寄存器,一个触摸点用4个寄存器存储坐标数据
#define FT5426_XYCOORDREG_NUM       30   // 触摸点坐标寄存器数量
#define FT5426_INIT_FINISHED        1    // 触摸屏初始化完成
#define FT5426_INIT_NOTFINISHED     0    // 触摸屏初始化未完成
#define FT5426_TOUCH_EVENT_DOWN     0x00 // 按下
#define FT5426_TOUCH_EVENT_UP       0x01 // 释放
#define FT5426_TOUCH_EVENT_ON       0x02 // 接触
#define FT5426_TOUCH_EVENT_RESERVED 0x03 // 没有事件

/* 触摸屏结构体 */
typedef struct ft5426_dev_s {
    unsigned char init_falg; // 触摸屏初始化状态
    unsigned char int_flag;  // 标记中断有没有发生
    unsigned char point_num; // 触摸点
    unsigned short x[5];     // X轴坐标
    unsigned short y[5];     // Y轴坐标
} ft5426_dev_t;

extern ft5426_dev_t ft5426_dev;

/**
 * @brief  ft5426初始化
 * @return void
 */
void ft5426_init(void);

/**
 * @brief  ft5426 CT_INT引脚中断服务函数
 * @return void
 */
void gpio1_io09_irqhandler(void);

/**
 * @brief  从ft5426写入一个字节的数据
 * @param  addr 设备地址
 * @param  reg  要写入的寄存器
 * @param  data 要写入寄存器的值
 * @return unsigned char 操作结果
 */
unsigned char ft5426_write_byte(unsigned char addr, unsigned char reg, unsigned char data);

/**
 * @brief  从ft5426读取一个字节的数据
 * @param  addr 设备地址
 * @param  reg  要读取的寄存器
 * @return unsigned char 读取到的数据
 */
unsigned char ft5426_read_byte(unsigned char addr, unsigned char reg);

/**
 * @brief  从ft5426读取多个寄存器的数据
 * @param  addr 设备地址
 * @param  reg  要读取的寄存器
 * @param  len  要读取的长度
 * @param  buf  读取到的值
 * @return void
 */
void ft5426_read_len(unsigned char addr, unsigned char reg, unsigned char len, unsigned char* buf);

/**
 * @brief  读取当前触摸点个数
 * @return void
 */
void ft5426_read_tpnum(void);

/**
 * @brief  读取当前触摸点坐标
 * @return void
 */
void ft5426_read_tpcoord(void);

#endif // __BSP_FT5426_H__