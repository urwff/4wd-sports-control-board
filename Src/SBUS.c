#include "SBUS.h"

SBUS_CH_Struct SBUS_CH;
/*
[startbyte] [data1][data2]…[data22][flags][endbyte]
startbyte=0x0f;
endbyte=0x00;
flags标志位是用来检测控制器与遥控器是否断开的标志位。
flags=1：控制器与接收器保持连接
flags=0：控制器与接收器断开
*/
void SBUS(uint8_t *DataArray)
{
	static uint8_t DisconnectCount=10;
	if(DataArray[0]==0x0F && DataArray[24]==0x00)//判断帧头与帧尾是否正确
	{
		SBUS_CH.CH1=0;
		SBUS_CH.CH1 = DataArray[1];
		SBUS_CH.CH1 |= (DataArray[2]  & 0x07)<<8;
		
		SBUS_CH.CH2=0;
		SBUS_CH.CH2 = DataArray[2]>>3;
		SBUS_CH.CH2 |= (DataArray[3] & 0x3F)<<5;
		
		SBUS_CH.CH3=0;
		SBUS_CH.CH3 = DataArray[3]>>6;
		SBUS_CH.CH3 |= DataArray[4]<<2;
		SBUS_CH.CH3 |= (DataArray[5] & 0x01)<<10;
		
		SBUS_CH.CH4=0;
		SBUS_CH.CH4 = DataArray[5]>>1;
		SBUS_CH.CH4 |= (DataArray[6]  & 0x0F)<<7;
		
		SBUS_CH.CH5=0;
		SBUS_CH.CH5 = DataArray[6]>>4;
		SBUS_CH.CH5 |= (DataArray[7]  & 0x7F)<<4;
		
		SBUS_CH.CH6=0;
		SBUS_CH.CH6 = DataArray[7]>>7;
		SBUS_CH.CH6 |= DataArray[8]<<1;
		SBUS_CH.CH6 |= (DataArray[9] & 0x03)<<9;
		
		if(SBUS_CH.CH1==1001 && SBUS_CH.CH2==1001 && SBUS_CH.CH3==41 && SBUS_CH.CH4==1001) 
		{
			if(DisconnectCount < 10) DisconnectCount++;
			else SBUS_CH.ConnectState=0;
		}
		else if(DisconnectCount) DisconnectCount--;	
		else SBUS_CH.ConnectState=1;
	}
}
