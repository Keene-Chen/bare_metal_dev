/**
 * @file    : bsp_rtc.h
 * @author  : KeeneChen
 * @date    : 2022.11.23-09:30:19
 * @details : RTC驱动
 */

#ifndef __BSP_RTC_H__
#define __BSP_RTC_H__

/* 相关宏定义 */
#define SECONDS_IN_A_DAY    (86400) // 一天86400秒
#define SECONDS_IN_A_HOUR   (3600)  // 一个小时3600秒
#define SECONDS_IN_A_MINUTE (60)    // 一分钟60秒
#define DAYS_IN_A_YEAR      (365)   // 一年365天
#define YEAR_RANGE_START    (1970)  // 开始年份1970年
#define YEAR_RANGE_END      (2099)  // 结束年份2099年

/* 时间日期结构体 */
typedef struct rtc_datetime_s {
    unsigned short year;  // 范围为:1970 ~ 2099
    unsigned char month;  // 范围为:1 ~ 12
    unsigned char day;    // 范围为:1 ~ 31 (不同的月，天数不同)
    unsigned char hour;   // 范围为:0 ~ 23
    unsigned char minute; // 范围为:0 ~ 59
    unsigned char second; // 范围为:0 ~ 59
} rtc_datetime_t;

/* 函数声明 */
void rtc_init(void);
void rtc_enable(void);
void rtc_disable(void);
unsigned int rtc_coverdate_to_seconds(rtc_datetime_t* datetime);
unsigned int rtc_getseconds(void);
void rtc_setdatetime(rtc_datetime_t* datetime);
void rtc_getdatetime(rtc_datetime_t* datetime);

#endif // __BSP_RTC_H__