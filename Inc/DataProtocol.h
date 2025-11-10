#ifndef __DATAPROTOCOL_H
#define __DATAPROTOCOL_H
#include "main.h"
#define Data_Length_Max 200
extern __IO uint8_t ParseDataState;////数据解析状态，1=解析出一帧数据，0=未解析出有效数据帧
extern uint8_t Data_Array[Data_Length_Max];//储存数据的数组
void ParseData(uint8_t Data);//解析数据函数
void ParseData_RingBuff(void);//解析环形缓存区的数据
#endif
