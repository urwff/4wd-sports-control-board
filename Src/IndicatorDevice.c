/**************************************************************************
文件功能：
1、控制蜂鸣器鸣叫次数、鸣叫时长
2、控制红色LED亮灭次数、亮灭时长
3、控制绿色LED亮灭次数、亮灭时长
**************************************************************************/
#include "IndicatorDevice.h"

uint16_t Buzzer_Times=0;//蜂鸣器鸣叫次数
__IO uint16_t BuzzerPeriod_Time=0;//蜂鸣器鸣叫时间
uint16_t BuzzerPeriod_Time_Buffer;//缓存蜂鸣器鸣叫时间
uint8_t Buzzer_LockedStatus=0;//蜂鸣器上锁状态 =1代表上锁 =0代表未上锁

uint16_t LED_Green_Times=0;//绿色LED闪烁次数
__IO uint16_t LED_Green_Period_Time=0;//绿色LED闪烁时间
uint16_t LED_Green_Period_Time_Buffer;//缓存绿色LED亮灯时间
uint8_t LED_Green_LockedStatus=0;//绿色LED上锁状态 =1代表上锁 =0代表未上锁

uint16_t LED_Red_Times=0;//红色LED闪烁次数
__IO uint16_t LED_Red_Period_Time=0;//红色LED闪速时间
uint16_t LED_Red_PeriodTime_Buffer;//缓存红色LED闪烁时间
uint8_t LED_Red_LockedStatus=0;//红色LED上锁状态 =1代表上锁 =0代表未上锁

#define Buzzer(a) 																						 \
						if(a) 																						 \
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,GPIO_PIN_SET);\
						else 																							 \
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,GPIO_PIN_RESET)
						
#define Buzzer_Toggle HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_2)
								
#define LED_Green(a) 																						 \
						if(a) 																						 \
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_SET);\
						else 																							 \
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET)
						
#define LED_Green_Toggle HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_0)//控制LED1 IO口翻转接口

#define LED_Red(a)																				 \
						if(a) 																						 \
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_SET);\
						else 																							 \
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_RESET)			

#define LED_Red_Toggle HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_1)
						
void Buzzer_GPIO_Init(void)//蜂鸣器IO口初始化
{
   /* 定义IO硬件初始化结构体变量 */
  GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOC_CLK_ENABLE();//使能(开启)引脚对应IO端口时钟
	
	/*电机驱动芯片1对应GPIO引脚配置*/
  GPIO_InitStruct.Pin = GPIO_PIN_2;//设定对应引脚IO编号
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//设定对应引脚IO为输出模式
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW ;//设对应引脚IO操作速度
	GPIO_InitStruct.Pull = GPIO_NOPULL;//内部浮空
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);//初始化对应引脚IO  
}

void LED_Green_GPIO_Init(void)//绿色LED IO口初始化
{
   /* 定义IO硬件初始化结构体变量 */
  GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOC_CLK_ENABLE();//使能(开启)引脚对应IO端口时钟
	
	/*电机驱动芯片1对应GPIO引脚配置*/
  GPIO_InitStruct.Pin = GPIO_PIN_0;//设定对应引脚IO编号
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//设定对应引脚IO为输出模式
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW ;//设对应引脚IO操作速度
	GPIO_InitStruct.Pull = GPIO_NOPULL;//内部浮空
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);//初始化对应引脚IO  
}

void LED_Red_GPIO_Init(void)//红色LED IO口初始化
{
   /* 定义IO硬件初始化结构体变量 */
  GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOC_CLK_ENABLE();//使能(开启)引脚对应IO端口时钟
	
	/*电机驱动芯片1对应GPIO引脚配置*/
  GPIO_InitStruct.Pin = GPIO_PIN_1;//设定对应引脚IO编号
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//设定对应引脚IO为输出模式
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW ;//设对应引脚IO操作速度
	GPIO_InitStruct.Pull = GPIO_NOPULL;//内部浮空
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);//初始化对应引脚IO  
}

/*-----------------------------------------------------------
函数功能: 蜂鸣器控制函数
输入参数: 报警次数，鸣叫周期
返 回 值: None
说    明: 
报警次数代表蜂鸣器连续报警的次数，例如该参数为5，代表蜂鸣器会报警5次后停止，如果该参数为0xFFFF，则代表让蜂鸣器一直报警不停
鸣叫周期代表蜂鸣器鸣叫和不叫的时间周期，例如该参数为5，代表蜂鸣器一次报警中的鸣叫和不叫的时间为5ms，如果该参数为0xFFFF，则代表让蜂鸣器一直保持鸣叫的状态
 -----------------------------------------------------------*/
void BuzzerControl(uint16_t Times, uint16_t PeriodTime)//蜂鸣器控制函数
{
	static uint16_t LastTimes=0,LastPeriodTime=0;
	if(LastTimes != Times || LastPeriodTime != PeriodTime || (!Buzzer_Times && !BuzzerPeriod_Time))//防止反复调用该函数导致不正常执行
	{
		LastTimes=Times;
		LastPeriodTime=PeriodTime;
		
		Buzzer_LockedStatus=1;//蜂鸣器上锁状态为1
		
		if(Times)//如果鸣叫次数不为0
		{
			if(PeriodTime == 0xFFFF) Buzzer(1);//如果蜂鸣器鸣叫时间为0xFFFF 代表一直鸣叫
			else if(!PeriodTime) Buzzer(0);//否则如果蜂鸣器鸣叫时间为0 代表停止鸣叫
			else//否则
			{
				Buzzer(0);//蜂鸣器不鸣叫
				Buzzer_Times=Times*2;//蜂鸣器鸣叫次数*2  PS：一开一关为一个周期，因此需要乘2
				BuzzerPeriod_Time_Buffer= PeriodTime;//缓存蜂鸣器鸣叫时间更新
				BuzzerPeriod_Time =0;//蜂鸣器鸣叫时间清0
			}
		}
		else Buzzer(0);//否则蜂鸣器不鸣叫
		
		Buzzer_LockedStatus=0;//蜂鸣器上锁状态为0
	}
}


/*-----------------------------------------------------------
函数功能: 绿色LED控制函数
输入参数: 闪烁次数，闪烁周期
返 回 值: None
说    明: 
闪烁次数代表LED灯连续闪烁的次数，例如该参数为5，代表LED会闪烁5次后停止，如果该参数为0xFFFF，则代表让LED一直闪烁不停
闪烁周期代表LED灯亮灯和灭灯的时间周期，例如该参数为5，代表LED灯一次闪烁中的灯亮灯和灭灯的时间为5ms，如果该参数为0xFFFF，则代表让LED一直保持亮的状态
 -----------------------------------------------------------*/
void LED_Green_Control(uint16_t Times, uint16_t PeriodTime)//绿色LED控制函数
{
	static uint16_t LastTimes=0,LastPeriodTime=0;
	if(LastTimes != Times || LastPeriodTime != PeriodTime || (!LED_Green_Times && !LED_Green_Period_Time))//防止反复调用该函数导致不正常执行
	{
		LastTimes=Times;
		LastPeriodTime=PeriodTime;
		
		LED_Green_LockedStatus=1;//绿色LED上锁状态为1
		
		if(Times)//如果绿色LED闪烁次数不为0
		{
			if(PeriodTime == 0xFFFF) LED_Green(1);//如果绿色LED闪烁时间为0xFFFF 代表一直闪烁
			else if(!PeriodTime) LED_Green(0);//否则如果绿色LED闪烁时间为0 代表绿色LED不亮
			else//否则
			{
				LED_Green(0);//绿色LED不亮
				LED_Green_Times=Times*2;//绿色LED闪烁次数*2  PS：一开一关为一个周期，因此需要乘2
				LED_Green_Period_Time_Buffer= PeriodTime;//缓存绿色LED闪烁时间更新
				LED_Green_Period_Time =0;//绿色LED闪烁时间清0
			}
		}
		else LED_Green(0);//否则绿色LED不亮
		
		LED_Green_LockedStatus=0;//绿色LED上锁状态为0
	}
}

/*-----------------------------------------------------------
函数功能: 红色LED控制函数
输入参数: 闪烁次数，闪烁周期
返 回 值: None
说    明: 
闪烁次数代表LED灯连续闪烁的次数，例如该参数为5，代表LED会闪烁5次后停止，如果该参数为0xFFFF，则代表让LED一直闪烁不停
闪烁周期代表LED灯亮灯和灭灯的时间周期，例如该参数为5，代表LED灯一次闪烁中的灯亮灯和灭灯的时间为5ms，如果该参数为0xFFFF，则代表让LED一直保持亮的状态
 -----------------------------------------------------------*/
void LED_Red_Control(uint16_t Times, uint16_t PeriodTime)//红色LED控制函数
{	
	static uint16_t LastTimes=0,LastPeriodTime=0;
	if(LastTimes != Times || LastPeriodTime != PeriodTime || (!LED_Red_Times && !LED_Red_Period_Time))//防止反复调用该函数导致不正常执行
	{
		LastTimes=Times;
		LastPeriodTime=PeriodTime;
		
		LED_Red_LockedStatus=1;//红色LED上锁状态为1
		
		if(Times)//如果红色LED闪烁次数不为0
		{
			if(PeriodTime == 0xFFFF) LED_Red(1);//如果红色LED闪烁时间为0xFFFF 代表一直闪烁
			else if(!PeriodTime) LED_Red(0);//否则如果红色LED闪烁时间为0 代表红色LED不亮
			else//否则
			{
				LED_Red(0);//红色LED不亮
				LED_Red_Times=Times*2;//红色LED闪烁次数*2  PS：一开一关为一个周期，因此需要乘2
				LED_Red_PeriodTime_Buffer= PeriodTime;//缓存红色LED闪烁时间更新
				LED_Red_Period_Time =0;//红色LED闪烁时间清0
			}
		}
		else LED_Red(0);//否则红色LED不亮
		
		LED_Red_LockedStatus=0;//红色LED上锁状态为0
	}
}

/*-----------------------------------------------------------
函数功能: 指示设备控制函数
输入参数: None
返 回 值: None
说    明: 该函数是控制指示设备的开、关状态，需要将该函数放到
滴答定时器中断函数中运行
 -----------------------------------------------------------*/
void IndicatorDeviceControl(void)
{
	if(BuzzerPeriod_Time) BuzzerPeriod_Time--;//蜂鸣器鸣叫时间
	if(LED_Green_Period_Time) LED_Green_Period_Time--;//绿色LED闪烁时间
	if(LED_Red_Period_Time) LED_Red_Period_Time--;//红色LED闪速时间
	
	if(!Buzzer_LockedStatus)//蜂鸣器没有上锁才执行
	{
		if(!BuzzerPeriod_Time && Buzzer_Times)//如果蜂鸣器鸣叫使时间和鸣叫次数不为0
		{
			Buzzer_Toggle;//控制蜂鸣器IO口电平翻转
			Buzzer_Times--;//蜂鸣器鸣叫次数减1
			BuzzerPeriod_Time=BuzzerPeriod_Time_Buffer;//蜂鸣器鸣叫时间重新赋值
		}
	}
	
	/*------------------*/
	if(!LED_Green_LockedStatus)//绿色LED没有上锁才执行
	{
		if(!LED_Green_Period_Time && LED_Green_Times)//如果绿色LED闪烁时间和闪烁次数不为0
		{
			LED_Green_Toggle;//控制绿色LED IO口电平翻转
			LED_Green_Times--;//绿色LED闪烁次数减1
			LED_Green_Period_Time = LED_Green_Period_Time_Buffer;//绿色LED闪烁时间重新赋值
		}
	}
	
	/*------------------*/
	if(!LED_Red_LockedStatus)//红色LED没有上锁才执行
	{
		if(!LED_Red_Period_Time && LED_Red_Times)//如果红色LED闪烁时间和闪烁次数不为0
		{
			LED_Red_Toggle;//控制红色LED IO口电平翻转
			LED_Red_Times--;//红色LED闪烁次数减1
			LED_Red_Period_Time=LED_Red_PeriodTime_Buffer;//红色LED闪烁时间重新赋值
		}
	}
}
