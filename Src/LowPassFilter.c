#include "LowPassFilter.h"

LPF_Struct LPF_SupplyVoltage;//创建低通滤波电源电压值结构体

void LPF_Struct_Init(void)//低通滤波结构体参数初始化
{
	LPF_SupplyVoltage.a = 0.3;//电源电压值低通滤波系数
}

float LowPassFilter(LPF_Struct *v)//低通滤波器函数  
{   
	v->OutValue = v->a * v->SampleValue +  (1 - v->a) * v->OutValue ;   
	return v->OutValue;
} 
