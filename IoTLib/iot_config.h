
#ifndef __IOT_CONFIG_H
#define __IOT_CONFIG_H


#include "iot_comm.h"
#include "iot_functions.h"


/* Command configuration */
#define USER_CONTROL 				0x01
#define SETTING_CONTROL				0x02
#define PROGRAM_CONTROL				0x03


/*Application Code*/
#define APPLIANCES_WASHING_MACHINE		0x01
#define APPLIANCES_MICROWAVE_OVEN		0x02
#define APPLIANCES_AIR_CONDITIONERS		0x03
#define APPLIANCES_REFRIGERATOR			0x04




/*IOT Function setting*/
#define DETERGENT_ORDER			0x01
#define AP_MODE_SETTING			0x02
#define BLE_SETTING			    0x04
#define BROWNOUT_STATUS			0x08
#define FOTA_STATUS			    0x10
#define WIFI_ENABLE_DISABLE		0x20
#define MACHINE_ON_OFF			0x40
#define TUBCLEAN_REMINDER		0x80



/*Spin settings*/
#define NO_SPIN				0x00
#define SPIN_400			0x01
#define SPIN_600			0x02
#define SPIN_700			0x03	
#define SPIN_800			0x04
#define SPIN_900			0x05
#define SPIN_1000			0x06
#define SPIN_1200			0x07
#define SPIN_1400			0x08
#define SPIN_RINSE_HOLD			0x09


/*Delay*/
#define NO_DELAY			0
#define DELAY_MIN_30			1
#define DELAY_MIN_60			2
#define DELAY_MIN_90			3
#define DELAY_MIN_120			4
#define DELAY_MIN_150			5
#define DELAY_MIN_180			6
#define DELAY_MIN_210			7
#define DELAY_MIN_240			8
#define DELAY_MIN_270			9
#define DELAY_MIN_300			10
#define DELAY_MIN_360			11
#define DELAY_MIN_420			12
#define DELAY_MIN_480			13
#define DELAY_MIN_540			14
#define DELAY_MIN_600			15
#define DELAY_MIN_660			16
#define DELAY_MIN_720			17
#define DELAY_MIN_780			18
#define DELAY_MIN_840			19
#define DELAY_MIN_960			21
#define DELAY_MIN_1080			23
#define DELAY_MIN_1200			25
#define DELAY_MIN_1320			26
#define DELAY_MIN_1440			27
/* Extra Rinse Count Setting */
#define EXTRA_RINSE_0 			0x00
#define EXTRA_RINSE_1 			0x01
#define EXTRA_RINSE_2 			0x02
#define EXTRA_RINSE_3 			0x03

/* Temperature setting */
#define SET_TEMP_HOT					0x00
#define SET_TEMP_WARM					0x01
#define SET_TEMP_COLD					0x02
#define SET_TEMP_30 					0x03
#define SET_TEMP_40 					0x04
#define SET_TEMP_60 					0x05
#define SET_TEMP_95 					0x06
//#define SET_TEMP_20					0x07
#define SET_TEMP_40E					0x08
#define SET_TEMP_60E					0x09


/* User Control Option Setting */
#define CMD_START					0x01
#define CMD_STOP					0x02
#define CMD_PAUSE					0x03
#define CMD_PROG_CHANGE				0x04
#define CMD_SLEEP					0x05			//WakeUP
#define CMD_TEST					0x06
#define CMD_READ_STATUS				0x07
#define CMD_READ_DIAGNOSIS 			0x08
#define CMD_RESET_MCU				0x09
#define CMD_READ_DATA1				0x0B
#define CMD_READ_DATA2				0x0C
#define CMD_IOT_FUNCTION_FLAG		0x0D
#define CMD_DIAGNOSTIC				0x0E
#define CMD_REBOOT					0x0F
#define CMD_READ_VERSION			0x10
#define CMD_MACHINE_SWITCH_ON   	0x11
#define CMD_MACHINE_SWITCH_OFF   	0x12

#define CMD_FAVORITE			0x17
#define CMD_FAVORITE_PROG		0x18

/* HIL Control Option Setting */
#define READ_DATA	 		0x01
#define PROGRAM_SETTING			0x02
#define TEMPERATURE_SETTING 		0x03
#define RAPIDWASH_SETTING		0x04
#define SPEED_SETTING			0x05
#define PREWASH_SETTING 		0x06
#define EXTRARINSE_SETTING 		0x07
#define RINSEHOLD_SETTING		0x08
#define DELAY_SETTING			0x09
#define SOAK_SETTING			0x0A
#define CHILDLOCK_SETTING		0x0B
#define HOT_RINSE			0x0C
#define TIME_SAVER			0x0D
#define ECHO_SETTING			0x0E
#define SOILAGE_SETTING			0x0F
#define ANTICRERASE_SETTING		0x10
//#define FAVORITE_SETTING		0x11 //command is changed
#define DRYER_SETTING		        0x12
#define STEAM_SETTING			0x13
#define MACID_SETTING			0x14
//Used for Aroma			0x15
#define WARM_SOAK_SETTING		0x16


/* IoT State Setting */
#define STATE_IOT_IDEAL				0x00
#define STATE_IOT_STANDBY 			0x01
#define STATE_IOT_START 			0x02
#define STATE_IOT_PREWASH 			0x03
#define STATE_IOT_MAINWASH 			0x04
#define STATE_IOT_EXTRA_RINSE_1 		0x05
#define STATE_IOT_EXTRA_RINSE_2	 		0x06
#define STATE_IOT_EXTRA_RINSE_3	 		0x07
#define STATE_IOT_RINSE_1 			0x08
#define STATE_IOT_RINSE_2 			0x09
#define STATE_IOT_FINAL_RINSE	 		0x0A
#define STATE_IOT_FINAL_SPIN	 		0x0B
#define STATE_IOT_ANTI_CREASE	 		0x0C
#define STATE_IOT_END 				0x0D
#define STATE_IOT_PAUSE 			0x0E
#define STATE_IOT_SOAK 				0x0F
#define STATE_IOT_RINSEHOLD 			0x10
#define STATE_IOT_HEATING 			0x11
#define STATE_IOT_DRAIN 			0x12
#define STATE_IOT_INTERMEDIATE_SPIN		0x13
#define STATE_IOT_DELAY_START 			0x14
#define STATE_IOT_DIAGNOSTIC_MODE	 	0x15
#define STATE_IOT_COOL_DOWN 			0x16
#define STATE_IOT_TESTMODE 			0x17
#define STATE_IOT_RINSE_1_MV			0x18
#define STATE_IOT_RINSE_1_PV			0x19
#define STATE_IOT_RINSE_2_MV			0x1A
#define STATE_IOT_RINSE_1_PV_MV			0x1B
#define STATE_IOT_RINSE_2_PV_MV			0x1C
#define STATE_IOT_RINSE_3_PV_MV			0x1D
#define STATE_IOT_EXTRA_RINSE_PV 		0x1E
#define STATE_IOT_EXTRA_RINSE_1_PV 		0x1F
#define STATE_IOT_EXTRA_RINSE_2_PV 		0x20
#define STATE_IOT_EXTRA_RINSE_3_PV 		0x21
#define STATE_IOT_EXTRA_RINSE_MV 		0x22
#define STATE_IOT_HOT_RINSE			0x23
#define STATE_IOT_STEAM				0x24
#define STATE_IOT_RE_HEATING_1			0x25
#define STATE_IOT_RE_HEATING_2			0x26
#define STATE_IOT_RINSE_HOLD_AND_PAUSE          0x27
#define STATE_IOT_DRY 				0x28
//Reserved					0x29
#define STATE_IOT_WARM_SOAK			0x2A
//#define STATE_IOT_SNOOZE_MODE			0xFF


/* Set/Reset Setting for IoT function calls  */
#define SET 				0x01
#define CLEAR 				0x00


/*Option Enable 1*/
#define OPTION_1_PREWASH 			0x01
#define OPTION_1_SOAK 				0x02
#define OPTION_1_WARM_SOAK 			0x04//Change to use for Warm_Soak
#define OPTION_1_HOT_RINSE 			0x08
#define OPTION_1_TIMESAVER 			0x10
#define OPTION_1_DELAYSTART 			0x20
#define OPTION_1_ECO 				0x40
#define OPTION_1_RAPIDWASH 			0x80


/*Option Enable 2*/
#define OPTION_2_SOILAGE 			0x01
#define OPTION_2_FAVORITE 			0x02
#define OPTION_2_RINSE_HOLD 			0x04
#define OPTION_2_ANTI_CREASE 			0x08
#define OPTION_2_SPIN_SPEED 			0x10
#define OPTION_2_TEMPERATURE 			0x20
#define OPTION_2_DOSAGE				0x40
#define OPTION_2_STEAM				0x80


/*Option Enable 3*/
#define OPTION_3_DRY_TIME			0x01
#define OPTION_3_DRY_ECO			0x02
#define OPTION_3_DRY_CUPBOARD			0x04
#define OPTION_3_DRY_IRON			0x08
#define OPTION_3_DRY_LOW_TEMP			0x10
#define OPTION_3_SOLAR_ASSIST			0x20
#define OPTION_3_TURBIDITY			0x40
#define OPTION_3_RESERVED			0x80


/*Wash Program Groups*/
#define WASH_PROGRAM					0x01
#define CUSTOM_WASH						0x02
#define FAVORITES						0x03
#define SPECIAL_PROGRAM					0x04
#define REPEAT_PROGRAM					0x05


/*ALARM 1*/
#define ALARM_1_DOOR_UNLOCK 			0x01
#define ALARM_1_TRIAC_SHORT	 		0x02
#define ALARM_1_TACHO_ERROR		 	0x04
#define ALARM_1_MOTOR_FAILED 			0x08
#define ALARM_1_OVERFLOW 			0x10
#define ALARM_1_OVER_HEAT 			0x20
#define ALARM_1_PRESSURE_SW_FAILED 		0x40
#define ALARM_1_DOOR_LOCK_FAILED 		0x80


/*ALARM 2*/
#define ALARM_2_NO_WATER 			0x01
#define ALARM_2_LOW_WATER_PRESSURE 		0x02
#define ALARM_2_HEATER_FAILURE 			0x04
#define ALARM_2_NTC_ERROR 			0x08
#define ALARM_2_DRAIN_PUMP_FAILED 		0x10
#define ALARM_2_LOW_VOLTAGE 			0x20
#define ALARM_2_HIGH_VOLTAGE 			0x40
#define ALARM_2_UNBALANCE 			0x80


/*Alarm 3*/
#define ALARM_3_BLK_ROTOR			0x01
#define ALARM_3_DRYER_NTC			0x02
#define ALARM_3_BLOWER_MOTOR			0x04
#define ALARM_3_IPM_OVERCURRENT			0x08
#define ALARM_3_IPM_OVERHEAT			0x10
#define ALARM_3_COMM_ERROR			0x20
#define ALARM_3_WIFI_COMM_ERR			0x40
#define ALARM_3_DRYER_HTR			0x80

/* Error Byte Setting */
//#define NO_ERROR 		0x00	//it is already defined in r_cg_userdefined.h (used in controller side too)
#define ERR_DOOR_UNLOCK	 				0x01
#define ERR_OVERFLOW 					0x02
#define ERR_PRESSOSTATE 				0x03
#define ERR_MOTOR_FAILURE 				0x04
#define ERR_TRIAC_SHORT 				0x05
#define ERR_OVER_HEAT 					0x06
#define ERR_DOOR_LOCK 					0x07
#define ERR_NO_WATER 					0x08
#define ERR_LOW_WATER 					0x09
#define ERR_HEATER 					0x0A
#define ERR_NTC 					0x0B
#define ERR_DRAIN_PUMP 					0x0C
#define ERR_LOW_VOLTAGE 				0x0D
#define ERR_HIGH_VOLTAGE 				0x0E
#define ERR_UNBALANCE 					0x0F
#define ERR_COMMUNICATION				0x10
#define ERR_DRYER_NTC					0x11
#define ERR_BLOWER_MOTOR				0x12
#define ERR_IPM_OVERHEAT				0x13
#define ERR_IPM_OVERCURRENT				0x14
#define ERR_DRYER_HEATER				0x15


//Diagnostics Output 1
#define OUTPUT_1_RESET				0x00
#define OUTPUT_1_PRE_WASH_VALVE_1		0x01
#define OUTPUT_1_MAIN_WASH_VALVE_2		0x02
#define OUTPUT_1_WASHER_HEATER			0x04
#define OUTPUT_1_MOTOR_CLOCKWISE		0x08
#define OUTPUT_1_MOTOR_ANTI_CLOCKWISE		0x10
#define OUTPUT_1_DRAIN_PUMP			0x20
#define OUTPUT_1_DOOR_LOCK			0x40
#define OUTPUT_1_PRESSURE_SENSOR		0x80

//Diagnostics Output 2
#define OUTPUT_2_RESET				0x00
#define OUTPUT_2_SPIN_MOTOR			0x01
#define OUTPUT_2_WASHER_NTC			0x02
#define OUTPUT_2_DRYER_HEATER			0x04
#define OUTPUT_2_STEAM_HEATER			0x08
#define OUTPUT_2_CIRCULATION_PUMP		0x10
#define OUTPUT_2_BUBBLE_PUMP			0x20
#define OUTPUT_2_STEAM_MAGNETIC_VALVE		0x40
#define OUTPUT_2_BLOWER_MOTOR			0x80

//Diagnostics Output 3
#define OUTPUT_3_RESET				0x00
#define OUTPUT_3_DRYER_NTC			0x01
#define OUTPUT_3_DRYER_STEAM_THERMOSTATE	0x02


/* Door Lock Setting */  //TBD
#define DOOR_UNLOCKED 				0x01
#define DOOR_LOCKED 				0x02
#define DOOR_LOCKING				0x03
#define DOOR_UNLOCKING 				0x04


/* WiFi Related Setting */
#define WIFI_MODE_AP 				0x01
#define WIFI_MODE_STATION 			0x02
#define WIFI_MODE_LAN 				0x03
/* Washer Load */
#define LOAD_FLAG_1_DOOR					0x01
#define LOAD_FLAG_1_PREWASH_VALVE			0x02
#define LOAD_FLAG_1_MAIN_WASH_VALVE			0x04
#define LOAD_FLAG_1_DRAIN_VALVE_PUMP		0x08
#define LOAD_FLAG_1_WASHER_HEATER			0x10
#define LOAD_FLAG_1_INTER_SPIN				0x20
#define LOAD_FLAG_1_MOTOR_CLK				0x40
#define LOAD_FLAG_1_MOTOR_ACLK				0x80
/* Dryer Load */
#define LOAD_FLAG_2_DRYER_HEATER			0x01
#define LOAD_FLAG_2_WLDP					0x02
#define LOAD_FLAG_2_ALDP					0x04
#define LOAD_FLAG_2_DRY_FAN					0x08
#define LOAD_FLAG_2_DRY_VALVE				0x10
#define LOAD_FLAG_2_BUBBLE_PUMP				0x20
#define LOAD_FLAG_2_CIRCULATION_PUMP		0x40
#define LOAD_FLAG_2_STEAM_VALVE				0x80

/* Load Flag Byte Setting */
#define LOAD_PREWASH 					0x03
#define LOAD_MAINWASH 					0x05
#define LOAD_DRAIN 						0x09
#define LOAD_HEATER 					0x11


//----------------------------------------------------------


/* Program LED Setting */
#define PROGRAM_LED_COTTON		0x01
#define PROGRAM_LED_COTTONECHO		0x02
#define PROGRAM_LED_SYNTHETIC 		0x04
#define PROGRAM_LED_MIXEDSOIL 		0x08
#define PROGRAM_LED_BABYWEAR 		0x10
#define PROGRAM_LED_CRADLE 		0x20
#define PROGRAM_LED_DRAINSPIN 		0x40
#define PROGRAM_LED_ADDITIVERINSE 	0x80

/* Programs LED1 Byte Setting */
#define PROGRAM_LED1_TUBCLEAN		0x01
#define PROGRAM_LED1_HYGINE		0x02
#define PROGRAM_LED1_WOOLLENS		0x04
#define PROGRAM_LED1_EXPRESS		0x08
#define PROGRAM_LED1_UNIFORM		0x10
#define PROGRAM_LED1_BULKY		0x20
#define PROGRAM_LED1_SPORTSWEAR		0x40
#define PROGRAM_LED1_CURTAINS		0x80

/* Status LED Byte Setting */
#define STATUS_LED_CHILDLOCK 	0x01
#define STATUS_LED_WASH 	0x02
#define STATUS_LED_RINSE 	0x04
#define STATUS_LED_END 		0x08
#define STATUS_LED_DOOR 	0x10
#define STATUS_LED_ERROR 	0x20
#define STATUS_LED_STARTPAUSE 	0x40
#define STATUS_LED_PROGRESS 	0x80

/* Status LED1 Byte Setting */
#define STATUS_LED1_KG 		0x01
#define STATUS_LED1_DEGREE 	0x02
#define STATUS_LED1_RPM 	0x04
#define STATUS_LED1_NOSPIN 	0x08
#define STATUS_LED1_DELAY 	0x10
#define STATUS_LED1_PREWASH 	0x20
#define STATUS_LED1_SPIN 	0x40
#define STATUS_LED1_TAP 	0x80	//water filling

/* Status LED2 Byte Setting */
#define STATUS_LED2_DRAIN 0x01

/* Option LED1 Byte Setting */
#define OPTION_LED1_PREWASH 	0x01
#define OPTION_LED1_SOAK 	0x02
#define OPTION_LED1_EXTRARINSE 	0x04
#define OPTION_LED1_HOTRINSE 	0x08
#define OPTION_LED1_TIMESAVER 	0x10
#define OPTION_LED1_DELAYSTART 	0x20
#define OPTION_LED1_ECO 	0x40


//IOT_END

#endif
