/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/**
 * NOTICE:用了自己的内存管理，堆区大小设置为0，标准库的的内存管理不再可用
 * 
 * 
 * 
 * 
 * 
*/

#include "MQ-3.h"
#include "HC-06.h"
#include "sysInit.h"
#include "UserConf.h"
#include "DHT11.h"
#include "TimingAndDelay.h"


void setup(){
    //initial...
    
    
    
    sys_Init();
    TimingAndDelay_Init();
    MQ3_Init();
    DHT11_Init();
    Delay_Millisecond(1500);
    DHT11_Update();
    UserConf_Recover();
    HC06_Init();
}

void loop(){
    MQ3_Handler();
    HC06_Handle();
    DHT11_Handle();
    UserConf_Save();
}

int main(){
    setup();
    for(;;){
        loop();
    }
}
