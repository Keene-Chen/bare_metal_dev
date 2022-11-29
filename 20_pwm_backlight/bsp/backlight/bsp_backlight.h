/**
 * @file    : bsp_backlight.h
 * @author  : KeeneChen
 * @date    : 2022.11.29-10:54:40
 * @details : pwm调节lcd背光
 */

#ifndef __BSP_BACKLIGHT_H__
#define __BSP_BACKLIGHT_H__

/* 背光PWM结构体 */
typedef struct backlight_dev_s {
    unsigned char pwm_duty; // 占空比
} backlight_dev_t;

extern backlight_dev_t backlight_dev;

void backlight_init(void);
void pwm1_set_sample_value(unsigned int value);
void pwm1_set_period_value(unsigned int value);
void pwm1_set_duty(unsigned char duty);
void pwm1_irqhandler(void);
void pwm1_enable(void);

#endif // __BSP_BACKLIGHT_H__