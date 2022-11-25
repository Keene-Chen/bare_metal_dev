/**
 * @file    : bsp_lcd.h
 * @author  : KeeneChen
 * @date    : 2022.11.21-16:00:33
 * @details : LCD驱动
 */

#ifndef __BSP_LCD_H__
#define __BSP_LCD_H__

/* 屏幕颜色 */
#define LCD_WHITE         0x00FFFFFF
#define LCD_GRAY          0x00808080
#define LCD_BLACK         0x00000000
#define LCD_BROWN         0x00A52A2A
#define LCD_ORANGE        0x00FFA500
#define LCD_TRANSPARENT   0x00000000
#define LCD_BLUE          0x000000FF
#define LCD_GREEN         0x0000FF00
#define LCD_RED           0x00FF0000
#define LCD_CYAN          0x0000FFFF
#define LCD_MAGENTA       0x00FF00FF
#define LCD_YELLOW        0x00FFFF00
#define LCD_LIGHT_BLUE    0x008080FF
#define LCD_LIGHT_GREEN   0x0080FF80
#define LCD_LIGHT_RED     0x00FF8080
#define LCD_LIGHT_CYAN    0x0080FFFF
#define LCD_LIGHT_MAGENTA 0x00FF80FF
#define LCD_LIGHT_YELLOW  0x00FFFF80
#define LCD_LIGHT_GRAY    0x00D3D3D3
#define LCD_DARK_BLUE     0x00000080
#define LCD_DARK_GREEN    0x00008000
#define LCD_DARK_RED      0x00800000
#define LCD_DARK_CYAN     0x00008080
#define LCD_DARK_MAGENTA  0x00800080
#define LCD_DARK_YELLOW   0x00808000
#define LCD_DARK_GRAY     0x00404040

/* 屏幕ID */
#define ATK4342 0X4342 // 4.3寸  480*272
#define ATK4384 0X4384 // 4.3寸  800*480
#define ATK7084 0X7084 // 7寸    800*480
#define ATK7016 0X7016 // 7寸    1024*600
#define ATK1018 0X1018 // 10.1寸 1280*800
#define ATKVGA  0xff00 // VGA

/* LCD显存地址 */
#define LCD_FRAMEBUF_ADDR (0x89000000)

/* LCD控制参数结构体 */
typedef struct tftlcd_s {
    unsigned short height;    // LCD屏幕高度
    unsigned short width;     // LCD屏幕宽度
    unsigned short vspw;      // VSYNC 信号持续时间
    unsigned short vbpd;      // 帧同步信号后肩
    unsigned short vfpd;      // 帧同步信号前肩
    unsigned short hspw;      // HSYNC 信号持续时间
    unsigned short hbpd;      // 行同步信号后肩
    unsigned short hfpd;      // 行同步信号前肩
    unsigned char pixsize;    // LCD每个像素所占字节大小
    unsigned int framebuffer; // LCD显存首地址
    unsigned int forecolor;   // 前景色
    unsigned int backcolor;   // 背景色
    unsigned int id;          // 屏幕ID
} tftlcd_t;
extern tftlcd_t tftlcd_dev;

/**
 * @brief  LCD初始化
 * @return void
 */
void lcd_init();

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
unsigned short lcd_read_panelid(void);

/**
 * @brief  屏幕IO初始化
 * @return void
 */
void lcd_gpio_init(void);

/**
 * @brief  LCD时钟初始化, LCD时钟计算公式如下:
 *         LCD CLK = 24 * loop_div / pre_div / div
 * @param  loop_div 设置DIV_SELECT 27~54
 * @param  pre_div  lcdif prediv值 1~8
 * @param  div	    lcdif div值 1~8
 * @return void
 */
void lcd_clk_init(unsigned char loop_div, unsigned char pre_div, unsigned char div);

/**
 * @brief  复位LCD
 * @return void
 */
void lcd_reset(void);

/**
 * @brief  取消复位LCD
 * @return void
 */
void lcd_noreset(void);

/**
 * @brief  使能LCD控制器
 * @return void
 */
void lcd_enable(void);

/**
 * @brief 画点函数
 * @param x	x轴坐标
 * @param y	y轴坐标
 * @param color 颜色值
 * @return void
 */
void lcd_draw_point(unsigned short x, unsigned short y, unsigned int color);

/**
 * @brief 读取指定点的颜色值
 * @param x x轴坐标
 * @param y y轴坐标
 * @return unsigned int 读取到的指定点的颜色值
 */
unsigned int lcd_read_point(unsigned short x, unsigned short y);

/**
 * @brief 清屏
 * @param color 颜色值
 * @return void
 */
void lcd_clear(unsigned int color);

/**
 * @brief 以指定的颜色填充一块矩形
 * @param x0 矩形起始点坐标X轴
 * @param y0 矩形起始点坐标Y轴
 * @param x1 矩形终止点坐标X轴
 * @param y1 矩形终止点坐标Y轴
 * @param color要填充的颜色
 * @return void
 */
void lcd_fill(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, unsigned int color);

#endif // __BSP_LCD_H__