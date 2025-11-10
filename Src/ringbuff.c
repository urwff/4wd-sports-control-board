/**************************************************************************
文件功能：
1、往环形缓冲区写入一组数据函数
2、从环形缓冲区读取一个数据函数
**************************************************************************/
#include "ringbuff.h"

RingBuff_t ringBuff;//创建一个ringBuff的环形缓冲区

void RingBuff_Init(void)//初始化环形缓冲区
{
	ringBuff.Head = 0;
	ringBuff.Tail = 0;
	ringBuff.Lenght =0;
}

__IO uint16_t RingBuff_Count1=0;
__IO uint16_t RingBuff_LastCount1=0;

__IO uint16_t RingBuff_Count2=0;
__IO uint16_t RingBuff_LastCount2=0;

__IO uint8_t CoundState=1;

/*-----------------------------------------------------------
函数功能: 往环形缓冲区写入一组数据函数
输入参数: 待写入数据的数组首地址，数据长度
返 回 值: 0:环形缓冲区已满，写入失败
					1:写入成功
说    明: 往环形缓冲区写入数据环形缓冲缓存区会自动处理数据的排序
先入先出FIFO
 -----------------------------------------------------------*/
uint8_t Write_RingBuff(uint8_t *Buff, uint16_t Leng)
{
	uint16_t i=0;
	for(; i < Leng; i++)
	{
		if(ringBuff.Lenght >= RINGBUFF_LEN)//判断缓冲区是否已满
		{
			return 0;
		}
		ringBuff.Ring_Buff[ringBuff.Tail]=*(Buff+i);
		ringBuff.Tail = (ringBuff.Tail+1)%RINGBUFF_LEN;//防止越界非法访问

		if(CoundState == 1) RingBuff_Count1++;
		else if(CoundState == 2) RingBuff_Count2++;
	}
	if(i == Leng) return 1;
}


/*-----------------------------------------------------------
函数功能: 从环形缓冲区读取一个数据函数
输入参数: 用于保存读取的数据
返 回 值: 0:环形缓冲区没有数据，读取失败
					1:读取成功
说    明: 从环形缓冲区读取数据环形缓冲缓存区会自动处理数据的排序
先入先出FIFO
 -----------------------------------------------------------*/
uint8_t Read_RingBuff(uint8_t *Data)
{
	if(!ringBuff.Lenght)//判断空
	{
		return 0;
	}
  *Data = ringBuff.Ring_Buff[ringBuff.Head];//先进先出FIFO，从缓冲区头出
  ringBuff.Head = (ringBuff.Head+1)%RINGBUFF_LEN;//防止越界非法访问

	ringBuff.Lenght--;
  return 1;
}

/*-----------------------------------------------------------
函数功能: 环形缓存区互斥访问保护函数
输入参数: None
返 回 值: None
说    明: 
解决ringBuff.Lenght变量在中断和主函数处理过程中遇到的互斥访问问题
该函数需要快速轮询
 -----------------------------------------------------------*/
void RingBuff_Mutex(void)
{
	if(CoundState == 2)
	{
		if(RingBuff_Count1 > RingBuff_LastCount1)
		{
			ringBuff.Lenght += RingBuff_Count1 - RingBuff_LastCount1;
			RingBuff_LastCount1 = RingBuff_Count1;
		}
		else if(RingBuff_Count1 < RingBuff_LastCount1)
		{
			ringBuff.Lenght += (65536-RingBuff_LastCount1)+RingBuff_Count1;
			RingBuff_LastCount1 = RingBuff_Count1;
		}
		else CoundState=1;
	}
	else
	{
		if(RingBuff_Count2 > RingBuff_LastCount2)
		{
			ringBuff.Lenght += RingBuff_Count2 - RingBuff_LastCount2;
			RingBuff_LastCount2 = RingBuff_Count2;
		}
		else if(RingBuff_Count2 < RingBuff_LastCount2)
		{
			ringBuff.Lenght += (65536-RingBuff_LastCount2)+RingBuff_Count2;
			RingBuff_LastCount2 = RingBuff_Count2;
		}
		else CoundState = 2;
	}
}
