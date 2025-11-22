#include "waterfill.h"
#include "r_cg_userdefine.h"
#include "string.h"
#include "WashOperations_Global.h"


// *********Global Variables**********//
extern boolean bStartButton,bTwoLitreReached,TubClean_Activated;
extern boolean bStop_Minutes_Count,bMotor_Start,bPrewashValve,bMainWashValve;
extern boolean bWater_Fill_Completed,bWaterFillStage,bCirculation_Pump;
extern boolean bHeater;
extern uint8_t uiStep_Minutes_Completed,uiResetState,uiNoWaterState,uiTumblingCount;
extern uint8_t uiError_Mode,uiLowPressureTime,uiNoWaterTime,uiTempSel,uiNextStep_Start_Delay;
extern uint8_t uiFillingState,uiLowPressureState,uiPresent_Step,uiStep_Seconds;
extern uint8_t uiExtraTime_Seconds,uiTime_Seconds,uiTime_Minutes_Completed;
extern uint8_t cdigits_data[7],uiSelected_Position,uiWaterResetCount,uiPrewashWaterResetCount,uiMachineState;
extern uint8_t uiTemperature,uiTemperature_Required,uiSmartProgram_Selected;

extern uint16_t iWaterLevel;
extern uint16_t ucMotor_Rotate_Seconds;
extern uint16_t uiCurrent_step_time,BalanceTime;
extern uint16_t uicycle_mincompleted;
extern uint8_t AI_ExecState_ui8;

//added as Prs error condition on reset count more than 15 was not executed properly;
//reset count was incremented only for filling step; resets in other washing steps were not considered -- fixed
//uint8_t uiResetFill_Count;
//********Global Function prototype*******//
extern void LowPressure(uint8_t,uint8_t,uint16_t);
extern void NoWater(uint8_t);
extern void Valveselection(uint8_t);
extern void Valveselection1(uint8_t);
extern void motor(uint8_t);
extern boolean reset_filling(uint8_t,uint16_t);
extern void MotorStop();

//*******Local Variables*******//
static boolean result;
boolean bLevelReached,bEcoLevelReached,bSteam_Filling_Start;
uint8_t uiSteamFill_Seconds;

//******local function prototype*******//
void LevelReached(uint8_t);

/**********************************************************************************************************************
* Function Name: WashFilling
* Description  : This function execute the filling of water to desired level.
* Arguments    : uint8_t,uint8_t,uint8_t,
		 uint16_t,uint16_t.
* Return Value : boolean
***********************************************************************************************************************/
boolean WashFilling(uint8_t uiValveSelect,uint8_t uiStepTime,uint8_t uiDuty_Selcted,uint16_t iSetlevel,uint16_t iTumblelevel)
{
	static uint16_t iResetLevel,iWashDesired;
	static boolean heat_done;
	if((GET_PROGRAM) == COTTON)
	{	
		if(AI_ExecState_ui8 == AI_Runing)
		{
			iResetLevel = 40369;//ECO_RESET_LEVEL;;
		}
		else
		{
			iResetLevel = ECO_RESET_LEVEL;
		}
		//iResetLevel = ECO_RESET_LEVEL;
	}
	else if((GET_PROGRAM) == MIXEDSOIL)
	{
		iResetLevel = MIXED_RESET_LEVEL;
	}
	else if((GET_PROGRAM) == SYNTHETIC)
	{
		if(AI_ExecState_ui8 == AI_Runing)
		{
			iResetLevel = 40369;;
		}
		else
		{
			 iResetLevel = SYNTHETIC_RESET_LEVEL;
		}
		//iResetLevel = SYNTHETIC_RESET_LEVEL;
	}
	else
	{
		iResetLevel = RESET_LEVEL;
	}
	bWaterFillStage = TRUE;
	//**********************************************
	    /*if((uiSelected_Position == WOOLLENS)||(TubClean_Activated == TRUE))
	    {
		if(iSetlevel > RINSE_DESIRED)
		{
			iWashDesired = iSetlevel;
		}
		else
		{
			iWashDesired = RINSE_DESIRED;
		}
	    }
	    else
	    {
		    iWashDesired = iSetlevel;
	    }*/
	    iWashDesired = iSetlevel;
	//**********************************************
	switch(uiFillingState)
	{
	
		case STATE_FILL:
						if(uiStep_Minutes_Completed == ZERO_MINUTE)
						{
							if(iWaterLevel > iWashDesired)
							{
								Wash_WithinOneMinute(uiValveSelect,iTumblelevel);
								
							}
							else
							{
								bWater_Fill_Completed = YES;
								BalanceTime --;
								uicycle_mincompleted++;
								uiCurrent_step_time --;
								uiStep_Minutes_Completed = ONE_MINUTE;
								uiTime_Minutes_Completed = ONE_MINUTE;
								uiStep_Seconds = RESET_VALUE;
								uiTime_Seconds = RESET_VALUE;
								MotorStop();
								
								if((uiSelected_Position == EXPRESS)&&(uiPresent_Step == 2))
								{
									uiFillingState = STATE_FILL_FINISHED;
								}
								else
								{
									uiNextStep_Start_Delay =2;
									bMainWashValve = SWITCH_OFF;
									bPrewashValve = SWITCH_OFF;
							 		uiFillingState = STATE_LEVEL_REACHED;
								}
							}
						}
						//if uiStep_Minutes_Completed is Reached to ONE_MINUTE?
						else if(uiStep_Minutes_Completed == ONE_MINUTES)
						{
							//If iWaterLevel is reached to  LOW_PRESSURE?
							if(iWaterLevel < LOW_PRESSURE)
							{
								uiFillingState = STATE_LOW_PRESSURE;
							}
							else
							{
								uiFillingState = STATE_NO_WATER;
								bStop_Minutes_Count = YES;
							}
						}
						uiNoWaterState = RESET_VALUE;
						uiLowPressureTime = RESET_VALUE; // reset low iWaterLevel , now iWaterLevel time vlaues
						uiNoWaterTime  = RESET_VALUE;
						uiLowPressureState = RESET_VALUE;
						result = STEP_INPROGRESS ;
						bHeater = SWITCH_OFF;
						break;
					
		case STATE_LEVEL_REACHED: 
						if(iWaterLevel > iResetLevel)
						{
							uiFillingState = STATE_RESET_FILLING;
							//ucMotor_Rotate_Seconds = RESET_VALUE;
							bMotor_Start = FALSE;
							bCirculation_Pump = SWITCH_OFF;
							bHeater = SWITCH_OFF;
							MotorStop();
						}
						//if uiStep_Minutes_Completed is Reached to uiStepTime?
						else if(uiStep_Minutes_Completed >= uiStepTime)
						{
							uiFillingState = STATE_FILL_FINISHED;	
						}
						else if((uiSelected_Position == EXPRESS)&&(uiPresent_Step == 2))
						{
							uiFillingState = STATE_FILL_FINISHED;
						}
						else
						{
							LevelReached(uiDuty_Selcted);
							//if(uiSelected_Position == EXPRESS_30)
							if((uiSelected_Position == SMARTPLUS) && (uiSmartProgram_Selected == EXPRESS_30))
							{	/*TODO: Logic Added only for Heating in filling 
								As per spec EL-PROG-285 Rev_19     */
								if(uiTemperature>=uiTemperature_Required)
								{
									bHeater = SWITCH_OFF;
									heat_done = TRUE;
								}
								else if(heat_done == FALSE)
								{
									bHeater = SWITCH_ON;
								}
							}
						}
						
						MachineErrors.EBits.NoWaterError = 0; 
						result = STEP_INPROGRESS ;
						break;
						
		case STATE_LOW_PRESSURE:
						//if iWaterlvel Reached iSetlevel? 
						if(iWashDesired > iWaterLevel)
						{
							//ucMotor_Rotate_Seconds = RESET_VALUE;
							MotorStop();
							uiLowPressureState = RESET_VALUE;
							uiError_Mode = RESET_VALUE;
							bMainWashValve = SWITCH_OFF;
							bPrewashValve = SWITCH_OFF;
							uiNextStep_Start_Delay = 2;
							uiFillingState = STATE_LEVEL_REACHED;
						}
						/*else if(iWaterLevel > (LOW_PRESSURE))
						{
							//ucMotor_Rotate_Seconds = RESET_VALUE;
							MotorStop();
							uiLowPressureState = RESET_VALUE;
							uiError_Mode = RESET_VALUE;
							uiFillingState = STATE_NO_WATER;
							uiExtraTime_Seconds = RESET_VALUE;
						}*/
						else
						{
							if(bStartButton == FALSE)
							{
								uiExtraTime_Seconds = RESET_VALUE;
							}
							 if((uiSelected_Position == CRADLEWASH))//||(uiSelected_Position == WETCLEAN)) // Added WC
							{
								LowPressure(DUTY_EMI,uiValveSelect,iTumblelevel);
							}
							else if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == WOOLLENS))
							{
								LowPressure(DUTY_RWOOL,uiValveSelect,iTumblelevel);
							}
							else 
							{
								LowPressure(DUTY_R1,uiValveSelect,iTumblelevel);
							}
							bTwoLitreReached = TRUE;
						}
						bHeater = SWITCH_OFF;
						bCirculation_Pump = SWITCH_OFF;
						result = STEP_INPROGRESS ;
						break;	
						
		case STATE_NO_WATER:	
						// if iWaterlvel Reached iTumblevel?
						if((LOW_PRESSURE-12) > iWaterLevel)
						{
							uiFillingState = STATE_LOW_PRESSURE;
							uiNoWaterState = RESET_VALUE;
							uiExtraTime_Seconds = RESET_VALUE;
						}
						else
						{
							if(bStartButton == FALSE)
							{
								uiExtraTime_Seconds = RESET_VALUE;
							}
							NoWater(uiValveSelect);	
						}
						bHeater = SWITCH_OFF;
						bCirculation_Pump = SWITCH_OFF;
						result = STEP_INPROGRESS ;
						break;
						
		case STATE_RESET_FILLING:
						// reset_filling(iSetlevel,iTumblelevel) is Completed?
						if(reset_filling(uiValveSelect,iWashDesired))
						{
							//uiResetFill_Count++;
							uiFillingState = STATE_LEVEL_REACHED;
							uiResetState = RESET_VALUE;
							//ucMotor_Rotate_Seconds = RESET_VALUE;
							MotorStop();
							if(uiTumblingCount == OPTIONAL_PREWASH)
							{
								uiPrewashWaterResetCount++;
							}
							else if (uiTumblingCount == FILLING)
							{
								uiWaterResetCount++;
							}
							
						}
						bHeater = SWITCH_OFF;
						bCirculation_Pump = SWITCH_OFF;
						result = STEP_INPROGRESS ;
						break;
						
		case STATE_FILL_FINISHED: 	bHeater = SWITCH_OFF;
						heat_done = FALSE;
						bLevelReached = FALSE;
						bPrewashValve = SWITCH_OFF;
						bEcoLevelReached = FALSE;
						bCirculation_Pump = SWITCH_OFF;
						bMainWashValve = SWITCH_OFF;
						uiFillingState = RESET_VALUE;
						bTwoLitreReached = FALSE;
						bWaterFillStage = RESET_VALUE;
						result = STEP_FINISHED;
						break;
					
		default : // No operation required
			if(uiFillingState == 0xFF)
			{
				uiFillingState =0;
				result = STEP_INPROGRESS;
			}
			break;
	}
 return result;
}

/**********************************************************************************************************************
* Function Name: Wash_WithinOneMinute
* Description  : This function is to check the desired level is attained with in a minute or not.
* Arguments    : uint8_t
		 uint16_t
* Return Value : void
***********************************************************************************************************************/
void Wash_WithinOneMinute(uint8_t uiValveSelect,uint16_t iTumblelevel)
{
	if(iWaterLevel > iTumblelevel) 
	{
		if(iWaterLevel > TWO_LITER)
		{
			Valveselection(uiValveSelect);	// vale on/off based on valve selection
		}
		else
		{
			Valveselection1(uiValveSelect);
		}
		bWater_Fill_Completed = NO;
		if(bTwoLitreReached == TRUE)
		{
			bMotor_Start = YES; 
			motor(DUTY_R1);  
		}
		else if((bTwoLitreReached == TRUE)&&((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == WOOLLENS)))
		{
			bMotor_Start = YES; 
			motor(DUTY_RWOOL);  
		}
		else if((bTwoLitreReached == TRUE)&&((uiSelected_Position == CRADLEWASH)))//||(uiSelected_Position == WETCLEAN))) //Added WC
		{
			bMotor_Start = YES; 
			motor(DUTY_EMI);  
		}
		else
		{
			bMotor_Start = NO; 	
		}
	}
	else
	{
		bTwoLitreReached = TRUE;
		Valveselection1(uiValveSelect);
		bMotor_Start = YES; 
		if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == WOOLLENS))
		{
			motor(DUTY_RWOOL);
		}
		else if((uiSelected_Position == CRADLEWASH))//||(uiSelected_Position == WETCLEAN)) //Added WC
		{
			motor(DUTY_EMI);
		}
		else
		{
			motor(DUTY_R1);  
		}
	}
}

/**********************************************************************************************************************
* Function Name: LevelReached
* Description  : This function execute after desired level is attained.
* Arguments    : uint8_t
* Return Value : void
***********************************************************************************************************************/
void LevelReached(uint8_t uiDuty_Selcted)
{
	bStop_Minutes_Count = NO;	 // donot step uiStep_Minutes_Completed
	uiLowPressureTime = RESET_VALUE; // reset the value of low pressure time  & no iWaterLevel time
	uiNoWaterTime  = RESET_VALUE;	
	uiError_Mode = ERROR_FREE;	 // no uiError_Occured , reset the mode
	if((((TubClean_Activated == TRUE)&&(iWaterLevel > SIXTEEN_LITER))
	||(((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == WOOLLENS))&&(iWaterLevel > WOOL_WASHLEVEL)))
	&&(bLevelReached == FALSE))
	{
		bMainWashValve = SWITCH_ON;	
	}
	else
	{
		bLevelReached = TRUE;
		bMainWashValve = SWITCH_OFF;
	}
	bPrewashValve = SWITCH_OFF;
	bMotor_Start = YES;
	motor(uiDuty_Selcted);
	uiLowPressureState = RESET_VALUE;
	uiNoWaterState = RESET_VALUE;
	
	if(((uiSelected_Position == COTTON) && ((uiTempSel != Wash40E)&&(uiTempSel != Wash60E)))//uiTempSel<=Wash95
	||(uiSelected_Position == MIXEDSOIL) 
	||(uiSelected_Position == SYNTHETIC) 
	||((uiSelected_Position == SMARTPLUS) && (uiSmartProgram_Selected == EXPRESS_30)))//(uiSelected_Position == EXPRESS_30)))
	{
		;
	}
//	else if(uiSelected_Position == WETCLEAN)  //Added WC
//	{
//		bCirculation_Pump = SWITCH_OFF;
//	}
	else if((uiStep_Minutes_Completed < ELEVEN_MINUTES)&&((uiSelected_Position != COTTON) 
	/*||((uiSelected_Position == COTTON)&&((uiTempSel != Wash40E)&&(uiTempSel != Wash60E)))*/))
	{
		bCirculation_Pump = SWITCH_ON;
	}
	else
	{
		bCirculation_Pump = SWITCH_OFF;
	}
	
	
	if(uiSelected_Position == COTTON)
	{
		bEcoLevelReached = TRUE;
	}
}


/**********************************************************************************************************************
* Function Name: SteamFill
* Description  : This function execute the filling of water to desired level in steam program.
* Arguments    : uint8_t
* Return Value : void
***********************************************************************************************************************/
boolean SteamFill(uint8_t uiValveSelect,uint16_t iSet_WaterLevel)
{
	
	switch(uiFillingState)
	{
		case STATE_FILL:
				if(uiStep_Minutes_Completed <= ONE_MINUTES)
				{
					if(iWaterLevel > iSet_WaterLevel)
					{
						SteamFilling();	
					}
					else
					{
						if(uiCurrent_step_time > RESET_VALUE)
						{
							BalanceTime --;
							uicycle_mincompleted++;
							uiCurrent_step_time --;
						}
						uiFillingState = STATE_FILL_FINISHED;
					}
				}
				//if uiStep_Minutes_Completed is Reached to ONE_MINUTE?
				else if(uiStep_Minutes_Completed == TWO_MINUTES)
				{
					if(iWaterLevel > iSet_WaterLevel)
					{
						uiFillingState = STATE_NO_WATER;
						bStop_Minutes_Count = YES;
					}
					else
					{
						uiFillingState = STATE_FILL_FINISHED;
					}
				}
				uiNoWaterState = RESET_VALUE;
				uiNoWaterTime  = RESET_VALUE;
				MachineErrors.EBits.NoWaterError = 0;
				result = STEP_INPROGRESS ;
				break;
									
		case STATE_NO_WATER:	
				// if iWaterlvel Reached iTumblevel?
				if(iWaterLevel > iSet_WaterLevel)
				{
					if(bStartButton == FALSE)
					{
						uiExtraTime_Seconds = RESET_VALUE;
						uiNoWaterTime  = RESET_VALUE;
						uiNoWaterState = RESET_VALUE;
					}
					NoWater(uiValveSelect);	
				}
				else
				{
					uiFillingState = STATE_FILL_FINISHED;
				}
				result = STEP_INPROGRESS ;
				break;
				
		case STATE_FILL_FINISHED: 
				bMainWashValve = SWITCH_OFF;
				uiFillingState = RESET_VALUE;
				MachineErrors.EBits.NoWaterError = 0; 
				uiSteamFill_Seconds = RESET_VALUE;
				bSteam_Filling_Start = NO;
				result = STEP_FINISHED;
				break;
					
		default : // No operation required
				//if(uiFillingState == 0xFF)
				//{
					uiFillingState =0;
					result = STEP_INPROGRESS;
				//}commented due to steam filling not happening
				break;
	}
	
	return result;
}

/**********************************************************************************************************************
* Function Name: SteamFilling
* Description  : This function executes waterfilling in steam.
* Arguments    : None
* Return Value : void
***********************************************************************************************************************/
void SteamFilling()
{
	bSteam_Filling_Start = YES;
	if(uiSteamFill_Seconds < ONE_SECOND)
	{
		if((uiPresent_Step <= 3)&&(uiSelected_Position != STEAMFRESH))
		{
	 		bMainWashValve = SWITCH_ON;
		}
		else
		{
			bPrewashValve = SWITCH_ON;
			bMainWashValve = SWITCH_ON;
		}
	}
	else if(uiSteamFill_Seconds < NINE_SECONDS)
	{
		bMainWashValve = SWITCH_OFF;
		bPrewashValve = SWITCH_OFF;
	}
	else if(uiSteamFill_Seconds >= NINE_SECONDS)
	{
		uiSteamFill_Seconds = RESET_VALUE;
	}	
}
