/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_userdefine.h
* Version      : CodeGenerator for RL78/G13 V2.05.06.02 [08 Nov 2021]
* Device(s)    : R5F100JJ
* Tool-Chain   : CA78K0R
* Description  : This file includes user definition.
* Creation Date: 02-08-2023
***********************************************************************************************************************/

#ifndef _USER_DEF_H
#define _USER_DEF_H

/***********************************************************************************************************************
User definitions
***********************************************************************************************************************/
#include "Version.h"
#include "AI_Comm_Global.h"
/* Start user code for function. Do not edit comment generated here */

//****************************************

#define _1SEC_COUNT_TESTMODE	1004
#define _1SEC_COUNT		1009
#define TOGGLE_BIT 1

//#define MAX_LOADSENSE_COUNT	3//9//2
#define MAX_LOADSENSE_COUNT_COTTON	3
#define MAX_LOADSENSE_COUNT_SYNTH_MIX 2
//#define LS_ANTICREASE_TIME	2
#define LS_SYNTH_RINSE_FILL_DIFFERENCE	3//4
#define LS_COTTON_RINSE_FILL_DIFFERENCE	5
#define LS_MIX_RINSE_FILL_DIFFERENCE	2

#define SWITCH_ON  1
#define SWITCH_OFF 0

#define YES 1
#define NO 0

#define RESET_VALUE 0
#define STEP_FINISHED 1
#define STEP_INPROGRESS 0

#define ENABLE_IOT  1 //To enable IOT Functionality
#define REACHED 1
#define NOT_REACHED 0
#define LOW_PRESSURE_CONDITION 1
#define NO_WATER_CONDITION 2


#define STLED316_BRGHT_MODE_CONST 	0x18
#define STLED316_BRGHT_MODE_VAR 	0x0 

//-----------------------------------------------------------------------//
//*********************WaterFrequency Level******************//
#define DANGER_WATERLEVEL 	36669//36379


#define WOOL_WASHLEVEL 		38209 	// wool wash level
#define WOOL_RINSELEVEL 	38209 	// wool rinse level

#define TUMBLE_SPECIAL 		39629 	// tumbling for wool and delicates
#define TUMBLE_NORMAL 		41249//41209   // tumbling for other programs

#define RINSESPIN_RINSELEVEL 	38099 // Add Rin#define SYNTHETIC_WASHLEVEL 24580   // synthetic wash level


#define CRADLE_WASHLEVEL  	37439  // cradle washlevel
#define CRADLE_RINSELEVL  	37439  // cradle rinselevel

#define RESET_LEVEL 		40369  // iWaterLevel reset level
#define EASY_WATERLEVEL 	39459

#define ECO_WASHLEVEL 		39519
#define ECO_RINSELEVEL 		38559//39459

#define ECO60_WASHLEVEL_1	40139
#define ECO60_WASHLEVEL		39879
#define ECO60_HEAT_WASHLEVEL	40429

#define CURTAINS_WASH_LEVEL	39999
#define CURTAINS_RINSE_LEVEL	38329

#define TUBCLEAN_WASHLEVEL	38329//39459

/*-------------------------------DEFAULT CAPACITY-----------------------------------------------*/
#define EXPRESS_DEFAULT_KG	25//30
#define WETCLEAN_DEFAULT_KG	20
#define CRADLE_DEFAULT_KG	20
#define ADDITIVESRINSE_DEF_KG	80
#define STEAMFRESH_DEFAULT_KG	10
#define ANTIALLERGEN_DEFAULT_KG	30
#define BABYWEAR_DEFAUL_KG	30
#define STEAMWASH_DEFAULT_KG	30
#define INNERWEAR_DEFAULT_KG	10//15
#define CURTAINS_DEFAULT_KG	20
#define DARKWASH_DEFAULT_KG	30
#define SPORTSWEAR_DEFAULT_KG	20
#define JEANS_DEFAULT_KG	35
#define UNIFORM_DEFAULT_KG	30
#define SHIRTS_DEFAULT_KG	20
#define LINENWASH_DEFAULT_KG	30
#define WOOLENS_DEFAULT_KG	20

/*------------------------------__PREWASH TIME__-------------------------------------*/

#define BABY_PREWASH_TIME	15
#define ANTI_PREWASH_TIME	15

/*------------------------------__Mixed Program Parameter__------------------------------------- */

#define TIME_SAVE_MIEXD40_TIME  	57//56	/* Timesaver Time*/
#define TIME_SAVE_MIXED30_TIME 		61//58
#define TIME_SAVE_MIXED60_TIME 		69//63//68//67	
#define TIME_SAVE_MIXEDCLD_TIME 	54//62	

#define MIXED40_TIME 			60	/* Default Time*/
#define MIXED30_TIME 			70//67	
#define MIXED60_TIME 			75	
#define MIXEDCLD_TIME 			60//75	

#define MIXEDSOIL_LENGTH 		55//54//47//46//DRAIN PREWASH STEP ADDED TO ENSURE COMPLETE DRAIN IN PREWASH AI PROGRAMS

#define MIXED_PREWASH_TIME		12//11 incremented for drain prewash
#define MIXED_ER_TIME			10//8
#define MIXED_ANTICREEASE_TIME		2//3
#define MIXED_FINALSPIN_TIME		11
#define MIXED_DEFAULT_KG		50//80//90
#define MIXED_MAX_ER			3	/*Max Extra Rinse Count*/
#define MIXED_WASHLEVEL 		39159	 
#define MIXED_RINSELEVEL 		38849	 
#define MIXED_RESET_LEVEL		40130	
#define MIXED_STEAM_RESET_LEVEL         42000//Given for MIXED tcs changes

#define MIXED_MAX_RESET_LIMIT		6 	// Max Reset updated as per Spec EL-PROG-285 Rev17

/* EC-TL-285 As per Spec rev-16 max reset count mentioned is 6, if reset count*/
/*increases beyond 6 we will consider 6 for "MIXED_TumbleTXResetTime_ui8 chart".*/
#define MIXED_RESET_LIMIT(num)	num = num>MIXED_MAX_RESET_LIMIT?MIXED_MAX_RESET_LIMIT:num	/*Dev- Prashant_H 25_05_2022*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------__COTTON cold,30,40,60,95 Program Parameter__-------------------------------------------- */

#define TIME_SAVE_COTTONCLD_TIME   	96//97//84//89//75 	/* Timesaver Time*/
#define TIME_SAVE_COTTON30_TIME    	106//107//96//82	//1 min extra
#define TIME_SAVE_COTTON40_TIME    	108//109//95//81//80  //recalculated
#define TIME_SAVE_COTTON60_TIME    	102//103//87//88//73 
#define TIME_SAVE_COTTON90_TIME    	138//139//104//92//77 

#define COTTONCLD_TIME    		104//105//110//108//116//108 	/* Default Time*/
#define COTTON30_TIME     		117//118//119//119//116//108	//1 min extra
#define COTTON40_TIME     		117//118//119//110//118//110//109  /*One minute increased to remove the simulataneous occurence of daat tx and step switch DOC:12/07/2022*/
#define COTTON60_TIME     		124//125//130//114//122//114 
#define COTTON90_TIME     		164//165//125//135//120//127//120 

#define C_LENGTH 			71//69//68//62
#define C_60_90_LENGTH 			63//62//56//NEOO

#define COTTON_PREWASH_TIME		18		
#define COTTON_ER_TIME			11
#define COTTON_RINSE1_TIME		15
#define COTTON_FINALSPIN		10//11
#define COTTON_ANTICREASE_TIME		2
#define COTTON_DEFAULT_KG		80
#define COTTON_MAX_ER			3		/*Max Extra Rinse Count*/
#define C_WASHLEVEL 			39999	 
#define C_RINSELEVEL			38559
#define COTTON_RESET_LEVEL
#define COTTON_RINSE_RESET_LEVEL	40710

#define COTTON_MAX_RESET_LIMIT		6	// Max Reset updated as per Spec EL-PROG-285 Rev17

/* EC-TL-285 As per Spec rev-16 max reset count mentioned is 6, if reset count*/
/*increases beyond 6 we will consider 6 for "COTTON_TumbleTXResetTime_ui8 chart".*/
#define COTTON_RESET_LIMIT(num)	num = num>COTTON_MAX_RESET_LIMIT?COTTON_MAX_RESET_LIMIT:num	/*Dev- Prashant_H 25_05_2022*/

/*---------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------__SYNTHETIC cold,30,40,60 Program Parameter__-------------------------------------------------------------------------------------- */

#define TIME_SAVE_SYNTHETIC60_TIME 		104//81//84//81	/* Timesaver Time*/		
#define TIME_SAVE_SYNTHETIC40_TIME 		105//76	
#define TIME_SAVE_SYNTHETIC30_TIME 		97//73
#define TIME_SAVE_SYNTHETICCLD_TIME 		75//80//86//78//81//78

#define SYNTHETIC60_TIME 			119//115//91	/* Default Time*/	
#define SYNTHETIC40_TIME 			118//118//92//91	
#define SYNTHETIC30_TIME 			117//115//117//100//101	
#define SYNTHETICCLD_TIME 			81//84//81//92//100//101	

#define SYNTH_LENGTH 				61//56//55 //DRAIN PREWASH IS ADDED TO ENSURE COMPLETE DRAIN IN AI PREWASH SLELCTED PROGRAM
#define SYNTH_PREWASH_TIME			15///14 incremeted for drain prewash
#define SYNTH_ER_TIME				10//8
#define SYNTH_ANTICREAS_TIME			2
#define SYNTH_FINALSPIN_TIME			11
#define SYNTH_RINSE2_TIME			13
#define SYNTH_RINSE1_TIME			13
#define SYNTH_DEFAULT_KG			50//80
#define SYNTH_MAX_ER				3	/*Max Extra Rinse Count*/
#define SYNTHETIC_RINSELEVEL 			38849//38209 	
#define SYNTHETIC_WASHLEVEL     		39150	
#define SYNTHETIC_RESET_LEVEL			40139	

#define SYNTHETIC_MAX_RESET_LIMIT		4	/* Max Reset updated as per Spec EL-PROG-285 Rev17*/

/* EC-TL-285 As per Spec rev-16 max reset count mentioned is 6, if reset count*/
/*increases beyond 4 we will consider 4 for "SYNTH_TumbleTXResetTime_ui8 chart".*/
#define SYNTHETIC_RESET_LIMIT(num)	num = num>SYNTHETIC_MAX_RESET_LIMIT?SYNTHETIC_MAX_RESET_LIMIT:num	/*Dev- Prashant_H 25_05_2022*/

/*---------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------__EXPRESS_30 Program Parameters__------------------------------------------------------------- */

#define EXPRESS_40_TIME			30
#define EXPRESS_60_TIME			55
#define EXPS_30_LENGTH			25//24
#define EXPRESS_30_RPM1200_TIME		2

#define EXPRESS_30_WASHSET_LEVEL	39399
#define EXPRESS_30_RINSELEVEL 		37979 
#define EXPRESS_30_ER_TIME		7
#define EXPRESS_30_MAX_ER		2
#define EXPRESS_30_DEFAULT_KG		50//diptesh told to change
#define EXPRESS_30_FINALSPIN		6
#define EXPRESS_30_ANTICREASE_TIME	2

/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*TODO: Added for AI program-user selected program.*/
#define GET_PROGRAM 			(getAIComm_ExecState_ui8() == AI_Done)? getAIComm_Program_ui8(): uiSelected_Position  

#define EXPRESS_15_WASHLEVEL		39399		
#define EXPRESS_RINSELEVEL 		37979 


#define BABY_WASHLEVEL 		39159 	 // cotton wash level
#define BABY_RINSELEVEL 	38329 
#define BABY_EXTRARINSELEVEL 	38209 
#define ALLERGY_RINSELEVEL	37979
#define SPORTSWEAR_RINSELEVEL	38679
#define STEAMWASH_RINSELEVEL	38979
#define INNERWEAR_RINSELEVEL	38079
#define SHIRTS_RINSELEVEL	39029
#define SHIRTS_WASHLEVEL	39289

#define TWO_LITER 		41209
#define THREE_LITER  		41139
#define FOUR_LITER 		41469//freq changed as per PR by PED/quality--spec updated;for S5H line issue//41309//41059
#define SF2_HTR_ON_FREQ		41419//as per above PR and spec update
#define SIXTEEN_LITER		38359
#define S2_LEVEL		41340
#define S3_LEVEL		41180
#define S4_LEVEL		41050

#define ON_PAUSE_DOOR_OPEN_WATERLEVEL 	39709//39399
#define COOL_DOWN_LIMIT_FREQUENCY 	36679
#define FOAM_LEVEL_LIMIT 		36679
#define EMPTYLEVEL 			42300
#define EMPTYLEVEL1 			42120
#define HYGINE_DRAIN			40460
#define HYGINE_DRAIN_1			40460//41150//40460//41000//41150 //Added for mixed TCS

#define LOW_PRESSURE 		41100
#define ADDITIVES_MAIN 		40259
#define ADDITIVES_PRE 		40409
#define RINSE_DESIRED 		39469
#define EIGHTEEN_LITRES 	38079
#define FIVE_LITER		40209

#define ECO_RESET_LEVEL 	40619     // iWaterLevel reset level for Eco Program
#define FOAM_SENSING_FREQUENCY 	40259//39609//39399 
#define FOAM_ECOSENSING_FREQUENCY 37639//37249

//----------------------------------------------------------------//
//********** Temperature values of program*********************//
//#define TEMP_20		20
#define TEMP_30		30
#define TEMP_36 	36
#define TEMP_40		40
#define TEMP_43		43
#define TEMP_56    	56
#define TEMP_57   	57
#define TEMP_60		60
#define TEMP_65 	65
#define TEMP_90		80

//------------------------------------------------------------------//
//*****************************program lenghts*********************//

//------------------------------------------------
#define WOOL_LENGTH 	16
#define DEL30_LENGTH 	13
#define EASY30_LENGTH 	19
#define EASY_LENGTH 	26
#define ECO_LENGTH 	28
//------------------------------------------------
#define ECO_COLD_LENGTH 20
#define ECO_40_LENGTH 	24//23
#define ECO_60_LENGTH 	24//23
//------------------------------------------------
#define S40_LENGTH 	28
#define S60_LENGTH 	31

//------------------------------------------------
//BTRA//41
//------------------------------------------------
#define EXPS_LENGTH 	18//17

//#define EXPS_LENGTH 	6
//------------------------------------------------
#define CRDL_LENGTH 	19//18
//------------------------------------------------
#define WOOL30_LENGTH 	17
//------------------------------------------------
#define SPORT_LENGTH	28
//------------------------------------------------
#define ANTIAL_LENGTH 	34//31//30
//------------------------------------------------
#define CUR_LENGTH	24
//------------------------------------------------
#define WASH_LENGTH	26
#define INNER_LENGTH	23
#define UNI_LENGTH	33
#define JEANS_LENGTH	45
#define SHIRTS_LENGTH	29
#define LINENWASH_LENGTH      34

#define STEAM_FRESH_LENGTH  5
#define STEAM_LENGTH	20//19 	//17 TODO: Added CP on extra step As spec Rev9.
//------------------------------------------------
#define BABYCARE_LENGTH 35//31
#define TUB_LENGTH	22
//----------------------------------------------------------//

#define RINSE_SPIN_LENGTH 	5
#define DRAIN_SPIN_LENGTH 	5
#define ADDITIVES_RINSE_LENGTH 	14
#define ADDITIVES_ENDURANCE_LENGTH 8
//*************************************************************************//
//------------------------------------------------------------
//************Low/No Water ErrorCondition Timming***********//
#define LOWPRESSURE_STEPONE_TIME 	2
#define LOWPRESSURE_STEPTWO_TIME 	10
#define LOWPRESSURE_STEPTHREE_TIME 	20
#define LOWPRESSURE_STEPFOUR_TIME 	40
#define LOWPRESSURE_STEPFIVE_TIME 	60
#define NOWATER_STEPONE_TIME 		3 //TODO: Revert the changes as per pervious set time 
#define NOWATER_STEPTWO_TIME 		8
#define NOWATER_STEPTHREE_TIME 		13
#define NOWATER_STEPFOUR_TIME 		18
//------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//***************Valve Selection Value**************************//
#define RINSE_PREWASH 			20  	// rinse bPrewashValve
#define RINSE_MAINWASH 			21 	// rinse bMainWashValve
#define RINSE_BOTH 			26     	// rinse additives
#define MAIN_WASH 			6     	// bMainWashValve
#define PRE_VALVE 			1     	// bPrewashValve 
#define SPECIAL_VALVE 			23 
//---------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//********************************************

//*******************************************
//--------------------------------------------------------------
#define COOLDOWN_TEMPERATURE 	60
//--------------------------------------------
//**************Timming*****************************// 
#define FILL_TIME 			1
#define ONE_MINUTE 			1
#define ZERO_MINUTE 			0
#define TUMBLE_SWITCH_SECONDS 		2
#define ADDTITIONAL_HEAT_MINUTES 	10
#define ADDTITIONAL_DRAIN_THREE_MINUTES 3
#define ADDTITIONAL_DRAIN_FOUR_MINUTES 	4
#define MILLI_THOUSAND 		1000
#define NEXTSTEP_DELAY 		2

#define ONE_MINUTE_SECONDS 	59
#define HALF_MINUTE_SECONDS 	30

#define ONE_SECOND 		1
#define TWO_SECONDS 		2
#define THREE_SECONDS 		3
#define FOUR_SECONDS 		4
#define FIVE_SECONDS 		5
#define SIX_SECONDS 		6
#define SEVEN_SECONDS 		7
#define EIGHT_SECONDS 		8
#define NINE_SECONDS		9
#define TEN_SECONDS 		10
#define ELEVEN_SECONDS 		11
#define TWELVE_SECONDS 		12
#define THIRTEEN_SECONDS 	13
#define FOURTEEN_SECONDS 	14
#define SEVENANDHALFMINUTES 	15
#define FIFTEEN_SECONDS 	15
#define SIXTEEN_SECONDS 	16
#define SEVENTEEN_SECONDS 	17
#define EIGHTEEN_SECONDS 	18
#define NINETEEN_SECONDS 	19
#define TWENTY_SECONDS 		20
#define TWENTY_ONE_SECONDS 	21
#define TWENTY_FOUR_SECONDS 	24
#define TWENTY_FIVE_SECONDS 	25
#define THIRTY_SECONDS 		30
#define FOURTY_SECONDS 		40
#define FIFTY_SECONDS 		50
#define FIFTY_ONE_SECONDS 	51
#define FIFTY_FIVE_SECONDS 	55
#define SIXTY_SECONDS 		60
#define SIXTY_TWO_SECONDS	62
#define SIXTY_FOUR_SECONDS 	64
#define SIXTY_EIGHT_SECONDS 	68
#define SIXTY_NINE_SECONDS 	69
#define SEVENTY_TWO_SECONDS 	72
#define SEVENTY_SEVEN_SECONDS 	77
#define EIGHTY_ONE_SECONDS 	81
#define NINETY_FIVE_SECONDS 	93
#define NINETY_NINE_SECONDS 	99

#define ONE_HUNDRED_SECONDS 		100
#define ONE_HUNDRED_FOUR_SECONDS 	104
#define ONE_HUNDRED_FOURTEEN_SECONDS 	114
#define ONE_HUNDRED_FIFTEEN_SECONDS 	115
#define ONE_HUNDRED_NINETEEN_SECONDS 	119
#define ONE_HUNDRED_TWENTY_SECONDS 	120
#define ONE_HUNDRED_FORTY_FIVE_SECONDS 	145
#define ONE_HUNDRED_SEVENTY_NINE_SECONDS 179
#define ONE_HUNDRED_NINETY_FOUR_SECONDS 194
#define TWO_HUNDRED_TWENTY_FOUR_SECONDS 224
#define TWO_HUNDRED_SEVENTY_FIVE_SECONDS 275
#define TWO_HUNDRED_FORTY_SECONDS       240
#define TWO_HUNDRED_NINETY_THREE_SECONDS 293
#define THREE_HUNDRED_NINETY_FIVE_SECONDS        395
#define FOUR_HUNDRED_TWENTY_FIVE_SECONDS         425
#define FIVE_HUNDRED_FORTY_FIVE_SECONDS          545
#define FIVE_HUNDRED_SEVENTY_FIVE_SECONDS        575
#define SIX_HUNDRED_FIVE_SECONDS        	 605
#define SEVEN_HUNDRED_TWENTY_FIVE_SECONDS        725
#define SEVEN_HUNDRED_FIFTY_FIVE_SECONDS         755
#define SEVEN_HUNDRED_EIGHTY_FIVE_SECONDS	 785
#define EIGHT_HUNDRED_SEVENTY_FIVE_SECONDS       875
#define NINE_HUNDRED_FIVE_SECONDS       	 905
#define NINE_HUNDRED_THIRTY_FIVE_SECONDS         935

#define TWO_HUNDRED_THIRTEEN_SECONDS 213
#define ONE_HUNDRED_SEVENTY_ONE_SECONDS 171
#define TWO_HUNDRED_NINETY_ONE 291
#define FOUR_HUNDRED_ELEVEN 411
#define FIVE_HUNDRED_THIRTY_ONE 531
#define SIX_HUNDRED_FIFTY_ONE 651
#define SIX_HUNDRED_EIGHTY_ONE 681
#define SEVEN_HUNDRED_TWENTY_SECONDS 720




//----------------------------------------------------------------
//***************Key Value*********************//
#define BUTTON_MASK 		0x70
#define BUTTON_START 		7
#define BUTTON_TEMPERATURE 	0xb
#define BUTTON_RAPIDWASH 	0xd
#define BUTTON_DELAY_START 	0xe
#define BUTTON_TESTMODE 	0xc
#define BUTTON_CHILDLOCK 	0x22
#define KEY_DEBOUNCE_TIME 	10
//--------------------------------------------------------------------
#define SET_BIT(value,sbit) (value = value | (1<<sbit))
#define CLR_BIT(value,sbit) (value = value &(~(1<<sbit)))
//---------------------------------------------------------------------
//*************Option Value**************//
//#define RinseHold_YES	0x40
#define Prewash_Yes	0x01
#define WarmSoak_Yes	0x02
#define ExtraRinse_Yes 	0x04
#define Steam_Yes 	0x08
#define DelayStart_Yes	0x10
#define TimeSaver_Yes	0x20
//----------------------------------------------------------------------
//*************RPM Value***********************//
#define Rinsehold	0x00
#define Rpm000		0x10
#define Rpm400		0x20
#define Rpm600		0x30
#define Rpm800		0x40
#define Rpm1000		0x50
#define Rpm1200		0x60
//#define Rpm1400		0x70
//----------------------------------------------------------------------
//*****************Program Timmings***************************//




//-----------------------------------------------------------------------
//#define TIME_SAVE_COTTON20_TIME    97//96 // verified 


//----------------------------------------------------------------------
#define COTTONECO40_TIME 	160//164//167//153
#define COTTONECO60_TIME 	166//167//169//167//170//172//177//175//168//161//163//issue reported
#define COTTON_ECO_FINALSPIN	10//13//FS2 TO 10 MINS
//-----------------------------------------------------------------------


/*-----------------------------------------*/

#define BABYWEARCLD_TIME		99//100//102 // verified
#define TIME_SAVE_BABYWEARCLD_TIME	66//67//69 //63//68  // verified 65+4 filling
#define BABYWEAR40_TIME			109//110//121 // verified 
#define TIME_SAVE_BABYWEAR40_TIME	72//73 //67//78  // verified 69+4 filling
#define BABYWEAR60_TIME			109//110//121 // verified 
#define TIME_SAVE_BABYWEAR60_TIME	81//82//85 //77//88  // verified 81+4 filling
//-----------------------------------------------------------------------
#define CRADLEWASHCLD_TIME  	44//41//43 // verified (41) actual 44--tumb step afer heating should be 6
#define CRADLEWASH30_TIME  	45//46//44 // verified (45)
#define CRADLEWASH40_TIME  	58//56//57 // verified (58)
//-----------------------------------------------------------------------
#define ANTIALG40_TIME			117//122//121 // verified 
#define TIME_SAVE_ANTIALG40_TIME	78//81 // verified 
#define ANTIALG60_TIME			117//117//116 // verified 
#define TIME_SAVE_ANTIALG60_TIME	91//91 // verified 
#define ANTIALG95_TIME			113//117//116 // verified 
#define TIME_SAVE_ANTIALG95_TIME	109//91 // verified 
//-----------------------------------------------------------------------
#define SPORTWAERCLD_TIME		65//64 // verified 
#define TIME_SAVE_SPORTWAERCLD_TIME	50//49 // verified 
#define SPORTWAER40_TIME		74//73 // verified 
#define TIME_SAVE_SPORTWAER40_TIME	58//57 // verified 
//-----------------------------------------------------------------------
#define TUBCLEANCLD_TIME 		64//63 // verified //updated
#define TUBCLEAN60_TIME			110 // verified 
#define TUBCLEAN90_TIME			105//111 // verified 
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//#define SPINDRY_TIME     	18//17 // verified 
//#define TIME_SAVE_SPINDRY_TIME 	13//12 // verified 
//-----------------------------------------------------------------------
#define EXPRESSCLD_TIME 	15 // verified 36
#define EXPRESS30_TIME  	15 // verified 36
#define EXPRESS40_TIME  	20 // verified 51
//-----------------------------------------------------------------------
#define DARKWASH40_TIME		78//77
#define TIME_SAVE_DARKWASH40_TIME 69//68

#define CURTAINCLD_TIME 69//68//70(26 Apr 20)
#define CURTAIN60_TIME	99//98
#define CURTAIN40_TIME	79//78

#define TIME_SAVE_CURTAIN60_TIME  81//80	
#define TIME_SAVE_CURTAIN40_TIME  61//60	
#define TIME_SAVE_CURTAINCLD_TIME 41//40
//-----------------------------------------------------------------------
#define WOOLLENSCLD_TIME    	43 // verified 
#define WOOLLENS30_TIME    	43 // verified 
#define WOOLLENS40_TIME    	46 // verified 
//-----------------------------------------------------------------------
#define RINSESPIN_TIME   		18//19//22//23//21 // verified 
#define TIME_SAVE_RINSESPIN_TIME   	13//14//17//18//16 // verified 
//-----------------------------------------------------------------------
#define INNERWEAR_TIME			73//72//
#define TIME_SAVE_INNERWEAR_TIME	60//59//
//-----------------------------------------------------------------------
#define STEAMWASH_TIME		30//30//
#define STEAMFRESH_TIME		25//
#define STEAM_LINEN             17
//-----------------------------------------------------------------------
#define UNIFORMCLD_TIME			100//25/6
#define TIME_SAVE_UNIFORMCLD_TIME	66//
#define UNIFORM40_TIME			100//
#define TIME_SAVE_UNIFORM40_TIME	71//
#define UNIFORM60_TIME			119//
#define TIME_SAVE_UNIFORM60_TIME	91//
//-----------------------------------------------------------------------
#define SHIRTCLD_TIME		69//
#define SHIRT30_TIME		75//
#define SHIRT40_TIME		89//
#define SHIRT60_TIME		109//

#define TIME_SAVE_SHIRTCLD_TIME		56//
#define TIME_SAVE_SHIRT30_TIME		61//
#define TIME_SAVE_SHIRT40_TIME		76//
#define TIME_SAVE_SHIRT60_TIME		96//
//-----------------------------------------------------------------------
#define JEANS60_TIME		182//183//186//25/6
#define TIME_SAVE_JEANS60_TIME	128//129//132//
//----------------------------------------------------------------------
#define WETCLEAN_LENGTH         28//27
#define WETCLEANCLD_TIME        40
#define WETCLEAN30_TIME         45
#define WETCLEAN40_TIME         60

#define LINENWASHCLD_TIME       52
#define LINENWASH30_TIME        49
#define LINENWASH40_TIME        49
//----------------------------------------------------------------------
#define SNOOZEMODE_MINUTESCOUNT 	30000

//-----------------------------------------------------------------------
#define PREWASH_TIME1    	11	//BTRA //12
#define PREWASH_TIME2		17
#define PREWASH_TIME3		14	//BTRA //15
#define SOAK_TIME 		10
#define HOTRINSE_TIME 		20
//-------------------------------------------------------------------------
//****************** ERRORS Storage ********************//
#define E1 	0x01
#define E2 	0x02
#define E3 	0x03
#define E4 	0x04
#define E5 	0x05
#define E6 	0x06
#define E7 	0x07
#define E8 	0x08
#define E9 	0x09
#define E10 	0x0A
#define E11 	0x0B
#define E12 	0x0C
#define E13 	0x0D
#define E14 	0x0E
#define E15 	0x0F 
#define E16 	0x10 
#define E17	0x11
#define E18	0x12
#define E19	0x13
//------------------------------------------------------------------------------
//*************************Key Values****************//
#define POWER_KEY 	0x5000// touch/tact key in neo series direct to micom

#define START_KEY 	0x1000//0x0020 //SW2 ON IB
#define TEMPERATURE_KEY 0x0010//0x0200 //SW6 ON IB
#define SPIN_KEY 	0x0200//0x0002 //0x0010 //SW1 ON IB
#define SELECT_KEY 	0x0002//0x0100 //SW5 ON IB//select key is used as STEAM key in neo series
#define TIMESAVER_KEY	0x0100//SW4 ON IB
#define SOAK_KEY	0x0020//SW3 ON IB
//#define PROG_SW_1_5	0x1000 //SW2 ON IB// PROGRAM SWITCH KEY  1- 5 INSTEAD OF ENCODER
//#define PROG_SW_6_10	0x0010 //SW6 ON IB// PROGRAM SWITCH KEY  6- 10 INSTEAD OF ENCODER

//-----MULTI KEYS---//NAMES KEPT SAME 
#define TEMP_OPTION_KEY 0x0210 //0x0804 // DEBUG MODE KEY //SW6 + SW1 //temp + spin
#define SPIN_TEMP_KEY   0x0202 //0x0C00 // CHILD LOCK KEY //SW5 + SW1//spin+steam

//--MULTI KEYS WITH POWER KEY
#define TEMP_SELECT_KEY 0x0012//0x0102 //test mode // SW6 + SW5
#define TEST_MODE_KEY	0x5012 //for set the TEST mode set var // SW5 + SW 4 + POWER //neo with knob SPIN+STEAM+POWER
#define LAB_MODE_KEY	0x5212//SW6 + SW1 + SW5 + POWER
#define LAB_MODE	0x0212//SW6 + SW1 + SW5 
#define SPIN_SELECT_KEY 0x1020//0x0120//0x0202 // fct mode same values//SW3 + SW4
#define FCT_MODE_KEY	0x6020//0x5120 // for the FCT mode set var//SW3 + SW4 + POWER

//----KEY DOUBTS--//
#define OPTION_KEY 	0x00A0//0x0002 //NOT USING 
//-----------------------------------------------------------------------------------
//************Display Symbol Set Value*************************//
#define TIMEDOTS_ON 1
#define TIMEDOTS_OFF 0
#define KG_ON	1
#define KG_OFF	0
#define	SEMI_ON	1
#define SEMI_OFF 0
#define RPM_ON	1
#define RPM_OFF 0 
#define NOSPIN_ON 1
#define NOSPIN_OFF 0 
#define CENTI_ON 1
#define CENTI_OFF 0 
#define TIMELEFT_ON 1
#define TIMELEFT_OFF 0
#define OXYJET_ON	1
#define OXYJET_OFF	0

/* TubClean Program setting for IOT Only */
#if ENABLE_IOT
//#define IOT_INNERWEAR 	 12
//#define IOT_CURTAINS     13
//#define IOT_DARKWASH     14
//#define IOT_TUBCLEAN     15
//#define IOT_SPORTWEAR	 16
//#define IOT_JEANS	 17
//#define IOT_UNIFORM      18
//#define IOT_SHIRTS       19
//#define IOT_STEAMWASH	 20
//#define IOT_LINEN        21//21
//#define IOT_WOOLLENS     22//22
//#define IOT_ANTIALLERGEN 23//21
//#define IOT_EXPRESS_30   24//22

#define IOT_WOOLLENS     10
#define IOT_SPORTWEAR	 11
//#define IOT_INNERWEAR  11
//#define IOT_CURTAINS   12
#define IOT_DARKWASH     12
#define IOT_TUBCLEAN     13
#define IOT_INNERWEAR 	 14
#define IOT_JEANS	 15
#define IOT_UNIFORM      16
#define IOT_SHIRTS       17
#define IOT_STEAMWASH	 18
#define IOT_LINEN        19//21
#define IOT_ANTIALLERGEN 20//21
#define IOT_EXPRESS_30   21//22//need to be removed 
//neo 8kg reference is 7kg sensorless hence there is only express 15 & steamwash [facia prog as express 30]
//not commenting from userdefine to avoid error in code //IOT team will have only till enum 22
#endif

//----------------------------------------------------------------------------------------------------------------------------------------------


enum WASOPERATIONS_STATES
{
	ENTRY =0,
	DO,
	EXIT,
	LOAD_OPERATION
};
enum WASH_TEMP
{
	WashCold	= 0x01,
	Wash30,		//2	
	Wash40,		//3	
	Wash60,		//4	
	Wash95,		//5	
	Wash40E,	//6		
	Wash60E,	//7
	P_COLD,		//8
	P_30,		//9
	P_40,		//10
	P_60,		//11
	P_95		//12
};

//enum ePrograms
//{
//	POSITION0,
//	MIXEDSOIL,
//	COTTON,
//	BABYWEAR,
//	EXPRESS, //ANTIALLERGEN,
//	//EXPRESS_30,/*STEAMWASH,*/	//BTRA New update.
//	STEAMFRESH,
//	ADDITIVESRINSE,/*DRAINSPIN,*/
//	CRADLEWASH,
//	WETCLEAN, //WOOLLENS//SYNTHETIC
//	SYNTHETIC, //WOOLLENS
//	SMARTPLUS,
//	NO_OF_PROGRAMS
//};

//----New Facia--//
enum ePrograms
{
	POSITION0,
	MIXEDSOIL,
	COTTON,
	SYNTHETIC,
	BABYWEAR, 
	STEAMFRESH,
	ADDITIVESRINSE,
	CRADLEWASH,
	CURTAINS,
	EXPRESS, //WOOLLENS
	SMARTPLUS,
	NO_OF_PROGRAMS
};

enum SmartProgram
{
	WOOLLENS,
	SPORTWEAR,//INNERWEAR,
	//CURTAINS,
	DARKWASH,
	TUBCLEAN,
	INNERWEAR,//SPORTWEAR,
	JEANS,
	UNIFORM,
	SHIRTS,
	STEAMWASH,
	LINENWASH,
	ANTIALLERGEN,
	EXPRESS_30
};
//----------------------------------------------------------------------------------------------------------------------------------------------
enum Stage{
	DELAYSTART,
	OPTIONAL_PREWASH,
	PREWASH_TUMBLING,
	PRE_HEAT,
	PREWASH_SPIN,
	DRAIN_PREWASH,
	FILLING,
	HEAT,
	SOAK,
	TUMBLING,
	TUMBLING_TIMESAVE,
	TUMBLINGSPECIAL,
	REHEAT,
	COOL_DOWN,
	TUMBLING_TX,//below 4 added for laundry add led logic
	HEATING,
	INITIAL_DRAIN,
	MAINWASH_HALFDRAIN,
	DRAIN,
	MAINWASH_INTERMIDIATE_SPIN, //15
	OPTION_EXTRA_RINSE_MAINWASH,
	OPTION_EXTRA_RINSE_PREWASH,
	OPTIONAL_DRAIN,
	OPTIONAL_SPIN_TUMBLE,
	OPTIONAL_SPIN, //20
	RINSE_PREWASH_FILL,
	RINSE_MAINWASH_FILL,
	RINSE_DRAIN,
	RINSE_INTERMIDIATE_SPIN,
	HOTRINSE,
	RINSE3_ADDITIVES,
	RINSE_HOLD,
	SPIN,
	ANTI_CREASE,
	DRAIN_TUMBLE,
	SPIN_TUMBLE,
	RINSE_TUMBLING,
	MAIN_TUMBLE,
	RINSE_HALFDRAIN,
	STEAM,
	STEAM_FILL,
	STEAM_CREASE,
	STEAM_DRAIN,
	OPTION_EXTRA_ADDITIVE_RINSE,
	STEAM_CP,
	IDLE_STAGE	
};
	    
//----------------------------------------------------------------------------------------------------------------------------------------------		
enum Rhythm
	{
            DUTY_IDLE,//0
	    DUTY_PMX,//1
            DUTY_C1, //2 
            DUTY_C2, //3 
            DUTY_C3, //4 
            DUTY_C4, //5 
            DUTY_C5, //6
            DUTY_C6, //7
            DUTY_C7, //8
            DUTY_C8, //9
            DUTY_C9, //10
            DUTY_C10,//11
            DUTY_C11,//12 
            DUTY_C12,//13 
            DUTY_C13,//14 
            DUTY_C14,//15 
            DUTY_C15,//16
            DUTY_C16,//17
            DUTY_C17,//18
            DUTY_E1, //19
            DUTY_E2, //20
            DUTY_D1, //21 
            DUTY_D2, //22
            DUTY_D3, //23
            DUTY_D4, //24
            DUTY_D5, //25
            DUTY_D6, //26
            DUTY_D7, //27
            DUTY_D8, //28
            DUTY_D9, //29
            DUTY_D10,//30 
            DUTY_D11,//31
            DUTY_D12,//32 
            DUTY_D13,//33 
            DUTY_D14,//34
            DUTY_W1, //35
            DUTY_W2, //36
            DUTY_W3, //37
            DUTY_Q1, //38
            DUTY_Q2, //39
            DUTY_M1, //40
            DUTY_M2, //41
            DUTY_R1, //42
            DUTY_L1, //43
            DUTY_S1, //44
            DUTY_S2, //45
            DUTY_S3, //46
            DUTY_S4, //47
            DUTY_N1, //48
            DUTY_N2, //49
            DUTY_N3, //50
            DUTY_N4, //51
            DUTY_N5, //52
            DUTY_N6, //53
            DUTY_N7, //54
            DUTY_N8, //55
            DUTY_N9, //56
            DUTY_N10,//57 
            DUTY_N11,//58 
            DUTY_N12,//59 
            DUTY_N13,//60
            DUTY_C21,//61
            DUTY_C22,//62 
            DUTY_C23,//63 
            DUTY_C24,//64 
            DUTY_C25,//65 
            DUTY_C26,//66
            DUTY_C27,//67 
            DUTY_C28,//68 
            DUTY_C30,//69 
            DUTY_C31,//70 
            DUTY_C32,//71
            DUTY_C33,//72 
            DUTY_C34,//73 
            DUTY_C35,//74
	    DUTY_C36,//75
            DUTY_HALT1,//76
            DUTY_HALT2, //77
            DUTY_MC2, //78
            DUTY_MC3, //79
            DUTY_MC7,//80
	    DUTY_RWOOL,//81
	    DUTY_CWOOL, //82
	    DUTY_C1A,//83
	    DUTY_C2A,//84
	    DUTY_C3A,//85
	    DUTY_C5A,//86
	    DUTY_C7A,  //87
	    DUTY_SOAK, // 88
	    
	    DUTY_EMI, // 89 //For testing
	    DUTY_EMC, // 90 // Remove after testing
	    DUTY_TESTMODE, //91
	    DUTY_FOAMSENSE, //92
	    DUTY_ST1, //93
	    DUTY_ST2, //94
	    DUTY_ST3, //95
	    DUTY_C40, //96
	    DUTY_C41, //97
	    
	    DUTY_X1, //98
            DUTY_X2,//99
            DUTY_X3,//100
            DUTY_X4, //101
            DUTY_X5,//102
            DUTY_X6,//103
            DUTY_X7,//104
            DUTY_X8,//105
            DUTY_X8_1,//106
            DUTY_X9, //107
	    
            SPECIAL_MOTION_MAX1,//108
	    SPIN_PROFILE_IDLE,//109
	    SPIN_PROFILE_FS2,//110
	    SPIN_PROFILE_FS3,//111
	    SPIN_PROFILE_FS4,//112
	    SPIN_PROFILE_FS5,//113
	    SPIN_PROFILE_FS6,//114
	    SPIN_PROFILE_FS7,//115
	    SPIN_PROFILE_FS8,//116
	    SPIN_PROFILE_FS9,//117
	    SPIN_PROFILE_FS10,//118
	    SPIN_PROFILE_FS11,//119
	    SPIN_PROFILE_FS12,//120
	    SPIN_PROFILE_FS16,//121
	    SPIN_PROFILE_IS1,//122
	    SPIN_PROFILE_IS2,//123
	    SPIN_PROFILE_IS3,//124
	    SPIN_PROFILE_IS4,//125
	    SPIN_PROFILE_IS5,//126
	    SPIN_PROFILE_IS6,//127
	    SPIN_PROFILE_IS7,//128
	    SPIN_PROFILE_IS13,//129
            SPIN_PROFILE_HALT, //130
	    SPIN_PROFILE_LS,//131
	    
	    //addition for spec rev 14 onwards
	    SPIN_PROFILE_IS1_BEE, //132
	    DUTY_SD,//133//new
	    DUTY_M3,//134
	    DUTY_SW,//135
	    DUTY_SW1,//136
	    DUTY_SL1,//137
	    SPIN_PROFILE_ISA,//138
	    SPIN_PROFILE_ISL,//139
	    DUTY_STL, //140
	    DUTY_SN2, //141
	    DUTY_SN3, //142
	    SPIN_PROFILE_FS1,//143,
	    TESTMODE_DUTY,//144
	    DUTY_PW1, //145

	};

//----------------------------------------------------------------------------------------------------------------------------------------------
enum Duration { IDLE=0, 
		ONE_MINUTES=1,
		TWO_MINUTES=2, 
		THREE_MINUTES=3,
		FOUR_MINUTES=4,
		FIVE_MINUTES=5,
		SIX_MINUTES=6,
		SEVEN_MINUTES=7,
		EIGHT_MINUTES=8,
		NINE_MINUTES=9,
		TEN_MINUTES=10,
		ELEVEN_MINUTES=11,
		TWELVE_MINUTES=12,
		THIRTEEN_MINUTES=13,
		FOURTEEN_MINUTES=14,
		FIFTEEN_MINUTES=15,
		SIXTEEN_MINUTES=16,
		SEVENTEEN_MINUTES =17,
		EIGHTEEN_MINUTES=18,
		NINETEEN_MINUTES =19,
		TWENTY_MINUTES=20,
		TWENTY_ONE_MINUTES= 21,
		TWENTY_TWO_MINUTES=22,
		TWENTY_THREE_MINUTES=23,
		TWENTY_FOUR_MINUTES=24, 
		TWENTY_FIVE_MINUTES=25,
		TWENTY_SEVEN_MINUTES=27,
		TWENTY_EIGHT_MINUTES=28,
		TWENTY_NINE_MINUTES=29,
		THIRTY_MINUTES=30,
		THIRTY_ONE_MINUTES =31,
		THIRTY_TWO_MINUTES =32,
		THIRTY_THREE_MINUTES =33,
	        THIRTY_FIVE_MINUTES=35,
		THIRTY_SIX_MINUTES=36,
		THIRTY_SEVEN_MINUTES=37,
		THIRTY_EIGHT_MINUTES=38,
		THIRTY_NINE_MINUTES=39,
		FOURTY_MINUTES=40,
		FOURTY_TWO_MINUTES=42,
		FOURTY_THREE_MINUTES=43,
		FOURTY_FOUR_MINUTES=44,
		FOURTY_FIVE_MINUTES = 45,
		FOURTY_SEVEN_MINUTES=47,
		FOURTY_EIGHT_MINUTES=48,
		FOURTY_NINE_MINUTES=49,
		FIFTY_MINUTES=50,
		FIFTY_TWO_MINUTES=52,
		FIFTY_FOUR_MINUTES=54,
		FIFTY_FIVE_MINUTES=55,
		SIXTY_MINUTES=60,
		SIXTY_TWO_MINUTES=62,
		SIXTY_FIVE_MINUTES=65,
		SIXTY_SIX_MINUTES=66,
		SEVENTY_MINUTES=70,
		SEVENTY_TWO_MINUTES=72,
		SEVENTY_FIVE_MINUTES=75,
		EIGHTY_MINUTES = 80,
		NINETY_MINUTES=90,
		ONE_HUNDRED_MINUTES=100
};
//----------------------------------------------------------------------------------------------------------------------------------------------
enum Errors{ERROR_FREE, DOOR_NOT_CLOSED, NO_WATER_SUPPLY, DRAIN_PUMP_NOT_WORKING, HIGH_UNBALANCE, HIGHLOWERROR, TACHO_ERROR, TRIAC_SHORT, HEATER_OPEN, NTC_OPEN, OVER_HEAT,PRESSURE_SWITCH_ERROR, FAULTY_MAGNETIC_VALVE};
//----------------------------------------------------------------------------------------------------------------------------------------------
enum FillStates{STATE_FILL, STATE_LEVEL_REACHED, STATE_LOW_PRESSURE, STATE_NO_WATER, STATE_RESET_FILLING, STATE_FILL_FINISHED, STATE_DESIRED};
//----------------------------------------------------------------------------------------------------------------------------------------------
enum DrainStates {STATE_DRAIN, STATE_EXTRA_DRAIN_THREE_MINUTES, STATE_DRAIN_IDLE, STATE_EXTRA_DRAIN_FOUR_MINUTES, STAE_ERROR, STATE_DRAIN_FINISHED};
//----------------------------------------------------------------------------------------------------------------------------------------------
enum HeatStates{STATE_HEAT, STATE_EXTRA_HEAT_TEN_MINUTES, STATE_RESET_INTIALIZATION, STATE_HEAT_RESET_FILLING, STATE_HEAT_FINISHED};
//----------------------------------------------------------------------------------------------------------------------------------------------
enum PreviousStates{STATE_TUMBLE,STATE_TUMBLE_FILL,STATE_TUMBLE_COMPLETED,STATE_TUMBLEHEAT};
//----------------------------------------------------------------------------------------------------------------------------------------------
enum CoolDownStates{STATE_COOL,STATE_COOL_FINISHED,STATE_COOL_RESET_FILL};
//----------------------------------------------------------------------------------------------------------------------------------------------
//enum SpecialHeatStates{SPECIALSTATE_HEAT, STATE_EXTRA_HEAT_TEN_MINUTES, STATE_RESET_INTIALIZATION, STATE_HEAT_RESET_FILLING, STATE_HEAT_FINISHED};
//----------------------------------------------------------------------------------------------------------------------------------------------
enum TumblingStates{CLOCKWISE_ON,CLOCKWISE_OFF,ANTICLOCKWISE_ON,ANTICLOCKWISE_OFF};
//----------------------------------------------------------------------------------------------------------------------------------------------
enum LowPressureStates{LOW_INTIALISE,TWO_MINUTES_ON,EIGHT_MINUTES_ON,TEN_MINUTES_MOTOR_STOP,TWENTY_MINUTES_VALVE_OFF,TWENTY_MINUTES_VALVE_ON};
//----------------------------------------------------------------------------------------------------------------------------------------------
enum NoWaterStates{FIVE_MINUTES_VALVE_ON,FIVE_MINUTES_DRAIN_ON,FIVE_MINUTES_IDLE,FIVE_MINUTES_VALAVE_ON_AGAIN};
//----------------------------------------------------------------------------------------------------------------------------------------------
enum MachineStates{STARTUP,PAUSE_PRESSED,START_PRESSED,IDLE_START,PROGRAM_END,PROGRAM_RESET,TESTMODE,SNOOZEMODE,HIGHLOWVOLTAGE,WATERERROR,PRESSUREERROR,HEATERROR,MOTORFAILURE,FCTMODE,DEBUG_MODE,POWEROFF,RESTARTPOWER,COMMUNICATIONERROR};
//----------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------
enum EncoderStates{ENCODEREVENTDETECTED,CLKWISEDETECTION,ANTICLKWISEDETECTION,NOEVENTDETECTED};
//----------------------------------------------------------------------------------------------------------------------------------------------
enum SpecialFunctionStates{step1,step2,step3,step4,step5,step6,step7,step8,step9,step10,step11,step12};
//----------------------------------------------------------------------------------------------------------------------------------------------
enum MOTOR_MOTION{
	TUMBLE_MOTION = 1,
	SPECIAL_MOTION = 2,
	SPIN_MOTION =3,
};
//********************************************************************************//
//-------------------------------------------------------------
#define MAX_TEST_MODE_CYCLE	999
#define MAX_CYCLE		9999
#define TRUE 			1
#define FALSE 			0
#define TUMBLING_REQUIRED 	1
#define TUMBLING_NOT_REQUIRED 	0
#define NO_DUTY_CYCLE 		0
#define COMPLETED		0
#define ZERO_VALUE 		0
#define ZERO			0
#define NOT_COMPLETED 		0

//---------------------------------------------------------------
#define ON_PAUSE_DOOR_OPEN_TEMPERATURE 	50
#define TEMPEATURE_NOT_ATTAINED 	3
#define EMPTYLEVEL_NOT_ATTAINED 	4
#define ERROR_DRAINPUMP 	3
#define SELECTED 		1
#define NOT_SELECTED 		0
#define NO_ERROR 		0
#define FATAL_DRAIN 		3
#define SAMPLES_EIGHT 		8
#define STAND_BY 		0
#define OVER_HEAT_TEMPERATURE 	92

#define MemoryLength 		145//145//144//143//142//141//140

//--------------------------------------------------------------------------
#define NO_INPUT 0
#define CLKWISE 1
#define ACLKWISE 2

#define UNBALNCE_VALUE1 21
#define UNBALNCE_VALUE2 32
#define UNBALNCE_VALUE3 42

//****************************************
enum CircPump{
	CP_OFF,
	CP_ON,
	CP10MinON_2MinOFF,
	CP5MinOFF_10MinON,
	CP5MinOFF_5MinON,
	CP_1MinON_1MinOFF,
	CP_5MinON,
	CP_NONE,
	CP_LAST_Stage
};

typedef struct
{
	enum Stage eWork;
	enum Duration eDuration;
	enum Rhythm eRhythm;
	enum CircPump eCircPump_Exec;
}__far const Operation;
//*************************************
//****Buzzeer****//
typedef struct
{
	uint16_t WaveWidth;
	uint8_t ToneFrequency;
}__far const Buzzer;
//Buzzer 
//typedef enum{NOFREQUENCY,FREQUENCY1240,FREQUENCY1570,FREQUENCY2050}BuzzerFrequency;
typedef enum{NOFREQUENCY,FREQUENCY1240,FREQUENCY1570,FREQUENCY2050,FREQUENCY3050,FREQUENCY4050}BuzzerFrequency;

#define WIDTH_100_MILLISECONDS 100
#define WIDTH_200_MILLISECONDS 200
//typedef enum {TONEINTIALIZE,TONECHARGE,TONEDISCHARGE}ToneGenerationState;
typedef enum {TONEINTIALIZE,TONECHARGE1,TONECHARGE2,TONECHARGE3,TONECHARGE4,TONEDISCHARGE}ToneGenerationState;
typedef enum{NOTONE,STARTUPTONE,STARTPRESSTONE,PAUSETONE,ERRORTONE,ENDTONE,INAPPLICABLE,KEYPRESSTONE,KEYPRESSTONE2,ENCODERTONE};
typedef enum{PAUSEPRESSED,STARTPRESSED};
//***********************************************
//***********Supply Voltage Value*******//
#define SUPPLY170 169
#define SUPPLY160 159 //new
#define SUPPLY150 147 //65
#define SUPPLY270 274 //138
#define SUPPLY260 262 //130
#define SUPPLY120 120

/*
#define SUPPLY170 70//85 old
#define SUPPLY160 65//81
#define SUPPLY150 61//75  //65
#define SUPPLY270 111//136
#define SUPPLY260 105//133
*/



#define _20MS_TO_1MS_FACTOR	50
#define LAST_30S_DRAIN_COUNT	30

enum TimerStates{
	SET_TIME,
	START_TIMER,
	PAUSE_TIMER,
	RESUME_TIMER,
	CHECK_TIMER
	//LAST_30S_DRAIN_CHECK_TIMER
};
enum TimerNames{
	AI200Sec_TIMER,
	AI_1Sec_Interveval_TMER,
	EXTRA_DRAIN_1MIN_TIMER,
	AI_DATA_RecieveTimeout_Status,
	NO_USER_TIMER
};

typedef struct
{
	const unsigned char name;	//Name defined in macro
	uint32_t total_ui32;		//Time in units of 20ms
	uint32_t counter_ui32;		//Initialise to 0
	uint8_t countdownTimer_b	:1;	//Initialise to FALSE
} TimerObjectType;
typedef struct
{
	uint32_t CPON_ui16;
	uint32_t CPOFF_ui16;
}st_CPObject;

//*************************************
typedef struct
{
	uint8_t DoorError:1;  
	uint8_t NoWaterError:1;
	uint8_t DrainPump:1;
	uint8_t HighUnbalance:1;
	uint8_t VoltageError:1;
	uint8_t MotorError:1;
	uint8_t HeaterError:1;
	uint8_t PressureError:1;
	uint8_t WifiComnError:1;//Jayesh
	
}sErrors;
typedef union 
{
//uint8_t EReg;
uint16_t EReg; //Jayesh
sErrors EBits;
}SystemError;
extern SystemError MachineErrors;
//******************************************
typedef enum TestModeStates{SF01,SF02,SF03,SF04,SF05,SF06,SF07,SF08};

enum FoamSensingStates{FOAM_SENSING_INITIAL,FOAM_SENSING_WATER_FILLING,FOAM_SENSING_IDLE,FOAM_SENSING_TUMBLE,FOAM_SENSING_SPIN_CANCEL,CONTINUE_SPIN_OPERATION,STARTSPIN,DECISIONTAKING,FOAM_SENSING_HALT,FOAM_DELAY};

enum Loadrange{LOWER,MIDDLE,UPPER};

#define STEP_COUNT 3//2
#define COUNT_SIX 6
#define COUNT_EIGHT 8

//----------------------------------------------------------/
/* End user code. Do not edit comment generated here */
#endif
