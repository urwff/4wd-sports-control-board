#ifndef __INDICATORDEVICE_H
#define __INDICATORDEVICE_H
#include "main.h"

void Buzzer_GPIO_Init(void);//蜂鸣器IO口初始化
void LED_Green_GPIO_Init(void);//绿色LED灯IO口初始化
void LED_Red_GPIO_Init(void);//红色LED灯IO口初始化
void BuzzerControl(uint16_t Times, uint16_t PeriodTime);//蜂鸣器控制函数
void LED_Green_Control(uint16_t Times, uint16_t PeriodTime);//绿色LED控制函数
void LED_Red_Control(uint16_t Times, uint16_t PeriodTime);//红色LED控制函数
void IndicatorDeviceControl(void);//指示设备控制函数

#endif
