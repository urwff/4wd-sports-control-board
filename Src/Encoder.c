/**************************************************************************
文件功能：
1、获取和更新（小车左前、右前、左后、右后）编码器捕获计数值
2、获取和更新（小车左前、右前、左后、右后）编码器前后2次捕获计数的差值
**************************************************************************/

#include "Encoder.h"
#include "tim2.h"
#include "tim3.h"
#include "tim4.h"
#include "tim5.h"

Encoder_Struct Encoder_TIM2_Struct;
Encoder_Struct Encoder_TIM3_Struct;
Encoder_Struct Encoder_TIM4_Struct;
Encoder_Struct Encoder_TIM5_Struct;

/**************************************************************************
函数功能：初始化编码器结构体参数
入口参数：None
返 回 值：None
说    明: None
**************************************************************************/
void Encoder_Struct_Init(void)
{
	Encoder_TIM2_Struct.CaptureCount = 0;	//编码器捕获计数值
	Encoder_TIM2_Struct.OverflowCount = 0;//编码器溢出次数
	Encoder_TIM2_Struct.Capture_D_Value = 0;//编码器前后2次捕获计数的差值
	
	Encoder_TIM3_Struct.CaptureCount = 0;	//编码器捕获计数值
	Encoder_TIM3_Struct.OverflowCount = 0;//编码器溢出次数
	Encoder_TIM3_Struct.Capture_D_Value = 0;//编码器前后2次捕获计数的差值
	
	Encoder_TIM4_Struct.CaptureCount = 0;	//编码器捕获计数值
	Encoder_TIM4_Struct.OverflowCount = 0;//编码器溢出次数
	Encoder_TIM4_Struct.Capture_D_Value = 0;//编码器前后2次捕获计数的差值
	
	Encoder_TIM5_Struct.CaptureCount = 0;	//编码器捕获计数值
	Encoder_TIM5_Struct.OverflowCount = 0;//编码器溢出次数
	Encoder_TIM5_Struct.Capture_D_Value = 0;//编码器前后2次捕获计数的差值
}

/**************************************************************************
函数功能：编码器更新数值
入口参数：None
返 回 值：None
说    明: 调用该函数后编码器的
“捕获计数值”
“编码器前后2次捕获计数的差值”
会获得更新
**************************************************************************/
void Encoder_UpdataValue(void)
{
  uint16_t Encoder_TIM;
	
	/*------------------------------TIM2------------------------------*/
	static uint16_t Encoder_TIM2_Last=0;
	Encoder_TIM= __HAL_TIM_GET_COUNTER(&htim2);//读取TIM2->CNT计数器值
	if(Encoder_TIM > Encoder_TIM2_Last)
	{
		Encoder_TIM2_Struct.Capture_D_Value = Encoder_TIM - Encoder_TIM2_Last;//获取编码器前后2次捕获计数的差值
		if(Encoder_TIM2_Struct.Capture_D_Value > 10000) Encoder_TIM2_Struct.Capture_D_Value -= 65536;//判断是否溢出跳变
	}
	else
	{
		Encoder_TIM2_Struct.Capture_D_Value = 0-(Encoder_TIM2_Last - Encoder_TIM);//获取编码器前后2次捕获计数的差值
		if(Encoder_TIM2_Struct.Capture_D_Value > 10000) Encoder_TIM2_Struct.Capture_D_Value += 65536;//判断是否溢出跳变
	}
	Encoder_TIM2_Last = Encoder_TIM;

	Encoder_TIM2_Struct.CaptureCount = Encoder_TIM2_Struct.OverflowCount*65536 + Encoder_TIM;//获取编码器捕获计数值
		
	/*------------------------------TIM3------------------------------*/
	static uint16_t Encoder_TIM3_Last=0;
	Encoder_TIM= __HAL_TIM_GET_COUNTER(&htim3);//读取TIM3->CNT计数器值
	if(Encoder_TIM > Encoder_TIM3_Last)
	{
		Encoder_TIM3_Struct.Capture_D_Value = Encoder_TIM - Encoder_TIM3_Last;//获取编码器前后2次捕获计数的差值
		if(Encoder_TIM3_Struct.Capture_D_Value > 10000) Encoder_TIM3_Struct.Capture_D_Value -= 65536;//判断是否溢出跳变
	}
	else
	{
		Encoder_TIM3_Struct.Capture_D_Value = 0-(Encoder_TIM3_Last - Encoder_TIM);//获取编码器前后2次捕获计数的差值
		if(Encoder_TIM3_Struct.Capture_D_Value > 10000) Encoder_TIM3_Struct.Capture_D_Value += 65536;//判断是否溢出跳变
	}
	Encoder_TIM3_Last = Encoder_TIM;

	Encoder_TIM3_Struct.CaptureCount = Encoder_TIM3_Struct.OverflowCount*65536 + Encoder_TIM;//获取编码器捕获计数值

	/*------------------------------TIM4------------------------------*/
	static uint16_t Encoder_TIM4_Last=0;
	Encoder_TIM= __HAL_TIM_GET_COUNTER(&htim4);//读取TIM4->CNT计数器值
	
	if(Encoder_TIM > Encoder_TIM4_Last)
	{
		Encoder_TIM4_Struct.Capture_D_Value = Encoder_TIM - Encoder_TIM4_Last;//获取编码器前后2次捕获计数的差值
		if(Encoder_TIM4_Struct.Capture_D_Value > 10000) Encoder_TIM4_Struct.Capture_D_Value -= 65536;//判断是否溢出跳变
	}
	else
	{
		Encoder_TIM4_Struct.Capture_D_Value = 0-(Encoder_TIM4_Last - Encoder_TIM);//获取编码器前后2次捕获计数的差值
		if(Encoder_TIM4_Struct.Capture_D_Value > 10000) Encoder_TIM4_Struct.Capture_D_Value += 65536;//判断是否溢出跳变
	}
	Encoder_TIM4_Last = Encoder_TIM;

	Encoder_TIM4_Struct.Capture_D_Value = 0-Encoder_TIM4_Struct.Capture_D_Value;//获取编码器前后2次捕获计数的差值
	Encoder_TIM4_Struct.CaptureCount = 0-(Encoder_TIM4_Struct.OverflowCount*65536 + Encoder_TIM);//获取编码器捕获计数值
	
	/*------------------------------TIM5------------------------------*/
	static uint16_t Encoder_TIM5_Last=0;
	Encoder_TIM= __HAL_TIM_GET_COUNTER(&htim5);//读取TIM5->CNT计数器值
	
	if(Encoder_TIM > Encoder_TIM5_Last)
	{
		Encoder_TIM5_Struct.Capture_D_Value = Encoder_TIM - Encoder_TIM5_Last;//获取编码器前后2次捕获计数的差值
		if(Encoder_TIM5_Struct.Capture_D_Value > 10000) Encoder_TIM5_Struct.Capture_D_Value -= 65536;//判断是否溢出跳变
	}
	else
	{
		Encoder_TIM5_Struct.Capture_D_Value = 0-(Encoder_TIM5_Last - Encoder_TIM);//获取编码器前后2次捕获计数的差值
		if(Encoder_TIM5_Struct.Capture_D_Value > 10000) Encoder_TIM5_Struct.Capture_D_Value += 65536;//判断是否溢出跳变
	}
	Encoder_TIM5_Last = Encoder_TIM;
	
	Encoder_TIM5_Struct.Capture_D_Value = 0-Encoder_TIM5_Struct.Capture_D_Value;//获取编码器前后2次捕获计数的差值
	Encoder_TIM5_Struct.CaptureCount = 0-(Encoder_TIM5_Struct.OverflowCount*65536 + Encoder_TIM);//获取编码器捕获计数值
}