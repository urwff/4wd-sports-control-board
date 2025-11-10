/**************************************************************************
文件功能：
1、航模遥控器左右两侧摇杆值更新
2、航模遥控器控制小车运动
**************************************************************************/
#include "MC6C.h"
#include "SBUS.h"
#include "VelocityControl.h"

#define AnalogStick_Min 230
#define AnalogStick_Min_1 970	//死区值
															//  |
#define AnalogStick_Max_1 1030//死区值
#define AnalogStick_Max 1770

AnalogStick_Struct AnalogStick;

/*-----------------------------------------------------------
函数功能: 航模遥控器左右两侧摇杆值更新
输入参数: None
返 回 值: None
说    明: 调用该函数实现遥控器左右两侧摇杆数值更新
 -----------------------------------------------------------*/
void MC6C_UpdateAnalogStick(void)
{
	if(SBUS_CH.CH1 < AnalogStick_Min) AnalogStick.Right_X=-1.0;
	else if(SBUS_CH.CH1 < AnalogStick_Min_1) {AnalogStick.Right_X=((float)(SBUS_CH.CH1 - AnalogStick_Min) / (AnalogStick_Min_1 - AnalogStick_Min))-1.0f;}
	else if(SBUS_CH.CH1 > AnalogStick_Max) AnalogStick.Right_X=1.0f;
	else if(SBUS_CH.CH1 > AnalogStick_Max_1) {AnalogStick.Right_X=(float)(SBUS_CH.CH1 - AnalogStick_Max_1) / (AnalogStick_Max - AnalogStick_Max_1);}
	else AnalogStick.Right_X=0.0;
	
	if(SBUS_CH.CH2 < AnalogStick_Min) AnalogStick.Left_Y=-1.0;
	else if(SBUS_CH.CH2 < AnalogStick_Min_1) {AnalogStick.Left_Y=((float)(SBUS_CH.CH2 - AnalogStick_Min) / (AnalogStick_Min_1 - AnalogStick_Min))-1.0f;}
	else if(SBUS_CH.CH2 > AnalogStick_Max) AnalogStick.Left_Y=1.0f;
	else if(SBUS_CH.CH2 > AnalogStick_Max_1) {AnalogStick.Left_Y=(float)(SBUS_CH.CH2 - AnalogStick_Max_1) / (AnalogStick_Max - AnalogStick_Max_1);}
	else AnalogStick.Left_Y = 0.0;
	
	if(SBUS_CH.CH3 < AnalogStick_Min) AnalogStick.Right_Y=-1.0;
	else if(SBUS_CH.CH3 < AnalogStick_Min_1) {AnalogStick.Right_Y=((float)(SBUS_CH.CH3 - AnalogStick_Min) / (AnalogStick_Min_1 - AnalogStick_Min))-1.0f;}
	else if(SBUS_CH.CH3 > AnalogStick_Max) AnalogStick.Right_Y=1.0f;
	else if(SBUS_CH.CH3 > AnalogStick_Max_1) {AnalogStick.Right_Y=(float)(SBUS_CH.CH3 - AnalogStick_Max_1) / (AnalogStick_Max - AnalogStick_Max_1);}
	else AnalogStick.Right_Y = 0.0;
	
	if(SBUS_CH.CH4 < AnalogStick_Min) AnalogStick.Left_X=-1.0;
	else if(SBUS_CH.CH4 < AnalogStick_Min_1) {AnalogStick.Left_X=((float)(SBUS_CH.CH4 - AnalogStick_Min) / (AnalogStick_Min_1 - AnalogStick_Min))-1.0f;}
	else if(SBUS_CH.CH4 > AnalogStick_Max) AnalogStick.Left_X=1.0f;
	else if(SBUS_CH.CH4 > AnalogStick_Max_1) {AnalogStick.Left_X=(float)(SBUS_CH.CH4 - AnalogStick_Max_1) / (AnalogStick_Max - AnalogStick_Max_1);}
	else AnalogStick.Left_X=0.0;
	
}

/*-----------------------------------------------------------
函数功能: 航模遥控器控制小车运动
输入参数: 航模遥控器摇杆数值结构体
返 回 值: None
说    明: 调用该函数实现遥控器摇杆控制小车运动方向
左侧摇杆Y轴控制小车前后速度大小
右侧摇杆X轴控制小车左右转弯幅度大小
 -----------------------------------------------------------*/
void MC6C_Drive(AnalogStick_Struct AnalogStick)
{
	if(SBUS_CH.ConnectState)//处于连接状态
	{
		int16_t Velocity = AnalogStick.Left_Y * 70;//速度值
		float Radian = AnalogStick.Right_X * -230;//转向弧度值		 
		Kinematic_Analysis(Velocity, Radian);//小车运动数学模型函数
	}
}
