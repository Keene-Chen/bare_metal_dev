/**
 * @file    : bsp_lcd.c
 * @author  : KeeneChen
 * @date    : 2022.11.21-16:00:02
 * @details : LCD驱动
 */

/* 外部模块(优先引用) */
#include "imx6ull.h"
#include "stdio.h"

/* 内部模块 */
#include "bsp_delay.h"
#include "bsp_gpio.h"
#include "bsp_lcd.h"

/* 全局屏幕控制参数变量 */
tftlcd_t tftlcd_dev;

void lcd_init()
{
    /* 1.读取屏幕ID */
    unsigned short lcdid = lcd_read_panelid();
    // printf("LCD ID: %#x\r\n", lcdid);

    /* 2.初始化屏幕IO */
    lcd_gpio_init();
    lcd_reset();
    delay_ms(10);
    lcd_noreset();

    /**
     * 3.根据不同的屏幕ID设置参数
     * 4.设置像素时钟频率
     * 《【正点原子】I.MX6U嵌入式Linux驱动开发指南V1.6》 page 547
     */
    if (lcdid == ATK4342) {
        tftlcd_dev.height = 272;
        tftlcd_dev.width  = 480;
        tftlcd_dev.vspw   = 1;
        tftlcd_dev.vbpd   = 8;
        tftlcd_dev.vfpd   = 8;
        tftlcd_dev.hspw   = 1;
        tftlcd_dev.hbpd   = 40;
        tftlcd_dev.hfpd   = 5;
        lcd_clk_init(27, 8, 8); // 初始化LCD时钟 10.1MHz
    }
    else if (lcdid == ATK4384) {
        tftlcd_dev.height = 480;
        tftlcd_dev.width  = 800;
        tftlcd_dev.vspw   = 3;
        tftlcd_dev.vbpd   = 32;
        tftlcd_dev.vfpd   = 13;
        tftlcd_dev.hspw   = 48;
        tftlcd_dev.hbpd   = 88;
        tftlcd_dev.hfpd   = 40;
        lcd_clk_init(42, 4, 8); // 初始化LCD时钟 31.5MHz
    }
    else if (lcdid == ATK7084) {
        tftlcd_dev.height = 480;
        tftlcd_dev.width  = 800;
        tftlcd_dev.vspw   = 1;
        tftlcd_dev.vbpd   = 23;
        tftlcd_dev.vfpd   = 22;
        tftlcd_dev.hspw   = 1;
        tftlcd_dev.hbpd   = 46;
        tftlcd_dev.hfpd   = 210;
        lcd_clk_init(30, 3, 7); // 初始化LCD时钟 34.2MHz
    }
    else if (lcdid == ATK7016) {
        tftlcd_dev.height = 600;
        tftlcd_dev.width  = 1024;
        tftlcd_dev.vspw   = 3;
        tftlcd_dev.vbpd   = 20;
        tftlcd_dev.vfpd   = 12;
        tftlcd_dev.hspw   = 20;
        tftlcd_dev.hbpd   = 140;
        tftlcd_dev.hfpd   = 160;
        lcd_clk_init(32, 3, 5); // 初始化LCD时钟 51.2MHz
    }
    else if (lcdid == ATK1018) {
        tftlcd_dev.height = 800;
        tftlcd_dev.width  = 1280;
        tftlcd_dev.vspw   = 3;
        tftlcd_dev.vbpd   = 10;
        tftlcd_dev.vfpd   = 10;
        tftlcd_dev.hspw   = 10;
        tftlcd_dev.hbpd   = 80;
        tftlcd_dev.hfpd   = 70;
        lcd_clk_init(35, 3, 5); // 初始化LCD时钟 56MHz
    }
    else if (lcdid == ATKVGA) {
        tftlcd_dev.height = 768;
        tftlcd_dev.width  = 1366;
        tftlcd_dev.vspw   = 3;
        tftlcd_dev.vbpd   = 24;
        tftlcd_dev.vfpd   = 3;
        tftlcd_dev.hspw   = 143;
        tftlcd_dev.hbpd   = 213;
        tftlcd_dev.hfpd   = 70;
        lcd_clk_init(32, 3, 3); // 初始化LCD时钟 85MHz
    }
    tftlcd_dev.pixsize     = 4; // ARGB8888模式,每个像素4字节
    tftlcd_dev.framebuffer = LCD_FRAMEBUF_ADDR;
    tftlcd_dev.forecolor   = LCD_BLACK; // 前景色为黑色
    tftlcd_dev.backcolor   = LCD_WHITE; // 背景色为白色

    /**
     * 5.初始化ELCDIF的CTRL寄存器
     * bit [31]    : 0  停止复位
     * bit [19]    : 1  旁路计数器模式
     * bit [17]    : 1  LCD工作在dotclk模式
     * bit [15:14] : 00 输入数据不交换
     * bit [13:12] : 00 CSC不交换
     * bit [11:10] : 11 24位总线宽度
     * bit [9:8]   : 11 24位数据宽度,也就是RGB888
     * bit [5]     : 1  elcdif工作在主模式
     * bit [1]     : 0  所有的24位均有效
     */
    LCDIF->CTRL = 0;
    LCDIF->CTRL |= (1 << 19) | (1 << 17) | (0 << 14) | (0 << 12) | (3 << 10) | (3 << 8) | (1 << 5) | (0 << 1);

    /**
     * 6.初始化ELCDIF的寄存器CTRL1
     * bit [19:16] : 0x7 ARGB模式下,传输24位数据,A通道不用传输
     */
    LCDIF->CTRL1 = 0;
    LCDIF->CTRL1 = (0x7 << 16);

    /**
     * 7.初始化ELCDIF的寄存器TRANSFER_COUNT寄存器
     * bit [31:16] : 高度
     * bit [15:0]  : 宽度
     */
    LCDIF->TRANSFER_COUNT = (tftlcd_dev.height << 16) | (tftlcd_dev.width << 0);

    /**
     * 8.初始化ELCDIF的VDCTRL0寄存器
     * bit [29]   : 0 VSYNC输出
     * bit [28]   : 1 使能ENABLE输出
     * bit [27]   : 0 VSYNC低电平有效
     * bit [26]   : 0 HSYNC低电平有效
     * bit [25]   : 0 DOTCLK上升沿有效
     * bit [24]   : 1 ENABLE信号高电平有效
     * bit [21]   : 1 DOTCLK模式下设置为1
     * bit [20]   : 1 DOTCLK模式下设置为1
     * bit [17:0] : vsw参数
     */
    LCDIF->VDCTRL0 = 0;    // 先清零
    if (lcdid == ATKVGA) { // VGA需要特殊处理
        LCDIF->VDCTRL0 = (0 << 29) | (1 << 28) | (0 << 27) | (0 << 26) | (1 << 25) | (0 << 24) | (1 << 21) | (1 << 20) | (tftlcd_dev.vspw << 0);
    }
    else {
        LCDIF->VDCTRL0 = (0 << 29) | (1 << 28) | (0 << 27) | (0 << 26) | (0 << 25) | (1 << 24) | (1 << 21) | (1 << 20) | (tftlcd_dev.vspw << 0);
    }

    /**
     * 9.初始化ELCDIF的VDCTRL1寄存器
     * 设置VSYNC总周期
     */
    LCDIF->VDCTRL1 = tftlcd_dev.height + tftlcd_dev.vspw + tftlcd_dev.vfpd + tftlcd_dev.vbpd; // VSYNC周期

    /**
     * 10.初始化ELCDIF的VDCTRL2寄存器
     * 设置HSYNC周期
     * bit [31:18] : hsw
     * bit [17:0]  : HSYNC总周期
     */
    LCDIF->VDCTRL2 = (tftlcd_dev.hspw << 18) | (tftlcd_dev.width + tftlcd_dev.hspw + tftlcd_dev.hfpd + tftlcd_dev.hbpd);

    /**
     * 11.初始化ELCDIF的VDCTRL3寄存器
     * 设置HSYNC周期
     * bit [27:16] : 水平等待时钟数
     * bit [15:0]  : 垂直等待时钟数
     */
    LCDIF->VDCTRL3 = ((tftlcd_dev.hbpd + tftlcd_dev.hspw) << 16) | (tftlcd_dev.vbpd + tftlcd_dev.vspw);

    /**
     * 12.初始化ELCDIF的VDCTRL4寄存器
     * 设置HSYNC周期
     * bit [18]   : 1 当使用VSHYNC、HSYNC、DOTCLK的话此为置1
     * bit [17:0] : 宽度
     */
    LCDIF->VDCTRL4 = (1 << 18) | (tftlcd_dev.width);

    /**
     * 13.初始化ELCDIF的CUR_BUF和NEXT_BUF寄存器
     * 设置当前显存地址和下一帧的显存地址
     */
    LCDIF->CUR_BUF  = (unsigned int)tftlcd_dev.framebuffer;
    LCDIF->NEXT_BUF = (unsigned int)tftlcd_dev.framebuffer;

    /* 14.LCD使能 */
    lcd_enable();
    delay_ms(10);
    lcd_clear(LCD_WHITE);
}

/**
 * @brief  读取屏幕ID
 *         LCD_DATA23=R7(M0); LCD_DATA15=G7(M1); LCD_DATA07=B7(M2);
 *         M2:M1:M0
 *         0 :0 :0  4.3寸  480*272  RGB屏,ID=0X4342
 *         0 :0 :1  7寸    800*480  RGB屏,ID=0X7084
 *         0 :1 :0  7寸    1024*600 RGB屏,ID=0X7016
 *         1 :0 :0  4.3寸  800*480  RGB屏,ID=0X4384
 *         1 :0 :1  10.1寸 1280*800 RGB屏,ID=0X1018
 * @return unsigned short 屏幕ID
 */
unsigned short lcd_read_panelid(void)
{
    /* 1.打开模拟开关,设置LCD_VSYNC为高电平 */
    IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_GPIO3_IO03, 0);
    IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_GPIO3_IO03, 0x10B0);

    gpio_pin_conf_t lcdio_config;
    lcdio_config.direction    = GPIO_DIGITAL_OUTPUT;
    lcdio_config.output_logic = 1;
    gpio_init(GPIO3, 3, &lcdio_config);

    /* 2.读取屏幕ID */
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_GPIO3_IO12, 0); // B7(M2)
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_GPIO3_IO20, 0); // G7(M1)
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_GPIO3_IO28, 0); // R7(M0)
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_GPIO3_IO12, 0xF080);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_GPIO3_IO20, 0xF080);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_GPIO3_IO28, 0xF080);

    lcdio_config.direction = GPIO_DIGITAL_INPUT;
    gpio_init(GPIO3, 12, &lcdio_config);
    gpio_init(GPIO3, 20, &lcdio_config);
    gpio_init(GPIO3, 28, &lcdio_config);

    unsigned char idx = 0;

    /* 3.通过读取引脚电平,判断屏幕ID */
    idx = (unsigned char)gpio_pin_read(GPIO3, 12) << 0;  // M2
    idx |= (unsigned char)gpio_pin_read(GPIO3, 20) << 1; // M1
    idx |= (unsigned char)gpio_pin_read(GPIO3, 28) << 2; // M0

    if (idx == 0)
        return ATK4342;
    else if (idx == 1)
        return ATK7084;
    else if (idx == 2)
        return ATK7016;
    else if (idx == 4)
        return ATK4384;
    else if (idx == 5)
        return ATK1018;
    else
        return 0;
}

/**
 * IO引脚:
 * LCD_DATA[00-07]  -> LCD_B[0-7]
 * LCD_DATA[08-15] -> LCD_G[0-7]
 * LCD_DATA[16-23] -> LCD_R[0-7]
 *
 * LCD_CLK   -> LCD_CLK
 * LCD_VSYNC -> LCD_VSYNC
 * LCD_HSYNC -> LCD_HSYNC
 * LCD_DE    -> LCD_DE
 * LCD_BL    -> GPIO1_IO08
 */
void lcd_gpio_init(void)
{
    /* 1.配置IO复用 */
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA00_LCDIF_DATA00, 0); // LCD 8bit blue channel
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA01_LCDIF_DATA01, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA02_LCDIF_DATA02, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA03_LCDIF_DATA03, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA04_LCDIF_DATA04, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA05_LCDIF_DATA05, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA06_LCDIF_DATA06, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_LCDIF_DATA07, 0);

    IOMUXC_SetPinMux(IOMUXC_LCD_DATA08_LCDIF_DATA08, 0); // LCD 8bit green channel
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA09_LCDIF_DATA09, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA10_LCDIF_DATA10, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA11_LCDIF_DATA11, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA12_LCDIF_DATA12, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA13_LCDIF_DATA13, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA14_LCDIF_DATA14, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_LCDIF_DATA15, 0);

    IOMUXC_SetPinMux(IOMUXC_LCD_DATA16_LCDIF_DATA16, 0); // LCD 8bit red channel
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA17_LCDIF_DATA17, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA18_LCDIF_DATA18, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA19_LCDIF_DATA19, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA20_LCDIF_DATA20, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA21_LCDIF_DATA21, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA22_LCDIF_DATA22, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_LCDIF_DATA23, 0);

    IOMUXC_SetPinMux(IOMUXC_LCD_CLK_LCDIF_CLK, 0);       // LCD 时钟控制
    IOMUXC_SetPinMux(IOMUXC_LCD_ENABLE_LCDIF_ENABLE, 0); // LCD 启用
    IOMUXC_SetPinMux(IOMUXC_LCD_HSYNC_LCDIF_HSYNC, 0);   // 垂直同步信号
    IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_LCDIF_VSYNC, 0);   // 水平同步信号

    // IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);

    /**
     * 2.配置LCD IO电气属性
     * bit [16]   : 0   HYS关闭
     * bit [15:14]: 0   默认22K上拉
     * bit [13]   : 0   pull功能
     * bit [12]   : 0   pull/keeper使能
     * bit [11]   : 0   关闭开路输出
     * bit [7:6]  : 10  速度100Mhz
     * bit [5:3]  : 111 驱动能力为R0/7
     * bit [0]    : 1   高转换率
     */
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA00_LCDIF_DATA00, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA01_LCDIF_DATA01, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA02_LCDIF_DATA02, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA03_LCDIF_DATA03, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA04_LCDIF_DATA04, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA05_LCDIF_DATA05, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA06_LCDIF_DATA06, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_LCDIF_DATA07, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA08_LCDIF_DATA08, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA09_LCDIF_DATA09, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA10_LCDIF_DATA10, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA11_LCDIF_DATA11, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA12_LCDIF_DATA12, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA13_LCDIF_DATA13, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA14_LCDIF_DATA14, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_LCDIF_DATA15, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA16_LCDIF_DATA16, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA17_LCDIF_DATA17, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA18_LCDIF_DATA18, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA19_LCDIF_DATA19, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA20_LCDIF_DATA20, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA21_LCDIF_DATA21, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA22_LCDIF_DATA22, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_LCDIF_DATA23, 0xB9);

    IOMUXC_SetPinConfig(IOMUXC_LCD_CLK_LCDIF_CLK, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_ENABLE_LCDIF_ENABLE, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_HSYNC_LCDIF_HSYNC, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_LCDIF_VSYNC, 0xB9);

    /* 3.背光GPIO初始化 */
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO08_GPIO1_IO08, 0); // 背光BL引脚
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO08_GPIO1_IO08, 0xB9);
    gpio_pin_conf_t bl_config;
    bl_config.direction    = GPIO_DIGITAL_OUTPUT; // 输出
    bl_config.output_logic = 1;                   // 默认关闭背光
    gpio_init(GPIO1, 8, &bl_config);              // 背光默认打开
    gpio_pin_write(GPIO1, 8, 1);
}

void lcd_clk_init(unsigned char loop_div, unsigned char pre_div, unsigned char div)
{
    /* 1.禁用小数分频器 */
    CCM_ANALOG->PLL_VIDEO_NUM   = 0;
    CCM_ANALOG->PLL_VIDEO_DENOM = 0;

    /**
     * 2.PLL_VIDEO寄存器设置
     * bit [20:19] :  2  设置postDivider为1分频
     * bit [13]    :  1  使能VIDEO PLL时钟
     * bit [6:0]   : 32  设置loopDivider寄存器
     */
    CCM_ANALOG->PLL_VIDEO = (loop_div << 0) | (1 << 13) | (2 << 19);

    /**
     * 3.MISC2寄存器设置
     * bit [31:30] : 0  VIDEO的post-div设置,时钟源来源于postDivider,1分频
     */
    CCM_ANALOG->MISC2 &= ~(3 << 30);
    // CCM_ANALOG->MISC2 = (0 << 30);

    /* 4.LCD时钟源来源与PLL5,也就是VIDEO PLL  */
    CCM->CSCDR2 &= ~(7 << 15);
    CCM->CSCDR2 |= (2 << 15); // 设置LCDIF_PRE_CLK使用PLL5

    /* 5.设置LCDIF_PRE分频 */
    CCM->CSCDR2 &= ~(7 << 12);
    CCM->CSCDR2 |= ((pre_div - 1) << 12); // 设置分频

    /* 6.设置LCDIF分频 */
    CCM->CBCMR &= ~(7 << 23);
    CCM->CBCMR |= ((div - 1) << 23);

    /* 7.设置LCD时钟源为LCDIF_PRE时钟 */
    CCM->CSCDR2 &= ~(7 << 9); // 清除原来的设置
    CCM->CSCDR2 |= (0 << 9);  // LCDIF_PRE时钟源选择LCDIF_PRE时钟
}

void lcd_reset(void)
{
    LCDIF->CTRL |= (1 << 31);
}

void lcd_noreset(void)
{
    LCDIF->CTRL &= ~(1 << 31);
}

void lcd_enable(void)
{
    LCDIF->CTRL |= (1 << 0);
}

void lcd_draw_point(unsigned short x, unsigned short y, unsigned int color)
{
    *(unsigned int*)((unsigned int)tftlcd_dev.framebuffer + tftlcd_dev.pixsize * (tftlcd_dev.width * y + x)) = color;
}

unsigned int lcd_read_point(unsigned short x, unsigned short y)
{
    return *(unsigned int*)((unsigned int)tftlcd_dev.framebuffer + tftlcd_dev.pixsize * (tftlcd_dev.width * y + x));
}

void lcd_clear(unsigned int color)
{
    unsigned int* startaddr = (unsigned int*)tftlcd_dev.framebuffer;              // 指向帧缓存首地址
    unsigned int num        = (unsigned int)tftlcd_dev.width * tftlcd_dev.height; // 缓冲区总长度
    for (int i = 0; i < num; i++) {
        startaddr[i] = color;
    }
}

void lcd_fill(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, unsigned int color)
{
    if (x0 < 0)
        x0 = 0;
    if (y0 < 0)
        y0 = 0;
    if (x1 >= tftlcd_dev.width)
        x1 = tftlcd_dev.width - 1;
    if (y1 >= tftlcd_dev.height)
        y1 = tftlcd_dev.height - 1;

    for (unsigned short y = y0; y <= y1; y++) {
        for (unsigned short x = x0; x <= x1; x++)
            lcd_draw_point(x, y, color);
    }
}