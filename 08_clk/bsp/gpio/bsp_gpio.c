#include "bsp_gpio.h"

void gpio_init(GPIO_Type* base, int pin, gpio_pin_conf_t* config)
{
    // 设置GPIO为输入
    if (config->direction == GPIO_DIGITAL_INPUT) {
        base->GDIR &= ~(1 << pin);
    }
    else {
        base->GDIR |= (1 << pin);
        // 设置默认输出电平
        gpio_pin_write(base, pin, config->output_logic);
    }
}

void gpio_pin_write(GPIO_Type* base, int pin, int value)
{
    if (value == 0)
        base->DR &= ~(1 << pin);
    else
        base->DR |= (1 << pin);
}

int gpio_pin_read(GPIO_Type* base, int pin)
{
    return ((base->DR) >> pin & 0x1);
}