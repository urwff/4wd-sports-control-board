#ifndef __PID_H
#define __PID_H
#include "main.h"

typedef struct//PID参数结构体
{
	float Proportion; //比例常数 Proportional Const

	float Integral; //积分常数 Integral Const

	float Derivative; //微分常数 Derivative Const
	
	int Error;//E(k)
	
	int PrevError;//E(k-1)
	
	int LastError;//E(k-2)
} PID_TypeDef;

extern PID_TypeDef PID_L1;
extern PID_TypeDef PID_R1;
extern PID_TypeDef PID_L2;
extern PID_TypeDef PID_R2;

void IncPID_L1_Init(void);//左侧电机1 PID参数初始化
void IncPID_R1_Init(void);//右侧电机1 PID参数初始化
void IncPID_L2_Init(void);//左侧电机2 PID参数初始化
void IncPID_R2_Init(void);//右侧电机2 PID参数初始化
float Incremental_PID(PID_TypeDef *pid, int16_t NextPoint, int16_t SetPoint);//PID增量式计算函数
float Position_PID(PID_TypeDef *pid, int16_t NextPoint, int16_t SetPoint);//PD位置式计算函数
#endif
