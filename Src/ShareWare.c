#include "ShareWare.h"
#include "IndicatorDevice.h"
#include "tim2.h"
#include "tim3.h"
#include "tim4.h"
#include "tim5.h"
#include "Encoder.h"

extern __IO uint16_t Loop_10msTime;
extern __IO uint16_t Loop_500msTime;
extern __IO uint16_t HeartbeatTime;//心跳包时间
/*-----------------------------------------------------------
函数功能: 滴答定时器中断函数
输入参数: None
返 回 值: None
说    明: 滴答定时器中断周期为1ms，每隔1ms就会进入一次该函数
 -----------------------------------------------------------*/
void HAL_SYSTICK_Callback()//系统滴答定时器1ms中断函数
{
	if(Loop_10msTime) Loop_10msTime--;//10ms轮询时间
	if(Loop_500msTime) Loop_500msTime--;//500ms轮询时间
	if(HeartbeatTime) HeartbeatTime--;//心跳包时间
	IndicatorDeviceControl();
}


/*-----------------------------------------------------------
函数功能: 编码器计数溢出中断函数
输入参数: 定时器句柄
返 回 值: None
说    明: 编码器计数溢出后会进入该中断函数
 0xFFFF跳变到0x0000，溢出
 0x0000跳变到0xFFFF，溢出
 -----------------------------------------------------------*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2))
		{
			Encoder_TIM2_Struct.OverflowCount--;       //向下计数溢出
		}
		else
		{
			Encoder_TIM2_Struct.OverflowCount++;  		 //向上计数溢出
		}
	}
	
	if(htim->Instance == TIM3)
	{
		if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3))
		{
			Encoder_TIM3_Struct.OverflowCount--;       //向下计数溢出
		}
		else
		{
			Encoder_TIM3_Struct.OverflowCount++;  		 //向上计数溢出
		}
	}
	
	if(htim->Instance == TIM4)
	{
		if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim4))
		{
			Encoder_TIM4_Struct.OverflowCount--;       //向下计数溢出
		}
		else
		{
			Encoder_TIM4_Struct.OverflowCount++;  		 //向上计数溢出
		}
	}

	if(htim->Instance == TIM5)
	{
		if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim5))
		{
			Encoder_TIM5_Struct.OverflowCount--;       //向下计数溢出
		}
		else
		{
			Encoder_TIM5_Struct.OverflowCount++;  		 //向上计数溢出
		} 
	}
}
