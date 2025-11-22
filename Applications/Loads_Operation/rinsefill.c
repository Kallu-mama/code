#include "rinsefill.h"
#include "r_cg_userdefine.h"


//******global Variables*******//
extern boolean bStop_Minutes_Count,bMotor_Start,bPrewashValve,bMainWashValve;
extern boolean bWater_Fill_Completed,bWaterlevel_Desired,bWaterFillStage;
extern boolean bStartButton,bDutyCycle_Change,bCirculation_Pump,bNtcFailed;
extern boolean bHeater;
extern uint8_t uiError_Mode,uiLowPressureTime,uiNoWaterTime,uiSelected_Position,uiStartTemperature,uiTemperature;
extern uint8_t uiExtraTime_Seconds,uiTime_Minutes_Completed,uiTime_Seconds,uiTempSel;
extern uint8_t uiStep_Seconds,uiRinseFillState,uiStep_Minutes_Completed,uiRinse_Reset_Count;
extern uint8_t uiResetState,uiNoWaterState,uiLowPressureState,uiNextStep_Start_Delay;
extern uint8_t uiTemperature_Required,uiPresent_Step,uiTumblingCount,uiSmartProgram_Selected;
extern uint16_t BalanceTime,uiCurrent_step_time;
//extern uint16_t ucMotor_Rotate_Seconds;
extern uint16_t iWaterLevel,uicycle_mincompleted;

//******global Function prototype******//
extern void LowPressure(uint8_t,uint8_t,uint16_t);
extern void NoWater(uint8_t);
extern void Valveselection(uint8_t);
extern void Valveselection1(uint8_t);
extern void RinseTumble(uint8_t);
extern void motor(uint8_t);
extern boolean reset_filling(uint8_t,uint16_t);

//*******local Variable********//
static boolean result;
//*****local function prototype*****//


/**********************************************************************************************************************
* Function Name: RinseFilling
* Description  : This function execute the filling of water in rinsing step to desired level.
* Arguments    : uint8_t,uint8_t,uint8_t,
		 uint16_t,uint16_t.
* Return Value : boolean
***********************************************************************************************************************/
boolean RinseFilling(uint8_t uiValveSelect,uint8_t uiStepTime,uint8_t uiDuty_Selected,uint16_t iSetlevel,uint16_t iTumblelevel)
{
	static uint16_t iRinseDesired,iResetLevel;
	static boolean heat_done;
	//**************************************************
//		if((GET_PROGRAM) == COTTON)
//		{
//			iResetLevel = ECO_RESET_LEVEL;
//		}
		//if((uiSelected_Position == COTTON)&&(uiTempSel >= Wash40E))
		if((uiSelected_Position == COTTON)&&((uiTempSel == Wash40E)||(uiTempSel == Wash60E)))
		{
			iResetLevel = ECO_RESET_LEVEL;
		}
		else if(GET_PROGRAM == COTTON)
		{
			iResetLevel = COTTON_RINSE_RESET_LEVEL;
		}
		else if((GET_PROGRAM) == MIXEDSOIL)
		{
			iResetLevel = MIXED_RESET_LEVEL;
		}
		else if((GET_PROGRAM) == SYNTHETIC)
		{
			iResetLevel = SYNTHETIC_RESET_LEVEL;
		}
		else
		{
			iResetLevel = RESET_LEVEL;
		}
	//**********************************
		if(iSetlevel > RINSE_DESIRED)
		{
			iRinseDesired = iSetlevel;
		}
		else
		{
			if((uiSelected_Position == CRADLEWASH))//||(uiSelected_Position == WETCLEAN)) //Added WC
			{
				iRinseDesired = iSetlevel;
			}
			else
			{
				iRinseDesired = RINSE_DESIRED;
			}
		}
	//***************************************
	bWaterFillStage = TRUE;
	switch(uiRinseFillState)
	{
		case STATE_FILL:
				if(uiStep_Minutes_Completed == ZERO_MINUTE)
				{
					if(iWaterLevel > iRinseDesired)
					{
						Rinse_WithinOneMinute(uiValveSelect,uiDuty_Selected,iTumblelevel);
					}
					else
					{
						bMotor_Start = NO;
						uiNextStep_Start_Delay = 2;
						uiRinseFillState = STATE_DESIRED ;
						//ucMotor_Rotate_Seconds = RESET_VALUE;
					}
				}
				else if(uiStep_Minutes_Completed == ONE_MINUTE)
				{
					//If iWaterLevel is reached to  LOW_PRESSURE?
					if(iWaterLevel < LOW_PRESSURE)
					{
					 	uiRinseFillState = STATE_LOW_PRESSURE;
					}
					else 
					{
					 	uiRinseFillState = STATE_NO_WATER;
					 	bStop_Minutes_Count = YES;
					}	
				}
				result = STEP_INPROGRESS;
				uiNoWaterState = RESET_VALUE;
				uiLowPressureTime = RESET_VALUE; // reset low iWaterLevel , now iWaterLevel time vlaues
				uiNoWaterTime  = RESET_VALUE;
				uiLowPressureState = RESET_VALUE;
				
				break;
					
		case STATE_DESIRED:
				if(uiStep_Minutes_Completed >= uiStepTime)
				{
					uiRinseFillState = STATE_FILL_FINISHED;
					bPrewashValve = SWITCH_OFF;		// switch off the vales
					bMainWashValve = SWITCH_OFF;
					
				}
				else  if(iWaterLevel > iSetlevel)
				{
					RinseDesireLevelReached(uiDuty_Selected,uiValveSelect);
				}
				else
				{
					if(uiStep_Minutes_Completed == ZERO_MINUTE)
					{
						BalanceTime --;
						uiCurrent_step_time --;
						uicycle_mincompleted++;
						uiTime_Minutes_Completed = ONE_MINUTE;
						uiTime_Seconds = RESET_VALUE;
						uiStep_Minutes_Completed = ONE_MINUTE;
						uiStep_Seconds = RESET_VALUE;
					}
					uiRinseFillState = STATE_LEVEL_REACHED;
				}
				
				MachineErrors.EBits.NoWaterError = 0; 
				result = STEP_INPROGRESS;
				break;
					
		case STATE_LEVEL_REACHED: 
				if(uiStep_Minutes_Completed >= uiStepTime)
				{
					uiRinseFillState = STATE_FILL_FINISHED;	
				}
				else if(iWaterLevel > iResetLevel)
				{
					uiRinseFillState = STATE_RESET_FILLING;	
				}
				else
				{
					RinseLevelReached(uiDuty_Selected);
					//if((uiSelected_Position == EXPRESS_30) && (uiPresent_Step == 16/*15*/) && (uiTumblingCount == RINSE3_ADDITIVES))
					if(((uiSelected_Position == SMARTPLUS) && (uiSmartProgram_Selected == EXPRESS_30)) && (uiPresent_Step == 16) && (uiTumblingCount == RINSE3_ADDITIVES))
					{	/*TODO: Logic Added only for Heating in filling 
						As per spec EL-PROG-285 Rev_19     */
						if(uiTemperature >= uiTemperature_Required)
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
				result = STEP_INPROGRESS;
				break;
					
		case STATE_LOW_PRESSURE:
				/*if(iWaterLevel > LOW_PRESSURE)
				{
					uiRinseFillState = STATE_NO_WATER;
					uiExtraTime_Seconds = RESET_VALUE;
				}
				else*/ if(iWaterLevel > iRinseDesired)
				{
					if(bStartButton == FALSE)
					{
						uiExtraTime_Seconds = RESET_VALUE;
					}
					if((uiSelected_Position == CRADLEWASH))//||(uiSelected_Position == WETCLEAN)) //Added WC
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
					
				}
				else 
				{
					bMotor_Start = NO;
					uiNextStep_Start_Delay = 2;
					uiRinseFillState = STATE_DESIRED ;
					//ucMotor_Rotate_Seconds = RESET_VALUE;
				}
				bHeater = SWITCH_OFF;
				bCirculation_Pump = SWITCH_OFF;
				result = STEP_INPROGRESS;
				break;
					
		case STATE_NO_WATER:
				if(iWaterLevel > (LOW_PRESSURE -12))
				{
					
					if(bStartButton == FALSE)
					{
						uiExtraTime_Seconds = RESET_VALUE;	
					}
						NoWater(uiValveSelect);
				}
				else
				{
					uiRinseFillState = STATE_LOW_PRESSURE;
					uiExtraTime_Seconds = RESET_VALUE;
					uiLowPressureTime = RESET_VALUE;
					uiLowPressureState = RESET_VALUE;
					
				}
				bHeater = SWITCH_OFF;
				bCirculation_Pump = SWITCH_OFF;
				result = STEP_INPROGRESS;
				break;
					
		case STATE_RESET_FILLING:
				if((uiRinse_Reset_Count >= 1)&&(uiSmartProgram_Selected != WOOLLENS)&&(uiSelected_Position !=CRADLEWASH))//&&(uiSelected_Position == !WETCLEAN))//Added WC
				{
					iSetlevel = TUMBLE_SPECIAL;
				}
				if(reset_filling(uiValveSelect,iSetlevel))
				{
					uiRinseFillState = STATE_LEVEL_REACHED;
					uiResetState = RESET_VALUE;
					//ucMotor_Rotate_Seconds = RESET_VALUE;
					//uiMotor_Tumble_State = RESET_VALUE;
					uiRinse_Reset_Count++;
				}
				bHeater = SWITCH_OFF;
				bMotor_Start = NO;
				bCirculation_Pump = SWITCH_OFF;
				result = STEP_INPROGRESS;
				break;
					
		case STATE_FILL_FINISHED:
				heat_done = FALSE;
				bHeater = SWITCH_OFF;
				uiRinseFillState = RESET_VALUE;
				uiNoWaterState = RESET_VALUE;
				bCirculation_Pump = SWITCH_OFF;
				uiLowPressureTime = RESET_VALUE; // reset low iWaterLevel , now iWaterLevel time vlaues
				uiNoWaterTime  = RESET_VALUE;
				uiLowPressureState = RESET_VALUE;
				uiRinse_Reset_Count = RESET_VALUE;
				result = STEP_FINISHED;
				bWaterFillStage = RESET_VALUE;
				break;
		default : // No operation required
			if(uiRinseFillState == 0xFF)
			{
				uiRinseFillState = STATE_FILL;
				result = STEP_INPROGRESS;
			}
			break;
	}
	
 return result;
}

/**********************************************************************************************************************
* Function Name: RinseLevelReached
* Description  : This function execute after desired level is attained.
* Arguments    : uint8_t
* Return Value : void
***********************************************************************************************************************/
void RinseLevelReached(uint8_t uiDuty_Selected)
{
	bStop_Minutes_Count = NO;
	uiLowPressureTime = RESET_VALUE; // reset low iWaterLevel , now iWaterLevel time vlaues
	uiNoWaterTime  = RESET_VALUE;
	uiError_Mode = RESET_VALUE;
	bMotor_Start = YES;
	motor(uiDuty_Selected);		// tumble with selected cDuty_Selected cycle
	bPrewashValve = SWITCH_OFF;		// switch of the vales
	bMainWashValve = SWITCH_OFF;
	uiNoWaterState = RESET_VALUE;
//	if(uiSelected_Position == WETCLEAN)//Added WC
//	{
//		bCirculation_Pump = SWITCH_OFF;
//	}
//	else
//	{	
//		bCirculation_Pump = SWITCH_ON;
//	}
	//bCirculation_Pump = SWITCH_ON;
	uiLowPressureState = RESET_VALUE;
}

/**********************************************************************************************************************
* Function Name: Rinse_WithinOneMinute
* Description  : This function is to check the desired level is attained with in a minute or not.
* Arguments    : uint8_t,uint8_t
		 uint16_t
* Return Value : void
***********************************************************************************************************************/
void Rinse_WithinOneMinute(uint8_t uiValveSelect,uint8_t uiDuty_Selected,uint16_t iTumblelevel)
{
	if(iWaterLevel > iTumblelevel)
	{
			// normal
			bMotor_Start = NO;	// NO Water Tumbling
			bDutyCycle_Change = YES;
			bWater_Fill_Completed = NO; // iWaterLevel level not reached to set level
			bWaterlevel_Desired = NOT_REACHED;
			if(iWaterLevel > TWO_LITER)
			{
				Valveselection(uiValveSelect);	// vale on/off based on valve selection
			}
			else
			{
				Valveselection1(uiValveSelect);
			}
	}	
	else 
	{
		bWater_Fill_Completed = NO;
		bMotor_Start = YES;
		Valveselection1(uiValveSelect);	//vale on/off based on valve selection
		if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == WOOLLENS))
		{
			RinseTumble(DUTY_RWOOL);
		}
		else if((uiSelected_Position == CRADLEWASH))//||(uiSelected_Position == WETCLEAN)) //Added WC
		{
			RinseTumble(DUTY_EMI);
		}
		else
		{
			RinseTumble(DUTY_R1); 
		}
	}
}

void RinseDesireLevelReached(uint8_t cDuty_Selected,uint8_t uiValveSelect)
{
	bMotor_Start = YES;
	motor(cDuty_Selected);
	uiLowPressureTime = RESET_VALUE; 
	uiLowPressureState = RESET_VALUE;
	bStop_Minutes_Count = NO;
	
	Valveselection1(uiValveSelect);
}
