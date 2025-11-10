#include "pid.h"

PID_TypeDef PID_L1;
PID_TypeDef PID_R1;
PID_TypeDef PID_L2;
PID_TypeDef PID_R2;

void IncPID_L1_Init(void)//左侧电机1 PID参数初始化
{
	PID_L1.Proportion = 2.8; //比例常数 Proportional Const 

	PID_L1.Integral = 0.9; //积分常数Integral Const 

	PID_L1.Derivative = 0.4; //微分常数 Derivative Const	
}

void IncPID_R1_Init(void)//右侧电机1 PID参数初始化
{
	PID_R1.Proportion = 2.8; //比例常数 Proportional Const 

	PID_R1.Integral = 0.9; //积分常数Integral Const 

	PID_R1.Derivative = 0.4; //微分常数 Derivative Const	
}

void IncPID_L2_Init(void)//左侧电机2 PID参数初始化
{
	PID_L2.Proportion = 2.8; //比例常数 Proportional Const 

	PID_L2.Integral = 0.9; //积分常数Integral Const 

	PID_L2.Derivative = 0.4; //微分常数 Derivative Const	
}

void IncPID_R2_Init(void)//右侧电机2 PID参数初始化
{
	PID_R2.Proportion = 2.8; //比例常数 Proportional Const 

	PID_R2.Integral = 0.9; //积分常数Integral Const 

	PID_R2.Derivative = 0.4; //微分常数 Derivative Const	
}


/**************************************************************************
函数功能：增量PID控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
**************************************************************************/
float Incremental_PID(PID_TypeDef *pid, int16_t NextPoint, int16_t SetPoint)//PID增量式计算函数
{
	float iIncPid;//增量

  pid->Error = SetPoint - NextPoint;
    
  iIncPid = pid->Proportion * (pid->Error - pid->PrevError) + pid->Integral * pid->Error+\
  pid->Derivative * (pid->Error-2 * pid->PrevError + pid->LastError);//增量计算
    
  pid->LastError = pid->PrevError;//下一次迭代
  pid->PrevError = pid->Error;
  return iIncPid; 
}

/**************************************************************************
函数功能：位置式PID控制器
入口参数：编码器测量位置信息，目标位置
返回  值：电机PWM
根据位置式离散PID公式 
pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  
∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
pwm代表输出
**************************************************************************/

float Position_PID(PID_TypeDef *pid, int16_t NextPoint, int16_t SetPoint)//PD位置式计算函数
{	
	static float IntegralError=0;
	float PositionPid;
		
  pid->Error = SetPoint - NextPoint;
	
  IntegralError +=  pid->Error;
	if(IntegralError > 10.0f) IntegralError = 10.0f;//积分限幅，防止累积值一直增大
	else if(IntegralError < -10.0f) IntegralError = -10.0f;
	
  PositionPid = pid->Proportion * pid->Error + pid->Integral * IntegralError + pid->Derivative * (pid->Error - pid->PrevError);
  
  pid->PrevError = pid->Error;
  return PositionPid; 
}
