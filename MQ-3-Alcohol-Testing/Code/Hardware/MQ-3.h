/**
 * @file MQ-3.h
 * @author your name (you@domain.com)
 * @brief 简简单单进行一个烂的摆
 * @version 0.1
 * @date 2022-04-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _MQ_3_H_
#define _MQ_3_H_

#include "stdint.h"
#include "stdbool.h"
#include "stm32f10x.h"

typedef struct MQ3_MeasureResultTypedef{
    uint16_t raw;//ADC源值
    double air;//推算空气中浓度 mg/L
    double blood; //推算血液中浓度 mg/100mL
}MQ3_MeasureResultTypedef;


//传感器电源引脚定义...
#define MQ3_SENSOR_GPIO_APB2_PERIPH RCC_APB2Periph_GPIOA
#define MQ3_SENSOR_PWR_GPIO_GROUP GPIOA
#define MQ3_SENSOR_PWR_GPIO_PIN   GPIO_Pin_5
//风扇电源引脚定义...
#define MQ3_FAN_GPIO_APB2_PERIPH RCC_APB2Periph_GPIOA
#define MQ3_FAN_PWR_GPIO_GROUP GPIOA
#define MQ3_FAN_PWR_GPIO_PIN   GPIO_Pin_6

#define MQ3_SAMPLE_VALUE_BUFFER_SIZE 64

MQ3_MeasureResultTypedef MQ3_GetLastMeasureValue(void);
void MQ3_Handler(void);
bool MQ3_StartMeasure(void);
bool MQ3_StartCalibrate(void);
void MQ3_SetMeasureFinishCallback(void(*callback)());
void MQ3_Init(void);
bool MQ3_IsBusy(void);
bool MQ3_SetContinuousMeasure(int status);
bool MQ3_SetAutoFeedback(int status);
bool MQ3_SetSensorAlwaysOn(int status);

#endif
