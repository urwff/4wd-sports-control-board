#ifndef __MC6C_H
#define __MC6C_H
#include "main.h"
typedef struct
{	
	float Left_X;
	float Left_Y;
	float Right_X;
	float Right_Y;
}AnalogStick_Struct;

extern AnalogStick_Struct AnalogStick;

void MC6C_UpdateAnalogStick(void);//航模遥控器左右两侧摇杆值更新
void MC6C_Drive(AnalogStick_Struct AnalogStick);//航模遥控器控制小车运动
#endif
