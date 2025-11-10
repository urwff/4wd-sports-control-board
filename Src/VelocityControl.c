/**************************************************************************
文件功能：
1、建立小车运动数学模型
2、小车左前、右前、左后、右后，4个电机速度闭环控制
**************************************************************************/
#include "VelocityControl.h"
#include "MotorControl.h"
#include "pid.h"
#include "tim2.h"
#include "tim3.h"
#include "tim4.h"
#include "tim5.h"
#include "Encoder.h"

/*-----以下为4个电机速度接口，通过对接口赋值可控制对应电机的转速-----*/
__IO int16_t Velocity_L1_Motor;	//左侧电机速度
__IO int16_t Velocity_R1_Motor;//右侧电机速度
__IO int16_t Velocity_L2_Motor;	//左侧电机速度
__IO int16_t Velocity_R2_Motor;//右侧电机速度



/**************************************************************************
函数功能：小车运动数学模型函数
入口参数：小车速度值，旋转角速度
返回  值：None
**************************************************************************/
#define a_ 0.1325f//小车左右轮间距的一半值 单位（米）
#define b_ 0.110f//小车前后轮间距的一半值 单位（米）
void Kinematic_Analysis(int16_t Velocity,float AngularVelocity)//小车运动数学模型函数
{
	Velocity_L1_Motor = Velocity - AngularVelocity * (a_+b_);//左侧电机速度
	Velocity_R1_Motor = Velocity + AngularVelocity * (a_+b_);//右侧电机速度
	Velocity_L2_Motor = Velocity - AngularVelocity * (a_+b_);//左侧电机速度
	Velocity_R2_Motor = Velocity + AngularVelocity * (a_+b_);//右侧电机速度
}


#define MotorParameter 1.515151f//【60000（1min=60000ms） / 10（10ms轮询周期）】 / 【11（电机转一圈脉冲数） * 4（STM32编码器模式4倍频） * 90（电机减速比）】
/**************************************************************************
函数功能：速度闭环控制函数
入口参数：None
返回  值：None
注    意：该函数需要周期性执行不能连续调用
**************************************************************************/
void VelocityControl(void)
{
	static float PWM_L1=0,PWM_R1=0,PWM_L2=0,PWM_R2=0;
	
	Encoder_UpdataValue();//编码器更新数值
	int16_t Encoder_L1_Motor = Encoder_TIM5_Struct.Capture_D_Value;//读取左前电机编码器前后2次捕获计数的差值
	int16_t Encoder_R1_Motor = Encoder_TIM3_Struct.Capture_D_Value;//读取右前电机编码器前后2次捕获计数的差值
	int16_t Encoder_L2_Motor = Encoder_TIM4_Struct.Capture_D_Value;//读取左后电机编码器前后2次捕获计数的差值
	int16_t Encoder_R2_Motor = Encoder_TIM2_Struct.Capture_D_Value;//读取右后电机编码器前后2次捕获计数的差值
	
	/*------------------------左侧电机1速度调控------------------------*/
	if(Velocity_L1_Motor == 0)//左侧电机1停转
	{
		PWM_L1 = MotorPWM_Max;
	}
	else if(Velocity_L1_Motor > 0)//左侧电机1正转
	{
		Motor1_Forward();//电机1正传
		PWM_L1 -= Incremental_PID(&PID_L1, Encoder_L1_Motor, (float)Velocity_L1_Motor / MotorParameter);  //通过增量式PID计算获取左侧电机PWM输出
	}
	else//左侧电机1反转
	{
		Motor1_Reverse();//电机1反转
		PWM_L1 += Incremental_PID(&PID_L1, Encoder_L1_Motor, (float)Velocity_L1_Motor / MotorParameter);  //通过增量式PID计算获取左侧电机PWM输出
	}
	
	/*------------------------右侧电机1速度调控------------------------*/
	if(Velocity_R1_Motor == 0)//右侧电机1停转
	{
		PWM_R1 = MotorPWM_Max;
	}
	else if(Velocity_R1_Motor > 0)//右侧电机1正转 
	{
		Motor2_Reverse();//电机2反转
		PWM_R1 -= Incremental_PID(&PID_R1, Encoder_R1_Motor, (float)Velocity_R1_Motor / MotorParameter);//通过增量式PID计算获取右侧电机PWM输出
	}
	else//右侧电机1反转 
	{
		Motor2_Forward();//电机2正传
		PWM_R1 += Incremental_PID(&PID_R1, Encoder_R1_Motor, (float)Velocity_R1_Motor / MotorParameter);//通过增量式PID计算获取右侧电机PWM输出1.515151
	}
	
	/*------------------------左侧电机2速度调控------------------------*/
	if(Velocity_L2_Motor == 0)//左侧电机2停转
	{
		PWM_L2 = MotorPWM_Max;
	}
	else if(Velocity_L2_Motor > 0)//左侧电机2正转 
	{
		Motor3_Forward();//电机3正传
		PWM_L2 -= Incremental_PID(&PID_L2, Encoder_L2_Motor, (float)Velocity_L2_Motor / MotorParameter);//通过增量式PID计算获取右侧电机PWM输出
	}
	else//左侧电机2反转 
	{
		Motor3_Reverse();//电机3反转
		PWM_L2 += Incremental_PID(&PID_L2, Encoder_L2_Motor, (float)Velocity_L2_Motor / MotorParameter);//通过增量式PID计算获取右侧电机PWM输出
	}
	
	/*------------------------右侧电机2速度调控------------------------*/
	if(Velocity_R2_Motor == 0)//右侧电机2停转
	{
		PWM_R2 = MotorPWM_Max;
	}
	else if(Velocity_R2_Motor > 0)//右侧电机2正转 
	{
		Motor4_Reverse();//电机4反转
		PWM_R2 -= Incremental_PID(&PID_R2, Encoder_R2_Motor, (float)Velocity_R2_Motor / MotorParameter);//通过增量式PID计算获取右侧电机PWM输出
	}
	else//右侧电机2反转 
	{
		Motor4_Forward();//电机4正传
		PWM_R2 += Incremental_PID(&PID_R2, Encoder_R2_Motor, (float)Velocity_R2_Motor / MotorParameter);//通过增量式PID计算获取右侧电机PWM输出
	}
	
	/*---------PWM限值--------------*/
	if(PWM_L1 > MotorPWM_Max)     	PWM_L1 = MotorPWM_Max;
	else if(PWM_L1 < MotorPWM_Min) 	PWM_L1 = MotorPWM_Min;
	
	if(PWM_R1 > MotorPWM_Max)      	PWM_R1 = MotorPWM_Max;
	else if(PWM_R1 < MotorPWM_Min) 	PWM_R1 = MotorPWM_Min;
	
	if(PWM_L2 > MotorPWM_Max)     	PWM_L2 = MotorPWM_Max;
	else if(PWM_L2 < MotorPWM_Min) 	PWM_L2 = MotorPWM_Min;
	
	if(PWM_R2 > MotorPWM_Max)      	PWM_R2 = MotorPWM_Max;
	else if(PWM_R2 < MotorPWM_Min) 	PWM_R2 = MotorPWM_Min;
	/*-----------------------------*/
	
	if(Velocity_L1_Motor==0 && Encoder_L1_Motor==0)//消除速度为0的时候残留的PWM
	{
		PWM_L1=MotorPWM_Max;
	}
	
	if(Velocity_R1_Motor==0 && Encoder_R1_Motor==0)//消除速度为0的时候残留的PWM
	{
		PWM_R1=MotorPWM_Max;
	}
	
	if(Velocity_L2_Motor==0 && Encoder_L2_Motor==0)//消除速度为0的时候残留的PWM
	{
		PWM_L2=MotorPWM_Max;
	}
	
	if(Velocity_R2_Motor==0 && Encoder_R2_Motor==0)//消除速度为0的时候残留的PWM
	{
		PWM_R2=MotorPWM_Max;
	}
	
	Motor1_PWM = (uint16_t)PWM_L1;//控制电机1 PWM输出
	Motor2_PWM = (uint16_t)PWM_R1;//控制电机2 PWM输出
	Motor3_PWM = (uint16_t)PWM_L2;//控制电机3 PWM输出
	Motor4_PWM = (uint16_t)PWM_R2;//控制电机4 PWM输出
}
