/**************************************************************************
文件功能：
1、数据接收接口接收电机转速指令
2、数据发送接口发送小车左前、右前、左后、右后电机编码器计数值、电池电量、电压值数据
**************************************************************************/
#include "DataCom.h"
#include "uart.h"

DataCom_RX_Struct DataCom_RX1_Struct;
DataCom_TX_Struct DataCom_TX1_Struct;

/*-----------------------------------------------------------
函数功能: 数据接收接口
输入参数: 数据数组地址
返 回 值: None
说    明: 将解析出的数据帧传入该函数实现电机转速值更新
 -----------------------------------------------------------*/
void DataCom_RX(uint8_t *Buff)
{
	DataCom_RX1_Struct.Velocity_L1_Motor = Buff[3]*256 + Buff[4];//设置左前电机速度
	DataCom_RX1_Struct.Velocity_R1_Motor = Buff[5]*256 + Buff[6];//设置右前电机速度
	DataCom_RX1_Struct.Velocity_L2_Motor = Buff[7]*256 + Buff[8];//设置左后电机速度
	DataCom_RX1_Struct.Velocity_R2_Motor = Buff[9]*256 + Buff[10];//设置右后电机速度
}

/*-----------------------------------------------------------
函数功能: 数据发送接口
输入参数: 发送数据结构体
返 回 值: None
说    明: 将待发送的数据结构体传入该函数实现数据上传
 -----------------------------------------------------------*/
void DataCom_TX(DataCom_TX_Struct DataCom_TX1_Struct)
{
	Usart2_TX_Buffer[0] = 0xFE;//帧头1
	Usart2_TX_Buffer[1] = 0xEF;//帧头2
	Usart2_TX_Buffer[2] =19;  //数据长度
	
	Usart2_TX_Buffer[3] = DataCom_TX1_Struct.Encoder_L1_Count >> 24;
	Usart2_TX_Buffer[4] = (DataCom_TX1_Struct.Encoder_L1_Count >> 16) & 0xFF;
	Usart2_TX_Buffer[5] = (DataCom_TX1_Struct.Encoder_L1_Count >> 8) & 0xFF;
	Usart2_TX_Buffer[6] = DataCom_TX1_Struct.Encoder_L1_Count & 0xFF;
	
	Usart2_TX_Buffer[7] = DataCom_TX1_Struct.Encoder_R1_Count >> 24;
	Usart2_TX_Buffer[8] = (DataCom_TX1_Struct.Encoder_R1_Count >> 16) & 0xFF;
	Usart2_TX_Buffer[9] = (DataCom_TX1_Struct.Encoder_R1_Count >> 8) & 0xFF;
	Usart2_TX_Buffer[10]= DataCom_TX1_Struct.Encoder_R1_Count & 0xFF;
	
	Usart2_TX_Buffer[11] = DataCom_TX1_Struct.Encoder_L2_Count >> 24;
	Usart2_TX_Buffer[12] = (DataCom_TX1_Struct.Encoder_L2_Count >> 16) & 0xFF;
	Usart2_TX_Buffer[13] = (DataCom_TX1_Struct.Encoder_L2_Count >> 8) & 0xFF;
	Usart2_TX_Buffer[14] = DataCom_TX1_Struct.Encoder_L2_Count & 0xFF;
	
	Usart2_TX_Buffer[15] = DataCom_TX1_Struct.Encoder_R2_Count >> 24;
	Usart2_TX_Buffer[16] = (DataCom_TX1_Struct.Encoder_R2_Count >> 16) & 0xFF;
	Usart2_TX_Buffer[17] = (DataCom_TX1_Struct.Encoder_R2_Count >> 8) & 0xFF;
	Usart2_TX_Buffer[18] = DataCom_TX1_Struct.Encoder_R2_Count & 0xFF;
	
	Usart2_TX_Buffer[19]= DataCom_TX1_Struct.BatteryCapacity;//电量
	
	Usart2_TX_Buffer[20]= (uint16_t)DataCom_TX1_Struct.SupplyVoltage / 256;//电压高八位
	Usart2_TX_Buffer[21]= (uint16_t)DataCom_TX1_Struct.SupplyVoltage % 256;//电压低八位
	
	uint8_t SumCheck=0;//校验和
	uint8_t i = Usart2_TX_Buffer[2] + 3;
	for(;(i--)>0;)
	{
		SumCheck += Usart2_TX_Buffer[i];//计算校验和
	}
	
	Usart2_TX_Buffer[22]= SumCheck;//校验和
	
	HAL_UART_Transmit_DMA(&huart2, (uint8_t *)Usart2_TX_Buffer, 23);//串口DMA发送指令
}
