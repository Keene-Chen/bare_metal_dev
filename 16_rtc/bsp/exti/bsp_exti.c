/**
 * @file    : bsp_exti.c
 * @author  : KeeneChen
 * @date    : 2022.11.09-18:35:53
 * @details : 外部中断头文件
 */

#include "bsp_exti.h"
#include "bsp_beep.h"
#include "bsp_delay.h"
#include "bsp_gpio.h"
#include "bsp_int.h"

void exit_init(void)
{
    // 1.设置IO复用
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0); //复用为GPIO1_IO18
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);

    // 2.初始化GPIO为中断模式
    gpio_pin_conf_t key_config;
    key_config.direction      = GPIO_DIGITAL_INPUT;
    key_config.interrupt_mode = kGPIO_IntFallingEdge;
    key_config.output_logic   = 1;
    gpio_init(GPIO1, 18, &key_config);

    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn); // 使能GIC中对应的中断
    system_register_irqhandler(GPIO1_Combined_16_31_IRQn, (system_irq_handler_t)gpio1_io18_irqhandler, NULL); // 注册中断服务函数
    gpio_interrupt_enable(GPIO1, 18); // 使能GPIO1_IO18的中断功能
}

void gpio1_io18_irqhandler(void) {
    static unsigned char state = 0;

    //! 禁止在中断服务函数中使用延时函数,由于还未学习定时器所以使用延时函数测试功能
	delay(10);
	if(gpio_pin_read(GPIO1, 18) == 0)	// 按键按下了
	{
		state = !state;
		beep_switch(state);
	}
	
	gpio_interrupt_clear_flags(GPIO1, 18); // 清除中断标志位
}