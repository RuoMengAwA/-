#include "ADC_DMA.h"

/**
 * @brief 初始化ADC
 *
 * @param ADCPeriph
 * @param ADCChannel
 */
void ADC_Initialize(const ADC_PeriphTypeDef *ADCPeriph) {
    // // GPIO初始化
    GPIO_InitTypeDef GPIOInitData;
    GPIOInitData.GPIO_Mode = GPIO_Mode_AIN;
    for (uint8_t i = 0; i < ADCPeriph->channelNumber; i++) {
        switch ((unsigned int)ADCPeriph->channels[i - 1].GPIOx) {
        case (unsigned int)GPIOA:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
            GPIO_Init(GPIOA, &GPIOInitData);
            break;
        case (unsigned int)GPIOB:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
            GPIO_Init(GPIOB, &GPIOInitData);
            break;
        case (unsigned int)GPIOC:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
            GPIO_Init(GPIOC, &GPIOInitData);
            break;
        case (unsigned int)GPIOD:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
            GPIO_Init(GPIOD, &GPIOInitData);
            break;
        case (unsigned int)GPIOE:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
            GPIO_Init(GPIOE, &GPIOInitData);
            break;
        case (unsigned int)GPIOF:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
            GPIO_Init(GPIOF, &GPIOInitData);
            break;
        case (unsigned int)GPIOG:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
            GPIO_Init(GPIOG, &GPIOInitData);
            break;
        }
    }
    // DMA初始化
    DMA_InitTypeDef DMAInitData;
    DMAInitData.DMA_PeripheralBaseAddr = (uint32_t)(&(ADCPeriph->ADCx->DR));
    DMAInitData.DMA_MemoryBaseAddr = (uint32_t)ADCPeriph->conversionValue;
    DMAInitData.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMAInitData.DMA_BufferSize = ADCPeriph->channelNumber;
    DMAInitData.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMAInitData.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMAInitData.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMAInitData.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMAInitData.DMA_Mode = DMA_Mode_Circular;
    DMAInitData.DMA_Priority = DMA_Priority_High;
    DMAInitData.DMA_M2M = DMA_M2M_Disable;

    switch ((unsigned int)ADCPeriph->ADCx) {
    case (unsigned int)ADC1:
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
        DMA_DeInit(DMA1_Channel1);
        DMA_Init(DMA1_Channel1, &DMAInitData);
        DMA_Cmd(DMA1_Channel1, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
        break;
    default:
        break;
    }

    ADC_InitTypeDef ADCInitData;
    ADCInitData.ADC_Mode = ADC_Mode_Independent;
    ADCInitData.ADC_ScanConvMode = ENABLE;
    ADCInitData.ADC_ContinuousConvMode = ENABLE;
    ADCInitData.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADCInitData.ADC_DataAlign = ADC_DataAlign_Right;
    ADCInitData.ADC_NbrOfChannel = ADCPeriph->channelNumber;
    ADC_Init(ADCPeriph->ADCx, &ADCInitData);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    for(uint8_t i = 0; i < ADCPeriph->channelNumber; i++){
        ADC_RegularChannelConfig(ADCPeriph->ADCx, ADCPeriph->channels->channel, i + 1, ADCPeriph->channels->sampleTime);
    }
    ADC_DMACmd(ADCPeriph->ADCx, ENABLE);
    ADC_Cmd(ADCPeriph->ADCx, ENABLE);
    ADC_ResetCalibration(ADCPeriph->ADCx);
    while (ADC_GetResetCalibrationStatus(ADCPeriph->ADCx))
        ;
    ADC_StartCalibration(ADCPeriph->ADCx);
    while (ADC_GetCalibrationStatus(ADCPeriph->ADCx))
        ;
    ADC_SoftwareStartConvCmd(ADCPeriph->ADCx, ENABLE);
}

uint16_t ADC_GetConvValue(const ADC_PeriphTypeDef *ADCPeriph, uint8_t channelIndex) {
    return ADCPeriph->conversionValue[channelIndex];
}
