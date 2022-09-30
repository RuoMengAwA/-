#include "DHT11.h"
#include "TimingAndDelay.h"
#include "stdbool.h"
#include "stm32f10x.h"

DHT11_TempAndHumi DHT11_LastResult;

void DHT11_SetIOIn() {

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(DHT11_GPIO_GROUP, &GPIO_InitStructure);
}

void DHT11_SetIOOut() {

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(DHT11_GPIO_GROUP, &GPIO_InitStructure);
}

void DHT11_Reset() {
    DHT11_SetIOOut();
    GPIO_ResetBits(DHT11_GPIO_GROUP, DHT11_GPIO_PIN); //
    Delay_Millisecond(20);                            //拉低至少18ms
    GPIO_SetBits(DHT11_GPIO_GROUP, DHT11_GPIO_PIN);   //
    Delay_MicroSecond(30);                            //主机拉高20~40us
}

bool DHT11_Check() {
    uint8_t retry=0;
    DHT11_SetIOIn();//IO到输入状态	 
    while (GPIO_ReadInputDataBit(DHT11_GPIO_GROUP, DHT11_GPIO_PIN)&&retry<100){//DHT11会拉低40~80us
        retry++;
        Delay_MicroSecond(1);
    }	 
    if(retry>=100)return 1; else retry=0;
    while (!GPIO_ReadInputDataBit(DHT11_GPIO_GROUP, DHT11_GPIO_PIN)&&retry<100){//DHT11拉低后会再次拉高40~80us
        retry++;
        Delay_MicroSecond(1);
    }
    if(retry>=100)return 1;	    
    return 0;
}

uint8_t DHT11_ReadBit(void) {
    uint8_t retry = 0;
    while ((GPIO_ReadInputDataBit(DHT11_GPIO_GROUP, DHT11_GPIO_PIN) == 1) && retry < 100) //等待变为低电平
    {
        retry++;
        Delay_MicroSecond(1);
    }
    retry = 0;
    while ((GPIO_ReadInputDataBit(DHT11_GPIO_GROUP, DHT11_GPIO_PIN) == 0) && retry < 100) //等待变高电平
    {
        retry++;
        Delay_MicroSecond(1);
    }
    Delay_MicroSecond(40); //等待40us
    return GPIO_ReadInputDataBit(DHT11_GPIO_GROUP, DHT11_GPIO_PIN);
}

uint8_t DHT11_ReadByte(void) {
    uint8_t i, dat;
    dat = 0;
    for (i = 0; i < 8; i++) {
        dat <<= 1;
        dat |= DHT11_ReadBit();
    }
    return dat;
}

void DHT11_Init(void) {
    RCC_APB2PeriphClockCmd(DHT11_GPIO_PERIPH, ENABLE);
    DHT11_Reset();
    DHT11_Check();
}
void DHT11_Update(void) {
    uint8_t buf[5];
    uint8_t i;
    DHT11_Reset();
    if (DHT11_Check() == 0) {
        for (i = 0; i < 5; i++) { //读取40位数据
            buf[i] = DHT11_ReadByte();
        }
        if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4]) { //校验
            //校验成功
            DHT11_LastResult.humidity = buf[0];
            DHT11_LastResult.temperature = buf[2];
        }
    }
}
void DHT11_Handle(void) {
    static uint32_t lastMeasureTime = 0;
    if (Timing_GetMillisecond() - lastMeasureTime > DHT11_MEASURE_TIME_SPACING) {
        lastMeasureTime = Timing_GetMillisecond();
        DHT11_Update();
    }
}
DHT11_TempAndHumi DHT11_GetLastResult(void) {
    return DHT11_LastResult;
}
