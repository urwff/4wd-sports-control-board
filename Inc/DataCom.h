#ifndef __DATACOM_H
#define __DATACOM_H
#include "main.h"
typedef struct
{	
	__IO int16_t Velocity_L1_Motor;
	__IO int16_t Velocity_R1_Motor;
	__IO int16_t Velocity_L2_Motor;
	__IO int16_t Velocity_R2_Motor;
}DataCom_RX_Struct;

typedef struct
{	
	__IO uint32_t Encoder_L1_Count;	//左侧编码器计数值（用于串口上传，持续计数不清0）
	__IO uint32_t Encoder_R1_Count;	//右侧编码器计数值（用于串口上传，持续计数不清0）
	__IO uint32_t Encoder_L2_Count;	//左侧编码器计数值（用于串口上传，持续计数不清0）
	__IO uint32_t Encoder_R2_Count;	//右侧编码器计数值（用于串口上传，持续计数不清0）
	__IO uint8_t BatteryCapacity;//电池电量，百分比0-100
	__IO uint16_t SupplyVoltage;//电源电压
}DataCom_TX_Struct;

extern DataCom_RX_Struct DataCom_RX1_Struct;
extern DataCom_TX_Struct DataCom_TX1_Struct;

void DataCom_RX(uint8_t *Buff);//接收数据接口
void DataCom_TX(DataCom_TX_Struct DataCom_TX1_Struct);//发送数据接口
	
#endif
