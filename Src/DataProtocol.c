/**************************************************************************
文件功能：
1、解析数据
2、解析环形缓存区的数据
**************************************************************************/
#include "DataProtocol.h"
#include "ringbuff.h"

uint8_t Data_Array[Data_Length_Max];//储存数据的数组
__IO uint8_t ParseDataState=0;//数据解析状态，1=解析出一帧数据，0=未解析出有效数据帧

/*-----------------------------------------------------------
函数功能: 解析数据函数
输入参数: 数据
返 回 值: None
说    明: 将收到的一串数据逐个传入该函数进行解析，若是解析出
一帧完整数据“ParseDataState”标志位会自动置1
 -----------------------------------------------------------*/
void ParseData(uint8_t Data)
{
	static uint8_t FE_flag=0;//收到0xFE数据置1，下一位数据是0xEF则为新的帧头，否则置0
	static uint8_t FH=0;//帧头标志，0=帧头不完整、1=帧头完整
	static uint8_t Data_Count=0;//数据量计数
	
	if(!ParseDataState)//一帧数据未接收完整，继续处于接收数据状态
	{
		Data_Array[Data_Count]=Data;//数据存入数组
		
		switch(Data)
		{
			case 0xFE: FE_flag=1; break;
			
			case 0xEF:
			{
				if(FE_flag)//收到完整帧头
				{
					FE_flag=0;//清除标志
					FH=1;//完整帧头
					Data_Array[0]=0xFE;//重新赋值
					Data_Array[1]=0xEF;//重新赋值
					Data_Array[2]=Data_Length_Max;//预先将数据最大长度值赋值进数组数据长度位
					Data_Count=1;//重新校准当前数据计数值
				}		
			}break;
			
			default: FE_flag=0; break;
		}
	}
		
	if( ( Data_Count>(Data_Array[2]+2) ) && FH )//完整收齐一帧数据
	{
		uint8_t SumCheck=0;//校验码
		uint8_t i = Data_Array[2] + 3;
		for(;(i--)>0;) SumCheck += Data_Array[i];//计算校验和

		if(SumCheck == Data_Array[Data_Array[2]+3])//检验校验值是否一致
		{
			ParseDataState=1;//标识一帧数据完整
			FE_flag=0;//清除标志
			FH=0;//清除标志位
		}
		else//校验码不一致
		{
			FE_flag=0;//清除标志
			FH=0;//清除标志位
		}
	}
	if(Data_Count<(Data_Length_Max-1))Data_Count++;//防止数组溢出
	else Data_Count=0;
}

/*-----------------------------------------------------------
函数功能: 解析环形缓存区的数据
输入参数: None
返 回 值: None
说    明: 从环形缓存区读取数据并传入到“ParseData()”解析数据函数
进行解析，若解析出一帧完整数据“ParseDataState”标志位会自动置1，
然后停止解析，等待用户手动将“ParseDataState”标志位清0后才能继续
往下解析数据。
 -----------------------------------------------------------*/
void ParseData_RingBuff(void)
{
	RingBuff_Mutex();//环形缓存区互斥访问保护函数
	if(!ParseDataState)//判断解析出的数据是否已处理
	{
		uint8_t Data;
		if(Read_RingBuff(&Data))//判断缓存区是否有新数据
		{
			ParseData(Data);//解析数据
		}
	}
}
