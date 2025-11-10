#include "MotorControl.h"

#define M1_ENB(a)	\
						if (a)	\
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,GPIO_PIN_SET);\
						else		\
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,GPIO_PIN_RESET)
#define M2_ENB(a)	\
						if (a)	\
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,GPIO_PIN_SET);	\
						else		\
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,GPIO_PIN_RESET)
#define M3_ENB(a)	\
						if (a)	\
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,GPIO_PIN_SET);	\
						else		\
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,GPIO_PIN_RESET)
#define M4_ENB(a)	\
						if (a)	\
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15,GPIO_PIN_SET);	\
						else		\
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15,GPIO_PIN_RESET)						

void MotorControl_GPIO_Init(void)//电机控制IO初始化
{
	   /* 定义IO硬件初始化结构体变量 */
  GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOB_CLK_ENABLE();//使能(开启)引脚对应IO端口时钟
	
	/*电机驱动芯片对应GPIO引脚配置*/
	GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;//设定对应引脚IO编号 
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//设定对应引脚IO为输出模式
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW ;//设对应引脚IO操作速度
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);//设定对应引脚电平状态
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);//初始化对应引脚IO
}

/*--------------------------------*电机驱动--------------------------------*/
//占空比小 速度大
void Motor1_Forward(void)//电机1正传
{
	M1_ENB(0);
}

void Motor1_Reverse(void)//电机1反转
{
	M1_ENB(1);
}

void Motor2_Forward(void)//电机2正传
{
	M2_ENB(0);
}

void Motor2_Reverse(void)//电机2反转
{
	M2_ENB(1);
}

void Motor3_Forward(void)//电机3正传
{
	M3_ENB(0);
}

void Motor3_Reverse(void)//电机3反转
{
	M3_ENB(1);
}

void Motor4_Forward(void)//电机4正传
{
	M4_ENB(0);
}

void Motor4_Reverse(void)//电机4反转
{
	M4_ENB(1);
}
