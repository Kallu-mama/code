#include "waterheat.h"
#include "r_cg_userdefine.h"
#include "ErrorHandling.h"
//******* global variables ********//
extern boolean bStop_Minutes_Count,bMotor_Start,bHeater,bWater_ResetLevel,TubClean_Activated;
extern boolean bNtcFailed,bHeaterOpen,bHotRinse_Start,bMainWashValve,bHeater_Delay_Start;
extern boolean bFaciaTest,bHotRinse_Selected,bCirculation_Pump;

extern uint8_t uiStep_Minutes_Completed,uiAdditional_Heater_TurnOn_Mins,uiError_Mode,uiheat_Minutes_Completed,uiheat_Seconds_Completed;
extern uint8_t uiHeater_TurnOn_Delay_Value,uiHeatState,uiResetState;
extern uint8_t uiSelected_Position,uiStep_Seconds,uiTumblingCount;
extern uint8_t uiStartTemperature,uiSmartProgram_Selected;
extern uint8_t uidisp_select_option,uiTempSel;
extern uint16_t iWaterLevel,iMilliSeconds;
extern uint16_t uiCurrent_step_time;

extern uint8_t uiTemperature; //uiResetFill_Count;

//*****global function prototype******//
extern void motor(uint8_t);
//extern void MotorValues_Reset(void);
extern boolean reset_filling(uint8_t,uint16_t);
extern void MotorStop(void);

//****local variable*****//

boolean bHeatCompleted,bEco60Reset,bTubLevelReach;

/**********************************************************************************************************************
* Function Name: heating
* Description  : This function execute the heating of water to desired temperature.
* Arguments    : uint8_t,uint8_t,uint8_t,
		 uint16_t,uint16_t,uint16_t.
* Return Value : boolean
***********************************************************************************************************************/
boolean heating(uint8_t uiStepTime,uint8_t uiDuty_Selected,uint8_t uiTemperature_Selected,uint16_t iSet_WaterLevel,uint16_t iTumble_WaterLevel,uint16_t iMinimum_WaterLevel)
{
	static boolean result;
	switch(uiHeatState)
	{

		case STATE_HEAT:
//				if(((uiStep_Minutes_Completed == RESET_VALUE)||(uiStep_Minutes_Completed == 10)||
//                                    (uiStep_Minutes_Completed == 20)||(uiStep_Minutes_Completed == 30)||(uiStep_Minutes_Completed == 40)||(uiStep_Minutes_Completed == 50))
//                                    &&(uiStep_Seconds == 2)&&(bNtcFailed == FALSE))
//				{
//					uiStartTemperature = (uint8_t)uiTemperature;
//				}
				if(((uiheat_Minutes_Completed == RESET_VALUE)||(uiheat_Minutes_Completed == 10)||
                                    (uiheat_Minutes_Completed == 20)||(uiheat_Minutes_Completed == 30)||(uiheat_Minutes_Completed == 40)||(uiheat_Minutes_Completed == 50))
                                    &&(uiheat_Seconds_Completed == 2)&&(bNtcFailed == FALSE))
				{
					uiStartTemperature = (uint8_t)uiTemperature;
				}
				if((uiTemperature >= uiTemperature_Selected)&&(bHeatCompleted == FALSE)&&(bNtcFailed == FALSE)&&(uiTemperature < 100))				
				{
					if(((uiSelected_Position == COTTON) && (/*uiTempSel<= Wash95*/((uiTempSel != Wash40E)&&(uiTempSel != Wash60E))) && (uiTumblingCount != HOTRINSE))	//TODO: Added for BTRA_PSB
					||((uiSelected_Position == MIXEDSOIL) &&(uiTumblingCount != HOTRINSE))				//TODO: Continue the step after achieving the heating.
					||((uiSelected_Position == SYNTHETIC) && (uiTumblingCount != HOTRINSE))
					||((uiSelected_Position == SMARTPLUS) && ((uiSmartProgram_Selected == EXPRESS_30) || (uiSmartProgram_Selected == LINENWASH))))//(uiSelected_Position == EXPRESS_30)
					//||(uiSmartProgram_Selected == LINENWASH))
					{
						bHeatCompleted = TRUE;
					}
					
					else if(((uiSelected_Position == EXPRESS)||(((uiTempSel == Wash30)||(uiTempSel == P_30)/*(uiTempSel == Wash20)*/)&&((uiSelected_Position != CRADLEWASH)/*&&(uiSelected_Position != WETCLEAN)*/)&&(uiTumblingCount != HOTRINSE)))&&(bFaciaTest == FALSE))
					{
						bHeatCompleted = TRUE;
					}
					else
					{
						bHeatCompleted = FALSE;
						uiHeatState = STATE_HEAT_FINISHED;
					}
				}
				else if(uiStep_Minutes_Completed >= uiStepTime)
				{
	
					if((bNtcFailed == TRUE) ||(bHeaterOpen == TRUE)||(bHeatCompleted == TRUE))
					{					
						bHeater = SWITCH_OFF;	
						uiHeatState = STATE_HEAT_FINISHED;
					}
					else if(bHeatCompleted == FALSE)
					{
						uiError_Mode = TEMPEATURE_NOT_ATTAINED;
						bStop_Minutes_Count = YES;
						bHeater = SWITCH_ON;
						if((bHotRinse_Selected == TRUE)||((uiSelected_Position == COTTON)&&/*(uiTempSel >= Wash40E)*/((uiTempSel == Wash40E)||(uiTempSel == Wash60E)))||(uiSelected_Position == EXPRESS)|| ((uiSelected_Position == SMARTPLUS) && (uiSmartProgram_Selected == EXPRESS_30)))//(uiSelected_Position == EXPRESS_30))
						{
							uiHeatState = STATE_HEAT_FINISHED;
						}
						else
						{							
							uiHeatState = STATE_EXTRA_HEAT_TEN_MINUTES;
						}
					}
				}
//				else if((((uiStep_Minutes_Completed == 10)||(uiStep_Minutes_Completed == 20)||
//                                        (uiStep_Minutes_Completed == 30)||(uiStep_Minutes_Completed == 40)||(uiStep_Minutes_Completed == 50)||(uiStep_Minutes_Completed == 60))
//                                        &&(uiStep_Seconds == 0)&&(iMilliSeconds == 0)&&((uiTemperature <=(uiStartTemperature + 2))||(uiTemperature == 232))) && (bHeatCompleted == FALSE))

				else if((((uiheat_Minutes_Completed == 10)||(uiheat_Minutes_Completed == 20)||
                                        (uiheat_Minutes_Completed == 30)||(uiheat_Minutes_Completed == 40)||
					(uiheat_Minutes_Completed == 50)||(uiheat_Minutes_Completed == 60))
                                        &&(uiheat_Seconds_Completed == 0)&&(iMilliSeconds == 0)&&((uiTemperature <=(uiStartTemperature + 2))
					||(uiTemperature == 232))) && (bHeatCompleted == FALSE))
					{
						bHeater = SWITCH_OFF;
						bHeaterOpen = TRUE;
						if(bNtcFailed == TRUE)
						{
							Store_errors(E3);
						}
						else
						{
							Store_errors(E1);
						}
					}
				
				//If iWaterLevel  Reached to  iMinimum_WaterLevel?
				/*else */if(iWaterLevel < iMinimum_WaterLevel)
				{	
					NormalHeat(uiDuty_Selected);
				}
				else
				{
					uiHeatState = STATE_RESET_INTIALIZATION;					// motor reset
				}
				
				bEco60Reset = TRUE;
				result = STEP_INPROGRESS;
			break;
		case STATE_EXTRA_HEAT_TEN_MINUTES:
				// If uiTemperature is Reached uiTemperature_Selected?
				uiCurrent_step_time = RESET_VALUE;
				if(uiTemperature >= uiTemperature_Selected)
				{
					uiHeatState = STATE_HEAT_FINISHED;
				}
				else if(uiAdditional_Heater_TurnOn_Mins >= ADDTITIONAL_HEAT_MINUTES)
				{
					uiHeatState = STATE_HEAT_FINISHED;
					uiError_Mode = RESET_VALUE;
					uiAdditional_Heater_TurnOn_Mins = RESET_VALUE;
				}
				//If iWaterLevel  Reached to  iMinimum_WaterLevel?
				if(iWaterLevel < iMinimum_WaterLevel)
				{
					bStop_Minutes_Count = YES;
					uiError_Mode = TEMPEATURE_NOT_ATTAINED;
					ExtraHeat(uiDuty_Selected);
				}
				else
				{
					uiHeatState = STATE_RESET_INTIALIZATION;
				}
				
				result = STEP_INPROGRESS;
				break;
		case STATE_RESET_INTIALIZATION:
					bCirculation_Pump = SWITCH_OFF;
					bMotor_Start = NO;		                 // tumbling stop
					bStop_Minutes_Count = YES;	                 // stop minutes completed
					bWater_ResetLevel = YES;	                //enable water reset flag , to fill the water
					bHeater_Delay_Start = RESET_VALUE;	        //reset delay start
					uiHeater_TurnOn_Delay_Value = 4;	// reset heater delay 2 seconds to turn on after 2 seconds
					bHeater = SWITCH_OFF;			        // switch off the heater
					//MotorValues_Reset();
					uiHeatState= STATE_HEAT_RESET_FILLING;
					
					result = STEP_INPROGRESS;
					break;
		case STATE_HEAT_RESET_FILLING:
					if(reset_filling(MAIN_WASH,iSet_WaterLevel))
					{
						//uiResetFill_Count++;
						uiHeatState = STATE_HEAT;
						uiResetState = RESET_VALUE;
					}
					bCirculation_Pump = SWITCH_OFF;
					
					result = STEP_INPROGRESS;
			break;
		case STATE_HEAT_FINISHED:
					bCirculation_Pump = SWITCH_OFF;
					bHeater = SWITCH_OFF;	// switch off the heater
					bMainWashValve = SWITCH_OFF;
					bEco60Reset = TRUE;
					bTubLevelReach = FALSE;
					bStop_Minutes_Count = NO;
					MotorStop();
					bHeatCompleted = FALSE;
					uiStartTemperature = RESET_VALUE;
					uiAdditional_Heater_TurnOn_Mins = RESET_VALUE;
					result = STEP_FINISHED;
					uiHeatState = RESET_VALUE;
					bHeaterOpen = RESET_VALUE;
					break;
			default : 
					//No operation required
					if(uiHeatState == 0xFF)
					{
						uiHeatState = STATE_HEAT;
						result = STEP_INPROGRESS;
					}
			break;
	}
	return result;	// return 0
}
/**************************************************************************************************************
* Function Name: TumbleHeat
* Description  : This function called for heating operation.
* Arguments    : uint8_t
* Return Value : boolean
***************************************************************************************************************/
void TumbleHeat(uint8_t uiDuty_Selected)
{
	bMotor_Start = YES;	
	motor(uiDuty_Selected);
	bHeater_Delay_Start = YES;
	bStop_Minutes_Count = NO;
	if((uiHeater_TurnOn_Delay_Value == COMPLETED)&&(bNtcFailed == FALSE)&&(bHeaterOpen == FALSE)&&(bHeatCompleted == FALSE))
	{
		bHeater = SWITCH_ON;
	}
	else
	{
		bHeater = SWITCH_OFF;
	}
}
/**************************************************************************************************************
* Function Name: NormalHeat
* Description  : This function called for heating operation.
* Arguments    : uint8_t
* Return Value : boolean
***************************************************************************************************************/
void NormalHeat(uint8_t uiDuty_Selected)
{
	bMotor_Start = YES;	
	motor(uiDuty_Selected);
	bHeater_Delay_Start = YES;
	bStop_Minutes_Count = NO;
	if((TubClean_Activated == TRUE)&&(iWaterLevel > SIXTEEN_LITER)&&(bTubLevelReach == FALSE))
	{
		bMainWashValve = SWITCH_ON;
	}
	else
	{
		bTubLevelReach = TRUE;
		bMainWashValve = SWITCH_OFF;
	}
	//If uiHeater_TurnOn_Delay_Value is   COMPLETED?
	if((uiHeater_TurnOn_Delay_Value == COMPLETED)&&(bNtcFailed == FALSE)&&(bHeaterOpen == FALSE)&&(bHeatCompleted == FALSE))
	{
		bHeater = SWITCH_ON;
	}
	else
	{
		bHeater = SWITCH_OFF;
	}
}
/*************************************************************************************************
* Function Name: Heating_NoExtraTime
* Description  : This function called if desired temparature not attained in specified time.
* Arguments    : uint8_t
* Return Value : boolean
**************************************************************************************************/
void ExtraHeat(uint8_t uiDuty_Selected)
{
	bHeater = SWITCH_ON;
	bMotor_Start = YES;
	motor(uiDuty_Selected);
}
