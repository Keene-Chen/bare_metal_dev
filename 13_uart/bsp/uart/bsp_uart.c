/**
 * @file    : bsp_uart.c
 * @author  : KeeneChen
 * @date    : 2022.11.18-20:14:35
 * @details : UART串口通信实验
 */

#include "bsp_uart.h"
#include "fsl_iomuxc.h"

void uart_init(void)
{
    /* 1.初始化 UART IO */
    uart_io_init();

    /* 2.初始化 UART1 */
    uart_enable(UART1);
    uart_soft_reset(UART1);

    /* 3.清除UCR1寄存器 */
    UART1->UCR1 = 0;

    /**
     * 4.设置UART的UCR1寄存器,关闭自动波特率
     * bit[14]: 0 关闭自动波特率检测,手动设置波特率
     */
    UART1->UCR1 &= ~(1 << 14);

    /**
     * 5.设置UART的UCR2寄存器,设置内容包括字长,停止位,校验模式,关闭RTS硬件流控
     * bit[14]: 1 忽略RTS引脚
     * bit[8] : 0 关闭奇偶校验
     * bit[6] : 0 1位停止位
     * bit[5] : 1 8位数据位
     * bit[2] : 1 打开发送
     * bit[1] : 1 打开接收
     */
    UART1->UCR2 |= (1 << 14 | 1 << 5 | 1 << 2 | 1 << 1);

    /**
     * 6.UART1的UCR3寄存器
     * bit[2]: 1 必须设置为1 参考IMX6ULL参考手册3624页
     */
    UART1->UCR3 |= (1 << 2);

    /**
     * 7.设置波特率
     * 波特率计算公式:Baud Rate = Ref Freq / (16 * (UBMR + 1)/(UBIR+1))
     * 如果要设置波特率为115200，那么可以使用如下参数:
     * Ref Freq = 80M 也就是寄存器UFCR的bit9:7=101, 表示1分频
     * UBMR = 3124
     * UBIR =  71
     * 因此波特率= 80000000/(16 * (3124+1)/(71+1))=80000000/(16 * 3125/72)
     * = (80000000*72) / (16*3125) = 115200
     */
#if 0
    UART1->UFCR = (5 << 7); // ref freq等于ipg_clk/1=80Mhz
    UART1->UBIR = 71;
    UART1->UBMR = 3124;
#endif

    uart_set_baudrate(UART1, 115200, 80000000); /* 设置波特率 */

    /* 8.使能串口 */
    uart_enable(UART1);
}

void uart_io_init(void)
{
    // 设置IO复用与电气属性
    IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_TX, 0);
    IOMUXC_SetPinMux(IOMUXC_UART1_RX_DATA_UART1_RX, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX, 0x10B0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_RX_DATA_UART1_RX, 0x10B0);
}

void uart_soft_reset(UART_Type* base)
{
    base->UCR2 &= ~(1 << 0);        // UCR2的bit0为0,复位UART
    while ((base->UCR2 & 0x1) == 0) // 等待复位完成
        ;
}

void uart_set_baudrate(UART_Type* base, unsigned int baudrate, unsigned int srcclock_hz)
{
    uint32_t numerator       = 0u; // 分子
    uint32_t denominator     = 0U; // 分母
    uint32_t divisor         = 0U;
    uint32_t refFreqDiv      = 0U;
    uint32_t divider         = 1U;
    uint64_t baudDiff        = 0U;
    uint64_t tempNumerator   = 0U;
    uint32_t tempDenominator = 0u;

    /* get the approximately maximum divisor */
    numerator   = srcclock_hz;
    denominator = baudrate << 4;
    divisor     = 1;

    while (denominator != 0) {
        divisor     = denominator;
        denominator = numerator % denominator;
        numerator   = divisor;
    }

    numerator   = srcclock_hz / divisor;
    denominator = (baudrate << 4) / divisor;

    /* numerator ranges from 1 ~ 7 * 64k */
    /* denominator ranges from 1 ~ 64k */
    if ((numerator > (UART_UBIR_INC_MASK * 7)) || (denominator > UART_UBIR_INC_MASK)) {
        uint32_t m   = (numerator - 1) / (UART_UBIR_INC_MASK * 7) + 1;
        uint32_t n   = (denominator - 1) / UART_UBIR_INC_MASK + 1;
        uint32_t max = m > n ? m : n;
        numerator /= max;
        denominator /= max;
        if (0 == numerator) {
            numerator = 1;
        }
        if (0 == denominator) {
            denominator = 1;
        }
    }
    divider = (numerator - 1) / UART_UBIR_INC_MASK + 1;

    switch (divider) {
    case 1:
        refFreqDiv = 0x05;
        break;
    case 2:
        refFreqDiv = 0x04;
        break;
    case 3:
        refFreqDiv = 0x03;
        break;
    case 4:
        refFreqDiv = 0x02;
        break;
    case 5:
        refFreqDiv = 0x01;
        break;
    case 6:
        refFreqDiv = 0x00;
        break;
    case 7:
        refFreqDiv = 0x06;
        break;
    default:
        refFreqDiv = 0x05;
        break;
    }
    /* Compare the difference between baudRate_Bps and calculated baud rate.
     * Baud Rate = Ref Freq / (16 * (UBMR + 1)/(UBIR+1)).
     * baudDiff = (srcClock_Hz/divider)/( 16 * ((numerator / divider)/ denominator).
     */
    tempNumerator   = srcclock_hz;
    tempDenominator = (numerator << 4);
    divisor         = 1;
    /* get the approximately maximum divisor */
    while (tempDenominator != 0) {
        divisor         = tempDenominator;
        tempDenominator = tempNumerator % tempDenominator;
        tempNumerator   = divisor;
    }
    tempNumerator   = srcclock_hz / divisor;
    tempDenominator = (numerator << 4) / divisor;
    baudDiff        = (tempNumerator * denominator) / tempDenominator;
    baudDiff        = (baudDiff >= baudrate) ? (baudDiff - baudrate) : (baudrate - baudDiff);

    if (baudDiff < (baudrate / 100) * 3) {
        base->UFCR &= ~UART_UFCR_RFDIV_MASK;
        base->UFCR |= UART_UFCR_RFDIV(refFreqDiv);
        base->UBIR = UART_UBIR_INC(denominator - 1); // 要先写UBIR寄存器，然后在写UBMR寄存器，3592页
        base->UBMR = UART_UBMR_MOD(numerator / divider - 1);
    }
}

void uart_enable(UART_Type* base)
{
    base->UCR1 |= (1 << 0);
}

void uart_disable(UART_Type* base)
{
    base->UCR1 &= ~(1 << 0);
}

void putc(unsigned char c)
{
    while (((UART1->USR2 >> 3) & 0X01) == 0)
        ;                   // 等待上一次发送完成
    UART1->UTXD = c & 0xFF; // 发送数据
}

void puts(char* str)
{
    char* p = str;

    while (*p)
        putc(*p++);
}

unsigned char getc(void)
{
    while ((UART1->USR2 & 0x1) == 0)
        ;               // 等待接收完成
    return UART1->URXD; // 返回接收到的数据
}

void raise(int sig_nr) {}