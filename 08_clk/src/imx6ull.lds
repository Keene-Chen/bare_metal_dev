/**
 * @file    : imx6ull.lds
 * @author  : KeeneChen
 * @date    : 2022.11.03-11:05:42
 * @details : 汇编链接脚本
 */

SECTIONS{
	. = 0x87800000;
	.text :
	{
		obj/start.o
		*(.text)
	}
	.rodata ALIGN(4) : { *(.rodata*) }
	.data   ALIGN(4) : { *(.data) }

	/* 对当前程序计数器进行4字节对齐 */
	. = ALIGN(4);
	__bss_start = .;
	.bss    ALIGN(4) : { *(.bss) *(COMMON) }
	__bss_end = .;
}