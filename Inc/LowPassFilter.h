#ifndef __LOWPASSFILTER_H
#define __LOWPASSFILTER_H
#include "main.h"

typedef struct
{
	float a;// 滤波系数 取值范围0~1
	float OutValue;//滤波后的输出值
	float SampleValue;//采样值 
}LPF_Struct;

extern LPF_Struct LPF_SupplyVoltage;//创建低通滤波电源电压值结构体

extern void LPF_Struct_Init(void);//低通滤波结构体参数初始化

float LowPassFilter(LPF_Struct *v);//低通滤波器函数

#endif
