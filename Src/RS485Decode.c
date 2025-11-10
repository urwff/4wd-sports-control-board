#include "RS485Decode.h"
	
uint16_t POLYNOMIAL=0xA001;//多项式
uint16_t PRESET_VALUE=0xFFFF;//预设值
/*将需要校验的数组地址与校验的数据长度值代入下面函数便会返回CRC-16校验值*/
uint16_t ModbusCRC_16(uint8_t *pr,uint8_t byteLength)//计算CRC-16值函数
{ 
  uint8_t i, j;
  uint16_t fcs;
  fcs = PRESET_VALUE;
  for(i = 0; i < byteLength; i++)
  { fcs = fcs ^ pr[i];
    for (j = 0; j < 8; j++)
    { if (fcs & 0x01)
      { fcs = (fcs >> 1) ^ POLYNOMIAL;
      }
      else
      { fcs = (fcs >> 1);
      }
    }
  } 
  return(fcs);
}

