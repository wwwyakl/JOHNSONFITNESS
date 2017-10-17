#ifndef _AUTO_MODEL_H_
#define _AUTO_MODEL_H_

#include "Global_Config.h"

typedef enum
{
    MODEL_EVENT_NONE=0,
    MODEL_FIRST_DISPLAY,
    MODEL_EVENT_MODEL_CHANGED,
    MODEL_EVENT_LANGUAGE_CHANGE,
    MODEL_EVENT_UNIT_CHANGE,
    MODEL_EVENT_UNSUPPORTED_MCB_FOUND,
    MODEL_EVENT_COMPLETE,
    ALL_MODEL_EVENT   
}MODEL_EVENT;


void Model_Initial_Data(const MODEL_DEFAULTS *pModelDefaults, UINT8 totalModels, const UINT8 *pLang, UINT8 totalLang);
void Model_Set_VirtualKey(UINT16 vk);
UINT8 Model_Get_Set_Slot(void);
UINT8 Model_Get_Language_Index(void);
MODEL_EVENT Model_Process(void);

#endif