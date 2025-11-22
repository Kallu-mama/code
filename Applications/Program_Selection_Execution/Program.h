#ifndef PROGRAMSELECTION_H_
#define PROGRAMSELECTION_H_

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_userdefine.h"
#include "WashOperations_Global.h"
//***************************************************************************************************************************************//
//***************************************** Spin Dry Program **********************************************************************//
/*
Operation DrainSpin[DRAIN_SPIN_LENGTH]= {{DELAYSTART,IDLE,DUTY_IDLE},{DRAIN,ONE_MINUTES,DUTY_IDLE}, 
					{SPIN_TUMBLE,TWO_MINUTES,DUTY_C25},{SPIN,TEN_MINUTES,SPIN_PROFILE_FS2},
					{ANTI_CREASE,FIVE_MINUTES,DUTY_C6}}; *///1-5			 			


//***************************************************************************************************************************************//
//***************************************** Rinse Endurance Program **********************************************************************//

Operation AdditiveEndurance[ADDITIVES_ENDURANCE_LENGTH]= {
		{RINSE_DRAIN,			ONE_MINUTES,		NO,			CP_NONE},
		{RINSE3_ADDITIVES,		SIX_MINUTES,		DUTY_C2,		CP_NONE}, //7-9
		{RINSE_DRAIN,			ONE_MINUTES,		NO,			CP_NONE},
		{SPIN,				TEN_MINUTES,		SPIN_PROFILE_FS2,	CP_NONE},
		//STEAM
		{STEAM_FILL,			ONE_MINUTES,		DUTY_IDLE,		CP_NONE},
		{STEAM,				ELEVEN_MINUTES,		DUTY_ST1,		CP_NONE},
		{STEAM_CREASE,			TWO_MINUTES,		DUTY_ST1,		CP_NONE},
		{STEAM_DRAIN,			ONE_MINUTES,		DUTY_ST1,		CP_NONE}
};//11-14}; //6-12
			

//***************************************************************************************************************************************//
//***************************************** Cotton Eco 40 Program **********************************************************************//

Operation Eco_40[ECO_40_LENGTH] = {
		{DELAYSTART,			IDLE,			IDLE,				CP_NONE},
		{DRAIN,				ONE_MINUTES,		NO,				CP_NONE}, // 1-2

		//MAINWASH
		{FILLING,			ELEVEN_MINUTES,		DUTY_C36,			CP_NONE},
		{TUMBLING,			FIVE_MINUTES,		DUTY_C2,			CP_NONE},
		{SOAK,				IDLE,			DUTY_SOAK,			CP_NONE},
		{HEAT,				IDLE,			DUTY_C7,			CP_NONE}, //3-6//5 min
		{TUMBLING,			IDLE,			DUTY_C7,			CP_NONE},//40mins
		{TUMBLINGSPECIAL,		ONE_MINUTE,		DUTY_N9,			CP_NONE},
		{TUMBLING,			FOURTY_EIGHT_MINUTES,	DUTY_C7,			CP_NONE},
		{TUMBLINGSPECIAL,		ONE_MINUTE,		DUTY_N9,			CP_NONE},//7-10
		{COOL_DOWN,			SIX_MINUTES,		DUTY_D1,			CP_NONE},
		{DRAIN,				ONE_MINUTES,		NO,				CP_NONE},
		//{MAINWASH_INTERMIDIATE_SPIN,	SIX_MINUTES,		SPIN_PROFILE_IS1,		CP_NONE},//11-13
		{MAINWASH_INTERMIDIATE_SPIN,	THIRTEEN_MINUTES,	SPIN_PROFILE_IS1_BEE,		CP_NONE},
		
		//OPTIONAL RINSE
		{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,		DUTY_C3,			CP_NONE},
		{OPTIONAL_DRAIN,		ONE_MINUTES,		NO,				CP_NONE},
		{OPTIONAL_SPIN,			SIX_MINUTES,		SPIN_PROFILE_IS1,		CP_NONE},//14-16
		
		//RINSE
		{RINSE_PREWASH_FILL,		FOUR_MINUTES,		DUTY_C3,			CP_NONE},
		{RINSE_DRAIN,			ONE_MINUTES,		NO,				CP_NONE},
		//{RINSE_INTERMIDIATE_SPIN,	SIX_MINUTES,		SPIN_PROFILE_IS1,		CP_NONE},//17-19
		{RINSE_INTERMIDIATE_SPIN,	THIRTEEN_MINUTES,	SPIN_PROFILE_IS1_BEE,		CP_NONE},
		{RINSE3_ADDITIVES,		FIVE_MINUTES,		DUTY_C3,			CP_NONE},
		{RINSE_HOLD,			IDLE,			NO,				CP_NONE},
		{RINSE_DRAIN,			ONE_MINUTES,		NO,				CP_NONE},//20-22
		
		//FINAL SPIN
		{SPIN,				TEN_MINUTES,		SPIN_PROFILE_FS2,		CP_NONE},//22
		{IDLE_STAGE,			IDLE,			IDLE,				CP_NONE}//23   //added for safety to display end properly if door gets opened during ramp down-- production issue
};
			

//***************************************************************************************************************************************//
//***************************************** Cotton Eco 60 Program **********************************************************************//

Operation Eco_60[ECO_60_LENGTH] = {
		{DELAYSTART,			IDLE,			IDLE,				CP_NONE},
		{DRAIN,				ONE_MINUTES,		NO,				CP_NONE}, // 1-2

		//MAINWASH
		{FILLING,			ELEVEN_MINUTES,		DUTY_C36,			CP_NONE},
		{TUMBLING,			FIVE_MINUTES,		DUTY_C2,			CP_NONE},
		{SOAK,				IDLE,			DUTY_SOAK,			CP_NONE},
		{HEAT,				THREE_MINUTES,		DUTY_C7,			CP_NONE}, //3-6
		{TUMBLING,			IDLE,			DUTY_C7,			CP_NONE},
		{TUMBLINGSPECIAL,		ONE_MINUTE,		DUTY_N9,			CP_NONE},
		{TUMBLING,			THIRTY_SIX_MINUTES,	DUTY_C7,			CP_NONE},
		{TUMBLINGSPECIAL,		ONE_MINUTE,		DUTY_N9,			CP_NONE},//7-10
		{COOL_DOWN,			SIX_MINUTES,		DUTY_D1,			CP_NONE},
		{DRAIN,				ONE_MINUTES,		NO,				CP_NONE},
		{MAINWASH_INTERMIDIATE_SPIN,	THIRTEEN_MINUTES,	SPIN_PROFILE_IS1_BEE,		CP_NONE},//11-13
		
		//OPTIONAL RINSE
		{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,		DUTY_C3,			CP_NONE},
		{OPTIONAL_DRAIN,		ONE_MINUTES,		NO,				CP_NONE},
		{OPTIONAL_SPIN,			SIX_MINUTES,		SPIN_PROFILE_IS1,		CP_NONE},//14-16
		
		//RINSE
		{RINSE_PREWASH_FILL,		FOUR_MINUTES,		DUTY_C3,			CP_NONE},
		{RINSE_DRAIN,			ONE_MINUTES,		NO,				CP_NONE},
		{RINSE_INTERMIDIATE_SPIN,	THIRTEEN_MINUTES,	SPIN_PROFILE_IS1_BEE,		CP_NONE},//17-19
		{RINSE3_ADDITIVES,		FIVE_MINUTES,		DUTY_C3,			CP_NONE},
		{RINSE_HOLD,			IDLE,			NO,				CP_NONE},
		{RINSE_DRAIN,			ONE_MINUTES,		NO,				CP_NONE},//20-22
		
		//FINAL SPIN
		{SPIN,				TEN_MINUTES,		SPIN_PROFILE_FS2,		CP_NONE},
		{IDLE_STAGE,			IDLE,			IDLE,				CP_NONE}  //added for safety to display end properly if door gets opened during ramp down-- production issue
};//23			


//***************************************************************************************************************************************//
//***************************************** Additive Rinse Program **********************************************************************//

Operation AdditiveRinse[ADDITIVES_RINSE_LENGTH]= {
		{DELAYSTART,			IDLE,			IDLE,				CP_NONE},	//0
		{DRAIN,				ONE_MINUTES,		NO,				CP_NONE},	//1  

		/*----------------------------------------OPTIONAL RINSE--------------------------------------*/
		{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,		DUTY_C25,			CP_NONE},	//2
		{OPTIONAL_DRAIN,		ONE_MINUTES,		NO,				CP_NONE},	//3 
		{OPTIONAL_SPIN,			SIX_MINUTES,		SPIN_PROFILE_IS1,		CP_NONE},	//4
		{OPTION_EXTRA_ADDITIVE_RINSE,	FOUR_MINUTES,		DUTY_C25,			CP_NONE},	//5
		{RINSE_HOLD,			IDLE,			NO,				CP_NONE},	//6
		{DRAIN,				TWO_MINUTES,		NO,				CP_NONE},	//7
		
		/*----------------------------------------STEAM----------------------------------------------*/
		{STEAM_FILL,			ONE_MINUTES,		DUTY_IDLE,			CP_NONE},	//8
		{STEAM,				TWENTY_ONE_MINUTES,	DUTY_ST1,			CP_NONE},	//9
		{STEAM_CREASE,			TWO_MINUTES,		DUTY_ST1,			CP_NONE},	//10
		{STEAM_DRAIN,			ONE_MINUTES,		DUTY_ST1,			CP_NONE},	//11

		/*----------------------------------------SPIN----------------------------------------------*/
		{SPIN,				TEN_MINUTES,		SPIN_PROFILE_FS2,		CP_NONE},	//12
		{ANTI_CREASE,			FIVE_MINUTES,		DUTY_C17,			CP_NONE},	//13

};//11-14
			

//***************************************************************************************************************************************//
//***************************************** ExpressWash Program **********************************************************************//			

Operation ExpressWash[EXPS_LENGTH]= {
		{DELAYSTART,			IDLE,			IDLE,				CP_NONE},
		{DRAIN,				ONE_MINUTES,		NO,				CP_NONE}, //1-2

		//MAINWASH
		{FILLING,			ONE_MINUTES,		DUTY_C7A,			CP_NONE},
		{HEAT,				IDLE,			DUTY_C7A,			CP_NONE},
		{TUMBLING,			IDLE,			DUTY_C7A,			CP_NONE}, //3-5
		{COOL_DOWN,			SIX_MINUTES,		DUTY_D1,			CP_NONE},
		{DRAIN,				ONE_MINUTES,		NO,				CP_NONE}, //6-7
		
		//OPTIONAL RINSE
		{OPTION_EXTRA_RINSE_PREWASH,	TWO_MINUTES,		DUTY_C7A,			CP_NONE},
		{OPTIONAL_DRAIN,		ONE_MINUTES,		NO,				CP_NONE}, // 8-9
		 
		//RINSE 
		{RINSE_PREWASH_FILL,		TWO_MINUTES,		DUTY_C7A,			CP_NONE},
		{RINSE_DRAIN,			ONE_MINUTES,		NO,				CP_NONE}, //10-11
		{RINSE_MAINWASH_FILL,		TWO_MINUTES,		DUTY_C7A,			CP_NONE},
		{RINSE_DRAIN,			ONE_MINUTES,		NO,				CP_NONE},//11-12
		{RINSE3_ADDITIVES,		TWO_MINUTES,		DUTY_C7A,			CP_NONE},
		{RINSE_HOLD,			IDLE,			NO,				CP_NONE},
		{RINSE_DRAIN,			ONE_MINUTES,		NO,				CP_NONE}, //13-16
		
		//FINAL SPIN
		{SPIN,				IDLE,			IDLE,				CP_NONE},
		{IDLE_STAGE,			IDLE,			IDLE,				CP_NONE}   //added for safety to display end properly if door gets opened during ramp down-- production issue
};//17 	
			

//***************************************** ExpressWash55 Program **********************************************************************//			

Operation ExpressWash_30[EXPS_30_LENGTH]= {
	/*0*/	{DELAYSTART,			IDLE,				IDLE,			CP_NONE},	//0
	/*1*/	{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},	//1 

		//MAINWASH
	/*2*/	{FILLING,			FOUR_MINUTES,			DUTY_C30,		CP_ON},		//2 Heater should be on.
	/*3*/	{HEATING,			ONE_MINUTES,			DUTY_C23,		CP_ON},		//3
	/*4*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X1,		CP_ON},		//4 Heater should be on.
	/*5*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X6,		CP_ON},		//5 Heater should be on.
	/*6*/	{HEATING,			IDLE,				DUTY_C23,		CP_5MinON},	//6
	/*7*/	{TUMBLING,			ONE_MINUTES,			DUTY_C23,		CP_OFF},	//7 Last30 sec drain
	/*8*/	{COOL_DOWN,			SIX_MINUTES,			DUTY_D1,		CP_NONE},	//added in neo first as issue reported in silver: temp too high even after program end
	/*9*/	{MAINWASH_INTERMIDIATE_SPIN,	THREE_MINUTES,			SPIN_PROFILE_IS3,	CP_OFF},	//8
		
		//OPTIONAL RINSE
	/*10*/	{OPTION_EXTRA_RINSE_PREWASH,	THREE_MINUTES,			DUTY_C27,		CP_ON},		//9 Last30 sec drain
	/*11*/	{OPTIONAL_SPIN,			THREE_MINUTES,			SPIN_PROFILE_IS3,	CP_OFF},	//10
	/*12*/	{OPTION_EXTRA_RINSE_MAINWASH,	THREE_MINUTES,			DUTY_C27,		CP_ON},		//11 Last30 sec drain
	/*13*/	{OPTIONAL_SPIN,			THREE_MINUTES,			SPIN_PROFILE_IS3,	CP_OFF},	//12 
		
		//RINSE 
	/*14*/	{RINSE_PREWASH_FILL,		THREE_MINUTES,			DUTY_C27,		CP_ON},		//13 Last30 sec drain
	/*15*/	{RINSE_INTERMIDIATE_SPIN,	THREE_MINUTES,			SPIN_PROFILE_IS3,	CP_OFF},	//14
	/*16*/	{RINSE3_ADDITIVES,		THREE_MINUTES,			DUTY_C27,		CP_ON},		//15 Last30 sec drain
	/*17*/	{RINSE_HOLD,			IDLE,				NO,			CP_NONE},	//16
	/*18*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF},	//17
	
		//STEAM
	/*19*/	{STEAM_FILL,			ONE_MINUTES,			DUTY_IDLE,		CP_NONE},	//18
	/*20*/	{STEAM,				TWENTY_ONE_MINUTES,		DUTY_ST1,		CP_NONE},	//19
	/*21*/	{STEAM_CREASE,			TWO_MINUTES,			DUTY_ST1,		CP_NONE},	//20
	/*22*/	{STEAM_DRAIN,			ONE_MINUTES,			DUTY_ST1,		CP_NONE},	//21
		
		
		//FINAL SPIN
	/*23*/	{SPIN,				SIX_MINUTES,			SPIN_PROFILE_FS9,	CP_OFF},	//22
	/*24*/	{ANTI_CREASE,			TWO_MINUTES,			DUTY_C17,		CP_OFF}		//23
}; 	
			

				  
//***************************************************************************************************************************************//
//***************************************** CradleWash Program **********************************************************************//
Operation CradleWash[CRDL_LENGTH]= {
		{DELAYSTART,				IDLE,				IDLE,			CP_NONE}, 
		{DRAIN,					ONE_MINUTES,			NO,			CP_NONE}, //1-2

		//MAINWASH
		{FILLING,				FOUR_MINUTES,			DUTY_S1,		CP_NONE},
		{HEAT,					IDLE,				DUTY_S1,		CP_NONE},
		{TUMBLING,		/*THREE_MINUTES*/SIX_MINUTES,			DUTY_S1,		CP_NONE},
		{TUMBLING,				IDLE,				DUTY_S1,		CP_NONE}, //3-6
		{COOL_DOWN,				SIX_MINUTES,			DUTY_D1,		CP_NONE},
		{DRAIN,					TWO_MINUTES,			NO,			CP_NONE},//7-8
		
		//OPTIONAL RINSE
		{OPTION_EXTRA_RINSE_PREWASH,		SIX_MINUTES,			DUTY_S1,		CP_NONE}, 
		{OPTIONAL_DRAIN,			TWO_MINUTES,			NO,			CP_NONE}, //9-10
		
		//RINSE
		{RINSE_PREWASH_FILL,			SIX_MINUTES,			DUTY_S1,		CP_NONE}, 
		{RINSE_DRAIN,				TWO_MINUTES,			NO,			CP_NONE},//11-12
		{RINSE_PREWASH_FILL,			SIX_MINUTES,			DUTY_S1,		CP_NONE},
		{RINSE_DRAIN,				TWO_MINUTES,			NO,			CP_NONE}, //12-13
		{RINSE3_ADDITIVES,			SIX_MINUTES,			DUTY_S1,		CP_NONE},
		{RINSE_HOLD,				IDLE,				NO,			CP_NONE}, 
		{RINSE_DRAIN,				TWO_MINUTES,			NO,			CP_NONE}, //14-16
		
		//FINAL SPIN
		{SPIN,					FOUR_MINUTES,			SPIN_PROFILE_FS3,	CP_NONE},
		{IDLE_STAGE,				IDLE,				IDLE,			CP_NONE}   //added for safety to display end properly if door gets opened during ramp down-- production issue
};//17 				  
				 

			
//****************************************************************************************************************************************//
//************************************************ UNIFORM Program **********************************************************************//		

Operation uniform[UNI_LENGTH] = {
		{DELAYSTART,			IDLE,				DUTY_C1,		CP_NONE},	//0
		{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},	//1

		//OPTIONAL PREWASH
		{OPTIONAL_PREWASH,		ELEVEN_MINUTES,			DUTY_C23,		CP_NONE},	//2
		{DRAIN_PREWASH,			ONE_MINUTES,			NO,			CP_NONE},	//3
		{PREWASH_SPIN,			FOUR_MINUTES,			SPIN_PROFILE_IS2,	CP_NONE},	//4
		
		//MAINWASH
		{FILLING,			IDLE,				IDLE,			CP_NONE},	//5
		{TUMBLING,			FIVE_MINUTES,			DUTY_C23,		CP_NONE},	//6
		{SOAK,				IDLE,				DUTY_SOAK,		CP_NONE},	//7
		{HEAT,				IDLE,				DUTY_C6,		CP_NONE},	//8
		{REHEAT,			IDLE,				DUTY_C28,		CP_NONE},	//9
		{TUMBLING,			IDLE,				DUTY_C22,		CP_NONE},	//10
		{TUMBLING,			TEN_MINUTES,			DUTY_C22,		CP_NONE},	//11
		{COOL_DOWN,			SIX_MINUTES,			DUTY_D1,		CP_NONE},	//12
		{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},	//13
		{MAINWASH_INTERMIDIATE_SPIN,	THREE_MINUTES,			SPIN_PROFILE_IS6,	CP_NONE},	//14
		
		
		//OPTIONAL RINSE
		{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_E2,		CP_NONE},	//15
		{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_NONE},	//16
		{OPTIONAL_SPIN,			THREE_MINUTES,			SPIN_PROFILE_IS6,	CP_NONE},	//17
		{OPTION_EXTRA_RINSE_MAINWASH,	FOUR_MINUTES,			DUTY_E2,		CP_NONE},	//18
		{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_NONE},	//19
		{OPTIONAL_SPIN,			THREE_MINUTES,			SPIN_PROFILE_IS6,	CP_NONE},	//20
		
		//RINSE
		{RINSE_PREWASH_FILL,		FOUR_MINUTES,			DUTY_E2,		CP_NONE},	//21
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},	//22
		{RINSE_INTERMIDIATE_SPIN,	THREE_MINUTES,			SPIN_PROFILE_IS6,	CP_NONE},	//23
		{RINSE3_ADDITIVES,		FIVE_MINUTES,			DUTY_E2,		CP_NONE},	//24
		{RINSE_HOLD,			IDLE,				NO,			CP_NONE},	//25
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},	//26
		
		//STEAM
		{STEAM_FILL,			ONE_MINUTES,			DUTY_IDLE,		CP_NONE},	//27
		{STEAM,				TWENTY_ONE_MINUTES,		DUTY_ST1,		CP_NONE},	//28
		{STEAM_CREASE,			TWO_MINUTES,			DUTY_ST1,		CP_NONE},	//29
		{STEAM_DRAIN,			ONE_MINUTES,			DUTY_ST1,		CP_NONE},	//30
		
		//FINAL SPIN
		{SPIN,				SEVEN_MINUTES,			SPIN_PROFILE_FS6,	CP_NONE},	//31
		{ANTI_CREASE,			THREE_MINUTES,			DUTY_C17,		CP_NONE},	//32
		
		
};//30-33			
				
				

				
//**************************************************************************************************************************************//
//******************************************************* Steam Fresh Program **********************************************************//				

Operation SteamFresh[STEAM_FRESH_LENGTH]= {
		{DRAIN,				ONE_MINUTES,			DUTY_IDLE,		CP_NONE},
		{STEAM_FILL,			ONE_MINUTES,			DUTY_IDLE,		CP_NONE}, 
		{STEAM,				TWENTY_ONE_MINUTES,		DUTY_ST1,		CP_NONE},
		{STEAM_CREASE,			TWO_MINUTES,			DUTY_ST1,		CP_NONE},
		{STEAM_DRAIN,			ONE_MINUTES,			DUTY_ST1,		CP_NONE}
};//1-5
					

					
//*************************************************************************************************************************************//
//************************************************ Steam Wash Program ****************************************************************//					

Operation steamwash[STEAM_LENGTH] = {
		/*0*/	{DELAYSTART,			IDLE,				NO,			CP_NONE},
		/*1*/	{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},//1-2

			//STEAM
		/*2*/	{STEAM_FILL,			ONE_MINUTES,			DUTY_IDLE,		CP_NONE},
		//	{STEAM_CP,			ONE_MINUTES,			IDLE,			CP_NONE},
		/*3*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_NONE},
		/*4*/	{STEAM,				FOUR_MINUTES,			DUTY_ST1,		CP_NONE},//3-4
		 
			//MAINWASH
		/*5*/	{FILLING,			FIVE_MINUTES,			DUTY_ST3,		CP_NONE},
		/*6*/	{COOL_DOWN,			SIX_MINUTES,			DUTY_D1,		CP_NONE},
		/*7*/	{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},
		/*8*/	{MAINWASH_INTERMIDIATE_SPIN,	THREE_MINUTES,			SPIN_PROFILE_IS7,	CP_NONE},//5-7
		
			//OPTIONAL RINSE
		/*9*/	{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_ST3,		CP_NONE},
		/*10*/	{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_NONE},
		/*11*/	{OPTIONAL_SPIN,			THREE_MINUTES,			SPIN_PROFILE_IS7,	CP_NONE},//8-10
		
			//RINSE + FINAL SPIN
		/*12*/	{RINSE_PREWASH_FILL,		THREE_MINUTES,			DUTY_ST3,		CP_NONE},
		/*13*/	{RINSE_HOLD,			IDLE,				NO,			CP_NONE},
		/*14*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},//11-13
		
			//FINAL STEAM
		/*15*/	{STEAM_FILL,			ONE_MINUTES,			DUTY_IDLE,		CP_NONE},
		/*16*/	{STEAM,				SEVEN_MINUTES,			DUTY_ST1,		CP_NONE},
		/*17*/	{SPIN_TUMBLE,			ONE_MINUTES,			DUTY_ST1,		CP_NONE},
		
			//SPIN
		/*18*/	{SPIN,				FOUR_MINUTES,			SPIN_PROFILE_FS4,	CP_NONE},
		/*19*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_NONE}//19   //added for safety to display end properly if door gets opened during ramp down-- production issue
};//14-16
				

				
//*****************************************************************************************************************************************//
//**************************************************** INNER Wear Program *****************************************************************//				

Operation innerwash[INNER_LENGTH] = {
		{DELAYSTART,			IDLE,				NO,			CP_NONE},	//0
		{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},	//1

		/*----------------------------------------------MAINWASH---------------------------------------*/
		{FILLING,			SIX_MINUTES,			DUTY_E1,		CP_NONE},	//2
		{HEAT,				IDLE,				DUTY_L1,		CP_NONE},	//3
		{TUMBLING,			IDLE,				DUTY_L1,		CP_NONE},	//4
		{TUMBLING,			SEVEN_MINUTES,			DUTY_L1,		CP_NONE},	//5
		{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},	//6
		{DRAIN_TUMBLE,			ONE_MINUTES,			DUTY_EMC,		CP_NONE},	//7
		
		/*--------------------------------------------OPTIONAL RINSE------------------------------------*/
		{OPTION_EXTRA_RINSE_PREWASH,	FIVE_MINUTES,			DUTY_L1,		CP_NONE},	//8
		{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_NONE},	//9
		{OPTIONAL_SPIN_TUMBLE,		ONE_MINUTES,			DUTY_EMC,		CP_NONE},	//10
		
		/*------------------------------------------------RINSE_1---------------------------------------*/
		{RINSE_PREWASH_FILL,		FIVE_MINUTES,			DUTY_L1,		CP_NONE},	//11
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},	//12
		{DRAIN_TUMBLE,			ONE_MINUTES,			DUTY_EMC,		CP_NONE},	//13
		
		/*------------------------------------------------RINSE_2---------------------------------------*/
		{RINSE3_ADDITIVES,		FIVE_MINUTES,			DUTY_L1,		CP_NONE},	//14
		{RINSE_HOLD,			IDLE,				NO,			CP_NONE},	//15
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},	//16

		/*-------------------------------------------------STEAM----------------------------------------*/
		{STEAM_FILL,			ONE_MINUTES,			DUTY_IDLE,		CP_NONE},	//17
		{STEAM,				TWENTY_ONE_MINUTES,		DUTY_ST1,		CP_NONE},	//18
		{STEAM_CREASE,			TWO_MINUTES,			DUTY_ST1,		CP_NONE},	//19
		{STEAM_DRAIN,			ONE_MINUTES,			DUTY_ST1,		CP_NONE},	//20

		/*----------------------------------------------FINAL SPIN-------------------------------------*/
		{SPIN,				FOUR_MINUTES,			SPIN_PROFILE_FS3,	CP_NONE},	//21
		{ANTI_CREASE,			TWO_MINUTES,			DUTY_C17,		CP_NONE},	//22
		
};//20-23				

				
				
//****************************************************************************************************************************************//
//********************************************************* DARK WASH Program ***********************************************************//				

Operation darkwash[WASH_LENGTH] = {
		{DELAYSTART,			IDLE,				NO,			CP_NONE},
		{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},//1-2

		//OPTIONAL PREWASH
		{OPTIONAL_PREWASH,		SIX_MINUTES,			DUTY_E1,		CP_NONE},
		{PRE_HEAT,			FIVE_MINUTES,			DUTY_E1,		CP_NONE}, //3-4
		{DRAIN_PREWASH,			ONE_MINUTES,			NO,			CP_NONE},
		{DRAIN_PREWASH,			ONE_MINUTES,			DUTY_EMC,		CP_NONE},//5-6
		
		//MAINWASH
		{FILLING,			FIVE_MINUTES,			DUTY_M1,		CP_NONE},
		{HEAT,				IDLE,				DUTY_M1,		CP_NONE},
		{TUMBLING,			IDLE,				DUTY_M1,		CP_NONE}, //7-9
		{TUMBLING,			THREE_MINUTES,			DUTY_E1,		CP_NONE},
		{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},
		{DRAIN_TUMBLE,			TWO_MINUTES,			DUTY_EMC,		CP_NONE},//10-12
		
		//OPTIONAL RINSE
		{OPTION_EXTRA_RINSE_PREWASH,	FIVE_MINUTES,			DUTY_M1,		CP_NONE},
		{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_NONE},
		{OPTIONAL_SPIN_TUMBLE,		TWO_MINUTES,			DUTY_EMC,		CP_NONE},//13-15
		{OPTION_EXTRA_RINSE_MAINWASH,	FIVE_MINUTES,			DUTY_M1,		CP_NONE},
		{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_NONE},
		{OPTIONAL_SPIN_TUMBLE,		TWO_MINUTES,			DUTY_EMC,		CP_NONE},//16-18
		
		//RINSE
		{RINSE_PREWASH_FILL,		FIVE_MINUTES,			DUTY_M1,		CP_NONE},
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},
		{DRAIN_TUMBLE,			TWO_MINUTES,			DUTY_EMC,		CP_NONE},//19-21
		{RINSE3_ADDITIVES,		SIX_MINUTES,			DUTY_M1,		CP_NONE},
		{RINSE_HOLD,			IDLE,				NO,			CP_NONE},
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE}, //22-24
		
		//FINAL SPIN
		{SPIN,				SEVEN_MINUTES,			SPIN_PROFILE_FS6,	CP_NONE},
		{ANTI_CREASE,			TWO_MINUTES,			DUTY_C17,		CP_NONE}

};//25-26
				
				
//*****************************************************************************************************************************************************//
//********************************************************** CURTAINS Program *************************************************************************//				

Operation curtains[CUR_LENGTH] = {
		{DELAYSTART,			IDLE,				DUTY_C1,		CP_NONE},
		{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},//1-2

		//OPTIONAL PREWASH
		{OPTIONAL_PREWASH,		SIXTEEN_MINUTES,		DUTY_D2,		CP_NONE},
		{DRAIN_PREWASH,			TWO_MINUTES,			NO,			CP_NONE},//3-4
		
		//MAINWASH
		{FILLING,			SEVEN_MINUTES,			DUTY_C8,		CP_NONE},
		{SOAK,				TEN_MINUTES,			DUTY_SOAK,		CP_NONE},
		{HEAT,				IDLE,				DUTY_C8,		CP_NONE},//5-7
		{TUMBLING,			IDLE,				DUTY_C8,		CP_NONE},
		{TUMBLING,			THREE_MINUTES,			DUTY_D2,		CP_NONE},
		{COOL_DOWN,			SIX_MINUTES,			DUTY_D1,		CP_NONE},//8-10
		{DRAIN,				TWO_MINUTES,			NO,			CP_NONE},//11
		
		//OPTIONAL RINSE
		{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_C8,		CP_NONE},
		{OPTIONAL_DRAIN,		TWO_MINUTES,			NO,			CP_NONE},//12-13
		{OPTION_EXTRA_RINSE_MAINWASH,	FOUR_MINUTES,			DUTY_C8,		CP_NONE},
		{OPTIONAL_DRAIN,		TWO_MINUTES,			NO,			CP_NONE},
		
		//RINSE
		{RINSE_PREWASH_FILL,		FOUR_MINUTES,			DUTY_C8,		CP_NONE},
		{RINSE_DRAIN,			TWO_MINUTES,			NO,			CP_NONE},//14-15
		{RINSE_MAINWASH_FILL,		FOUR_MINUTES,			DUTY_C8,		CP_NONE},
		{RINSE_DRAIN,			TWO_MINUTES,			NO,			CP_NONE},//16-17
		{RINSE3_ADDITIVES,		FIVE_MINUTES,			DUTY_C8,		CP_NONE},
		{RINSE_HOLD,			IDLE,				NO,			CP_NONE},
		{RINSE_DRAIN,			TWO_MINUTES,			NO,			CP_NONE},//18-20
		
		//FINAL SPIN
		{SPIN,				FOUR_MINUTES,			SPIN_PROFILE_FS8,	CP_NONE},
		{ANTI_CREASE,			FIVE_MINUTES,			DUTY_C17,		CP_NONE}

};//21-22	
				

//*****************************************************************************************************************************************************//
//********************************************************** WETCLEAN Program *************************************************************************//				

Operation wetclean[WETCLEAN_LENGTH]= {
		{DELAYSTART,				IDLE,				IDLE,			CP_NONE}, //0
		{DRAIN,					ONE_MINUTES,			NO,			CP_NONE}, //1

		//MAINWASH
		{FILLING,				FOUR_MINUTES,			DUTY_SW,		CP_NONE}, //2
		{HEAT,					IDLE,				DUTY_SW,		CP_NONE}, //3
		{SOAK,					ONE_MINUTES,			NO,			CP_NONE}, //4
		{TUMBLING,				ONE_MINUTES,			DUTY_SW1,		CP_NONE}, //5
		{SOAK,					ONE_MINUTES,			NO,			CP_NONE}, //6
		{TUMBLING,				ONE_MINUTES,			DUTY_SW1,		CP_NONE}, //7
		{SOAK,					ONE_MINUTES,			NO,			CP_NONE}, //8
		{TUMBLING,				ONE_MINUTES,			DUTY_SW1,		CP_NONE}, //9
		{SOAK,					ONE_MINUTES,			NO,			CP_NONE}, //10
		{TUMBLING,				ONE_MINUTES,			DUTY_SW1,		CP_NONE}, //11
		{SOAK,					ONE_MINUTES,			NO,			CP_NONE}, //12
		{TUMBLING,				ONE_MINUTES,			DUTY_SW1,		CP_NONE}, //13
		{TUMBLING,				EIGHT_MINUTES,			DUTY_SW,		CP_NONE}, //14
		{COOL_DOWN,				SIX_MINUTES,			DUTY_D1,		CP_NONE}, //15
		{DRAIN,					TWO_MINUTES,			NO,			CP_NONE}, //16
		
		//OPTIONAL RINSE 1
		{OPTION_EXTRA_RINSE_PREWASH,		SIX_MINUTES,			DUTY_SW,		CP_NONE}, //17
		{OPTIONAL_DRAIN,			TWO_MINUTES,			NO,			CP_NONE}, //18
		
		//OPTIONAL RINSE 2
		{OPTION_EXTRA_RINSE_MAINWASH,		SIX_MINUTES,			DUTY_SW,		CP_NONE}, //19
		{OPTIONAL_DRAIN,			TWO_MINUTES,			NO,			CP_NONE}, //20
		
		//RINSE
		{RINSE_PREWASH_FILL,			SIX_MINUTES,			DUTY_SW,		CP_NONE}, //21 
		{RINSE_DRAIN,				TWO_MINUTES,			NO,			CP_NONE}, //22
		{RINSE3_ADDITIVES,			SIX_MINUTES,			DUTY_SW,		CP_NONE}, //23
		{RINSE_HOLD,				IDLE,				NO,			CP_NONE}, //24
		{RINSE_DRAIN,				TWO_MINUTES,			NO,			CP_NONE}, //25
		
		//FINAL SPIN
		{SPIN,					FOUR_MINUTES,			SPIN_PROFILE_FS3,	CP_NONE}, //26
		{IDLE_STAGE,				IDLE,				IDLE,			CP_NONE}//27   //added for safety to display end properly if door gets opened during ramp down-- production issue
};
//*****************************************************************************************************************************************************//

//********************************************************** SHIRTS Program *************************************************************************//				

Operation shirts[SHIRTS_LENGTH] = {
		{DELAYSTART,			IDLE,				NO,			CP_NONE},	//0
		{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},	//1

		/*----------------------------------------------OPTIONAL_PREWASH--------------------------------*/
		{OPTIONAL_PREWASH,		SIX_MINUTES,			DUTY_C40,		CP_NONE},	//2
		{PREWASH_TUMBLING,		FIVE_MINUTES,			DUTY_C41,		CP_NONE},	//3
		{DRAIN_PREWASH,			ONE_MINUTES,			NO,			CP_NONE},	//4
		
		/*----------------------------------------------MAINWASH---------------------------------------*/
		{FILLING,			SIX_MINUTES,			DUTY_C40,		CP_NONE},	//5
		{HEAT,				IDLE,				DUTY_C41,		CP_NONE},	//6
		{SOAK,				IDLE,				DUTY_SOAK,		CP_NONE},	//7
		{TUMBLING,			IDLE,				DUTY_C41,		CP_NONE},	//8
		{MAIN_TUMBLE,			ONE_MINUTES,			DUTY_C41,		CP_NONE},	//9
		{MAINWASH_INTERMIDIATE_SPIN,	SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_NONE},	//10
		
		/*-------------------------------------------EXTRA_RINSE_1--------------------------------------*/
		{OPTION_EXTRA_RINSE_PREWASH,	SEVEN_MINUTES,			DUTY_C41,		CP_NONE},	//11
		{OPTIONAL_SPIN_TUMBLE,		ONE_MINUTES,			DUTY_C41,		CP_NONE},	//12
		{OPTIONAL_SPIN,			SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_NONE},	//13
		
		/*-------------------------------------------EXTRA_RINSE_2--------------------------------------*/
		{OPTION_EXTRA_RINSE_MAINWASH,	SEVEN_MINUTES,			DUTY_C41,		CP_NONE},	//14
		{OPTIONAL_SPIN_TUMBLE,		ONE_MINUTES,			DUTY_C41,		CP_NONE},	//15
		{OPTIONAL_SPIN,			SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_NONE},	//16
		
		/*------------------------------------------------RINSE_1----------------------------------------*/
		{RINSE_PREWASH_FILL,		SEVEN_MINUTES,			DUTY_C41,		CP_NONE},	//17
		{DRAIN_TUMBLE,			ONE_MINUTES,			DUTY_C41,		CP_NONE},	//18
		{RINSE_INTERMIDIATE_SPIN,	SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_NONE},	//19
		
		/*------------------------------------------------RINSE_2----------------------------------------*/
		{RINSE3_ADDITIVES,		SEVEN_MINUTES,			DUTY_C41,		CP_NONE},	//20
		{RINSE_HOLD,			IDLE,				NO,			CP_NONE},	//21
		{DRAIN_TUMBLE,			ONE_MINUTES,			DUTY_C41,		CP_NONE},	//22

		/*-------------------------------------------------STEAM----------------------------------------*/
		{STEAM_FILL,			ONE_MINUTES,			DUTY_IDLE,		CP_NONE},	//23
		{STEAM,				TWENTY_ONE_MINUTES,		DUTY_ST1,		CP_NONE},	//24
		{STEAM_CREASE,			TWO_MINUTES,			DUTY_ST1,		CP_NONE},	//25
		{STEAM_DRAIN,			ONE_MINUTES,			DUTY_ST1,		CP_NONE},	//26

		/*-------------------------------------------------SPIN----------------------------------------*/
		{SPIN,				SIX_MINUTES,			SPIN_PROFILE_FS11,	CP_NONE},	//27
		{ANTI_CREASE,			TWO_MINUTES,			DUTY_C17,		CP_NONE}	//28				
		
};
				
//*****************************************************************************************************************************************************//
//********************************************************** LINENWASH Program *************************************************************************//						

Operation linen[LINENWASH_LENGTH] = {
		{DELAYSTART,			IDLE,				NO,			CP_NONE},	//0
		{INITIAL_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},	//1
		
		/*----------------------------------------------STEAM--------------------------------*/
		{STEAM_FILL,			ONE_MINUTES,			DUTY_IDLE,		CP_OFF},//2
		{STEAM,				FIVE_MINUTES,			DUTY_SL1,		CP_OFF},//3
		
		/*----------------------------------------------MAINWASH---------------------------------------*/
		{FILLING,			IDLE,				DUTY_STL,		CP_ON},//4
		{HEAT,				IDLE,				DUTY_STL,		CP_OFF},//5
		{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X3,		CP_ON},	//6
		{TUMBLING,			IDLE,				DUTY_STL,		CP_ON}, //7
		{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X4,		CP_ON}, //8
		{TUMBLING,			IDLE,				DUTY_STL,		CP_ON}, //9
		{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X5,		CP_ON}, //10
		{TUMBLING,			IDLE,				DUTY_STL,		CP_ON}, //11
		{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X6,		CP_ON}, //12
		{TUMBLING,			IDLE,				DUTY_STL,		CP_ON}, //13
		{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X7,		CP_ON}, //14
		{TUMBLING,			IDLE,				DUTY_STL,		CP_ON}, //15
		{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X8,		CP_OFF}, //16
		{TUMBLING,			IDLE,				DUTY_STL,		CP_OFF}, //17
		{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X9,		CP_OFF}, //18
		{DRAIN,				ONE_MINUTES,			NO,			CP_OFF}, //19
		{MAINWASH_INTERMIDIATE_SPIN,	THREE_MINUTES,			SPIN_PROFILE_ISL,	CP_OFF}, //20
		
		/*-------------------------------------------EXTRA_RINSE_1--------------------------------------*/
		{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_STL,		CP_ON}, //21
		{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_OFF}, //22
		{OPTIONAL_SPIN,			THREE_MINUTES,			SPIN_PROFILE_ISL,	CP_OFF}, //23
		/*------------------------------------------------RINSE_1----------------------------------------*/
		{RINSE_MAINWASH_FILL,		SIX_MINUTES,			DUTY_STL,		CP_ON}, //24
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF},//25
		{RINSE_INTERMIDIATE_SPIN,	THREE_MINUTES,			SPIN_PROFILE_ISL,	CP_OFF}, //26
		
		/*------------------------------------------------ADDITIVE_RINSE----------------------------------------*/
		{RINSE3_ADDITIVES,		SIX_MINUTES,			DUTY_STL,		CP_ON}, //27
		
		/*------------------------------------------------RINSE_HOLD----------------------------------------*/
		{RINSE_HOLD,			IDLE,				NO,			CP_OFF}, //28
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF}, //29
		
		/*------------------------------------------------FINAL SPIN----------------------------------------*/
		{SPIN,				THREE_MINUTES,			SPIN_PROFILE_FS3,	CP_OFF}, //30
		
		/*------------------------------------------------FINAL STEAM----------------------------------------*/
		{STEAM_FILL,			ONE_MINUTES,			DUTY_IDLE,		CP_OFF}, //31
		{STEAM,				TEN_MINUTES,			DUTY_SL1,		CP_OFF}, //32
		{SPIN_TUMBLE,			ONE_MINUTES,			DUTY_SL1,		CP_OFF}, //33	
};

//********************************************************** Jeans Program *************************************************************************//				

Operation Jeans[JEANS_LENGTH] = {
		{DELAYSTART,			IDLE,				NO,			CP_NONE},
		{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},//1-2

		//OPTIONAL PREWASH
		{OPTIONAL_PREWASH,		SEVEN_MINUTES,			DUTY_C31,		CP_NONE},
		{PREWASH_TUMBLING,		SIX_MINUTES,			DUTY_C23,		CP_NONE}, //3-4
		{DRAIN_PREWASH,			ONE_MINUTES,			NO,			CP_NONE},
		{PREWASH_SPIN,			FOUR_MINUTES,			SPIN_PROFILE_IS2,	CP_NONE},//5-6
		
		//MAINWASH
		{FILLING,			SEVEN_MINUTES,			DUTY_X1,		CP_NONE},
		{TUMBLING,			TWO_MINUTES,			DUTY_C24,		CP_NONE},
		{TUMBLING,			THREE_MINUTES,			DUTY_X2,		CP_NONE}, //7-9
		{TUMBLING,			ONE_MINUTES,			DUTY_C23,		CP_NONE},
		{TUMBLING,			TWO_MINUTES,			DUTY_C24,		CP_NONE},
		{SOAK,				IDLE,				DUTY_SOAK,		CP_NONE},
		{HEAT,				IDLE,				DUTY_C6,		CP_NONE}, //10-12
		
		//ReHeat - Tx Step
		{TUMBLING,			TWO_MINUTES,			DUTY_C22,		CP_NONE},
		{REHEAT,			ONE_MINUTES,			DUTY_C22,		CP_NONE},
		{TUMBLING,			TWO_MINUTES,			DUTY_C22,		CP_NONE},
		{REHEAT,			ONE_MINUTES,			DUTY_C22,		CP_NONE},//13-16
		{TUMBLING,			TWO_MINUTES,			DUTY_C22,		CP_NONE},
		{REHEAT,			ONE_MINUTES,			DUTY_C22,		CP_NONE},
		{TUMBLING,			TWO_MINUTES,			DUTY_C22,		CP_NONE},
		{REHEAT,			ONE_MINUTES,			DUTY_C22,		CP_NONE},//17-20
		{TUMBLING,			TWO_MINUTES,			DUTY_C22,		CP_NONE},
		{REHEAT,			ONE_MINUTES,			DUTY_C22,		CP_NONE},
		{TUMBLING,			TWO_MINUTES,			DUTY_C24,		CP_NONE},
		{TUMBLING,			IDLE,				DUTY_C28,		CP_NONE},//21-24
		
		{TUMBLING,			TWO_MINUTES,			DUTY_C24,		CP_NONE},
		{TUMBLING,			TWO_MINUTES,			DUTY_C24,		CP_NONE},
		{COOL_DOWN,			SIX_MINUTES,			DUTY_D1,		CP_NONE},//25-27
		{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},
		{MAINWASH_INTERMIDIATE_SPIN,	SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_NONE}, // 28-29
		
		//OPTIONAL RINSE
		{OPTION_EXTRA_RINSE_MAINWASH,	FOUR_MINUTES,			DUTY_C25,		CP_NONE},
		{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_NONE},
		{OPTIONAL_SPIN,			SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_NONE}, //30-32
		
		//RINSE 
		{RINSE_PREWASH_FILL,		SIX_MINUTES,			DUTY_C25,		CP_NONE},
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},
		{RINSE_INTERMIDIATE_SPIN,	SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_NONE}, // 33-35
		
		{RINSE_MAINWASH_FILL,		SIX_MINUTES,			DUTY_C25,		CP_NONE},
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},
		{RINSE_INTERMIDIATE_SPIN,	SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_NONE}, //36-38
		
		{RINSE3_ADDITIVES,		SIX_MINUTES,			DUTY_C25,		CP_NONE},
		{HOTRINSE,			TWENTY_MINUTES,			DUTY_C25,		CP_NONE},
		{RINSE_HOLD,			IDLE,				NO,			CP_NONE},
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},//39-42
		
		//FINAL SPIN
		{SPIN,				TEN_MINUTES,			SPIN_PROFILE_FS2,	CP_NONE},
		{ANTI_CREASE,			FIVE_MINUTES,			DUTY_C17,		CP_NONE}
};//43-44	
								
//*****************************************************************************************************************************************************//
//******************************************************** ANTIALLERGEN Program **********************************************************************//				

Operation antiAllergen[ANTIAL_LENGTH] = {
		{DELAYSTART,			IDLE,				IDLE,			CP_NONE},	//0
		{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},	//1
		/*----------------------------------------------PREWASH---------------------------------------*/
		
		{OPTIONAL_PREWASH,		TEN_MINUTES,			DUTY_PW1,		CP_NONE},	//2
		{DRAIN_PREWASH,			ONE_MINUTES,			NO,			CP_NONE}, 	//3
		{PREWASH_SPIN,			FOUR_MINUTES,			SPIN_PROFILE_IS2,	CP_NONE},	//4
		
		/*----------------------------------------------MAINWASH---------------------------------------*/
		{FILLING,			EIGHT_MINUTES,			DUTY_N10,		CP_NONE},	//2
		{SOAK,				IDLE,				DUTY_SOAK,		CP_NONE},	//3
		{HEAT,				IDLE,				DUTY_M1,		CP_NONE},	//4
		{REHEAT,			IDLE,				DUTY_M1,		CP_NONE},	//5
		{COOL_DOWN,			SIX_MINUTES,			DUTY_D1,		CP_NONE},	//6
		{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},	//7
		{MAINWASH_INTERMIDIATE_SPIN,	THREE_MINUTES,			SPIN_PROFILE_IS3,	CP_NONE},	//8
		
		/*----------------------------------------------EXTRA_RINSE_1---------------------------------------*/
		{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_M1,		CP_NONE},	//9
		{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_NONE},	//10
		{OPTIONAL_SPIN,			THREE_MINUTES,			SPIN_PROFILE_IS3,	CP_NONE},	//11
		
		/*----------------------------------------------EXTRA_RINSE_2------------------------------------*/
		{OPTION_EXTRA_RINSE_MAINWASH,	FOUR_MINUTES,			DUTY_M1,		CP_NONE},	//12
		{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_NONE},	//13
		{OPTIONAL_SPIN,			THREE_MINUTES,			SPIN_PROFILE_IS3,	CP_NONE},	//14
		
		/*----------------------------------------------RINSE_1------------------------------------------*/
		{RINSE_PREWASH_FILL,		FOUR_MINUTES,			DUTY_M1,		CP_NONE},	//15
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},	//16
		{RINSE_INTERMIDIATE_SPIN,	THREE_MINUTES,			SPIN_PROFILE_IS3,	CP_NONE},	//17
		
		/*----------------------------------------------RINSE_2------------------------------------------*/
		{RINSE_MAINWASH_FILL,		FOUR_MINUTES,			DUTY_M1,		CP_NONE},	//18
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},	//19
		{RINSE_INTERMIDIATE_SPIN,	THREE_MINUTES,			SPIN_PROFILE_IS3,	CP_NONE},	//20			
		
		/*----------------------------------------------RINSE_3------------------------------------------*/
		{RINSE3_ADDITIVES,		FIVE_MINUTES,			DUTY_M1,		CP_NONE},	//21
		{HOTRINSE,			TWENTY_MINUTES,			DUTY_R1,		CP_NONE},	//22
		{RINSE_HOLD,			IDLE,				NO,			CP_NONE},	//23
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},	//24

		/*----------------------------------------------STEAM------------------------------------------*/
		{STEAM_FILL,			ONE_MINUTES,			DUTY_IDLE,		CP_NONE},	//25
		{STEAM,				TWENTY_ONE_MINUTES,		DUTY_ST1,		CP_NONE},	//26
		{STEAM_CREASE,			TWO_MINUTES,			DUTY_ST1,		CP_NONE},	//27
		{STEAM_DRAIN,			ONE_MINUTES,			DUTY_ST1,		CP_NONE},	//28
		
		/*----------------------------------------------SPIN------------------------------------------*/
		{SPIN,				SEVEN_MINUTES,			SPIN_PROFILE_FS6,	CP_NONE},	//29
		{IDLE_STAGE,			IDLE,				IDLE,			CP_NONE}//30   //added for safety to display end properly if door gets opened during ramp down-- production issue


};//27-30
	
				
				
//***************************************************************************************************************************************************//
//****************************************************** BABY WEAR Program **************************************************************************//				

Operation babyCare[BABYCARE_LENGTH] = {
		{DELAYSTART,			IDLE,				IDLE,			CP_NONE},	//0
		{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},	//1
		/*----------------------------------------------PREWASH---------------------------------------*/
		
		{OPTIONAL_PREWASH,		TEN_MINUTES,			DUTY_PW1,		CP_NONE},	//2
		{DRAIN_PREWASH,			ONE_MINUTES,			NO,			CP_NONE}, 	//3
		{PREWASH_SPIN,			FOUR_MINUTES,			SPIN_PROFILE_IS2,	CP_NONE},	//4

		/*----------------------------------------------MAINWASH---------------------------------------*/
		{FILLING,			SIX_MINUTES,			DUTY_R1,		CP_NONE},	//2
		{SOAK,				IDLE,				DUTY_SOAK,		CP_NONE},	//3
		{HEAT,				IDLE,				DUTY_R1,		CP_NONE},	//4
		{TUMBLING,			IDLE,				DUTY_R1,		CP_NONE},	//5
		{COOL_DOWN,			SIX_MINUTES,			DUTY_D1,		CP_NONE},	//6
		{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},	//7
		{MAINWASH_INTERMIDIATE_SPIN,	TWO_MINUTES,			SPIN_PROFILE_IS4,	CP_NONE},	//8
		
		/*-------------------------------------------EXTRA_RINSE_1---------------------------------------*/
		{OPTION_EXTRA_RINSE_PREWASH,	SIX_MINUTES,			DUTY_R1,		CP_NONE},	//9
		{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_NONE},	//10
		{OPTIONAL_SPIN,			TWO_MINUTES,			SPIN_PROFILE_IS4,	CP_NONE},	//11
		
		/*-------------------------------------------EXTRA_RINSE_2---------------------------------------*/
		{OPTION_EXTRA_RINSE_MAINWASH,	SIX_MINUTES,			DUTY_R1,		CP_NONE},	//12
		{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_NONE},	//13
		{OPTIONAL_SPIN,			TWO_MINUTES,			SPIN_PROFILE_IS4,	CP_NONE},	//14
		
		/*-------------------------------------------RINSE_1-----------------------------------------*/
		{RINSE_PREWASH_FILL,		SIX_MINUTES,			DUTY_R1,		CP_NONE},	//15
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},	//16
		{RINSE_INTERMIDIATE_SPIN,	TWO_MINUTES,			SPIN_PROFILE_IS4,	CP_NONE},	//17
		
		/*-------------------------------------------RINSE_2-----------------------------------------*/
		{RINSE_MAINWASH_FILL,		SIX_MINUTES,			DUTY_R1,		CP_NONE},	//18
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},	//19
		{RINSE_INTERMIDIATE_SPIN,	TWO_MINUTES,			SPIN_PROFILE_IS4,	CP_NONE},	//20			
		
		/*-------------------------------------------RINSE_3-----------------------------------------*/
		{RINSE3_ADDITIVES,		SIX_MINUTES,			DUTY_R1,		CP_NONE},	//21
		{HOTRINSE,			TWENTY_MINUTES,			DUTY_R1,		CP_NONE},	//22
		{RINSE_HOLD,			IDLE,				NO,			CP_NONE},	//23
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},	//24

		/*-------------------------------------------STEAM-----------------------------------------*/
		{STEAM_FILL,			ONE_MINUTES,			DUTY_IDLE,		CP_NONE},	//25
		{STEAM,				TWENTY_ONE_MINUTES,		DUTY_ST1,		CP_NONE},	//26
		{STEAM_CREASE,			TWO_MINUTES,			DUTY_ST1,		CP_NONE},	//27
		{STEAM_DRAIN,			ONE_MINUTES,			DUTY_ST1,		CP_NONE},	//28

		/*-------------------------------------------SPIN-----------------------------------------*/
		{SPIN,				THREE_MINUTES,			SPIN_PROFILE_FS4,	CP_NONE},	//29
		{ANTI_CREASE,			THREE_MINUTES,			DUTY_C4,		CP_NONE},	//30

};
				
				
				
//*******************************************************************************************************************************************************//
//********************************************************* SPORTSWEAR Program **************************************************************************//				

Operation sports[SPORT_LENGTH] = {
		{DELAYSTART,			IDLE,				IDLE,			CP_NONE},
		{DRAIN,				ONE_MINUTES,			NO,			CP_NONE}, //1-2

		//OPTIONAL PREWASH		
		{OPTIONAL_PREWASH,		SIX_MINUTES,			DUTY_C1,		CP_NONE},
		{PREWASH_TUMBLING,		SIX_MINUTES,			DUTY_E1,		CP_NONE},
		{DRAIN_PREWASH,			ONE_MINUTES,			NO,			CP_NONE}, //3-5
		{PREWASH_SPIN,			FOUR_MINUTES,			SPIN_PROFILE_IS2,	CP_NONE}, //6
		
		//MAINWASH
		{FILLING,			SIX_MINUTES,			DUTY_C1,		CP_NONE},
		{SOAK,				IDLE,				DUTY_SOAK,		CP_NONE},
		{HEAT,				IDLE,				DUTY_E2,		CP_NONE},
		{TUMBLING,			IDLE,				DUTY_E2,		CP_NONE},//7-10
		{TUMBLING,			FOUR_MINUTES,			DUTY_C5,		CP_NONE},
		{COOL_DOWN,			SIX_MINUTES,			DUTY_D1,		CP_NONE},
		{DRAIN,				ONE_MINUTES,			NO,			CP_NONE}, //11-13
		{MAINWASH_INTERMIDIATE_SPIN,	THREE_MINUTES,			SPIN_PROFILE_IS3,	CP_NONE},// 14
		
		//OPTIONAL RINSE
		{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_E2,		CP_NONE},
		{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_NONE},
		{OPTIONAL_SPIN,			THREE_MINUTES,			SPIN_PROFILE_IS3,	CP_NONE}, //15-17
		{OPTION_EXTRA_RINSE_MAINWASH,	FOUR_MINUTES,			DUTY_E2,		CP_NONE},
		{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_NONE},
		{OPTIONAL_SPIN,			THREE_MINUTES,			SPIN_PROFILE_IS3,	CP_NONE},//18-20
		
		//RINSE
		{RINSE_PREWASH_FILL,		FOUR_MINUTES,			DUTY_E2,		CP_NONE},
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},
		{RINSE_INTERMIDIATE_SPIN,	THREE_MINUTES,			SPIN_PROFILE_IS3,	CP_NONE},//21-23
		{RINSE3_ADDITIVES,		FIVE_MINUTES,			DUTY_C4,		CP_NONE},
		{RINSE_HOLD,			IDLE,				NO,			CP_NONE},
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE}, //24-26
		
		//FINAL SPIN
		{SPIN,				FOUR_MINUTES,			SPIN_PROFILE_FS8,	CP_NONE},
		{ANTI_CREASE,			TWO_MINUTES,			DUTY_C17,		CP_NONE}
};//27-28
			
			
//*********************************************************************************************************************************************//
//*********************************************************** SYNTHETIC Program **************************************************************//			
/*
Operation synth[SYNTH_LENGTH] = {
		{DELAYSTART,			IDLE,				NO,			CP_NONE},
		{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},   // 1-2
		//OPTIONAL PREWASH
		{OPTIONAL_PREWASH,		SIX_MINUTES,			DUTY_C31,		CP_NONE},
		{PREWASH_TUMBLING,		EIGHT_MINUTES,			DUTY_C23,		CP_NONE},
		{DRAIN_PREWASH,			ONE_MINUTES,			NO,			CP_NONE}, // 3-5
		
		//MAINWASH
		{FILLING,			SIX_MINUTES,			DUTY_C31,		CP_NONE},
		{TUMBLING,			IDLE,				DUTY_C23,		CP_NONE},
		{TUMBLINGSPECIAL,		TWO_MINUTES,			DUTY_X1,		CP_NONE},
		{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X6,		CP_NONE},//6-9
		{SOAK,				IDLE,				DUTY_SOAK,		CP_NONE},
		{HEAT,				IDLE,				DUTY_C23,		CP_NONE}, // 10-11
		
		{TUMBLING,			TWO_MINUTES,			DUTY_C23,		CP_NONE},
		{REHEAT,			ONE_MINUTES,			DUTY_C23,		CP_NONE},
		{TUMBLING,			TWO_MINUTES,			DUTY_C23,		CP_NONE},
		{REHEAT,			ONE_MINUTES,			DUTY_C23,		CP_NONE},//12-15
		{TUMBLING,			IDLE,				IDLE,			CP_NONE},
		{TUMBLING,			THREE_MINUTES,			IDLE,			CP_NONE}, //16-17
		
		{COOL_DOWN,			SIX_MINUTES,			DUTY_D1,		CP_NONE},
		{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},
		{MAINWASH_INTERMIDIATE_SPIN,	THREE_MINUTES,			SPIN_PROFILE_IS6,	CP_NONE}, //18-20
		
		
		//OPTIONAL RINSE
		{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_C27,		CP_NONE},
		{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_NONE},
		{OPTIONAL_SPIN,			THREE_MINUTES,			SPIN_PROFILE_IS6,	CP_NONE}, //21-23
		{OPTION_EXTRA_RINSE_MAINWASH,	FOUR_MINUTES,			DUTY_C27,		CP_NONE},
		{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_NONE},
		{OPTIONAL_SPIN,			THREE_MINUTES,			SPIN_PROFILE_IS6,	CP_NONE}, //24-26
		
		//RINSE 
		{RINSE_PREWASH_FILL,		FIVE_MINUTES,			DUTY_C27,		CP_NONE},
		{RINSE_HALFDRAIN,		ONE_MINUTES,			NO,			CP_NONE},
		{RINSE_TUMBLING,		ONE_MINUTES,			DUTY_C12,		CP_NONE},
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},
		{RINSE_INTERMIDIATE_SPIN,	THREE_MINUTES,			SPIN_PROFILE_IS6,	CP_NONE},// 27-31
		{RINSE_MAINWASH_FILL,		FIVE_MINUTES,			DUTY_C27,		CP_NONE},
		{RINSE_HALFDRAIN,		ONE_MINUTES,			NO,			CP_NONE},
		{RINSE_TUMBLING,		ONE_MINUTES,			DUTY_C12,		CP_NONE},
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},
		{RINSE_INTERMIDIATE_SPIN,	THREE_MINUTES,			SPIN_PROFILE_IS6,	CP_NONE},//32-36
		{RINSE3_ADDITIVES,		SIX_MINUTES,			DUTY_C27,		CP_NONE},
		{HOTRINSE,			TWENTY_MINUTES,			DUTY_C27,		CP_NONE},
		{RINSE_HOLD,			IDLE,				NO,			CP_NONE},
		{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},//37-40
		
		//FINAL SPIN
		{SPIN,				FOUR_MINUTES,			SPIN_PROFILE_FS8,	CP_NONE},
		{ANTI_CREASE,			THREE_MINUTES,			DUTY_C17,		CP_NONE}
};//41-42*/
			
//*********************************************************** SYNTHETIC Cold,30C,40C,60C **************************************************************//			

//Operation synth[SYNTH_LENGTH] = {
//	/*0*/	{DELAYSTART,			IDLE,				NO,			CP_OFF},	//0
//	/*1*/	{INITIAL_DRAIN,			ONE_MINUTES,			NO,			CP_OFF},	//1  
//		/*------------------------------------------OPTIONAL PREWASH------------------------------------*/
//	/*2*/	{OPTIONAL_PREWASH,		FOUR_MINUTES,			DUTY_C30,		CP_OFF},	//2
//	/*3*/	{IDLE_STAGE,				IDLE,				IDLE,			CP_OFF},	//3 //This step won't be executed it has beed adjusted in previous state.
//	/*4*/	{PREWASH_TUMBLING,		TWO_MINUTES,			DUTY_SN1,		CP_ON},		//4  //SN2 NEW DUTY
//	/*5*/	{PREWASH_TUMBLING,		EIGHT_MINUTES,			DUTY_C23,		CP_OFF},	//5 // LAST 30SEC 
//	/*6*/   {DRAIN_PREWASH,			ONE_MINUTES,			NO,			CP_NONE}, //1-2
//		
//		/*-------------------------------------------MAINWASH-------------------------------------------*/
//	/*7*/	{FILLING,			FOUR_MINUTES,			DUTY_C30,		CP_OFF},	//6
//	/*8*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF},	//7 //This step won't be executed it has beed adjusted in previous state.
//	/*9*/	{TUMBLING,			TWO_MINUTES,			DUTY_SN1,		CP_ON},		//8 //SN1 NEW DUTY
//	/*10*/	{TUMBLING,			FOUR_MINUTES,			DUTY_SN2,		CP_ON},		//9 //SN2 NEW DUTY
//	/*11*/	{TUMBLING,			ONE_MINUTES,			DUTY_SN2,		CP_ON},		//10 //SN2 NEW DUTY
//	/*12*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X1,		CP_ON},		//11
//	/*13*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X6,		CP_ON},	//12
//	/*14*/	{SOAK,				IDLE,				DUTY_SOAK,		CP_NONE},	//13
//	/*15*/	{HEATING,			IDLE,				DUTY_SN2,		CP_1MinON_1MinOFF},//14 // SN2 
//	/*16*/	{TUMBLING,			TWO_MINUTES,			DUTY_SN2,		CP_OFF},	//15 //SN2
//	/*17*/	{REHEAT,			ONE_MINUTES,			DUTY_SN2,		CP_OFF},	//16 //SN2
//	/*18*/	{TUMBLING,			TWO_MINUTES,			DUTY_SN2,		CP_OFF},	//17 //SN2
//	/*19*/	{REHEAT,			ONE_MINUTES,			DUTY_SN2,		CP_OFF},	//18 //SN2
//	/*20*/	{TUMBLING_TX,			IDLE,				DUTY_SN2,		CP10MinON_2MinOFF},//19 //TX SN2 DUTY
//	/*21*/	{TUMBLING,			THREE_MINUTES,			DUTY_C23,		CP_OFF},	//20 LAST 30 sec
//	/*22*/	{COOL_DOWN,			SIX_MINUTES,			DUTY_D1,		CP_OFF},	//21
//	/*23*/	{MAINWASH_INTERMIDIATE_SPIN,	FOUR_MINUTES,			SPIN_PROFILE_IS8,	CP_OFF},	//22
//		
//		
//		/*-----------------------------------------OPTIONAL EXTRA RINSE_1------------------------------------------*/
//	/*24*/	{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_SN3,		CP_ON},	//23  LAST 30SEC 
//	/*25*/	{IDLE_STAGE,				IDLE,				IDLE,			CP_OFF},	//24 //This step won't be executed it has beed adjusted in previous state.
//	/*26*/	{OPTIONAL_SPIN,			FOUR_MINUTES,			SPIN_PROFILE_IS8,	CP_OFF},	//25 //21-23
//		
//		/*-----------------------------------------OPTIONAL EXTRA RINSE_2------------------------------------------*/
//	/*27*/	{OPTION_EXTRA_RINSE_MAINWASH,	FOUR_MINUTES,			DUTY_SN3,		CP_ON},	//26
//	/*28*/	{IDLE_STAGE,				IDLE,				IDLE,			CP_OFF},	//27 This step won't be executed it has beed adjusted in previous state.
//	/*29*/	{OPTIONAL_SPIN,			FOUR_MINUTES,			SPIN_PROFILE_IS8,	CP_OFF},	//28
//
//		/*-----------------------------------------OPTIONAL EXTRA RINSE_3------------------------------------------*/
//	/*30*/	{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_SN3,		CP_ON},	//29
//	/*31*/	{IDLE_STAGE,				IDLE,				IDLE,			CP_OFF},		//30 This step won't be executed it has beed adjusted in previous state.
//	/*32*/	{OPTIONAL_SPIN,			FOUR_MINUTES,			SPIN_PROFILE_IS8,	CP_OFF},	//31
//
//		/*-----------------------------------------------RINSE_1-----------------------------------------------*/ 
//	/*33*/	{RINSE_PREWASH_FILL,		SIX_MINUTES,			DUTY_SN3,		CP_ON},	//32
//	/*34*/	{IDLE_STAGE,				IDLE,				IDLE,			CP_OFF},	//33//This step won't be executed it has beed adjusted in previous state.
//	/*35*/	{RINSE_HALFDRAIN,		ONE_MINUTES,			NO,			CP_OFF},	//34
//	/*36*/	{RINSE_TUMBLING,		ONE_MINUTES,			DUTY_C12,		CP_OFF},	//35
//	/*37*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF},	//36
//	/*38*/	{RINSE_INTERMIDIATE_SPIN,	FOUR_MINUTES,			SPIN_PROFILE_IS8,	CP_OFF},	//37
//		
//		/*-----------------------------------------------RINSE_2-----------------------------------------------*/ 
//	/*39*/	{RINSE_MAINWASH_FILL,		IDLE,				DUTY_SN3,		CP_ON},	//38
//	/*40*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF},	//39 This step won't be executed it has beed adjusted in previous state.
//	/*41*/	{RINSE_HALFDRAIN,		ONE_MINUTES,			NO,			CP_OFF},	//40
//	/*42*/	{RINSE_TUMBLING,		ONE_MINUTES,			DUTY_C12,		CP_OFF},	//41
//	/*43*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF},	//42
//	/*44*/	{RINSE_INTERMIDIATE_SPIN,	FOUR_MINUTES,			SPIN_PROFILE_IS8,	CP_OFF},	//43
//		
//		/*-----------------------------------------------RINSE_3-----------------------------------------------*/ 
//	/*45*/	{RINSE3_ADDITIVES,		IDLE,				DUTY_SN3,		CP_ON},	//44 This step won't be executed it has beed adjusted in previous state.
//	/*46*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF},		//45//SN3 
//	/*47*/	{HOTRINSE,			TWENTY_MINUTES,			DUTY_C27,		CP_OFF},	//46 
//	/*48*/	{RINSE_HOLD,			IDLE,				NO,			CP_OFF},	//47
//	/*49*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF},	//48
//
//		/*-------------------------------------------STEAM-----------------------------------------*/
//	/*50*/	{STEAM_FILL,			ONE_MINUTES,			DUTY_IDLE,		CP_OFF},	//49
//	/*51*/	{STEAM,				TWENTY_ONE_MINUTES,		DUTY_ST1,		CP_OFF},	//50
//	/*52*/	{STEAM_CREASE,			TWO_MINUTES,			DUTY_ST1,		CP_OFF},	//51
//	/*53*/	{STEAM_DRAIN,			ONE_MINUTES,			DUTY_ST1,		CP_OFF},	//52
//		
//		/*--------------------------------------------FINAL SPIN----------------------------------------------*/
//	/*54*/	{SPIN,				ELEVEN_MINUTES,			SPIN_PROFILE_FS1,	CP_OFF},	//53
//	/*55*/	{ANTI_CREASE,			TWO_MINUTES,			DUTY_C17,		CP_OFF}		//54
//};

Operation synth[SYNTH_LENGTH] = {
	/*0*/	{DELAYSTART,			IDLE,				NO,			CP_OFF},	//0
	/*1*/	{INITIAL_DRAIN,			ONE_MINUTES,			NO,			CP_OFF},	//1  
	
		/*------------------------------------------OPTIONAL PREWASH------------------------------------*/
	/*2*/	{OPTIONAL_PREWASH,		FOUR_MINUTES,			DUTY_C30,		CP_OFF},	//2
	/*3*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF},	//3 //This step won't be executed it has beed adjusted in previous state.
	/*4*/	{PREWASH_TUMBLING,		TWO_MINUTES,			DUTY_C31,		CP_OFF},		//4  //SN2 NEW DUTY
	/*5*/	{PREWASH_TUMBLING,		EIGHT_MINUTES,			DUTY_C23,		CP_OFF},	//5 // LAST 30SEC 
	/*6*/   {DRAIN_PREWASH,			ONE_MINUTES,			NO,			CP_NONE}, //1-2
		
		/*-------------------------------------------MAINWASH-------------------------------------------*/
	/*7*/	{FILLING,			FOUR_MINUTES,			DUTY_C30,		CP_OFF},	//6
	/*8*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF},	//7 //This step won't be executed it has beed adjusted in previous state.
	/*9*/	{TUMBLING,			TWO_MINUTES,			DUTY_C31,		CP_OFF},		//8 //SN1 NEW DUTY
	/*10*/	{TUMBLING,			ONE_MINUTES,			DUTY_PMX,		CP_OFF},		//9 //SN2 NEW DUTY
	/*11*/	{TUMBLING,			IDLE,				DUTY_C23,		CP_OFF},		//10 //SN2 NEW DUTY
	/*12*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X1,		CP_OFF},		//11
	/*13*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X6,		CP_OFF},	//12
	/*14*/	{TUMBLING,			ONE_MINUTES,			DUTY_PMX,		CP_OFF},		
	/*15*/	{SOAK,				IDLE,				DUTY_SOAK,		CP_NONE},	//13
	/*16*/	{HEATING,			IDLE,				DUTY_C23,		CP_OFF},//14 // SN2 
	
	/*17*/	{TUMBLING,			IDLE,				DUTY_C23,		CP_OFF},	//15 //SN2
	/*18*/	{REHEAT,			IDLE,				DUTY_C23,		CP_OFF},	//16 //SN2
	/*19*/	{TUMBLING,			IDLE,				DUTY_C23,		CP_OFF},	//17 //SN2
	/*20*/	{REHEAT,			IDLE,				DUTY_C23,		CP_OFF},	//18 //SN2
	
	/*21*/	{TUMBLING_TX,			IDLE,				DUTY_C22,		CP_OFF},//19 //TX SN2 DUTY
	/*22*/	{TUMBLING,			THREE_MINUTES,			DUTY_C23,		CP_OFF},	//20 LAST 30 sec
	/*23*/	{COOL_DOWN,			SIX_MINUTES,			DUTY_D1,		CP_OFF},	//21
	/*24*/	{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},
	/*25*/	{MAINWASH_INTERMIDIATE_SPIN,	FIVE_MINUTES,			SPIN_PROFILE_IS13,	CP_OFF},	//22
		
		
		/*-----------------------------------------OPTIONAL EXTRA RINSE_1------------------------------------------*/
	/*26*/	{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_C27,		CP_OFF},	//23  LAST 30SEC 
	/*27*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF},	//24 //This step won't be executed it has beed adjusted in previous state.
	/*28*/	{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_NONE},
	/*29*/	{OPTIONAL_SPIN,			FIVE_MINUTES,			SPIN_PROFILE_IS13,	CP_OFF},	//25 //21-23
	
		/*-----------------------------------------OPTIONAL EXTRA RINSE_2------------------------------------------*/
	/*30*/	{OPTION_EXTRA_RINSE_MAINWASH,	FOUR_MINUTES,			DUTY_C27,		CP_OFF},	//26
	/*31*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF},	//27 This step won't be executed it has beed adjusted in previous state.
	/*32*/	{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_NONE},
	/*33*/	{OPTIONAL_SPIN,			FIVE_MINUTES,			SPIN_PROFILE_IS13,	CP_OFF},	//28

		/*-----------------------------------------OPTIONAL EXTRA RINSE_3------------------------------------------*/
	/*34*/	{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_C27,		CP_OFF},	//29
	/*35*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF},		//30 This step won't be executed it has beed adjusted in previous state.
	/*36*/	{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_NONE},
	/*37*/	{OPTIONAL_SPIN,			FIVE_MINUTES,			SPIN_PROFILE_IS13,	CP_OFF},	//31

		/*-----------------------------------------------RINSE_1-----------------------------------------------*/ 
	/*38*/	{RINSE_PREWASH_FILL,		FIVE_MINUTES,			DUTY_C27,		CP_OFF},	//32
	/*39*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF},	//33//This step won't be executed it has beed adjusted in previous state.
	/*40*/	{RINSE_HALFDRAIN,		ONE_MINUTES,			NO,			CP_OFF},	//34
	/*41*/	{RINSE_TUMBLING,		ONE_MINUTES,			DUTY_C12,		CP_OFF},	//35
	/*42*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF},	//36
	/*43*/	{RINSE_INTERMIDIATE_SPIN,	FIVE_MINUTES,			SPIN_PROFILE_IS13,	CP_OFF},	//37
		
		/*-----------------------------------------------RINSE_2-----------------------------------------------*/ 
	/*44*/	{RINSE_MAINWASH_FILL,		IDLE,				DUTY_C27,		CP_OFF},	//38
	/*45*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF},	//39 This step won't be executed it has beed adjusted in previous state.
	/*46*/	{RINSE_HALFDRAIN,		ONE_MINUTES,			NO,			CP_OFF},	//40
	/*47*/	{RINSE_TUMBLING,		ONE_MINUTES,			DUTY_C12,		CP_OFF},	//41
	/*48*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF},	//42
	/*49*/	{RINSE_INTERMIDIATE_SPIN,	FIVE_MINUTES,			SPIN_PROFILE_IS13,	CP_OFF},	//43
		
		/*-----------------------------------------------RINSE_3-----------------------------------------------*/ 
	/*50*/	{RINSE3_ADDITIVES,		IDLE,				DUTY_C27,		CP_OFF},	//44 This step won't be executed it has beed adjusted in previous state.
	/*51*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF},		//45//SN3 
	/*52*/	{HOTRINSE,			TWENTY_MINUTES,			DUTY_C27,		CP_OFF},	//46 
	/*53*/	{RINSE_HOLD,			IDLE,				NO,			CP_OFF},	//47
	/*54*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF},	//48

		/*-------------------------------------------STEAM-----------------------------------------*/
	/*55*/	{STEAM_FILL,			ONE_MINUTES,			DUTY_IDLE,		CP_OFF},	//49
	/*56*/	{STEAM,				TWENTY_ONE_MINUTES,		DUTY_ST1,		CP_OFF},	//50
	/*57*/	{STEAM_CREASE,			TWO_MINUTES,			DUTY_ST1,		CP_OFF},	//51
	/*58*/	{STEAM_DRAIN,			ONE_MINUTES,			DUTY_ST1,		CP_OFF},	//52
		
		/*--------------------------------------------FINAL SPIN----------------------------------------------*/
	/*59*/	{SPIN,				ELEVEN_MINUTES,			SPIN_PROFILE_FS16,	CP_OFF},	//53
	/*60*/	{ANTI_CREASE,			TWO_MINUTES,			DUTY_C17,		CP_OFF}		//54
};
//********************************************************************************************************************************************************//
//*********************************************************** MIXED SOILED Program **********************************************************************//			
/*
Operation mixedSoil[MIXEDSOIL_LENGTH] = {
			{DELAYSTART,			IDLE,				NO,			CP_NONE},
			{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},  // 1-2
			//OPTIONAL PREWASH
			{OPTIONAL_PREWASH,		SIX_MINUTES,			DUTY_C30,		CP_NONE},
			{PREWASH_TUMBLING,		FIVE_MINUTES,			DUTY_C23,		CP_NONE},
			{DRAIN_PREWASH,			ONE_MINUTES,			NO,			CP_NONE}, // 3-5
			
			//MAINWASH
			{FILLING,			SIX_MINUTES,			DUTY_C30,		CP_NONE},
			{TUMBLING,			TWO_MINUTES,			DUTY_C23,		CP_NONE},
			{TUMBLINGSPECIAL,		TWO_MINUTES,			DUTY_X1,		CP_NONE},
			{TUMBLINGSPECIAL,		TWO_MINUTES,			DUTY_X6,		CP_NONE},//6-9
			{SOAK,				IDLE,				DUTY_SOAK,		CP_NONE},
			{HEAT,				IDLE,				DUTY_C23,		CP_NONE},
			{TUMBLING,			TWO_MINUTES,			DUTY_C23,		CP_NONE},
			{REHEAT,			ONE_MINUTES,			DUTY_C23,		CP_NONE},
			{TUMBLING,			IDLE,				DUTY_C23,		CP_NONE}, //10-14
			{COOL_DOWN,			SIX_MINUTES,			DUTY_D1,		CP_NONE},
			{DRAIN,				ONE_MINUTES,			NO,			CP_NONE},
			{MAINWASH_INTERMIDIATE_SPIN,	THREE_MINUTES,			SPIN_PROFILE_IS3,	CP_NONE}, //15-17
			
			//OPTIONAL RINSE
			{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_C27,		CP_NONE},
			{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_NONE},
			{OPTIONAL_SPIN,			THREE_MINUTES,			SPIN_PROFILE_IS3,	CP_NONE}, //18-20
			{OPTION_EXTRA_RINSE_MAINWASH,	FOUR_MINUTES,			DUTY_C27,		CP_NONE},
			{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_NONE},
			{OPTIONAL_SPIN,			THREE_MINUTES,			SPIN_PROFILE_IS3,	CP_NONE}, //18-20
			{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_C27,		CP_NONE},
			{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_NONE},
			{OPTIONAL_SPIN,			THREE_MINUTES,			SPIN_PROFILE_IS3,	CP_NONE}, //18-20
			
			//RINSE 
			{RINSE_PREWASH_FILL,		SIX_MINUTES,			DUTY_C27,		CP_NONE},
			{RINSE_HALFDRAIN,		ONE_MINUTES,			NO,			CP_NONE},
			{RINSE_TUMBLING,		ONE_MINUTES,			DUTY_C12,		CP_NONE},
			{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},
			{RINSE_INTERMIDIATE_SPIN,	THREE_MINUTES,			SPIN_PROFILE_IS3,	CP_NONE}, // 21-25
			{RINSE3_ADDITIVES,		SIX_MINUTES,			DUTY_C27,		CP_NONE},
			{HOTRINSE,			TWENTY_MINUTES,			DUTY_C27,		CP_NONE},
			{RINSE_HOLD,			IDLE,				NO,			CP_NONE},
			{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},//31-34
			
			//FINAL SPIN
			{SPIN,				SEVEN_MINUTES,			SPIN_PROFILE_FS6,	CP_NONE},
			{ANTI_CREASE,			THREE_MINUTES,			DUTY_C17,		CP_NONE},//35-36
  			
  			//STEAM
			{STEAM_FILL,			ONE_MINUTES,			DUTY_IDLE,		CP_NONE},
			{STEAM,				TWENTY_ONE_MINUTES,		DUTY_ST1,		CP_NONE},
			{STEAM_CREASE,			TWO_MINUTES,			DUTY_ST1,		CP_NONE},
			{STEAM_DRAIN,			ONE_MINUTES,			DUTY_ST1,		CP_NONE}

};//37-40*/
	
//*********************************************************** MIXED SOILED Program **********************************************************************//			

Operation mixedSoil[MIXEDSOIL_LENGTH] = {
	/*0*/	{DELAYSTART,			IDLE,				NO,			CP_NONE},	//0
	/*1*/	{INITIAL_DRAIN,			ONE_MINUTES,			NO,			CP_NONE},	//1  
		
		/*--------------------------------------OPTIONAL PREWASH-----------------------------------------*/
	/*2*/	{OPTIONAL_PREWASH,		FOUR_MINUTES,			DUTY_C30,		CP_OFF},	//2
	/*3*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF},	//3	//This step won't be executed it has beed adjusted in previous state.
	/*4*/	{PREWASH_TUMBLING,		TWO_MINUTES,			DUTY_C30,		CP_OFF},		//4
	/*5*/	{PREWASH_TUMBLING,		FIVE_MINUTES,			DUTY_C23,		CP_OFF},	//5//Last 30 sec drain
	/*6*/	{DRAIN_PREWASH,			ONE_MINUTES,			NO,			CP_NONE}, //1-2
		/*---------------------------------------MAINWASH------------------------------------------------*/
	/*7*/	{FILLING,			FOUR_MINUTES,			DUTY_C30,		CP_OFF},	//6
	/*8*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF},	//7	//This step won't be executed it has beed adjusted in previous state.
	/*9*/	{TUMBLING,			THREE_MINUTES,			DUTY_M3,		CP_OFF},		//8// M23/M3 new Duty
	/*10*/	{SOAK,				ONE_MINUTES,			NO,			CP_OFF},
	/*11*/	{TUMBLING,			ONE_MINUTES,			DUTY_SD,		CP_OFF},		//9// M23 new Duty
	/*12*/	{SOAK,				ONE_MINUTES,			NO,			CP_OFF},
	/*13*/	{TUMBLING,			ONE_MINUTES,			DUTY_SD,		CP_OFF},	
	/*14*/	{SOAK,				ONE_MINUTES,			NO,			CP_OFF},
	/*15*/	{TUMBLING,			ONE_MINUTES,			DUTY_SD,		CP_OFF},
	/*16*/	{TUMBLING,			IDLE,				DUTY_M3,		CP_OFF},
	/*17*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X1,		CP_OFF},		//10
	/*18*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X6,		CP_OFF},		//11
	/*19*/	{SOAK,				EIGHTEEN_MINUTES,		DUTY_SOAK,		CP_NONE},	//12
	/*20*/	{HEATING,			IDLE,				DUTY_M3,		CP_OFF},		//13 	//M23
	/*21*/	{TUMBLING,			IDLE,				DUTY_M3,		CP_OFF},	//14
	/*22*/	{REHEAT,			IDLE,				DUTY_M3,		CP_OFF},	//15
	/*23*/	{TUMBLING_TX,			IDLE,				DUTY_M3,		CP_OFF},//16	//TX Last 30 sec
	/*24*/	{MAINWASH_HALFDRAIN,		ONE_MINUTES,			NO,			CP_OFF},
	/*25*/	{STEAM,				TWO_MINUTES,			DUTY_ST1,		CP_OFF},	
	/*26*/	{COOL_DOWN,			SIX_MINUTES,			DUTY_D1,		CP_OFF},	//17
	/*27*/	{DRAIN,				ONE_MINUTES,                    NO,			CP_OFF},
	/*28*/	{MAINWASH_INTERMIDIATE_SPIN,	FOUR_MINUTES,			SPIN_PROFILE_ISA,	CP_OFF},	//18 
		
		/*------------------------------------OPTIONAL EXTRA_RINSE_1---------------------------------------*/
	/*29*/	{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_M3,		CP_OFF},		//19 LAST 30 SEC
	/*30*/	{OPTIONAL_DRAIN,		ONE_MINUTES,                    NO,			CP_OFF},
	/*31*/	{OPTIONAL_SPIN,			FIVE_MINUTES,			SPIN_PROFILE_IS13,	CP_OFF},	//21 
		
		/*------------------------------------OPTIONAL EXTRA_RINSE_2---------------------------------------*/
	/*32*/	{OPTION_EXTRA_RINSE_MAINWASH,	FOUR_MINUTES,			DUTY_M3,		CP_OFF},		//22 LAST 30 SEC
	/*33*/	{OPTIONAL_DRAIN,		ONE_MINUTES,                    NO,			CP_OFF},
	/*34*/	{OPTIONAL_SPIN,			FIVE_MINUTES,			SPIN_PROFILE_IS13,	CP_OFF},	//24
	
		/*------------------------------------OPTIONAL EXTRA_RINSE_3---------------------------------------*/
	/*35*/	{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_M3,		CP_OFF},		//25 LAST 30 SEC 
	/*36*/	{OPTIONAL_DRAIN,		ONE_MINUTES,                    NO,			CP_OFF},
	/*37*/	{OPTIONAL_SPIN,			FIVE_MINUTES,			SPIN_PROFILE_IS13,	CP_OFF},	//27 
		
		/*------------------------------------------RINSE_1---------------------------------------------------*/ 
	/*38*/	{RINSE_PREWASH_FILL,		IDLE,				DUTY_M3,		CP_OFF},		//28
	/*39*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF},
	/*40*/	{RINSE_HALFDRAIN,		ONE_MINUTES,			NO,			CP_OFF},	//30
	/*41*/	{RINSE_TUMBLING, 		ONE_MINUTES,			DUTY_C12,		CP_OFF},	//31 
	/*42*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF},	//32
	/*43*/	{RINSE_INTERMIDIATE_SPIN,	FIVE_MINUTES,			SPIN_PROFILE_IS13,	CP_OFF},	//33 
		
		/*------------------------------------------RINSE_2---------------------------------------------------*/ 
	/*44*/	{RINSE3_ADDITIVES,		IDLE,				DUTY_M3,		CP_OFF},		//34
	/*45*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF},
	/*46*/	{HOTRINSE,			TWENTY_MINUTES,			DUTY_C27,		CP_OFF},	//36
	/*47*/	{RINSE_HOLD,			IDLE,				NO,			CP_OFF},	//37
	/*48*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF},	//38

		/*----------------------------------------STEAM-------------------------------------------------------*/
	/*49*/	{STEAM_FILL,			ONE_MINUTES,			DUTY_IDLE,		CP_OFF},	//39
	/*50*/	{STEAM,				TWENTY_ONE_MINUTES,		DUTY_ST1,		CP_OFF},	//40
	/*51*/	{STEAM_CREASE,			TWO_MINUTES,			DUTY_ST1,		CP_OFF},	//41
	/*52*/	{STEAM_DRAIN,			ONE_MINUTES,			DUTY_ST1,		CP_OFF},	//42

		/*---------------------------------------FINAL SPIN----------------------------------------------------*/
	/*53*/	{SPIN,				ELEVEN_MINUTES,			SPIN_PROFILE_FS16,	CP_OFF},	//43
	/*54*/	{ANTI_CREASE,			TWO_MINUTES,			DUTY_C17,		CP_OFF},	//44
		

};
			
//***************************************************************************************************************************************************************************//
//*******************************************************************************************************************************************************************//			

//*************************************************** COTTON 60-95 Program **************************************************************************************************//			
//ref. original
//Operation cotton_60_90[C_60_90_LENGTH]= {
//	/*0*/	{DELAYSTART,			IDLE,				NO,			CP_NONE}, //0
//	/*1*/	{INITIAL_DRAIN,			ONE_MINUTES,			NO,			CP_NONE}, //1 
//		
//		/*-------------------------------PREWASH-------------------------------------------------------*/
//	/*2*/	{OPTIONAL_PREWASH,		FOUR_MINUTES,			DUTY_C30,		CP_OFF}, //2
//	/*3*/	{IDLE_STAGE,				IDLE,				IDLE,			CP_OFF}, //3	//This step won't be executed it has beed adjusted in previous state.
//	/*4*/	{PREWASH_TUMBLING,		THREE_MINUTES,			DUTY_C31,		CP_ON},  //4
//	/*5*/	{PREWASH_TUMBLING,		SIX_MINUTES,			DUTY_C23,		CP_OFF}, //5 30 sec drain
//	/*6*/	{PREWASH_SPIN,			FOUR_MINUTES,			SPIN_PROFILE_IS2,	CP_OFF}, //6 
//		
//		/*-------------------------------MAINWASH-----------------------------------------------------*/
//	/*7*/	{FILLING,			FOUR_MINUTES,			DUTY_C30,		CP_OFF}, //7
//	/*8*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF}, //8 //This step won't be executed it has beed adjusted in previous state.
//	/*9*/	{TUMBLING,			ONE_MINUTES,			DUTY_C31,		CP_ON},  //9
//	/*10*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X1,		CP_ON},  //10
//	/*11*/	{TUMBLING,			IDLE,				DUTY_C23,		CP_ON},	 //11
//	/*12*/	{SOAK,				IDLE,				DUTY_SOAK,		CP_NONE},//12
//	/*13*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X2,		CP_ON},  //13
//	/*14*/	{HEATING,			IDLE,				DUTY_C6,		CP_1MinON_1MinOFF}, //14
//		
//		//ReHeat - Tx Step
//	/*15*/	{TUMBLING,			ONE_MINUTES,			DUTY_C22,		CP_NONE}, //15
//	/*16*/	{REHEAT,			ONE_MINUTES,			DUTY_C22,		CP_NONE}, //16
//	/*17*/	{TUMBLING,			ONE_MINUTES,			DUTY_C22,		CP_NONE}, //17
//	/*18*/	{REHEAT,			ONE_MINUTES,			DUTY_C22,		CP_NONE}, //18
//	/*19*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X3,		CP_ON}, //19
//	/*20*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X4,		CP_ON},   //20
//	/*21*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X5,		CP_ON},	  //21
//	/*22*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X6,		CP_ON},   //22
//	/*23*/	{TUMBLING_TX,			IDLE,				DUTY_C28,		CP10MinON_2MinOFF}, //23		
//	/*24*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X7,		CP_ON},  //24
//	/*25*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X8,		CP_ON},  //25
//	/*26*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X9,		CP_ON},  //26
//	/*27*/	{TUMBLING,			IDLE,				DUTY_C23,		CP_OFF}, //27 30Sec drain last
//	/*28*/	{COOL_DOWN,			SIX_MINUTES,			DUTY_D1,		CP_OFF}, //28
//	/*29*/	{MAINWASH_INTERMIDIATE_SPIN,	SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF},//29
//		
//		/*-------------------------------OPTIONAL EXTRA RINSE_1-----------------------------------------*/
//	/*30*/	{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_C25,		CP_ON}, //30 Last 30 sec drain
//	/*31*/	{IDLE_STAGE,				IDLE,				IDLE,			CP_OFF}, //31 
//	/*32*/	{OPTIONAL_SPIN,			SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF}, //32
//	
//		/*-------------------------------OPTIONAL EXTRA RINSE_2-----------------------------------------*/
//	/*33*/	{OPTION_EXTRA_RINSE_MAINWASH,	FOUR_MINUTES,			DUTY_C25,		CP_ON}, //33 Last 30 sec drain
//	/*34*/	{IDLE_STAGE,				IDLE,				IDLE,			CP_OFF}, //34 
//	/*35*/	{OPTIONAL_SPIN,			SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF}, //35
//		
//		/*-------------------------------OPTIONAL EXTRA RINSE_3-----------------------------------------*/
//	/*36*/	{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_C25,		CP_ON}, //36 Last 30 sec drain
//	/*37*/	{IDLE_STAGE,				IDLE,				IDLE,			CP_OFF}, //37 
//	/*38*/	{OPTIONAL_SPIN,			SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF}, //38
//		
//		/*----------------------------------------RINSE_1----------------------------------------------*/ 
//	/*39*/	{RINSE_PREWASH_FILL,		FIVE_MINUTES,			DUTY_C25,		CP_ON}, //39
//	/*40*/	{IDLE_STAGE,				IDLE,				IDLE,			CP_OFF},  //40
//	/*41*/	{RINSE_HALFDRAIN,		ONE_MINUTES,			NO,			CP_OFF}, //41
//	/*42*/	{RINSE_TUMBLING,		ONE_MINUTES,			DUTY_C12,		CP_OFF}, //42
//	/*43*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF}, //43
//	/*44*/	{RINSE_INTERMIDIATE_SPIN,	SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF}, //44 
//	
//	/*----------------------------------------RINSE_2----------------------------------------------*/ 
//	/*45*/	{RINSE_MAINWASH_FILL,		IDLE,				DUTY_C25,		CP_ON}, //45
//	/*46*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF},  //46
//	/*47*/	{RINSE_HALFDRAIN,		ONE_MINUTES,			NO,			CP_OFF}, //47
//	/*48*/	{RINSE_TUMBLING,		ONE_MINUTES,			DUTY_C12,		CP_OFF}, //48
//	/*49*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF}, //49
//	/*50*/	{RINSE_INTERMIDIATE_SPIN,	SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF}, //50 
//		
//		/*----------------------------------------RINSE_3----------------------------------------------*/ 
//	/*51*/	{RINSE3_ADDITIVES,		IDLE,				DUTY_C25,		CP_ON}, //45 Last 30 sec
//	/*52*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF}, //46 
//	/*53*/	{HOTRINSE,			TWENTY_MINUTES,			DUTY_C25,		CP_NONE},//47 
//	/*54*/	{RINSE_HOLD,			IDLE,				NO,			CP_OFF},//48 
//	/*55*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF}, //49
//
//		/*----------------------------------------------STEAM---------------------------------------------*/
//	/*56*/	{STEAM_FILL,			ONE_MINUTES,			DUTY_IDLE,		CP_OFF}, //50
//	/*57*/	{STEAM,				TWENTY_ONE_MINUTES,		DUTY_ST1,		CP_OFF}, //51
//	/*58*/	{STEAM_CREASE,			TWO_MINUTES,			DUTY_ST1,		CP_OFF}, //52
//	/*59*/	{STEAM_DRAIN,			ONE_MINUTES,			DUTY_ST1,		CP_OFF},  //53
//
//		/*-------------------------------------------FINAL SPIN-------------------------------------------*/
//	/*60*/	{SPIN,				FOURTEEN_MINUTES,		SPIN_PROFILE_FS2,	CP_OFF}, //54
//	/*61*/	{ANTI_CREASE,			TWO_MINUTES,				DUTY_C17,		CP_OFF}, //55
//		
//
//};

//---------------------------------------------------NEO 8KG CHANGES-------------------------------------------------------//
Operation cotton_60_90[C_60_90_LENGTH]= {
	/*0*/	{DELAYSTART,			IDLE,				NO,			CP_NONE}, //0
	/*1*/	{INITIAL_DRAIN,			ONE_MINUTES,			NO,			CP_NONE}, //1 
		
		/*-------------------------------PREWASH-------------------------------------------------------*/
	/*2*/	{OPTIONAL_PREWASH,		FOUR_MINUTES,			DUTY_C30,		CP_OFF}, //2
	/*3*/	{PREWASH_TUMBLING,		THREE_MINUTES,			DUTY_C31,		CP_OFF},  //4	//This step won't be executed it has beed adjusted in previous state.
	/*4*/	{PREWASH_TUMBLING,		SIX_MINUTES,			DUTY_C23,		CP_OFF}, //5 30 sec drain
	/*5*/	{DRAIN_PREWASH,			ONE_MINUTES,			NO,			CP_OFF}, //5 30 sec drain
	/*6*/	{PREWASH_SPIN,			FOUR_MINUTES,			SPIN_PROFILE_IS2,	CP_OFF}, //6 
		
		/*-------------------------------MAINWASH-----------------------------------------------------*/
	/*7*/	{FILLING,			FOUR_MINUTES,			DUTY_C30,		CP_OFF}, //7
	/*8*/	{TUMBLING,			THREE_MINUTES,			DUTY_X1,		CP_OFF}, //8 //This step won't be executed it has beed adjusted in previous state.
	/*9*/	{TUMBLING,			ONE_MINUTES,			DUTY_PMX,		CP_OFF}, //9
	/*10*/	{TUMBLING,			FIVE_MINUTES,			DUTY_X2,		CP_OFF}, //10
	/*11*/	{TUMBLING,			ONE_MINUTES,			DUTY_PMX,		CP_OFF}, //11
	/*12*/	{TUMBLING,			TWO_MINUTES,			DUTY_C32,		CP_OFF}, //12
	/*13*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X3,		CP_OFF}, 
	/*14*/	{TUMBLING,			ONE_MINUTES,			DUTY_X2,		CP_OFF}, 
	/*15*/	{SOAK,				IDLE,				DUTY_SOAK,		CP_NONE},//12
	/*16*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X4,		CP_OFF},  //13
	
		//-----HEAT-----//
	/*17*/	{HEATING,			IDLE,				DUTY_C6,		CP_OFF}, //14
		
		//----ReHeat----//---SKIP 
	/*18*/	{TUMBLING,			ONE_MINUTES,			DUTY_C22,		CP_OFF}, //15
	/*19*/	{REHEAT,			ONE_MINUTES,			DUTY_C22,		CP_OFF}, //16
	/*20*/	{TUMBLING,			ONE_MINUTES,			DUTY_C22,		CP_OFF}, //17
	/*21*/	{REHEAT,			ONE_MINUTES,			DUTY_C22,		CP_OFF}, //18
	
	
	/*22*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X5,		CP_OFF}, //19
	/*23*/	{TUMBLING,			ONE_MINUTES,			DUTY_C23,		CP_OFF},   //20
	/*24*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X6,		CP_OFF},	  //21
	
		//--------TX STEP-----//
	/*25*/	{TUMBLING_TX,			IDLE,				DUTY_C23,		CP_OFF}, //23
	
	/*26*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X7,		CP_OFF},  //24
	/*27*/	{TUMBLING,			ONE_MINUTES,			DUTY_C23,		CP_OFF},   //20
	/*28*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X8,		CP_OFF},  //25
	/*29*/	{TUMBLING,			ONE_MINUTES,			DUTY_C23,		CP_OFF},   //20
	/*30*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X9,		CP_OFF},  //26
	/*31*/	{COOL_DOWN,			SIX_MINUTES,			DUTY_D1,		CP_OFF}, //28
	/*32*/	{DRAIN,				ONE_MINUTES,			NO,			CP_OFF},
	/*33*/	{MAINWASH_INTERMIDIATE_SPIN,	SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF},//29
		
		/*-------------------------------OPTIONAL EXTRA RINSE_1-----------------------------------------*/
	/*34*/	{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_C25,		CP_OFF}, //30 Last 30 sec drain
	/*35*/	{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_OFF}, //31 
	/*36*/	{OPTIONAL_SPIN,			SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF}, //32
	
		/*-------------------------------OPTIONAL EXTRA RINSE_2-----------------------------------------*/
	/*37*/	{OPTION_EXTRA_RINSE_MAINWASH,	FOUR_MINUTES,			DUTY_C25,		CP_OFF}, //33 Last 30 sec drain
	/*38*/	{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_OFF}, //34 
	/*39*/	{OPTIONAL_SPIN,			SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF}, //35
		
		/*-------------------------------OPTIONAL EXTRA RINSE_3-----------------------------------------*/
	/*40*/	{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_C25,		CP_OFF}, //36 Last 30 sec drain
	/*41*/	{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_OFF}, //37 
	/*42*/	{OPTIONAL_SPIN,			SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF}, //38
		
		/*----------------------------------------RINSE_1----------------------------------------------*/ 
	/*43*/	{RINSE_PREWASH_FILL,		SIX_MINUTES,			DUTY_C25,		CP_OFF}, //39
	/*44*/	{RINSE_HALFDRAIN,		ONE_MINUTES,			NO,			CP_OFF}, //41
	/*45*/	{RINSE_TUMBLING,		ONE_MINUTES,			DUTY_C12,		CP_OFF}, //42
	/*46*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF}, //43
	/*47*/	{RINSE_INTERMIDIATE_SPIN,	SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF}, //44 
	
	/*----------------------------------------RINSE_2----------------------------------------------*/ 
	/*48*/	{RINSE_MAINWASH_FILL,		IDLE,				DUTY_C25,		CP_OFF}, //45
	/*49*/	{RINSE_HALFDRAIN,		ONE_MINUTES,			NO,			CP_OFF}, //47
	/*50*/	{RINSE_TUMBLING,		ONE_MINUTES,			DUTY_C12,		CP_OFF}, //48
	/*51*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF}, //49
	/*52*/	{RINSE_INTERMIDIATE_SPIN,	SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF}, //50 
		
		/*----------------------------------------RINSE_3----------------------------------------------*/ 
	/*53*/	{RINSE3_ADDITIVES,		IDLE,				DUTY_C25,		CP_OFF}, //45 Last 30 sec
	/*54*/	{HOTRINSE,			TWENTY_MINUTES,			DUTY_C25,		CP_NONE},//47 
	/*55*/	{RINSE_HOLD,			IDLE,				NO,			CP_OFF},//48 
	/*56*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF}, //49

		/*----------------------------------------------STEAM---------------------------------------------*/
	/*57*/	{STEAM_FILL,			ONE_MINUTES,			DUTY_IDLE,		CP_OFF}, //50
	/*58*/	{STEAM,				TWENTY_ONE_MINUTES,		DUTY_ST1,		CP_OFF}, //51
	/*59*/	{STEAM_CREASE,			TWO_MINUTES,			DUTY_ST1,		CP_OFF}, //52
	/*60*/	{STEAM_DRAIN,			ONE_MINUTES,			DUTY_ST1,		CP_OFF},  //53
		/*-------------------------------------------FINAL SPIN-------------------------------------------*/
	/*61*/	{SPIN,				TEN_MINUTES,			SPIN_PROFILE_FS2,	CP_OFF}, //54
	/*62*/	{ANTI_CREASE,			TWO_MINUTES,			DUTY_C17,		CP_OFF}, //55
	

};
//***********************************************************************************************************************************************//
//***********************************************************************************************************************************************//
//********************************************** COTTON COLD-30-40 Program *******************************************************************************//
//Operation cotton[C_LENGTH]= {
//	/*0*/	{DELAYSTART,			IDLE,				NO,			CP_NONE}, 
//	/*1*/	{INITIAL_DRAIN,			ONE_MINUTES,			NO,			CP_NONE}, 
//		/*--------------------------------Load Sensing Phase ------------------------------------------*/
//		//{Load_Sensing,                  ONE_MINUTES,                    SPIN_PROFILE_LS,         CP_NONE},
//		/*-------------------------------PREWASH-------------------------------------------------------*/
//	/*2*/	{OPTIONAL_PREWASH,		FOUR_MINUTES,			DUTY_C30,		CP_OFF}, 
//	/*3*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF},  //This step won't be executed it has beed adjusted in previous state.
//	/*4*/	{PREWASH_TUMBLING,		THREE_MINUTES,			DUTY_C31,		CP_ON},  
//	/*5*/	{PREWASH_TUMBLING,		SIX_MINUTES,			DUTY_C23,		CP_OFF}, // last 30 sec 
//	/*6*/	{PREWASH_SPIN,			FOUR_MINUTES,			SPIN_PROFILE_IS2,	CP_OFF},  
//		
//		/*-------------------------------MAINWASH-----------------------------------------------------*/
//	/*7*/	{FILLING,			FOUR_MINUTES,			DUTY_C30,		CP_OFF}, 
//	/*8*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF}, //This step won't be executed it has beed adjusted in previous state.
//	/*9*/	{TUMBLING,			TWO_MINUTES,			DUTY_C31,		CP_ON},  
//	/*10*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X1,		CP_ON},  
//	/*11*/	{TUMBLING,			FOUR_MINUTES,			DUTY_C23,		CP_ON},	 
//	/*12*/	{SOAK,				IDLE,				DUTY_SOAK,		CP_NONE},
//	/*13*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X2,		CP_ON},  //13
//	/*14*/	{HEATING,			IDLE,				DUTY_C6,		CP_1MinON_1MinOFF}, //14
//		
//		//ReHeat - Tx Step
//	/*15*/	{TUMBLING,			ONE_MINUTES,			DUTY_C22,		CP_NONE}, //15
//	/*16*/	{REHEAT,			ONE_MINUTES,			DUTY_C22,		CP_NONE}, //16
//	/*17*/	{TUMBLING,			ONE_MINUTES,			DUTY_C22,		CP_NONE}, //17
//	/*18*/	{REHEAT,			ONE_MINUTES,			DUTY_C22,		CP_NONE}, //18
//	/*19*/	{TUMBLING,			ONE_MINUTES,			DUTY_C22,		CP_NONE}, //19
//	/*20*/	{REHEAT,			ONE_MINUTES,			DUTY_C22,		CP_NONE}, //20
//	/*21*/	{TUMBLING,			ONE_MINUTES,			DUTY_C22,		CP_NONE}, //21
//	/*22*/	{REHEAT,			ONE_MINUTES,			DUTY_C22,		CP_NONE}, //22
//	/*23*/	{TUMBLING,			ONE_MINUTES,			DUTY_C22,		CP_NONE}, //23
//	/*24*/	{REHEAT,			ONE_MINUTES,			DUTY_C22,		CP_NONE}, //24
//	/*25*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X3,		CP_ON}, //25
//	/*26*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X4,		CP_ON},   //26
//	/*27*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X5,		CP_ON},	  //27
//	/*28*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X6,		CP_ON},   //28
//	/*29*/	{TUMBLING_TX,			IDLE,				DUTY_C28,		CP10MinON_2MinOFF}, //29 TX updattion 
//	/*30*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X7,		CP_ON},  //30
//	/*31*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X8,		CP_ON},  //31
//	/*32*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X9,		CP_ON},  //32
//	/*33*/	{TUMBLING,			IDLE,				DUTY_C23,		CP_OFF}, //33 30Sec drain last
//	/*34*/	{COOL_DOWN,			SIX_MINUTES,			DUTY_D1,		CP_OFF}, //34
//	/*35*/	{MAINWASH_INTERMIDIATE_SPIN,	SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_NONE},//35
//		
//		/*-------------------------------OPTIONAL EXTRA RINSE_1-----------------------------------------*/
//	/*36*/	{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_C25,		CP_ON}, //36 Last 30 sec drain
//	/*37*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF}, //37 
//	/*38*/	{OPTIONAL_SPIN,			SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF}, //38
//		/*-------------------------------OPTIONAL EXTRA RINSE_2-----------------------------------------*/
//	/*39*/	{OPTION_EXTRA_RINSE_MAINWASH,	FOUR_MINUTES,			DUTY_C25,		CP_ON}, //39 Last 30 sec drain
//	/*40*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF}, //40 
//	/*41*/	{OPTIONAL_SPIN,			SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF}, //41
//		/*-------------------------------OPTIONAL EXTRA RINSE_3-----------------------------------------*/
//	/*42*/	{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_C25,		CP_ON}, //42 Last 30 sec drain
//	/*43*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF}, //43 
//	/*44*/	{OPTIONAL_SPIN,			SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF}, //44
//
//		/*----------------------------------------RINSE_1----------------------------------------------*/ 
//	/*45*/	{RINSE_PREWASH_FILL,		FIVE_MINUTES,			DUTY_C25,		CP_ON}, //45
//	/*46*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF},  //46
//	/*47*/	{RINSE_HALFDRAIN,		ONE_MINUTES,			NO,			CP_OFF}, //47
//	/*48*/	{RINSE_TUMBLING,		ONE_MINUTES,			DUTY_C12,		CP_OFF}, //48
//	/*49*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF}, //49
//	/*50*/	{RINSE_INTERMIDIATE_SPIN,	SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF}, //50 
//	
//	
//		/*----------------------------------------RINSE_2----------------------------------------------*/ 
//	/*51*/	{RINSE_MAINWASH_FILL,		IDLE,				DUTY_C25,		CP_ON}, //45
//	/*52*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF},  //46
//	/*53*/	{RINSE_HALFDRAIN,		ONE_MINUTES,			NO,			CP_OFF}, //47
//	/*54*/	{RINSE_TUMBLING,		ONE_MINUTES,			DUTY_C12,		CP_OFF}, //48
//	/*55*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF}, //49
//	/*56*/	{RINSE_INTERMIDIATE_SPIN,	SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF}, //50 
//		
//		/*----------------------------------------RINSE_3----------------------------------------------*/ 
//	/*57*/	{RINSE3_ADDITIVES,		IDLE,				DUTY_C25,		CP_ON}, //51
//	/*58*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF},  //52 Last 30 sec
//	/*59*/	{HOTRINSE,			TWENTY_MINUTES,			DUTY_C25,		CP_NONE},//53 Last 30 sec
//	/*60*/	{RINSE_HOLD,			IDLE,				NO,			CP_NONE},//54 
//	/*61*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF}, //55
//		
//		/*----------------------------------------------STEAM---------------------------------------------*/
//	/*62*/	{STEAM_FILL,			ONE_MINUTES,			DUTY_IDLE,		CP_NONE}, //56
//	/*63*/	{STEAM,				TWENTY_ONE_MINUTES,		DUTY_ST1,		CP_NONE}, //57
//	/*64*/	{STEAM_CREASE,			TWO_MINUTES,			DUTY_ST1,		CP_NONE}, //58
//	/*65*/	{STEAM_DRAIN,			ONE_MINUTES,			DUTY_ST1,		CP_NONE},  //59
//
//		/*-------------------------------------------FINAL SPIN-------------------------------------------*/
//	/*66*/	{SPIN,				FOURTEEN_MINUTES,		SPIN_PROFILE_FS2,	CP_OFF}, //60
//	/*67*/	{ANTI_CREASE,			TWO_MINUTES,			DUTY_C17,		CP_OFF}, //61
//};

//__________________________________________________COTTON COLD-30-40 Program_________________________________________________________________//
//--------------------------------------------------------------NEO 8KG CHANGES---------------------------------------------------------------------------------------//
Operation cotton[C_LENGTH]= {
	/*0*/	{DELAYSTART,			IDLE,				NO,			CP_OFF}, //0
	/*1*/	{INITIAL_DRAIN,			ONE_MINUTES,			NO,			CP_OFF}, //1 
		
		/*----------------------------------------------------PREWASH-----------------------------------------------------------------------------------------*/
	/*2*/	{OPTIONAL_PREWASH,		FOUR_MINUTES,			DUTY_C30,		CP_OFF}, //2
	/*3*/	{IDLE_STAGE,			IDLE,				IDLE,			CP_OFF},
	/*4*/	{PREWASH_TUMBLING,		THREE_MINUTES,			DUTY_C31,		CP_OFF},  //4	//This step won't be executed it has beed adjusted in previous state.
	/*5*/	{PREWASH_TUMBLING,		SIX_MINUTES,			DUTY_C23,		CP_OFF}, //5 30 sec drain
	/*6*/	{DRAIN_PREWASH,			ONE_MINUTES,			NO,			CP_OFF}, //5 30 sec drain
	/*7*/	{PREWASH_SPIN,			FOUR_MINUTES,			SPIN_PROFILE_IS2,	CP_OFF}, //6 
		
		/*----------------------------------------------------MAINWASH---------------------------------------------------------------------------------------*/
	/*8*/	{FILLING,			FOUR_MINUTES,			DUTY_C30,		CP_OFF}, //7
	/*9*/	{TUMBLING,			IDLE,				DUTY_X1,		CP_OFF}, //8 //This step won't be executed it has beed adjusted in previous state.
	/*10*/	{TUMBLING,			ONE_MINUTES,			DUTY_PMX,		CP_OFF}, //9
	/*11*/	{TUMBLING,			IDLE,				DUTY_X2,		CP_OFF}, //10
	/*12*/	{TUMBLING,			ONE_MINUTES,			DUTY_PMX,		CP_OFF}, //11
	/*13*/	{TUMBLING,			TWO_MINUTES,			DUTY_C32,		CP_OFF}, //12
	/*14*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X3,		CP_OFF}, 
	/*15*/	{TUMBLING,			ONE_MINUTES,			DUTY_X2,		CP_OFF}, 
	/*16*/	{SOAK,				IDLE,				DUTY_SOAK,		CP_OFF},//12
	/*17*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X4,		CP_OFF},  //13
	
		//--------------------------------------------------------HEAT-----------------------------------------------------------------------------------//
	/*18*/	{HEATING,			IDLE,				DUTY_C6,		CP_OFF}, //14
		
		//-------------------------------------------------------ReHeat----------------------------------------------------------------------------------//---SKIP 
	/*19*/	{TUMBLING,			ONE_MINUTES,			DUTY_C22,		CP_OFF}, //15
	/*20*/	{REHEAT,			ONE_MINUTES,			DUTY_C22,		CP_OFF}, //16
	/*21*/	{TUMBLING,			ONE_MINUTES,			DUTY_C22,		CP_OFF}, //17
	/*22*/	{REHEAT,			ONE_MINUTES,			DUTY_C22,		CP_OFF}, //18
	/*23*/	{TUMBLING,			ONE_MINUTES,			DUTY_C22,		CP_OFF}, //15
	/*24*/	{REHEAT,			ONE_MINUTES,			DUTY_C22,		CP_OFF}, //16
	/*25*/	{TUMBLING,			ONE_MINUTES,			DUTY_C22,		CP_OFF}, //17
	/*26*/	{REHEAT,			ONE_MINUTES,			DUTY_C22,		CP_OFF}, //18
	/*27*/	{TUMBLING,			ONE_MINUTES,			DUTY_C22,		CP_OFF}, //15
	/*28*/	{REHEAT,			ONE_MINUTES,			DUTY_C22,		CP_OFF}, //16
	
	
	/*29*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X5,		CP_OFF}, //19
	/*30*/	{TUMBLING,			ONE_MINUTES,			DUTY_C23,		CP_OFF},   //20
	/*31*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X6,		CP_OFF},	  //21
	
		//----------------------------------------------------------TX STEP-----------------------------------------------------------------------------//
	/*32*/	{TUMBLING_TX,			IDLE,				DUTY_C23,		CP_OFF}, //23
	
	/*33*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X7,		CP_OFF},  //24
	/*34*/	{TUMBLING,			ONE_MINUTES,			DUTY_C23,		CP_OFF},   //20
	/*35*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X8,		CP_OFF},  //25
	/*36*/	{TUMBLING,			ONE_MINUTES,			DUTY_C23,		CP_OFF},   //20
	/*37*/	{TUMBLINGSPECIAL,		ONE_MINUTES,			DUTY_X9,		CP_OFF},  //26
	/*38*/	{COOL_DOWN,			SIX_MINUTES,			DUTY_D1,		CP_OFF}, //28
	/*39*/	{DRAIN,				ONE_MINUTES,			NO,			CP_OFF},
	/*40*/	{MAINWASH_INTERMIDIATE_SPIN,	SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF},//29
		
		/*-------------------------------OPTIONAL EXTRA RINSE_1-----------------------------------------*/
	/*41*/	{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_C25,		CP_OFF}, //30 Last 30 sec drain
	/*42*/	{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_OFF}, //31 
	/*43*/	{OPTIONAL_SPIN,			SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF}, //32
	
		/*-------------------------------OPTIONAL EXTRA RINSE_2-----------------------------------------*/
	/*44*/	{OPTION_EXTRA_RINSE_MAINWASH,	FOUR_MINUTES,			DUTY_C25,		CP_OFF}, //33 Last 30 sec drain
	/*45*/	{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_OFF}, //34 
	/*46*/	{OPTIONAL_SPIN,			SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF}, //35
		
		/*-------------------------------OPTIONAL EXTRA RINSE_3-----------------------------------------*/
	/*47*/	{OPTION_EXTRA_RINSE_PREWASH,	FOUR_MINUTES,			DUTY_C25,		CP_OFF}, //36 Last 30 sec drain
	/*48*/	{OPTIONAL_DRAIN,		ONE_MINUTES,			NO,			CP_OFF}, //37 
	/*49*/	{OPTIONAL_SPIN,			SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF}, //38
		
		/*----------------------------------------RINSE_1----------------------------------------------*/ 
	/*50*/	{RINSE_PREWASH_FILL,		FIVE_MINUTES,			DUTY_C25,		CP_OFF}, //39
	/*51*/	{RINSE_HALFDRAIN,		ONE_MINUTES,			NO,			CP_OFF}, //41
	/*52*/	{RINSE_TUMBLING,		ONE_MINUTES,			DUTY_C12,		CP_OFF}, //42
	/*53*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF}, //43
	/*54*/	{RINSE_INTERMIDIATE_SPIN,	SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF}, //44 
	
		/*----------------------------------------RINSE_2----------------------------------------------*/ 
	/*55*/	{RINSE_MAINWASH_FILL,		IDLE,				DUTY_C25,		CP_OFF}, //45
	/*56*/	{RINSE_HALFDRAIN,		ONE_MINUTES,			NO,			CP_OFF}, //47
	/*57*/	{RINSE_TUMBLING,		ONE_MINUTES,			DUTY_C12,		CP_OFF}, //48
	/*58*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF}, //49
	/*59*/	{RINSE_INTERMIDIATE_SPIN,	SIX_MINUTES,			SPIN_PROFILE_IS1,	CP_OFF}, //50 
		
		/*----------------------------------------RINSE_3----------------------------------------------*/ 
	/*60*/	{RINSE3_ADDITIVES,		IDLE,				DUTY_C25,		CP_OFF}, //45 Last 30 sec
	/*61*/	{HOTRINSE,			TWENTY_MINUTES,			DUTY_C25,		CP_NONE},//47 
	/*62*/	{RINSE_HOLD,			IDLE,				NO,			CP_OFF},//48 
	/*63*/	{RINSE_DRAIN,			ONE_MINUTES,			NO,			CP_OFF}, //49

		/*----------------------------------------------STEAM---------------------------------------------*/
	/*64*/	{STEAM_FILL,			ONE_MINUTES,			DUTY_IDLE,		CP_OFF}, //50
	/*65*/	{STEAM,				TWENTY_ONE_MINUTES,		DUTY_ST1,		CP_OFF}, //51
	/*66*/	{STEAM_CREASE,			TWO_MINUTES,			DUTY_ST1,		CP_OFF}, //52
	/*67*/	{STEAM_DRAIN,			ONE_MINUTES,			DUTY_ST1,		CP_OFF},  //53
		/*-------------------------------------------FINAL SPIN-------------------------------------------*/
	/*68*/	{SPIN,				TEN_MINUTES,			SPIN_PROFILE_FS2,	CP_OFF}, //54
	/*69*/	{ANTI_CREASE,			TWO_MINUTES,			DUTY_C17,		CP_OFF}, //55
	

};			
//*********************************************************************************************************************************************//
//********************************************* WOOLLENS Program ******************************************************************************//	

Operation Wool[WOOL_LENGTH]= {
		{DELAYSTART,			IDLE,				DUTY_IDLE,		CP_NONE},
		{DRAIN,				ONE_MINUTES,			NO,			CP_NONE}, //1-2
		
		//MAINWASH
		{FILLING,			THREE_MINUTES,			DUTY_RWOOL,		CP_NONE},
		{HEAT,				IDLE,				DUTY_RWOOL,		CP_NONE}, 
		{TUMBLING,			IDLE,				DUTY_RWOOL,		CP_NONE}, //3-5
		{COOL_DOWN,			SIX_MINUTES,			DUTY_D1,		CP_NONE},
		{DRAIN,				TWO_MINUTES,			NO,			CP_NONE}, //6-7
		
		//OPTIONAL RINSE
		{OPTION_EXTRA_RINSE_MAINWASH,	FIVE_MINUTES,			DUTY_RWOOL,		CP_NONE}, 
		{OPTIONAL_DRAIN,		TWO_MINUTES,			NO,			CP_NONE}, //8-9
		
		//RINSE
		{RINSE_PREWASH_FILL,		FIVE_MINUTES,			DUTY_RWOOL,		CP_NONE}, 
		{RINSE_DRAIN,			TWO_MINUTES,			NO,			CP_NONE},//10-11
		{RINSE3_ADDITIVES,		FIVE_MINUTES,			DUTY_RWOOL,		CP_NONE},
		{RINSE_HOLD,			IDLE,				NO,			CP_NONE},
		{DRAIN_TUMBLE,			TWO_MINUTES,			DUTY_RWOOL,		CP_NONE},//12-13
		
		//FINAL SPIN
		{SPIN,				SIX_MINUTES,			SPIN_PROFILE_FS5,	CP_NONE},
		{ANTI_CREASE,			TWO_MINUTES,			DUTY_CWOOL,		CP_NONE}
};//14-15
			
			
//***************************************************************************************************************************************//
//***************************************** TubClean Program **********************************************************************//				 
Operation tubClean[TUB_LENGTH] = {
		{DELAYSTART,			IDLE,				IDLE,		CP_NONE},
		{DRAIN,				ONE_MINUTES,			NO,		CP_NONE},//1-2

		//MAINWASH
		{FILLING,			THREE_MINUTES,			DUTY_C2,	CP_NONE},
		{HEAT,				IDLE,				DUTY_C15,	CP_NONE},
		{TUMBLING,			IDLE,				IDLE,		CP_NONE},
		{TUMBLING,			IDLE,				IDLE,		CP_NONE},//3-6
		{REHEAT,			IDLE,				DUTY_C15,	CP_NONE},
		{TUMBLING,			THREE_MINUTES,			DUTY_C13,	CP_NONE},
		{TUMBLING,			THREE_MINUTES,			DUTY_C14,	CP_NONE}, //7-9
		{COOL_DOWN,			SIX_MINUTES,			DUTY_D1,	CP_NONE},
		{MAIN_TUMBLE,			ONE_MINUTES,			DUTY_C2,	CP_NONE},// 10-11
		
		//RINSE
		{RINSE_PREWASH_FILL,		FOUR_MINUTES,			DUTY_C2,	CP_NONE},
		{RINSE_TUMBLING,		TWO_MINUTES,			DUTY_C11,	CP_NONE},
		{RINSE_TUMBLING,		TWO_MINUTES,			DUTY_C12,	CP_NONE},//12-14
		{DRAIN_TUMBLE,			ONE_MINUTES,			DUTY_C2,	CP_NONE},
		{RINSE_MAINWASH_FILL,		FOUR_MINUTES,			DUTY_C2,	CP_NONE},
		{RINSE_TUMBLING,		TWO_MINUTES,			DUTY_C11,	CP_NONE},//15-17
		{RINSE_TUMBLING,		TWO_MINUTES,			DUTY_C12,	CP_NONE},
		{DRAIN_TUMBLE,			ONE_MINUTES,			DUTY_C2,	CP_NONE},//18-19			
		{RINSE3_ADDITIVES,		FOUR_MINUTES,			DUTY_C2,	CP_NONE},
		{RINSE_HOLD,			IDLE,				NO,		CP_NONE},
		{DRAIN_TUMBLE,			ONE_MINUTES,			DUTY_C2,	CP_NONE}

};//20-22
				 
				 
//**********************************************************************************************************************************************************//
//**********************************************************************************************************************************************************//			
/*------------------------------------Water Freq Calculation-------------------------------*/

//const uint16_t MIXED_RinseWaterResetFreq_ui16[11] = {0, 39390,39390,39390,39150,39150,39150,39150,38840,38840,38840};//{39390,39150,38840,}; //Depend on load sense count
//				   	      	  /*NONE  1    2     3     4     5     6     7     8     9     10*/   
//
//const uint16_t COTTON_RinseWaterResetFreq_ui16[11] = {0, 40130,40130,40130,39800,39800,39800,39800,39600,39600,39600};//{40130,39800,39800,}; //Depend on load sense count
//				   	      	   /*NONE   1    2     3     4     5     6     7     8     9     10*/  	
//		
//const uint16_t SYNTHETIC_RinseWaterResetFreq_ui16[11] = {0, 39390,39390,39390,39150,39150,39150,39150,38840,38840,38840};//{39380,39150,38840}; //Depend on load sense count
//				   	      	     /*NONE   1    2     3     4     5     6     7     8     9     10*/ 	

const uint16_t MIXED_RinseWaterResetFreq_ui16[4] = {39399, 39159, 38849, 38849}; //Depend on load sense count
						/*    0	     1	    2	   3	*/	
						
const uint16_t COTTON_RinseWaterResetFreq_ui16[4] = {40139, 40139, 39809, 39609}; //Depend on load sense count
						/*    0	     1	    2	   3	*/
						
const uint16_t SYNTHETIC_RinseWaterResetFreq_ui16[4] = {39399, 39159, 38849, 38849}; //Depend on load sense count
						    /*    0	 1	2	3*/
/*-----------------------------------------------------------------------------------------*/

boolean ProgramMixed();
boolean ProgramCottonEco60();
boolean ProgramCottonEco40();
boolean ProgramAdditivesRinse();
//boolean ProgramDrainSpin();
boolean ProgramWool30();
boolean ProgramExpress30();
boolean ProgramCotton();
boolean ProgramCotton_60_90();
boolean ProgramSportsWear();
boolean ProgramTubClean();
boolean ProgramCradleWash30();
boolean ProgramBabyCare();
boolean ProgramInnerWear();
boolean ProgramAntiAllergen();
boolean ProgramAdditivesEndurance();
boolean ProgramUniform();
boolean ProgramInnerWear();
boolean ProgramSteamFresh();
boolean ProgramSteamWash();
boolean Program_ExpressWash_30(void);
boolean ProgramWetclean();
boolean ProgramLinenwash();
static void Circulation_Check(void);
void Cotton_WashLevel(void);
void AI_Set_Reset_Frequency_Update(void);

#endif
