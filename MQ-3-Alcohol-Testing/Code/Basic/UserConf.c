#include "UserConf.h"
#include "stm32f10x.h"
#include "TimingAndDelay.h"



//flash写入的起始地址
#define FLASH_USER_CONF_BASE_ADDR (FLASH_BASE_ADDR + FLASH_USER_CONF_BLOCK * FLASH_BLOCK_SIZE)
//flash结束块
#define FLASH_USER_CONF_END_BLOCK (FLASH_USER_CONF_BLOCK + (FLASH_USER_CONF_SIZE / FLASH_BLOCK_SIZE))
//根据flash地址推算所在的块...
#define FLASH_GetBlockAddr(block) (FLASH_BASE_ADDR + block * FLASH_BLOCK_SIZE)
//用户信息的总大小...
#define FLASH_USER_CONF_SIZE (FLASH_USER_CONF_NUM * sizeof(int))

//flash中内容在内存中的拷贝...实际上修改和访问的是内存中的内容，然后再定时同步到flash中
uint32_t UserConf_Data[FLASH_USER_CONF_SIZE];

//这样就可以像访问数组一样访问flash内容了...不过不能直接写入就是了
const uint32_t *UserConf_FlashData = (uint32_t *)FLASH_USER_CONF_BASE_ADDR;



//默认数据...
const uint32_t UserConf_DefaultData[FLASH_USER_CONF_SIZE] = {
    false,//UserConf_12HourClock
    255,//UserConf_DisplayContrast
};

/**
 * @brief 恢复用户数据
 *
 */
void UserConf_Recover() {
    for (unsigned int i = 0; i < FLASH_USER_CONF_NUM; i++) {
        UserConf_Data[i] = UserConf_FlashData[i];
    }
}

/**
 * @brief 保存用户数据，可能擦写flash
 *
 */
void UserConf_Save() {
    //记录上次写入的时间，防止写入频率过高
    static uint32_t lastSaveTime = 0;
    if(Timing_GetMillisecond()-lastSaveTime<FLASH_USER_CONF_SAVE_TIME_INTERVAL)
        return;
    lastSaveTime = Timing_GetMillisecond();
    //校验是否更改过。如果没有则不写入flash，减少擦写次数
    bool changed = false;
    for (unsigned int i = 0; i < FLASH_USER_CONF_NUM; i++) {
        if (UserConf_Data[i] != UserConf_FlashData[i]) {
            changed = true;
            break;
        }
    }
    //写入flash
    if (changed) {
        FLASH_Unlock();
        for (unsigned int i = FLASH_USER_CONF_BLOCK; i <= FLASH_USER_CONF_END_BLOCK; i++) {
            FLASH_ErasePage(FLASH_GetBlockAddr(i));
        }
        for (unsigned int i = 0; i < FLASH_USER_CONF_NUM  + 1; i++) {
            FLASH_ProgramWord((uint32_t)&UserConf_FlashData[i], UserConf_Data[i]);
            FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
        }
        FLASH_Lock();
    }
}


/**
 * @brief 擦除用户设置并复位系统
 * 
 */
void UserConf_Erase(){
    //擦除用户数据
    FLASH_Unlock();
    for (unsigned int i = FLASH_USER_CONF_BLOCK; i <= FLASH_USER_CONF_END_BLOCK; i++) {
        FLASH_ErasePage(FLASH_GetBlockAddr(i));
    }
    //写入默认数据
    for (unsigned int i = 0; i < FLASH_USER_CONF_NUM + 1; i++) {
            FLASH_ProgramWord((uint32_t)&UserConf_FlashData[i], UserConf_DefaultData[i]);
            FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    }
    FLASH_Lock();
    //软复位
    NVIC_SystemReset();
}

