/**
 * @file    : bsp_int.h
 * @author  : KeeneChen
 * @date    : 2022.11.09-14:19:38
 * @details : 中断驱动头文件
 */

#ifndef __BSP_INT_H__
#define __BSP_INT_H__
#include "imx6ull.h"

/* 中断服务函数形式 */
typedef void (*system_irq_handler_t)(unsigned int gicc_iar, void* param);

/* 中断服务函数结构体 */
typedef struct sys_irq_handler_s {
    system_irq_handler_t irq_handler; // 中断服务函数
    void* user_param;                 // 中断服务函数参数
} sys_irq_handler_t;

/**
 * @brief  中断初始化函数
 * @return void
 */
void int_init(void);

/**
 * @brief  初始化中断服务函数表
 * @return void
 */
void system_irqtable_init(void);

/**
 * @brief 给指定的中断号注册中断服务函数
 * @param  irq        要注册的中断号
 * @param  handler    要注册的中断处理函数
 * @param  user_param 中断服务处理函数参数
 * @return void
 */
void system_register_irqhandler(IRQn_Type irq, system_irq_handler_t handler, void* user_param);

/**
 * @brief  C语言中断服务函数，irq汇编中断服务函数会
 *         调用此函数，此函数通过在中断服务列表中查
 *         找指定中断号所对应的中断处理函数并执行。
 * @param  gicc_iar 中断号
 * @return void
 */
void system_irqhandler(unsigned int gicc_iar);

/**
 * @brief  默认中断服务函数
 * @param  gicc_iar   中断号
 * @param  user_param 中断服务处理函数参数
 * @return void
 */
void default_irqhandler(unsigned int gicc_iar, void* user_param);

#endif // __BSP_INT_H__
