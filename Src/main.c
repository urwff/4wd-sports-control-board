#include "main.h"
#include "tim8.h"
#include "ShareWare.h"
#include "delayus.h"
#include "MotorControl.h"

#include <stdarg.h>
#include "adc1.h"
#include "tim2.h"
#include "tim3.h"
#include "tim4.h"
#include "tim5.h"
#include "uart.h"
#include "pid.h"
#include "SBUS.h"
#include "VelocityControl.h"
#include "ringbuff.h"
#include "DataProtocol.h"
#include "IndicatorDevice.h"
#include "Encoder.h"
#include "DataCom.h"
#include "LowPassFilter.h"
#include "SupplyVoltage.h"
#include "MC6C.h"
#include "math.h"

/*	
左前
右前
左后
右后
*/
__IO uint16_t Loop_10msTime=0;
__IO uint16_t Loop_500msTime=0;
__IO uint16_t HeartbeatTime=0;//心跳包时间
void SystemClock_Config(void);       

void RS485_RD_GPIO_Init(void)//485控制IO初始化
{
	   /* 定义IO硬件初始化结构体变量 */
  GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOC_CLK_ENABLE();//使能(开启)引脚对应IO端口时钟
	
	/*电机驱动芯片对应GPIO引脚配置*/
	GPIO_InitStruct.Pin = GPIO_PIN_14;//设定对应引脚IO编号 
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//设定对应引脚IO为输出模式
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW ;//设对应引脚IO操作速度
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);//设定对应引脚电平状态
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);//初始化对应引脚IO
}

uint32_t batter_tick;
extern uint32_t uwTick;

_Bool ON_Flag;
uint8_t ON_Num;


void Get_BatteryCar(void)
{
	if(uwTick - batter_tick < 500)
		return ;
	batter_tick = uwTick;
	
	ReadSupplyVoltage();//读取电源电压值
	BatteryCapacity = ReadBatteryCapacity(SupplyVoltage);//读取电池剩余电量百分比
	ON_Num++;
	
	if(ON_Num == 18){
		ON_Flag = 1;
	}
	
	if(ON_Num % 12 == 0 && ON_Flag == 1)
	{
		HAL_UART_Transmit(&huart5,(uint8_t *)&BatteryCapacity,1,50);
	}
}


uint32_t move_tick = 0;
uint8_t Move_Fx;
_Bool rotate_flag;
#define ROTATE_90_DEGREE_MS  1300  // 理论计算值（需替换为实测值）
void Move_proc(void) {

    if (uwTick - move_tick < 10) return; // 10ms周期控制
    move_tick = uwTick;

	VelocityControl(); // 速度闭环（如PID）
	
    switch (Move_Fx) {
        case 0:
            Velocity_L1_Motor = 0;
            Velocity_R1_Motor = 0;
            Velocity_L2_Motor = 0;
            Velocity_R2_Motor = 0;
            break;

        case 1:
            Velocity_L1_Motor = 50;
            Velocity_R1_Motor = 50;
            Velocity_L2_Motor = 50;
            Velocity_R2_Motor = 50;
            break;

        case 2:
            Velocity_L1_Motor = -50;
            Velocity_R1_Motor = -50;
            Velocity_L2_Motor = -50;
            Velocity_R2_Motor = -50;
            break;

       case 3:  // 左转（逆时针）
            Velocity_L1_Motor = -50; // 左前轮反转
            Velocity_R1_Motor = 50;  // 右前轮正转
            Velocity_L2_Motor = -50; // 左后轮反转
            Velocity_R2_Motor = 50;   // 右后轮正转
			rotate_flag = 1;
            break;

        case 4: // 右转（顺时针）
            Velocity_L1_Motor = 50;   // 左前轮正转
            Velocity_R1_Motor = -50; // 右前轮反转
            Velocity_L2_Motor = 50;   // 左后轮正转
            Velocity_R2_Motor = -50;  // 右后轮反转
			rotate_flag = 1;
            break;
    }
}

uint32_t rotate_tick;
uint32_t temp_tick;
void rotate_car(void)
{
	if(uwTick - temp_tick < 20)
		return ;
	temp_tick  = uwTick;
	
	if(rotate_tick  >= ROTATE_90_DEGREE_MS)
	{
		rotate_flag = 0;
		Move_Fx = 0;
	}
}



void Temp(void)
{
// 左转90°
Move_Fx = 3;
HAL_Delay(ROTATE_90_DEGREE_MS);
Move_Fx = 0;           // 持续旋转2秒（约旋转4.3圈）
}

int main(void)
{
  HAL_Init();//初始化HAL库
  SystemClock_Config();//系统时钟配置
	MX_TIM8_Init();//TIM8-PWM初始化
	MotorControl_GPIO_Init();//电机控制IO初始化
	
	MX_ADC1_Init();//ADC1初始化
	MX_TIM2_Init();//TIM2-编码器模式初始化
	MX_TIM3_Init();//TIM3-编码器模式初始化
	MX_TIM4_Init();//TIM4-编码器模式初始化
	MX_TIM5_Init();//TIM5-编码器模式初始化
	MX_USART1_Init();//串口1初始化
	MX_USART2_Init();//串口2初始化	
	MX_USART3_Init();//串口3初始化	
	MX_UART4_Init();//串口4初始化
	MX_UART5_Init();//串口5初始化
	IncPID_L1_Init();//左侧电机1 PID参数初始化
	IncPID_R1_Init();//右侧电机1 PID参数初始化
	IncPID_L2_Init();//左侧电机2 PID参数初始化
	IncPID_R2_Init();//右侧电机2 PID参数初始化	
	
	RS485_RD_GPIO_Init();
	
	/*-------------------测试电机转动---------------*/
//	Motor1_Reverse();//电机1正传
//	Motor2_Forward();//电机1正传
//	Motor3_Forward();//电机1正传
//	Motor4_Forward();//电机1正传
//	TIM8 ->CCR1 = 100;//数值越小 电机转速越快
//	TIM8 ->CCR2 = 100;//数值越小 电机转速越快
//	TIM8 ->CCR3 = 100;//数值越小 电机转速越快
//	TIM8 ->CCR4 = 100;//数值越小 电机转速越快
//  while(1){}

//	RingBuff_Init();//初始化环形缓冲区
//	
//	Buzzer_GPIO_Init();//蜂鸣器IO口初始化
//	LED_Green_GPIO_Init();//绿色LED IO口初始化
//	LED_Red_GPIO_Init();//红色LED IO口初始化
	Encoder_Struct_Init();//初始化编码器结构体参数
	LPF_Struct_Init();//低通滤波结构体参数初始化
//	
//	HAL_Delay(10);
//	SBUS_CH.ConnectState = 0;//上电默认将SUBS断开连接标志位置0，防止小车上电就跑
//	
//	BuzzerControl(10, 50);//蜂鸣器控制函数
//	LED_Green_Control(2,50);//绿色LED灯控制函数
//	LED_Red_Control(2,50);//红色LED灯控制函数
	
	while (1)
	{	
		Get_BatteryCar();
		Move_proc();
		rotate_car();
  }
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}



/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
// _Error_Handler(__FILE__, __LINE__);
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
