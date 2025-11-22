#include "valveselection.h"
#include "waterfill.h"
#include "r_cg_userdefine.h"


extern void motor(uint8_t);
extern void MotorStop(void);
extern void SteamFilling();
extern void Delay_ms(uint16_t t);
//*****global variables******//
extern boolean bStop_Minutes_Count,bMotor_Start,bDrainPump,bPrewashValve,bWaterLevel_Read_Required;//bExpress_30_Activatted;
extern boolean bMainWashValve,bSkip_R1,bDutyCycle_Change,TubClean_Activated;
extern boolean bWater_Fill_Completed,bWaterlevel_Desired,bStartButton;

extern uint8_t uiLowPressureState,uiNoWaterState,uiLowPressureTime,uiTumblingCount;
extern uint8_t uiError_Mode,uiError_Occured,uiNoWaterTime,uiResetState,uiSmartProgram_Selected;

extern uint16_t iWaterLevel;
extern uint16_t ucMotor_Rotate_Seconds;
extern uint8_t uiprev_error,uiSelected_Position;
extern uint8_t uiStep_Seconds,uiMachineState,uiExtraTime_Seconds;

//*****local variables******//
static boolean result;
extern boolean bNoWater;
//*******local function prototype********//



/*************************************************************************************************************
* Function Name: LowPressure
* Description  : This function is to execute low pressure error condition.
* Arguments    : uint8_t,uint8_t,uint16_t
* Return Value : None
**************************************************************************************************************/
void LowPressure(uint8_t uiDuty_Selected,uint8_t uiValveSelect,uint16_t iTumblelevel)
{
	uiNoWaterTime  = RESET_VALUE;
	//uiError_Occured = NO_WATER_SUPPLY;
	switch(uiLowPressureState)
	{
		case LOW_INTIALISE :	
					MachineErrors.EBits.NoWaterError = RESET_VALUE;
					uiprev_error = 0xFF;
					bWater_Fill_Completed = RESET_VALUE;
					bStop_Minutes_Count = YES;
					uiError_Mode = LOW_PRESSURE_CONDITION;
					uiLowPressureState = TWO_MINUTES_ON;
					bDrainPump = SWITCH_OFF;
					uiNoWaterState = RESET_VALUE;
					
					break;
		case TWO_MINUTES_ON:
						if(uiLowPressureTime < LOWPRESSURE_STEPONE_TIME)
						{
							//uiError_Occured = RESET_VALUE;
							CommonOperation(uiValveSelect,uiDuty_Selected,iTumblelevel);
						}
						else
						{
							uiLowPressureState = EIGHT_MINUTES_ON;
						}	
						MachineErrors.EBits.NoWaterError = 0;
						break;
		case EIGHT_MINUTES_ON: 
						
						if(uiLowPressureTime < LOWPRESSURE_STEPTWO_TIME)
						{
						
							CommonOperation(uiValveSelect,uiDuty_Selected,iTumblelevel);	
						}
						else
						{
							uiLowPressureState = TEN_MINUTES_MOTOR_STOP;
						}
						MachineErrors.EBits.NoWaterError = 0;
						break;
		case TEN_MINUTES_MOTOR_STOP:
						bWaterLevel_Read_Required = TRUE;
						if(uiLowPressureTime < LOWPRESSURE_STEPTHREE_TIME)
						{
							
							bMotor_Start = NO;
							MotorStop();
							Valveselection1(uiValveSelect);   			
						}
						else
						{
							uiLowPressureState = TWENTY_MINUTES_VALVE_OFF;
						}
						MachineErrors.EBits.NoWaterError = 0; 
						break;
		case TWENTY_MINUTES_VALVE_OFF:
						bWaterLevel_Read_Required = TRUE;
						if (uiLowPressureTime < LOWPRESSURE_STEPFOUR_TIME)
						{			  	  
							bPrewashValve = SWITCH_OFF;	
							bMainWashValve = SWITCH_OFF;
							bMotor_Start = NO;
							MotorStop();
						}
						else
						{
							uiLowPressureState = TWENTY_MINUTES_VALVE_ON;
						}
						MachineErrors.EBits.NoWaterError = 1; 
						break;
		case TWENTY_MINUTES_VALVE_ON:
						if(uiLowPressureTime < LOWPRESSURE_STEPFIVE_TIME)
						{
							
							if(iWaterLevel > iTumblelevel)
							{
								bMotor_Start = NO;
								bWaterLevel_Read_Required = TRUE;
								Valveselection(uiValveSelect );   
							}
							
							else
							{
								Valveselection1(uiValveSelect );			
							}
						}
						else
						{
							uiLowPressureTime = LOWPRESSURE_STEPTHREE_TIME;
							uiLowPressureState = TEN_MINUTES_MOTOR_STOP; 
						}
						MachineErrors.EBits.NoWaterError = 1; 
						break;
		default : // No operation required;
			break;
	}
	
}

/*************************************************************************************************************
* Function Name: CommonOperation
* Description  : This function is to execute to check tumblelevel.
* Arguments    : uint8_t,uint8_t,uint16_t
* Return Value : None
**************************************************************************************************************/
void CommonOperation(uint8_t uiValveSelect,uint8_t uiDuty_Selected , uint16_t iTumblelevel )
{
	if(iWaterLevel > iTumblelevel)
	{
		bWaterLevel_Read_Required = TRUE;
		bMotor_Start = NO;
		
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
		bMotor_Start = YES;
		//bDuty_Swicth_Start = NO;
      		RinseTumble(uiDuty_Selected);
		Valveselection1(uiValveSelect); 
	}	
}


/*************************************************************************************************************
* Function Name: NoWater
* Description  : This function is to execute No water error condition.
* Arguments    : uint8_t
* Return Value : None
**************************************************************************************************************/
void NoWater(uint8_t uiValveSelect)
{
	static uint8_t u8NoWater_Time;
	uiError_Mode = NO_WATER_CONDITION; 
	bStop_Minutes_Count = YES;
	bMotor_Start = NO;
	uiLowPressureState = RESET_VALUE;
	if((/*(uiSelected_Position == SMARTPLUS)&&*/(uiSmartProgram_Selected == STEAMWASH /*|| bExpress_30_Activatted == TRUE*/))||(uiSelected_Position == STEAMFRESH))
	{
		u8NoWater_Time = NOWATER_STEPONE_TIME;
	}
	else
	{
		u8NoWater_Time = NOWATER_STEPONE_TIME + 1;
	}
	switch(uiNoWaterState)
	{
	case FIVE_MINUTES_VALVE_ON:
					bWaterLevel_Read_Required = TRUE;
					uiprev_error = 0xFF;
					if(uiNoWaterTime  < u8NoWater_Time)
					{
						/*if(uiTumblingCount == STEAM_FILL)
						{
							SteamFilling();  
						}
						else
						{*/
							Valveselection(uiValveSelect );
						//}
					}
					else
					{
						uiNoWaterState = FIVE_MINUTES_DRAIN_ON;
					}
					MachineErrors.EBits.NoWaterError = RESET_VALUE;
					break;
	case FIVE_MINUTES_DRAIN_ON:
					bWaterLevel_Read_Required = TRUE;
					bNoWater = TRUE;
					if(uiNoWaterTime  < (u8NoWater_Time +5))
					{
						 bDrainPump = SWITCH_ON;		
						 bPrewashValve = SWITCH_OFF;
						 bMainWashValve = SWITCH_OFF;
					}
					else
					{
						uiNoWaterState = FIVE_MINUTES_IDLE;
					}
					if(bNoWater == 1)
					{
						MachineErrors.EBits.NoWaterError = 1;
					}
					else
					{
						MachineErrors.EBits.NoWaterError = 0;
					}
					break;
	case FIVE_MINUTES_IDLE:
					bWaterLevel_Read_Required = TRUE;
					if(uiNoWaterTime  < (u8NoWater_Time +10))
					{
						bDrainPump = SWITCH_OFF;		
					}
					else
					{
						uiNoWaterState = FIVE_MINUTES_VALAVE_ON_AGAIN;
					}
					if(bNoWater == 1)
					{
						MachineErrors.EBits.NoWaterError = 1;
					}
					else
					{
						MachineErrors.EBits.NoWaterError = 0;
					}
					break;
	case FIVE_MINUTES_VALAVE_ON_AGAIN:
					bWaterLevel_Read_Required = TRUE;
					if(uiNoWaterTime  < (u8NoWater_Time +15))
					{
						/*if(uiTumblingCount == STEAM_FILL)
						{
							SteamFilling();  
						}
						else
						{*/
							Valveselection(uiValveSelect );
						//}   
					}
					else
					{
						uiNoWaterState = FIVE_MINUTES_DRAIN_ON;
						uiNoWaterTime  = u8NoWater_Time;  
						bNoWater = 1;
						MachineErrors.EBits.NoWaterError = 1;
					}
					if(bNoWater == 1)
					{
						MachineErrors.EBits.NoWaterError = 1;
					}
					else
					{
						MachineErrors.EBits.NoWaterError = 0;
					}
					break;
	default : // No operation required;
				break;
	}
}


/*************************************************************************************************************
* Function Name: reset_filling
* Description  : This function is to execute when waterlevel is below than minimum level.
* Arguments    : uint8_t,uint16_t
* Return Value : None
**************************************************************************************************************/
boolean reset_filling(uint8_t uiValveSelect, uint16_t iSetlevel)
{
	if(iWaterLevel < iSetlevel)
	{
		bStop_Minutes_Count = NO;
		bDrainPump = SWITCH_OFF;	
		uiLowPressureTime = RESET_VALUE;	
		uiNoWaterTime  = RESET_VALUE;
		uiError_Mode = RESET_VALUE;
		bPrewashValve = SWITCH_OFF;
		MachineErrors.EBits.NoWaterError = 0;
		if((TubClean_Activated == TRUE)&&(iWaterLevel > SIXTEEN_LITER))
		{
			bMainWashValve = SWITCH_ON;	
		}
		else
		{
			bMainWashValve = SWITCH_OFF;
			result = STEP_FINISHED;	
		}
		 	
	}
	else if(iWaterLevel < LOW_PRESSURE)
	{
		ResetLowPressure(uiValveSelect);
		result = STEP_INPROGRESS;
	}
	else
	{
		 NoWater(uiValveSelect);
		 result = STEP_INPROGRESS;
	}		
	return result;
}

/*************************************************************************************************************
* Function Name: Valveselection
* Description  : This function is to select valve from which water should filll.
* Arguments    : uint8_t
* Return Value : None
**************************************************************************************************************/
void Valveselection(uint8_t uiValveSelect)
{	
	if((uiValveSelect == MAIN_WASH) || (uiValveSelect == RINSE_PREWASH) || (uiValveSelect == PRE_VALVE) || (uiValveSelect == SPECIAL_VALVE))
	{
		bPrewashValve = SWITCH_ON;
		bMainWashValve = SWITCH_OFF;	
	}
	
	else if(uiValveSelect == RINSE_MAINWASH)
	{
		bPrewashValve = SWITCH_OFF;
		bMainWashValve = SWITCH_ON;
	}
	// select valve on/off
	else if(uiValveSelect == RINSE_BOTH)
	{
			bPrewashValve = SWITCH_ON;
			if(iWaterLevel < ADDITIVES_MAIN) // bMainWashValve valve after 5 lit
			{
				bMainWashValve = SWITCH_ON;
			}
			else if(iWaterLevel > ADDITIVES_PRE)
			{
				bMainWashValve = SWITCH_OFF;
			}
	}
}

/*************************************************************************************************************
* Function Name: Valveselection1
* Description  : This function is to select valve from which water should filll.
* Arguments    : uint8_t
* Return Value : None
**************************************************************************************************************/
void Valveselection1(uint8_t uiValveSelect)
{
	if((uiValveSelect == MAIN_WASH) || (uiValveSelect == RINSE_MAINWASH) || (uiValveSelect == SPECIAL_VALVE))
	{
		bMainWashValve = SWITCH_ON;
		bPrewashValve = SWITCH_OFF;
	}
	else if((uiValveSelect == RINSE_PREWASH) || (uiValveSelect == PRE_VALVE))
	{
		
			bPrewashValve = SWITCH_ON;
			bMainWashValve = SWITCH_OFF;
	}
	else if(uiValveSelect == RINSE_BOTH)
	{
		 	bPrewashValve = SWITCH_ON;
			if(iWaterLevel < ADDITIVES_MAIN)
			{
				bMainWashValve = SWITCH_ON;
			}
			else if(iWaterLevel > ADDITIVES_PRE)
			{
				bMainWashValve = SWITCH_OFF;
			}
	}
}

/**********************************************************************************************************
* Function Name: RinseTumble
* Description  : This function is to start tumbling after rinsing desired level attained.
* Arguments    : uint8_t
* Return Value : None
**********************************************************************************************************/
void RinseTumble(uint8_t uiDuty_Selected)
{
	motor(uiDuty_Selected);
}

/**********************************************************************************************************
* Function Name: ResetLowPressure
* Description  : This function is to select valve from which water should filll.
* Arguments    : uint8_t
* Return Value : None
**********************************************************************************************************/
void ResetLowPressure(uint8_t uiValveSelect)
{
	switch(uiResetState)
	{
		case LOW_INTIALISE: 
					bStop_Minutes_Count = YES;
					uiNoWaterTime  = RESET_VALUE;
					uiError_Mode = LOW_PRESSURE_CONDITION;
					bMotor_Start = NO;
					uiResetState = TWO_MINUTES_ON;
					MachineErrors.EBits.NoWaterError = 0; 
					break;
		case TWO_MINUTES_ON:
					if(uiLowPressureTime < LOWPRESSURE_STEPONE_TIME)
					{
						MachineErrors.EBits.NoWaterError =  0;
						ResetFill_Valve_Selection(uiValveSelect);
					}
					else
					{
						uiResetState = EIGHT_MINUTES_ON;
					}
					MachineErrors.EBits.NoWaterError = 0;  
					break;
		case EIGHT_MINUTES_ON:
					if(uiLowPressureTime < LOWPRESSURE_STEPTWO_TIME)
					{
						ResetFill_Valve_Selection(uiValveSelect);
					}
					else
					{
						uiResetState = TEN_MINUTES_MOTOR_STOP;
					}
					break;
		case TEN_MINUTES_MOTOR_STOP:  
		
					if(uiLowPressureTime < LOWPRESSURE_STEPTHREE_TIME)
					{
						ResetFill_Valve_Selection(uiValveSelect);
					}
					else
					{
						uiResetState = TWENTY_MINUTES_VALVE_OFF;
					}
					break;
		
		case  TWENTY_MINUTES_VALVE_OFF :
					if (uiLowPressureTime < LOWPRESSURE_STEPFOUR_TIME)
					{			  	  
						 bPrewashValve = SWITCH_OFF;	
						 bMainWashValve = SWITCH_OFF;
					}
					else
					{
						uiResetState = TWENTY_MINUTES_VALVE_ON;
					}
					MachineErrors.EBits.NoWaterError = 1; 
					break;
		
		case  TWENTY_MINUTES_VALVE_ON:
					if(uiLowPressureTime < LOWPRESSURE_STEPFIVE_TIME)
					{
						ResetFill_Valve_Selection(uiValveSelect);
					}
					else
					{
						uiLowPressureTime = LOWPRESSURE_STEPTHREE_TIME;
					}
					MachineErrors.EBits.NoWaterError = 1; 
					break;
		default :	// No operation required
					break;
	}
}

/**********************************************************************************************************
* Function Name: ResetFill_Valve_Selection
* Description  : This function is to select valve from which water should filll.
* Arguments    : uint8_t
* Return Value : None
**********************************************************************************************************/
void ResetFill_Valve_Selection(uint8_t uiValveSelect)
{
	if((uiValveSelect == MAIN_WASH) || (uiValveSelect == RINSE_MAINWASH) || (uiValveSelect == SPECIAL_VALVE))
	{
		bPrewashValve = SWITCH_OFF;
		
		bMainWashValve = SWITCH_ON;
	}
	else if((uiValveSelect == RINSE_PREWASH) || (uiValveSelect == PRE_VALVE))
	{
		
		bMainWashValve = SWITCH_OFF;
		
		bPrewashValve = SWITCH_ON;
	}
	else if(uiValveSelect == RINSE_BOTH)
	{
			
		 	bPrewashValve = SWITCH_ON;
			if(iWaterLevel < ADDITIVES_MAIN)
			{
				
				bMainWashValve = SWITCH_ON;
			}
			else if(iWaterLevel > ADDITIVES_PRE)
			{
				bMainWashValve = SWITCH_OFF;
			}
	}
}
