#include "HC-06.h"
#include "DHT11.h"
#include "MQ-3.h"
#include "TimingAndDelay.h"
#include "USART.h"
#include "UserConf.h"
#include "cJSON.h"
#include "stdbool.h"
#include "string.h"
#include "sysInfo.h"

//错误号定义
#define HC06_ERR_COMMAND_TOO_LONG 0
#define HC06_ERR_COMMAND_FORMAT_ERROR 1
#define HC06_ERR_PARAM_ERROR 2
#define HC06_ERR_INVALID_COMMAND 3
#define HC06_ERR_COMMAND_WORD_NOT_FOUND 4

static char HC06_CmdRtnStrBuffer[HC06_CMD_RTN_STR_MAX_LENGTH];
USART_PeriphTypedef USART2Periph = {USART2};
USART_PeriphTypedef USART1Periph = {USART1};
#define HC06_USARTPeriph &USART1Periph
#define DEBUG_USARTPeriph &USART1Periph

void HC06_RtnHandler(cJSON *rtn);

void HC06_MeansureFinishCallback() {
    cJSON *rtn = cJSON_CreateObject();
    cJSON_AddItemToObject(rtn, "cmd", cJSON_CreateNumber(1));
    cJSON_AddItemToObject(rtn, "status", cJSON_CreateNumber(1));
    cJSON_AddItemToObject(rtn, "raw", cJSON_CreateNumber(MQ3_GetLastMeasureValue().raw));
    cJSON_AddItemToObject(rtn, "air", cJSON_CreateNumber(MQ3_GetLastMeasureValue().air));
    cJSON_AddItemToObject(rtn, "blood", cJSON_CreateNumber(MQ3_GetLastMeasureValue().blood));
    cJSON_AddItemToObject(rtn, "temp", cJSON_CreateNumber(DHT11_GetLastResult().temperature));
    cJSON_AddItemToObject(rtn, "humi", cJSON_CreateNumber(DHT11_GetLastResult().humidity));
    HC06_RtnHandler(rtn);
    cJSON_Delete(rtn);
}
/**
 * @brief 命令返回
 *
 * @param rtn 返回字串
 */
void HC06_RtnHandler(cJSON *rtn) {
    cJSON_PrintPreallocated(rtn, HC06_CmdRtnStrBuffer, HC06_CMD_RTN_STR_MAX_LENGTH, false); //写入命令缓冲
    USART_WriteStr(HC06_USARTPeriph, HC06_CmdRtnStrBuffer);
    USART_WriteStr(HC06_USARTPeriph, "\r\n"); //按刘璇的要求加上换行...                                 //写入串口发送缓冲...缓冲不要炸。。堆区给4KB了啊
    //USART_WriteStr(DEBUG_USARTPeriph, HC06_CmdRtnStrBuffer);
    //USART_WriteStr(DEBUG_USARTPeriph, "\r\n");//同样的一份给到电脑上
}

/**
 * @brief 错误返回...
 *
 * @param errNum 错误号
 */
void HC06_RtnErr(unsigned int errNum) {
    cJSON *rtn = cJSON_CreateObject();
    cJSON_AddItemToObject(rtn, "cmd", cJSON_CreateNumber(-1));
    cJSON_AddItemToObject(rtn, "err", cJSON_CreateNumber(errNum));
    HC06_RtnHandler(rtn);
    cJSON_Delete(rtn);
}

/**
 * @brief 命令执行函数
 *
 * @param cmd 经过cJSON解析后的玩意
 */
void HC06_CmdHandler(cJSON *cmd) {
    cJSON *item = NULL;                     //命令中的项
    cJSON *rtn = cJSON_CreateObject();      //返回值
    bool rtnInFunction = true;              //是否在本函数结束前发送返回值
    item = cJSON_GetObjectItem(cmd, "cmd"); //不会开辟新内存，所以应该不需要释放...吧
    if (!item) {                            //没得命令字
        cJSON_Delete(rtn);
        HC06_RtnErr(HC06_ERR_COMMAND_WORD_NOT_FOUND);
        return;
    }
    unsigned int cmdWord = item->valueint;                          //命令字
    cJSON_AddItemToObject(rtn, "cmd", cJSON_CreateNumber(cmdWord)); //将命令字加入返回字串中...
    switch (cmdWord) {
            /** PS:case后面语句组加括号可以触发代码折叠...至少折起来好看些*/

        case 0: { //获取版本
            cJSON_AddItemToObject(rtn, "version", cJSON_CreateString(SYS_VERSION_STRING));
        } break;
        case 1: { //检测酒精浓度
            MQ3_SetMeasureFinishCallback(HC06_MeansureFinishCallback);
            if (MQ3_StartMeasure() == true) {
                rtnInFunction = false;
            } else { //正忙
                cJSON_AddItemToObject(rtn, "status", cJSON_CreateNumber(0));
                cJSON_AddItemToObject(rtn, "raw", cJSON_CreateNumber(MQ3_GetLastMeasureValue().raw));
                cJSON_AddItemToObject(rtn, "air", cJSON_CreateNumber(MQ3_GetLastMeasureValue().air));
                cJSON_AddItemToObject(rtn, "blood", cJSON_CreateNumber(MQ3_GetLastMeasureValue().blood));
                cJSON_AddItemToObject(rtn, "temp", cJSON_CreateNumber(DHT11_GetLastResult().temperature));
                cJSON_AddItemToObject(rtn, "humi", cJSON_CreateNumber(DHT11_GetLastResult().humidity));
            }
        } break;
        case 2: { //最后一次检测结果
            cJSON_AddItemToObject(rtn, "raw", cJSON_CreateNumber(MQ3_GetLastMeasureValue().raw));
            cJSON_AddItemToObject(rtn, "air", cJSON_CreateNumber(MQ3_GetLastMeasureValue().air));
            cJSON_AddItemToObject(rtn, "blood", cJSON_CreateNumber(MQ3_GetLastMeasureValue().blood));
            cJSON_AddItemToObject(rtn, "temp", cJSON_CreateNumber(DHT11_GetLastResult().temperature));
            cJSON_AddItemToObject(rtn, "humi", cJSON_CreateNumber(DHT11_GetLastResult().humidity));
        } break;
        case 3: { //开始校准
            cJSON_AddItemToObject(rtn, "status", cJSON_CreateNumber(MQ3_StartCalibrate()));
        } break;
        case 4: { //重启
            item = cJSON_GetObjectItem(cmd, "confirn");
            if (item == NULL) {
                goto PARAM_ERROR;
            }
            if (strcmp("restart", item->valuestring) == 0) {
                cJSON_AddItemToObject(rtn, "status", cJSON_CreateNumber(0));
                NVIC_SystemReset();
            } else {
                cJSON_AddItemToObject(rtn, "status", cJSON_CreateNumber(-1));
            }
        } break;
        case 5: { //擦除
            item = cJSON_GetObjectItem(cmd, "confirn");
            if (item == NULL) {
                goto PARAM_ERROR;
            }
            if (strcmp("erase", item->valuestring) == 0) {
                cJSON_AddItemToObject(rtn, "status", cJSON_CreateNumber(0));
                UserConf_Erase();
            } else {
                cJSON_AddItemToObject(rtn, "status", cJSON_CreateNumber(-1));
            }
        } break;
        case 6: { //设置持续监测
            MQ3_SetMeasureFinishCallback(HC06_MeansureFinishCallback);
            item = cJSON_GetObjectItem(cmd, "status");
            if (item == NULL) {
                goto PARAM_ERROR;
            }
            cJSON_AddItemToObject(rtn, "status", cJSON_CreateNumber(MQ3_SetContinuousMeasure(item->valueint)));
        } break;
        case 7: { //设置自动回传
            item = cJSON_GetObjectItem(cmd, "status");
            if (item == NULL) {
                goto PARAM_ERROR;
            }
            cJSON_AddItemToObject(rtn, "status", cJSON_CreateNumber(MQ3_SetAutoFeedback(item->valueint)));
        } break;
        case 8: { //设置传感器不断电
            item = cJSON_GetObjectItem(cmd, "status");
            if (item == NULL) {
                goto PARAM_ERROR;
            }
            cJSON_AddItemToObject(rtn, "status", cJSON_CreateNumber(MQ3_SetSensorAlwaysOn(item->valueint)));
        } break;
        case 9: { //设置时间
            Time_Typedef newTime;
            item = cJSON_GetObjectItem(cmd, "yr");
            if (item == NULL) {
                goto PARAM_ERROR;
            }
            newTime.year = item->valueint;
            item = cJSON_GetObjectItem(cmd, "mon");
            if (item == NULL) {
                goto PARAM_ERROR;
            }
            newTime.month = item->valueint;
            item = cJSON_GetObjectItem(cmd, "day");
            if (item == NULL) {
                goto PARAM_ERROR;
            }
            newTime.day = item->valueint;
            item = cJSON_GetObjectItem(cmd, "hr");
            if (item == NULL) {
                goto PARAM_ERROR;
            }
            newTime.hour = item->valueint;
            item = cJSON_GetObjectItem(cmd, "min");
            if (item == NULL) {
                goto PARAM_ERROR;
            }
            newTime.minute = item->valueint;
            item = cJSON_GetObjectItem(cmd, "sec");
            if (item == NULL) {
                goto PARAM_ERROR;
            }
            newTime.second = item->valueint;
            Timing_SetTime(&newTime);
        } break;
        case 10: { //获取最后一次温湿度
            cJSON_AddItemToObject(rtn, "temp", cJSON_CreateNumber(DHT11_GetLastResult().temperature));
            cJSON_AddItemToObject(rtn, "humi", cJSON_CreateNumber(DHT11_GetLastResult().humidity));
        } break;
        case 11: { //强制刷新温湿度
            DHT11_Update();
            cJSON_AddItemToObject(rtn, "status", cJSON_CreateNumber(1));
        } break;
        default: { //命令不存在
            rtnInFunction = false;
            HC06_RtnErr(HC06_ERR_INVALID_COMMAND);
            break;
        }
    }
    if (rtnInFunction) {
        HC06_RtnHandler(rtn);
    }
    cJSON_Delete(rtn); //释放
    return;
PARAM_ERROR:           //参数错误
    cJSON_Delete(rtn); //释放
    HC06_RtnErr(HC06_ERR_PARAM_ERROR);
    return;
}

/**
 * @brief 解析命令函数
 *
 * @param JSONStr JSON字符串，详见命令表
 */
void HC06_CmdDecoder(char *JSONStr) {
    cJSON *cmd;
    cmd = cJSON_Parse(JSONStr); //此时命令缓冲区已经无用，可以给返回值使用了
    if (!cmd) {                 //无法解析字串
        goto COMMAND_FORMAT_ERROR;
    }
    HC06_CmdHandler(cmd);
    cJSON_Delete(cmd);
    return;
COMMAND_FORMAT_ERROR:
    cJSON_Delete(cmd);
    HC06_RtnErr(HC06_ERR_COMMAND_FORMAT_ERROR);
    return;
}

/**
 * @brief 捕获命令函数
 *
 */
void HC06_CmdCatcher() {
    //从0开始，接收到一个左大括号+1，右大括号-1，再次回到0时说明接收到一个完整JSON字串
    static uint8_t bracketDepth = 0;
    //是否在接收命令
    static bool cmdCatching = false;
    // USART读出的一字节
    uint8_t USARTReadByte;
    //命令缓冲区末位置下标
    static unsigned int cmdBufferEndPtr = 0;
    //重置命令缓冲区计数
    static uint8_t resetCount = 0;
    while (USART_DataAvaliable(HC06_USARTPeriph)) {
        USARTReadByte = USART_Read(HC06_USARTPeriph);
        if (USARTReadByte == '\n') { //重置计数
            resetCount++;
            if (resetCount == 5) {
                cmdCatching = false; //放弃全部接收到的内容
                bracketDepth = 0;
                cmdBufferEndPtr = 0;
            }
        } else {
            resetCount = 0;
        }
        if (USARTReadByte == '{') {
            if (bracketDepth == 0) { //第一个左括号，开始接收命令
                cmdCatching = true;
                cmdBufferEndPtr = 0; //下标指针归零
            }
            bracketDepth++;
        }
        if (cmdCatching) {
            HC06_CmdRtnStrBuffer[cmdBufferEndPtr] = USARTReadByte;
            cmdBufferEndPtr++;
            if (cmdBufferEndPtr == HC06_CMD_RTN_STR_MAX_LENGTH - 1) { //命令过长
                HC06_RtnErr(HC06_ERR_COMMAND_TOO_LONG);
                cmdCatching = false; //放弃全部接收到的内容
                bracketDepth = 0;
                cmdBufferEndPtr = 0;
            }
        }
        if (USARTReadByte == '}') {
            bracketDepth--;
            if (bracketDepth == 0) { //最后一个右括号
                cmdCatching = false;
                HC06_CmdRtnStrBuffer[cmdBufferEndPtr] = '\0'; //添加结束符
                HC06_CmdDecoder(HC06_CmdRtnStrBuffer);        //解析和执行命令
            }
        }
    }
}

/**
 * @brief HC-06处理函数，放在主循环中...
 *
 */
void HC06_Handle() {
    HC06_CmdCatcher();
}

void HC06_Init() {
    USART_Initialize(HC06_USARTPeriph, 9600, USART_WordLength_8b, USART_Parity_No, USART_StopBits_1, 0, 0);
    USART_Initialize(DEBUG_USARTPeriph, 9600, USART_WordLength_8b, USART_Parity_No, USART_StopBits_1, 0, 0);
    USART_WriteStr(DEBUG_USARTPeriph, "HC06 Debug Start... Waiting for a JSON String from USART2...\r\n");
}
