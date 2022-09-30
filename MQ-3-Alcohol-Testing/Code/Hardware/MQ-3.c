/**
 * @file MQ-3.c
 * @author your name (you@domain.com)
 * @brief 非常感谢你来维护这一坨屎
 * @version 0.1
 * @date 2022-04-04
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "MQ-3.h"
#include "ADC_DMA.h"
#include "TimingAndDelay.h"
#include "USART.h"
#include "UserConf.h"
#include "math.h"
#include "stdio.h"


static const ADC_ChannelTypeDef ADC1_Channel[2] = {{ADC_Channel_0, ADC_SampleTime_239Cycles5, GPIO_Pin_0, GPIOA},
                                                   {ADC_Channel_1, ADC_SampleTime_239Cycles5, GPIO_Pin_1, GPIOA}};
static ADC_PeriphTypeDef ADC1Periph = {ADC1, 2, ADC1_Channel};

//连接在MQ3测试引脚上的基准电阻值
#define MQ3_REFERENCE_RESISTANCE_VALUE 1000.0

#define MQ3_AIR_BLOOD_TRANS_RATIO 2200.0

//转换拟合曲线所使用的symmetrical sigmoidal函数的参数A，B，C和D
// y=d+((a-d)/(1+(x/c)^b))
#define MQ3_RES_TRANS_CONST_A 172384684415.19
#define MQ3_RES_TRANS_CONST_B 1.21479420337627
#define MQ3_RES_TRANS_CONST_C 2.42689583805007E-11
#define MQ3_RES_TRANS_CONST_D -0.0177145071258038

//校正拟合的误差，虽然这样略显粗暴
#define MQ3_RES_TRANS_CONST_OFFSET -4.239830063E-3

#define MQ3_TASK_MEASURE 0
#define MQ3_TASK_CALIBRATE 1

//各阶段的最小采样时间和允许误差
//校准
#define MQ3_SAMPLE_TIME_CALIBRATE 7000
#define MQ3_SAMPLE_ERROR_CALIBRATE 10
//采样
#define MQ3_SAMPLE_TIME_SAMPLE 7000
#define MQ3_SAMPLE_ERROR_SAMPLE 10
//收集
#define MQ3_SAMPLE_TIME_COLLECT 5000
#define MQ3_SAMPLE_ERROR_COLLECT 15
//换气
#define MQ3_SAMPLE_TIME_CLEAN 5000
#define MQ3_SAMPLE_ERROR_CLEAN 15
//预热
#define MQ3_SAMPLE_TIME_WARNUP 5000
#define MQ3_SAMPLE_ERROR_WARNUP 15

typedef enum MQ3_MeasureStep {
    MQ3_MeasureStep_Init,      //初始化
    MQ3_MeasureStep_WarnUp,    //预热
    MQ3_MeasureStep_Clean,     //清洁，或者说是换气
    MQ3_MeasureStep_Calibrate, //校准
    MQ3_MeasureStep_Collect,   //采集气体
    MQ3_MeasureStep_Sample     //采样

} MQ3_MeasureStep;

typedef enum MQ3_CalibrateStep {
    MQ3_CalibrateStep_Init,      //初始化
    MQ3_CalibrateStep_WarnUp,    //预热
    MQ3_CalibrateStep_Collect,   //收集
    MQ3_CalibrateStep_Calibrate, //校准
} MQ3_CalibrateStep;

//施加在测试电压脚的电压对应的ADC值
static uint16_t MQ3_PowerADCValue;

static bool MQ3_Busy = false;
static MQ3_MeasureResultTypedef MQ3_LastMeasureValue;
static Time_Typedef MQ3_LastMeasureTime;
static uint8_t MQ3_Task;

//传感器不断电
static bool MQ3_SensorAlwaysOn = true;
static bool MQ3_ContinuousMeasure = false;
static bool MQ3_AutoFeedback = false;

static void (*MQ3_MeasureFinishCallback)() = NULL;

static int MQ3_SampleStabilizeTime;
static uint16_t MQ3_SampleError;
static uint32_t MQ3_SampleStartTime;

/**
 * @brief 就是简单的求绝对值，不知道为什么math.h里找不到abs
 *
 * @param val
 * @return int
 */
int MQ3_Abs(int val) {
    return (val > 0) ? val : -val;
}

/**
 * @brief 简简单单求平均值
 *
 * @param dat
 * @param num
 * @return uint16_t
 */
uint16_t MQ3_UInt16Arg(uint16_t dat[], unsigned int num) {
    unsigned int sum = 0;
    for (unsigned int i = 0; i < num; i++) {
        sum += dat[i];
    }
    return sum / num;
}

/**
 * @brief 将MQ3的ADC值转化为酒精浓度值，单位mg/L
 *
 * @param baseADCValue 无酒精时的基准ADC值
 * @param curADCValue 当前ADC值
 * @return float
 */
double MQ3_GetAirAlcoholConcentration(uint16_t baseADCValue, uint16_t curADCValue) {
    double baseResistanceValue = (MQ3_REFERENCE_RESISTANCE_VALUE * (MQ3_PowerADCValue - baseADCValue)) / baseADCValue;
    double curResistanceValue = (MQ3_REFERENCE_RESISTANCE_VALUE * (MQ3_PowerADCValue - curADCValue)) / curADCValue;
    double resistanceValueRatio = curResistanceValue / baseResistanceValue;
    return MQ3_RES_TRANS_CONST_D + ((MQ3_RES_TRANS_CONST_A - MQ3_RES_TRANS_CONST_D) / (1.0 + pow(resistanceValueRatio / MQ3_RES_TRANS_CONST_C, MQ3_RES_TRANS_CONST_B))) + MQ3_RES_TRANS_CONST_OFFSET;
}

/**
 * @brief 由ADC值推算血液浓度，单位mg/100mL
 *
 * @param baseADCValue
 * @param curADCValue
 * @return double
 */
double MQ3_GetBloodAlcoholConcentration(uint16_t baseADCValue, uint16_t curADCValue) {
    return MQ3_GetAirAlcoholConcentration(baseADCValue, curADCValue) * MQ3_AIR_BLOOD_TRANS_RATIO;
}

void MQ3_SetMeasureResult(uint16_t raw) {
    MQ3_LastMeasureValue.raw = raw;
    MQ3_LastMeasureValue.air = MQ3_GetAirAlcoholConcentration(UserConf_MQ3_LastCalibrationValue, raw);
    MQ3_LastMeasureValue.blood = MQ3_GetBloodAlcoholConcentration(UserConf_MQ3_LastCalibrationValue, raw);
}

/**
 * @brief 开始测量...不管测量的是啥
 * ADC的值在stabilizeTime的时间内波动不超过err即算测量成功...
 *
 * @param stabilizeTime 要求稳定的时间
 * @param err 允许的误差
 */
void MQ3_SampleStart(unsigned int stabilizeTime, uint16_t err) {
    MQ3_SampleStabilizeTime = stabilizeTime;
    MQ3_SampleError = err;
    MQ3_SampleStartTime = Timing_GetMillisecond();
}

/**
 * @brief 测量处理...
 *
 * @return int16_t 采样中中返回-1，否则返回最后一次采样结果
 */
int16_t MQ3_SampleHandler() {
    static uint16_t samleValueBuffer[MQ3_SAMPLE_VALUE_BUFFER_SIZE];
    static unsigned int samleValueBufferPtr = 0;
    static unsigned int sampleTimes = 0;
    static uint16_t sampleStartValue;
    samleValueBuffer[samleValueBufferPtr] = ADC_GetConvValue(&ADC1Periph, 0);
    samleValueBufferPtr++;
    if (samleValueBufferPtr == MQ3_SAMPLE_VALUE_BUFFER_SIZE) {
        samleValueBufferPtr = 0;
    }
    sampleTimes++;
    if (sampleTimes > MQ3_SAMPLE_VALUE_BUFFER_SIZE) { //已经取到初值
        if (MQ3_Abs(MQ3_UInt16Arg(samleValueBuffer, MQ3_SAMPLE_VALUE_BUFFER_SIZE) - sampleStartValue) > MQ3_SampleError) {
            //误差过大重开
            samleValueBufferPtr = 0;                       //复位缓冲指针 //不复位也没关系
            sampleTimes = 0;                               //复位采样次数
            MQ3_SampleStartTime = Timing_GetMillisecond(); //复位开始时间
        }
        if (Timing_GetMillisecond() - MQ3_SampleStartTime > MQ3_SampleStabilizeTime) {
            //撑过了稳定时间，波动没有超过误差值
            samleValueBufferPtr = 0;                       //复位缓冲指针 //不复位也没关系
            sampleTimes = 0;                               //复位采样次数
            MQ3_SampleStartTime = Timing_GetMillisecond(); //复位开始时间
            return MQ3_UInt16Arg(samleValueBuffer, MQ3_SAMPLE_VALUE_BUFFER_SIZE);
        }

    } else if (sampleTimes == MQ3_SAMPLE_VALUE_BUFFER_SIZE) { //此时缓冲刚好填满，将缓冲的平均值设为初始值
        sampleStartValue = MQ3_UInt16Arg(samleValueBuffer, MQ3_SAMPLE_VALUE_BUFFER_SIZE);
    }
    return -1;
}

/**
 * @brief 酒精浓度测量处理函数，loop内重复执行
 * TIPS:这一段看着很乱，其实把switch的几个分支单独看的话还是可以的
 */
void MQ3_MeasureHandler() {
    static MQ3_MeasureStep step = MQ3_MeasureStep_Init;
    int16_t sampleVal;
    if (!MQ3_Busy)
        return;
    switch (step) {
        case MQ3_MeasureStep_Init: {
            if (MQ3_SensorAlwaysOn) {                                                             //传感器常开
                if (MQ3_LastMeasureValue.blood > 1.0) {                                           //上次检测饮酒
                    if (Timing_GetTime().integerForm - MQ3_LastMeasureTime.integerForm >= 3600) { //大于一小时
                                                                                                  //准备换气
                        step = MQ3_MeasureStep_Clean;
                        MQ3_SampleStart(MQ3_SAMPLE_TIME_CLEAN, MQ3_SAMPLE_ERROR_CLEAN);
                        GPIO_SetBits(MQ3_FAN_PWR_GPIO_GROUP, MQ3_FAN_PWR_GPIO_PIN); //风扇上电
                    } else {                                                        //小于一小时
                                                                                    //准备收集
                        step = MQ3_MeasureStep_Collect;
                        MQ3_SampleStart(MQ3_SAMPLE_TIME_COLLECT, MQ3_SAMPLE_ERROR_COLLECT);
                        GPIO_SetBits(MQ3_FAN_PWR_GPIO_GROUP, MQ3_FAN_PWR_GPIO_PIN); //风扇上电
                    }
                } else { //上次检测未饮酒
                    //准备校准
                    step = MQ3_MeasureStep_Calibrate;
                    MQ3_SampleStart(MQ3_SAMPLE_TIME_CALIBRATE, MQ3_SAMPLE_ERROR_CALIBRATE);
                }
            } else { //非传感器常开
                step = MQ3_MeasureStep_WarnUp;
                MQ3_SampleStart(MQ3_SAMPLE_TIME_WARNUP, MQ3_SAMPLE_ERROR_WARNUP);
            }
            GPIO_SetBits(MQ3_SENSOR_PWR_GPIO_GROUP, MQ3_SENSOR_PWR_GPIO_PIN); //传感器上电

        } break;
        case MQ3_MeasureStep_WarnUp: { //预热
            sampleVal = MQ3_SampleHandler();
            if (sampleVal != -1) {
                if (MQ3_LastMeasureValue.blood > 1.0) {                                           //上次检测饮酒
                    if (Timing_GetTime().integerForm - MQ3_LastMeasureTime.integerForm >= 3600) { //大于一小时
                                                                                                  //准备换气
                        step = MQ3_MeasureStep_Clean;
                        MQ3_SampleStart(MQ3_SAMPLE_TIME_CLEAN, MQ3_SAMPLE_ERROR_CLEAN);
                        GPIO_SetBits(MQ3_FAN_PWR_GPIO_GROUP, MQ3_FAN_PWR_GPIO_PIN); //风扇上电
                    } else {                                                        //小于一小时
                                                                                    //准备收集
                        step = MQ3_MeasureStep_Collect;
                        MQ3_SampleStart(MQ3_SAMPLE_TIME_COLLECT, MQ3_SAMPLE_ERROR_COLLECT);
                        GPIO_SetBits(MQ3_FAN_PWR_GPIO_GROUP, MQ3_FAN_PWR_GPIO_PIN); //风扇上电
                    }
                } else { //上次检测未饮酒
                    //准备校准
                    step = MQ3_MeasureStep_Calibrate;
                    MQ3_SampleStart(MQ3_SAMPLE_TIME_CALIBRATE, MQ3_SAMPLE_ERROR_CALIBRATE);
                }
            }
        } break;
        case MQ3_MeasureStep_Clean: { //换气
            sampleVal = MQ3_SampleHandler();
            if (sampleVal != -1) {
                //准备校准
                step = MQ3_MeasureStep_Calibrate;
                MQ3_SampleStart(MQ3_SAMPLE_TIME_CALIBRATE, MQ3_SAMPLE_ERROR_CALIBRATE);
                GPIO_ResetBits(MQ3_FAN_PWR_GPIO_GROUP, MQ3_FAN_PWR_GPIO_PIN); //风扇断电
            }
        } break;
        case MQ3_MeasureStep_Calibrate: { //校准
            sampleVal = MQ3_SampleHandler();
            if (sampleVal != -1) {
                UserConf_MQ3_LastCalibrationValue = sampleVal;
                //准备收集
                step = MQ3_MeasureStep_Collect;
                MQ3_SampleStart(MQ3_SAMPLE_TIME_COLLECT, MQ3_SAMPLE_ERROR_COLLECT);
                GPIO_SetBits(MQ3_FAN_PWR_GPIO_GROUP, MQ3_FAN_PWR_GPIO_PIN); //风扇上电
            }
        } break;
        case MQ3_MeasureStep_Collect: { //收集
            sampleVal = MQ3_SampleHandler();
            if (sampleVal != -1) {
                //准备采样
                step = MQ3_MeasureStep_Sample;
                MQ3_SampleStart(MQ3_SAMPLE_TIME_SAMPLE, MQ3_SAMPLE_ERROR_SAMPLE);
                GPIO_ResetBits(MQ3_FAN_PWR_GPIO_GROUP, MQ3_FAN_PWR_GPIO_PIN); //风扇断电
            }
        } break;
        case MQ3_MeasureStep_Sample: { //采样
            sampleVal = MQ3_SampleHandler();
            if (sampleVal != -1) {
                //采样成功
                MQ3_LastMeasureTime = Timing_GetTime();
                MQ3_SetMeasureResult(sampleVal);
                if (!MQ3_SensorAlwaysOn) {
                    GPIO_ResetBits(MQ3_SENSOR_PWR_GPIO_GROUP, MQ3_SENSOR_PWR_GPIO_PIN);
                }
                if (MQ3_MeasureFinishCallback) {
                    MQ3_MeasureFinishCallback();
                }
                step = MQ3_MeasureStep_Init;
                MQ3_Busy = false;
            }
        }
    }
}

/**
 * @brief 开始测量
 *
 * @return true 成功
 * @return false 失败，正忙
 */
bool MQ3_StartMeasure(void) {
    if (MQ3_Busy) {
        return false;
    }
    MQ3_Busy = true;
    MQ3_Task = MQ3_TASK_MEASURE;
    return true;
}

bool MQ3_StartCalibrate(void) {
    if (MQ3_Busy) {
        return false;
    }
    MQ3_Busy = true;
    MQ3_Task = MQ3_TASK_CALIBRATE;
    return true;
}

/**
 * @brief 设置测量完成的回调函数
 *
 * @param callback
 */
void MQ3_SetMeasureFinishCallback(void (*callback)()) {
    MQ3_MeasureFinishCallback = callback;
}

/**
 * @brief 获取最后一次酒精检测值
 *
 */
MQ3_MeasureResultTypedef MQ3_GetLastMeasureValue() {
    return MQ3_LastMeasureValue;
}

bool MQ3_SetContinuousMeasure(int status) {
    if (status == 0) {
        MQ3_ContinuousMeasure = false;
        MQ3_Busy = false;
        if (!MQ3_SensorAlwaysOn) {
            GPIO_ResetBits(MQ3_SENSOR_PWR_GPIO_GROUP, MQ3_SENSOR_PWR_GPIO_PIN); //传感器断电
        }
    } else if (status == 1) {
        MQ3_ContinuousMeasure = true;
        MQ3_Busy = true;
        GPIO_SetBits(MQ3_SENSOR_PWR_GPIO_GROUP, MQ3_SENSOR_PWR_GPIO_PIN); //传感器上电
    }
    return MQ3_ContinuousMeasure;
}

bool MQ3_SetAutoFeedback(int status) {
    if (status == 0) {
        MQ3_AutoFeedback = false;
    } else if (status == 1) {
        MQ3_AutoFeedback = true;
    }
    return MQ3_AutoFeedback;
}

bool MQ3_SetSensorAlwaysOn(int status) {
    if (status == 0) {
        MQ3_SensorAlwaysOn = false;
    } else if (status == 1) {
        MQ3_SensorAlwaysOn = true;
    }
    return MQ3_SensorAlwaysOn;
}

/**
 * @brief 获取MQ3的工作状态
 *
 * @return true MQ3正忙
 * @return false MQ3空闲
 */
bool MQ3_IsBusy() {
    return MQ3_Busy;
}

void MQ3_ContinuousMeasureHandler() {
    static uint32_t lastUpdateTime = 0;
    static uint64_t measureTotal = 0;
    static unsigned int measureTimes = 0;
    measureTotal += ADC_GetConvValue(&ADC1Periph, 0);
    measureTimes++;
    if (Timing_GetMillisecond() - lastUpdateTime >= 1000) {
        MQ3_SetMeasureResult(measureTotal / measureTimes);
        lastUpdateTime = Timing_GetMillisecond();
        MQ3_LastMeasureTime = Timing_GetTime();
        measureTotal = 0;
        measureTimes = 0;
        if (MQ3_MeasureFinishCallback && MQ3_AutoFeedback) {
            MQ3_MeasureFinishCallback();
        }
    }
}

void MQ3_CalibrateHandler() {
    static MQ3_CalibrateStep step = MQ3_CalibrateStep_Init;
    int16_t sampleVal;
    if(!MQ3_Busy){
        return;
    }
    switch (step) {
        case MQ3_CalibrateStep_Init: {
            if (!MQ3_SensorAlwaysOn) {
                GPIO_SetBits(MQ3_SENSOR_PWR_GPIO_GROUP, MQ3_SENSOR_PWR_GPIO_PIN); //传感器上电
                //准备预热
                step = MQ3_CalibrateStep_WarnUp;
                MQ3_SampleStart(MQ3_SAMPLE_TIME_WARNUP, MQ3_SAMPLE_ERROR_WARNUP);
            } else {
                //准备收集
                step = MQ3_CalibrateStep_Collect;
                MQ3_SampleStart(MQ3_SAMPLE_TIME_COLLECT, MQ3_SAMPLE_ERROR_COLLECT);
                GPIO_SetBits(MQ3_FAN_PWR_GPIO_GROUP, MQ3_FAN_PWR_GPIO_PIN); //风扇上电
            }
        } break;
        case MQ3_CalibrateStep_WarnUp: {
            sampleVal = MQ3_SampleHandler();
            if (sampleVal != -1) {
                //准备收集
                step = MQ3_CalibrateStep_Collect;
                MQ3_SampleStart(MQ3_SAMPLE_TIME_COLLECT, MQ3_SAMPLE_ERROR_COLLECT);
                GPIO_SetBits(MQ3_FAN_PWR_GPIO_GROUP, MQ3_FAN_PWR_GPIO_PIN); //风扇上电
            }
        } break;
        case MQ3_CalibrateStep_Collect: {
            sampleVal = MQ3_SampleHandler();
            if (sampleVal != -1) {
                //准备校准
                step = MQ3_CalibrateStep_Calibrate;
                MQ3_SampleStart(MQ3_SAMPLE_TIME_CALIBRATE, MQ3_SAMPLE_ERROR_CALIBRATE);
                GPIO_ResetBits(MQ3_FAN_PWR_GPIO_GROUP, MQ3_FAN_PWR_GPIO_PIN); //风扇断电
            }
        } break;
        case MQ3_CalibrateStep_Calibrate: {
            sampleVal = MQ3_SampleHandler();
            if (sampleVal != -1) {
                UserConf_MQ3_LastCalibrationValue = sampleVal;
                //准备收集
                step = MQ3_CalibrateStep_Init;
                MQ3_Busy = false;
            }
        } break;
    }
}

void MQ3_Init() {
    ADC_Initialize(&ADC1Periph);
    RCC_APB2PeriphClockCmd(MQ3_SENSOR_GPIO_APB2_PERIPH | MQ3_FAN_GPIO_APB2_PERIPH, ENABLE);
    GPIO_InitTypeDef GPIOInitData;
    GPIOInitData.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIOInitData.GPIO_Speed = GPIO_Speed_2MHz;
    GPIOInitData.GPIO_Pin = MQ3_SENSOR_PWR_GPIO_PIN;
    GPIO_Init(MQ3_SENSOR_PWR_GPIO_GROUP, &GPIOInitData);
    GPIOInitData.GPIO_Pin = MQ3_FAN_PWR_GPIO_PIN;
    GPIO_Init(MQ3_FAN_PWR_GPIO_GROUP, &GPIOInitData);
    GPIO_ResetBits(MQ3_SENSOR_PWR_GPIO_GROUP, MQ3_SENSOR_PWR_GPIO_PIN);
    GPIO_ResetBits(MQ3_FAN_PWR_GPIO_GROUP, MQ3_FAN_PWR_GPIO_PIN);
    if (MQ3_SensorAlwaysOn) {
        GPIO_SetBits(MQ3_SENSOR_PWR_GPIO_GROUP, MQ3_SENSOR_PWR_GPIO_PIN);
    }
}

void MQ3_Handler() {
    MQ3_PowerADCValue = 2 * ADC_GetConvValue(&ADC1Periph, 1);
    if (MQ3_ContinuousMeasure) {
        MQ3_ContinuousMeasureHandler();
    } else {
        if (MQ3_Task == MQ3_TASK_MEASURE) {
            MQ3_MeasureHandler();
        } else if (MQ3_Task == MQ3_TASK_CALIBRATE) {
            MQ3_CalibrateHandler();
        }
    }
}
