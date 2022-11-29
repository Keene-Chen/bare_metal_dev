/**
 * @file    : start.s
 * @author  : KeeneChen
 * @date    : 2022.11.08-16:23:58
 * @details : 添加中断向量表
 */

/* 全局标号 */
.global _start

/* 设置中断向量表 */
_start:
	ldr pc, =Rest_Handler      @复位-中断服务函数
	ldr pc, =Undefined_Handler @未定义指令-中断服务函数
	ldr pc, =SVC_Handler       @SVC-中断服务函数
	ldr pc, =PreAbort_Handler  @指令预取中止-中断服务函数
	ldr pc, =DataAbort_Handler @数据访问中止-中断服务函数
	ldr pc, =NotUsed_Handler   @未使用-中断服务函数
	ldr pc, =IRQ_Handler       @外部-中断服务函数
	ldr pc, =FIQ_Handler       @快速-中断服务函数

/* 中断函数定义 */
Rest_Handler:
	/**
	 * 关闭instruction cache、data cache、MMU
	 * 常用读改写的方式,修改SCTLR寄存器
	 */
	cpsid i                    @禁止IRQ中断,防止其他中断干扰

	mrc p15, 0, r0, c1, c0, 0  @读取SCTLR寄存器数据到ARM r0寄存器
	bic r0, r0, #(1 << 12)     @关闭instruction cache
	bic r0, r0, #(1 << 11)     @关闭分支预测
	bic r0, r0, #(1 << 2)      @关闭data cache
	bic r0, r0, #(1 << 1)      @关闭对齐
	bic r0, r0, #(1 << 0)      @关闭MMU
	mcr p15, 0, r0, c1, c0, 0  @写入ARM r0寄存器数据到SCTLR寄存器

#if 0
	/* 汇编版本设置中断向量表偏移 */
	ldr r0, =0x87800000
	dsb 					   @数据同步屏障指令,等待前面的指令完成后再执行后面的指令。
							   @dmb：数据内存屏障指令,等待前面访问内存的指令完成后再执行后面访问内存的指令。
	isb 					   @指令同步屏障,等待流水线中所有指令执行完成后再执行后面的指令。
	mcr p15, 0, r0, c12, c0, 0 @设置VBAR寄存器=0x87800000
	dsb
	isb
#endif

	/* 设置处理器进入IRQ模式 */
	mrs r0,   cpsr
	bic r0,   r0,  #0x1f @将r0寄存器中的低5位清零 cpsr的M0~M4
	orr r0,   r0,  #0x12 @r0或上0x13,表示使用IRQ模式
	msr cpsr, r0		 @将r0 的数据写入到cpsr_c中
	ldr sp, =0x80600000	 @设置IRQ模式下的栈指针,2M大小

	/* 设置处理器进入SYS模式 */
	mrs r0,   cpsr
	bic r0,   r0,  #0x1f @将r0寄存器中的低5位清零 cpsr的M0~M4
	orr r0,   r0,  #0x1f @r0或上0x13,表示使用SYS模式
	msr cpsr, r0		 @将r0 的数据写入到cpsr_c中
	ldr sp, =0x80400000	 @设置SYS模式下的栈指针,2M大小

	/* 设置处理器进入SVC模式 */
	mrs r0,   cpsr
	bic r0,   r0,  #0x1f @将r0寄存器中的低5位清零 cpsr的M0~M4
	orr r0,   r0,  #0x13 @r0或上0x13,表示使用SVC模式
	msr cpsr, r0		 @将r0 的数据写入到cpsr_c中
	ldr sp, =0x80200000	 @设置IRQ模式下的栈指针,2M大小

	/* 设置BSS段起始/终止位置 */
.global __bss_start
_bss_start:
	.word __bss_start

.global __bss_end
_bss_end:
	.word __bss_end
	
	/* 清BSS段 */
	ldr r0, _bss_start
	ldr r1, _bss_end
	mov r2, #0
bss_loop:
	stmia r0!, {r2}      @向r0的地址写入0,然后r0寄存器保存的地址值加1
	cmp   r0,  r1        @比较r0和r1，也就是__bss_start和__bss_end的值
	ble   bss_loop       @如果小于等于的话就跳转到bss_loop继续清bss段

	cpsie i              @使能IRQ中断

    /* 跳转到main函数 */
	b main

Undefined_Handler:
	ldr r0, =Undefined_Handler
	bx  r0
SVC_Handler:
	ldr r0, =SVC_Handler
	bx  r0
PreAbort_Handler:
	ldr r0, =PreAbort_Handler
	bx  r0
DataAbort_Handler:
	ldr r0, =DataAbort_Handler
	bx  r0
NotUsed_Handler:
	ldr r0, =NotUsed_Handler
	bx  r0
IRQ_Handler:
	push {lr}					@保存lr地址
	push {r0-r3, r12}			@保存r0-r3，r12寄存器

	mrs  r0, spsr				@读取spsr寄存器
	push {r0}					@保存spsr寄存器

	mrc p15,  4, r1, c15, c0, 0 @从CP15的C0寄存器内的值到R1寄存器中
	add r1,  r1, #0X2000		@GIC基地址加0X2000，也就是GIC的CPU接口端基地址
	ldr r0, [r1, #0XC]			@GIC的CPU接口端基地址加0X0C就是GICC_IAR寄存器
								@GICC_IAR寄存器保存这当前发生中断的中断号,我们要根据
								@这个中断号来绝对调用哪个中断服务函数
	push {r0, r1}				@保存r0,r1

	cps #0x13					@进入SVC模式，允许其他中断再次进去

	push {lr}					@保存SVC模式的lr寄存器
	ldr r2, =system_irqhandler	@!加载C语言中断处理函数到r2寄存器中
	blx r2						@运行C语言中断处理函数，带有一个参数，保存在R0寄存器中

	pop {lr}					@执行完C语言中断服务函数，lr出栈
	cps #0x12					@进入IRQ模式
	pop {r0, r1}
	str r0, [r1, #0X10]			@中断执行完成，写EOIR

	pop {r0}
	msr spsr_cxsf, r0			@恢复spsr

	pop {r0-r3, r12}			@r0-r3,r12出栈
	pop {lr}					@lr出栈
	subs pc, lr, #4				@!将lr-4赋给pc

FIQ_Handler:
	ldr r0, =FIQ_Handler
	bx  r0
