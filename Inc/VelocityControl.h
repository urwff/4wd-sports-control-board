#ifndef __VELOCITYCONTROL_H
#define __VELOCITYCONTROL_H
#include "main.h"

extern __IO int16_t Velocity_L1_Motor;	//左侧电机速度
extern __IO int16_t Velocity_R1_Motor;//右侧电机速度
extern __IO int16_t Velocity_L2_Motor;	//左侧电机速度
extern __IO int16_t Velocity_R2_Motor;//右侧电机速度

void Kinematic_Analysis(int16_t Velocity,float AngularVelocity);//小车运动数学模型函数
void VelocityControl(void);//速度闭环控制函数

#endif
