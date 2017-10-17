#include "Strings.h"
#include "SystemConfig.h"

__no_init static LANGUAGEID by_languageID;
const UCHAR* const Strings[STRINGIDEND][LANGUAGEIDEND]={
    
    {
	 "INITIAL",
#ifdef __LANGUAGE_FRENCH__
	"INITIAL for french",
#endif
#ifdef __LANGUAGE_GERMAN__
	"INITIAL for German",
#endif
#ifdef __LANGUAGE_DUTCH__
	"INITIAL for dutch",
#endif
#ifdef __LANGUAGE_SPANISH__
	"INITIAL for spanish",
#endif
#ifdef __LANGUAGE_ITALIAN__
	"INITIAL for italian",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
	"INITIAL for portuguese",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
	"INITIAL for virtnamese",
#endif
    },/*STR_INITIAL_ID*/	
    
    {
	 "HR",
#ifdef __LANGUAGE_FRENCH__
	"HR for french",
#endif
#ifdef __LANGUAGE_GERMAN__
	"HR for German",
#endif
#ifdef __LANGUAGE_DUTCH__
	"HR for dutch",
#endif
#ifdef __LANGUAGE_SPANISH__
	"HR for spanish",
#endif
#ifdef __LANGUAGE_ITALIAN__
	"HR for italian",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
	"HR for portuguese",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
	"HR for virtnamese",
#endif
    },/*STR_HEARTRATEMETER_ID*/

    {
        "PASSPORT READY",
#ifdef __LANGUAGE_FRENCH__
"PASSPORT PRET",
#endif
#ifdef __LANGUAGE_GERMAN__
"Passport Ready",
#endif
#ifdef __LANGUAGE_DUTCH__
"Passport klaar",
#endif
#ifdef __LANGUAGE_SPANISH__
"Passport esta listo",
#endif
    },/*STR_PASSPORTREADY_ID*/
    
    {
        "RF SYNC",
#ifdef __LANGUAGE_FRENCH__
"SYNC FR",
#endif
#ifdef __LANGUAGE_GERMAN__
"RF SYNC",
#endif
#ifdef __LANGUAGE_DUTCH__
"RF SYNC",
#endif
#ifdef __LANGUAGE_SPANISH__
"SINCRONIZACION DE RF",
#endif
    },/*STR_RFSYNC_ID*/
    
    {
	 "SELECT USER",//"SELECT USER",
#ifdef __LANGUAGE_FRENCH__
	"SELECT USER for french",
#endif
#ifdef __LANGUAGE_GERMAN__
	"SELECT USER for German",
#endif
#ifdef __LANGUAGE_DUTCH__
	"SELECT USER for dutch",
#endif
#ifdef __LANGUAGE_SPANISH__
	"SELECT USER for spanish",
#endif
#ifdef __LANGUAGE_ITALIAN__
	"SELECT USER for italian",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
	"SELECT USER for portuguese",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
	"SELECT USER for virtnamese",
#endif
    },/*STR_SELECTUSER_ID*/
      
       {
        "UPDATING USER INFO",
#ifdef __LANGUAGE_FRENCH__
	"UPDATING for french",
#endif
#ifdef __LANGUAGE_GERMAN__
	"UPDATING for German",
#endif
#ifdef __LANGUAGE_DUTCH__
	"UPDATING for dutch",
#endif
#ifdef __LANGUAGE_SPANISH__
	"UPDATING for spanish",
#endif
#ifdef __LANGUAGE_ITALIAN__
	"UPDATING for italian",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
	"UPDATING for portuguese",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
	"UPDATING for virtnamese",
#endif    
},/*STR_UPDATE_USER_ID*/
      {
        "SET TIMEZONE",
#ifdef __LANGUAGE_FRENCH__
"SET TIMEZONE",
#endif
#ifdef __LANGUAGE_GERMAN__
"SET TIMEZONE",
#endif
#ifdef __LANGUAGE_DUTCH__
"SET TIMEZONE",
#endif
#ifdef __LANGUAGE_SPANISH__
"SET TIMEZONE",
#endif
    },/*STR_TIMEZONE_ID*/
    
    {
        "GUEST",
#ifdef __LANGUAGE_FRENCH__
	"GUEST for french",
#endif
#ifdef __LANGUAGE_GERMAN__
	"GUEST for German",
#endif
#ifdef __LANGUAGE_DUTCH__
	"GUEST for dutch",
#endif
#ifdef __LANGUAGE_SPANISH__
	"GUEST for spanish",
#endif
#ifdef __LANGUAGE_ITALIAN__
	"GUEST for italian",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
	"GUEST for portuguese",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
	"GUEST for virtnamese",
#endif  
    },/*STR_GUEST_ID*/
    
    {
        "USER 1",
#ifdef __LANGUAGE_FRENCH__
	"USER1 for french",
#endif
#ifdef __LANGUAGE_GERMAN__
	"USER1 for German",
#endif
#ifdef __LANGUAGE_DUTCH__
	"USER1 for dutch",
#endif
#ifdef __LANGUAGE_SPANISH__
	"USER1 for spanish",
#endif
#ifdef __LANGUAGE_ITALIAN__
	"USER1 for italian",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
	"USER1 for portuguese",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
	"GUEST for virtnamese",
#endif  
    },/*STR_USER1_ID*/ // 10
    {
        "USER 2",
#ifdef __LANGUAGE_FRENCH__
	"USER2 for french",
#endif
#ifdef __LANGUAGE_GERMAN__
	"USER2 for German",
#endif
#ifdef __LANGUAGE_DUTCH__
	"USER2 for dutch",
#endif
#ifdef __LANGUAGE_SPANISH__
	"USER2 for spanish",
#endif
#ifdef __LANGUAGE_ITALIAN__
	"USER2 for italian",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
	"USER2 for portuguese",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
	"GUEST for virtnamese",
#endif 
    },/*STR_USER2_ID*/
    {
        "USER 3",
#ifdef __LANGUAGE_FRENCH__
	"USER3 for french",
#endif
#ifdef __LANGUAGE_GERMAN__
	"USER3 for German",
#endif
#ifdef __LANGUAGE_DUTCH__
	"USER3 for dutch",
#endif
#ifdef __LANGUAGE_SPANISH__
	"USER3 for spanish",
#endif
#ifdef __LANGUAGE_ITALIAN__
	"USER3 for italian",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
	"USER3 for portuguese",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
	"GUEST for virtnamese",
#endif 
    },/*STR_USER3_ID*/
    {
        "USER 4",
#ifdef __LANGUAGE_FRENCH__
	"USER4 for french",
#endif
#ifdef __LANGUAGE_GERMAN__
	"USER4 for German",
#endif
#ifdef __LANGUAGE_DUTCH__
	"USER4 for dutch",
#endif
#ifdef __LANGUAGE_SPANISH__
	"USER4 for spanish",
#endif
#ifdef __LANGUAGE_ITALIAN__
	"USER4 for italian",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
	"USER4 for portuguese",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
	"USER4 for virtnamese",
#endif 
    },/*STR_USER4_ID*/
{
        "USER 5",
#ifdef __LANGUAGE_FRENCH__
	"USER5 for french",
#endif
#ifdef __LANGUAGE_GERMAN__
	"USER5 for German",
#endif
#ifdef __LANGUAGE_DUTCH__
	"USER5 for dutch",
#endif
#ifdef __LANGUAGE_SPANISH__
	"USER5 for spanish",
#endif
#ifdef __LANGUAGE_ITALIAN__
	"USER5 for italian",
#endif
#ifdef __LANGUAGE_PORTUGUESE__
	"USER5 for portuguese",
#endif
#ifdef __LANGUAGE_VIETNAMESE__
	"USER5 for virtnamese",
#endif 
    },/*STR_USER5_ID*/
{
        "CONNECTING ",
#ifdef __LANGUAGE_FRENCH__
"CONNECTING ",
#endif
#ifdef __LANGUAGE_GERMAN__
"CONNECTING ",
#endif
#ifdef __LANGUAGE_DUTCH__
"CONNECTING ",
#endif
#ifdef __LANGUAGE_SPANISH__
"CONNECTING ",
#endif
    },/*STR_CONNECTING_ID*/  
//{
//        "SYNC USER INFO",
//#ifdef __LANGUAGE_FRENCH__
//"SYNC USER INFO ",
//#endif
//#ifdef __LANGUAGE_GERMAN__
//"SYNC USER INFO ",
//#endif
//#ifdef __LANGUAGE_DUTCH__
//"SYNC USER INFO ",
//#endif
//#ifdef __LANGUAGE_SPANISH__
//"SYNC USER INFO ",
//#endif
//    },/*STR_SYNC_USER_ID*/   
    
    {
        "set WEIGHT",
#ifdef __LANGUAGE_FRENCH__
"SAISIR LE POIDS",
#endif
#ifdef __LANGUAGE_GERMAN__
"GEWICHT AUSWAEHLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"GEWICHT KIEZEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"ESTABLECER PESO",
#endif
    },/*STR_SELECTWEIGHT_ID*/
    
    {
        "set AGE",
#ifdef __LANGUAGE_FRENCH__
"SAISIR LAGE",
#endif
#ifdef __LANGUAGE_GERMAN__
"ALTER AUSWAEHLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"LEEFTIJD KIEZEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"ESTABLECER EDAD",
#endif
    },/*STR_SELECTAGE_ID*/
    
    {
        "set gender",
#ifdef __LANGUAGE_FRENCH__
"SELECTIONNER LE SEXE",
#endif
#ifdef __LANGUAGE_GERMAN__
"Geschlecht auswaehlen",
#endif
#ifdef __LANGUAGE_DUTCH__
"geslacht kiezen",
#endif
#ifdef __LANGUAGE_SPANISH__
"seleccionar genero",
#endif
    },/*STR_SELECTGENDER_ID*/
    
    {
        "female",
#ifdef __LANGUAGE_FRENCH__
"FEMME",
#endif
#ifdef __LANGUAGE_GERMAN__
"weiblich",
#endif
#ifdef __LANGUAGE_DUTCH__
"VROUW",
#endif
#ifdef __LANGUAGE_SPANISH__
"Femenino",
#endif
    },/*STR_FEMALE_ID*/
    
    {
        "male",
#ifdef __LANGUAGE_FRENCH__
"Homme",
#endif
#ifdef __LANGUAGE_GERMAN__
"maennlich",
#endif
#ifdef __LANGUAGE_DUTCH__
"MAN",
#endif
#ifdef __LANGUAGE_SPANISH__
"Masculino",
#endif
    },/*STR_MALE_ID*/
    
    {
        "SET TIME",
#ifdef __LANGUAGE_FRENCH__
"SAISIR LA DUREE",
#endif
#ifdef __LANGUAGE_GERMAN__
"ZEIT AUSWAEHLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"TIJD KIEZEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"ESTABLECER TIEMPO",
#endif
    },/*STR_SELECTTIME_ID*/ // 20
    
    {
        "SET SPEED",
#ifdef __LANGUAGE_FRENCH__
"SELECT SPEED",
#endif
#ifdef __LANGUAGE_GERMAN__
"SELECT SPEED",
#endif
#ifdef __LANGUAGE_DUTCH__
"SELECT SPEED",
#endif
#ifdef __LANGUAGE_SPANISH__
"SELECT SPEED",
#endif
    },/*STR_SELECTSPEED_ID*/
    
    {
        "SET INCLINE",
#ifdef __LANGUAGE_FRENCH__
"SELECT INCLINE",
#endif
#ifdef __LANGUAGE_GERMAN__
"SELECT INCLINE",
#endif
#ifdef __LANGUAGE_DUTCH__
"SELECT INCLINE",
#endif
#ifdef __LANGUAGE_SPANISH__
"SELECT INCLINE",
#endif
    },/*STR_SELECTINCLINE_ID*/
    
    {
        "SET RESISTANCE",
#ifdef __LANGUAGE_FRENCH__
"SAISIR LA RESISTANCE",
#endif
#ifdef __LANGUAGE_GERMAN__
"WIDERSTAND AUSWAEHLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"WEERSTAND KIEZEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"ESTABLECER RESISTENCIA",
#endif
    },/*STR_SELECTRESISTANCE_ID*/
    
    {
        "SET LEVEL",
#ifdef __LANGUAGE_FRENCH__
"SAISIR LE NIVEAU",
#endif
#ifdef __LANGUAGE_GERMAN__
"LEVEL AUSWAEHLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"LEVEL KIEZEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"ESTABLECER NIVEL",
#endif
    },/*STR_SELECTLEVEL_ID*/
    
    {
        "CONSTANT WATTS",
#ifdef __LANGUAGE_FRENCH__
"WATTS CONSTANT",
#endif
#ifdef __LANGUAGE_GERMAN__
"Konstante WATT",
#endif
#ifdef __LANGUAGE_DUTCH__
"CONSTANT Wattage",
#endif
#ifdef __LANGUAGE_SPANISH__
"vatios constantes",
#endif
    },/*STR_CONSTWATTS_ID*/
    
    {
        "CUSTOM 1",
#ifdef __LANGUAGE_FRENCH__
"PERSONNALISE 1",
#endif
#ifdef __LANGUAGE_GERMAN__
"BENUTZERDEFINIERT 1",
#endif
#ifdef __LANGUAGE_DUTCH__
"GEBRUIKER 1",
#endif
#ifdef __LANGUAGE_SPANISH__
"PERSONALIZADO 1",
#endif
    },/*STR_CUSTOM1_ID*/
    
    {
        "MANUAL",
#ifdef __LANGUAGE_FRENCH__
"MANUEL",
#endif
#ifdef __LANGUAGE_GERMAN__
"MANUELL",
#endif
#ifdef __LANGUAGE_DUTCH__
"HANDMATIG",
#endif
#ifdef __LANGUAGE_SPANISH__
"Manual",
#endif
    },/*STR_MANUAL_ID*/
    
    {
        "INTERVAL WATTS",
#ifdef __LANGUAGE_FRENCH__
"WATT INTERVALLE",
#endif
#ifdef __LANGUAGE_GERMAN__
"INTERVALL WATT",
#endif
#ifdef __LANGUAGE_DUTCH__
"INTERVAL WATT",
#endif
#ifdef __LANGUAGE_SPANISH__
"VATIOS DE INTERVALO",
#endif
    },/*STR_INTERVALWATTS_ID*/
    
    {
        "TARGET HRT RATE",
#ifdef __LANGUAGE_FRENCH__
"ZONE FCC",
#endif
#ifdef __LANGUAGE_GERMAN__
"ZHF ZONE",
#endif
#ifdef __LANGUAGE_DUTCH__
"DHS ZONE",
#endif
#ifdef __LANGUAGE_SPANISH__
"ZONA RCM",
#endif
    },/*STR_THRZONE_ID*/
    
    {
        "% THR",
#ifdef __LANGUAGE_FRENCH__
"% FCC",
#endif
#ifdef __LANGUAGE_GERMAN__
"% ZHF",
#endif
#ifdef __LANGUAGE_DUTCH__
"% DHS",
#endif
#ifdef __LANGUAGE_SPANISH__
"% RCM",
#endif
    },/*STR_THRPERCENT_ID*/// 30
    
    {
        "CUSTOM 2",
#ifdef __LANGUAGE_FRENCH__
"PERSONNALISE 2",
#endif
#ifdef __LANGUAGE_GERMAN__
"BENUTZERDEFINIERT 2",
#endif
#ifdef __LANGUAGE_DUTCH__
"GEBRUIKER 2",
#endif
#ifdef __LANGUAGE_SPANISH__
"PERSONALIZADO 2",
#endif
    },/*STR_CUSTOM2_ID*/
    
    {
        "SELECT WATT INTERVAL 1",
#ifdef __LANGUAGE_FRENCH__
"SAISIR L INTERVALLE DE WATTS 1",
#endif
#ifdef __LANGUAGE_GERMAN__
"WATTINTERVALL 1 AUSWAEHLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"KIES WATTINTERVAL 1",
#endif
#ifdef __LANGUAGE_SPANISH__
"SELECCIONAR INTERVALO DE VATIO 1",
#endif
    },/*STR_SELECTINTERVALWATTS_ID*/
    
    {
        "SET TARGET WATTS",
#ifdef __LANGUAGE_FRENCH__
"SAISIR LE NOMBRE DE WATTS CIBLE",
#endif
#ifdef __LANGUAGE_GERMAN__
"ZIELWATT AUSWAEHLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"DOELWATT KIEZEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"SELECCIONAR OBJETIVO DE VATIOS",
#endif
    },/*STR_SELECTTARGETWATTS_ID*/
    
    {
        "SELECT WATT INTERVAL 2",
#ifdef __LANGUAGE_FRENCH__
"SAISIR L INTERVALLE DE WATTS 2",
#endif
#ifdef __LANGUAGE_GERMAN__
"WATTINTERVALL 2 AUSWAEHLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"KIES WATTINTERVAL 2",
#endif
#ifdef __LANGUAGE_SPANISH__
"SELECCIONAR INTERVALO DE VATIO 2",
#endif
    },/*STR_SELECTINTERVALWATTS2_ID*/
    
    {
        "SELECT HEART RATE PERCENT",
#ifdef __LANGUAGE_FRENCH__
"SELECTIONNER LA FREQUENCE CARDIAQUE EN %",
#endif
#ifdef __LANGUAGE_GERMAN__
"HERZFREQUENZ PROZENT AUSWAEHLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"KIES HARTSLAG PROCENT",
#endif
#ifdef __LANGUAGE_SPANISH__
"SELECCIONAR PORCENTAJE DE RITMO CARDIACO",
#endif
    },/*STR_SELECTHEARTRATEPERCENT_ID*/
    
    {
        "SELECT TARGET HEART RATE",
#ifdef __LANGUAGE_FRENCH__
"ENTRER FREQUENCE CARDIAQUE CIBLE",
#endif
#ifdef __LANGUAGE_GERMAN__
"ZIELHERZFREQUENZ AUSWAEHLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"DOELHARTSLAG KIEZEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"ESTABLECER RITMO CARDIACO OBJETIVO",
#endif
    },/*STR_TARGETHEARTRATEPERCENT_ID*/
    
    {
        "PRESS START",
#ifdef __LANGUAGE_FRENCH__
"APPUYER SUR START",
#endif
#ifdef __LANGUAGE_GERMAN__
"START DRUECKEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"START INDRUKKEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"OPRIMIR START",
#endif
    },/*STR_PRESSSTART_ID*/
    
    {
        "DISPLAY TEST",
#ifdef __LANGUAGE_FRENCH__
"ESSAI DAFFICHAGE",
#endif
#ifdef __LANGUAGE_GERMAN__
"ANZEIGETEST",
#endif
#ifdef __LANGUAGE_DUTCH__
"TEST WEERGEVEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"VER TEST",
#endif
    },/*STR_DISPLAYTEST_ID*/
    
    {
        "HARDWARE TEST",
#ifdef __LANGUAGE_FRENCH__
"ESSAI DE MATERIEL",
#endif
#ifdef __LANGUAGE_GERMAN__
"HARDWARE TEST",
#endif
#ifdef __LANGUAGE_DUTCH__
"HARDWARE TEST",
#endif
#ifdef __LANGUAGE_SPANISH__
"test de hardware",
#endif
    },/*STR_HARDWARE_ID*/
    {
        "NOT AVAILABLE",
#ifdef __LANGUAGE_FRENCH__
"NOT AVAILABLE",
#endif
#ifdef __LANGUAGE_GERMAN__
"NOT AVAILABLE",
#endif
#ifdef __LANGUAGE_DUTCH__
"NOT AVAILABLE",
#endif
#ifdef __LANGUAGE_SPANISH__
"NOT AVAILABLE",
#endif
    },/*STR_HARDWARE_ID*/// 40
    {
        "SWITCH FUNCTION",
#ifdef __LANGUAGE_FRENCH__
"CHANGER DE FONCTION",
#endif
#ifdef __LANGUAGE_GERMAN__
"FUNKTION WECHSELN",
#endif
#ifdef __LANGUAGE_DUTCH__
"FUNCTIE WISSELEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"CAMBIAR FUNCION",
#endif
    },/*STR_SWITCHFUNCATION_ID*/
    
    {
        "INFORMATION",
#ifdef __LANGUAGE_FRENCH__
"INFORMATION",
#endif
#ifdef __LANGUAGE_GERMAN__
"INFORMATION",
#endif
#ifdef __LANGUAGE_DUTCH__
"INFORMATIE",
#endif
#ifdef __LANGUAGE_SPANISH__
"Informacion",
#endif
    },/*STR_INFORMATION_ID*/
#ifdef TM
    {
        "INCLINE SPEED"
    },
#elif  (defined EP)	
	{
        "RPM   RESIS"
    },/*STR_HARDWARE1_ID*/
#else
    {
        "INC   RESIS"
    },/*STR_HARDWARE1_ID*/
#endif
    {
        "KM",
#ifdef __LANGUAGE_FRENCH__
"METRIQUE",
#endif
#ifdef __LANGUAGE_GERMAN__
"METRISCH",
#endif
#ifdef __LANGUAGE_DUTCH__
"METRISCH",
#endif
#ifdef __LANGUAGE_SPANISH__
"METRICO",
#endif
    },/*STR_METRIC_ID*/
    
    {
        "MILE",
#ifdef __LANGUAGE_FRENCH__
"STANDARD",
#endif
#ifdef __LANGUAGE_GERMAN__
"STANDARD",
#endif
#ifdef __LANGUAGE_DUTCH__
"STANDAARD",
#endif
#ifdef __LANGUAGE_SPANISH__
"ESTANDAR",
#endif
    },/*STR_STANDARD_ID*/
        {
        "English",
#ifdef __LANGUAGE_FRENCH__
"Anglais",
#endif
#ifdef __LANGUAGE_GERMAN__
"Englisch",
#endif
#ifdef __LANGUAGE_DUTCH__
"ENGELS",
#endif
#ifdef __LANGUAGE_SPANISH__
"English",
#endif
    },/*STR_ENGLISH_ID*/
        {
        "French",
#ifdef __LANGUAGE_FRENCH__
"FRANCAIS",
#endif
#ifdef __LANGUAGE_GERMAN__
"Franzoesisch",
#endif
#ifdef __LANGUAGE_DUTCH__
"FRANS",
#endif
#ifdef __LANGUAGE_SPANISH__
"FRANCES",
#endif
    },/*STR_FRENCH_ID*/
//    {
//        "German",
//#ifdef __LANGUAGE_FRENCH__
//"Allemand",
//#endif
//#ifdef __LANGUAGE_GERMAN__
//"Deutsch",
//#endif
//#ifdef __LANGUAGE_DUTCH__
//"DUITS",
//#endif
//#ifdef __LANGUAGE_SPANISH__
//"ALEMAN",
//#endif
//    },/*STR_GERMAN_ID*/
//        {
//        "Dutch",
//#ifdef __LANGUAGE_FRENCH__
//"HOLLANDAIS",
//#endif
//#ifdef __LANGUAGE_GERMAN__
//"NIEDERLAENDISCH",
//#endif
//#ifdef __LANGUAGE_DUTCH__
//"NEDERLANDS",
//#endif
//#ifdef __LANGUAGE_SPANISH__
//"HOLANDES",
//#endif
//
//    },/*STR_DUTCH_ID*/    
    {
        "SPANISH",
#ifdef __LANGUAGE_FRENCH__
"ESPAGNOL",
#endif
#ifdef __LANGUAGE_GERMAN__
"SPANISCHE",
#endif
#ifdef __LANGUAGE_DUTCH__
"SPAANS",
#endif
#ifdef __LANGUAGE_SPANISH__
"ESPANOL",
#endif
    },/*STR_SPANISH_ID*/// 80
//        {
//        "ITALIAN",
//    },/*STR_ITALIAN_ID*/    
//    {
//        "PORTUGUESE",
//    },/*STR_PORTUGUESE_ID*/
//     {
//        "VIETNAMESE",
//    },/*STR_VIETNAMESE_ID*/   
    {
        "ENERGY SAVE ON",
#ifdef __LANGUAGE_FRENCH__
"eCONOMISEUR DeNERGIE ACTIVe",
#endif
#ifdef __LANGUAGE_GERMAN__
"ENERGIESPARMODUS EIN",
#endif
#ifdef __LANGUAGE_DUTCH__
"ENERGIEBESPARENDE MODUS AAN",
#endif
#ifdef __LANGUAGE_SPANISH__
"AHORRO DE ENERGiA ACTIVADO",
#endif
    },/*STR_ENERGYSAVEON_ID*/
    
    {
        "ENERGY SAVE OFF",
#ifdef __LANGUAGE_FRENCH__
"eCONOMISEUR DeNERGIE DESACTIVe",
#endif
#ifdef __LANGUAGE_GERMAN__
"ENERGIESPARMODUS AUS",
#endif
#ifdef __LANGUAGE_DUTCH__
"ENERGIEBESPARENDE MODus UIT",
#endif
#ifdef __LANGUAGE_SPANISH__
"AHORRO DE ENERGiA DESACTIVADO",
#endif
    },/*STR_ENERGYSAVEOFF_ID*/
    
    {
        "ENG8",
#ifdef __LANGUAGE_FRENCH__
"Essai",
#endif
#ifdef __LANGUAGE_GERMAN__
"TEST",
#endif
#ifdef __LANGUAGE_DUTCH__
"TEST",
#endif
#ifdef __LANGUAGE_SPANISH__
"TEST",
#endif
    },/*STR_TEST_ID*/// 50
    
    {
        "QUICK",
#ifdef __LANGUAGE_FRENCH__
"RAPIDE",
#endif
#ifdef __LANGUAGE_GERMAN__
"SCHNELL",
#endif
#ifdef __LANGUAGE_DUTCH__
"SNEL",
#endif
#ifdef __LANGUAGE_SPANISH__
"RAPIDO",
#endif
    },/*STR_QUICK_ID*/
    
    {
        "NORMAL",
#ifdef __LANGUAGE_FRENCH__
"NORMAL",
#endif
#ifdef __LANGUAGE_GERMAN__
"NORMAL",
#endif
#ifdef __LANGUAGE_DUTCH__
"NORMAAL",
#endif
#ifdef __LANGUAGE_SPANISH__
"NORMAL",
#endif
    },/*STR_NORMAL_ID*/
    
    {
        "NO CLEAR",
#ifdef __LANGUAGE_FRENCH__
"NE PAS EFFACER",
#endif
#ifdef __LANGUAGE_GERMAN__
"NICHT LOESCHEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"NIET WISSEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"SIN BORRAR",
#endif
    },/*STR_NOCLEAR_ID*/
    
    {
        "CLEAR",
#ifdef __LANGUAGE_FRENCH__
"EFFACER",
#endif
#ifdef __LANGUAGE_GERMAN__
"Klar",
#endif
#ifdef __LANGUAGE_DUTCH__
"Klaar",
#endif
#ifdef __LANGUAGE_SPANISH__
"Borrar",
#endif
    },/*STR_CLEAR_ID*/
    
    {
        "PRESS START ENTER ERP",
#ifdef __LANGUAGE_FRENCH__
"APPUYER SUR START POUR PASSER EN MODE STAND BY",
#endif
#ifdef __LANGUAGE_GERMAN__
"START TASTE DRUECKEN UM IN DEN STANDBY MODUS ZU GELANGEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"DRUK OP DE TOETS OM NAAR DE STANDBY MODE TE GAAN",
#endif
#ifdef __LANGUAGE_SPANISH__
"PULSAR EL BOTON INICIAR PARA ENTRAR EN EL MODO DE ESPERA",
#endif
    },/*STR_PRESSSTARTBUTTON_ID*/
    
    {
        "RPM OFF",
#ifdef __LANGUAGE_FRENCH__
"TPM DeSACTIVe",
#endif
#ifdef __LANGUAGE_GERMAN__
"UPM AUS",
#endif
#ifdef __LANGUAGE_DUTCH__
"TPM UIT",
#endif
#ifdef __LANGUAGE_SPANISH__
"RPM DESACTIVADO",
#endif
    },/*STR_RPMOFF_ID*/
    
    {
        "RPM ON",
#ifdef __LANGUAGE_FRENCH__
"TPM ACTIVe",
#endif
#ifdef __LANGUAGE_GERMAN__
"UPM EIN",
#endif
#ifdef __LANGUAGE_DUTCH__
"TPM AAN",
#endif
#ifdef __LANGUAGE_SPANISH__
"RPM ACTIVADO",
#endif
    },/*STR_RPMON_ID*/
    
    {
        "WORKOUT PAUSED",
#ifdef __LANGUAGE_FRENCH__
"PAUSE TRAINING",
#endif
#ifdef __LANGUAGE_GERMAN__
"TRAININGS PAUSE",
#endif
#ifdef __LANGUAGE_DUTCH__
"TRAININGS PAUZE",
#endif
#ifdef __LANGUAGE_SPANISH__
"SESIoN EN PAUSA",
#endif
    },/*STR_WORKOUTPAUSE_ID*/
    
    {
        "ERROR NO HEART RATE",
#ifdef __LANGUAGE_FRENCH__
"ERREUR PAS DE FREQUENCE CARDIAQUE",
#endif
#ifdef __LANGUAGE_GERMAN__
"FEHLER KEINE HERZFREQUENZ",
#endif
#ifdef __LANGUAGE_DUTCH__
"FOUT GEEN HARTSLAG",
#endif
#ifdef __LANGUAGE_SPANISH__
"ERROR NO HAY RITMO CARDIACO",
#endif
    },/*STR_ERRORNOHEARTRATE_ID*/
    
    {
        "HEART RATE CONTROL",
#ifdef __LANGUAGE_FRENCH__
"CONTROLE FREQUENCE CARDIAQUE",
#endif
#ifdef __LANGUAGE_GERMAN__
"HERZFREQUENZKONTROLLE",
#endif
#ifdef __LANGUAGE_DUTCH__
"HARTSLAGCONTROLE",
#endif
#ifdef __LANGUAGE_SPANISH__
"CONTROL DE RITMO CARDIACO",
#endif
    },/*STR_HEARTRATECONTORL_ID*/// 60
        
    {
        "WORKOUT COMPLETE",
#ifdef __LANGUAGE_FRENCH__
"ENTRAINEMENT TERMINE",
#endif
#ifdef __LANGUAGE_GERMAN__
"TRAINING BEENDET",
#endif
#ifdef __LANGUAGE_DUTCH__
"TRAINING BEEINDIGD",
#endif
#ifdef __LANGUAGE_SPANISH__
"SESION COMPLETA",
#endif
    },/*STR_WORKOUTFINISH_ID*/
    
    {
        "INTERVALS",
#ifdef __LANGUAGE_FRENCH__
"INTERVALLES",
#endif
#ifdef __LANGUAGE_GERMAN__
"INTERVALL",
#endif
#ifdef __LANGUAGE_DUTCH__
"Intervallen",
#endif
#ifdef __LANGUAGE_SPANISH__
"Intervalos",
#endif
    },/*STR_INTERVALS_ID*/
    
    
    
    {
        "GAME",
#ifdef __LANGUAGE_FRENCH__
"JEU",
#endif
#ifdef __LANGUAGE_GERMAN__
"SPIEL",
#endif
#ifdef __LANGUAGE_DUTCH__
"SPEL",
#endif
#ifdef __LANGUAGE_SPANISH__
"JUEGO",
#endif
    },/*STR_GAME_ID*/
    
    {
        "FAT BURN",//"WEIGHT LOSS",
#ifdef __LANGUAGE_FRENCH__
"PERTE DE POIDS",
#endif
#ifdef __LANGUAGE_GERMAN__
"GEWICHTSREDUZIERUNG",
#endif
#ifdef __LANGUAGE_DUTCH__
"GEWICHTSVERMINDERING",
#endif
#ifdef __LANGUAGE_SPANISH__
"PERDIDA DE PESO",
#endif
    },/*STR_FATBURN_ID*/
    
    {
        
        "WEIGHT LOSS PLUS",
#ifdef __LANGUAGE_FRENCH__
"PERTE DE POIDS PLUS",
#endif
#ifdef __LANGUAGE_GERMAN__
"GEWICHTSREDUZIERUNG PLUS",
#endif
#ifdef __LANGUAGE_DUTCH__
"GEWICHTSVERMINDERING PLUS",
#endif
#ifdef __LANGUAGE_SPANISH__
"PERDIDA DE PESO ADICIONAL",
#endif
    },/*STR_WEIGHTLOSSPLUS_ID*/
    
    {
        "ROLLING",  
#ifdef __LANGUAGE_FRENCH__
"ENDURANCE",
#endif
#ifdef __LANGUAGE_GERMAN__
"HUEGEL",
#endif
#ifdef __LANGUAGE_DUTCH__
"HEUVEL",
#endif
#ifdef __LANGUAGE_SPANISH__
"Escalada",
#endif
    },/*STR_ROLLING_ID*/
    
    {
        "Reverse Train",
#ifdef __LANGUAGE_FRENCH__
"Mouvement inverse",
#endif
#ifdef __LANGUAGE_GERMAN__
"Training umkehren",
#endif
#ifdef __LANGUAGE_DUTCH__
"Omgekeerd trainen",
#endif
#ifdef __LANGUAGE_SPANISH__
"Entrenamiento inverso",
#endif
    },/*STR_CADENCE_ID*/
    
    {
        "RANDOM",
#ifdef __LANGUAGE_FRENCH__
"Aleatoire",
#endif
#ifdef __LANGUAGE_GERMAN__
"ZUFALL",
#endif
#ifdef __LANGUAGE_DUTCH__
"Willekeurig",
#endif
#ifdef __LANGUAGE_SPANISH__
"Aleatorio",
#endif
    },/*STR_RANDOM_ID*/
    
    {
        "WIN",
#ifdef __LANGUAGE_FRENCH__
"GAGNER",
#endif
#ifdef __LANGUAGE_GERMAN__
"GEWINNEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"WINNEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"GANAR",
#endif
    },/*STR_WIN_ID*/// 70
    
    {
        "FAIL",
#ifdef __LANGUAGE_FRENCH__
"Echec",
#endif
#ifdef __LANGUAGE_GERMAN__
"fehlgeschlagen",
#endif
#ifdef __LANGUAGE_DUTCH__
"MISLUKT",
#endif
#ifdef __LANGUAGE_SPANISH__
"ERROR",
#endif
    },/*STR_FAIL_ID*/
    
    {
        "FITNESS TEST",
#ifdef __LANGUAGE_FRENCH__
"TEST FITNESS",
#endif
#ifdef __LANGUAGE_GERMAN__
"FITNESS TEST",
#endif
#ifdef __LANGUAGE_DUTCH__
"Fitheids test",
#endif
#ifdef __LANGUAGE_SPANISH__
"Test En Forma",
#endif
    },/*STR_FITNESS_ID*/
    
    {
        "Strength Builder",
#ifdef __LANGUAGE_FRENCH__
"AFFERMISSEMENT",
#endif
#ifdef __LANGUAGE_GERMAN__
"KRAEFTIGUNG",
#endif
#ifdef __LANGUAGE_DUTCH__
"VERSTERKING",
#endif
#ifdef __LANGUAGE_SPANISH__
"FORTALECIMIENTO",
#endif
    },/*STR_STRENGTHBUILD_ID*/
    
    {
        "Please keep rpm between 60 and 80",
#ifdef __LANGUAGE_FRENCH__
"Maintenez votre cadence entre 60 et 80 TPM",
#endif
#ifdef __LANGUAGE_GERMAN__
"Bitte zwischen 60 und 80 UPM bleiben.",
#endif
#ifdef __LANGUAGE_DUTCH__
"Houd de tpm tussen 60 en 80",
#endif
#ifdef __LANGUAGE_SPANISH__
"Mantener entre 60 y 80 rpm",
#endif
    },/*STR_KEEPSPEED_ID*/
    
    {
        "very poor",
#ifdef __LANGUAGE_FRENCH__
"TRES faible",
#endif
#ifdef __LANGUAGE_GERMAN__
"sehr schlecht",
#endif
#ifdef __LANGUAGE_DUTCH__
"ZEER SLECHT",
#endif
#ifdef __LANGUAGE_SPANISH__
"MUY POBRE",
#endif
    },/*STR_VERYPOOR_ID*/
    
    {
        "poor",
#ifdef __LANGUAGE_FRENCH__
"FAIBLE",
#endif
#ifdef __LANGUAGE_GERMAN__
"schlecht",
#endif
#ifdef __LANGUAGE_DUTCH__
"SLECHT",
#endif
#ifdef __LANGUAGE_SPANISH__
"BAJO",
#endif
    },/*STR_POOR_ID*/
    
    {
        "Average",
#ifdef __LANGUAGE_FRENCH__
"Moyenne",
#endif
#ifdef __LANGUAGE_GERMAN__
"Durchschnitte",
#endif
#ifdef __LANGUAGE_DUTCH__
"Gemiddelden",
#endif
#ifdef __LANGUAGE_SPANISH__
"Medias",
#endif
    },/*STR_AVERAGE_ID*/
    
    {
        "GOOD",
#ifdef __LANGUAGE_FRENCH__
"BONNE",
#endif
#ifdef __LANGUAGE_GERMAN__
"GUT",
#endif
#ifdef __LANGUAGE_DUTCH__
"GOED",
#endif
#ifdef __LANGUAGE_SPANISH__
"BIEN",
#endif
    },/*STR_GOOD_ID*/
    
    {
        "EXCELLENT",
#ifdef __LANGUAGE_FRENCH__
"EXCELLENTE",
#endif
#ifdef __LANGUAGE_GERMAN__
"EXZELLENT",
#endif
#ifdef __LANGUAGE_DUTCH__
"UITSTEKEND",
#endif
#ifdef __LANGUAGE_SPANISH__
"EXCELENTE",
#endif
    },/*STR_EXCELLENT_ID*/
    

    
    {
        "begin Workout",
#ifdef __LANGUAGE_FRENCH__
"Commencer EXERCICE",
#endif
#ifdef __LANGUAGE_GERMAN__
"TRAINING Anfang",
#endif
#ifdef __LANGUAGE_DUTCH__
"TRAINING Begin",
#endif
#ifdef __LANGUAGE_SPANISH__
"Comenzar ENTRENAMIENTO",
#endif
    },/*STR_BEGINWKOT_ID*/
    
    {
        "RESUME Workout",
#ifdef __LANGUAGE_FRENCH__
"REPRENDRE lentrainement",
#endif
#ifdef __LANGUAGE_GERMAN__
"TRAINING WEITERFuHREN",
#endif
#ifdef __LANGUAGE_DUTCH__
"TRAINING HERVATTEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"REANUDAR sesion",
#endif
    },/*STR_RESUMEWKOT_ID*/
    
    {
        "Time out",
#ifdef __LANGUAGE_FRENCH__
"Depassement temps",
#endif
#ifdef __LANGUAGE_GERMAN__
"Unterbrechung",
#endif
#ifdef __LANGUAGE_DUTCH__
"Onderbreking",
#endif
#ifdef __LANGUAGE_SPANISH__
"Tiempo de espera agotado",
#endif
    },/*STR_TIMEOUT_ID*/
    
    {
        "Volume",
#ifdef __LANGUAGE_FRENCH__
"Volume",
#endif
#ifdef __LANGUAGE_GERMAN__
"Volumen",
#endif
#ifdef __LANGUAGE_DUTCH__
"Volume",
#endif
#ifdef __LANGUAGE_SPANISH__
"Volumen",
#endif
    },/*STR_VOL_ID*/
    
    {
        "LEVEL",
#ifdef __LANGUAGE_FRENCH__
"Niveau",
#endif
#ifdef __LANGUAGE_GERMAN__
"level",
#endif
#ifdef __LANGUAGE_DUTCH__
"Niveau",
#endif
#ifdef __LANGUAGE_SPANISH__
"Nivel",
#endif
    },/*STR_LEVEl_ID*/
    
    
    //#ifdef __WIFI_FUNC__
    
    {
        "XID",
#ifdef __LANGUAGE_FRENCH__
"XID"
#endif
#ifdef __LANGUAGE_GERMAN__
"XID"
#endif
#ifdef __LANGUAGE_DUTCH__
"XID"
#endif
#ifdef __LANGUAGE_SPANISH__
"XID"
#endif
    },/*STR_XID_ID*/
    
    {
        "ENTER PASSCODE",
#ifdef __LANGUAGE_FRENCH__
"SAISIR LE MOT DE PASSE",
#endif
#ifdef __LANGUAGE_GERMAN__
"PASSCODE EINGEBEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"VOER TOEGANGSCODE IN",
#endif
#ifdef __LANGUAGE_SPANISH__
"INTRODUCIR CoDIGO DE ACCESO",
#endif
    },/*STR_PASSCOD_ID*/
    
    {
        "ENTER XID",
#ifdef __LANGUAGE_FRENCH__
"SAISIR UN XID",
#endif
#ifdef __LANGUAGE_GERMAN__
"XID EINGEBEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"VOER EEN XID IN",
#endif
#ifdef __LANGUAGE_SPANISH__
"INTRODUCIR XID",
#endif
    },/*STR_ENTER_XID_ID*/
    
    {
        "DELETE A USER",
#ifdef __LANGUAGE_FRENCH__
"Supprimer",
#endif
#ifdef __LANGUAGE_GERMAN__
"loschen",
#endif
#ifdef __LANGUAGE_DUTCH__
"Wissen",
#endif
#ifdef __LANGUAGE_SPANISH__
"Eliminar",
#endif
    },/*STR_DELETE_ID*/
            {
        "EDIT A USER",
#ifdef __LANGUAGE_FRENCH__
"MODIFIER",
#endif
#ifdef __LANGUAGE_GERMAN__
"BEARBEITEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"Bewerken",
#endif
#ifdef __LANGUAGE_SPANISH__
"EDITAR",
#endif
    },/*STR_EDIT_ID*/// 90

    
    {
        "SELECT A performance program",
#ifdef __LANGUAGE_FRENCH__
"SeLECTIONNER UN programme dentrainement",
#endif
#ifdef __LANGUAGE_GERMAN__
"Leistungsprogramm AUSWaHLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"Een prestatieprogramma KIEZEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"SELECCIONAR un programa de rendimiento",
#endif
    },/*STR_SETPROFORMANCE_ID*/
    
    {
        "SELECT AN INTERVAL program",
#ifdef __LANGUAGE_FRENCH__
"SELECTIONNEZ un programme dintervalles",
#endif
#ifdef __LANGUAGE_GERMAN__
"Intervallprogramm AUWAHLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"Een intervalprogramma KIEZEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"SELECCIONAR UN programa de INTERVALO",
#endif
    },/*STR_SETINTERVAL_ID*/
    
    {
        "SELECT A CUSTOM program",
#ifdef __LANGUAGE_FRENCH__
"SeLECTIONNER UN PROGRAMME personnalise",
#endif
#ifdef __LANGUAGE_GERMAN__
"BENUTZERDEFINIERTES Programm AUWaHLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"Een programma OP MAAT KIEZEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"SELECCIONAR un programa PERSONALIZADO",
#endif
    },/*STR_SETCUSTOM_ID*/
    
    {
        "SELECT program",
#ifdef __LANGUAGE_FRENCH__
"SELECTIONNEZ programme",
#endif
#ifdef __LANGUAGE_GERMAN__
"WaHLEN Sie ein Programm",
#endif
#ifdef __LANGUAGE_DUTCH__
"KIES een programma",
#endif
#ifdef __LANGUAGE_SPANISH__
"Comenzar ENTRENAMIENTO",
#endif
    },/*STR_SETPROGRAM_ID*/
    
    {
        "Wifi Test",
#ifdef __LANGUAGE_FRENCH__
"Essai WiFi",
#endif
#ifdef __LANGUAGE_GERMAN__
"Wifi Test",
#endif
#ifdef __LANGUAGE_DUTCH__
"Wifi test",
#endif
#ifdef __LANGUAGE_SPANISH__
"Test de WI-FI",
#endif
    },/*STR_WIFITEST_ID*/
    
    {
        "SELECT TARGET CALORIES",
#ifdef __LANGUAGE_FRENCH__
"SAISIR LE NOMBRE DE CALORIES",
#endif
#ifdef __LANGUAGE_GERMAN__
"KALORIEN EINGEBEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"DOELCALORIEEN KIEZEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"ESTABLECER CALORiAS",
#endif
    },/*STR_SETCAL_ID*/
    
    {
        "SELECT TARGET DISTANCE",
#ifdef __LANGUAGE_FRENCH__
"SAISIR LA DISTANCE CIBLE",
#endif
#ifdef __LANGUAGE_GERMAN__
"ZIELDISTANZ AUSWuHLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"DOELAFSTAND KIEZEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"SELECCIONAR DISTANCIA OBJETIVO",
#endif
    },/*STR_SETDISTANCE_ID*/
    
    {
        "SELECT TARGET STEPS",
#ifdef __LANGUAGE_FRENCH__
"SAISIR LE NOMBRE DE PAS CIBLE",
#endif
#ifdef __LANGUAGE_GERMAN__
"ZIELSCHRITTE AUSWaHLEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"AANTAL DOELSTAPPEN KIEZEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"SELECCIONAR OBJETIVO DE PASOS",
#endif
    },/*STR_SETSTEP_ID*/
    
    {
        "MILES",
#ifdef __LANGUAGE_FRENCH__
"MILES",
#endif
#ifdef __LANGUAGE_GERMAN__
"MEILEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"MIJL",
#endif
#ifdef __LANGUAGE_SPANISH__
"MILLAS",
#endif
    },/*STR_MILES_ID*/
    
    {
        "half marathon",
#ifdef __LANGUAGE_FRENCH__
"demi Marathon",
#endif
#ifdef __LANGUAGE_GERMAN__
"Halbmarathon",
#endif
#ifdef __LANGUAGE_DUTCH__
"Halve marathon",
#endif
#ifdef __LANGUAGE_SPANISH__
"media maraton",
#endif
    },/*STR_HALFMALSON_ID*/// 100
    
    {
        "Activated",
#ifdef __LANGUAGE_FRENCH__
"Active",
#endif
#ifdef __LANGUAGE_GERMAN__
"Aktiviert",
#endif
#ifdef __LANGUAGE_DUTCH__
"Geactiveerd",
#endif
#ifdef __LANGUAGE_SPANISH__
"Activado",
#endif
    },/*STR_ACTIVATED_ID*/
    
    {
        "EDIT A USER",
#ifdef __LANGUAGE_FRENCH__
"MODIFIER UN UTILISATEUR",
#endif
#ifdef __LANGUAGE_GERMAN__
"BENUTZER BEARBEITEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"EEN GEBRUIKER BEWERKEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"EDITAR UN USUARIO",
#endif
    },/*STR_EDITAUSER_ID*/
    
    {
        "DELETE A USER",
#ifdef __LANGUAGE_FRENCH__
"Supprimer UN lutilisateur",
#endif
#ifdef __LANGUAGE_GERMAN__
"Benutzer loschen",
#endif
#ifdef __LANGUAGE_DUTCH__
"EEN Gebruiker verwijderen",
#endif
#ifdef __LANGUAGE_SPANISH__
"Eliminar UN usuario",
#endif
    },/*STR_DELETEUSER_ID*/
    
    {
        "USER LOGIN",
#ifdef __LANGUAGE_FRENCH__
"CONNEXION UTILISATEUR",
#endif
#ifdef __LANGUAGE_GERMAN__
"NUTZERANMELDUNG",
#endif
#ifdef __LANGUAGE_DUTCH__
"AANMELDEN GEBRUIKER",
#endif
#ifdef __LANGUAGE_SPANISH__
"INICIO DE SESIoN DE USUARIO",
#endif
    },/*STR_LOGIN_ID*/
    
    {
        "Please Wait",
#ifdef __LANGUAGE_FRENCH__
"Veuillez patienter",
#endif
#ifdef __LANGUAGE_GERMAN__
"Bitte warten",
#endif
#ifdef __LANGUAGE_DUTCH__
"Moment a.u.b.",
#endif
#ifdef __LANGUAGE_SPANISH__
"espere",
#endif
    },/*STR_WAIT_ID*/
    
    {
        "Welcome",
#ifdef __LANGUAGE_FRENCH__
"Bienvenue",
#endif
#ifdef __LANGUAGE_GERMAN__
"Willkommen",
#endif
#ifdef __LANGUAGE_DUTCH__
"Welkom",
#endif
#ifdef __LANGUAGE_SPANISH__
"Bienvenido",
#endif
    },/*STR_WELCOME_ID*/
    
    {
        "INVALID USER/PIN",
#ifdef __LANGUAGE_FRENCH__
"UTILISATEUR/NIP INVALIDE",
#endif
#ifdef __LANGUAGE_GERMAN__
"BENUTZERNAME/PIN UNGuLTIG",
#endif
#ifdef __LANGUAGE_DUTCH__
"ONGELDIGE GEBRUIKER/PIN",
#endif
#ifdef __LANGUAGE_SPANISH__
"USUARIO/PIN NO VALIDO",
#endif
    },/*STR_INVALIDPW_ID*/
    
    {
        "Success",
#ifdef __LANGUAGE_FRENCH__
"Succes",
#endif
#ifdef __LANGUAGE_GERMAN__
"Erfolg",
#endif
#ifdef __LANGUAGE_DUTCH__
"Geslaagd",
#endif
#ifdef __LANGUAGE_SPANISH__
"Correcto",
#endif
    },/*STR_SUCCESS_ID*/
    
    {
        "WIFI IS OFFLINE",
#ifdef __LANGUAGE_FRENCH__
"WIFI DeCONNECTe",
#endif
#ifdef __LANGUAGE_GERMAN__
"WIFI IST offline",
#endif
#ifdef __LANGUAGE_DUTCH__
"WIFI IS OFFLINE",
#endif
#ifdef __LANGUAGE_SPANISH__
"WI-FI ESTA FUERA DE LINEA",
#endif
    },/*STR_OFFLINE_ID*/
    
    {
        "SPEED INTERVALS"   ,
#ifdef __LANGUAGE_FRENCH__
"INTERVALLE DE VITESSE",
#endif
#ifdef __LANGUAGE_GERMAN__
"GESCHWINDIGKEITSINTERVALL",
#endif
#ifdef __LANGUAGE_DUTCH__
"SNELHEIDSINTERVAL",
#endif
#ifdef __LANGUAGE_SPANISH__
"INTERVALOS DE VELOCIDAD",
#endif
    },/*STR_SPINTERVAL_ID*/// 110
    
    {
        "ENDURANCE INTERVALS"   ,
#ifdef __LANGUAGE_FRENCH__
"INTERVALLE DENDURANCE",
#endif
#ifdef __LANGUAGE_GERMAN__
"AUSDAUERINTERVALL",
#endif
#ifdef __LANGUAGE_DUTCH__
"UITHOUDINGSVERMOGEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"Intervalos de RESISTENCIA",
#endif
    },/*STR_ENDURANCE_ID*/
    
    {
        "CALorieS" ,
#ifdef __LANGUAGE_FRENCH__
"CALORIES",
#endif
#ifdef __LANGUAGE_GERMAN__
"KALORIEN",
#endif
#ifdef __LANGUAGE_DUTCH__
"CALORIEEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"CALORiAS",
#endif
    },/*STR_CALORIES_ID*/
    
    {
        "STEPS"   ,
#ifdef __LANGUAGE_FRENCH__
"PAS",
#endif
#ifdef __LANGUAGE_GERMAN__
"SCHRITTE",
#endif
#ifdef __LANGUAGE_DUTCH__
"STAPPEN",
#endif
#ifdef __LANGUAGE_SPANISH__
"PASOS",
#endif
    },/*STR_STEPS_ID*/
    
    {
        "Distance"   ,
#ifdef __LANGUAGE_FRENCH__
"Distance",
#endif
#ifdef __LANGUAGE_GERMAN__
"Entfernung",
#endif
#ifdef __LANGUAGE_DUTCH__
"Afstand",
#endif
#ifdef __LANGUAGE_SPANISH__
"Distancia",
#endif
    },/*STR_DISTANCE_ID*/
    {
        "WEIGHT LOSS",
    },/*STR_WEIGHT_LOSS_ID*/
    {
        "PERFORMANCE" ,
#ifdef __LANGUAGE_FRENCH__
"PERFORMANCE",
#endif
#ifdef __LANGUAGE_GERMAN__
"LEISTUNG",
#endif
#ifdef __LANGUAGE_DUTCH__
"PRESTATIE",
#endif
#ifdef __LANGUAGE_SPANISH__
"RENDIMIENTO",
#endif
    },/*STR_PROFORMANCE_ID*/
    
    {
        "CUSTOM"   ,
#ifdef __LANGUAGE_FRENCH__
"PERSONNALISe",
#endif
#ifdef __LANGUAGE_GERMAN__
"BENUTZERDEFINIERT",
#endif
#ifdef __LANGUAGE_DUTCH__
"AANGEPAST",
#endif
#ifdef __LANGUAGE_SPANISH__
"PERSONALIZADO",
#endif
    },/*STR_CUSTOM_ID*/
    
    {
        "WIFI IS OK",
#ifdef __LANGUAGE_FRENCH__
"Wifi CONNECTe",
#endif
#ifdef __LANGUAGE_GERMAN__
"WIFI ist OK",
#endif
#ifdef __LANGUAGE_DUTCH__
"WIFI is OK",
#endif
#ifdef __LANGUAGE_SPANISH__
"WI-FI CORRECTA",
#endif
    },/*STR_WIFIOK_ID*/
    
    {
        "No IP Address" ,
#ifdef __LANGUAGE_FRENCH__
"Pas dadresse IP",
#endif
#ifdef __LANGUAGE_GERMAN__
"Keine IP Adresse",
#endif
#ifdef __LANGUAGE_DUTCH__
"Geen IP adres",
#endif
#ifdef __LANGUAGE_SPANISH__
"No hay direccion IP",
#endif
    },/*STR_NOIP_ID*/
    
    {
        "No MAC Address" ,
#ifdef __LANGUAGE_FRENCH__
"Pas dadresse MAC",
#endif
#ifdef __LANGUAGE_GERMAN__
"Keine MAC Adresse",
#endif
#ifdef __LANGUAGE_DUTCH__
"Geen MAC adres",
#endif
#ifdef __LANGUAGE_SPANISH__
"No hay direccion MAC",
#endif
    },/*STR_NOMAC_ID*/
    
    //#endif
    
    {
        "calories/HR",
        
    },/*STR_CALORIES_TITLE_ID*/// 120
    
    {
        "mile",
        
    },/*STR_MILE_TITLE_ID*/
    
    {
        "km",
        
    },/*STR_km_TITLE_ID*/
    {
        "incline",
        
    },/*STR_INCLINE_TITLE_ID*/
    
    {
        "mph",
        
    },/*STR_MPH_TITLE_ID*/
    
    {
        "SET RPM",
        
    },/*STR_SPEED_TITLE_ID*/
    
    {
        "pace",
        
    },/*STR_PACE_TITLE_ID*/
    
    {
        "Heart Rate",
        
    },/*STR_HR_TITLE_ID*/
    
    {
        "mets",
        
    },/*STR_METS_TITLE_ID*/
    
    {
        "res",
       // "resistance",
        
    },/*STR_RESISTANCE_TITLE_ID*/
    
    {
        "watts",
        
    },/*STR_WATTS_TITLE_ID*/
    
    {
        "rpm",
        
    },/*STR_RPM_TITLE_ID*/// 130
    
    {
        "press start",
        
    },/*STR_PRESSSTART_TITLE_ID*/
    
    {
        "pause",
        
    },/*STR_PRESSPAUSE_TITLE_ID*/
    
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
    
    
    {
        "safety key off",
        
    },/*STR_SAFEKEY_OFF_ID*/
    
    {
        "set target heartrate",
        
    },/*STR_SETHEARTRATE_ID*/
    
    {
        "hill Climb",
        
    },/*STR_HILLCLIMB_ID*/
    
    {
        "constant watts",
        
    },/*STR_CONWATTS_ID*/
    
//    {
//        "sprint8",
//    },/*STR_SPRINT8_ID*/
    
    /*{
        "Models1",
    },//STR_Models1_ID,
    {
        "Models2",
    },
    {
        "Models3",
    },// 150
    {
        "Models4",
    },
    {
        "Models5",
    },
    {
        "Models6",
    },
    {
        "Models7",
    },
    {
        "Models8",
    },
    {
        "Models9",
    },
    {
        "Models10",
    },
    {
        "Models11",
    },
    {
        "Models12",
    },*/
    {
        "RF SYNC",// 160
    },//STR_RF_SYNC_ID
    {
        "RF SYNC FINISH",
    },
    {
        "RF SYNC TIMEOUT",
    },
    {
        "Eng0", //zsf
    },
    {
        "Eng1",//zsf
    },
    {
        "Eng2",//zsf
    },
    {
        "Eng3",//zsf
    },
    {
        "Eng4",//zsf
    },
    {
        "Eng5",//zsf
    },
    {
        "Eng8",//zsf
    },
    {
        "set dapi",
    },//STR_DAPI_ID,
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
        "USB TEST",
    },
    {
        "USB TEST TESTING",
    },// 170
    {
        "USB TEST PASS",
    },
    {
        "USB TEST FAIL",
    },
    {
        "P1",
    },
    {
        "P2",
    },
    {
        "P3",
    },
    {
        "P4",
    },
    {
        "P5",
    },
    {
        "P6",
    },
    {
        "TREADMILL",
    },
    {
        "ELLIPTICAL",
    },// 180
    {
        "BIKE",
    },
    {
        "ASCENT",
    },
    
    {
        "tf30",
        
    },/*STR_Models1_ID*/
    {
        "tf50",
        
    },/*STR_Models2_ID*/
    {
        "t50",
        
    },/*STR_Models3_ID*/
    
    {
        "r30",
        
    },/*STR_Models5_ID*/
    {
        "u30",
        
    },/*STR_Models7_ID*/
    {
        "e30",
        
    },/*STR_Models11_ID*/
    
    {
        "r50",
        
    },/*STR_Models6_ID*/
   
    {
        "u50",
        
    },/*STR_Models8_ID*/// 190
    {
        "e50",
        
    },/*STR_Models12_ID*/

    {
        "a30",
        
    },/*STR_Models9_ID*/
    {
        "a50",
        
    },/*STR_Models10_ID*/
    {
        "t70",
        
    },/*STR_Models4_ID*/
    {
    "rebooting",
    },/*STR_RETOOTING_ID*/
    
    {
    "BOOT ON",
    },
    {
    "BOOT OFF",
    },
    {
    "TIME",
    },
    {
    "ACC INFO",
    },
    {
    "UCB ID",
    },
    {
    "MCB ID",
    },// 200
    {
    "BURN",
    },
    {
    "SET ERP TIME",
    },//STR_SET_ERP_TIME_ID
    {
    "SET RPM",
    },//STR_SET_RPM_ID
    {
        "WIFI SUCCESS",
        
    },/*STR_WIFI_SUCCESS_ID*/
    {
        "WIFI FAIL",
        
    },/*STR_WIFI_FAIL_ID*/
    {
        "WIFI WAITTING",
        
    },/*STR_WIFI_WAITTING_ID*/
    {
        "WIFI TIMEOUT",
        
    },/*STR_WIFI_TIMEOUT_ID*/
    {
        "WIFI ACTIVATED",
        
    },/*STR_WIFI_ACTIVATED_ID*/
    
    {
        "SETUP ENG",
        
    },/*STR_SETUPENG_ID*/// 
    {
        "KGS",
        
    },/*STR_SETUPENG_ID*/// 
    {
        "LB",
        
    },/*STR_SETUPENG_ID*/// 210
    {
        "KPH",
    },/*STR_SETUPENG_ID*/// 
    {
        "RPM",
    },/*STR_SETUPENG_ID*///
    {
        "SET ADC",
    },/*STR_SETUPENG_ID*///
    {
        "ECB",
    },/*STR_SETUPENG_ID*///
    {
        "PWM",
    },/*STR_SETUPENG_ID*///
    {
        "eng0 display test",
    }, //STR_TITLE_DISPLAY_ID,
    {
        "all dots on",
    },//STR_TITLE_ALLDOTSON_ID,
    {
        "all dots off",
    },//STR_TITLE_ALLDOTSOFF_ID,
    {
        "seg1 test",
    },//STR_TITLE_SEG1_ID,
    {
        "seg2 test",
    },//STR_TITLE_SEG2_ID,
    {
        "seg3 test",
    },//STR_TITLE_SEG3_ID,
    {
        "eng1 hardware test",
    },//STR_TITLE_HARDWARETEST_ID,
    {
        "basic function",
    },//STR_TITLE_BASICFUNCTION_ID,
    {
        "usb port test",
    },//STR_TITLE_USBPORT_ID,
    {
        "wifi test",
    },//STR_TITLE_WIFI_ID,
    {
        "eng2 not available",
    },//STR_TITLE_NOTAVAILABLE_ID,
    {
        "eng3 switch function",
    },//STR_TITLE_SWITCHFUNCTION_ID,
    {
        "standard or metric",
    },//STR_TITLE_STANDARDORMETRIC_ID,
    {
        "language selection",
    },//STR_TITLE_LANGUAGESELECTION_ID,
    {
        "machine selection",
    },//STR_TITLE_MACHINESELECTION_ID,
    {
        "model selection",
    },//    STR_TITLE_MODELSELECTION_ID,
    {
        "energy mode",
    },//STR_TITLE_ENERGYMODE_ID,
    {
        "first boot",
    },//STR_TITLE_FIRSTBOOT_ID,
    {
        "eng4 information",
    },//STR_TITLE_INFORMATION_ID,
    {
        "accumulated inf0",
    },//STR_TITLE_ACCUMULATEDINF_ID,
    {
        "ucb version",
    },//STR_TITLE_UCBVERSION_ID,
    {
        "mcb version",
    },//STR_TITLE_MCBVERSION_ID,
    {
        "eng8 test",
    },//STR_TITLE_TEST_ID,
    {
        "program acceleerator",
    },//STR_TITLE_PROGRAMACCELERATOR_ID,
    {
        "clear user data",
    },//STR_TITLE_CLEARUSERDATA_ID,
    {
        "burn check",
    },//STR_TITLE_BURNCHECK_ID,
    {
        "quick standby mode",
    },//STR_TITLE_QUICKSTANDBYMODE_ID,
    {
        "reserved",
    },//STR_TITLE_QUICKSTANDBYMODE_ID,
    {
        "Machine",
    },//STR_SELECT_MODEL_ID,
    {
        "language",
    },//STR_SELECT_LANGUAGE_ID,
    {
        "unit",
    },//STR_SELECT_UNIT_ID,
    {
        "unsupported MCB found",
    },//STR_UNSUPPORTED_MCB_FOUND_ID,
    {
       "press enter to update from usb", 
    },
    //STR_USB_UPDATE_NOTIES_ID
    {
       "press enter to update bt",  //file from usb
    },
    //STR_USB_BTUPDATE_NOTIES_ID,
    {
       "updated fail", 
    },
    //STR_USB_BTUPDATEED_FAIL_ID,
        /*STRINGIDEND*/
    {
        "demo srtatus",
    },/*STR_DEMO_ID*/
    {
        " ",
    }, /*STR_NULL_ID*/
    {
        "warm up",
    }, /*STR_WARMUP_MPH_ID*/
	{
        "cool down",
    }, /*STR_COOLDOWN_ID*/
  	{
		"ready",
	},//STR_READY_ID
	{
		"PROGRAM STARTING",
	},//STR_RUN_ID
	{
		"SPRINT 1",
	},//STR_SPRINT1_ID,
	{
		"SPRINT 2",
	},//STR_SPRINT2_ID,
	{
		"SPRINT 3",
	},//STR_SPRINT3_ID,
	{
		"  SPRINT 4",
	},//STR_SPRINT4_ID,
	{
		"  SPRINT 5",
	},//STR_SPRINT5_ID,
	{
		"  SPRINT 6",
	},//STR_SPRINT6_ID,
	{
		"  SPRINT 7",
	},//STR_SPRINT7_ID,
	{
		"  SPRINT 8",
	},//STR_SPRINT8_ID,
	{
        "TIME"
    },
    {
        "DISTANCE"
    },
    {
        "SET SEGMENT"
    },
    {
        "CUSTOM HRT RATE"
    },
    {
        "TOTAL TIME"
    },
    {
        "MAX DISTANCE PRESS START"
    },
    {
        "MY FIRST 5K"
    },
    {
        "WALK SPEED"
    },
    {
        "JOG SPEED"
    },
    {
        "CALORIES"
    },
    {
        "1 MILE"
    },
    {
        "2 MILES"
    },
    {
        "5 K"
    },
    {
        "5 MILES"
    },
    {
        "10 K"
    },
    {
        "8 MILES"
    },
    {
        "15 K"
    },
    {
        "10 MILES"
    },
    {
        "20 K"
    },
    {
        "HALF MARATHON"
    },
    {
        "15 MILES"
    },
    {
        "20 MILES"
    },
    {
        "MARATHON"
    },
    {
        "HEART RATE"
    },
    {
        "TIME BASED OR DISTANCE BASED"
    },
    {
        "NEXT WORKOUT IN 2 DAYS"
    }, 
    {
        "NEXT WORKOUT IN 3 DAYS"
    },    
    {
        "CONGRATULATIONS GOOD LUCK WITH YOUR 5K"
    }, 
    {
        "1 WEEK MISSED REPEAT LAST WEEK"
    },
    {
        "2 WEEK MISSED START OVER"
    },

    {
		"HEART RATE MONITOR ONLY  "
	},/*   STR_HEART_RATE_MONITOR_ONLY_ID*/
	{
		"TABLET ENABLED"
	},/*   STR_HEART_RATE_MONITOR_ONLY_ID*/
    {
        "KEY TEST"
    },
};





char* String_Get_String( STRINGID StringID )
{
    if( Strings[StringID][by_languageID] )
    {
        return (char*)Strings[StringID][by_languageID];
    }
    else
    {
        return (char*)Strings[StringID][_ENGLISH];
    }
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



