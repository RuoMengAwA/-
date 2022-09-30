/**
 * @file UserConf.h
 * @author your name (you@domain.com)
 * @brief emm类似于注册表的玩意
 * @version 0.1
 * @date 2022-03-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _USER_CONF_H_
#define _USER_CONF_H_

#include "stdint.h"
#include "stdbool.h"


// flash基地址 //查询数据手册
#define FLASH_BASE_ADDR 0x08000000
// flash页大小(字节) //查询数据手册
#define FLASH_BLOCK_SIZE 1024
//用户信息起始页(从0开始计数) //根据程序大小而定，需要保证此块之后的空间没有程序
#define FLASH_USER_CONF_BLOCK 64
//用户信息数目 //就是有多少条信息，每条信息长度都是4字节，当然组合一下也没问题，毕竟时连续内存空间
//会自动根据用户信息的数目推算所需FLASH块数...
#define FLASH_USER_CONF_NUM 2
//最短两次保存时间间隔,单位ms
#define FLASH_USER_CONF_SAVE_TIME_INTERVAL 60000


extern uint32_t UserConf_Data[];


#define UserConf_MQ3_LastCalibrationValue     (*(unsigned int *)&UserConf_Data[0])

void UserConf_Save(void);
void UserConf_Recover(void);
void UserConf_Erase(void);

#endif // !_USER_CONF_H_

