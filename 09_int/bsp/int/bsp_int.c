/**
 * @file    : bsp_int.c
 * @author  : KeeneChen
 * @date    : 2022.11.09-14:20:07
 * @details : 中断驱动文件
 */

#include "bsp_int.h"

/* 中断嵌套计数器 */
static unsigned int irq_nesting;

/* 中断服务函数表 */
static sys_irq_handler_t irq_table[NUMBER_OF_INT_VECTORS];

void int_init(void)
{
    GIC_Init();                       // 初始化GIC
    system_irqtable_init();           // 初始化中断表
    __set_VBAR((uint32_t)0x87800000); // 中断向量表偏移，偏移到起始地址
}

void system_irqtable_init(void)
{
    unsigned int i = 0;
    irq_nesting    = 0;

    /* 先将所有的中断服务函数设置为默认值 */
    for (i = 0; i < NUMBER_OF_INT_VECTORS; i++) {
        system_register_irqhandler((IRQn_Type)i, default_irqhandler, NULL);
    }
}

void system_register_irqhandler(IRQn_Type irq, system_irq_handler_t handler, void* user_param)
{
    irq_table[irq].irq_handler = handler;
    irq_table[irq].user_param  = user_param;
}

void system_irqhandler(unsigned int gicc_iar)
{
    uint32_t int_num = gicc_iar & 0x3FFUL; // 获取中断号

    /* 检查中断号是否符合要求 */
    if ((int_num == 1023) || (int_num >= NUMBER_OF_INT_VECTORS)) {
        return;
    }

    irq_nesting++; // 中断嵌套计数器加一

    /* 根据传递进来的中断号，在irqTable中调用确定的中断服务函数*/
    irq_table[int_num].irq_handler(int_num, irq_table[int_num].user_param);

    irq_nesting--; // 中断执行完成，中断嵌套寄存器减一
}

void default_irqhandler(unsigned int gicc_iar, void* user_param)
{
    while (1)
        ;
}
