#include "uart.h"
#include <stdarg.h>
#include "SBUS.h"
#include "ringbuff.h"

UART_HandleTypeDef huart1;
//__IO uint8_t Usart1_TX_Buffer[Usart1TransmitLEN];
__IO uint8_t Usart1_RX_Buffer[Usart1ReceiveLEN];
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;


UART_HandleTypeDef huart2;
__IO uint8_t Usart2_TX_Buffer[Usart2TransmitLEN];
__IO uint8_t Usart2_RX_Buffer[Usart2ReceiveLEN];
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;


UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;
__IO uint8_t Usart3_RX_Buffer[Usart3ReceiveLEN];
__IO uint8_t Usart3_TX_Buffer[Usart3TransmitLEN];
__IO uint8_t Uart3TxReady=1;
uint8_t aRxBuffer3;//串口接收缓存
uint8_t aTxBuffer3;//串口发送缓存

UART_HandleTypeDef huart4;
__IO uint8_t Uart4_TX_Buffer[Uart4TransmitLEN];
__IO uint8_t Uart4_RX_Buffer[Uart4ReceiveLEN];
DMA_HandleTypeDef hdma_uart4_rx;
DMA_HandleTypeDef hdma_uart4_tx;

UART_HandleTypeDef huart5;
__IO uint8_t aRxBuffer5;//串口接收缓存
__IO uint8_t aTxBuffer5;//串口发送缓存

/*
黄3.3V
绿GND
蓝Tx
紫Rx
白5V
*/

void MX_USART1_Init(void)//串口1初始化
{
  huart1.Instance = USART1;//串口1
  huart1.Init.BaudRate = 9600;//波特率
  huart1.Init.WordLength = UART_WORDLENGTH_9B;//8位数据位,1位校验位
  huart1.Init.StopBits = UART_STOPBITS_1;//1位停止位
  huart1.Init.Parity = UART_PARITY_EVEN;//偶校验
  huart1.Init.Mode = UART_MODE_TX_RX;//全双工串口模式
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);
		
	HAL_UART_Receive_DMA(&huart1, Usart1_RX_Buffer, Usart1ReceiveLEN);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);//开启空闲接收中断
}

uint8_t rx_data;
void MX_USART2_Init(void)//串口2初始化
{
  huart2.Instance = USART2;//串口2
  huart2.Init.BaudRate = 115200;//波特率
  huart2.Init.WordLength = UART_WORDLENGTH_8B;//8位数据位
  huart2.Init.StopBits = UART_STOPBITS_1;//1位停止位
  huart2.Init.Parity = UART_PARITY_NONE;//无校验
  huart2.Init.Mode = UART_MODE_TX_RX;//全双工串口模式
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart2);
		
//	HAL_UART_Receive_IT(&huart2,&rx_data,1);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);//开启空闲接收中断
}

void MX_USART3_Init(void)//串口3初始化
{
  huart3.Instance = USART3;//串口3
  huart3.Init.BaudRate = 115200;//波特率
  huart3.Init.WordLength = UART_WORDLENGTH_8B;//8位数据位
  huart3.Init.StopBits = UART_STOPBITS_1;//1位停止位
  huart3.Init.Parity = UART_PARITY_NONE;//无校验
  huart3.Init.Mode = UART_MODE_TX_RX;//全双工串口模式
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart3);
	
	HAL_UART_Receive_DMA(&huart3, Usart3_RX_Buffer, Usart3ReceiveLEN);
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);//开启空闲接收中断
}

void MX_UART4_Init(void)//串口4初始化
{
  huart4.Instance = UART4;//串口4
  huart4.Init.BaudRate = 9600;//波特率
  huart4.Init.WordLength = UART_WORDLENGTH_9B;//8位数据位，1位校验位
  huart4.Init.StopBits = UART_STOPBITS_1;//1位停止位
  huart4.Init.Parity = UART_PARITY_EVEN;//偶校验
  huart4.Init.Mode = UART_MODE_TX_RX;//全双工串口模式
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart4);

	HAL_UART_Receive_DMA(&huart4, Uart4_RX_Buffer, Uart4ReceiveLEN);
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);//开启空闲接收中断
}

void MX_UART5_Init(void)//串口5初始化
{
  huart5.Instance = UART5;//串口5
  huart5.Init.BaudRate = 115200;//波特率
  huart5.Init.WordLength = UART_WORDLENGTH_8B;//8位数据位
  huart5.Init.StopBits = UART_STOPBITS_1;//1位停止位
  huart5.Init.Parity = UART_PARITY_NONE;//无校验
  huart5.Init.Mode = UART_MODE_TX_RX;//全双工串口模式
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart5);

	HAL_UART_Receive_IT(&huart5,&rx_data,1);//监听中断接收数据
}

extern uint8_t Move_Fx;
void Read_JetsonNano(uint8_t Buff, uint16_t Leng)
{
		if(Buff== 1)
		{
			Move_Fx = 1;
		}else if(Buff== 0)
		{
			Move_Fx = 0;
		}
}


void UsartReceive_IDLE(UART_HandleTypeDef *huart)//串口DMA接收中断函数
{
	uint32_t temp = 0;
	
	if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
	{	
		/*------------Usart1-------------*/
		if(huart->Instance == USART1)
		{
				__HAL_UART_CLEAR_IDLEFLAG(huart);
				temp = huart->Instance->SR;
				temp = huart->Instance->DR;
				temp = hdma_usart1_rx.Instance->CNDTR; 
				HAL_UART_DMAStop(huart);
			
		//		HAL_UART_Transmit_DMA(huart, Usart1_RX_Buffer, Usart1ReceiveLEN-temp);
//				SBUS(Usart1_RX_Buffer);
//			Read_JetsonNano(Usart1_RX_Buffer,Usart1ReceiveLEN);
				
				HAL_UART_Receive_DMA(huart, Usart1_RX_Buffer, Usart1ReceiveLEN);
		}
		
		/*------------Usart2-------------*/
		if(huart->Instance == USART2)
		{
				__HAL_UART_CLEAR_IDLEFLAG(huart);
//				temp = huart->Instance->SR;
//				temp = huart->Instance->DR;
//				temp = hdma_usart2_rx.Instance->CNDTR; 
//				HAL_UART_DMAStop(huart);
		//		Tx2_Flag = 0;	
				
				//Read_JetsonNano(rx_data,1);
//				Write_RingBuff(Usart2_RX_Buffer, Usart2ReceiveLEN-temp);//往环形缓冲区写入一组数据函数
				HAL_UART_Receive_IT(&huart2,&rx_data,1);
		}
		
		/*------------Usart3-------------*/
		if(huart->Instance == USART3)
		{
			__HAL_UART_CLEAR_IDLEFLAG(huart);
			temp = huart->Instance->SR;
			temp = huart->Instance->DR;
			temp = hdma_usart3_rx.Instance->CNDTR; 
			HAL_UART_DMAStop(huart);

			
			HAL_UART_Receive_DMA(huart, Usart3_RX_Buffer, Usart3ReceiveLEN);
		}
		
		/*------------Uart4-------------*/
		if(huart->Instance == UART4)
		{
			__HAL_UART_CLEAR_IDLEFLAG(huart);
			temp = huart->Instance->SR;
			temp = huart->Instance->DR;
			temp = hdma_uart4_rx.Instance->CNDTR; 
			HAL_UART_DMAStop(huart);
			
	//		Tx1_Flag = 0;
			
//			AoaModuleDecode(Uart4_RX_Buffer, Uart4ReceiveLEN-temp);//AOA模块解码函数
			HAL_UART_Receive_DMA(huart, Uart4_RX_Buffer, Uart4ReceiveLEN);
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)//串口接收中断
{	
	if(huart->Instance==UART5)
	{					
		HAL_UART_Receive_IT(&huart5,&rx_data,1);//该函数必须在最上
		Read_JetsonNano(rx_data,1); 
		/*-----------------------串口接收到数据后通过串口发送接收到的数据-----------------------*/
//		aTxBuffer5 = aRxBuffer5;//串口接收到数据赋值给串口发送数据的缓存
//		HAL_UART_Transmit(&huart5, &aTxBuffer5, 1, 0xFFFF);//串口堵塞方式发送数据
		
	}
}

//aTxBuffer3=0x54;//中断发送方式
//	Uart3TxReady=0; HAL_UART_Transmit_IT(&huart3, &aTxBuffer3, 1);while(!Uart3TxReady);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)//串口发送中断
{
	if(huart->Instance==USART3)
	{
		Uart3TxReady = 1;
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	/*如文档说明，要清除ORE中断需要按顺序读取USART_SR和USART_DR寄存器！*/
  uint32_t isrflags   = READ_REG(huart->Instance->SR);//手册上有讲，清错误都要先读SR
	if((__HAL_UART_GET_FLAG(huart, UART_FLAG_PE))!=RESET)
	{
		READ_REG(huart->Instance->DR);//第二步读DR
		__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_PE);//PE清标志	
	}
	if((__HAL_UART_GET_FLAG(huart, UART_FLAG_FE))!=RESET)
	{
		READ_REG(huart->Instance->DR);//第二步读DR
		__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_FE);//FE清标志
	}
        
	if((__HAL_UART_GET_FLAG(huart, UART_FLAG_NE))!=RESET)
	{
		READ_REG(huart->Instance->DR);//第二步读DR
		__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_NE);//NE清标志
  }        
        
  if((__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE))!=RESET)
  {
    READ_REG(huart->Instance->CR1);//第二步读CR
		__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_ORE);//ORE清标志
  }      
}


#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
	Uart3TxReady=0; HAL_UART_Transmit_IT(&huart3, (uint8_t *)&ch, 1);while(!Uart3TxReady);
  return ch;
}


void dma_printf(const char *format, ...)
{
    uint32_t length;
    va_list args;
    va_start(args, format);
    length = vsnprintf((char *)Usart3_TX_Buffer, sizeof(Usart3_TX_Buffer), (char *)format, args);
    va_end(args);

    HAL_UART_Transmit_DMA(&huart3, (uint8_t *)Usart3_TX_Buffer, length);
}
