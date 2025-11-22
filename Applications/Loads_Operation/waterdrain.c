#include "waterdrain.h"

//************************* global variables ************************************//

extern boolean bStop_Minutes_Count,bMotor_Start,bDrainPump,bTestMode_Selection,bStartButton;//bExpress_30_Activatted;
extern boolean bWaterLevel_Read_Required;
extern boolean bPrewashValve;
extern boolean Foam_Removal_Enable,bFoamDetection;
extern uint8_t Foam_Removal_Complete,uiSmartProgram_Selected;
boolean bFoamRm_Step;

extern uint8_t uiStep_Minutes_Completed,uiError_Mode,uiAdditional_Drain_TurnOn_Mins,uiTempSel;
extern uint8_t uiDrainState,uiStep_Seconds,uiTime_Minutes_Completed,uiSelected_Position;
extern uint8_t uiprev_error,uiTumblingCount,uiDrain_Off_Seconds,uiTestModeState,uiPresent_Step;
extern uint16_t BalanceTime,uicycle_mincompleted,uiCurrent_step_time,iWaterLevel;

//****** global function prototype *****//
//extern void MotorValues_Reset(void);
extern void motor(uint8_t);
extern void MotorStop(void);

//********** Local variables *******//
uint8_t uiDrainOffSeconds = 10;
uint8_t uiFoamDrain_Seconds,uiFoamDrain_TotalSeconds,uiTest_drain;// new variable added to resolve dPEr issue during spin
boolean bDrainLevelReach;
boolean bDrain_Time_Correct;


boolean draining(uint8_t uiStepTime,uint8_t uiTumbling_Required,uint8_t uiDuty_Selected,uint16_t uiSet_DrainLevel)
{
	static boolean result;
	bWaterLevel_Read_Required = TRUE;
	
	switch(uiDrainState)
	{
		case STATE_DRAIN:
				MachineErrors.EBits.DrainPump = RESET_VALUE;
				uiprev_error = 0xFF;
				if((iWaterLevel > uiSet_DrainLevel)&&(uiTumbling_Required == FALSE)&&((uiTestModeState!= SF01)||(uiTestModeState != SF02)))
				{
					bDrainLevelReach = TRUE;
					if((uiDrainOffSeconds == RESET_VALUE)||(uiTumblingCount == RINSE_HALFDRAIN))
					{
						if((uiStepTime == TWO_SECONDS)&&(uiStep_Minutes_Completed == RESET_VALUE))
						{
							BalanceTime -= 2;
							uicycle_mincompleted += 2;
							uiCurrent_step_time -= 2;
						}
						else
						{
							if(uiTumblingCount == INITIAL_DRAIN)
							{
								;
							}
							else if(((uiSelected_Position != EXPRESS)
							&&((uiSmartProgram_Selected != STEAMWASH /*&& bExpress_30_Activatted == FALSE*/))
							&&(uiPresent_Step != RESET_VALUE)))
							{
								BalanceTime = (BalanceTime - uiCurrent_step_time);
								//uicycle_mincompleted++;
								uicycle_mincompleted = uicycle_mincompleted + uiCurrent_step_time ;
							}
							uiCurrent_step_time--;
						}
							
						uiDrainState = STATE_DRAIN_FINISHED;
					}
				}
				else if((uiStep_Minutes_Completed >= uiStepTime)&&(iWaterLevel > uiSet_DrainLevel))
				{
					/*if(uiStepTime == TWO_SECONDS)
					{
						BalanceTime -= 2;
						uicycle_mincompleted += 2;
						uiCurrent_step_time -= 2;
					}*/
					//else
					{
						if(uiTumblingCount == INITIAL_DRAIN)
						{
							;
						}
						else if(((uiSelected_Position != EXPRESS)&&(uiPresent_Step != RESET_VALUE)
						&&((uiSmartProgram_Selected != STEAMWASH /*&& bExpress_30_Activatted == FALSE*/))))
						{
							BalanceTime = (BalanceTime - uiCurrent_step_time);
							//uicycle_mincompleted++;
							uicycle_mincompleted = uicycle_mincompleted + uiCurrent_step_time ;
						}
						
						if(uiCurrent_step_time > RESET_VALUE)
						uiCurrent_step_time--;
					}
					uiDrainState = STATE_DRAIN_FINISHED;
				}
				else if(uiStep_Minutes_Completed < uiStepTime)
				{	
					bDrainPump = SWITCH_ON;
					//( uiSelected_Position != STEAMFRESH
					Foam_Removal_prewash_gui8();		// for express spin drain & spin
					
					uiAdditional_Drain_TurnOn_Mins = RESET_VALUE;
				}
				else
				{
					uiDrainState = STATE_EXTRA_DRAIN_THREE_MINUTES;
				}
				
				//If uiTumbling_Required is  TRUE?
				
				if(uiTumbling_Required == TRUE)
				{
			  		bMotor_Start = YES;
					motor(uiDuty_Selected);
				}
				bStop_Minutes_Count = NO;
				uiError_Mode = RESET_VALUE;
				//MachineErrors.EBits.DrainPump = RESET_VALUE;
				result = STEP_INPROGRESS;
				break;
		case STATE_EXTRA_DRAIN_THREE_MINUTES: 
		
					bStop_Minutes_Count = YES;	
			 		uiError_Mode = EMPTYLEVEL_NOT_ATTAINED; 
					//If iWaterLevel is Reached to EMPTYLEVEL?
					if(iWaterLevel > uiSet_DrainLevel)
					{
						uiDrainState = STATE_DRAIN_FINISHED;			
					}
					// if uiAdditional_Drain_TurnOn_Mins is reached ADDTITIONAL_DRAIN_THREE_MINUTES?
					else if(uiAdditional_Drain_TurnOn_Mins >= ADDTITIONAL_DRAIN_THREE_MINUTES)
					{
						uiDrainState = STATE_DRAIN_IDLE;
						bDrainPump = SWITCH_OFF;
						uiAdditional_Drain_TurnOn_Mins = RESET_VALUE;
						uiDrain_Off_Seconds = 5;
						bMotor_Start = NO;
						//MotorValues_Reset();
					}
					else
					{
						bDrainPump = SWITCH_ON;
					}
					if(uiTumbling_Required == TRUE) 
					{
						bMotor_Start = YES;	
						motor(uiDuty_Selected);
					}
					MachineErrors.EBits.DrainPump = 0;
					result = STEP_INPROGRESS;
					break;
		case STATE_DRAIN_IDLE:	
		
					bStop_Minutes_Count = YES;	
					uiError_Mode = EMPTYLEVEL_NOT_ATTAINED;
					if(uiDrain_Off_Seconds == COMPLETED)
					{
			 			bDrainPump = SWITCH_ON; 
						uiAdditional_Drain_TurnOn_Mins = RESET_VALUE;
						uiDrainState = STATE_EXTRA_DRAIN_FOUR_MINUTES;
						bStop_Minutes_Count = YES;
					}
					else
					{
						bMotor_Start = NO;
						MotorStop();
						bDrainPump = SWITCH_OFF;
					}
					result = STEP_INPROGRESS;
					break;
					
		case STATE_EXTRA_DRAIN_FOUR_MINUTES: 
					bStop_Minutes_Count = YES;	
				 	uiError_Mode = EMPTYLEVEL_NOT_ATTAINED; 
					//If iWaterLevel is Reached to EMPTYLEVEL?
					if(iWaterLevel > uiSet_DrainLevel)
					{
						uiDrainState = STATE_DRAIN_FINISHED;			
					}
					//if uiAdditional_Drain_TurnOn_Mins is reached ADDTITIONAL_DRAIN_THREE_MINUTES?
					else if(uiAdditional_Drain_TurnOn_Mins >= ADDTITIONAL_DRAIN_FOUR_MINUTES)
					{
						uiDrainState = STAE_ERROR;
						uiAdditional_Drain_TurnOn_Mins = RESET_VALUE;
						//MotorValues_Reset();
						bMotor_Start = NO;	
						bDrain_Time_Correct = FALSE;
					}
					if(uiTumbling_Required == TRUE) 
					{
						bDrainPump = SWITCH_ON;
				  		bMotor_Start = YES;	
				  		motor(uiDuty_Selected);
				 	}
					else
					{
						bDrainPump = SWITCH_ON;
					}
					result = STEP_INPROGRESS;
					break;
							
		case STAE_ERROR: 
		
				//uiError_Mode = RESET_VALUE;	
				MachineErrors.EBits.DrainPump = 1;
				bMotor_Start = NO;
				MotorStop();
				bDrainPump = SWITCH_OFF; 
				//bStartButton = PAUSEPRESSED;	 
				//uiMachineState = PAUSE_PRESSED;
				if((bStartButton == FALSE)&&(bDrain_Time_Correct == FALSE))
				{
					uiError_Mode = RESET_VALUE;
					if(uiStepTime == TWO_SECONDS)
					{
						BalanceTime += 2;
						uicycle_mincompleted -= 2;
						uiCurrent_step_time = 2;
						bDrain_Time_Correct = TRUE;
					}
					else
					{
						if(uiSelected_Position != EXPRESS)
						{
							BalanceTime ++;
							uicycle_mincompleted --;
							uiCurrent_step_time = 1;
							bDrain_Time_Correct = TRUE;
						}
					}	
				}
				uiStep_Minutes_Completed = RESET_VALUE;
				uiTime_Minutes_Completed = RESET_VALUE;
				uiAdditional_Drain_TurnOn_Mins = RESET_VALUE;
				result = STEP_INPROGRESS;
				break;
				
		case STATE_DRAIN_FINISHED:
		
					bDrainPump = SWITCH_OFF;
					bMotor_Start = NO;
					Foam_reset();	//drain finished
					//MotorValues_Reset();
					uiDrainState = RESET_VALUE;
				    	uiDrainOffSeconds = 10;
				    	bDrainLevelReach = FALSE;
				 	result = STEP_FINISHED;
					uiAdditional_Drain_TurnOn_Mins = RESET_VALUE;
					uiError_Mode = RESET_VALUE;
					MachineErrors.EBits.DrainPump = 0;
					break;
					
		default ://No operation required
			if(uiDrainState == 0xFF)
			{
				uiDrainState = RESET_VALUE;
				result = STEP_INPROGRESS;
			}
			break;
	}
 	return result;
}

void Foam_Removal_prewash_gui8(void)
{	
	if(Foam_Removal_Enable)
	{
		if(uiFoamDrain_Seconds < ONE_SECOND)
		{
			bPrewashValve = SWITCH_ON;
		}
		else if(uiFoamDrain_Seconds < FOUR_SECONDS)
		{
			bPrewashValve = SWITCH_OFF;
		}
		else if(uiFoamDrain_Seconds >= FOUR_SECONDS)
		{
			uiFoamDrain_Seconds = RESET_VALUE;
		}
		if(uiFoamDrain_TotalSeconds >= SIXTY_SECONDS)
		{
			bPrewashValve = SWITCH_OFF;
			uiFoamDrain_TotalSeconds = FALSE;
			Foam_Removal_Complete |= (uint8_t)(0x01 << Foam_Removal_Complete);//1
			bFoamRm_Step = 1;
			Foam_Removal_Enable = 0x00;	//reset after complete
		}
	}
	
}

void Foam_reset(void)
{
	bPrewashValve = FALSE;
	/*if(Foam_Removal_Enable)
	{
		//Foam_Removal_Complete = 0x01;
	//	Foam_Removal_Complete |= (uint8_t)(0x01 << Foam_Removal_Complete);//1
		Foam_Removal_Enable = 0x00;	//step reset
	}*/
}



/***********************************************************************
* Function_Name : Conditional_drain(uint16_t).
* Ret_type	: boolean.
* Working	: Draining Water & finding out Drain error. Function applicable only for Mixed, Cotton <= Wash95, Synthetic.
* Developed By	: Prashant_H
* Date		: 16/04/2022
************************************************************************/
boolean Conditional_drain(uint16_t uiSet_DrainLevel)
{
	static boolean result;
	bWaterLevel_Read_Required = TRUE;
	/*Return should be required if following cycles are not selected*/
	if(((uiSelected_Position != MIXEDSOIL) 
		&&(uiSelected_Position != SYNTHETIC)
		&&(uiSmartProgram_Selected != EXPRESS_30) //(uiSelected_Position != EXPRESS_30)
		&&((uiSelected_Position != COTTON)|| (uiSelected_Position == COTTON && ((uiTempSel == Wash40E)||(uiTempSel == Wash60E))/*uiTempSel > Wash95*/)))
		|| (bFoamDetection == TRUE))
	{
		return TRUE;
	}
	
	switch(uiDrainState)
	{
		case STATE_DRAIN:
				MachineErrors.EBits.DrainPump = RESET_VALUE;
				uiprev_error = 0xFF;
				bStop_Minutes_Count = YES;	
			 	uiError_Mode = EMPTYLEVEL_NOT_ATTAINED; 
				if((iWaterLevel >= uiSet_DrainLevel)&&((uiTestModeState!= SF01)||(uiTestModeState != SF02)))
				{
					uiTest_drain = YES;
					uiDrainState = STATE_DRAIN_FINISHED;
				}
				else if(iWaterLevel >= uiSet_DrainLevel)
				{
					uiTest_drain = YES;
					uiDrainState = STATE_DRAIN_FINISHED;
				}
				if((iWaterLevel < uiSet_DrainLevel) && (uiAdditional_Drain_TurnOn_Mins >= ONE_MINUTE))
				{
					bDrainPump = SWITCH_OFF;
					uiAdditional_Drain_TurnOn_Mins = RESET_VALUE;
					if(uiTest_drain == YES)
					{
						uiDrainState = STATE_DRAIN_FINISHED;
					}
					else
					{
						uiDrainState = STATE_EXTRA_DRAIN_THREE_MINUTES;
					}
				}
				else
				{
					bDrainPump = SWITCH_ON;
					//correction for foam sensing delay
					if(uiTest_drain == YES)
					{
						uiDrainState = STATE_DRAIN_FINISHED;
					}
				}
				result = STEP_INPROGRESS;
				break;
		case STATE_EXTRA_DRAIN_THREE_MINUTES: 
		
					bStop_Minutes_Count = YES;	
			 		uiError_Mode = EMPTYLEVEL_NOT_ATTAINED; 
					//If iWaterLevel is Reached to EMPTYLEVEL?
					if(iWaterLevel > uiSet_DrainLevel)
					{
						uiTest_drain = YES;
						uiDrainState = STATE_DRAIN_FINISHED;			
					}
					// if uiAdditional_Drain_TurnOn_Mins is reached ADDTITIONAL_DRAIN_THREE_MINUTES?
					else if(uiAdditional_Drain_TurnOn_Mins >= ADDTITIONAL_DRAIN_THREE_MINUTES)
					{
						uiDrainState = STATE_DRAIN_IDLE;
						bDrainPump = SWITCH_OFF;
						uiAdditional_Drain_TurnOn_Mins = RESET_VALUE;
						uiDrain_Off_Seconds = 5;
					}
					else
					{
						bDrainPump = SWITCH_ON;
					}
					
					MachineErrors.EBits.DrainPump = 0;
					result = STEP_INPROGRESS;
					break;
		case STATE_DRAIN_IDLE:	
		
					bStop_Minutes_Count = YES;	
					uiError_Mode = EMPTYLEVEL_NOT_ATTAINED;
					if(uiDrain_Off_Seconds == COMPLETED)
					{
			 			bDrainPump = SWITCH_ON; 
						uiAdditional_Drain_TurnOn_Mins = RESET_VALUE;
						uiDrainState = STATE_EXTRA_DRAIN_FOUR_MINUTES;
						bStop_Minutes_Count = YES;
					}
					else
					{
						bDrainPump = SWITCH_OFF;
					}
					result = STEP_INPROGRESS;
					break;
					
		case STATE_EXTRA_DRAIN_FOUR_MINUTES: 
					bStop_Minutes_Count = YES;	
				 	uiError_Mode = EMPTYLEVEL_NOT_ATTAINED; 
					//If iWaterLevel is Reached to EMPTYLEVEL?
					if(iWaterLevel > uiSet_DrainLevel)
					{
						uiTest_drain = YES;
						uiDrainState = STATE_DRAIN_FINISHED;			
					}
					//if uiAdditional_Drain_TurnOn_Mins is reached ADDTITIONAL_DRAIN_THREE_MINUTES?
					else if(uiAdditional_Drain_TurnOn_Mins >= ADDTITIONAL_DRAIN_FOUR_MINUTES)
					{
						uiDrainState = STAE_ERROR;
						uiAdditional_Drain_TurnOn_Mins = RESET_VALUE;	
						bDrain_Time_Correct = FALSE;
					}
					else
					{
						bDrainPump = SWITCH_ON;
					}
					result = STEP_INPROGRESS;
					break;
							
		case STAE_ERROR: 
		
				//uiError_Mode = RESET_VALUE;	
				MachineErrors.EBits.DrainPump = 1;
				bMotor_Start = NO;
				MotorStop();
				bDrainPump = SWITCH_OFF; 
				if((bStartButton == FALSE)&&(bDrain_Time_Correct == FALSE))
				{
					uiError_Mode = RESET_VALUE;
					
				}
				uiStep_Minutes_Completed = RESET_VALUE;
				uiTime_Minutes_Completed = RESET_VALUE;
				uiAdditional_Drain_TurnOn_Mins = RESET_VALUE;
				result = STEP_INPROGRESS;
				break;
				
		case STATE_DRAIN_FINISHED:
					//bDrainPump = SWITCH_OFF;//commented for safety condition
					uiDrainState = RESET_VALUE;
				    	uiDrainOffSeconds = 10;
				    	bDrainLevelReach = FALSE;
				 	result = STEP_FINISHED;
					uiAdditional_Drain_TurnOn_Mins = RESET_VALUE;
					uiError_Mode = RESET_VALUE;
					MachineErrors.EBits.DrainPump = 0;
					bStop_Minutes_Count = NO;
					break;
					
		default ://No operation required
			if(uiDrainState == 0xFF)
			{
				uiDrainState = RESET_VALUE;
				result = STEP_INPROGRESS;
			}
			break;
	}
 	return result;
}
