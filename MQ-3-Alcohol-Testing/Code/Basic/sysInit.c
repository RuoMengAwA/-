#include "sysInit.h"

#include "cJSON.h"
#include "MemoryManage.h"

void sys_Init(void){
    MeM_Init();
    cJSON_Hooks cJsonHooksInitData;
    cJsonHooksInitData.malloc_fn = MeM_Request;
    cJsonHooksInitData.free_fn = MeM_Release;
    cJSON_InitHooks(&cJsonHooksInitData);
}
