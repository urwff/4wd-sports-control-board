#include "adc1.h"

ADC_HandleTypeDef hadc1;

/*-----------------------------------------------------------
函数功能: ADC1初始化配置
输入参数: None
返 回 值: None
说    明: 配置ADC1-IN4参数，采集外部电压
 -----------------------------------------------------------*/
void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig;

  hadc1.Instance = ADC1;//ADC1
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;//单个通道采集不扫描其他通道
  hadc1.Init.ContinuousConvMode = ENABLE;//连续采集
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;//软件触发方式
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;//数据向右对齐
  hadc1.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadc1);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_4;//通道4
  sConfig.Rank = ADC_REGULAR_RANK_1;//采样顺序序号
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;//采样时间
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}


