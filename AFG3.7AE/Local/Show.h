#ifndef _Show_h_
#define _Show_h_

#include "Display.h"
#include "LED.h"
#include "Strings.h"
#include "page.h"
void ShowUsrName(void *pWindow);
void ShowBottomTextBox(void *pWindow);
void ShowWeightData(void *pWindow);
void ShowCommonTextBox(void *pWindow);
void ShowSetTime(void *pWindow);
void ShowSetTotalTime(void *pWindow);
void ShowSetSpeed(void *pWindow);
void ShowSetIncline(void *pWindow);
void ShowCommonData(void *pWindow);
void ShowSetGenderData(void *pWindow);
void ShowWifiIcon(void *pWindow);
void ShowUCBVision(void *pWindow);
void ShowMCBVision(void *pWindow);
void ShowACCTime(void *pWindow);
void ShowACCDis(void *pWindow);
void ShowMacAddress(void *pWindow);
void ShowBTVersion(void *pWindow);
void ShowBTUpdatePercent(void *pWindow);    //zsf

void ShowENGSpeedData(void *pWindow);
void ShowENGSetRPMData(void *pWindow);
void ShowENGLCBRPMData(void *pWindow);
void ShowENGInclineADCData(void *pWindow);
void ShowENGLCBECBCount(void *pWindow);
void ShowPasscode(void *pWindow);
void ShowXID(void *pWindow);
void ShowQuickMul(void *pWindow);
void ShowERPWaitTime(void *pWindow);
void ShowSettedRPM(void *pWindow);
//void ShowSETTEDRPMData(void *pWindow);
//void ShowVIRTUALRPMData(void *pWindow);
void ShowSETTEDADCData(void *pWindow);
void ShowVIRTUALADCData(void *pWindow);

void ShowSETTEDECBData(void *pWindow);
void ShowENG1HeartData(void *pWindow);
void ShowENG3P1Code(void *pWindow);
//void ShowENG4_1ACC_Dis(void *pWindow);
//void ShowENG4_1ACC_Time(void *pWindow);
void ShowENGKeyValue(void *pWindow);
void ShowENG0Number(void *pWindow);
void ShowENG0LedNumber(void *pWindow);

void ShowSafekeyOffLedNumber(void *pWindow);  //zsf

void ShowENG1ADCTitle(void *pWindow);
void ShowSETTEDPWMData(void *pWindow);
void ShowENGSettedECBCount(void *pWindow);
//title
void ShowAFGLOGOTitle(void *pWindow);
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
//void LED_SET_DOT(char by_Data);
//data
//void ShowProgramBLData(void *pWindow);
void ShowTargetHeartData(void *pWindow);
void ShowTargetWattsData(void *pWindow);
void ShowCountTimeData(void *pWindow);
//  void ShowElapsedTimeData(void *pWindow);

void ShowSpeedData(void *pWindow);
void ShowInclineData(void *pWindow);
void ShowIncline_LCDData(void *pWindow);
void ShowDistanceData(void *pWindow);
void ShowCaloriesData(void *pWindow);
void Show_Calories_PerHour(void *pWindow);
void ShowResistanceData(void *pWindow);
void ShowMetsData(void *pWindow);
void ShowWattsData(void *pWindow);
void ShowPaceData(void *pWindow);
void ShowPace_LcdData(void *pWindow);
void ShowHeartData(void *pWindow);
void ShowSetLevel(void *pWindow);
void ShowRpmData(void *pWindow);
void ShowTimezoneData(void *pWindow);
void ShowWeightUnit(void *pWindow);
void ShowAgeData(void *pWindow);
//matrix
void ShowMatrixData(void *pWindow);
//RF icon
void ShowRFICON(void *pWindow);
void ShowSetDistance(void *pWindow);
void showWeekWorkout(void *pWindow);
void ShowCustomSegment(void *pWindow);
void ShowSetResistance(void *pWindow);
void Show_Laps(void *pWindow);
void Show_LapsCircle(void *pWindow);
void ShowRTCYear(void *pWindow);
void ShowSetCalories(void *pWindow);
void ShowSelectDistance(void *pWindow);
void ShowLapsCircle(void *pWindow);
void ShowRTCMonth(void *pWindow);
void ShowRTCDate(void *pWindow);
void ShowRTCHour(void *pWindow);
void ShowRTCMinute(void *pWindow);
void ShowRTCAMPM(void *pWindow);
// void ShowLcdElapsedTimeData(void *pWindow);

void ShowLcdDistanceData(void *pWindow);
void ShowLcdCaloriesData(void *pWindow);
void ShowLcdSpeedData(void *pWindow);
void Show_LEDLight(LedType Index,LED_STATUS ON_OFF);
void Show_ClearLedLight(void);
void ShowTimeData(void *pWindow);//WWW
#endif
