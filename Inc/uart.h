#ifndef __UART_H
#define __UART_H
#include "main.h"

#define Usart1ReceiveLEN 15
#define Usart1TransmitLEN 30
extern UART_HandleTypeDef huart1;
extern __IO uint8_t Usart1_RX_Buffer[Usart1ReceiveLEN];
//extern __IO uint8_t Usart1_TX_Buffer[Usart1TransmitLEN];
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
void MX_USART1_Init(void);//串口1初始化

#define Usart2ReceiveLEN 15
#define Usart2TransmitLEN 60
extern UART_HandleTypeDef huart2;
extern __IO uint8_t Usart2_RX_Buffer[Usart2ReceiveLEN];
extern __IO uint8_t Usart2_TX_Buffer[Usart2TransmitLEN];
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
void MX_USART2_Init(void);//串口2初始化

#define Usart3ReceiveLEN 30
#define Usart3TransmitLEN 30
extern UART_HandleTypeDef huart3;
extern __IO uint8_t Usart3_RX_Buffer[Usart3ReceiveLEN];
extern __IO uint8_t Usart3_TX_Buffer[Usart3TransmitLEN];
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern __IO uint8_t Uart3TxReady;
extern uint8_t aRxBuffer3;//串口接收缓存
extern uint8_t aTxBuffer3;//串口发送缓存
void MX_USART3_Init(void);//串口3初始化

#define Uart4ReceiveLEN 30
#define Uart4TransmitLEN 30
extern UART_HandleTypeDef huart4;
extern __IO uint8_t Uart4_TX_Buffer[Uart4TransmitLEN];
extern __IO uint8_t Uart4_RX_Buffer[Uart4ReceiveLEN];
extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_uart4_tx;
void MX_UART4_Init(void);//串口4初始化


extern UART_HandleTypeDef huart5;
extern __IO uint8_t aRxBuffer5;//串口接收缓存
extern __IO uint8_t aTxBuffer5;//串口发送缓存
void MX_UART5_Init(void);//串口5初始化
	
void UsartReceive_IDLE(UART_HandleTypeDef *huart);//串口DMA接收中断函数

void dma_printf(const char *format, ...);

#endif
