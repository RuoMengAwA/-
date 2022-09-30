#ifndef _ADC_DMA_H_
#define _ADC_DMA_H_

#include "stm32f10x.h"

typedef struct ADC_ChannelTypeDef{
    uint8_t channel;
    uint8_t sampleTime;
    uint16_t GPIO_Pin_x;
    GPIO_TypeDef *GPIOx;
}ADC_ChannelTypeDef;

typedef struct ADC_PeriphTypeDef{
    ADC_TypeDef *ADCx;
    const uint8_t channelNumber;
    const ADC_ChannelTypeDef *channels;
    volatile uint16_t conversionValue[18];

}ADC_PeriphTypeDef;



void ADC_Initialize(const ADC_PeriphTypeDef *ADCPeriph);
uint16_t ADC_GetConvValue(const ADC_PeriphTypeDef *ADCPeriph, uint8_t channelIndex);

#endif
