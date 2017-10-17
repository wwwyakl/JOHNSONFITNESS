#include "Strings.h"
#include "SystemConfig.h"
#include  "EEPRom_Language.h"
#include "USBManager.h"
__no_init static LANGUAGEID by_languageID ;
extern unsigned char Lang_StringBuffer[_TheWordsOfOneString_];
const UCHAR* const Strings[STRINGIDEND][LANGUAGEIDEND]=
{	
    {
		"female",
        #ifdef __LANGUAGE_SPANISH__
        "Mujer",
        #endif
        #ifdef __LANGUAGE_FRENCH__
        "Femme",
        #endif
        #ifdef __LANGUAGE_ITALIAN__
        "Femmina",
        #endif
        #ifdef __LANGUAGE_GERMAN__
        "Weiblich",
        #endif
        #ifdef __LANGUAGE_DUTCH__
        "Vrouw",
        #endif
        #ifdef __LANGUAGE_PORTUGUESE__
        "Feminino",
        #endif
        #ifdef __LANGUAGE_VIETNAMESE__
        "Nu",
        #endif
    },//STR_FEMALE_ID
    
    {
		"       go   ",
#ifdef __LANGUAGE_SPANISH__
"       Ir   ",
#endif
#ifdef __LANGUAGE_FRENCH__
"       Go   ",
#endif
#ifdef __LANGUAGE_ITALIAN__
"       Go   ",
#endif
#ifdef __LANGUAGE_GERMAN__
"      Los   ",
#endif
#ifdef __LANGUAGE_DUTCH__
"    Start  ",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"  Iniciar ",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"       Di   ",
#endif
    },//STR_GO_ID,
    
    {
		"km",
#ifdef __LANGUAGE_SPANISH__
"km",
#endif
#ifdef __LANGUAGE_FRENCH__
"km",
#endif
#ifdef __LANGUAGE_ITALIAN__
"km",
#endif
#ifdef __LANGUAGE_GERMAN__
"km",
#endif
#ifdef __LANGUAGE_DUTCH__
"km",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"km",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"km",
#endif
    },//STR_KM_ID,
    //to do 
    {
		"STAIRS",
        
    },//STR_STAIRS_ID
    {
		"SPM",
        
    },//STR_SPM_ID
    {
		"HEIGHT",
        
    },//STR_HEIGHT_ID
    
    {
		"LEVEL",
#ifdef __LANGUAGE_SPANISH__
"Nivel",
#endif
#ifdef __LANGUAGE_FRENCH__
"Niveau",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Livello",
#endif
#ifdef __LANGUAGE_GERMAN__
"Level",
#endif
#ifdef __LANGUAGE_DUTCH__
"Niveau",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Nivel",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Trinh Do",
#endif
    },//STR_LEVEL_ID
    
    
    {
		"METRIC",
#ifdef __LANGUAGE_SPANISH__
"Metrico",
#endif
#ifdef __LANGUAGE_FRENCH__
"Metrique",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Metrico",
#endif
#ifdef __LANGUAGE_GERMAN__
"Metrisch",
#endif
#ifdef __LANGUAGE_DUTCH__
"Metriek",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Metrica",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"He Met",
#endif
    },//STR_METRIC_ID,
    
    {
		"mets",
#ifdef __LANGUAGE_SPANISH__
"METS",
#endif
#ifdef __LANGUAGE_FRENCH__
"METS",
#endif
#ifdef __LANGUAGE_ITALIAN__
"MET",
#endif
#ifdef __LANGUAGE_GERMAN__
"METS",
#endif
#ifdef __LANGUAGE_DUTCH__
"METS",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"METS",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"MeT",
#endif
    },//STR_METS_ID,
    
    {
		"MILES",
#ifdef __LANGUAGE_SPANISH__
"Millas",
#endif
#ifdef __LANGUAGE_FRENCH__
"Miles",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Miglia",
#endif
#ifdef __LANGUAGE_GERMAN__
"Meilen",
#endif
#ifdef __LANGUAGE_DUTCH__
"Mijl",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Milhas",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"DaM",
#endif
    },//STR_MILES_ID,
    
    {
		"rpm",
#ifdef __LANGUAGE_SPANISH__
"RPM",
#endif
#ifdef __LANGUAGE_FRENCH__
"TR/MIN",
#endif
#ifdef __LANGUAGE_ITALIAN__
"GIRI/M",
#endif
#ifdef __LANGUAGE_GERMAN__
"UPM",
#endif
#ifdef __LANGUAGE_DUTCH__
"RPM",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"RPM",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"RPM",
#endif
    },//STR_RPM_ID,
    
    {
		"  sprint 1",
#ifdef __LANGUAGE_SPANISH__
"  sprint 1",
#endif
#ifdef __LANGUAGE_FRENCH__
"  sprint 1",
#endif
#ifdef __LANGUAGE_ITALIAN__
"  sprint 1",
#endif
#ifdef __LANGUAGE_GERMAN__
"  sprint 1",
#endif
#ifdef __LANGUAGE_DUTCH__
"  sprint 1",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"  sprint 1",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"  sprint 1",
#endif
    },//STR_SPRINT1_ID
    
    {
		"  sprint 2",
#ifdef __LANGUAGE_SPANISH__
"  sprint 2",
#endif
#ifdef __LANGUAGE_FRENCH__
"  sprint 2",
#endif
#ifdef __LANGUAGE_ITALIAN__
"  sprint 2",
#endif
#ifdef __LANGUAGE_GERMAN__
"  sprint 2",
#endif
#ifdef __LANGUAGE_DUTCH__
"  sprint 2",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"  sprint 2",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"  sprint 2",
#endif
    },//STR_SPRINT2_ID
    
    {
		"  sprint 3",
#ifdef __LANGUAGE_SPANISH__
"  sprint 3",
#endif
#ifdef __LANGUAGE_FRENCH__
"  sprint 3",
#endif
#ifdef __LANGUAGE_ITALIAN__
"  sprint 3",
#endif
#ifdef __LANGUAGE_GERMAN__
"  sprint 3",
#endif
#ifdef __LANGUAGE_DUTCH__
"  sprint 3",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"  sprint 3",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"  sprint 3",
#endif
    },//STR_SPRINT3_ID,
    
    {
		"  sprint 4",
#ifdef __LANGUAGE_SPANISH__
"  sprint 4",
#endif
#ifdef __LANGUAGE_FRENCH__
"  sprint 4",
#endif
#ifdef __LANGUAGE_ITALIAN__
"  sprint 4",
#endif
#ifdef __LANGUAGE_GERMAN__
"  sprint 4",
#endif
#ifdef __LANGUAGE_DUTCH__
"  sprint 4",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"  sprint 4",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"  sprint 4",
#endif
    },//STR_SPRINT4_ID
    
    {
		"  sprint 5",
#ifdef __LANGUAGE_SPANISH__
"  sprint 5",
#endif
#ifdef __LANGUAGE_FRENCH__
"  sprint 5",
#endif
#ifdef __LANGUAGE_ITALIAN__
"  sprint 5",
#endif
#ifdef __LANGUAGE_GERMAN__
"  sprint 5",
#endif
#ifdef __LANGUAGE_DUTCH__
"  sprint 5",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"  sprint 5",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"  sprint 5",
#endif
    },//STR_SPRINT5_ID
    
    {
		"  sprint 6",
#ifdef __LANGUAGE_SPANISH__
"  sprint 6",
#endif
#ifdef __LANGUAGE_FRENCH__
"  sprint 6",
#endif
#ifdef __LANGUAGE_ITALIAN__
"  sprint 6",
#endif
#ifdef __LANGUAGE_GERMAN__
"  sprint 6",
#endif
#ifdef __LANGUAGE_DUTCH__
"  sprint 6",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"  sprint 6",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"  sprint 6",
#endif
    },//STR_SPRINT6_ID
    
    {
		"  sprint 7",
#ifdef __LANGUAGE_SPANISH__
"  sprint 7",
#endif
#ifdef __LANGUAGE_FRENCH__
"  sprint 7",
#endif
#ifdef __LANGUAGE_ITALIAN__
"  sprint 7",
#endif
#ifdef __LANGUAGE_GERMAN__
"  sprint 7",
#endif
#ifdef __LANGUAGE_DUTCH__
"  sprint 7",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"  sprint 7",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"  sprint 7",
#endif
    },//STR_SPRINT7_ID
    
    {
		"  sprint 8",
#ifdef __LANGUAGE_SPANISH__
" sprint 8",
#endif
#ifdef __LANGUAGE_FRENCH__
" sprint 8",
#endif
#ifdef __LANGUAGE_ITALIAN__
" sprint 8",
#endif
#ifdef __LANGUAGE_GERMAN__
" sprint 8",
#endif
#ifdef __LANGUAGE_DUTCH__
" sprint 8",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
" sprint 8",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
" sprint 8",
#endif
    },//STR_SPRINT8_ID
    {
		" LANDMARKS ",
#ifdef __LANGUAGE_SPANISH__
" LANDMARKS ",
#endif
#ifdef __LANGUAGE_FRENCH__
" LANDMARKS ",
#endif
#ifdef __LANGUAGE_ITALIAN__
" LANDMARKS ",
#endif
#ifdef __LANGUAGE_GERMAN__
" LANDMARKS ",
#endif
#ifdef __LANGUAGE_DUTCH__
" LANDMARKS ",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
" LANDMARKS ",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
" LANDMARKS ",
#endif
    },//STR_LANDMARK_ID    
    {
		"Recovery 1",
#ifdef __LANGUAGE_SPANISH__
"Recovery 1",
#endif
#ifdef __LANGUAGE_FRENCH__
"Recovery 1",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Recovery 1",
#endif
#ifdef __LANGUAGE_GERMAN__
"Recovery 1",
#endif
#ifdef __LANGUAGE_DUTCH__
"Recovery 1",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Recovery 1",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Recovery 1",
#endif
    },//STR_RECOVERY1_ID	
    {
		"Recovery 2",
#ifdef __LANGUAGE_SPANISH__
"Recovery 2",
#endif
#ifdef __LANGUAGE_FRENCH__
"Recovery 2",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Recovery 2",
#endif
#ifdef __LANGUAGE_GERMAN__
"Recovery 2",
#endif
#ifdef __LANGUAGE_DUTCH__
"Recovery 2",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Recovery 2",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Recovery 2",
#endif
    },//STR_RECOVERY2_ID
    {
		"Recovery 3",
#ifdef __LANGUAGE_SPANISH__
"Recovery 3",
#endif
#ifdef __LANGUAGE_FRENCH__
"Recovery 3",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Recovery 3",
#endif
#ifdef __LANGUAGE_GERMAN__
"Recovery 3",
#endif
#ifdef __LANGUAGE_DUTCH__
"Recovery 3",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Recovery 3",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Recovery 3",
#endif
    },//STR_RECOVERY3_ID	
    {
		"Recovery 4",
#ifdef __LANGUAGE_SPANISH__
"Recovery 4",
#endif
#ifdef __LANGUAGE_FRENCH__
"Recovery 4",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Recovery 4",
#endif
#ifdef __LANGUAGE_GERMAN__
"Recovery 4",
#endif
#ifdef __LANGUAGE_DUTCH__
"Recovery 4",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Recovery 4",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Recovery 4",
#endif
    },//STR_RECOVERY4_ID	
    {
		"Recovery 5",
#ifdef __LANGUAGE_SPANISH__
"Recovery 5",
#endif
#ifdef __LANGUAGE_FRENCH__
"Recovery 5",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Recovery 5",
#endif
#ifdef __LANGUAGE_GERMAN__
"Recovery 5",
#endif
#ifdef __LANGUAGE_DUTCH__
"Recovery 5",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Recovery 5",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Recovery 5",
#endif
    },//STR_RECOVERY5_ID	
    {
		"Recovery 6",
#ifdef __LANGUAGE_SPANISH__
"Recovery 6",
#endif
#ifdef __LANGUAGE_FRENCH__
"Recovery 6",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Recovery 6",
#endif
#ifdef __LANGUAGE_GERMAN__
"Recovery 6",
#endif
#ifdef __LANGUAGE_DUTCH__
"Recovery 6",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Recovery 6",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Recovery 6",
#endif
    },//STR_RECOVERY6_ID	
    {
		"Recovery 7",
#ifdef __LANGUAGE_SPANISH__
"Recovery 7",
#endif
#ifdef __LANGUAGE_FRENCH__
"Recovery 7",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Recovery 7",
#endif
#ifdef __LANGUAGE_GERMAN__
"Recovery 7",
#endif
#ifdef __LANGUAGE_DUTCH__
"Recovery 7",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Recovery 7",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Recovery 7",
#endif
    },//STR_RECOVERY7_ID	
    {
		"watts",
#ifdef __LANGUAGE_SPANISH__
"Vatios",
#endif
#ifdef __LANGUAGE_FRENCH__
"Watts",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Watt",
#endif
#ifdef __LANGUAGE_GERMAN__
"Watt",
#endif
#ifdef __LANGUAGE_DUTCH__
"Watt",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Watts",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Watt",
#endif
    },//STR_WATTS_ID
    
    
    {
		"mile",
#ifdef __LANGUAGE_SPANISH__
"MILLA",
#endif
#ifdef __LANGUAGE_FRENCH__
"MILES",
#endif
#ifdef __LANGUAGE_ITALIAN__
"MIGLIA",
#endif
#ifdef __LANGUAGE_GERMAN__
"MEILE",
#endif
#ifdef __LANGUAGE_DUTCH__
"MIJL",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"milha",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"dam",
#endif
    },//STR_MILE_ID,
    
    {
		"  warm up",
#ifdef __LANGUAGE_SPANISH__
"calentam.",
#endif
#ifdef __LANGUAGE_FRENCH__
"echauff.",
#endif
#ifdef __LANGUAGE_ITALIAN__
"riscald.",
#endif
#ifdef __LANGUAGE_GERMAN__
"aufwarmen",
#endif
#ifdef __LANGUAGE_DUTCH__
"opwarmen",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"   aquecer ",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"khoi dong",
#endif
    },//STR_WARMUP_ID,
    {
		"    ramp up  ",
#ifdef __LANGUAGE_SPANISH__
"Preparado",
#endif
#ifdef __LANGUAGE_FRENCH__
"    pret  ",
#endif
#ifdef __LANGUAGE_ITALIAN__
"  pronto ",
#endif
#ifdef __LANGUAGE_GERMAN__
"  bereit ",
#endif
#ifdef __LANGUAGE_DUTCH__
"   klaar  ",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"  pronto ",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"san sang",
#endif
    },//STR_RAMPUP_ID,    
    {
		"    ready  ",
#ifdef __LANGUAGE_SPANISH__
"Preparado",
#endif
#ifdef __LANGUAGE_FRENCH__
"    pret  ",
#endif
#ifdef __LANGUAGE_ITALIAN__
"  pronto ",
#endif
#ifdef __LANGUAGE_GERMAN__
"  bereit ",
#endif
#ifdef __LANGUAGE_DUTCH__
"   klaar  ",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"  pronto ",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"san sang",
#endif
    },//STR_READY_ID,
    {
		"      set   ",
#ifdef __LANGUAGE_SPANISH__
"    Definir ",
#endif
#ifdef __LANGUAGE_FRENCH__
"   Paramet. ",
#endif
#ifdef __LANGUAGE_ITALIAN__
"   Imposta  ",
#endif
#ifdef __LANGUAGE_GERMAN__
"   fertig   ",
#endif
#ifdef __LANGUAGE_DUTCH__
"  Instellen ",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"  Definir   ",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"   Cai dat  ",
#endif
    },//STR_SET_ID,
    {
		"HR",
#ifdef __LANGUAGE_SPANISH__
"FC",
#endif
#ifdef __LANGUAGE_FRENCH__
"FC",
#endif
#ifdef __LANGUAGE_ITALIAN__
"BC",
#endif
#ifdef __LANGUAGE_GERMAN__
"HR",
#endif
#ifdef __LANGUAGE_DUTCH__
"SNELHEID",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"HR",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Nhan Su",
#endif
    },//STR_HR_ID,
    
    {
		"male",
#ifdef __LANGUAGE_SPANISH__
"Hombre",
#endif
#ifdef __LANGUAGE_FRENCH__
"Homme",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Maschio",
#endif
#ifdef __LANGUAGE_GERMAN__
"Mannlich",
#endif
#ifdef __LANGUAGE_DUTCH__
"Man",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Masculino",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Nam",
#endif
    },//STR_MALE_ID,
    
    {
		"mph",
#ifdef __LANGUAGE_SPANISH__
"mph",
#endif
#ifdef __LANGUAGE_FRENCH__
"mph",
#endif
#ifdef __LANGUAGE_ITALIAN__
"mph",
#endif
#ifdef __LANGUAGE_GERMAN__
"mph",
#endif
#ifdef __LANGUAGE_DUTCH__
"mijl/u",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"mph",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"MPH",//"dam/gio",
#endif
    },//STR_MPH_ID
    
	{
		"kph",
#ifdef __LANGUAGE_SPANISH__
"kph",
#endif
#ifdef __LANGUAGE_FRENCH__
"kph",
#endif
#ifdef __LANGUAGE_ITALIAN__
"kph",
#endif
#ifdef __LANGUAGE_GERMAN__
"kph",
#endif
#ifdef __LANGUAGE_DUTCH__
"kijl/u",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"kph",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"KPH",//"dak/gio",
#endif
    },//STR_KPH_ID
    
    {
		"pace",
#ifdef __LANGUAGE_SPANISH__
"RITMO",
#endif
#ifdef __LANGUAGE_FRENCH__
"CADENCE",
#endif
#ifdef __LANGUAGE_ITALIAN__
"RITMO",
#endif
#ifdef __LANGUAGE_GERMAN__
"TEMPO",
#endif
#ifdef __LANGUAGE_DUTCH__
"TEMPO",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"RITMO",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"NHIP TIM",
#endif
    },//STR_PACE_ID,
    
    {
		"calories",
#ifdef __LANGUAGE_SPANISH__
"Calorias",
#endif
#ifdef __LANGUAGE_FRENCH__
"Calories",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Calorie",
#endif
#ifdef __LANGUAGE_GERMAN__
"Kalorien",
#endif
#ifdef __LANGUAGE_DUTCH__
"Calori.",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Calorias",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Calorie",
#endif
    },//STR_CALORIES_ID
    
    {
		"COOL DOWN",
#ifdef __LANGUAGE_SPANISH__
"RECUP.",
#endif
#ifdef __LANGUAGE_FRENCH__
"RECUP.",
#endif
#ifdef __LANGUAGE_ITALIAN__
"DEFATIC.",
#endif
#ifdef __LANGUAGE_GERMAN__
"ABKUEHLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"AFKOELING",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"RECUPERAR",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"HA NHIET",
#endif
    },//STR_COOLDOWN_ID
    
    {
		"incline",
#ifdef __LANGUAGE_SPANISH__
"inclin.",
#endif
#ifdef __LANGUAGE_FRENCH__
"Inclin.",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Inclin.",
#endif
#ifdef __LANGUAGE_GERMAN__
"Steig.",
#endif
#ifdef __LANGUAGE_DUTCH__
"Helling",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Inclin.",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"incline",
#endif
    },//STR_INCLINE_ID
    
    {
		"RES.",
#ifdef __LANGUAGE_SPANISH__
"resis.",
#endif
#ifdef __LANGUAGE_FRENCH__
"Res.",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Resist.",
#endif
#ifdef __LANGUAGE_GERMAN__
"Widrst.",
#endif
#ifdef __LANGUAGE_DUTCH__
"Weerst.",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Resist.",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"RES.",
#endif
    },//STR_RESISTANCE_ID
    
    {
		"speed",
#ifdef __LANGUAGE_SPANISH__
"Velocidad",
#endif
#ifdef __LANGUAGE_FRENCH__
"Vitesse",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Velocita",
#endif
#ifdef __LANGUAGE_GERMAN__
"Geschwindigkeit",
#endif
#ifdef __LANGUAGE_DUTCH__
"Snelheid",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Velocidade",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Toc Do",
#endif
    },//STR_SPEED_ID
    
    {
		"Distance",
#ifdef __LANGUAGE_SPANISH__
"Distancia",
#endif
#ifdef __LANGUAGE_FRENCH__
"Distance",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Distanza",
#endif
#ifdef __LANGUAGE_GERMAN__
"Distanz",
#endif
#ifdef __LANGUAGE_DUTCH__
"Afstand",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Distancia",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Khoang cach",
#endif
    },//STR_DISTANCE_ID
    
    {
		"rolling hills",
#ifdef __LANGUAGE_SPANISH__
"Escalada MontaNa",
#endif
#ifdef __LANGUAGE_FRENCH__
"Mode Cotes",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Percorso Collinare",
#endif
#ifdef __LANGUAGE_GERMAN__
"Hugel",
#endif
#ifdef __LANGUAGE_DUTCH__
"Heuvels",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Alpinismo",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Rolling Hills",
#endif
    },//STR_ROLLINGHILLS_ID
    
    {
		"pause",
#ifdef __LANGUAGE_SPANISH__
"PAUSA",
#endif
#ifdef __LANGUAGE_FRENCH__
"PAUSE",
#endif
#ifdef __LANGUAGE_ITALIAN__
"PAUSA",
#endif
#ifdef __LANGUAGE_GERMAN__
"PAUSE",
#endif
#ifdef __LANGUAGE_DUTCH__
"PAUZE",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"PAUSA",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"TAM DUNG",
#endif
    },//STR_PAUSE_ID
    
    {
		"Please Wait",
#ifdef __LANGUAGE_SPANISH__
"Por favor espere",
#endif
#ifdef __LANGUAGE_FRENCH__
"Attendez",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Attendere Prego",
#endif
#ifdef __LANGUAGE_GERMAN__
"Bitte warten",
#endif
#ifdef __LANGUAGE_DUTCH__
"Moment a.u.b.",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Por favor aguarde",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Vui long cho",
#endif
    },//STR_PLEASE_WAIT_ID
    
    {
		"THR",
#ifdef __LANGUAGE_SPANISH__
"RCO",
#endif
#ifdef __LANGUAGE_FRENCH__
"FC",
#endif
#ifdef __LANGUAGE_ITALIAN__
"THR",
#endif
#ifdef __LANGUAGE_GERMAN__
"ZHF",
#endif
#ifdef __LANGUAGE_DUTCH__
"THR",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"FQD",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"THR",
#endif
    },//STR_THR_ID
    
    {
		"TIME",
#ifdef __LANGUAGE_SPANISH__
"Tiempo",
#endif
#ifdef __LANGUAGE_FRENCH__
"Temps",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Tempo",
#endif
#ifdef __LANGUAGE_GERMAN__
"Zeit",
#endif
#ifdef __LANGUAGE_DUTCH__
"Tijd",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Tempo",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Thoi Gian",
#endif
    },//STR_TIME_ID
    
    {
		"TREADMILL",
#ifdef __LANGUAGE_SPANISH__
"Cinta De Correr",
#endif
#ifdef __LANGUAGE_FRENCH__
"Tapis De Course",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Treadmill",
#endif
#ifdef __LANGUAGE_GERMAN__
"Laufband",
#endif
#ifdef __LANGUAGE_DUTCH__
"Lopband",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Esteira",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"May Chay",
#endif
    },//STR_TREADMILL_ID,
    {
		"ELLIPTICAL",
#ifdef __LANGUAGE_SPANISH__
"Eliptica",
#endif
#ifdef __LANGUAGE_FRENCH__
"Elliptique",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Elliptical",
#endif
#ifdef __LANGUAGE_GERMAN__
"Elliptical",
#endif
#ifdef __LANGUAGE_DUTCH__
"Elliptical",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Eliptico",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"elip",
#endif
    },//STR_ELLIPTICAL_ID
    
    
    {
		"BIKE",
#ifdef __LANGUAGE_SPANISH__
"Bicicleta",
#endif
#ifdef __LANGUAGE_FRENCH__
"Velo",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Bicicletta",
#endif
#ifdef __LANGUAGE_GERMAN__
"Ergometer",
#endif
#ifdef __LANGUAGE_DUTCH__
"Fiets",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"BIKE",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"BIKE",
#endif
    },//STR_BIKE_ID,
    {
		"ASCENT",
#ifdef __LANGUAGE_SPANISH__
"ASCENT",
#endif
#ifdef __LANGUAGE_FRENCH__
"MONTeE",
#endif
#ifdef __LANGUAGE_ITALIAN__
"ASCENT",
#endif
#ifdef __LANGUAGE_GERMAN__
"ASCENT",
#endif
#ifdef __LANGUAGE_DUTCH__
"ASCENT",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"SUBIDA",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"TANG",
#endif
    },//STR_ASCENT_ID
{
		"STEPPER",
#ifdef __LANGUAGE_SPANISH__
"STEPPER",
#endif
#ifdef __LANGUAGE_FRENCH__
"STEPPER",
#endif
#ifdef __LANGUAGE_ITALIAN__
"STEPPER",
#endif
#ifdef __LANGUAGE_GERMAN__
"STEPPER",
#endif
#ifdef __LANGUAGE_DUTCH__
"STEPPER",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"STEPPER",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"STEPPER",
#endif
    },//STR_STEPPER_ID    
{
		"CLIMBMILL",
#ifdef __LANGUAGE_SPANISH__
"CLIMBMILL",
#endif
#ifdef __LANGUAGE_FRENCH__
"CLIMBMILL",
#endif
#ifdef __LANGUAGE_ITALIAN__
"CLIMBMILL",
#endif
#ifdef __LANGUAGE_GERMAN__
"CLIMBMILL",
#endif
#ifdef __LANGUAGE_DUTCH__
"CLIMBMILL",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"CLIMBMILL",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"CLIMBMILL",
#endif
    },//STR_CLIMBMILL_ID    
    {
		"Welcome",
#ifdef __LANGUAGE_SPANISH__
"Bienvenido",
#endif
#ifdef __LANGUAGE_FRENCH__
"Bienvenue",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Benvenuto",
#endif
#ifdef __LANGUAGE_GERMAN__
"Willkommen",
#endif
#ifdef __LANGUAGE_DUTCH__
"Welkom",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Bem-Vindo",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Chao Mung Ban",
#endif
    },//STR_WELCOME_ID,
    
    {
		"WORKOUT PAUSED",
#ifdef __LANGUAGE_SPANISH__
"ENTRENAMIENTO EN PAUSA",
#endif
#ifdef __LANGUAGE_FRENCH__
"ENTRAINEMENT INTERROMPU",
#endif
#ifdef __LANGUAGE_ITALIAN__
"ALLENAMENTO IN PAUSA",
#endif
#ifdef __LANGUAGE_GERMAN__
"TRAINING UNTERBROCHEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"TRAININGSPAUZE",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"PAUSA NO EXERCiCIO",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"TAM DUNG TAP",
#endif
    },//STR_WORKOUT_PAUSED_ID
    
    {
		"XID",
#ifdef __LANGUAGE_SPANISH__
"xID",
#endif
#ifdef __LANGUAGE_FRENCH__
"xID",
#endif
#ifdef __LANGUAGE_ITALIAN__
"xID",
#endif
#ifdef __LANGUAGE_GERMAN__
"xID",
#endif
#ifdef __LANGUAGE_DUTCH__
"xID",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"xID",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"xID",
#endif
    },//STR_XID_ID,
    
    {
		"Activated",
#ifdef __LANGUAGE_SPANISH__
"Activated",
#endif
#ifdef __LANGUAGE_FRENCH__
"Activated",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Activated",
#endif
#ifdef __LANGUAGE_GERMAN__
"Activated",
#endif
#ifdef __LANGUAGE_DUTCH__
"Activated",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Activated",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Da kich hoat",
#endif
    },//STR_ACTIVATED_ID,
    
    {
		"DELETE USER",
#ifdef __LANGUAGE_SPANISH__
"Eliminar usuario",
#endif
#ifdef __LANGUAGE_FRENCH__
"Supprimer l'utilisateur",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Cancella Utente",
#endif
#ifdef __LANGUAGE_GERMAN__
"Benutzer loschen",
#endif
#ifdef __LANGUAGE_DUTCH__
"NAAM INVOEREN",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"EXCLUIR USUaRIO",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"XoA NGUOI DUNG",
#endif
    },//STR_DELETE_USER_ID
    
    {
		"English",
#ifdef __LANGUAGE_SPANISH__
"English",
#endif
#ifdef __LANGUAGE_FRENCH__
"English",
#endif
#ifdef __LANGUAGE_ITALIAN__
"English",
#endif
#ifdef __LANGUAGE_GERMAN__
"English",
#endif
#ifdef __LANGUAGE_DUTCH__
"English",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"English",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"English",
#endif
    },//STR_ENGLISH_ID,
    
    {
		"SPANISH",
#ifdef __LANGUAGE_SPANISH__
"ESPANOL",
#endif
#ifdef __LANGUAGE_FRENCH__
"ESPAGNOL",
#endif
#ifdef __LANGUAGE_ITALIAN__
"SPAGNOLO",
#endif
#ifdef __LANGUAGE_GERMAN__
"SPANISCH",
#endif
#ifdef __LANGUAGE_DUTCH__
"SPAANS",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"ESPANHOL",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"TIENG TAY BAN NHA",
#endif
    },//STR_SPANISH_ID
    
    {
		"French",
#ifdef __LANGUAGE_SPANISH__
"FRANCE",
#endif
#ifdef __LANGUAGE_FRENCH__
"FRANCAIS",
#endif
#ifdef __LANGUAGE_ITALIAN__
"FRANCESE",
#endif
#ifdef __LANGUAGE_GERMAN__
"FRANZOESISCH",
#endif
#ifdef __LANGUAGE_DUTCH__
"FRANS",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Frances",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Tieng Phap",
#endif
    },//STR_FRENCH_ID,
    
    {
		"ITALIAN",
#ifdef __LANGUAGE_SPANISH__
"ITALIANO",
#endif
#ifdef __LANGUAGE_FRENCH__
"ITALIEN",
#endif
#ifdef __LANGUAGE_ITALIAN__
"ITALIANO",
#endif
#ifdef __LANGUAGE_GERMAN__
"ITALIENISCH",
#endif
#ifdef __LANGUAGE_DUTCH__
"ITALIAANS",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"ITALIANO",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"TIENG Y",
#endif
    },//STR_ITALIAN_ID
    
    {
		"German",
#ifdef __LANGUAGE_SPANISH__
"Germany",
#endif
#ifdef __LANGUAGE_FRENCH__
"ALLEMAND",
#endif
#ifdef __LANGUAGE_ITALIAN__
"TEDESCO",
#endif
#ifdef __LANGUAGE_GERMAN__
"DEUTSCH",
#endif
#ifdef __LANGUAGE_DUTCH__
"DUITS",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Alemao",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Tieng Duc",
#endif
    },//STR_GERMAN_ID,
    
    {
		"Dutch",
#ifdef __LANGUAGE_SPANISH__
"Dutch",
#endif
#ifdef __LANGUAGE_FRENCH__
"HOLLANDAIS",
#endif
#ifdef __LANGUAGE_ITALIAN__
"OLANDESE",
#endif
#ifdef __LANGUAGE_GERMAN__
"NIEDERLAENDISCH",
#endif
#ifdef __LANGUAGE_DUTCH__
"NEDERLANDS",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Holandes",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Tieng Ha Lan",
#endif
    },//STR_DUTCH_ID,	
    {
		"PORTUGUESE",
#ifdef __LANGUAGE_SPANISH__
"Portuguese",
#endif
#ifdef __LANGUAGE_FRENCH__
"Portuguese",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Portuguese",
#endif
#ifdef __LANGUAGE_GERMAN__
"Portuguese",
#endif
#ifdef __LANGUAGE_DUTCH__
"Portuguese",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"PORTUGUeS",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"TIENG BO DaO NHA",
#endif
    },//STR_PORTUGUESE_ID
    {
		"VIETNAMESE",
#ifdef __LANGUAGE_SPANISH__
"Vietnamita",
#endif
#ifdef __LANGUAGE_FRENCH__
"Vietnamien",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Vietnamita",
#endif
#ifdef __LANGUAGE_GERMAN__
"Vietnamesisch",
#endif
#ifdef __LANGUAGE_DUTCH__
"Vietnamees",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Vietnamita",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"TIENG VIET",
#endif
    },//STR_VIETNAMESE_ID
    
    {
		"LUBE CHAIN",
#ifdef __LANGUAGE_SPANISH__
"USB NO CONTIENE SOFTWARE",
#endif
#ifdef __LANGUAGE_FRENCH__
"AUCUN LOGICIEL SUR USB",
#endif
#ifdef __LANGUAGE_ITALIAN__
"NESSUN SOFTWARE NELL'USB",
#endif
#ifdef __LANGUAGE_GERMAN__
"KEINE SOFTWARE AUF USB",
#endif
#ifdef __LANGUAGE_DUTCH__
"GEEN SOFTWARE OP USB",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"NAO Ha SOFTWARE NO USB",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"KHoNG Co PHAN MEM BeN TRONG USB",
#endif
    },//STR_CLIMB_MILL_LUBE_BELT_ID,
    
    {
		"PASSPORT READY",
#ifdef __LANGUAGE_SPANISH__
"Passport esta listo.",
#endif
#ifdef __LANGUAGE_FRENCH__
"Passport pret",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Passport pronto",
#endif
#ifdef __LANGUAGE_GERMAN__
"Passport Ready",
#endif
#ifdef __LANGUAGE_DUTCH__
"Passport klaar",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"PASSPORT ESTa PRONTO",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"HO CHIEU SAN SaNG",
#endif
    },//STR_PASSPORT_READY_ID
    
    {
		"press start",
#ifdef __LANGUAGE_SPANISH__
"OPRIMIR START",
#endif
#ifdef __LANGUAGE_FRENCH__
"PRESSER START",
#endif
#ifdef __LANGUAGE_ITALIAN__
"PREMI START",
#endif
#ifdef __LANGUAGE_GERMAN__
"START DRUCKEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"START INDRUKKEN",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"pressione inicio",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"nhan bat dau",
#endif
    },//STR_PRESS_START_ID
    
    {
		"PROGRAMMING...",
#ifdef __LANGUAGE_SPANISH__
"PROGRAMANDO¡­",
#endif
#ifdef __LANGUAGE_FRENCH__
"PROGRAMMER...",
#endif
#ifdef __LANGUAGE_ITALIAN__
"IN PROGRAMMAZIONE",
#endif
#ifdef __LANGUAGE_GERMAN__
"PROGRAMMIEREN",
#endif
#ifdef __LANGUAGE_DUTCH__
"PROGRAMMEREN...",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"PROGRAMANDO...",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"DANG LAP TRINH...",
#endif
    },//STR_PROGRAMMING_ID 
    
    {
		"RF SYNC",
#ifdef __LANGUAGE_SPANISH__
"SINCRONIZACION DE RF",
#endif
#ifdef __LANGUAGE_FRENCH__
"SYNC FR",
#endif
#ifdef __LANGUAGE_ITALIAN__
"SINCRONIZZAZIONE RF",
#endif
#ifdef __LANGUAGE_GERMAN__
"RF SYNC",
#endif
#ifdef __LANGUAGE_DUTCH__
"RF SYNC",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"SINCR RF",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"DONG BO RF",
#endif
    },//STR_RF_SYNC_ID
    
    {
		"SELECT USER",
#ifdef __LANGUAGE_SPANISH__
"SELECCIONAR EL USARIO",
#endif
#ifdef __LANGUAGE_FRENCH__
"SELECTIONNER UTILISATEUR",
#endif
#ifdef __LANGUAGE_ITALIAN__
"IMPOSTA UTENTE",
#endif
#ifdef __LANGUAGE_GERMAN__
"BENUTZER AUSWAHLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"GEBRUIKER KIEZEN",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"SELECIONAR USUaRIO",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"CHON NGUOI DUNG",
#endif
    },//STR_SELECT_USER_ID
    
    {
		"set AGE",
#ifdef __LANGUAGE_SPANISH__
"ESTABLECER EDAD",
#endif
#ifdef __LANGUAGE_FRENCH__
"ENTRER AGE",
#endif
#ifdef __LANGUAGE_ITALIAN__
"IMPOSTA ETA'",
#endif
#ifdef __LANGUAGE_GERMAN__
"ALTER EINSTELLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"LEEFTIJD INSTELLEN",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"definir IDADE",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"cai dat DO TUOI",
#endif
    },//STR_SET_AGE_ID
    
    {
		"set gender",
#ifdef __LANGUAGE_SPANISH__
"ESTABLECER SEXO",
#endif
#ifdef __LANGUAGE_FRENCH__
"INDIQUER LE SEXE",
#endif
#ifdef __LANGUAGE_ITALIAN__
"IMPOSTA SESSO",
#endif
#ifdef __LANGUAGE_GERMAN__
"GESCHLECHT EINGEBEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"GESLACHT INSTELLEN",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"definir sexo",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"cai dat gioi tinh",
#endif
    },//STR_SET_GENDER_ID
    
    {
		"SET INCLINE",
#ifdef __LANGUAGE_SPANISH__
"Establecer inclinacion",
#endif
#ifdef __LANGUAGE_FRENCH__
"Definir la pente",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Imposta Inclinazione",
#endif
#ifdef __LANGUAGE_GERMAN__
"Steigung einstellen",
#endif
#ifdef __LANGUAGE_DUTCH__
"Helling instellen",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"DEFINIR INCLINACAO",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"CaI DAT TU CHOI",
#endif
    },//STR_SET_INCLINE_ID,
    
    {
		"SET LEVEL",
#ifdef __LANGUAGE_SPANISH__
"ESTABLECER NIVEL",
#endif
#ifdef __LANGUAGE_FRENCH__
"REGLER NIVEAU",
#endif
#ifdef __LANGUAGE_ITALIAN__
"IMPOSTA LIVELLO",
#endif
#ifdef __LANGUAGE_GERMAN__
"LEVEL EINSTELLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"LEVEL INSTELLEN",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"DEFINIR NiVEL",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"CaI DAT CAP",
#endif
    },//STR_SET_LEVEL_ID,
    
    {
		"SET RESISTANCE",
#ifdef __LANGUAGE_SPANISH__
"ESTABLECER RESISTENCIA",
#endif
#ifdef __LANGUAGE_FRENCH__
"ENTRER NIVEAU",
#endif
#ifdef __LANGUAGE_ITALIAN__
"IMPOSTA RESISTENZA",
#endif
#ifdef __LANGUAGE_GERMAN__
"resistentie instellen",
#endif
#ifdef __LANGUAGE_DUTCH__
"LEVEL INSTELLEN",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"DEFINIR RESISTeNCIA",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"CaI DAT LUC KHaNG",
#endif
    },//STR_SET_RESISTANCE_ID,
    
    {
		"SET SPEED",
#ifdef __LANGUAGE_SPANISH__
"Establecer velocidad",
#endif
#ifdef __LANGUAGE_FRENCH__
"Definir la vitesse",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Imposta Velocita",
#endif
#ifdef __LANGUAGE_GERMAN__
"Geschwindigkeit einstellen",
#endif
#ifdef __LANGUAGE_DUTCH__
"Snelheid instellen",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"DEFINIR VELOCIDADE",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"CaI DAT TOC DO",
#endif
    },//STR_SET_SPEED_ID,
    
    {
		"SET TIME",
#ifdef __LANGUAGE_SPANISH__
"ESTABLECER TIEMPO",
#endif
#ifdef __LANGUAGE_FRENCH__
"DEFINIR DUREE",
#endif
#ifdef __LANGUAGE_ITALIAN__
"IMPOSTA TEMPO",
#endif
#ifdef __LANGUAGE_GERMAN__
"ZEIT EINSTELLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"TIJD INSTELLEN",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"DEFINIR HORA",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"CaI DAT THOI GIAN",
#endif
    },//STR_SET_TIME_ID
    {
		"Tower of Pisa",
#ifdef __LANGUAGE_SPANISH__
"Statue of Liberty",
#endif
#ifdef __LANGUAGE_FRENCH__
"SET TARGET",
#endif
#ifdef __LANGUAGE_ITALIAN__
"SET TARGET",
#endif
#ifdef __LANGUAGE_GERMAN__
"SET TARGET",
#endif
#ifdef __LANGUAGE_DUTCH__
"SET TARGET",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"SET TARGET",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"SET TARGET",
#endif
    },//STR_SET_TARGET_ID    
    {
		"Statue of Liberty",
#ifdef __LANGUAGE_SPANISH__
"Statue of Liberty",
#endif
#ifdef __LANGUAGE_FRENCH__
"SET TARGET",
#endif
#ifdef __LANGUAGE_ITALIAN__
"SET TARGET",
#endif
#ifdef __LANGUAGE_GERMAN__
"SET TARGET",
#endif
#ifdef __LANGUAGE_DUTCH__
"SET TARGET",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"SET TARGET",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"SET TARGET",
#endif
    },//STR_SET_TARGET1   
    {
		"Taipei 101",
#ifdef __LANGUAGE_SPANISH__
"Taipei 101",
#endif
#ifdef __LANGUAGE_FRENCH__
"Taipei 101",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Taipei 101",
#endif
#ifdef __LANGUAGE_GERMAN__
"SET TARGET",
#endif
#ifdef __LANGUAGE_DUTCH__
"SET TARGET",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"SET TARGET",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"SET TARGET",
#endif
    },//STR_SET_TARGET2
    {
		"Empire State Building",
#ifdef __LANGUAGE_SPANISH__
"Statue of Liberty",
#endif
#ifdef __LANGUAGE_FRENCH__
"SET TARGET",
#endif
#ifdef __LANGUAGE_ITALIAN__
"SET TARGET",
#endif
#ifdef __LANGUAGE_GERMAN__
"SET TARGET",
#endif
#ifdef __LANGUAGE_DUTCH__
"SET TARGET",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"SET TARGET",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"SET TARGET",
#endif
    },//STR_SET_TARGET3  
    {
		"One World Trade Center",
#ifdef __LANGUAGE_SPANISH__
"Statue of Liberty",
#endif
#ifdef __LANGUAGE_FRENCH__
"SET TARGET",
#endif
#ifdef __LANGUAGE_ITALIAN__
"SET TARGET",
#endif
#ifdef __LANGUAGE_GERMAN__
"SET TARGET",
#endif
#ifdef __LANGUAGE_DUTCH__
"SET TARGET",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"SET TARGET",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"SET TARGET",
#endif
    },//STR_SET_TARGET4 
    {
		"Burj Khalifa - Dubai",
#ifdef __LANGUAGE_SPANISH__
"Statue of Liberty",
#endif
#ifdef __LANGUAGE_FRENCH__
"SET TARGET",
#endif
#ifdef __LANGUAGE_ITALIAN__
"SET TARGET",
#endif
#ifdef __LANGUAGE_GERMAN__
"SET TARGET",
#endif
#ifdef __LANGUAGE_DUTCH__
"SET TARGET",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"SET TARGET",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"SET TARGET",
#endif
    },//STR_SET_TARGET5  
    {
		"St. Basil's Cathedral",
#ifdef __LANGUAGE_SPANISH__
"Statue of Liberty",
#endif
#ifdef __LANGUAGE_FRENCH__
"SET TARGET",
#endif
#ifdef __LANGUAGE_ITALIAN__
"SET TARGET",
#endif
#ifdef __LANGUAGE_GERMAN__
"SET TARGET",
#endif
#ifdef __LANGUAGE_DUTCH__
"SET TARGET",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"SET TARGET",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"SET TARGET",
#endif
    },//STR_SET_TARGET6 
    {
		"Great Pyramid of Giza",
#ifdef __LANGUAGE_SPANISH__
"Statue of Liberty",
#endif
#ifdef __LANGUAGE_FRENCH__
"SET TARGET",
#endif
#ifdef __LANGUAGE_ITALIAN__
"SET TARGET",
#endif
#ifdef __LANGUAGE_GERMAN__
"SET TARGET",
#endif
#ifdef __LANGUAGE_DUTCH__
"SET TARGET",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"SET TARGET",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"SET TARGET",
#endif
    },//STR_SET_TARGET7 
    {
		"Taj Mahal",
#ifdef __LANGUAGE_SPANISH__
"Statue of Liberty",
#endif
#ifdef __LANGUAGE_FRENCH__
"SET TARGET",
#endif
#ifdef __LANGUAGE_ITALIAN__
"SET TARGET",
#endif
#ifdef __LANGUAGE_GERMAN__
"SET TARGET",
#endif
#ifdef __LANGUAGE_DUTCH__
"SET TARGET",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"SET TARGET",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"SET TARGET",
#endif
    },//STR_SET_TARGET8 
    {
		"Eiffel Tower",
#ifdef __LANGUAGE_SPANISH__
"Statue of Liberty",
#endif
#ifdef __LANGUAGE_FRENCH__
"SET TARGET",
#endif
#ifdef __LANGUAGE_ITALIAN__
"SET TARGET",
#endif
#ifdef __LANGUAGE_GERMAN__
"SET TARGET",
#endif
#ifdef __LANGUAGE_DUTCH__
"SET TARGET",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"SET TARGET",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"SET TARGET",
#endif
    },//STR_SET_TARGET9 
    {
		"Christ the Redeemer Statue",
#ifdef __LANGUAGE_SPANISH__
"Statue of Liberty",
#endif
#ifdef __LANGUAGE_FRENCH__
"SET TARGET",
#endif
#ifdef __LANGUAGE_ITALIAN__
"SET TARGET",
#endif
#ifdef __LANGUAGE_GERMAN__
"SET TARGET",
#endif
#ifdef __LANGUAGE_DUTCH__
"SET TARGET",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"SET TARGET",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"SET TARGET",
#endif
    },//STR_SET_TARGET10 
    {
		"set WEIGHT",
#ifdef __LANGUAGE_SPANISH__
"ESTABLECER PESO",
#endif
#ifdef __LANGUAGE_FRENCH__
"ENTRER POIDS",
#endif
#ifdef __LANGUAGE_ITALIAN__
"IMPOSTA PESO",
#endif
#ifdef __LANGUAGE_GERMAN__
"GEWICHT EINSTELLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"GEWICHT INSTELLEN",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"definir PESO",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"cai dat TRONG LUONG",
#endif
    },//STR_SET_WEIGHT_ID
    
    {
		"STANDARD",
#ifdef __LANGUAGE_SPANISH__
"ESTaNDAR",
#endif
#ifdef __LANGUAGE_FRENCH__
"STANDARD",
#endif
#ifdef __LANGUAGE_ITALIAN__
"STANDARD",
#endif
#ifdef __LANGUAGE_GERMAN__
"STANDARD",
#endif
#ifdef __LANGUAGE_DUTCH__
"STANDAARD",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"STANDARD",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"TIeU CHUAN",
#endif
    },//STR_STANDARD_ID 
    
    {
		"UPDATE COMPLETE",
#ifdef __LANGUAGE_SPANISH__
"Actualizacion completada",
#endif
#ifdef __LANGUAGE_FRENCH__
"Mise a jour terminee",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Aggiornamento completato",
#endif
#ifdef __LANGUAGE_GERMAN__
"Update abgeschlossen",
#endif
#ifdef __LANGUAGE_DUTCH__
"Update is voltooid",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Atualizacao concluida",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"CAP NHAT HOaN TAT",
#endif
    },//STR_UPDATE_COMPLETE_ID 
    
    {
		"UPDATE COMPLETED PLEASE PRESS ENTER",
#ifdef __LANGUAGE_SPANISH__
"ACTUALIZACION COMPLETA OPRIMA ENTER",
#endif
#ifdef __LANGUAGE_FRENCH__
"MISE A JOUR LOGICIELLE AVORTEE VEUILLEZ PRESSER ENTREE",
#endif
#ifdef __LANGUAGE_ITALIAN__
"AGGIORNAMENTO COMPLETATO PREMI ENTER",
#endif
#ifdef __LANGUAGE_GERMAN__
"AKTUALISIERUNG BEENDET BITTE EINGABE DRUCKEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"UPDATE VOLTOOID DRUK OP ENTER",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"ATUALIZACAO CONCLUiDA PRESSIONE ENTER",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"CAP NHAT DA HOaN TAT VUI LONG NHAN ENTER",
#endif
    },//STR_UPDATE_UPDATE_COMPLETED_PLEASE_PRESS_ENTER_ID 
    
    {
		"UPDATE SOFTWARE FAILED PLEASE PRESS ENTER",
#ifdef __LANGUAGE_SPANISH__
"ACTUALIZACION ABORTADA OPRIMA ENTER",
#endif
#ifdef __LANGUAGE_FRENCH__
"MISE A JOUR LOGICIELLE AVORTEE VEUILLEZ PRESSER ENTREE",
#endif
#ifdef __LANGUAGE_ITALIAN__
"AGGIORNAMENTO FALLITO PREMI ENTER",
#endif
#ifdef __LANGUAGE_GERMAN__
"SOFTWARE AKTUALISIERUNG FEHLGESCHLAGEN BITTE EINGABE DRUECKEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"SOFTWARE UPDATE MISLUKT DRUK OP ENTER",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"FALHA AO ATUALIZAR SOFTWARE PRESSIONE ENTER",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"CAP NHAT PHAN MEM THAT BAI VUI LONG NHAN ENTER",
#endif
    },//STR_UPDATE_SOFTWARE_FAILED_PLEASE_PRESS_ENTER_ID  
    
	
    
    {
		"WORKOUT COMPLETE",
#ifdef __LANGUAGE_SPANISH__
"Ejercicio completado",
#endif
#ifdef __LANGUAGE_FRENCH__
"EntraInement termine",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Esercizio completato",
#endif
#ifdef __LANGUAGE_GERMAN__
"Training abgeschlossen",
#endif
#ifdef __LANGUAGE_DUTCH__
"Training is voltooid",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Exercicio concluido",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"TAP XONG",
#endif
    },//STR_WORKOUT_COMPLETE_ID
    
    {
		"INITIAL",
#ifdef __LANGUAGE_SPANISH__
"INICIAL",
#endif
#ifdef __LANGUAGE_FRENCH__
"PRePARATION",
#endif
#ifdef __LANGUAGE_ITALIAN__
"INIZIALE",
#endif
#ifdef __LANGUAGE_GERMAN__
"ANFANG",
#endif
#ifdef __LANGUAGE_DUTCH__
"BEGIN",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"INICIAL",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"BAN DAU",
#endif
    },//STR_INITIAL_ID
    
    {
		"UPDATING USER INFO",
#ifdef __LANGUAGE_SPANISH__
"ACTUALIZANDO DATOS DE USUARIO",
#endif
#ifdef __LANGUAGE_FRENCH__
"MISE a JOUR DES INFORMATIONS SUR L'UTILISATEUR",
#endif
#ifdef __LANGUAGE_ITALIAN__
"AGGIORNAMENTO INFO UTENTE",
#endif
#ifdef __LANGUAGE_GERMAN__
"BENUTZERINFORMATIONEN AKTUALISIEREN",
#endif
#ifdef __LANGUAGE_DUTCH__
"GEBRUIKERSGEGEVENS WORDEN BIJGEWERKT",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"ATUALIZANDO INFO DO USUaRIO",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"CAP NHAT THoNG TIN NGUOI DUNG",
#endif
    },//STR_UPDATING_USER_INFO_ID
    
    {
		"SET TIMEZONE",
#ifdef __LANGUAGE_SPANISH__
"DEFINIR ZONA HORARIA",
#endif
#ifdef __LANGUAGE_FRENCH__
"DeFINIR LE FUSEAU HORAIRE",
#endif
#ifdef __LANGUAGE_ITALIAN__
"IMPOSTA FUSO ORARIO",
#endif
#ifdef __LANGUAGE_GERMAN__
"ZEITZONE EINSTELLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"TIJDSZONE INSTELLEN",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"DEFIIR FUSO HORaRIO",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"CaI DAT MUI GIO",
#endif
    },//STR_SET_TIMEZONE_ID
    
    {
		"CONNECTING wifi",
#ifdef __LANGUAGE_SPANISH__
"CONECTANDO CON WI-FI",
#endif
#ifdef __LANGUAGE_FRENCH__
"CONNEXION AU Wi-Fi",
#endif
#ifdef __LANGUAGE_ITALIAN__
"COLLEGAMENTO wifi in corso",
#endif
#ifdef __LANGUAGE_GERMAN__
"MIT WIFI VERBINDEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"VERBINDING MAKEN MET wifi ",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"CONECTANDO A WIF-FI",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"DANG KET NOI wifi",
#endif
    },//STR_CONNECTING_WIFI_ID
    
    {
		"SET TARGET WATTS",
#ifdef __LANGUAGE_SPANISH__
"SELECCIONAR OBJETIVO DE VATIOS",
#endif
#ifdef __LANGUAGE_FRENCH__
"DeFINIR UN OBJECTIF DE WATTS",
#endif
#ifdef __LANGUAGE_ITALIAN__
"IMPOSTA WATT OBIETTIVO",
#endif
#ifdef __LANGUAGE_GERMAN__
"ZIELWATT EINSTELLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"DOELWATT INSTELLEN",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"DEFINIR WATTS DE DESTINO",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"CaI DAT CoNG SUAT MUC TIeU",
#endif
    },//STR_SET_TARGET_WATTS_ID
    
    {
		"SET THR",
#ifdef __LANGUAGE_SPANISH__
"SELECCIONAR  UMBRAL",
#endif
#ifdef __LANGUAGE_FRENCH__
"DeFINIR LA FCC",
#endif
#ifdef __LANGUAGE_ITALIAN__
"IMPOSTA THR",
#endif
#ifdef __LANGUAGE_GERMAN__
"GRENZW. EINSTELLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"DOELHARTSLAG INSTELLEN",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"DEFINIR THR",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"CaI DAT THR",
#endif
    },//STR_SET_THR_ID
    
	
    {
		"ERROR NO HEART RATE",
#ifdef __LANGUAGE_SPANISH__
"ERROR, NO HAY RITMO CARDIACO",
#endif
#ifdef __LANGUAGE_FRENCH__
"ERREUR : PAS DE FReQUENCE CARDIAQUE",
#endif
#ifdef __LANGUAGE_ITALIAN__
"ERRORE FREQUENZA CARDIACA ASSENTE",
#endif
#ifdef __LANGUAGE_GERMAN__
"FEHLER KEINE HERZFREQUENZ",
#endif
#ifdef __LANGUAGE_DUTCH__
"FOUT GEEN HARTSLAG",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"ERRO SEM FREQUeNCIA CARDiACA",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Co LOI, KHoNG Co NHIP TIM",
#endif
    },//STR_ERROR_NO_HEART_RATE_ID 
    
    {
		"HEART RATE CONTROL",
#ifdef __LANGUAGE_SPANISH__
"CONTROL DE RITMO CARDIACO",
#endif
#ifdef __LANGUAGE_FRENCH__
"CONTRoLE DE FReQUENCE CARDIAQUE",
#endif
#ifdef __LANGUAGE_ITALIAN__
"CONTROLLO FREQUENZA CARDIACA",
#endif
#ifdef __LANGUAGE_GERMAN__
"HERZFREQUENZKONTROLLE",
#endif
#ifdef __LANGUAGE_DUTCH__
"HARTSLAGCONTROLE",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"CONTRLE DE FREQUeNCIA CARDiACA",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"KIEM SOaT NHIP TIM",
#endif
    },//STR_HEART_RATE_CONTROL_ID
    
    {
		"Time out",
#ifdef __LANGUAGE_SPANISH__
"Tiempo de espera agotado",
#endif
#ifdef __LANGUAGE_FRENCH__
"Arret",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Timeout",
#endif
#ifdef __LANGUAGE_GERMAN__
"Unterbrechung",
#endif
#ifdef __LANGUAGE_DUTCH__
"Onderbreking",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Tempo esgotado",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Het thoi gian",
#endif
    },//STR_TIME_OUT_ID
    
    {
		"ENTER PASSCODE",
#ifdef __LANGUAGE_SPANISH__
"INTRODUCIR CoDIGO DE ACCESO",
#endif
#ifdef __LANGUAGE_FRENCH__
"SAISIR LE MOT DE PASSE",
#endif
#ifdef __LANGUAGE_ITALIAN__
"INSERISCI PASSCODE",
#endif
#ifdef __LANGUAGE_GERMAN__
"PASSCODE EINGEBEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"VOER TOEGANGSCODE IN",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"INSERIR SENHA",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"NHAP MAT MA",
#endif
    },//STR_ENTER_PASSCODE_ID
    
    {
		"ENTER XID",
#ifdef __LANGUAGE_SPANISH__
"INTRODUCIR XID",
#endif
#ifdef __LANGUAGE_FRENCH__
"SAISIR L'IDENTIFIANT",
#endif
#ifdef __LANGUAGE_ITALIAN__
"INSERISCI XID",
#endif
#ifdef __LANGUAGE_GERMAN__
"XID EINGEBEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"VOER XID IN",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"INSERIR XID",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"NHAP XID",
#endif
    },//STR_ENTER_XID_ID 
    
    {
		"EDIT USER",
#ifdef __LANGUAGE_SPANISH__
"EDITAR USUARIO",
#endif
#ifdef __LANGUAGE_FRENCH__
"MODIFIER L'UTILISATEUR",
#endif
#ifdef __LANGUAGE_ITALIAN__
"MODIFICA UTENTE",
#endif
#ifdef __LANGUAGE_GERMAN__
"NUTZER BEARBEITEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"GEBRUIKER BEWERKEN",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"EDITAR USUaRIO",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"SUA NGUOI DUNG",
#endif
    },//STR_EDIT_USER_ID
    
    {
		"SELECT program",
#ifdef __LANGUAGE_SPANISH__
"SELECCIONAR programa",
#endif
#ifdef __LANGUAGE_FRENCH__
"SeLECTIONNER un programme",
#endif
#ifdef __LANGUAGE_ITALIAN__
"SELEZIONA programma",
#endif
#ifdef __LANGUAGE_GERMAN__
"Programm AUSWaHLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"programma SELECTEREN",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"SELECIONAR programa",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"CHON chuong trinh",
#endif
    },//STR_SELECT_PROGRAM_ID  
    
    {
		"EDIT A USER",
#ifdef __LANGUAGE_SPANISH__
"EDITAR UN USUARIO",
#endif
#ifdef __LANGUAGE_FRENCH__
"MODIFIER UN UTILISATEUR",
#endif
#ifdef __LANGUAGE_ITALIAN__
"MODIFICA UN UTENTE",
#endif
#ifdef __LANGUAGE_GERMAN__
"BENUTZER BEARBEITEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"EEN GEBRUIKER BEWERKEN",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"EDITAR USUaRIO",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"SUA NGUOI DUNG",
#endif
    },//STR_EDIT_A_USER_ID
    
    {
		"DELETE A USER",
#ifdef __LANGUAGE_SPANISH__
"ELIMINAR UN USUARIO",
#endif
#ifdef __LANGUAGE_FRENCH__
"SUPPRIMER UN UTILISATEUR",
#endif
#ifdef __LANGUAGE_ITALIAN__
"ELIMINA UN UTENTE",
#endif
#ifdef __LANGUAGE_GERMAN__
"BENUTZER LoSCHEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"EEN GEBRUIKER VERWIJDEREN",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"EXCLUIR UM USUaRIO",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"XoA NGUOI DUNG",
#endif
    },//STR_DELETE_A_USER_ID
    
    {
		"USER LOGIN...",
#ifdef __LANGUAGE_SPANISH__
"INICIO DE SESIoN DE USUARIO",
#endif
#ifdef __LANGUAGE_FRENCH__
"CONNEXION UTILISATEUR",
#endif
#ifdef __LANGUAGE_ITALIAN__
"LOGIN UTENTE",
#endif
#ifdef __LANGUAGE_GERMAN__
"BENUTZERANMELDUNG",
#endif
#ifdef __LANGUAGE_DUTCH__
"AANMELDEN GEBRUIKER",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"LOGIN DE USUaRIO",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"NGUOI DUNG DANG NHAP",
#endif
    },//STR_USER_LOGIN_ID
    
    {
		"INVALID USER/PIN",
#ifdef __LANGUAGE_SPANISH__
"USUARIO/PIN NO VaLIDO",
#endif
#ifdef __LANGUAGE_FRENCH__
"Code PIN/UTILISATEUR non valide",
#endif
#ifdef __LANGUAGE_ITALIAN__
"UTENTE/PIN NON VALIDO",
#endif
#ifdef __LANGUAGE_GERMAN__
"BENUTZERNAME/PIN UNGuLTIG",
#endif
#ifdef __LANGUAGE_DUTCH__
"ONGELDIGE GEBRUIKER/PIN",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"USUaRIO/PIN INVaLIDO",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"NGUOI DUNG/MA PIN KHoNG HOP LE",
#endif
    },//STR_INVALID_USER_PIN_ID 
    
    {
		"success",
#ifdef __LANGUAGE_SPANISH__
"Correcto",
#endif
#ifdef __LANGUAGE_FRENCH__
"reussite",
#endif
#ifdef __LANGUAGE_ITALIAN__
"riuscito",
#endif
#ifdef __LANGUAGE_GERMAN__
"Erfolg",
#endif
#ifdef __LANGUAGE_DUTCH__
"succes",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"sucesso",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"thanh cong",
#endif
    },//STR_SUCCESS_ID  
    
    {
		"WIFI IS OFFLINE",
#ifdef __LANGUAGE_SPANISH__
"WI-FI ESTa FUERA DE LiNEA",
#endif
#ifdef __LANGUAGE_FRENCH__
"WI-FI DeCONNECTe",
#endif
#ifdef __LANGUAGE_ITALIAN__
"WIFI OFFLINE",
#endif
#ifdef __LANGUAGE_GERMAN__
"WIFI ist offline",
#endif
#ifdef __LANGUAGE_DUTCH__
"WIFI IS OFFLINE",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"REDE WI-FI ESTa OFF-LINE",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"WIFI KHoNG Co MANG",
#endif
    },//STR_WIFI_IS_OFFLINE_ID
    
    {
		"safety key off",
#ifdef __LANGUAGE_SPANISH__
"Clave de seguridad desactivada",
#endif
#ifdef __LANGUAGE_FRENCH__
"cle de securite desactivee",
#endif
#ifdef __LANGUAGE_ITALIAN__
"chiave di sicurezza off",
#endif
#ifdef __LANGUAGE_GERMAN__
"Sicherheitsschlussel aus",
#endif
#ifdef __LANGUAGE_DUTCH__
"veiligheidssleutel uitgeschakeld",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"chave de seguranca desligada",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"tat khoa an toan",
#endif
    },//STR_SAFETY_KEY_OFF_ID
    
    {
		"heartrate",
#ifdef __LANGUAGE_SPANISH__
"ritmo cardiaco",
#endif
#ifdef __LANGUAGE_FRENCH__
"frequence cardiaque",
#endif
#ifdef __LANGUAGE_ITALIAN__
"frequenza cardiaca",
#endif
#ifdef __LANGUAGE_GERMAN__
"Herzfrequenz",
#endif
#ifdef __LANGUAGE_DUTCH__
"hartslag",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"frequencia cardiaca",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"nhip tim",
#endif
    },//STR_HEARTRATE_ID
    {
		"set target heart rate",
#ifdef __LANGUAGE_SPANISH__
"ritmo cardiaco",
#endif
#ifdef __LANGUAGE_FRENCH__
"frequence cardiaque",
#endif
#ifdef __LANGUAGE_ITALIAN__
"frequenza cardiaca",
#endif
#ifdef __LANGUAGE_GERMAN__
"Herzfrequenz",
#endif
#ifdef __LANGUAGE_DUTCH__
"hartslag",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"frequencia cardiaca",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"nhip tim",
#endif
    },//STR_SET_TARGET_HEARTRATE_ID
    {
		"RF SYNC TIMEOUT",
#ifdef __LANGUAGE_SPANISH__
"TIEMPO DE ESPERA DE SINCRONIZACIoN RF AGOTADO",
#endif
#ifdef __LANGUAGE_FRENCH__
"ARReT DE LA SYNCHRONISATION RF",
#endif
#ifdef __LANGUAGE_ITALIAN__
"TIMEOUT SINCR RF",
#endif
#ifdef __LANGUAGE_GERMAN__
"RF SYNC TIMEOUT",
#endif
#ifdef __LANGUAGE_DUTCH__
"ONDERBREKING SYNC RF",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"TEMPO ESGOTADO SINCR RF",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"HET THOI GIAN DONG BO RF",
#endif
    },//STR_RF_SYNC_TIMEOUT_ID
    
    
    
    {
		"tf30",
#ifdef __LANGUAGE_SPANISH__
"tf30",
#endif
#ifdef __LANGUAGE_FRENCH__
"tf30",
#endif
#ifdef __LANGUAGE_ITALIAN__
"tf30",
#endif
#ifdef __LANGUAGE_GERMAN__
"tf30",
#endif
#ifdef __LANGUAGE_DUTCH__
"tf30",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"tf30",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"tf30",
#endif
    },//STR_TF30_ID
    
    {
		"tf50",
#ifdef __LANGUAGE_SPANISH__
"tf50",
#endif
#ifdef __LANGUAGE_FRENCH__
"tf50",
#endif
#ifdef __LANGUAGE_ITALIAN__
"tf50",
#endif
#ifdef __LANGUAGE_GERMAN__
"tf50",
#endif
#ifdef __LANGUAGE_DUTCH__
"tf50",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"tf50",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"tf50",
#endif
    },//STR_TF50_ID
    
    {
		"t50",
#ifdef __LANGUAGE_SPANISH__
"t50",
#endif
#ifdef __LANGUAGE_FRENCH__
"t50",
#endif
#ifdef __LANGUAGE_ITALIAN__
"t50",
#endif
#ifdef __LANGUAGE_GERMAN__
"t50",
#endif
#ifdef __LANGUAGE_DUTCH__
"t50",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"t50",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"t50",
#endif
    },//STR_T50_ID
    
    {
		"r30",
#ifdef __LANGUAGE_SPANISH__
"r30",
#endif
#ifdef __LANGUAGE_FRENCH__
"r30",
#endif
#ifdef __LANGUAGE_ITALIAN__
"r30",
#endif
#ifdef __LANGUAGE_GERMAN__
"r30",
#endif
#ifdef __LANGUAGE_DUTCH__
"r30",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"r30",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"r30",
#endif
    },//STR_R30_ID
    
    {
		"u30",
#ifdef __LANGUAGE_SPANISH__
"u30",
#endif
#ifdef __LANGUAGE_FRENCH__
"u30",
#endif
#ifdef __LANGUAGE_ITALIAN__
"u30",
#endif
#ifdef __LANGUAGE_GERMAN__
"u30",
#endif
#ifdef __LANGUAGE_DUTCH__
"u30",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"u30",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"u30",
#endif
    },//STR_U30_ID
    
    {
		"E30",
#ifdef __LANGUAGE_SPANISH__
"E30",
#endif
#ifdef __LANGUAGE_FRENCH__
"E30",
#endif
#ifdef __LANGUAGE_ITALIAN__
"E30",
#endif
#ifdef __LANGUAGE_GERMAN__
"E30",
#endif
#ifdef __LANGUAGE_DUTCH__
"E30",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"E30",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"E30",
#endif
    },//STR_E30_ID
    
    {
		"r50",
#ifdef __LANGUAGE_SPANISH__
"r50",
#endif
#ifdef __LANGUAGE_FRENCH__
"r50",
#endif
#ifdef __LANGUAGE_ITALIAN__
"r50",
#endif
#ifdef __LANGUAGE_GERMAN__
"r50",
#endif
#ifdef __LANGUAGE_DUTCH__
"r50",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"r50",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"r50",
#endif
    },//STR_R50_ID
    
    {
		"u50",
#ifdef __LANGUAGE_SPANISH__
"u50",
#endif
#ifdef __LANGUAGE_FRENCH__
"u50",
#endif
#ifdef __LANGUAGE_ITALIAN__
"u50",
#endif
#ifdef __LANGUAGE_GERMAN__
"u50",
#endif
#ifdef __LANGUAGE_DUTCH__
"u50",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"u50",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"u50",
#endif
    },//STR_U50_ID
    
    {
		"E50",
#ifdef __LANGUAGE_SPANISH__
"E50",
#endif
#ifdef __LANGUAGE_FRENCH__
"E50",
#endif
#ifdef __LANGUAGE_ITALIAN__
"E50",
#endif
#ifdef __LANGUAGE_GERMAN__
"E50",
#endif
#ifdef __LANGUAGE_DUTCH__
"E50",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"E50",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"E50",
#endif
    },//STR_E50_ID
    
    {
		"a30",
#ifdef __LANGUAGE_SPANISH__
"a30",
#endif
#ifdef __LANGUAGE_FRENCH__
"a30",
#endif
#ifdef __LANGUAGE_ITALIAN__
"a30",
#endif
#ifdef __LANGUAGE_GERMAN__
"a30",
#endif
#ifdef __LANGUAGE_DUTCH__
"a30",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"a30",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"a30",
#endif
    },//STR_A30_ID
    
    {
		"a50",
#ifdef __LANGUAGE_SPANISH__
"a50",
#endif
#ifdef __LANGUAGE_FRENCH__
"a50",
#endif
#ifdef __LANGUAGE_ITALIAN__
"a50",
#endif
#ifdef __LANGUAGE_GERMAN__
"a50",
#endif
#ifdef __LANGUAGE_DUTCH__
"a50",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"a50",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"a50",
#endif
    },//STR_A50_ID
    {
		"t70",
#ifdef __LANGUAGE_SPANISH__
"t70",
#endif
#ifdef __LANGUAGE_FRENCH__
"t70",
#endif
#ifdef __LANGUAGE_ITALIAN__
"t70",
#endif
#ifdef __LANGUAGE_GERMAN__
"t70",
#endif
#ifdef __LANGUAGE_DUTCH__
"t70",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"t70",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"t70",
#endif
    },//STR_T70_ID
    {
		"C50",
#ifdef __LANGUAGE_SPANISH__
"C50",
#endif
#ifdef __LANGUAGE_FRENCH__
"C50",
#endif
#ifdef __LANGUAGE_ITALIAN__
"C50",
#endif
#ifdef __LANGUAGE_GERMAN__
"C50",
#endif
#ifdef __LANGUAGE_DUTCH__
"C50",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"C50",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"C50",
#endif
    },//STR_C50_ID
    {
		"t30",
#ifdef __LANGUAGE_SPANISH__
"t30",
#endif
#ifdef __LANGUAGE_FRENCH__
"t30",
#endif
#ifdef __LANGUAGE_ITALIAN__
"t30",
#endif
#ifdef __LANGUAGE_GERMAN__
"t30",
#endif
#ifdef __LANGUAGE_DUTCH__
"t30",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"t30",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"t30",
#endif
    },//STR_T30_ID
    {
		"t75",
#ifdef __LANGUAGE_SPANISH__
"t75",
#endif
#ifdef __LANGUAGE_FRENCH__
"t75",
#endif
#ifdef __LANGUAGE_ITALIAN__
"t75",
#endif
#ifdef __LANGUAGE_GERMAN__
"t75",
#endif
#ifdef __LANGUAGE_DUTCH__
"t75",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"t75",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"t75",
#endif
    },//STR_T75_ID

    {
		"rebooting......",
#ifdef __LANGUAGE_SPANISH__
"reiniciando...",
#endif
#ifdef __LANGUAGE_FRENCH__
"redemarrage......",
#endif
#ifdef __LANGUAGE_ITALIAN__
"riavvio in corso...",
#endif
#ifdef __LANGUAGE_GERMAN__
"Neustart......",
#endif
#ifdef __LANGUAGE_DUTCH__
"bezig met herstarten...",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"reinicializando...",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"dang khoi dong lai...",
#endif
    },//STR_REBOOTING_ID
    
    {
		"WIFI SUCCESS",
#ifdef __LANGUAGE_SPANISH__
"CONEXIoN CON WI-FI CORRECTA",
#endif
#ifdef __LANGUAGE_FRENCH__
"WI-FI CONNECTe",
#endif
#ifdef __LANGUAGE_ITALIAN__
"WIFI RIUSCITO",
#endif
#ifdef __LANGUAGE_GERMAN__
"WIFI ERFOLG",
#endif
#ifdef __LANGUAGE_DUTCH__
"WIFI GESLAAGD",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"WI-FI CONECTADA",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"WIFI THaNH CoNG",
#endif
    },//STR_WIFI_SUCCESS_ID
    
    {
		"WIFI FAIL",
#ifdef __LANGUAGE_SPANISH__
"ERROR EN LA CONEXIoN CON WI-FI",
#endif
#ifdef __LANGUAGE_FRENCH__
"eCHEC DE CONNEXION AU WI-FI",
#endif
#ifdef __LANGUAGE_ITALIAN__
"WIFI NON RIUSCITO",
#endif
#ifdef __LANGUAGE_GERMAN__
"WIFI FEHLSCHLAG",
#endif
#ifdef __LANGUAGE_DUTCH__
"WIFI MISLUKT",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"FALHA DE WI-FI",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"WIFI THAT BAI",
#endif
    },//STR_WIFI_FAIL_ID
    
    {
		"WIFI WAITTING",
#ifdef __LANGUAGE_SPANISH__
"A LA ESPERA DE LA CONEXIoN WI-FI",
#endif
#ifdef __LANGUAGE_FRENCH__
"WI-FI EN ATTENTE",
#endif
#ifdef __LANGUAGE_ITALIAN__
"WIFI IN ATTESA",
#endif
#ifdef __LANGUAGE_GERMAN__
"WIFI WARTEND",
#endif
#ifdef __LANGUAGE_DUTCH__
"WACHT OP WIFI",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"AGUARDANDO WI-FI",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"DANG CHO WIFI",
#endif
    },//STR_WIFI_WAITING_ID
    
    {
		"WIFI TIMEOUT",
#ifdef __LANGUAGE_SPANISH__
"TIEMPO DE ESPERA DE CONEXIoN WI-FI AGOTADO",
#endif
#ifdef __LANGUAGE_FRENCH__
"CONNEXION AU WI-FI EXPIReE",
#endif
#ifdef __LANGUAGE_ITALIAN__
"TIMEOUT WIFI",
#endif
#ifdef __LANGUAGE_GERMAN__
"WIFI TIMEOUT",
#endif
#ifdef __LANGUAGE_DUTCH__
"ONDERBREKING VAN WIFI",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"TEMPO ESGOTADO DA WI-FI",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"HET THOI GIAN WIFI",
#endif
    },//STR_WIFI_TIMEOUT_ID
    
    {
		"power off/on",
#ifdef __LANGUAGE_SPANISH__
"Encendido/Apagado",
#endif
#ifdef __LANGUAGE_FRENCH__
"Alimentation off/on",
#endif
#ifdef __LANGUAGE_ITALIAN__
"spegnimento/accensione",
#endif
#ifdef __LANGUAGE_GERMAN__
"Ein-/Ausschalten",
#endif
#ifdef __LANGUAGE_DUTCH__
"ingeschakeld/uitgeschakeld",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"ligar/desligar",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"tat/bat nguon",
#endif
    },//STR_POWER_OFF_ON_ID
    
    {
		"DOWNLOADING SOFTWARE ",
#ifdef __LANGUAGE_SPANISH__
"DESCARGANDO SOFTWARE ",
#endif
#ifdef __LANGUAGE_FRENCH__
"TeLeCHARGEMENT DU LOGICIEL ",
#endif
#ifdef __LANGUAGE_ITALIAN__
"DOWNLOAD SOFTWARE IN CORSO ",
#endif
#ifdef __LANGUAGE_GERMAN__
"SOFTWARE WIRD HERUNTERGELADEN ",
#endif
#ifdef __LANGUAGE_DUTCH__
"SOFTWARE WORDT GEDOWNLOAD ",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"BAIXANDO SOFTWARE ",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"DANG TAI VE PHAN MEM ",
#endif
    },//STR_DOWNLOADING_SOFTWARE_ID
    {
		"DOWNLOADING... ",
#ifdef __LANGUAGE_SPANISH__
"DESCARGANDO... ",
#endif
#ifdef __LANGUAGE_FRENCH__
"TeLeCHARGEMENT... ",
#endif
#ifdef __LANGUAGE_ITALIAN__
"DOWNLOAD... ",
#endif
#ifdef __LANGUAGE_GERMAN__
"HERUNTERGELADEN...  ",
#endif
#ifdef __LANGUAGE_DUTCH__
"GEDOWNLOAD...  ",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"BAIXANDO... ",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"DANG TAI... ",
#endif
    },//STR_DOWNLOADING_ID
    {
		"UPDATE SOFTWARE",
#ifdef __LANGUAGE_SPANISH__
"ACTUALIZAR SOFTWARE",
#endif
#ifdef __LANGUAGE_FRENCH__
"MISE a JOUR DU LOGICIEL",
#endif
#ifdef __LANGUAGE_ITALIAN__
"AGGIORNA SOFTWARE",
#endif
#ifdef __LANGUAGE_GERMAN__
"SOFTWARE WIRD AKTUALISIERT",
#endif
#ifdef __LANGUAGE_DUTCH__
"SOFTWARE BIJWERKEN",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"ATUALIZAR SOFTWARE",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"CAP NHAT PHAN MeM",
#endif
    },//STR_UPDATE_SOFTWARE_ID
    
    {
		"SOFTWARE UPDATE PRESS ENTER",
#ifdef __LANGUAGE_SPANISH__
"ACTUALIZACION DE SOFTWARE PULSE ENTER ",
#endif
#ifdef __LANGUAGE_FRENCH__
"MISE A JOUR DU LOGICIEL APPUYEZ SUR ENTREE",
#endif
#ifdef __LANGUAGE_ITALIAN__
"AGGIORNAMENTO SOFTWARE PREMI INVIO",
#endif
#ifdef __LANGUAGE_GERMAN__
"SOFTWARE UPDATE EINGABETASTE DRUCKEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"SOFTWARE UPDATE DRUK OP ENTER",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"ATUALIZAR SOFTWARE. APERTE ENTER",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"CAP NHAT PHAN MEM. NHAN ENTER",
#endif
    },//STR_SOFTWARE_UPDATE_PRESS_ENTER_FOR_YES_BACK_FOR_NO_ID
    
    {
		"SAVE WORKOUT   Press Enter for Yes Back for No",
#ifdef __LANGUAGE_SPANISH__
"GUARDAR SESIoN DE ENTRENAMIENTO Pulse Intro para Si, Atras para No",
#endif
#ifdef __LANGUAGE_FRENCH__
"SAUVEGARDER L'ENTRAINEMENT   Appuyer sur Entree pour accepter, sur Retour pour annuler",
#endif
#ifdef __LANGUAGE_ITALIAN__
"SALVA ALLENAMENTO   Premere Invio per Si o Indietro per No",
#endif
#ifdef __LANGUAGE_GERMAN__
"TRAINING SPEICHERN   Drucken Sie Enter fur Ja Back fur Nein",
#endif
#ifdef __LANGUAGE_DUTCH__
"TRAINING OPSLAAN  Druk op Enter voor Ja en Backspace voor Nee",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"SALVAR TREINO   Pressione Enter para Sim, Voltar para Nao",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"LUU BaI TAP Nhan Enter neu Co Tro lai neu Khong",
#endif
    },//STR_SAVE_WORKOUT_PRESS_ENTER_FOR_YESY_BACK_FOR_NO_ID
    
    {
		"PRESS ENTER TO CONFIRM",
#ifdef __LANGUAGE_SPANISH__
"PULSE INTRO PARA CONFIRMAR",
#endif
#ifdef __LANGUAGE_FRENCH__
"APPUYER SUR ENTReE POUR CONFIRMER",
#endif
#ifdef __LANGUAGE_ITALIAN__
"PREMERE INVIO PER CONFERMARE",
#endif
#ifdef __LANGUAGE_GERMAN__
"DRuCKEN SIE ZUR BESTaTIGUNG ,ENTER.",
#endif
#ifdef __LANGUAGE_DUTCH__
"DRUK OP ENTER TER BEVESTIGING",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"PRESSIONE ENTER PARA CONFIRMAR",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"NHAN ENTER DE XaC NHAN",
#endif
    },//STR_PRESS_ENTER_TO_CONFIRM_ID
    
    {
		"SOFTWARE UPDATE    NO SOFTWARE ON USB",
#ifdef __LANGUAGE_SPANISH__
"ACTUALIZAR SOFTWARE - NO HAY SOFTWARE EN USB",
#endif
#ifdef __LANGUAGE_FRENCH__
"MISE a JOUR DU LOGICIEL    PAS DE LOGICIEL SUR USB",
#endif
#ifdef __LANGUAGE_ITALIAN__
"AGGIORNAMENTO SOFTWARE    SOFTWARE NON RILEVATO NEL DISPOSITIVO USB",
#endif
#ifdef __LANGUAGE_GERMAN__
"SOFTWARE-UPDATE    KEINE SOFTWARE AUF USB",
#endif
#ifdef __LANGUAGE_DUTCH__
"BIJWERKING SOFTWARE   GEEN SOFTWARE OP USB",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"ATUALIZACAO DE SOFTWARE   NAO Ha SOFTWARE NO USB",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"CAP NHAT PHAN MEM KHoNG Co PHAN MEM TRONG USB",
#endif
    },//STR_SOFTWARE_UPDATE_NO_SOFTWARE_ON_USB_ID
    
    {
		"SOFTWARE UPDATE PRESS ENTER",
#ifdef __LANGUAGE_SPANISH__
"ACTUALIZACION DE SOFTWARE. PULSE ENTER.",
#endif
#ifdef __LANGUAGE_FRENCH__
"MISE A JOUR DU LOGICIEL APPUYEZ SUR ENTREE",
#endif
#ifdef __LANGUAGE_ITALIAN__
"AGGIORNAMENTO SOFTWARE, PREMI INVIO",
#endif
#ifdef __LANGUAGE_GERMAN__
"SOFTWARE-UPDATE: EINGABETASTE DRUCKEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"SOFTWARE-UPDATE DRUK OP ENTER",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"ATUALIZAR SOFTWARE. APERTE ENTER",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"CAP NHAT PHAN MEM, NHAN ENTER",
#endif
    },//STR_SOFTWARE_UPDATE_PRESS_ENTER_ID
    
    {
		"CONSTANT WATTS",
#ifdef __LANGUAGE_SPANISH__
"Vatios Constantes",
#endif
#ifdef __LANGUAGE_FRENCH__
"Constant Watts",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Watt Costanti",
#endif
#ifdef __LANGUAGE_GERMAN__
"Watt",
#endif
#ifdef __LANGUAGE_DUTCH__
"Constante Wattage",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Potencia Constante",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Watt Khong Doi",
#endif
    },//STR_CONSTANT_WATTS_ID
    
    {
		"FAT BURN",
#ifdef __LANGUAGE_SPANISH__
"Quema Calorias",
#endif
#ifdef __LANGUAGE_FRENCH__
"Brulage Des Graisses",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Brucia Grassi",
#endif
#ifdef __LANGUAGE_GERMAN__
"Fettverbrennung",
#endif
#ifdef __LANGUAGE_DUTCH__
"Vetverbranding",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Queima De Calorias",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Dot Chay Mo Thua",
#endif
    },//STR_FATBURN_ID
    
    
    {
		"INTERVALS",
#ifdef __LANGUAGE_SPANISH__
"Intervalos",
#endif
#ifdef __LANGUAGE_FRENCH__
"Intervalles",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Intervalli",
#endif
#ifdef __LANGUAGE_GERMAN__
"Intervall",
#endif
#ifdef __LANGUAGE_DUTCH__
"Intervallen",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Intervalos",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Khoang Thoi Gian",
#endif
    },//STR_INTERVALS_ID
    
    {
		"MANUAL",
#ifdef __LANGUAGE_SPANISH__
"Manual",
#endif
#ifdef __LANGUAGE_FRENCH__
"Manuel",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Manuale",
#endif
#ifdef __LANGUAGE_GERMAN__
"Manuell",
#endif
#ifdef __LANGUAGE_DUTCH__
"Handmatig",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Manual",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Thu Cong",
#endif
    },//STR_MANUAL_ID
    {
		"FITNESS TEST",
#ifdef __LANGUAGE_SPANISH__
"Test En Forma",
#endif
#ifdef __LANGUAGE_FRENCH__
"Test De Condition Physique",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Test Fitness",
#endif
#ifdef __LANGUAGE_GERMAN__
"Fitnesstest",
#endif
#ifdef __LANGUAGE_DUTCH__
"Fitheidstest",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Teste De Aptidao",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Kiem Tra The Luc",
#endif
    },//STR_FITNESS_TEST_ID
    
    {
		"GUEST",
#ifdef __LANGUAGE_SPANISH__
"Invitado",
#endif
#ifdef __LANGUAGE_FRENCH__
"Invite",
#endif
#ifdef __LANGUAGE_ITALIAN__
"Ospite",
#endif
#ifdef __LANGUAGE_GERMAN__
"Gast",
#endif
#ifdef __LANGUAGE_DUTCH__
"Gast",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"Convidado",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"Khach",
#endif
    },//STR_GUEST_ID
    {
        //"Huangmingzhu Menggaoling Fufangpeng ",
		"USER 1 ",
#ifdef __LANGUAGE_SPANISH__
"USUARIO 1 ",
#endif
#ifdef __LANGUAGE_FRENCH__
"UTLISATEUR 1 ",
#endif
#ifdef __LANGUAGE_ITALIAN__
"UTENTE 1 ",
#endif
#ifdef __LANGUAGE_GERMAN__
"BENUTZER 1 ",
#endif
#ifdef __LANGUAGE_DUTCH__
"GEBRUIKER 1 ",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"USUaRIO 1 ",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"NGUOI DUNG 1 ",
#endif
    },//STR_USER1_ID
    
    {
		"USER 2 ",
#ifdef __LANGUAGE_SPANISH__
"USUARIO 2 ",
#endif
#ifdef __LANGUAGE_FRENCH__
"UTLISATEUR 2 ",
#endif
#ifdef __LANGUAGE_ITALIAN__
"UTENTE 2 ",
#endif
#ifdef __LANGUAGE_GERMAN__
"BENUTZER 2 ",
#endif
#ifdef __LANGUAGE_DUTCH__
"GEBRUIKER 2 ",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"USUaRIO 2 ",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"NGUOI DUNG 2 ",
#endif
    },//STR_USER2_ID
    
    {
		"USER 3 ",
#ifdef __LANGUAGE_SPANISH__
"USUARIO 3 ",
#endif
#ifdef __LANGUAGE_FRENCH__
"UTLISATEUR 3 ",
#endif
#ifdef __LANGUAGE_ITALIAN__
"UTENTE 3 ",
#endif
#ifdef __LANGUAGE_GERMAN__
"BENUTZER 3 ",
#endif
#ifdef __LANGUAGE_DUTCH__
"GEBRUIKER 3 ",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"USUaRIO 3 ",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"NGUOI DUNG 3 ",
#endif
    },//STR_USER3_ID
    
    {
		"USER 4 ",
#ifdef __LANGUAGE_SPANISH__
"USUARIO 4 ",
#endif
#ifdef __LANGUAGE_FRENCH__
"UTLISATEUR 4 ",
#endif
#ifdef __LANGUAGE_ITALIAN__
"UTENTE 4 ",
#endif
#ifdef __LANGUAGE_GERMAN__
"BENUTZER 4 ",
#endif
#ifdef __LANGUAGE_DUTCH__
"GEBRUIKER 4 ",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"USUaRIO 4 ",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"NGUOI DUNG 4 ",
#endif
    },//STR_USER4_ID
    
    {
		"LUBE BELT",
#ifdef __LANGUAGE_SPANISH__
"LUBE BELT",
#endif
#ifdef __LANGUAGE_FRENCH__
"LUBE BELT",
#endif
#ifdef __LANGUAGE_ITALIAN__
"LUBE BELT",
#endif
#ifdef __LANGUAGE_GERMAN__
"LUBE BELT",
#endif
#ifdef __LANGUAGE_DUTCH__
"LUBE BELT",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"LUBE BELT",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"LUBE BELT",
#endif
    },//STR_LUBE_BELT_ID
    
    {
        "LBS",
#ifdef __LANGUAGE_SPANISH__
"LBS",
#endif
#ifdef __LANGUAGE_FRENCH__
"LBS",
#endif
#ifdef __LANGUAGE_ITALIAN__
"LBS",
#endif
#ifdef __LANGUAGE_GERMAN__
"LBS",
#endif
#ifdef __LANGUAGE_DUTCH__
"LBS",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"LBS",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"LBS",
#endif
    },//STR_LBS_ID
    
    {
        "KG",
#ifdef __LANGUAGE_SPANISH__
"KG",
#endif
#ifdef __LANGUAGE_FRENCH__
"KG",
#endif
#ifdef __LANGUAGE_ITALIAN__
"KG",
#endif
#ifdef __LANGUAGE_GERMAN__
"KG",
#endif
#ifdef __LANGUAGE_DUTCH__
"KG",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"KG",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"KG",
#endif
    },//STR_KG_ID
    {
        "IR SENSOR ERROR"
    },//STR_IR_SENSOR_ERROR_ID
    {
        "CONTROL ZONE ERROR"
    },//STR_IR_SENSOR_ERROR_ID
    {
        "UCB EMERGENCY"
    },//STR_IR_SENSOR_ERROR_ID
    {
        "HANDRAIL EMERGENCY"
    },//STR_IR_SENSOR_ERROR_ID
    {
        "0",
    },/*STR_0_TITLE_ID*/
    {
        "1",
    },/*STR_1_TITLE_ID*/
    {
        "2",
    },/*STR_2_TITLE_ID*/
    {
        "3",
    },/*STR_3_TITLE_ID*/
    {
        "4",
    },/*STR_4_TITLE_ID*/
    {
        "5",
    },/*STR_5_TITLE_ID*/
    {
        "6",
    },/*STR_6_TITLE_ID*/
    {
        "7",
    },/*STR_7_TITLE_ID*/// 140
    {
        "8",
    },/*STR_8_TITLE_ID*/
    {
        "9",
    },/*STR_9_TITLE_ID*/
    
    /*Below is used for ENG */
    {
        "ENG0",
    },/*STR_ENG0_ID*/
    {
        "ENG1",
    },/*STR_ENG1_ID*/
    {
        "ENG2",
    },/*STR_ENG2_ID*/
    {
        "ENG3",
    },/*STR_ENG3_ID*/
    {
        "ENG4",
    },/*STR_ENG4_ID*/
    {
        "ENG5",
    },/*STR_ENG5_ID*/
    {
        "ENG8",
    },/*STR_ENG8_ID*/
    
    {
        "DISPLAY TEST",
    },/*STR_DISPLAY_TEST_ID*/
    
    {
        "HARDWARE TEST",
    },/*STR_HARDWARE_TEST_ID*/
    
    {
        "NOT AVAILABLE",
    },/*STR_NOT_AVAILABLE_ID*/
    
    {
        "SWITCH FUNCTION",
    },/*STR_SWITCH_FUNCATION_ID*/
    
    {
        "INFORMATION",
    },/*STR_INFORMATION_ID*/
    
    {
        "ENERGY SAVE ON",
    },/*STR_ENERGY_SAVE_ON_ID*/
    
    {
        "ENERGY SAVE OFF",
    },/*STR_ENERGY_SAVE_OFF_ID*/
    
    {
        "TEST",
    },/*STR_TEST_ID*/
    
    {
        "QUICK",
    },/*STR_QUICK_ID*/
    
    {
        "NORMAL",
    },/*STR_NORMAL_ID*/
    
    {
        "NO CLEAR",
    },/*STR_NO_CLEAR_ID*/
    
    {
        "CLEAR",
    },/*STR_CLEAR_ID*/
    
    {
        "PRESS START ENTER ERP",
    },/*STR_PRESS_START_ENTER_ERP_ID*/
    
    {
        "RPM OFF",
    },/*STR_RPM_OFF_ID*/
    
    {
        "RPM ON",
    },/*STR_RPM_ON_ID*/
    
    {
        "Wifi Test",
    },/*STR_WIFI_TEST_ID*/
    {
        "AUTO Cailbration",
    },/*STR_AUTO_ADC_ID*/
    {
        "Auto Calibration PASS",
    },/*STR_AUTO_INCLINE_PASS*/
    {
        "Auto Calibration FAIL",
    },/*STR_AUTO_INCLINE_FAIL*/
    {
        "WIFI IS OK",
    },/*STR_WIFI_IS_OK_ID*/
    
    {
        "No IP Address" ,
    },/*STR_NO_IP_ADDRESS_ID*/
    
    {
        "No MAC Address" ,
    },//STR_NO_MAC_ADDRESS_ID
    {
        "wifi test fail" ,
    },//STR_WIFI_TEST_FAIL_ID
    
    {
        "set dapi",
    },//STR_SET_DAPI_ID,
    {
        "production",
    },//STR_PRODUCTION_ID,
    {
        "QA",
    },//STR_QA_ID,
    {
        "staging",
    },//STR_STAGING_ID,
    {
        "dev",
    },//STR_DEV_ID,
    {
        "USB TEST",
    },//STR_USB_TEST_ID,
    {
        "USB TESTING",
    },// STR_USB_TESTING_ID
    {
        "USB TEST PASS",
    },//STR_USB_TEST_PASS_ID
    {
        "USB TEST FAIL",
    },//STR_USB_TEST_FAIL_ID
    {
        "WIFI TEST FAIL",
    },//STR_WIFITESTFAIL_ID,
//    {
//        "P1",
//    },//STR_P1_ID
//    {
//        "P2",
//    },//STR_P2_ID
//    {
//        "P3",
//    },//STR_P3_ID
//    {
//        "P4",
//    },//STR_P4_ID
//    {
//        "P5",
//    },//STR_P5_ID
//    {
//        "P6",
//    },//STR_P6_ID
//    {
//        "P7",
//    },//STR_P7_ID
//    {
//        "P8",
//    },//STR_P8_ID
//    {
//        "P9",
//    },//STR_P9_ID
//    {
//        "P10",
//    },//STR_P10_ID
    {
        "BOOT ON",
    },//STR_BOOT_ON_ID
    {
        "BOOT OFF",
    },//STR_BOOT_OFF_ID
    {
        "OVER SPEED ON",
    },// STR_OVERSPEED_ON_ID
    {
        "OVER SPEED OFF",
    },// STR_OVERSPEED_OFF_ID
	
    {
        "ERRORCODE ON",
    },// STR_ERRORCODE_ON_ID
    {
        "ERRORCODE OFF",
    },// STR_ERRORCODE_OFF_ID
    {
        "FIRST CONTROL ZONE ON",
    },// STR_CONTROL_ZONE_ON_ID
    {
        "FIRST CONTROL ZONE OFF",
    },// STR_CONTROL_ZONE_OFF_ID 
    {
        "SECOND CONTROL ZONE ON",
    },//STR_SECOND_CONTROL_ZONE_ON_ID
    {
        "SECOND CONTROL ZONE OFF",
    },//STR_SECOND_CONTROL_ZONE_OFF_ID
    {
        "FIRST",
    },// STR_1ST_CONTROL_ZONE_ID
    {
        "SECOND",
    },// STR_2ND_CONTROL_ZONE_ID
    {
        "REAL",
    },// STR_REAL_CONTROL_ZONE_ID
    {
        "TARGET",
    },// STR_TRAGET_CONTROL_ZONE_ID
    {
        "IR SENSOR ON",
    },// STR_IR_SENSOR_ON_ID
    {
        "IR SENSOR OFF",
    },// STR_IR_SENSOR_OFF_ID
    {
        "MAX LEVEL",
    },// STR_MAX_LEVEL_ID
    
    
    
    
    {
        "ACC INFO",
    },//STR_ACC_INFO_ID
    
    {
        "SET ERP TIME",
    },//STR_SET_ERP_TIME_ID
    {
        "SET over speed",
    },//STR_SET_OVER_SPEED_ID
    {
        "SET RPM",
    },//STR_SET_RPM_ID
    
    {
        "ADC",
    },//STR_ADC_ID
    {
        "ECB",
    },//STR_ECB_ID
    {
        "PWM",
    },//STR_PWM_ID
    
    {
        "accumulated inf0",
    },//STR_ACCUMULATED_INFO_ID,
    {
        "ucb version",
    },//STR_UCB_VERSION_ID,
    {
        "mcb version",
    },//STR_MCB_VERSION_ID,
    
    {
        "reserved",
    },//STR_RESERVED_ID,
    {
        "Machine",
    },//STR_MACHINE_ID,
    {
        "language",
    },//STR_LANGUAGE_ID,
    {
        "unit",
    },//STR_UNIT_ID,
    
    {
        "update from usb? Y/N", 
    },//STR_UPDATE_FROM_USB_Y_N_ID,
    
    {
        " ",
    }, //STR_NULL_ID
	{
		"ACCUMULATE AGING HOURS",
	},//STR_ACCUMULATE_AGING_HOURS_ID
    {
        "KEY TEST",
    },//STR_KEY_TEST_ID
    {
        "SPEED RANGE ERROR",
    },//STR_SPEED_RANGE_ERROR_ID
        {
		"LANGUAGE UPDATE PRESS ENTER",
#ifdef __LANGUAGE_SPANISH__
"ACTUALIZACION DE SOFTWARE. PULSE ENTER.",
#endif
#ifdef __LANGUAGE_FRENCH__
"MISE A JOUR DU LOGICIEL APPUYEZ SUR ENTREE",
#endif
#ifdef __LANGUAGE_ITALIAN__
"AGGIORNAMENTO SOFTWARE, PREMI INVIO",
#endif
#ifdef __LANGUAGE_GERMAN__
"SOFTWARE-UPDATE: EINGABETASTE DRUCKEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"SOFTWARE-UPDATE DRUK OP ENTER",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
"ATUALIZAR SOFTWARE. APERTE ENTER",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
"CAP NHAT PHAN MEM, NHAN ENTER",
#endif
    },//STR_LANGUAGE_UPDATE_PRESS_ENTER_ID
    {
        "language updateing ...",
    },//STR_LANGUAGE_UPDATEING_ID
};

const UCHAR* const PX_Strings[PX_STRINGIDEND]=
{
    "P1",//STR_P1_ID
    "P2",//STR_P2_ID
    "P3",//STR_P3_ID
    "P4",//STR_P4_ID
    "P5",//STR_P5_ID
    "P6",//STR_P6_ID
    "P7",//STR_P7_ID
    "P8",//STR_P8_ID
    "P9",//STR_P9_ID
    "P10",//STR_P10_ID
    "RSCU ON",
    "RSCU OFF",
};

static unsigned  char gReadLangFlag = 0;
unsigned char String_Get_Flag(void)
{
    return gReadLangFlag;
}



char* String_Get_String( STRINGID StringID )
{
    Language_GetLanguageAmount();
    char* p;
    if(by_languageID == _ENGLISH)
    {
        p =  (char*)Strings[StringID][_ENGLISH];
    }
    else
    {
        gReadLangFlag = 1;
        if(Language_GetString(_Lang_Language_Data_,StringID+1)==_Lang_Success_)
        {
            p = (Lang_StringBuffer);
        }
        else
        {
            p = (char*)Strings[StringID][_ENGLISH];
        }
           
        gReadLangFlag = 0;
    }
    
    return p;
}
char* PxString_Get_PxString( PX_STRINGID StringID )
{
    return (char*)PX_Strings[StringID];
}
void String_Set_Language( UCHAR languageID )
{
    by_languageID = ( LANGUAGEID ) languageID;
}

UCHAR String_Get_LanguageIndex(void)
{
    return by_languageID;
}

void String_Init_Data( void )
{
   	by_languageID = ( LANGUAGEID )SystemConfig_Get_Char(LANGUAGE_ID_UINT8);
}



