#ifndef _Show_h_
#define _Show_h_
#include "HAL_Lib.h"
#include "Display.h"
#include "Strings.h"
#include "page.h"
void ShowUsrName(void *pWindow);
void ShowBottomTextBox(void *pWindow);
void ShowWeightData(void *pWindow);
void ShowCommonTextBox(void *pWindow);
void ShowPXTextBox(void *pWindow);
void ShowCommonData(void *pWindow);
void ShowSetGenderData(void *pWindow);
void ShowWifiIcon(void *pWindow);
void ShowUCBVision(void *pWindow);
void ShowMCBVision(void *pWindow);
void ShowMCBType(void *pWindow);
void ShowACCTime(void *pWindow);
void ShowACCDis(void *pWindow);
void ShowACCStairs(void *pWindow);
void ShowACCAgingHours(void *pWindow);
void ShowENGSetRPMData(void *pWindow);
void ShowENGLCBRPMData(void *pWindow);
void ShowENGInclineADCData(void *pWindow);
void ShowCurrentInclineADC(void *pWindow);
void ShowENGLCBECBCount(void *pWindow);
void ShowPasscode(void *pWindow);
void ShowXID(void *pWindow);
void ShowQuickMul(void *pWindow);
void ShowERPWaitTime(void *pWindow);
//void ShowSETTEDRPMData(void *pWindow);
//void ShowVIRTUALRPMData(void *pWindow);
void ShowSETTEDADCData(void *pWindow);
void ShowVIRTUALADCData(void *pWindow);
void ShowENGKeyValue(void *pWindow);
void ShowENG0Number(void *pWindow);

void ShowSETTEDECBData(void *pWindow);
void ShowENG1HeartData(void *pWindow);
void ShowENG3P1Code(void *pWindow);
//void ShowENG4_1ACC_Dis(void *pWindow);
//void ShowENG4_1ACC_Time(void *pWindow);

void ShowENG1ADCTitle(void *pWindow);
void ShowSETTEDPWMData(void *pWindow);
void ShowENGSettedECBCount(void *pWindow);
//title
void ShowMaleTitle(void *pWindow);
void ShowSpeedTitle(void *pWindow);
void ShowInclineTitle(void *pWindow);
void ShowDistanceTitle(void *pWindow);
void ShowCaloriesTitle(void *pWindow);
void ShowResistanceTitle(void *pWindow);
void ShowMetsTitle(void *pWindow);
void ShowRpmTitle(void *pWindow);
void ShowHeartTitle(void *pWindow);
void ShowWattsTitle(void *pWindow);
void ShowPaceTitle(void *pWindow);
void ShowMileTitle(void *pWindow);
void ShowOverSpeedValue(void *pWindow);
void ShowLevelTitle(void *pWindow);
void ShowStairsTitle(void *pWindow);
void ShowSPMTitle(void *pWindow);
void ShowHeightTitle(void *pWindow);
void ShowSPMData(void *pWindow);
void ShowHeightData(void *pWindow);
//data
void ShowTargetHeartData(void *pWindow);
void ShowTargetWattsData(void *pWindow);
void ShowTimeData(void *pWindow);
void ShowSpeedData(void *pWindow);
void ShowInclineData(void *pWindow);
void ShowDistanceData(void *pWindow);
void ShowCaloriesData(void *pWindow);
void ShowResistanceData(void *pWindow);
void ShowMetsData(void *pWindow);
void ShowWattsData(void *pWindow);
void ShowPaceData(void *pWindow);
void ShowHeartData(void *pWindow);
void ShowLevelData(void *pWindow);
void ShowEngMaxLevelData(void *pWindow);
void ShowEngSetControlZoneData(void *pWindow);
void ShowEng1StControlZoneData(void *pWindow);
void ShowEng2NdControlZoneData(void *pWindow);
void ShowEngRealControlZoneData(void *pWindow);
void ShowEngIrSensorData(void *pWindow);
void ShowRpmData(void *pWindow);
void ShowTimezoneData(void *pWindow);
void ShowWeightUnit(void *pWindow);
void ShowAgeData(void *pWindow);
void ShowStairsData(void *pWindow);
void ShowClimbMillLevelData(void *pWindow);

//matrix
void ShowMatrixData(void *pWindow);
//RF icon
void ShowRFICON(void *pWindow);
void ShowWiFiUpdateProcess(void *pWindow);
void ShowErrorCode(void *pWindow);
void ShowUpGradeLanguagePackage(void *pWindow);
#endif
