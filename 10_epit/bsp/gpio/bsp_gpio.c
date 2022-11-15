/**
 * @file    : bsp_gpio.c
 * @author  : KeeneChen
 * @date    : 2022.11.09-16:18:46
 * @details : 通用GPIO配置
 */

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

void gpio_interrupt_enable(GPIO_Type* base, int pin)
{
    base->IMR |= (1 << pin);
}

void gpio_interrupt_disable(GPIO_Type* base, int pin)
{
    base->IMR &= ~(1 << pin);
}

void gpio_interrupt_clear_flags(GPIO_Type* base, int pin)
{
    base->ISR |= (1 << pin);
}

void gpio_interrupt_config(GPIO_Type* base, int pin, gpio_interrupt_mode_um pin_int_mode)
{
    volatile uint32_t* icr;
    uint32_t icr_shift;

    icr_shift = pin;

    base->EDGE_SEL &= ~(1U << pin);

    if (pin < 16) // 低16位
    {
        icr = &(base->ICR1);
    }
    else // 高16位
    {
        icr = &(base->ICR2);
        icr_shift -= 16;
    }
    
    switch (pin_int_mode) {
    case (kGPIO_IntLowLevel):
        *icr &= ~(3U << (2 * icr_shift));
        break;
    case (kGPIO_IntHighLevel):
        *icr = (*icr & (~(3U << (2 * icr_shift)))) | (1U << (2 * icr_shift));
        break;
    case (kGPIO_IntRisingEdge):
        *icr = (*icr & (~(3U << (2 * icr_shift)))) | (2U << (2 * icr_shift));
        break;
    case (kGPIO_IntFallingEdge):
        *icr |= (3U << (2 * icr_shift));
        break;
    case (kGPIO_IntRisingOrFallingEdge):
        base->EDGE_SEL |= (1U << pin);
        break;
    default:
        break;
    }
}