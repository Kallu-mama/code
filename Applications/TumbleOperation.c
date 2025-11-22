
#include "TumbleOperation.h"
#include "r_cg_userdefine.h"
#include "ErrorHandling.h"
//************** global variables **********//
extern boolean bStop_Minutes_Count,bMotor_Start,bPrewashValve,bMainWashValve,bHalfMinute_Process_Start;
extern boolean bWater_ResetLevel,bHeater,bNtcFailed,bCoolDown_Process_Start,bCirculation_Pump,bHeaterOpen,TubClean_Activated;
extern uint8_t uiStep_Minutes_Completed,uiHalfMinute_Completed,uiHalfMinutes_Seconds,uiheat_Seconds_Completed,uiheat_Minutes_Completed;
extern uint8_t uiCoolDown_Seconds,uiTumbleState,uiStepState,uiPresent_Step,uiStep_Seconds;
extern uint8_t uiReheatTime,uiTemperature_Required;
extern uint8_t uiStartTemperature,uiTumblingCount;
extern uint16_t iWaterLevel,iMilliSeconds;

extern uint8_t uiTemperature,uiTempSel,uiTemperature_Required;
extern uint8_t	uiSelected_Position,uiSmartProgram_Selected,uiAISelected_Position;//uiResetFill_Count;
//*****global function prototype******//
//extern void MotorValues_Reset(void);
extern void motor(uint8_t);
extern boolean reset_filling(uint8_t,uint16_t);
extern void MotorStop();

//****local variable*****//
boolean bReheatHeater;


/**********************************************************************************************************************
* Function Name: Tumbling
* Description  : This function executes tumbling operation in mainwash step.
* Arguments    : uint8_t,uint8_t,uint16_t,uint16_t,uint16_t,uint8_t
* Return Value : boolean
***********************************************************************************************************************/
boolean Tumbling(uint8_t uiStepTime,uint8_t uiDuty_Selected,uint16_t iMinimum_WaterLevel,uint16_t iSet_WaterLevel,uint16_t iTumble_WaterLevel,uint8_t ValveSelected)
{
	static boolean result;
	switch(uiTumbleState)
	{
		case STATE_TUMBLE: 
				if((((uiSelected_Position == MIXEDSOIL)||(uiAISelected_Position == MIXEDSOIL)) && 
				((uiPresent_Step == 10)||(uiPresent_Step == 11)||(uiPresent_Step == 12) || 
				(uiPresent_Step == 13) ||(uiPresent_Step == 14)||
				(uiPresent_Step == 15))))//||((uiSelected_Position == WETCLEAN)&&(uiPresent_Step < 14))) //added for MIXED TCS & wetclean
				{
					if(uiStep_Seconds >= 30)
					{
						bMotor_Start = NO;
						MotorStop();
			 			uiTumbleState = STATE_TUMBLE_COMPLETED;
					}
					else if(iWaterLevel < iMinimum_WaterLevel) 
					{
						bPrewashValve = SWITCH_OFF;
						bStop_Minutes_Count = NO;
						bMotor_Start = YES;
						motor(uiDuty_Selected);
					}
				}
				else if(uiStepTime <= uiStep_Minutes_Completed)
				{
					bMotor_Start = NO;
					MotorStop();
			 		uiTumbleState = STATE_TUMBLE_COMPLETED;
					
				}
				else if((iWaterLevel < iMinimum_WaterLevel)||(uiDuty_Selected == DUTY_C11)||(uiDuty_Selected == DUTY_C12)||(uiDuty_Selected == DUTY_C13)||(uiDuty_Selected == DUTY_C14)/*||(uiDuty_Selected == DUTY_C25)*/
				||(bCirculation_Pump == SWITCH_ON)) //added duty C25 in the condition: PW valve used to remain on for the entire rinse step
				{
					bPrewashValve = SWITCH_OFF;
					bStop_Minutes_Count = NO;
					bMotor_Start = YES;
					motor(uiDuty_Selected);
				}
				else
				{
					bMotor_Start = NO;
					bStop_Minutes_Count = YES;
					uiTumbleState = STATE_TUMBLE_FILL;
					//MotorValues_Reset();
					
				}
				
				result = STEP_INPROGRESS;
				break;
					
		case STATE_TUMBLE_FILL :
				if(reset_filling(ValveSelected,iSet_WaterLevel))
				{
					//uiResetFill_Count++;
					uiTumbleState = STATE_TUMBLE;
				}
				
				bCirculation_Pump = SWITCH_OFF;
				result = STEP_INPROGRESS;
				break;
						
		case STATE_TUMBLE_COMPLETED:
				result = STEP_FINISHED;
				bCirculation_Pump = SWITCH_OFF;
				uiTumbleState = RESET_VALUE;
				break;

		default:
			// no operation required
			//if(uiTumbleState == 0xFF)
			//{
				uiTumbleState = STATE_TUMBLE;
				result = STEP_INPROGRESS;
			//}
			break;
	}
	return result;
}


/**********************************************************************************************************************
* Function Name: TumblingSpecial
* Description  : This function executes half minutes tumbling mainwash step.
* Arguments    : uint8_t,uint8_t,uint16_t,uint16_t,uint16_t,uint8_t
* Return Value : boolean
***********************************************************************************************************************/
boolean TumblingSpecial(uint8_t uiStepTime,uint8_t uiDuty_Selected,uint16_t iMinimum_WaterLevel,uint16_t iSet_WaterLevel,uint16_t iTumble_WaterLevel,uint8_t ValveSelected)
{
	static boolean result;
	static boolean heat_done;
	switch(uiTumbleState)
	{
		case STATE_TUMBLE: 
				if((((uiDuty_Selected == DUTY_X3)||(uiDuty_Selected == DUTY_X5)||(uiDuty_Selected == DUTY_X6)||(uiDuty_Selected == DUTY_X7)||(uiDuty_Selected == DUTY_N9))&&(uiHalfMinutes_Seconds >=29))
				||((uiDuty_Selected == DUTY_X9)&&(uiHalfMinutes_Seconds >=25)))
				{
					uiHalfMinute_Completed = RESET_VALUE;
					bHalfMinute_Process_Start = RESET_VALUE;
					uiHalfMinutes_Seconds = RESET_VALUE;
					uiTumbleState = STATE_TUMBLE_COMPLETED;	
				}
				else if((((uiDuty_Selected == DUTY_X1)||(uiDuty_Selected == DUTY_X2))&&(uiHalfMinute_Completed == 1)&&(uiHalfMinutes_Seconds >=3))
				||((uiDuty_Selected == DUTY_X4)&&(uiHalfMinute_Completed == 1)&&(uiHalfMinutes_Seconds >=11))
				||((uiDuty_Selected == DUTY_X8)&&(uiHalfMinute_Completed == 1)&&(uiHalfMinutes_Seconds >=19)))
				{
					uiHalfMinute_Completed = RESET_VALUE;
					bHalfMinute_Process_Start = RESET_VALUE;
					uiHalfMinutes_Seconds = RESET_VALUE;
					uiTumbleState = STATE_TUMBLE_COMPLETED;	
				}
				else if(iWaterLevel < iMinimum_WaterLevel)
				{
					bMotor_Start = YES;
					bHalfMinute_Process_Start = YES;
					bStop_Minutes_Count = NO;
					motor(uiDuty_Selected);
				}
				else
				{
					bMotor_Start = NO;
					bStop_Minutes_Count = YES;
					uiTumbleState = STATE_TUMBLE_FILL;
				}
				//if(uiSelected_Position == EXPRESS_30)
				if((uiSelected_Position == SMARTPLUS) && (uiSmartProgram_Selected == EXPRESS_30))
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
				
				result = STEP_INPROGRESS;
				break;
					
		case STATE_TUMBLE_FILL :
				if(reset_filling(ValveSelected,iSet_WaterLevel))
				{
					//uiResetFill_Count++;
					uiTumbleState = STATE_TUMBLE;

				}
				bHeater = SWITCH_OFF;
				bCirculation_Pump = SWITCH_OFF;
				result = STEP_INPROGRESS;
				break;
						
		case STATE_TUMBLE_COMPLETED:
				bHeater = SWITCH_OFF;
				heat_done = FALSE;
				result = STEP_FINISHED;
				bCirculation_Pump = SWITCH_OFF;
				uiTumbleState = RESET_VALUE;
				break;

		default :	
				// no operation required
				//if(uiTumbleState == 0xFF)
				//{
					uiTumbleState = STATE_TUMBLE;
					result = STEP_INPROGRESS;
				//}
				break;
	}
	return result;
}

/**********************************************************************************************************************
* Function Name: TumblingSpecialHeat
* Description  : This function executes ReHeating cycle in mainwash step.
* Arguments    : uint8_t,uint8_t,uint16_t,uint16_t,uint16_t,uint8_t
* Return Value : boolean
***********************************************************************************************************************/
boolean TumblingSpecialHeat(uint8_t uiStepTime,uint8_t uiDuty_Selected,uint16_t iMinimum_WaterLevel,uint16_t iSet_WaterLevel,uint8_t uiCutin_Temperature,uint8_t ValveSelected)
{
	static boolean result;
	
	if(uiTumblingCount == SOAK)	uiTemperature_Required = TEMP_40; //For Warm Soak
	
	switch(uiTumbleState)
	{
		case STATE_TUMBLE: 		
//						if(((uiStep_Minutes_Completed == RESET_VALUE)||(uiStep_Minutes_Completed == 10)||(uiStep_Minutes_Completed == 20)
//                                    		||(uiStep_Minutes_Completed == 30)||(uiStep_Minutes_Completed == 40)||(uiStep_Minutes_Completed == 50))
//                                    		&&(uiStep_Seconds == 2)&&(uiTemperature <=(uiTemperature_Required - 3)))

						if(((uiheat_Minutes_Completed == RESET_VALUE)||(uiheat_Minutes_Completed == 10)||(uiheat_Minutes_Completed == 20)
                                    		||(uiheat_Minutes_Completed == 30)||(uiheat_Minutes_Completed == 40)||(uiheat_Minutes_Completed == 50))
                                    		&&(uiheat_Seconds_Completed == 2)&&(uiTemperature <=(uiTemperature_Required - 3)))
						{
							uiStartTemperature = (uint8_t)uiTemperature;
						}
						else if((((uiSelected_Position == MIXEDSOIL)||(uiAISelected_Position == MIXEDSOIL)) && 
						((uiPresent_Step == 10) || 
						(uiPresent_Step == 12) ||
						(uiPresent_Step == 14))))//||(uiSelected_Position == WETCLEAN)) //added for MIXED TCS & wetclean
						{
							if(uiStep_Seconds >= 30)
							{
								bMotor_Start = NO;
								MotorStop();
			 					uiTumbleState = STATE_TUMBLE_COMPLETED;
							}
							else if(iWaterLevel < iMinimum_WaterLevel) 
							{
								bPrewashValve = SWITCH_OFF;
								bStop_Minutes_Count = NO;
								bMotor_Start = YES;
								motor(uiDuty_Selected);
							}
						}
						else if(uiStepTime <= uiStep_Minutes_Completed)
						{
							bMotor_Start = NO;
							MotorStop();
					 		uiTumbleState = STATE_TUMBLE_COMPLETED;
						}
//						else if(((uiStep_Minutes_Completed == 10)||(uiStep_Minutes_Completed == 20)||
//                                        	(uiStep_Minutes_Completed == 30)||(uiStep_Minutes_Completed == 40)||(uiStep_Minutes_Completed == 50)||(uiStep_Minutes_Completed == 60))
//                                        	&&(uiStep_Seconds == 0)&&(iMilliSeconds == 0)&&((uiTemperature <=(uiStartTemperature + 2))||(uiTemperature == 232)))
//						
						else if(((uiheat_Minutes_Completed == 10)||(uiheat_Minutes_Completed == 20)||
                                        	(uiheat_Minutes_Completed == 30)||(uiheat_Minutes_Completed == 40)||(uiheat_Minutes_Completed == 50)||(uiheat_Minutes_Completed == 60))
                                        	&&(uiheat_Seconds_Completed == 0)&&(iMilliSeconds == 0)&&((uiTemperature <=(uiStartTemperature + 2))||(uiTemperature == 232)))
						{
//							bHeater = SWITCH_OFF;
//							bHeaterOpen = TRUE;
//							if(bNtcFailed == TRUE)
//							{
//								Store_errors(E3);
//							}
//							else
//							{
//								uiTest = uiTest | 0x01;
//								Store_errors(E1);
//							}
							//below logic is added to avoid htr error in antiallergen due to cutin temp
							//if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected != ANTIALLERGEN))
							if((uiSmartProgram_Selected != ANTIALLERGEN)&&(TubClean_Activated==FALSE)&&(uiSmartProgram_Selected != UNIFORM))
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
//							else if(((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == ANTIALLERGEN))&&
//							((uiTempSel == Wash40 && uiTemperature < 37) || 
//							(uiTempSel == Wash60 && uiTemperature < 53) ||
//							(uiTempSel == Wash95 && uiTemperature < 76)))
							else if((((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == ANTIALLERGEN))&&
							(((uiTempSel == Wash40 || uiTempSel == P_40) && uiTemperature < 37) || 
							((uiTempSel == Wash60 || uiTempSel == P_60) && uiTemperature < 53) ||
							((uiTempSel == Wash95 || uiTempSel == P_95) && uiTemperature < 76))) ||
							((TubClean_Activated==TRUE)&& (((uiTempSel == Wash60) && (uiTemperature < 56)) ||
							((uiTempSel == Wash95) && (uiTemperature < 76)))) ||
							(((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == UNIFORM)) && (((uiTempSel == Wash40) && (uiTemperature < 36))||
							((uiTempSel == Wash60) && (uiTemperature < 56)))))
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
						}
						else if(iWaterLevel < iMinimum_WaterLevel)
						{
							bPrewashValve = SWITCH_OFF;
							bStop_Minutes_Count = NO;
							
							bMotor_Start = YES;
							motor(uiDuty_Selected);
							if((uiTemperature < uiTemperature_Required)&&(uiStep_Seconds == 4)&&(uiStep_Minutes_Completed == RESET_VALUE))
							{
								uiTumbleState = STATE_TUMBLEHEAT;
							}
							//else if((uiTemperature <= (uiTemperature_Required - uiCutin_Temperature)) && (uiStep_Minutes_Completed < TEN_MINUTES))
							else if((uiTemperature <= (uiTemperature_Required - uiCutin_Temperature)) && (uiStep_Minutes_Completed < FIFTY_FIVE_MINUTES))
							{
								uiTumbleState = STATE_TUMBLEHEAT;
							}
							else if((uiTumblingCount == SOAK)&&(uiStep_Minutes_Completed >= TEN_MINUTES))//within 10 min Heat & Reheat takes place upto only 40*C , Reheat starts
							{
								bHeater = SWITCH_OFF;						//when temp reaches to 30*C, After 10 min CP should on for remaining step time. 
								bCirculation_Pump = SWITCH_ON;
								bReheatHeater = FALSE;						//Safer Side CP on step added in both STATE_TUMBLE & STATE_TUMBLEHEAT sate.
							}
							else if(uiTemperature >= uiTemperature_Required)
							{
								bHeater = SWITCH_OFF;
								bHeaterOpen = FALSE;
								bReheatHeater = FALSE;	
							}
						}
						else
						{
							bMotor_Start = NO;
							bStop_Minutes_Count = YES;
							uiTumbleState = STATE_TUMBLE_FILL;
							//MotorValues_Reset();
							bHeater = SWITCH_OFF;
						}
						
						result = STEP_INPROGRESS;
						break;
					
		case STATE_TUMBLE_FILL :
						if(reset_filling(ValveSelected,iSet_WaterLevel))
						{
							//uiResetFill_Count++;
							uiTumbleState = STATE_TUMBLE;
						}
						
						bCirculation_Pump = SWITCH_OFF;
						result = STEP_INPROGRESS;
						break;
						
		case STATE_TUMBLE_COMPLETED:
						result = STEP_FINISHED;
						bMotor_Start = NO;
						MotorStop();
						bCirculation_Pump = SWITCH_OFF;
						uiTumbleState = RESET_VALUE;
						uiStartTemperature = RESET_VALUE;
						if(uiPresent_Step >=  10)
						{
							bHeater = SWITCH_OFF;
							bReheatHeater = FALSE;
						}
						break;
		case STATE_TUMBLEHEAT:
//						if(((uiStep_Minutes_Completed == RESET_VALUE)||(uiStep_Minutes_Completed == 10)||(uiStep_Minutes_Completed == 20)
//                                    		||(uiStep_Minutes_Completed == 30)||(uiStep_Minutes_Completed == 40)||(uiStep_Minutes_Completed == 50))
//                                    		&&(uiStep_Seconds == 2)&&(uiTemperature <=(uiTemperature_Required - 3)))
						if(((uiheat_Minutes_Completed == RESET_VALUE)||(uiheat_Minutes_Completed == 10)||(uiheat_Minutes_Completed == 20)
                                    		||(uiheat_Minutes_Completed == 30)||(uiheat_Minutes_Completed == 40)||(uiheat_Minutes_Completed == 50))
                                    		&&(uiheat_Seconds_Completed == 2)&&(uiTemperature <=(uiTemperature_Required - 3)))
						{
							uiStartTemperature = (uint8_t)uiTemperature;
						}
						if(uiTemperature >= uiTemperature_Required)
						{
							bHeater = SWITCH_OFF;
							bHeaterOpen = FALSE;
							uiTumbleState = STATE_TUMBLE;
						}
						else if(uiStepTime <= uiStep_Minutes_Completed)
						{
							uiTumbleState = STATE_TUMBLE_COMPLETED;	
							bHeater = SWITCH_OFF;
							//MotorValues_Reset();
						}
//						else if(((uiStep_Minutes_Completed == 10)||(uiStep_Minutes_Completed == 20)||
//                                        	(uiStep_Minutes_Completed == 30)||(uiStep_Minutes_Completed == 40)||(uiStep_Minutes_Completed == 50)||(uiStep_Minutes_Completed == 60))
//                                        	&&(uiStep_Seconds == 0)&&(iMilliSeconds == 0)&&((uiTemperature <=(uiStartTemperature + 2))||(uiTemperature == 232)))
						else if(((uiheat_Minutes_Completed == 10)||(uiheat_Minutes_Completed == 20)||
                                        	(uiheat_Minutes_Completed == 30)||(uiheat_Minutes_Completed == 40)||(uiheat_Minutes_Completed == 50)||(uiheat_Minutes_Completed == 60))
                                        	&&(uiheat_Seconds_Completed == 0)&&(iMilliSeconds == 0)&&((uiTemperature <=(uiStartTemperature + 2))||(uiTemperature == 232)))
						{
//							bHeater = SWITCH_OFF;
//							bHeaterOpen = TRUE;
//							if(bNtcFailed == TRUE)
//							{
//								Store_errors(E3);
//							}
//							else
//							{
//								Store_errors(E1);
//							}
							//below logic is added to avoid htr error in antiallergen due to cutin temp
							//if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected != ANTIALLERGEN))
							if((uiSmartProgram_Selected != ANTIALLERGEN)&&(TubClean_Activated==FALSE)&&(uiSmartProgram_Selected != UNIFORM))
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
//							else if(((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == ANTIALLERGEN))&&
//							((uiTempSel == Wash40 && uiTemperature < 37) || 
//							(uiTempSel == Wash60 && uiTemperature < 53) ||
//							(uiTempSel == Wash95 && uiTemperature < 76)))
							else if((((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == ANTIALLERGEN))&&
							(((uiTempSel == Wash40 || uiTempSel == P_40) && uiTemperature < 37) || 
							((uiTempSel == Wash60 || uiTempSel == P_60) && uiTemperature < 53) ||
							((uiTempSel == Wash95 || uiTempSel == P_95) && uiTemperature < 76))) ||
							((TubClean_Activated==TRUE)&& (((uiTempSel == Wash60) && (uiTemperature < 56)) ||
							((uiTempSel == Wash95) && (uiTemperature < 76)))) ||
							(((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == UNIFORM)) && (((uiTempSel == Wash40) && (uiTemperature < 36))||
							((uiTempSel == Wash60) && (uiTemperature < 56)))))
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
						}
						else if(iWaterLevel < iMinimum_WaterLevel)
						{	
							 if((uiTumblingCount == SOAK)&&(uiStep_Minutes_Completed >= TEN_MINUTES))
							{
								bHeater = SWITCH_OFF;
								bCirculation_Pump = SWITCH_ON;
								bReheatHeater = FALSE;
							}	
							//else if((uiReheatTime < 40)&&(bNtcFailed == FALSE)&&(bHeaterOpen == FALSE))
							else if((uiReheatTime < 55)&&(bNtcFailed == FALSE)&&(bHeaterOpen == FALSE))
							{
								bHeater = SWITCH_ON;
								bCirculation_Pump = SWITCH_OFF;
								bReheatHeater = TRUE;
							}
							else
							{
								bHeater = SWITCH_OFF;
								bReheatHeater = FALSE;
							}
							
							bMotor_Start = YES;
							motor(uiDuty_Selected);
							//bMotor_Start = YES;
							bHalfMinute_Process_Start = NO;
							bStop_Minutes_Count = NO;
							//motor(uiDuty_Selected);
						}
						else
						{
							bMotor_Start = NO;
							bStop_Minutes_Count = YES;
							uiTumbleState = STATE_TUMBLE_FILL;
							//MotorValues_Reset();
							bHeater = SWITCH_OFF;
						}
						break;

		default :
			// no operation required
			//if(uiTumbleState == 0xFF)
			//{
					uiTumbleState = STATE_TUMBLE;
					result = STEP_INPROGRESS;
			//}
			break;
	}
	return result;
	
}

/**********************************************************************************************************************
* Function Name: anticrease
* Description  : This function executes to remove creaing from cloths after final spin cycle.
* Arguments    : uint8_t,uint8_t
* Return Value : boolean
***********************************************************************************************************************/
boolean anticrease(uint8_t uiStepTime,uint8_t uiDuty_Selected)
{
	static boolean result;
	bStop_Minutes_Count = NO;
	if(uiStepTime == uiStep_Minutes_Completed)
	{
		bCirculation_Pump = SWITCH_OFF;
		result = STEP_FINISHED;
	}
	else
	{
		if(uiDuty_Selected != DUTY_IDLE)
		{
			bMotor_Start = YES;
			motor(uiDuty_Selected);
		}
		else
		{
			bMotor_Start = NO;
			bCirculation_Pump = SWITCH_ON;
		}
		result = STEP_INPROGRESS;
	}
	return result;
}

/**********************************************************************************************************************
* Function Name: cooldown
* Description  : This function executes to cool down water tempearture below 60C.
* Arguments    : uint8_t,uint8_t,uint16_t,uint16_t,uint16_t
* Return Value : boolean
***********************************************************************************************************************/
boolean cooldown(uint8_t uiStepTime,uint8_t uiDuty_Selected,uint16_t iSetlevel,uint16_t iSet_WaterLevel,uint16_t iTumble_WaterLevel)
{
	static boolean result;
	
	switch(uiStepState)
	{
		case STATE_COOL :
						if(uiStepTime <= uiStep_Minutes_Completed)
						{
							 bCoolDown_Process_Start = NO;
							 uiCoolDown_Seconds = RESET_VALUE;
							 bMainWashValve = SWITCH_OFF;
							 uiStepState = STATE_COOL_FINISHED;
						}
						else if((iWaterLevel < iSetlevel)||(((uiSelected_Position == MIXEDSOIL)||(uiAISelected_Position == MIXEDSOIL))&&(iWaterLevel < MIXED_STEAM_RESET_LEVEL)))//Added for MIXED TCS changes as freq will be changed due to halfdrain.
						{
							if(uiTemperature >= COOLDOWN_TEMPERATURE)
							{
								if(iWaterLevel > COOL_DOWN_LIMIT_FREQUENCY)
								{
									Cooling(uiDuty_Selected);
								}
								else
								{
									 bCoolDown_Process_Start = NO;
									 bMainWashValve = SWITCH_OFF;
									 uiCoolDown_Seconds = RESET_VALUE;
									 uiStepState = STATE_COOL_FINISHED;
								 }
							}
							else
							{
								bCoolDown_Process_Start=NO;
								uiCoolDown_Seconds = RESET_VALUE;
								bMainWashValve = SWITCH_OFF;
								uiStepState = STATE_COOL_FINISHED;
							}
						}
						else
						{
							bMotor_Start = NO;
							bStop_Minutes_Count = YES;
							uiTumbleState = STATE_TUMBLE_FILL;
							//MotorValues_Reset();
							uiStepState = STATE_COOL_RESET_FILL;
						}
						result = STEP_INPROGRESS;
						break;
						
		case  STATE_COOL_RESET_FILL:
						if(reset_filling(MAIN_WASH,iSet_WaterLevel))
						{
							//uiResetFill_Count++;
							uiTumbleState = STATE_TUMBLE;
							uiStepState = STATE_COOL;
						}
						result = STEP_INPROGRESS;
						break;
						
		case STATE_COOL_FINISHED : 
						result = STEP_FINISHED;
						uiStepState = RESET_VALUE;
						break;

		default : // no operation required
			break;
	}
	
	return result;
}

/**********************************************************************************************************************
* Function Name: Cooling
* Description  : This function executes waterfilling in cool down step.
* Arguments    : uint8_t
* Return Value : void
***********************************************************************************************************************/
void Cooling(uint8_t uiDuty_Selected)
{
	bCoolDown_Process_Start = YES;
	if(uiCoolDown_Seconds < SIX_SECONDS)
	{
	 	bMainWashValve = SWITCH_ON;
	}
	else if(uiCoolDown_Seconds < TWENTY_SECONDS)
	{
		bMainWashValve = SWITCH_OFF;
	}
	else if(uiCoolDown_Seconds > TWENTY_SECONDS)
	{
		uiCoolDown_Seconds = RESET_VALUE;
	}
		bMotor_Start = YES;
		motor(uiDuty_Selected);
		bStop_Minutes_Count = NO;
}

/**********************************************************************************************************************
* Function Name: Steam_Heat
* Description  : This function executes ReHeating cycle in Steam step.
* Arguments    : uint8_t,uint8_t,uint16_t,uint16_t,uint16_t,uint8_t
* Return Value : boolean
***********************************************************************************************************************/
boolean SteamHeat(uint8_t uiStepTime,uint8_t uiDuty_Selected,uint16_t iMinimum_WaterLevel,uint16_t iSet_WaterLevel,uint8_t uiCutin_Temperature,uint8_t ValveSelected)
{
	static boolean result;

	switch(uiTumbleState)
	{
		case STATE_TUMBLE: 		
//						if(((uiStep_Minutes_Completed == RESET_VALUE)||(uiStep_Minutes_Completed == 11)||(uiStep_Minutes_Completed == 22)
//                                    		||(uiStep_Minutes_Completed == 33)||(uiStep_Minutes_Completed == 44)||(uiStep_Minutes_Completed == 55))
//                                    		&&(uiStep_Seconds == 2)&&(uiTemperature <=(uiTemperature_Required - 3)))
						
						if(((uiheat_Minutes_Completed == RESET_VALUE)||(uiheat_Minutes_Completed == 11)||(uiheat_Minutes_Completed == 22)
                                    		||(uiheat_Minutes_Completed == 33)||(uiheat_Minutes_Completed == 44)||(uiheat_Minutes_Completed == 55))
                                    		&&(uiheat_Seconds_Completed == 2)&&(uiTemperature <=(uiTemperature_Required - 3)))
						{
							uiStartTemperature = (uint8_t)uiTemperature;
						}
						if(uiStepTime <= uiStep_Minutes_Completed)
						{
							bMotor_Start = NO;
							MotorStop();
					 		uiTumbleState = STATE_TUMBLE_COMPLETED;
						}
//						else if(((uiStep_Minutes_Completed == 11)||(uiStep_Minutes_Completed == 22)||
//                                        	(uiStep_Minutes_Completed == 33)||(uiStep_Minutes_Completed == 44)||(uiStep_Minutes_Completed == 55)||(uiStep_Minutes_Completed == 66))
//                                        	&&(uiStep_Seconds == 0)&&(iMilliSeconds == 0)&&((uiTemperature <=(uiStartTemperature + 2))||(uiTemperature == 232)))

						else if(((uiheat_Minutes_Completed == 11)||(uiheat_Minutes_Completed == 22)||
                                        	(uiheat_Minutes_Completed == 33)||(uiheat_Minutes_Completed == 44)||(uiheat_Minutes_Completed == 55)||(uiheat_Minutes_Completed == 66))
                                        	&&(uiheat_Seconds_Completed == 0)&&(iMilliSeconds == 0)&&((uiTemperature <=(uiStartTemperature + 2))||(uiTemperature == 232)))
						{
//							bHeater = SWITCH_OFF;
//							bHeaterOpen = TRUE;
//							if(bNtcFailed == TRUE)
//							{
//								Store_errors(E3);
//							}
//							else
//							{
//								Store_errors(E1);
//							}
							//added to avoid htr error  
							if(uiTemperature < 70)
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
						}
						else if(iWaterLevel < iMinimum_WaterLevel)
						{
							bPrewashValve = SWITCH_OFF;
							bStop_Minutes_Count = NO;
							
							bMotor_Start = YES;
							motor(uiDuty_Selected);
							if((uiTemperature < uiTemperature_Required)&&(uiStep_Seconds == 4)&&(uiStep_Minutes_Completed == RESET_VALUE))
							{
								uiTumbleState = STATE_TUMBLEHEAT;
							}
							else if((uiTemperature <= (uiTemperature_Required - uiCutin_Temperature)))
							{
								uiTumbleState = STATE_TUMBLEHEAT;
							}
							else if(uiTemperature >= uiTemperature_Required)
							{
								bHeater = SWITCH_OFF;
								bHeaterOpen = FALSE;
								bReheatHeater = FALSE;	
							}
						}
						else
						{
							bMotor_Start = NO;
							bStop_Minutes_Count = YES;
							uiTumbleState = STATE_TUMBLE_FILL;
							//MotorValues_Reset();
							bHeater = SWITCH_OFF;
						}
						
						result = STEP_INPROGRESS;
						break;
					
		case STATE_TUMBLE_FILL :
						if(reset_filling(ValveSelected,iSet_WaterLevel))
						{
							uiTumbleState = STATE_TUMBLE;
						}
						bCirculation_Pump = SWITCH_OFF;
						result = STEP_INPROGRESS;
						break;
						
		case STATE_TUMBLE_COMPLETED:
						result = STEP_FINISHED;
						bMotor_Start = NO;
						MotorStop();
						bCirculation_Pump = SWITCH_OFF;
						uiTumbleState = RESET_VALUE;
						uiStartTemperature = RESET_VALUE;
						if(uiPresent_Step >=  10)
						{
							bHeater = SWITCH_OFF;
							bReheatHeater = FALSE;
						}
						break;
		case STATE_TUMBLEHEAT:
//						if(((uiStep_Minutes_Completed == RESET_VALUE)||(uiStep_Minutes_Completed == 11)||(uiStep_Minutes_Completed == 22)
//                                    		||(uiStep_Minutes_Completed == 33)||(uiStep_Minutes_Completed == 44)||(uiStep_Minutes_Completed == 55))
//                                    		&&(uiStep_Seconds == 2)&&(uiTemperature <=(uiTemperature_Required - 3)))

						if(((uiheat_Minutes_Completed == RESET_VALUE)||(uiheat_Minutes_Completed == 11)||(uiheat_Minutes_Completed == 22)
                                    		||(uiheat_Minutes_Completed == 33)||(uiheat_Minutes_Completed == 44)||(uiheat_Minutes_Completed == 55))
                                    		&&(uiheat_Seconds_Completed == 2)&&(uiTemperature <=(uiTemperature_Required - 3)))
						{
							uiStartTemperature = (uint8_t)uiTemperature;
						}
						if(uiTemperature >= uiTemperature_Required)
						{
							bHeater = SWITCH_OFF;
							bHeaterOpen = FALSE;
							uiTumbleState = STATE_TUMBLE;
						}
						else if(uiStepTime <= uiStep_Minutes_Completed)
						{
							uiTumbleState = STATE_TUMBLE_COMPLETED;	
							bHeater = SWITCH_OFF;
							//MotorValues_Reset();
						}
//						else if(((uiStep_Minutes_Completed == 11)||(uiStep_Minutes_Completed == 22)||
//                                        	(uiStep_Minutes_Completed == 33)||(uiStep_Minutes_Completed == 44)||(uiStep_Minutes_Completed == 55)||(uiStep_Minutes_Completed == 66))
//                                        	&&(uiStep_Seconds == 0)&&(iMilliSeconds == 0)&&((uiTemperature <=(uiStartTemperature + 2))||(uiTemperature == 232)))

						else if(((uiheat_Minutes_Completed == 11)||(uiheat_Minutes_Completed == 22)||
                                        	(uiheat_Minutes_Completed == 33)||(uiheat_Minutes_Completed == 44)||(uiheat_Minutes_Completed == 55)||(uiheat_Minutes_Completed == 66))
                                        	&&(uiheat_Seconds_Completed == 0)&&(iMilliSeconds == 0)&&((uiTemperature <=(uiStartTemperature + 2))||(uiTemperature == 232)))
						{
//							bHeater = SWITCH_OFF;
//							bHeaterOpen = TRUE;
//							if(bNtcFailed == TRUE)
//							{
//								Store_errors(E3);
//							}
//							else
//							{
//								Store_errors(E1);
//							}
							//added to avoid htr error  
							if(uiTemperature < 70)
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
						}
						else if(iWaterLevel < iMinimum_WaterLevel)
						{	
							if((uiReheatTime < 40)&&(bNtcFailed == FALSE)&&(bHeaterOpen == FALSE))
							{
								bHeater = SWITCH_ON;
								bCirculation_Pump = SWITCH_OFF;
								bReheatHeater = TRUE;
							}
							else
							{
								bHeater = SWITCH_OFF;
								bReheatHeater = FALSE;
							}
							
							bMotor_Start = YES;
							motor(uiDuty_Selected);
							//bMotor_Start = YES;
							bHalfMinute_Process_Start = NO;
							bStop_Minutes_Count = NO;
							//motor(uiDuty_Selected);
						}
						else
						{
							bMotor_Start = NO;
							bStop_Minutes_Count = YES;
							uiTumbleState = STATE_TUMBLE_FILL;
							//MotorValues_Reset();
							bHeater = SWITCH_OFF;
						}
						break;

		default :
			// no operation required
			//if(uiTumbleState == 0xFF)
			//{
					uiTumbleState = STATE_TUMBLE;
					result = STEP_INPROGRESS;
			//}
			break;
	}
	return result;
	
}
