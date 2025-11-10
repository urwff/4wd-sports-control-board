/**************************************************************************
文件功能：
1、读取电源电压值
2、读取电池剩余电量百分比
**************************************************************************/
#include "SupplyVoltage.h"
#include "adc1.h"
#include "LowPassFilter.h"

__IO float SupplyVoltage;//电源电压值

/*-----------------------------------------------------------
函数功能: 读取电源电压值
输入参数: None
返 回 值: 电源电压值
说    明: 调用该函数ADC采集电源电压并返回电源电压值
 -----------------------------------------------------------*/
float ReadSupplyVoltage(void)//读取电源电压值
{
	float VREF=3.4;//参考电压
	
	HAL_ADC_Start(&hadc1);//ADC开始采集电源电压信号
	LPF_SupplyVoltage.SampleValue = (float)(HAL_ADC_GetValue(&hadc1)*VREF)*0.001907f;//获取ADC采集的数值 换算成电压值赋值给低通滤波器结构体中的采集值
	SupplyVoltage = LowPassFilter(&LPF_SupplyVoltage);//电源电压值经过低通滤波器后数值会更加稳定
	return SupplyVoltage;
}

__IO uint8_t BatteryCapacity=0;//电池电量，百分比0-100
#define BatterySeriesNumber 3//锂电池串联数
/*-----------------------------------------------------------
函数功能: 读取电池剩余电量百分比
输入参数: 当前电源电压值
返 回 值: 当前电池剩余电量百分比
说    明: 电量百分比数值范围0-100，该函数仅对锂电池电量计算有效。
 -----------------------------------------------------------*/
uint8_t ReadBatteryCapacity(float SupplyVoltage)
{
	float BatteryVoltag;
	uint8_t BatteryCapacity;//电池电量，百分比0-100
	BatteryVoltag = (float)SupplyVoltage / BatterySeriesNumber;
	if(BatteryVoltag >= 4.06) BatteryCapacity=(((float)(BatteryVoltag - 4.06)/0.14)*10+90);
	else if(BatteryVoltag >= 3.98) BatteryCapacity=(((float)(BatteryVoltag - 3.98)/0.08)*10+80);
	else if(BatteryVoltag >= 3.92) BatteryCapacity=(((float)(BatteryVoltag - 3.92)/0.06)*10+70);
	else if(BatteryVoltag >= 3.87) BatteryCapacity=(((float)(BatteryVoltag - 3.87)/0.05)*10+60);
	else if(BatteryVoltag >= 3.82) BatteryCapacity=(((float)(BatteryVoltag - 3.82)/0.05)*10+50);
	else if(BatteryVoltag >= 3.79) BatteryCapacity=(((float)(BatteryVoltag - 3.79)/0.03)*10+40);
	else if(BatteryVoltag >= 3.77) BatteryCapacity=(((float)(BatteryVoltag - 3.77)/0.02)*10+30);
	else if(BatteryVoltag >= 3.74) BatteryCapacity=(((float)(BatteryVoltag - 3.74)/0.03)*10+20);
	else if(BatteryVoltag >= 3.68) BatteryCapacity=(((float)(BatteryVoltag - 3.68)/0.06)*10+10);
	else if(BatteryVoltag >= 3.45) BatteryCapacity=(((float)(BatteryVoltag - 3.45)/0.23)*10+5);
	else if(BatteryVoltag >= 3.0) BatteryCapacity=(((float)(BatteryVoltag - 3.0)/0.45)*10+0);
	else BatteryCapacity = 0;
	if(BatteryCapacity > 100) BatteryCapacity = 100;
	return BatteryCapacity;
}
