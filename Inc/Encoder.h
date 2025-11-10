#ifndef __ENCODER_H
#define __ENCODER_H
#include "main.h"

typedef struct
{	
	__IO uint32_t CaptureCount;	//编码器捕获计数值
	__IO uint16_t OverflowCount ;//编码器溢出次数
	__IO int16_t  Capture_D_Value;//编码器前后2次捕获计数的差值
}Encoder_Struct;

extern Encoder_Struct Encoder_TIM2_Struct;
extern Encoder_Struct Encoder_TIM3_Struct;
extern Encoder_Struct Encoder_TIM4_Struct;
extern Encoder_Struct Encoder_TIM5_Struct;
void Encoder_Struct_Init(void);//初始化编码器结构体参数
void Encoder_UpdataValue(void);//编码器更新数值
#endif
