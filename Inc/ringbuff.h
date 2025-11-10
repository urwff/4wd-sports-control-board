#ifndef __RINGBUFF_H__
#define __RINGBUFF_H__
#include "main.h"

/*使用唤环形缓冲区形式接收数据*/
#define USER_RINGBUFF 1

#if USER_RINGBUFF
	/*缓冲区大小*/
	#define RINGBUFF_LEN 300
	void RingBuff_Init(void);//初始化环形缓冲区
	uint8_t Write_RingBuff(uint8_t *Buff, uint16_t Leng);//往环形缓冲区写入一组数据函数
	uint8_t Read_RingBuff(uint8_t *Data);//从环形缓冲区读取一个数据函数
	void RingBuff_Mutex(void);//环形缓存区互斥保护函数
#endif

typedef struct
{
	uint16_t Head;//头部
	uint16_t Tail;//尾部
	uint16_t Lenght;//当前数据长度
	uint8_t  Ring_Buff[RINGBUFF_LEN];
}RingBuff_t;



#endif
