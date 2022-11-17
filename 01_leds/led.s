/**
 * @file    : led.s
 * @author  : KeeneChen
 * @date    : 2022.11.02-12:24:52
 * @details : I.MX6ULL 汇编点灯
 */

/* .global 全局标号(伪指令) _start为入口 */
.global _start

_start:

    /**
     * 1.使能所有时钟
     * 寄存器: CCM_CCGR[0-6]
     */
    ldr r0, =0x020C4068 @读取立即数 CCGR0
    ldr r1, =0xFFFFFFFF @待写入数据
    str r1, [r0]        @写入数据

    ldr r0, =0x020C406C @CCGR1
    str r1, [r0]

    ldr r0, =0x020C4070 @CCGR2
    str r1, [r0]

    ldr r0, =0x020C4074 @CCGR3
    str r1, [r0]

    ldr r0, =0x020C4078 @CCGR4
    str r1, [r0]

    ldr r0, =0x020C407C @CCGR5
    str r1, [r0]

    ldr r0, =0x020C4080 @CCGR6
    str r1, [r0]

    /**
     * 2.配置GPIO1_IO03 PIN为GPIO功能
     * 寄存器: IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03
     * 寄存器地址: 0x020E0068
     */
    ldr r0, =0x020E0068 @IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03 地址
    ldr r1, =0x00000005 @设置GPIO模式  二进制: 0101 十六进制: 0x00000005
    str r1, [r0]

    /**
     * 3.配置GPIO1_IO03的电气属性
     * 寄存器: IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03
     * 寄存器地址: 0x020E02F4
     *
     * bit[16]   : 0   HYS关闭
     * bit[15:14]: 00  默认下拉
     * bit[13]   : 0   kepper功能
     * bit[12]   : 1   pull/keeper使能
     * bit[11]   : 0   关闭开路输出
     * bit[10:8] :     保留字段
     * bit[7:6]  : 10  速度100Mhz
     * bit[5:3]  : 110 R0/6驱动能力
     * bit[2:1]  :     保留字段
     * bit[0]    : 0   低转换率
     */
    ldr r0, =0x020E02F4
    ldr r1, =0x10B0
    str r1,[r0]

    /**
     * 4.配置GPIO,设置GPIO1_IO03为输出模式
     * 寄存器: GPIO1_GDIR
     * 寄存地址: 0x0209C004
     */
    ldr r0, =0x0209C004
    ldr r1, =0x0000008
    str r1, [r0]

    /**
     * 5.点亮LED0,设置GPIO1_IO03输出低电平
     * 寄存器: GPIO1_DR
     * 寄存地址: 0x0209C000
     */
    ldr r0, =0x0209C000
    ldr r1, =0
    str r1, [r0]


/* loop 死循环 */
loop:
    b loop
