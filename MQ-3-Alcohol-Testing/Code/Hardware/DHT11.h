#ifndef _DHT11_H_
#define _DHT11_H_


#define DHT11_GPIO_PIN  GPIO_Pin_4
#define DHT11_GPIO_GROUP GPIOA
#define DHT11_GPIO_PERIPH RCC_APB2Periph_GPIOA

#define DHT11_MEASURE_TIME_SPACING 60000

typedef struct DHT11_TempAndHumi{
    unsigned int temperature;
    unsigned int humidity;

} DHT11_TempAndHumi;

void DHT11_Init(void);
void DHT11_Update(void);
void DHT11_Handle(void);
DHT11_TempAndHumi DHT11_GetLastResult(void);

#endif
