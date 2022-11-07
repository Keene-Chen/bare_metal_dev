/**
 * @file    : start.s
 * @author  : KeeneChen
 * @date    : 2022.11.03-17:03:23
 * @details : 配置C语言环境
 */

.global _start

.global __bss_start
_bss_start:
	.word __bss_start

.global __bss_end
_bss_end:
	.word __bss_end

_start:
    /* 进入SVC模式 */
	mrs r0,   cpsr
	bic r0,   r0,  #0x1F   @将r0寄存器中的低5位清零 cpsr的M0~M4
	orr r0,   r0,  #0x13   @r0或上0x13,表示使用SVC模式
	msr cpsr, r0		   @将r0 的数据写入到cpsr_c中

	/* 清BSS段 */
	ldr r0, _bss_start
	ldr r1, _bss_end
	mov r2, #0
bss_loop:
	stmia r0!, {r2}     @向r0的地址写入0,然后r0寄存器保存的地址值加1
	cmp r0, r1          @比较r0和r1，也就是__bss_start和__bss_end的值
	ble bss_loop        @如果小于等于的话就跳转到bss_loop继续清bss段

	ldr sp, =0x80200000	   @设置栈指针
	b main                 @跳转到main函数
