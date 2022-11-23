/**
 * @file    : bsp_key.c
 * @author  : KeeneChen
 * @date    : 2022.11.05-19:01:02
 * @details : bsp_key
 */

#include "bsp_key.h"
#include "bsp_gpio.h"
#include "fsl_iomuxc.h"

void key_init(void)
{
    // 1.设置IO复用为GPIO1_IO18
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);

    /**
     * 2.配置UART1_CTS_B的IO属性
     * bit [16]   : 0   HYS关闭
     * bit [15:14]: 11  默认22K上拉
     * bit [13]   : 1   pull功能
     * bit [12]   : 1   pull/keeper使能
     * bit [11]   : 0   关闭开路输出
     * bit [7:6]  : 10  速度100Mhz
     * bit [5:3]  : 000 关闭输出
     * bit [0]    : 0   低转换率
     */
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF80);

    // 3.GPIO初始化
    gpio_pin_conf_t key_config;
    key_config.direction = GPIO_DIGITAL_INPUT;
    gpio_init(GPIO1, 18, &key_config);
};

int key_getvalue(void)
{
    int ret                = 0;
    static uint8_t release = 1; // 按键释放标志,1:释放,0未释放

    if (release == 1 && gpio_pin_read(GPIO1, 18) == 0) {
        delay(10); // 按键消抖
        release = 0;
        if (gpio_pin_read(GPIO1, 18) == 0) {
            ret = KEY0_VALUE;
        }
    }
    else if (gpio_pin_read(GPIO1, 18) == 1) {
        ret     = KEY_NONE;
        release = 1;
    }

    return ret;
}