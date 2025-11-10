#ifndef  __SUPPLYVOLTAGE_H
#define __SUPPLYVOLTAGE_H
#include "main.h"
extern __IO float SupplyVoltage;//电源电压值
extern __IO uint8_t BatteryCapacity;//电池电量，百分比0-100
float ReadSupplyVoltage(void);//读取电源电压值
uint8_t ReadBatteryCapacity(float SupplyVoltage);//读取电池剩余电量百分比
#endif
