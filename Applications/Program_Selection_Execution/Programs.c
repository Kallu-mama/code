#include "Program.h"
#include "r_cg_userdefine.h"
#include "waterfill.h"
#include "AI_Comm_Global.h"


//****************** global Variables **********************//
extern boolean bSkip_R1,bRapidWashButton,bProgramStart,bDoorLockStatus,bStartButton;
extern boolean bProgram_Ended,bEco60Reset,bEcoLevelReached,bCirculation_Pump;
extern uint8_t uiPresent_Step,uiTumblingCount,uiExtraRinse_Press_Select;
extern uint8_t uiTemperature_Required,uiWaterResetCount,uiTumbleState,uiMachineState,uiPrewashWaterResetCount;
extern uint8_t uidisp_select_option;
extern uint8_t uiTempSel,AI_WaterFreq_State_ui8 ;
extern uint8_t uiRpmSel,uiStep_Minutes_Completed,uiNextStep_Start_Delay;
extern uint8_t uiProgramLast_step,uiHeatState;
extern uint16_t iWater_Washlevel,iRinselevel,iTumble_Waterlevel,BalanceTime;
boolean Foam_Removal_Enable;
extern boolean bMainWashValve,bPrewashValve,bReheatHeater;
uint8_t Foam_Removal_Complete = 0;
extern uint8_t AI_ExecState_ui8;
extern uint8_t uiFinalWt,uiLoadRange;
extern uint16_t uiCurrent_step_time;
//***************************************************//
//******** global function prototype ********//
extern void washing(Operation *);
extern void Delay_ms(uint16_t);


//**************************************************************************************************//
//********************************* WOOLLENS Program **********************************************//
boolean ProgramWool()
{
		
	static boolean result;	
	
	//***************************************************
	switch(uiTempSel)
	{
		case Wash30:
			uiTemperature_Required = TEMP_30;
			break;
		case Wash40:
			uiTemperature_Required = TEMP_40;
			break;
		default: break;
	}
	//***************************************************
	
	
	if(uiPresent_Step < WOOL_LENGTH)
	{
		uiProgramLast_step = WOOL_LENGTH;
		if((uiTempSel == WashCold)&& (uiPresent_Step == 3)) // skip the heating stage
		{
			uiTemperature_Required = NO;
			uiPresent_Step++;
		}
		if((uiPresent_Step == 4)&&(bStartButton == TRUE))
		{
			Circulation_Check();
		}
		washing(&Wool[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else 
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}
	if(uiWaterResetCount > 0)
	{
		iWater_Washlevel = 39369;
	}
	else
	{
		iWater_Washlevel = WOOL_WASHLEVEL;
	}
	iRinselevel = WOOL_RINSELEVEL;
	iTumble_Waterlevel = TUMBLE_SPECIAL;
	bSkip_R1 = YES;	
	return result;
}

//**************************************************************************************************************//
//********************************* COTTON COLD-30-40 Program **********************************************//
boolean ProgramCotton()
{		
	static boolean result;
	
	//***************************************************
	switch(uiTempSel)
	{
		case Wash30:
		case P_30:
			uiTemperature_Required = TEMP_30;
			break;
		case Wash40:
		case P_40:
			uiTemperature_Required = TEMP_40;
			break;
		default: break;
	}
	//***************************************************
	
	if(uiPresent_Step < C_LENGTH)
	{
		
		uiProgramLast_step = C_LENGTH;
		//Below step are skipped to eradicated the hang issue observed during the reliabilioty test
		if((uiPresent_Step == 3)
		//|| (uiPresent_Step == 8) 
//		|| (uiPresent_Step == 37)
//		|| (uiPresent_Step == 40)
//		|| (uiPresent_Step == 43) // skip the heating stage
//		|| (uiPresent_Step == 46)
//		|| (uiPresent_Step == 52)
//		|| (uiPresent_Step == 58)
		) // skip the heating stage
		{
			//uiTemperature_Required = NO;
			uiPresent_Step++;
		}
		
		/*Rinse1 skip logic for 1-7kg load*/
		//if((uidisp_select_option & Prewash_Yes) == Prewash_Yes)
		//{
			
		//	if((uiFinalWt<=7) && (uiPresent_Step == 45))
		//	{
		//		uiPresent_Step = 51;
		//	}
		//}
		//else
//		if((uiFinalWt<=7) && (uiPresent_Step == 45))
//		{
//			uiPresent_Step = 51;
//		}

		//Rinse1 skip logic for 1-6kg load
//		if((uiLoadRange<=2)&&(uiPresent_Step == 45))
//		{
//			uiPresent_Step = 51;
//		}
		//if((uiLoadRange<=2)&&(uiPresent_Step == 49))
		if((uiLoadRange < 2)&&(uiPresent_Step == 50))
		{
			uiPresent_Step = 55;
		}

		washing(&cotton[uiPresent_Step]);
		//if(uiNextStep_Start_Delay == COMPLETED)
		{	
			AI_Execution();
		}
		//CirculationPump_Execution(&cotton[uiPresent_Step]);
		
		result = STEP_INPROGRESS;
	}
	else
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
		AI_WaterFreq_State_ui8 = 0;
	}
	/**************************************************AI_SET_RESET_FREQUENCY_UPDATE******************************************************/
	if(AI_ExecState_ui8 == AI_Runing)
	{
		AI_Set_Reset_Frequency_Update();
	}
	else
	{
		 Cotton_WashLevel(); 
	}
	/*************************************************************************************************************************************/
        iTumble_Waterlevel= TUMBLE_NORMAL;    
        //Cotton_WashLevel();  
//	iRinselevel = ((uidisp_select_option & Prewash_Yes) == Prewash_Yes)? COTTON_RinseWaterResetFreq_ui16[COTTON_RESET_LIMIT(uiFinalWt)]: //BTRA      
//									     COTTON_RinseWaterResetFreq_ui16[COTTON_RESET_LIMIT(uiFinalWt)]; 
	iRinselevel = COTTON_RinseWaterResetFreq_ui16[uiLoadRange]; 
       bSkip_R1 = RESET_VALUE;
       return result;
}

//************************************************************************************************************//
//********************************* COTTON 60-90 Program *****************************************************//
boolean ProgramCotton_60_90()
{
	
	static boolean result;
	
	//***************************************************
	switch(uiTempSel)
	{
		case Wash60:
		case P_60:
			uiTemperature_Required = TEMP_60;
			break;
		case Wash95:
		case P_95:
			uiTemperature_Required = TEMP_90;
			break;
		default: break;
	}
	//***************************************************
	
	if(uiPresent_Step < C_60_90_LENGTH)
	{
		uiProgramLast_step = C_60_90_LENGTH;
		//Below step are skipped to eradicated the hang issue observed during the reliabilioty test
//		if((uiPresent_Step == 3)
//		|| (uiPresent_Step == 44) 
//		|| (uiPresent_Step == 50)
//		|| (uiPresent_Step == 56))
////		|| (uiPresent_Step == 37) // skip the heating stage
////		|| (uiPresent_Step == 40)
////		|| (uiPresent_Step == 46)
////		|| (uiPresent_Step == 52)) // skip the heating stage
//		{
//			//uiTemperature_Required = NO;
//			uiPresent_Step++;
//		}
		/*Rinse1 skip logic for 1-7kg load*/
//		if((uidisp_select_option & Prewash_Yes) == Prewash_Yes)
//		{
//			
//			if((uiFinalWt<=7) && (uiPresent_Step == 39))
//			{
//				uiPresent_Step = 45;
//			}
//		}
//		else 
//		if((uiFinalWt<=7) && (uiPresent_Step == 39))
//		{
//			uiPresent_Step = 45;
//		}

		//Rinse1 skip logic for 1-6kg load
//		if((uiLoadRange<=2)&&(uiPresent_Step == 39))
//		{
//			uiPresent_Step = 45;
//		}
		if((uiLoadRange<=1)&&(uiPresent_Step == 43))
		{
			uiPresent_Step = 48;
		}
		
		washing(&cotton_60_90[uiPresent_Step]);
		//if(uiNextStep_Start_Delay == COMPLETED)
		//{
			//AI_Execution();
		//}
		//CirculationPump_Execution(&cotton_60_90[uiPresent_Step]);
		
		result = STEP_INPROGRESS;
	}
	else
        {
    	  	bProgram_Ended = YES;
	   	result = STEP_FINISHED;
		AI_WaterFreq_State_ui8 = 0;
        }
       
	iTumble_Waterlevel = TUMBLE_NORMAL;
       	Cotton_WashLevel();

	iRinselevel = COTTON_RinseWaterResetFreq_ui16[uiLoadRange]; 

       return result;
}

//**********************************************************************************************************//
//********************************* MIXED SOILED Program **************************************************//
boolean ProgramMixed()
{				
	static boolean result;	
	
	//***************************************************
	switch(uiTempSel)
	{
		case Wash60:
		case P_60:
			uiTemperature_Required = TEMP_60;
			break;
		case Wash30:
		case P_30:
			uiTemperature_Required = TEMP_30;
			break;
		case Wash40:
		case P_40:
			uiTemperature_Required = TEMP_40;
			break;
		default: break;
	}
	//***************************************************
	
	if(uiPresent_Step < MIXEDSOIL_LENGTH)
	{	
		uiProgramLast_step = MIXEDSOIL_LENGTH;
		//Below step are skipped to eradicated the hang issue observed during the reliabilioty test
		if((uiPresent_Step == 3)
		|| (uiPresent_Step == 8)
		|| (uiPresent_Step == 39)
		|| (uiPresent_Step == 45))//|| (uiPresent_Step == 34)|| (uiPresent_Step == 37)|| (uiPresent_Step == 43)) // skip the heating stage
		{
			//uiTemperature_Required = NO;
			uiPresent_Step++;
		}
		if(((uiTempSel == WashCold)||(uiTempSel ==P_COLD))&&((uiPresent_Step == 24)||(uiPresent_Step == 25))) // check selected temp is cold then skip heating stage
		{
			uiPresent_Step++;
		}
		washing(&mixedSoil[uiPresent_Step]);
		//if(uiNextStep_Start_Delay == COMPLETED)
		{
			AI_Execution();
		}
		//CirculationPump_Execution(&mixedSoil[uiPresent_Step]);
		
		result = STEP_INPROGRESS;
	}
	else
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
		AI_WaterFreq_State_ui8 = 0;
	}
       	iWater_Washlevel = MIXED_WASHLEVEL;
      
	 iRinselevel = MIXED_RinseWaterResetFreq_ui16[uiLoadRange];      
       
       iTumble_Waterlevel = TUMBLE_NORMAL;
       return result;
}

//********************************************************************************************************//
//*************************************** SYNTHETIC Program **********************************************//
boolean ProgramSynthetic()
{				
	static boolean result;	
	
	//****************************************************
	switch(uiTempSel)
	{
		case Wash30:
		case P_30:
			uiTemperature_Required = TEMP_30;
			break;
		case Wash40:
		case P_40:
			uiTemperature_Required = TEMP_40;
			break;
		case Wash60:
		case P_60:
			uiTemperature_Required = TEMP_60;
			break;	
		default: break;
	}
	//****************************************************
	
	if(uiPresent_Step < SYNTH_LENGTH)
	{
		//check_optavailable();
		uiProgramLast_step = SYNTH_LENGTH;
		//Below step are skipped to eradicated the hang issue observed during the reliability test
		if((uiPresent_Step == 3)
		|| (uiPresent_Step == 8) 
		|| (uiPresent_Step == 27)
		|| (uiPresent_Step == 31)
		|| (uiPresent_Step == 35) // skip the heating stage
		|| (uiPresent_Step == 39)
		|| (uiPresent_Step == 45)
		|| (uiPresent_Step == 51)) // skip the heating stage
		{
			//uiTemperature_Required = NO;
			uiPresent_Step++;
		}
		
		/*Rinse1 skip logic for 1-7kg load*/
//		if((uidisp_select_option & Prewash_Yes) == Prewash_Yes)
//		{
//			
//			if((uiFinalWt<=7) && (uiPresent_Step == 33))
//			{
//				uiPresent_Step = 39;
//			}
//		}
//		else 
//		if((uiFinalWt<=7) && (uiPresent_Step == 33))
//		{
//			uiPresent_Step = 39;
//		}
		//Rinse1 skip logic for 1-6kg load
		if((uiLoadRange<=1)&&(uiPresent_Step == 38))
		{
			uiPresent_Step = 44;
		}
		
		washing(&synth[uiPresent_Step]);
		AI_Execution();
		
		CirculationPump_Execution(&synth[uiPresent_Step]);
		
		result = STEP_INPROGRESS;
	}
	else
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
		AI_WaterFreq_State_ui8 = 0;
	}
	/**************************************************************Set_Reset_Freq_Update*****************************************/
	if(AI_ExecState_ui8 == AI_Runing)
	{
		AI_Set_Reset_Frequency_Update();
	}
	else
	{
		iWater_Washlevel = SYNTHETIC_WASHLEVEL; 
		//Cotton_WashLevel(); 
	}
	/***************************************************************************************************************************/
       //iWater_Washlevel = SYNTHETIC_WASHLEVEL;
       iRinselevel = SYNTHETIC_RinseWaterResetFreq_ui16[uiLoadRange];     
       iTumble_Waterlevel = TUMBLE_NORMAL;
       return result;
}
//******************************************************************************************************//
//********************************* COTTONECO 40 Program **********************************************//
boolean ProgramCottonEco40()
{
	static boolean result;
	
	uiTemperature_Required = TEMP_36;
	
	if(uiPresent_Step < ECO_40_LENGTH)
	{
		uiProgramLast_step = ECO_40_LENGTH;
		if((uiPresent_Step == 6)&&(uiWaterResetCount <= 1))
		{
			uiPresent_Step ++;
		}
			
		washing(&Eco_40[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}
	//***********************************************************************************
	if(((uiPresent_Step == 3)||(uiPresent_Step == 8))&&(bStartButton == TRUE))
	{
		//bCirculation_Pump = SWITCH_ON;
		Circulation_Check();
	}
	else if((uiPresent_Step == 6)&&(bStartButton == TRUE))
	{
		if((uiStep_Minutes_Completed <= NINE_MINUTES)&&(uiNextStep_Start_Delay == COMPLETED)&&(bDoorLockStatus == 1)) bCirculation_Pump = SWITCH_ON;
		else	bCirculation_Pump = SWITCH_OFF;
	}
	//***********************************************************************************
       Cotton_WashLevel();
       iRinselevel = EASY_WATERLEVEL;
       iTumble_Waterlevel = TUMBLE_NORMAL;
       return result;
}

//******************************************************************************************************//
//********************************* COTTONECO 60 Program **********************************************//
boolean ProgramCottonEco60()
{
	static boolean result;
	
	uiTemperature_Required = TEMP_65;
	
	if(uiPresent_Step < ECO_60_LENGTH)
	{
		uiProgramLast_step = ECO_60_LENGTH;
		washing(&Eco_60[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}
       //***********************************************************************************
	if(((uiPresent_Step == 3)||(uiPresent_Step == 8))&&(bStartButton == TRUE))
	{
		//bCirculation_Pump = SWITCH_ON;
		Circulation_Check();
	}
	else if((uiPresent_Step == 6)&&(bStartButton == TRUE))
	{
		if((uiStep_Minutes_Completed <= NINE_MINUTES)&&(uiNextStep_Start_Delay == COMPLETED)&&(bDoorLockStatus == 1)) bCirculation_Pump = SWITCH_ON;
		else	bCirculation_Pump = SWITCH_OFF;
	}
	//***********************************************************************************
	Cotton_WashLevel();
	iRinselevel = ECO_RINSELEVEL;
	iTumble_Waterlevel = TUMBLE_NORMAL;
	return result;
}
//********************************************************************************************************//
//********************************* SPORTSWEAR Program **********************************************//
boolean ProgramSportsWear()
{	
	static boolean result;	
	
	if((uiTempSel == Wash40) || (uiTempSel == P_40))
	{
		uiTemperature_Required = TEMP_40;
	}
	if(uiPresent_Step < SPORT_LENGTH)
	{
		uiProgramLast_step = SPORT_LENGTH;
		if(((uiTempSel == WashCold)||(uiTempSel == P_COLD))&&(uiPresent_Step == 8)) // check selected temp is cold then skip heating stage
		{
			uiPresent_Step++;
		}
		if(((uiPresent_Step == 9)||(uiPresent_Step == 10))&&(bStartButton == TRUE))
		{
			Circulation_Check();
		}
		washing(&sports[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}
	
       iWater_Washlevel = C_WASHLEVEL;
       iRinselevel = SPORTSWEAR_RINSELEVEL;
       iTumble_Waterlevel = TUMBLE_NORMAL;
       return result;
}


//***************************************************************************************************//
//********************************* RINSE-SPIN Program **********************************************//
boolean ProgramAdditivesRinse()
{				
	static boolean result;
	if(uiPresent_Step < ADDITIVES_RINSE_LENGTH)
	{	
		uiProgramLast_step = ADDITIVES_RINSE_LENGTH;
		washing(&AdditiveRinse[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else 
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}
	if((uiExtraRinse_Press_Select == 1)&&(uiPresent_Step == 2))
	{
		uiPresent_Step = 5;
	}
	iWater_Washlevel = RINSESPIN_RINSELEVEL;
	iTumble_Waterlevel = TUMBLE_NORMAL;
	iRinselevel = RINSESPIN_RINSELEVEL;
	return result;
	
}
//********************************************************************************************************//
//********************************* EXPRESS WASH Program **********************************************//
boolean Program_ExpressWash()
{
	static boolean result;
	
	//***************************************************
	switch(uiTempSel)
	{
		case Wash30:
			uiTemperature_Required = TEMP_30;
			break;
		case Wash40:
			uiTemperature_Required = TEMP_40;
			break;
		default: break;
	}
	//***************************************************
	
	
	if(uiPresent_Step < EXPS_LENGTH)
	{
		uiProgramLast_step = EXPS_LENGTH;
		
		if((uiTempSel == WashCold)&&(uiPresent_Step == 3))  //check selected temp is cold then skip heating stage
		{
			uiPresent_Step++;
		}
		/*uiPresent 3 Added to CP on for 1st step as per spec Rev9*/
		if(uiMachineState == START_PRESSED) //TODO:  CP should not be on if machine state is not Start_Pressed -23/11/21 
		{
			if((bStartButton == TRUE)&&(uiNextStep_Start_Delay == COMPLETED)&&(bDoorLockStatus == 1))
			{
				if(uiPresent_Step == 4) 
				{ bCirculation_Pump = SWITCH_ON; }
				else if((uiPresent_Step == 3) && (uiHeatState != STATE_HEAT_RESET_FILLING)) //TODO: Added to avoid cp on in heat resetfilling 
				{ bCirculation_Pump = SWITCH_ON; }
			}
		}
		if((uiPresent_Step == 15) && ((Foam_Removal_Complete & 0x01) != 0x01))
		{
				Foam_Removal_Enable = TRUE;	//NA // for express spin drain & spin
			
		}/*
		else 
		{
			if((uiPresent_Step == 16) && ((Foam_Removal_Complete & 0x02) != 0x02)){
				Foam_Removal_Enable = TRUE;	//NA
			}
			
		}*/
		washing(&ExpressWash[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else 
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
		/*Foam_Removal_Enable = NO;	//program end
		Foam_Removal_Complete = FALSE;	//program end*/
	}
	
	iWater_Washlevel = EXPRESS_15_WASHLEVEL;
	iTumble_Waterlevel= TUMBLE_NORMAL;
	iRinselevel = EXPRESS_RINSELEVEL;
	bSkip_R1 = RESET_VALUE;
	return result;
}

//********************************************************************************************************//

boolean Program_ExpressWash_30()
{
	static boolean result;
	
	//***************************************************
	switch(uiTempSel)
	{
		case Wash40:
			uiTemperature_Required = TEMP_40;
			break;
		case Wash60:
			uiTemperature_Required = TEMP_60;
			break;
		default: break;
	}

	if(uiPresent_Step < EXPS_30_LENGTH)
	{
		uiProgramLast_step = EXPS_30_LENGTH;
		
		washing(&ExpressWash_30[uiPresent_Step]);
		CirculationPump_Execution(&ExpressWash_30[uiPresent_Step]);
		
		result = STEP_INPROGRESS;
	}
	else 
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}
	
	iWater_Washlevel 	= EXPRESS_30_WASHSET_LEVEL;
	iTumble_Waterlevel 	= TUMBLE_NORMAL;
	iRinselevel 		= EXPRESS_30_RINSELEVEL;
	bSkip_R1 		= RESET_VALUE;
	return result;
}
//********************************* DRAIN SPIN Program **********************************************//
/*
boolean ProgramDrainSpin()
{	
	static boolean result;
	if(uiPresent_Step < DRAIN_SPIN_LENGTH)
	{	
		uiProgramLast_step = DRAIN_SPIN_LENGTH;
		
		washing(&DrainSpin[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else 
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}	
	return result;
}
*/
//********************************************************************************************************//
//********************************* CRADLE WASH Program **********************************************//
boolean ProgramCradleWash()
{
	static boolean result;
	
	//***************************************************
	switch(uiTempSel)
	{
		case Wash30:
			uiTemperature_Required = TEMP_30;
			break;
		case Wash40:
			uiTemperature_Required = TEMP_40;
			break;
		default: break;
	}
	//***************************************************
	
	if(uiPresent_Step < CRDL_LENGTH)
	{
		uiProgramLast_step = CRDL_LENGTH;
		if((uiTempSel == WashCold)&&(uiPresent_Step == 3)) // check selected temp is cold then skip heating stage
		{
			uiPresent_Step++;
		}
		else if(((uiTempSel == Wash40)||(uiTempSel == Wash30))&&(uiPresent_Step == 4)) // check selected temp is cold then skip heating stage
		{
			uiPresent_Step++;
		}
		if(((uiPresent_Step == 4)||(uiPresent_Step == 5))&&(bStartButton == TRUE)&&(uiNextStep_Start_Delay == COMPLETED)&&(bDoorLockStatus == 1))
		{
			bCirculation_Pump = SWITCH_ON;	
		}
		washing(&CradleWash[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}
	
        iWater_Washlevel = CRADLE_WASHLEVEL;
        iRinselevel = CRADLE_RINSELEVL;
        iTumble_Waterlevel = TUMBLE_SPECIAL;
	
	//********************
	bSkip_R1 = YES;
	
	return result;
}

//***************************************************************************************************//
//********************************* BABY WEAR Program **********************************************//
boolean ProgramBabyCare()
{				
	static boolean result;	
	
	//***************************************************
	switch(uiTempSel)
	{
		case Wash40:
		case P_40:
			uiTemperature_Required = TEMP_40;
			break;
		case Wash60:
		case P_60:
			uiTemperature_Required = TEMP_60;
			break;
		default: break;
	}
	//***************************************************
	
	if(uiPresent_Step < BABYCARE_LENGTH)
	{	
		uiProgramLast_step = BABYCARE_LENGTH;
		if((uiTempSel == WashCold)&&(uiPresent_Step == 4)) // check selected temp is cold then skip heating stage
		{
			uiPresent_Step++;
		}
		if((uiPresent_Step == 5)&&(bStartButton == TRUE))
		{
			Circulation_Check();
		}
		washing(&babyCare[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}
	
       iWater_Washlevel = BABY_WASHLEVEL;
       iRinselevel = BABY_RINSELEVEL;
       iTumble_Waterlevel = TUMBLE_NORMAL;
       return result;
}

//********************************************************************************************************//
//********************************* ANTIALLERGEN Program **********************************************//
boolean ProgramAntiAllergen()
{	
	
	static boolean result;	
	
	//***************************************************
	switch(uiTempSel)
	{
		case Wash40:
		case P_40:
			uiTemperature_Required = TEMP_40;
			break;
		case Wash60:
		case P_60:
			uiTemperature_Required = TEMP_57;
			break;
		case Wash95:
		case P_95:
			uiTemperature_Required = TEMP_90;
			break;
		default: break;
	}
	//***************************************************
	
	if(uiPresent_Step < ANTIAL_LENGTH)
	{
		uiProgramLast_step = ANTIAL_LENGTH;
		//check_optavailable();
		if((bStartButton == TRUE))
		{
			if(uiPresent_Step == 5)
			{
				Circulation_Check();
			}
			/*TODO: CP should on at Soak phase after 10 minutes as per spec REV 9.*/
			/*if((uiPresent_Step == 3)&&(uiTumbleState == STATE_TUMBLE))
			{
				if(uiStep_Minutes_Completed >= TEN_MINUTES)
					bCirculation_Pump = SWITCH_ON;
			}*/
		}
		
		washing(&antiAllergen[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}
	
       iWater_Washlevel = BABY_WASHLEVEL;
       iRinselevel = ALLERGY_RINSELEVEL;
       iTumble_Waterlevel = TUMBLE_NORMAL;
       return result;
}

//**************************************************************************************************//
//********************************* CURTAINS Program **********************************************//
boolean ProgramCurtains()
{	
	
	static boolean result;	
	
	//***************************************************
	switch(uiTempSel)
	{
		case Wash40:
		case P_40:
			uiTemperature_Required = TEMP_40;
			break;
		case Wash60:
		case P_60:
			uiTemperature_Required = TEMP_60;
			break;
		default: break;
	}
	//***************************************************
	
	if(uiPresent_Step < CUR_LENGTH)
	{
		uiProgramLast_step = CUR_LENGTH;
		if(((uiTempSel == WashCold)||(uiTempSel == P_COLD))&&(uiPresent_Step == 6)) // check selected temp is cold then skip heating stage
		{
			uiPresent_Step++;
		}
		if((uiPresent_Step == 7)&&(bStartButton == TRUE))
		{
			Circulation_Check();
		}
		washing(&curtains[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}
	
       iWater_Washlevel = CURTAINS_WASH_LEVEL;
       iRinselevel = CURTAINS_RINSE_LEVEL;
       iTumble_Waterlevel = TUMBLE_NORMAL;
       return result;
}
//**************************************************************************************************//
//********************************* DARK WASH Program **********************************************//
boolean ProgramDarkWash()
{	
	static boolean result;	
	
	//***************************************************
	uiTemperature_Required = TEMP_40;
	//***************************************************
	
	if(uiPresent_Step < WASH_LENGTH)
	{
		//check_optavailable();
		uiProgramLast_step = WASH_LENGTH;
		if(((uiPresent_Step == 8) || (uiPresent_Step == 9))&&(bStartButton == TRUE))
		{
			Circulation_Check();
		}
		washing(&darkwash[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}
	
       iWater_Washlevel = BABY_WASHLEVEL;
       iRinselevel = SPORTSWEAR_RINSELEVEL;
       iTumble_Waterlevel = TUMBLE_NORMAL;
       return result;
}
//**************************************************************************************************//
//********************************* INNER Wear Program **********************************************//
boolean ProgramInnerWear()
{	
  
	static boolean result;	
	
	//***************************************************
	uiTemperature_Required = TEMP_40;
	//***************************************************
	
	if(uiPresent_Step < INNER_LENGTH)
	{
		//check_optavailable();
		uiProgramLast_step = INNER_LENGTH;
		if((bRapidWashButton == TRUE)&&(bProgramStart == TRUE)&&(uiPresent_Step == 5))
		{
			uiPresent_Step++;
		}
		if((uiPresent_Step == 4)||((bRapidWashButton == FALSE)&&(uiPresent_Step == 5))&&(bStartButton == TRUE))
		{
			Circulation_Check();
		}	
		washing(&innerwash[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}
	
       iWater_Washlevel = SPORTSWEAR_RINSELEVEL;
       iRinselevel = INNERWEAR_RINSELEVEL;
       iTumble_Waterlevel = TUMBLE_NORMAL;
       return result;
}
//**************************************************************************************************//
//********************************* Steam Wash Program ********************************************//
boolean ProgramSteamWash()
{	
 
	static boolean result;
	
	//***************************************************
	uiTemperature_Required = TEMP_90;
	//***************************************************
	
	if(uiPresent_Step < STEAM_LENGTH)
	{
		uiProgramLast_step = STEAM_LENGTH;
			
		washing(&steamwash[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}
	/*In cycle Phases CP phase are not present so we added Tumbling Phase with IDLE Duty to achieve CP on*/
//	if((bStartButton == TRUE)&&(uiNextStep_Start_Delay == COMPLETED))// TODO: Added CP on extra step As spec Rev9. //16-11-2021
//	{
//		if(uiPresent_Step == 3 && uiTumbleState == STATE_TUMBLE)
//		{
//			bCirculation_Pump = SWITCH_ON;		
//		}
//	}
       iWater_Washlevel = SYNTHETIC_WASHLEVEL;
       iRinselevel = STEAMWASH_RINSELEVEL;
       iTumble_Waterlevel = TUMBLE_NORMAL;
       return result;
}
//*****************************************************************************************************//
//********************************* Steam Fresh Program **********************************************//
boolean ProgramSteamFresh()
{	
	
	static boolean result;
	
	//***************************************************
	uiTemperature_Required = TEMP_90;
	//***************************************************
	
	if(uiPresent_Step < STEAM_FRESH_LENGTH)
	{	
		uiProgramLast_step = STEAM_FRESH_LENGTH;
		
		washing(&SteamFresh[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else 
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}
	
	return result;
}
//**************************************************************************************************//
//********************************* TUB CLEAN Program **********************************************//
boolean ProgramTubClean()
{	
	static boolean result;	
	
	//***************************************************
	switch(uiTempSel)
	{
		case Wash60:
			uiTemperature_Required = TEMP_60;
			break;
		case Wash95:
			uiTemperature_Required = TEMP_90;
			break;
		default: break;
	}
	//***************************************************
	
	if(uiPresent_Step < TUB_LENGTH)
	{
		uiProgramLast_step = TUB_LENGTH;
		if((uiTempSel == WashCold)&&((uiPresent_Step == 3)||(uiPresent_Step == 6))) // check selected temp is cold then skip heating stage
		{
			uiPresent_Step++;
		}
//		if(((uiPresent_Step == 7)||(uiPresent_Step == 8))&&(bStartButton == TRUE)&&(uiTempSel != Wash95)&&(uiNextStep_Start_Delay == COMPLETED)&&(bDoorLockStatus == 1))
//		{
//			bCirculation_Pump = SWITCH_ON;	
//		}
		washing(&tubClean[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}
	
       iWater_Washlevel = TUBCLEAN_WASHLEVEL;
       iRinselevel = CRADLE_RINSELEVL;
       iTumble_Waterlevel = TUMBLE_NORMAL;
       return result;
}

//**************************************************************************************************//
//********************************* UNIFORM Program **********************************************//
boolean ProgramUniform()
{	
  
	static boolean result;	
	
	//***************************************************
	switch(uiTempSel)
	{
		case Wash40:
		case P_40:
			uiTemperature_Required = TEMP_40;
			break;
		case Wash60:
		case P_60:
			uiTemperature_Required = TEMP_60;
			break;
		default: break;
	}
	//***************************************************
	
	if(uiPresent_Step < UNI_LENGTH)
	{
		uiProgramLast_step = UNI_LENGTH;
		if(((bRapidWashButton == TRUE)||((uiTempSel == WashCold) || (uiTempSel == P_COLD)))&&(bProgramStart == TRUE)&&(uiPresent_Step == 6))
		{
			uiPresent_Step = 7;	
		}
		else if(((uiTempSel == WashCold) || (uiTempSel == P_COLD))&&(uiPresent_Step == 8)) // check selected temp is cold then skip heating stage
		{
			uiPresent_Step = 10;
		}
		/*else if((uiWaterResetCount <= 1)&&(uiTempSel > WashCold)&&((uidisp_select_option & WarmSoak_Yes) == WarmSoak_Yes)&&(uiPresent_Step == 9))
		{
			uiPresent_Step = 10;
		}*/
		else if((bProgramStart == TRUE)&&(uiPresent_Step == 10)) 
		{
			if((uiTempSel == Wash40)||(uiTempSel == Wash60)||(uiTempSel == P_60)||(uiTempSel == P_40)||(bRapidWashButton == TRUE))
			{
				uiPresent_Step = 11;
			}
		}
//		if(((uiPresent_Step == 9)||(uiPresent_Step == 10))&&(bStartButton == TRUE))
//		{
//			Circulation_Check();
//		}
		
		washing(&uniform[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}
	
       iWater_Washlevel = C_WASHLEVEL;
       iRinselevel = C_RINSELEVEL;
       iTumble_Waterlevel = TUMBLE_NORMAL;
       return result;
}
//***********************************************************************************************//
//********************************* JEANS Program **********************************************//
boolean ProgramJeans()
{	
	static boolean result;	
	//***************************************************
	uiTemperature_Required = TEMP_60;
	//***************************************************
	
	if(uiPresent_Step < JEANS_LENGTH)
	{
		uiProgramLast_step = JEANS_LENGTH;
		
		if((bRapidWashButton == TRUE)&&(uiPresent_Step == 24))
		{
			uiPresent_Step = 25;	
		}
		else if((bRapidWashButton == FALSE)&&(uiPresent_Step == 13))
		{
			uiPresent_Step = 23;	
		}
		
		if(((uiPresent_Step == 24)||(uiPresent_Step == 25)||(uiPresent_Step == 26))&&(bStartButton == TRUE))
		{
			Circulation_Check();
		}
		washing(&Jeans[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}
	iTumble_Waterlevel = TUMBLE_NORMAL;
       iWater_Washlevel = C_WASHLEVEL;
       if((uidisp_select_option & Prewash_Yes) == Prewash_Yes)
       {
	       if(uiPrewashWaterResetCount > 2)
	       {
			iRinselevel = C_RINSELEVEL;
	       }
	       else
	       {
		      	iRinselevel = 39029; 
	       }
       }
       else
       {
	        if(uiWaterResetCount > 2)
	       {
			iRinselevel = C_RINSELEVEL;
	       }
	       else
	       {
		      	iRinselevel = 39029; 
	       }
       }
      
       return result;
}
//**************************************************************************************************//
//********************************* SHIRTS Program **********************************************//
boolean ProgramShirts()
{	
	static boolean result;	
	
	//***************************************************
	switch(uiTempSel)
	{
		case Wash30:
		case P_30:
			uiTemperature_Required = TEMP_30;
			break;
		case Wash40:
		case P_40:
			uiTemperature_Required = TEMP_40;
			break;
		case Wash60:
		case P_60:
			uiTemperature_Required = TEMP_60;
			break;
		default: break;
	}
	//***************************************************
	
	if(uiPresent_Step < SHIRTS_LENGTH)
	{
		uiProgramLast_step = SHIRTS_LENGTH;
		
		if((uiTempSel == WashCold)&&(uiPresent_Step == 6)) // check selected temp is cold then skip heating stage
		{
			uiPresent_Step = 7;
		}
		if((uiPresent_Step == 8)&&(bStartButton == TRUE))
		{
			Circulation_Check();
		}
		washing(&shirts[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}
	
       iWater_Washlevel = SHIRTS_WASHLEVEL;
       iRinselevel = SHIRTS_RINSELEVEL;
       iTumble_Waterlevel = TUMBLE_NORMAL;
       return result;
}
//********************************* WETCLEAN Program **********************************************//
boolean ProgramWetclean()
{
static boolean result;
	
	//***************************************************
	switch(uiTempSel)
	{
		case Wash30:
			uiTemperature_Required = TEMP_30;
			break;
		case Wash40:
			uiTemperature_Required = TEMP_40;
			break;
		default: break;
	}
	//***************************************************
	
	if(uiPresent_Step < WETCLEAN_LENGTH)
	{
		uiProgramLast_step = WETCLEAN_LENGTH;
		if((uiTempSel == WashCold)&&(uiPresent_Step == 3)) // check selected temp is cold then skip heating stage
		{
			uiPresent_Step++;
		}
		washing(&wetclean[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}
	
        iWater_Washlevel = CRADLE_WASHLEVEL;
        iRinselevel = CRADLE_RINSELEVL;
        iTumble_Waterlevel = TUMBLE_SPECIAL;
	
	//********************
	bSkip_R1 = YES;
	
	return result;
}

boolean ProgramLinenwash()
{	
	static boolean result;	
	
	//***************************************************
	switch(uiTempSel)
	{
		case Wash30:
			uiTemperature_Required = TEMP_30;
			break;
		case Wash40:
			uiTemperature_Required = TEMP_40;
			break;
		default: break;
	}
	//***************************************************
	
	if(uiPresent_Step < LINENWASH_LENGTH) //SHIRTS_LENGTH
	{
		uiProgramLast_step = LINENWASH_LENGTH; //SHIRTS_LENGTH
		
		if((uiTempSel == WashCold)&&(uiPresent_Step == 5)) // check selected temp is cold then skip heating stage //6
		{
			uiPresent_Step++;
		}
		washing(&linen[uiPresent_Step]);
		CirculationPump_Execution(&linen[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}
	
       iWater_Washlevel = STEAMWASH_RINSELEVEL; //changed
       iRinselevel = STEAMWASH_RINSELEVEL; //changed 
       iTumble_Waterlevel = TUMBLE_NORMAL;
       return result;
}

//**************************************************************************************************//
//********************************* SPECIAL FUNCTION 6 Program ************************************//
boolean ProgramAdditivesEndurance()
{	

	static boolean result;	
	if(uiPresent_Step < ADDITIVES_ENDURANCE_LENGTH)
	{ 
		uiProgramLast_step = ADDITIVES_ENDURANCE_LENGTH;
		washing(&AdditiveEndurance[uiPresent_Step]);
		result = STEP_INPROGRESS;
	}
	else 
	{
		bProgram_Ended = YES;
		result = STEP_FINISHED;
	}

	iTumble_Waterlevel = TUMBLE_NORMAL;
	iRinselevel = RINSESPIN_RINSELEVEL;
	return result;
}
//****************************************************************************************//
//****************************************************************************************//
void Cotton_WashLevel(void)
{
	if(((uidisp_select_option & Prewash_Yes) == Prewash_Yes) && (uiTumblingCount == OPTIONAL_PREWASH))
	{
		if(((uiPrewashWaterResetCount < 2)&&(bEcoLevelReached == TRUE))&&(bEco60Reset == FALSE))
	       {
	       		iWater_Washlevel = ECO60_WASHLEVEL_1; //2nd-3rd Setlevel
	       }
	       else if((uiPrewashWaterResetCount >= 2)&&(bEco60Reset == FALSE))
	       {
		       iWater_Washlevel = 40369;  //4th & onwards Setlevel
	       }
	       else if(bEco60Reset == TRUE)
	       {
		       iWater_Washlevel = ECO60_HEAT_WASHLEVEL;  // During & After Heating WashLevel
	       }
	       else
	       {
		   	iWater_Washlevel = ECO60_WASHLEVEL;  //1St Setlevel
	       }
	}
	else
	{
		if(((uiWaterResetCount < 2)&&(bEcoLevelReached == TRUE))&&(bEco60Reset == FALSE))
	       {
	       		iWater_Washlevel = ECO60_WASHLEVEL_1; //2nd-3rd Setlevel
	       }
	       else if((uiWaterResetCount >= 2)&&(bEco60Reset == FALSE))
	       {
		       iWater_Washlevel = 40369;  //4th & onwards Setlevel
	       }
	       else if(bEco60Reset == TRUE)
	       {
		       iWater_Washlevel = ECO60_HEAT_WASHLEVEL;  // During & After Heating WashLevel
	       }
	       else
	       {
		   	iWater_Washlevel = ECO60_WASHLEVEL;  //1St Setlevel
	       }
	}
      
}
//*********************************************************************************************//
//***************************** Circulation ON Condition *************************************//
static void Circulation_Check(void)
{
	
	if(bMainWashValve || bPrewashValve || bReheatHeater)
	{
		bCirculation_Pump = SWITCH_OFF;
	}
	else if(((uiStep_Minutes_Completed <= NINE_MINUTES)||
	((uiStep_Minutes_Completed >= FIFTEEN_MINUTES)&&(uiStep_Minutes_Completed <= TWENTY_FOUR_MINUTES)))
	&&(uiNextStep_Start_Delay == COMPLETED)&&(bDoorLockStatus == 1))
	{
		bCirculation_Pump = SWITCH_ON;
	}
	else
	{
		bCirculation_Pump = SWITCH_OFF;
	}
}
void AI_Set_Reset_Frequency_Update(void)
{
	if(((uidisp_select_option & Prewash_Yes) == Prewash_Yes) && (uiTumblingCount == OPTIONAL_PREWASH))
	{
		switch(uiPrewashWaterResetCount)
		{
			case 0: iWater_Washlevel = 39999;
			break;
			
			case 1:
			case 2: 
				iWater_Washlevel = 40130;
			break;
			
			default:
				iWater_Washlevel = 40360;
			break;
		}
	}
	else
	{	
		switch(uiWaterResetCount)
		{
			case 0: iWater_Washlevel = 39999;
			break;
			
			case 1:
			case 2: 
				iWater_Washlevel = 40130;
			break;
			
			default:
				iWater_Washlevel = 40360;
			break;
		}
		
	}
}
/*uint8_t  getPrograms_StepTimeHold_ui8()
{
	static uint8_t ret_b;
	if()
	{
		ret_b = TRUE;
	}
	else
	{
		ret_b = FALSE;
	}
	return ret_b;
}*/
