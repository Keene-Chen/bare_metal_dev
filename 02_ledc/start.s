/**
 * @file    : start.s
 * @author  : KeeneChen
 * @date    : 2022.11.03-11:05:42
 * @details : 配置C语言环境
 */

.global _start

_start:
    /* 进入SVC模式 */
	mrs r0,   cpsr
	bic r0,   r0,  #0x1F   @将r0寄存器中的低5位清零 cpsr的M0~M4
	orr r0,   r0,  #0x13   @r0或上0x13,表示使用SVC模式
	msr cpsr, r0		   @将r0 的数据写入到cpsr_c中

	ldr sp, =0x80200000	   @设置栈指针
	b main				   @跳转到main函数
