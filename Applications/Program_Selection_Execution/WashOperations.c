#include "WashOperation.h"
#include "programselection.h"
#include "waterdrain.h"
#include "TumbleOperation.h"
#include "waterheat.h"
#include "r_cg_userdefine.h"
#include "Selection.h"
#include "waterfill.h"
#include "WashOperations_Global.h"


//*************************************************************************************************************//
//******************************* global variables ***********************************************************//

extern boolean bDrain_Reset_Seconds,bProgramStart,bprewash_selected,bRinsehold_Finished,bFoamDetection;
extern boolean bHeater_Delay_Start,bTimeSaver_Selected,bStep_Finished,bWaterLevel_Read_Required,bSteam_Start;//bExpress_30_Activatted;
extern boolean bStop_Minutes_Count,bMotor_Start,bWater_Fill_Completed,bWaterlevel_Desired,bEco60Reset,bCold_Soak_Selected;
extern boolean bWater_ResetLevel,bStart_Traic_Fire,TubClean_Activated,bSpin_Start,bTumbleSpecial,bUnbalanceError;
extern boolean bPrewash_Start,bSoak_Start,bHotRinse_Start,bExtraRinse_Start,bDelay_Start1,bHalfMinute_Process_Start;
extern boolean bTESTMODE_START,bSpinTumble,bProgram_Start,bTimeSaver_Start,bSpinStarted,bDrainPump,bHotRinse_Selected;
extern boolean bprewash_selected,bSpin_Available,bDelay_Start,bSpin_Status,bSpecialMotion_Finished,bCirculation_Pump,bNtcFailed, bMainWashValve;

extern uint8_t uiSelected_Position,uiPresent_Step,uiNextStep_Start_Delay,rflag,uiUnbalance_Level;
extern uint8_t uiNoWaterTime,uiError_Mode,uiLowPressureTime,uiStep_Seconds;
extern uint8_t uiRpmSel,uiSilkTumble,uiStep_Minutes_Completed,uiFoamState,uiheat_Minutes_Completed,uiheat_Seconds_Completed;
//extern uint8_t uiSelectedTone,uiPSER_Option_Avbl,uiUnbalance_Value;
extern uint8_t uiPSER_Option_Avbl,uiUnbalance_Value;
extern volatile uint8_t uiSelectedTone;
extern uint8_t uiTemperature_Required,uiWaterResetCount,uiHalfMinutes_Seconds,uiPrewashWaterResetCount;
extern uint8_t uidisp_select_option,uiHeater_TurnOn_Delay_Value;
extern uint8_t uiExtraRinse_Press_count,uiRpmSel;
extern uint8_t uiMachineState,uiProgram_Position,uiTime_Minutes_Completed;
extern uint8_t uiTempSel,uiReheatTime,uiTestModeState,SFState,uidefault_kg;
extern uint8_t uiRpmSel,uilast_selected_option,uiSoak_Press_count;
extern uint8_t uiTime_Seconds,uiHalfMinute_Completed,ucCount,uiTest_drain;
extern uint8_t ckey_digit_data[6],cdigits_data[7],uiFinalWt;

extern uint16_t iWater_Washlevel,iRinselevel,iTumble_Waterlevel,uiLS_RPMValue,uiN_RPMValue; 
extern uint16_t ucMotor_Rotate_Seconds,r_sec,uicycle_mincompleted;
extern uint16_t uidelaymin,BalanceTime,iRinseChnageWaterLevel;
extern uint16_t uicycle_mincompleted,uiRPMValue;

extern uint8_t uiTemperature;
extern uint8_t ANTICREASE_TIME;
extern uint8_t Spin_Final_Profile_Index,uiSW_VF_MD;

//***************************************************************************************************//
//***************************** global function prototype ******************************************//

extern void MemoryMirror();
extern void motor(uint8_t);
extern void FoamCheck();
extern boolean FoamSensing();
extern void MotorStop(void);
extern boolean WashFilling(uint8_t,uint8_t,uint8_t,uint16_t,uint16_t );
extern boolean RinseFilling(uint8_t,uint8_t,uint8_t,uint16_t,uint16_t);
extern boolean SteamFill(uint8_t ,uint16_t );
extern void Delay_ms(uint16_t);
extern boolean bFoamRm_Step;
//***************************************************************************************************//
//***************************** Local function prototype ******************************************//

static void StepFinished(void);
void Time_Update_Basedon_Set_Reset_Count(void);
void Load_Sensing_Calculation(void);
void After_LS_Update_Para(void);
//***************************************************************************************************//
//*********************************** Local Variables ******************************************//
boolean heat_done;
boolean step_start,bStep_Flag,bSpinDelay,bDelay_Finished;
uint8_t uiProgramLast_step,uiTumblingState,uiheat_temp;
uint8_t uiTumblingCount,uiOperationSelected,StepTime;
uint16_t uiCurrent_step_time;
uint8_t RetTime,ui8loadvalue;
uint8_t uiLS_Tx_subtraction;
uint8_t uiMix_Heat_subtraction;
uint8_t uiLS_Total_subtraction;

//#if ENABLE_IOT
#include "iot_config.h"
extern uint8_t uiSmartProgram_Selected;
#endif

uint8_t FS_Time = 0;
extern uint8_t debug_count;

extern boolean Foam_Removal_Enable;
extern uint8_t Foam_Removal_Complete;
extern uint8_t uiDrainOffSeconds;
extern uint8_t uiAISelected_Position,uiLS_Seconds;

extern boolean bUnbalanceError, bMotorError_Unbalnce, bUnbalance_Detected;
uint8_t presentSpin=0,uiLoadRange;			

float Y_weight, Y_weight_neg;
uint16_t Neg_debug_weight, debug_weight, RPMtest;
boolean LS_flag,bLoadSensing_Completed;
/***********************************************************************************************************************
/*------------------------CP variable, function declaration-----------------------*/
st_CPObject CPExec_objects;
boolean CpOffTimer_b,CpOnTimer_b;
uint8_t CP_State;
extern uint8_t CP20MsTo1MsCounter_ui8;
extern uint32_t CPTimeInSecCounter_ui32;
extern boolean bPrewashValve,bMainWashValve;
uint8_t WashOperationLast30sDrain_state_ui8;
/*--------------------------------------------------------------------------------*/	

/***********************************************************************************************************************
* Function Name: washing
* Description  : This function implements different machine step execution.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void washing(Operation *OperationSelected)
{
	static uint8_t uiDuration,uiValve_Select,uiCutIn_Temp;
	static uint16_t iResetLevel,iRinseAdditive;
	uiTumblingCount = OperationSelected->eWork;
	uiOperationSelected = OperationSelected->eRhythm;
	//StepTime =  OperationSelected->eDuration;

	//below condition is added to overcome issue of prewash temp
	//since prewash temp is not added in the tx array so generalised the temp comditions below 
	//only for tx and heat time logic 
	if(uiTempSel <= P_95)
	{
		if((uiTempSel == P_95) || (uiTempSel == Wash95))
		{
		uiheat_temp = Wash95;
		}
								
		else if((uiTempSel == P_60) || (uiTempSel == Wash60))
		{
		uiheat_temp = Wash60;
		}
								
		else if((uiTempSel == P_40) || (uiTempSel == Wash40))
		{
		uiheat_temp = Wash40;
		}
								
		else if((uiTempSel == P_30) || (uiTempSel == Wash30))
		{
		uiheat_temp = Wash30;
		}
		
		else if((uiTempSel == P_COLD) || (uiTempSel == WashCold))
		{
		uiheat_temp = WashCold;
		}
	}
	
	if(uiTumblingCount == SPIN){
		presentSpin=1;
	}
	else		presentSpin=0;
	
	switch(uiTumblingCount )
	{
		//-------------------------------------------------------------------
		
		//---------------------------------------------------------------------------------------------
		
		case OPTIONAL_PREWASH:
		case PREWASH_TUMBLING:
		case PRE_HEAT:
		case PREWASH_SPIN:
		case DRAIN_PREWASH:
			if(bprewash_selected == TRUE)
			{
				MachineState_LED(TRUE,FALSE,FALSE,FALSE);
#if ENABLE_IOT
				if(uiMachineState != PAUSE_PRESSED){
				set_IOTState(STATE_IOT_PREWASH);
				}
#endif		
			}
			else
			{
				MachineState_LED(FALSE,FALSE,FALSE,FALSE);
			}
			
		break;
		//---------------------------------------------------------------------------------------------
		case FILLING:
		case HEAT:
		case HEATING:
		case SOAK:
		case TUMBLING_TX:
		case TUMBLING:
		case TUMBLING_TIMESAVE:
		case TUMBLINGSPECIAL:
		case REHEAT:
		case COOL_DOWN:
		case INITIAL_DRAIN:
		case DRAIN:
		case MAINWASH_HALFDRAIN:
		case MAINWASH_INTERMIDIATE_SPIN:
		case MAIN_TUMBLE:
			if((uiPresent_Step != 1)&&(uiPresent_Step != 0))
			{
				if((uiSelected_Position == ADDITIVESRINSE) &&(uiPresent_Step == 7))
				{
					if((uidisp_select_option & ExtraRinse_Yes) !=  ExtraRinse_Yes)
					{
						MachineState_LED(FALSE,FALSE,FALSE,TRUE);
					}
					else
					{
						MachineState_LED(FALSE,FALSE,TRUE,FALSE);
					}
						
				}
				else
				{
					MachineState_LED(FALSE,TRUE,FALSE,FALSE);
				}
			}
			else
			{	
				MachineState_LED(FALSE,FALSE,FALSE,FALSE);
			}
			
		break;
		
		case OPTION_EXTRA_RINSE_MAINWASH:
		case OPTION_EXTRA_RINSE_PREWASH:
		case OPTION_EXTRA_ADDITIVE_RINSE:
		case OPTIONAL_DRAIN:
		case OPTIONAL_SPIN:
		
			if(uidisp_select_option & ExtraRinse_Yes) 
				MachineState_LED(FALSE,FALSE,TRUE,FALSE);
				
		break;
		
		case RINSE_PREWASH_FILL:
		case RINSE_MAINWASH_FILL:
		case RINSE_DRAIN:
		case RINSE_HALFDRAIN:
		case RINSE_INTERMIDIATE_SPIN:
		case HOTRINSE:
		case RINSE3_ADDITIVES:
		//case RINSE_HOLD:
		case DRAIN_TUMBLE:
		case RINSE_TUMBLING:
			MachineState_LED(FALSE,FALSE,TRUE,FALSE);
		break;
		
		case RINSE_HOLD:
			if((uiSelected_Position == ADDITIVESRINSE) && ((uidisp_select_option & ExtraRinse_Yes) !=  ExtraRinse_Yes))
			{
				MachineState_LED(FALSE,FALSE,FALSE,FALSE);	
			}
			else
			{
				MachineState_LED(FALSE,FALSE,TRUE,FALSE);
			}
		break;
		
		//---------------------------------------------------------------------------------------------
		case ANTI_CREASE:
		case SPIN:
		case SPIN_TUMBLE:
			MachineState_LED(FALSE,FALSE,FALSE ,TRUE);
			if((uiTumblingCount == SPIN)|| (uiTumblingCount == ANTI_CREASE))
				bSpin_Available = FALSE;
				
			
		break;

		case STEAM_FILL:
		case STEAM:
		case STEAM_CREASE:
		case STEAM_DRAIN:
			MachineState_LED(FALSE,FALSE,FALSE,FALSE);
#if ENABLE_IOT	
			if((uidisp_select_option & Steam_Yes) ==  Steam_Yes)
			{
				if(uiMachineState != PAUSE_PRESSED)
					set_IOTState(STATE_IOT_STEAM);
			}
#endif			
		break;
		
		//---------------------------------------------------------------------------------------------
		default:			
		break;
	
	}
	
	if((uiNextStep_Start_Delay == COMPLETED) && ((uiMachineState == START_PRESSED) || (uiMachineState == TESTMODE)))
	{
//		if((GET_PROGRAM) == COTTON)
//		{
//			iResetLevel = ECO_RESET_LEVEL;		
//		}
//		else
//		{
//			iResetLevel = RESET_LEVEL;
//		}
		//mix, cotton and synthetic have different reset levels which were followed only during main filling but it should have followed even during tumbling reset filling hence the below change
		if((GET_PROGRAM) == COTTON)
		{
			iResetLevel = ECO_RESET_LEVEL;		
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
		
		switch(OperationSelected->eWork)
  		{
				case DELAYSTART:
						if(uidelaymin > RESET_VALUE)
						{
							bStep_Finished = NO;
							bDelay_Start1 =  TRUE;
							//bProgram_Start = TRUE;
#if ENABLE_IOT
							set_IOTState(STATE_IOT_DELAY_START);
#endif
						}
						else
						{
							bStep_Finished = YES;
							bDelay_Start = FALSE;
							bDelay_Start1 =  TRUE;
							bDelay_Finished = TRUE;
							bProgram_Start = TRUE;
							ckey_digit_data[0] = ckey_digit_data[0] & (~DelayStart_Yes); // turn off navigation of HotRinse
							ckey_digit_data[1] = ckey_digit_data[1] & (~DelayStart_Yes); // turn off HotRinse option led
							ckey_digit_data[2] = ckey_digit_data[2] & (~DelayStart_Yes); // turn off HotRinse option led
							uidisp_select_option = uidisp_select_option & (~DelayStart_Yes);
							//cdigits_data[5] = cdigits_data[5] &  (~DelayStart_Yes); // turn off the Delay selection led	
						}
						break;
			//******************PREWASH OPERATION**********************//
			case OPTIONAL_PREWASH:	
						bPrewash_Start = TRUE;
						uiTumblingState = 1;
						if(bprewash_selected ==  TRUE)
						{
							uiDuration = OperationSelected->eDuration;
							if(step_start == FALSE)
							{
								uiCurrent_step_time = uiDuration;
								if(uiCurrent_step_time > RESET_VALUE)
								step_start = TRUE;
							}
							StepTime =  uiDuration;
							
							if(WashFilling(OperationSelected->eWork,uiDuration,uiOperationSelected,iWater_Washlevel,iTumble_Waterlevel))
							{
								bStep_Finished = YES;
							}
						}
						else
						{
							bPrewash_Start = TRUE;
							bStep_Finished = YES;
						}
						break;
			case PREWASH_TUMBLING: 
						if(bprewash_selected  == TRUE)
						{
							uiTumblingState = 1;
							uiDuration = OperationSelected->eDuration;
													
							if(step_start == FALSE)
							{
								uiCurrent_step_time = uiDuration;
								if(uiCurrent_step_time > RESET_VALUE)
								step_start = TRUE;
							}
							StepTime =  uiDuration;
							
							//if(WashOperation_Last30secDrain())
							//{
							//	bDrainPump = SWITCH_ON;
							//}
							//else
							//{
								if(Tumbling(uiDuration,uiOperationSelected,iResetLevel,iWater_Washlevel,iTumble_Waterlevel,PRE_VALVE))  
								{
									bStep_Finished = YES;
									Time_Update_Basedon_Set_Reset_Count();
								}
							//}
						}
						else
						{
							bStep_Finished = YES;
						}
						break;
			case PRE_HEAT :
						if(bprewash_selected  == TRUE)
						{
							uiTumblingState = 1;
							if(step_start == FALSE)
							{
								uiCurrent_step_time = OperationSelected->eDuration;
								if(uiCurrent_step_time > RESET_VALUE)
								step_start = TRUE;
							}
							StepTime =  OperationSelected->eDuration;
							if(heating(OperationSelected->eDuration,uiOperationSelected,30,iWater_Washlevel,iTumble_Waterlevel,RESET_LEVEL))
							{
								bStep_Finished = YES;
							}
						}
						else
						{
							bStep_Finished = YES;
						}
						break;
									
			case PREWASH_SPIN:
						if(bprewash_selected  == TRUE)
						{	
							if(Conditional_drain(EMPTYLEVEL))
							{
								if(step_start == FALSE)
								{
									MachineErrors.EBits.HighUnbalance = 0;
									bMotorError_Unbalnce = FALSE;
									bUnbalanceError = FALSE;	//reset unbalance before starting spin
									
									uiCurrent_step_time = OperationSelected->eDuration;
									if(uiCurrent_step_time > RESET_VALUE)
									step_start = TRUE;
								}
								StepTime =  OperationSelected->eDuration;
								if(bFoamDetection == FALSE)
								{
									uiTumblingState = 3;
									bMotor_Start = TRUE;
									bSpinStarted = TRUE;
									bDrainPump = TRUE;
									motor(uiOperationSelected);
								}
								else if((uiFoamState != FOAM_SENSING_TUMBLE)&&(bFoamDetection == TRUE))
								{
									bMotor_Start = FALSE;
									bSpinStarted = TRUE;
									//motor(uiOperationSelected);
								}
								FoamCheck();
								if((bSpin_Status == TRUE)||((step_start == TRUE)&&(uiCurrent_step_time == RESET_VALUE)))
								{
									bStep_Finished = YES;
									bDrainPump = SWITCH_OFF;
									uiTest_drain = RESET_VALUE;
									bSpinDelay = YES;
								}
							}
						}
						else
						{
							bStep_Finished = YES;
						}
						
						break;
			case DRAIN_PREWASH:
						if(bprewash_selected  == TRUE)
						{
							if(step_start == FALSE)
							{
								uiCurrent_step_time = OperationSelected->eDuration;
								if(uiCurrent_step_time > RESET_VALUE)
								step_start = TRUE;
							}
							StepTime =  OperationSelected->eDuration;
							if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == DARKWASH)&&(uiPresent_Step == 5))
							{
								if(draining(OperationSelected->eDuration,TUMBLING_REQUIRED,uiOperationSelected,EMPTYLEVEL))
								{
									bStep_Finished = YES;
									bDrain_Reset_Seconds = NO;
								}
							}
							else
							{	
								if(draining(OperationSelected->eDuration,TUMBLING_NOT_REQUIRED,NO_DUTY_CYCLE,EMPTYLEVEL))
								{
									bStep_Finished = YES;
									bDrain_Reset_Seconds = NO;
									if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected==UNIFORM))	//for uniform 2nd step of PW is drain and there is no PW tumbling step so tx update calculation was not taking place
									{
										Time_Update_Basedon_Set_Reset_Count();
									}
								}
							}
						}
						else
						{
							bStep_Finished = YES;
						}
						
						break;
			//*****************END OF PREWASH OPERATION******************//
			
			//*************************** MAINWASH OPERATION *********************************//
			case FILLING: 			
						uiTumblingState = 1;
						bEco60Reset = FALSE;
						uiDuration = OperationSelected->eDuration;
						if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == UNIFORM)&&(uiPresent_Step == 5))
						{
							if((uiTempSel == Wash60)||(uiTempSel == Wash40)||(uiTempSel == P_40)||(uiTempSel == P_60))
							{
								uiDuration = SIX_MINUTES;
								uiOperationSelected = DUTY_C21;
							}
							else
							{
								uiDuration = THIRTY_ONE_MINUTES;
								uiOperationSelected = DUTY_C22;
							}	
						}
						//if((uiSelected_Position == EXPRESS_30) && (uiTempSel == Wash60))
						if((uiSelected_Position == SMARTPLUS) && (uiSmartProgram_Selected == EXPRESS_30) && (uiTempSel == Wash60))
						{
							uiDuration = SIX_MINUTES;//FIVE_MINUTES;--vish
						}
						if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == LINENWASH)) 
						{
							if((uiTempSel == Wash30)||(uiTempSel == Wash40))
							{	
								uiDuration = SIX_MINUTES;
							}
							else
							{
								uiDuration = SEVEN_MINUTES;
							}
						}
						if(step_start == FALSE)
						{
							uiCurrent_step_time = uiDuration;
							if(uiCurrent_step_time > RESET_VALUE)
							step_start = TRUE;
						}
						StepTime =  uiDuration;
						if(bStep_Flag == FALSE)
						{
							step_start = FALSE;
							bStep_Flag = TRUE;
						}
						if(WashFilling(OperationSelected->eWork,uiDuration,uiOperationSelected,iWater_Washlevel,iTumble_Waterlevel)) 
						{
							
							bStep_Finished = YES;
							//After_LS_Update_Para();
							if(uiSelected_Position == SMARTPLUS)
							{
								Time_Update_Basedon_Set_Reset_Count();
							}
//							if((uidisp_select_option & Prewash_Yes) == Prewash_Yes)
//							{
//								if((uiPrewashWaterResetCount <= 1)&& ((GET_PROGRAM) == SYNTHETIC))
//								{	
//									BalanceTime = BalanceTime - SYNTH_RINSE2_TIME; /*Rinse 2 total time is 13min so deduct the 13 time from Main time */
//									uicycle_mincompleted = uicycle_mincompleted + SYNTH_RINSE2_TIME;
//								}
//								if(bTimeSaver_Selected == FALSE)
//								{	
//									switch(GET_PROGRAM)
//									{
//										case SYNTHETIC:/*MaxTX_Time - ResetCountTX_Time*/
//												BalanceTime = BalanceTime - (SYNTHETIC_TumbleTXResetTime_ui8[SYNTHETIC_MAX_RESET_LIMIT][uiTempSel] - 
//														SYNTHETIC_TumbleTXResetTime_ui8[uiPrewashWaterResetCount][uiTempSel]);
//												uicycle_mincompleted = uicycle_mincompleted + (SYNTHETIC_TumbleTXResetTime_ui8[SYNTHETIC_MAX_RESET_LIMIT][uiTempSel] - 
//														SYNTHETIC_TumbleTXResetTime_ui8[uiPrewashWaterResetCount][uiTempSel]);
//														
//														
//												break;
//										case MIXEDSOIL:/*MaxTX_Time - ResetCountTX_Time*/
//												BalanceTime = BalanceTime - (MIXED_TumbleTXResetTime_ui8[MIXED_MAX_RESET_LIMIT][uiTempSel] - 
//												     		MIXED_TumbleTXResetTime_ui8[uiPrewashWaterResetCount][uiTempSel]);
//												uicycle_mincompleted = uicycle_mincompleted + (MIXED_TumbleTXResetTime_ui8[MIXED_MAX_RESET_LIMIT][uiTempSel] - 
//												     		MIXED_TumbleTXResetTime_ui8[uiPrewashWaterResetCount][uiTempSel]);
//														
//												/*This is added because of heating time depends on water reset cout soe it should reduce the time after filling.*/		
//												BalanceTime = BalanceTime - (MIXED_HeatTempTime_ui8[MIXED_MAX_RESET_LIMIT][uiTempSel] - 
//												     		MIXED_HeatTempTime_ui8[uiPrewashWaterResetCount][uiTempSel]);
//												uicycle_mincompleted = uicycle_mincompleted + (MIXED_HeatTempTime_ui8[MIXED_MAX_RESET_LIMIT][uiTempSel] - 
//												     		MIXED_HeatTempTime_ui8[uiPrewashWaterResetCount][uiTempSel]);
//												break;
//										case COTTON:	/*MaxTX_Time - ResetCountTX_Time*/ //Exclude 40E & 60E Temp 
//												if(uiTempSel <= Wash95)
//												{
//													BalanceTime = BalanceTime - (COTTON_TumbleTXResetTime_ui8[COTTON_MAX_RESET_LIMIT][uiTempSel] - 
//													     		COTTON_TumbleTXResetTime_ui8[uiPrewashWaterResetCount][uiTempSel]);
//													uicycle_mincompleted = uicycle_mincompleted +(COTTON_TumbleTXResetTime_ui8[COTTON_MAX_RESET_LIMIT][uiTempSel] - 
//													     		COTTON_TumbleTXResetTime_ui8[uiPrewashWaterResetCount][uiTempSel]);
//												}
//												break; 
//										default	:	break;
//									}
//								}
//								if((uiPrewashWaterResetCount <= 1)&&(bTimeSaver_Selected == FALSE)&&(uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == UNIFORM))
//								{
//									BalanceTime = BalanceTime - 15; 
//									uicycle_mincompleted = uicycle_mincompleted + 15;
//								}
//								else if((uiSelected_Position == COTTON)&&(bTimeSaver_Selected == FALSE)) 
//								{
//									if((uiPrewashWaterResetCount <= 1)&&(uiTempSel >= Wash40E))
//									{
//										if(uiTempSel == Wash40E)
//										{
//											BalanceTime = BalanceTime - 40; 
//											uicycle_mincompleted = uicycle_mincompleted + 40;
//										}
//										else if(uiTempSel == Wash60E)
//										{
//											BalanceTime = BalanceTime - 27; 
//											uicycle_mincompleted = uicycle_mincompleted + 27;
//										}
//									}
//								}
//								else if((bTimeSaver_Selected == FALSE)&&(uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == JEANS))
//								{
//									if(uiPrewashWaterResetCount <= 2)
//									{
//										BalanceTime = BalanceTime - 20; 
//										uicycle_mincompleted = uicycle_mincompleted + 20;
//									}
//									else if((uiPrewashWaterResetCount > 2)&&(uiPrewashWaterResetCount <= 5))
//									{
//										BalanceTime = BalanceTime - 10; 
//										uicycle_mincompleted = uicycle_mincompleted + 10;
//									}
//								}
//							}
//							else
//							{
//								if((uiWaterResetCount <= 1)&& ((GET_PROGRAM) == SYNTHETIC))
//								{	
//									BalanceTime = BalanceTime - SYNTH_RINSE2_TIME; /*Rinse 2 total time is 13min so deduct the 13 time from Main time */
//									uicycle_mincompleted = uicycle_mincompleted + SYNTH_RINSE2_TIME;
//								}
//								if(bTimeSaver_Selected == FALSE) 
//								{	
//									
//									switch(GET_PROGRAM)
//									{
//										case SYNTHETIC:	/*MaxTX_Time - ResetCountTX_Time*/
//												BalanceTime = BalanceTime - (SYNTHETIC_TumbleTXResetTime_ui8[SYNTHETIC_MAX_RESET_LIMIT][uiTempSel] - 
//												     SYNTHETIC_TumbleTXResetTime_ui8[uiWaterResetCount][uiTempSel]);\
//												     
//												uicycle_mincompleted = uicycle_mincompleted + (SYNTHETIC_TumbleTXResetTime_ui8[SYNTHETIC_MAX_RESET_LIMIT][uiTempSel] - 
//														SYNTHETIC_TumbleTXResetTime_ui8[uiWaterResetCount][uiTempSel]);
//														
//												break;
//										case MIXEDSOIL:/*MaxTX_Time - ResetCountTX_Time*/
//												BalanceTime = BalanceTime - (MIXED_TumbleTXResetTime_ui8[MIXED_MAX_RESET_LIMIT][uiTempSel] - 
//												     MIXED_TumbleTXResetTime_ui8[uiWaterResetCount][uiTempSel]);
//												     
//												uicycle_mincompleted = uicycle_mincompleted + (MIXED_TumbleTXResetTime_ui8[MIXED_MAX_RESET_LIMIT][uiTempSel] - 
//												     		MIXED_TumbleTXResetTime_ui8[uiWaterResetCount][uiTempSel]);
//														
//												BalanceTime = BalanceTime - (MIXED_HeatTempTime_ui8[MIXED_MAX_RESET_LIMIT][uiTempSel] - 
//												     MIXED_HeatTempTime_ui8[uiWaterResetCount][uiTempSel]);
//												     
//												uicycle_mincompleted = uicycle_mincompleted + (MIXED_HeatTempTime_ui8[MIXED_MAX_RESET_LIMIT][uiTempSel] - 
//												     		MIXED_HeatTempTime_ui8[uiWaterResetCount][uiTempSel]);
//												break;
//										case COTTON:	/*MaxTX_Time - ResetCountTX_Time*/
//												if(uiTempSel<= Wash95)
//												{
//													BalanceTime = BalanceTime - (COTTON_TumbleTXResetTime_ui8[COTTON_MAX_RESET_LIMIT][uiTempSel] - 
//													     COTTON_TumbleTXResetTime_ui8[uiWaterResetCount][uiTempSel]);
//													
//													uicycle_mincompleted = uicycle_mincompleted + (COTTON_TumbleTXResetTime_ui8[COTTON_MAX_RESET_LIMIT][uiTempSel] - 
//													     		COTTON_TumbleTXResetTime_ui8[uiWaterResetCount][uiTempSel]);
//												}
//												break;
//										default	:	break;
//											
//									}
//								}
//								if((uiWaterResetCount <= 1)&&(bTimeSaver_Selected == FALSE)&&(uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == UNIFORM))
//								{
//									BalanceTime = BalanceTime - 15; 
//									uicycle_mincompleted = uicycle_mincompleted + 15;
//								}
//								
//								else if((uiSelected_Position == COTTON)&&(bTimeSaver_Selected == FALSE)) 
//								{
//									if((uiWaterResetCount <= 1)&&(uiTempSel >= Wash40E))
//									{
//										if(uiTempSel == Wash40E)
//										{
//											BalanceTime = BalanceTime - 40; 
//											uicycle_mincompleted = uicycle_mincompleted + 40;
//										}
//										else if(uiTempSel == Wash60E)
//										{
//											BalanceTime = BalanceTime - 27; 
//											uicycle_mincompleted = uicycle_mincompleted + 27;
//										}
//									}
//								}
//								else if((bTimeSaver_Selected == FALSE)&&(uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == JEANS))
//								{
//									if(uiWaterResetCount <= 2)
//									{
//										BalanceTime = BalanceTime - 20; 
//										uicycle_mincompleted = uicycle_mincompleted + 20;
//									}
//									else if((uiWaterResetCount > 2)&&(uiWaterResetCount <= 5))
//									{
//										BalanceTime = BalanceTime - 10; 
//										uicycle_mincompleted = uicycle_mincompleted + 10;
//									}
//								}
//							}
								
						}
#if ENABLE_IOT
						set_IOTState(STATE_IOT_MAINWASH);
#endif						
						break;
			case HEATING:
					if((uiTempSel != WashCold) && (uiTempSel != P_COLD))
					{
						uiTumblingState = TUMBLE_MOTION;
						uiDuration = OperationSelected->eDuration;
						switch(GET_PROGRAM)
						{
							//case EXPRESS_30:
							case SMARTPLUS:
								switch(uiSmartProgram_Selected)
								{	
									case EXPRESS_30:	
										uiDuration = WashOperations_HeatTime(uiDuration);
										break;
										
									default:
										break;
								}
								break;
							case MIXEDSOIL:
								switch(uiTempSel)
									{
										case P_60:
										case Wash60: 
												if(uiLoadRange==0)
												{
													uiDuration = SIX_MINUTES;
												}
												else if(uiLoadRange==1)
												{
													uiDuration = (bTimeSaver_Selected == TRUE)?EIGHT_MINUTES : TEN_MINUTES;/*Time In minutes*/
												}
												else
												{
													uiDuration = TEN_MINUTES;
												}
											//uiDuration = (bTimeSaver_Selected == TRUE)?TEN_MINUTES : FIFTEEN_MINUTES;/*Time In minutes*/
											break;
										case P_40:
										case Wash40:
												uiDuration = FIVE_MINUTES;/*Time In minutes : Timesaver is not present for temp 40*/
												break;
												
										case P_30:
										case Wash30:
												uiDuration = FIVE_MINUTES;/*Time In minutes : Timesaver is not present for temp 30*/
												break;
										default: break;
									}
									break;
							case COTTON:
								switch(uiTempSel)
								{
									case P_95:
									case Wash95:
										//uiDuration = (bTimeSaver_Selected == TRUE)?FIFTEEN_MINUTES : TWENTY_MINUTES;/*Time In minutes*/
										//--IN MP Spec heat time vary acc. to load cat
										if(uiLoadRange==0)
										{
											uiDuration = (bTimeSaver_Selected == TRUE)?TWENTY_MINUTES : THIRTY_MINUTES;/*Time In minutes*/
										}
										else if(uiLoadRange==1)
										{
											uiDuration = (bTimeSaver_Selected == TRUE)?THIRTY_MINUTES : FOURTY_MINUTES ;/*Time In minutes*/
										}
										else 
										{
											uiDuration = (bTimeSaver_Selected == TRUE)?FIFTY_MINUTES : SIXTY_MINUTES;/*Time In minutes*/
										}
										break;
									case P_60:
									case Wash60: uiDuration = (bTimeSaver_Selected == TRUE)?TEN_MINUTES : FIFTEEN_MINUTES;/*Time In minutes*/
										break;
									case P_40:
									case Wash40:uiDuration = TEN_MINUTES;/*Time In minutes : Timesaver is not present for temp 40*/
										break;
									case P_30:
									case Wash30:uiDuration = FIVE_MINUTES;/*Time In minutes : Timesaver is not present for temp 30*/
										break;
									default: break;
								}
								break;
							case SYNTHETIC:
								switch(uiTempSel)
								{
									case P_60:
									case Wash60: uiDuration = (bTimeSaver_Selected == TRUE)?TEN_MINUTES : FIFTEEN_MINUTES;/*Time In minutes*/
										break;
									case P_40:
									case Wash40: uiDuration = TEN_MINUTES;/*Time In minutes : Timesaver is not present for temp 40*/
										break;
									case P_30:
									case Wash30: uiDuration = FIVE_MINUTES;/*Time In minutes : Timesaver is not present for temp 30*/
										break;
									default: break;
								}
								break;
							default:	break;
						}
						if(step_start == FALSE)
						{
							uiCurrent_step_time = uiDuration;
								if(uiCurrent_step_time > RESET_VALUE)
								step_start = TRUE;
						}
						StepTime =  uiDuration;
						if(heating(uiDuration,uiOperationSelected,uiTemperature_Required,iWater_Washlevel,iTumble_Waterlevel,iResetLevel)) 
						{
							uiHalfMinute_Completed = RESET_VALUE;
							bStep_Finished = YES;
							//These logic was added for Mixed 40C as per prev specs //As per rev34 all reset value has 5min
//							if(bTimeSaver_Selected == TRUE)
//							{
//								if( (GET_PROGRAM == MIXEDSOIL) && (uiTempSel == Wash40))
//								{
//									if( (uiPrewashWaterResetCount < 6) || (uiWaterResetCount < 6 ))
//									{
//										BalanceTime = BalanceTime - 5;
//										uicycle_mincompleted = uicycle_mincompleted + 5;
//									}
//								}
//							}
						}
					}
					else
					{	uiHalfMinute_Completed = RESET_VALUE;
						bStep_Finished = YES;
					}
					
#if ENABLE_IOT
						set_IOTState(STATE_IOT_HEATING);
#endif						
					break;
			case HEAT: 
					if((uiTempSel != WashCold) && (uiTempSel != P_COLD))
					{
						uiTumblingState = TUMBLE_MOTION;
						uiDuration = OperationSelected->eDuration;
						if((uiTempSel == Wash30) || (uiTempSel == P_30))
						{
							if(uiSelected_Position == EXPRESS)
							{
								uiDuration = THREE_MINUTES;
							}
							else
							{
								uiDuration = FIVE_MINUTES;
							}
						}
						else if((uiTempSel == Wash40)||(uiTempSel == Wash40E) || (uiTempSel == P_40))
						{
							if((uiSelected_Position == COTTON)&&(uiTempSel == Wash40E))
							{
								uiDuration = FIVE_MINUTES;
							}
							else if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == WOOLLENS))
							{
								uiDuration = SIXTEEN_MINUTES;
							}
							else if((uiSelected_Position == EXPRESS)||((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == LINENWASH)))
							{
								uiDuration = FIVE_MINUTES;
							}
							else
							{
								uiDuration = TWENTY_MINUTES;
							}
						}
						else if((uiTempSel == Wash60)||(uiTempSel == Wash60E) || (uiTempSel == P_60))
						{
							 if((uiSelected_Position == COTTON)&&(uiTempSel == Wash60E))
							{
								uiDuration = THREE_MINUTES;//SIX_MINUTES;//EIGHT_MINUTES;
							}
							else
							{
								uiDuration = FOURTY_MINUTES;
							}
						}
						else if((uiTempSel == Wash95) || (uiTempSel == P_95))
						{

							uiDuration = SIXTY_MINUTES;
						}
						if(step_start == FALSE)
						{
							uiCurrent_step_time = uiDuration;
								if(uiCurrent_step_time > RESET_VALUE)
								step_start = TRUE;
						}
						StepTime =  uiDuration;
						if(heating(uiDuration,uiOperationSelected,uiTemperature_Required,iWater_Washlevel,iTumble_Waterlevel,iResetLevel)) 
						{
							uiHalfMinute_Completed = RESET_VALUE;
							bStep_Finished = YES;
						}
					}
					else
					{	uiHalfMinute_Completed = RESET_VALUE;
						bStep_Finished = YES;
					}
					
#if ENABLE_IOT
						set_IOTState(STATE_IOT_HEATING);
#endif						
						break;
			case SOAK :		
						if(((uidisp_select_option & WarmSoak_Yes) == WarmSoak_Yes)||
						(bCold_Soak_Selected == TRUE)||
						(((uiSelected_Position == MIXEDSOIL)||(uiAISelected_Position == MIXEDSOIL))&&
						((uiPresent_Step == 10)||(uiPresent_Step == 12)||(uiPresent_Step == 14))))//||
						//(uiSelected_Position == WETCLEAN)) //Added present step dependency for Mixed TCS default soak & wetclean
						{
							bSoak_Start = TRUE;
							
							 if(uiTempSel == WashCold || uiTempSel == P_COLD)
							 {
								if(uidefault_kg == 80 || uidefault_kg == 50)
								{
									if(((uiSelected_Position == MIXEDSOIL)||(uiAISelected_Position == MIXEDSOIL))&&
									((uiPresent_Step == 10)||(uiPresent_Step == 12)||(uiPresent_Step == 14)))//Added present step dependency for Mixed TCS default soak
									{	
										uiDuration =  ONE_MINUTES;
									}
									else
									{
										uiDuration =  EIGHT_MINUTES;
									}
								}
								else if((uidefault_kg == 35)||(uidefault_kg == 30))
								{
									uiDuration =  SIX_MINUTES;
								}
//								else if((uidefault_kg == 20)&&(uiSelected_Position == WETCLEAN))
//								{
//									uiDuration =  ONE_MINUTES;
//								}
								else
								{
									uiDuration =  FIVE_MINUTES;
								}
							 }
							 else
							 {
								uiCutIn_Temp = 10;
								if(uidefault_kg == 80 || uidefault_kg == 50)
								{	if(((uiSelected_Position == MIXEDSOIL)||(uiAISelected_Position == MIXEDSOIL))&&((uiPresent_Step == 10)||(uiPresent_Step == 12)||(uiPresent_Step == 14)))//Added present step dependency for Mixed TCS default soak
									{	
										uiDuration =  ONE_MINUTES;
									}
									else
									{
										uiDuration =  EIGHTEEN_MINUTES;
									}
								}
								else if((uidefault_kg == 35)||(uidefault_kg == 30))
								{
									uiDuration =  SIXTEEN_MINUTES;
								}
//								else if((uidefault_kg == 20)&&(uiSelected_Position == WETCLEAN))
//								{
//									uiDuration =  ONE_MINUTES;
//								}
								else
								{
									uiDuration =  FIFTEEN_MINUTES;
								}
							 }
						
							uiTumblingState = 1;
							if(step_start == FALSE)
							{
								uiCurrent_step_time = uiDuration;
								if(uiCurrent_step_time > RESET_VALUE)
								step_start = TRUE;
							}
							StepTime =  uiDuration;
							if((uiTempSel == WashCold)||(uiTempSel == P_COLD))
							{
//								if(uiSelected_Position != WETCLEAN)
//								{
//									bCirculation_Pump = SWITCH_ON;
//								}
								//bCirculation_Pump = SWITCH_ON;
								if(Tumbling(uiDuration,uiOperationSelected,iResetLevel,iWater_Washlevel,iTumble_Waterlevel,MAIN_WASH))
								{
									bStep_Finished = YES;
								}
							}
							else
							{	
								if(TumblingSpecialHeat(uiDuration,uiOperationSelected,iResetLevel,iWater_Washlevel,uiCutIn_Temp,MAIN_WASH))
								{
									bStep_Finished = YES;
								}
							}
#if ENABLE_IOT
							//if(uiTempSel > WashCold)
							if((uiTempSel != WashCold) && (uiTempSel != P_COLD))
								set_IOTState(STATE_IOT_WARM_SOAK);
							else
								set_IOTState(STATE_IOT_SOAK);
#endif
						}
						else
						{
							bSoak_Start = TRUE;
							bStep_Finished = YES;
						}
						
						break;	
						
						
			case TUMBLING_TX:	
						uiTumblingState = TUMBLE_MOTION;
						uiDuration = OperationSelected->eDuration;
						switch(GET_PROGRAM)
						{
							case MIXEDSOIL:
									if(bTimeSaver_Selected == TRUE)
									{
										//uiDuration = (uint8_t)MIXED_TimeSaverTumbleTXTime_ui8[uiLoadRange][uiTempSel];
										uiDuration = (uint8_t)MIXED_TimeSaverTumbleTXTime_ui8[uiLoadRange][uiheat_temp];
									}
									else
									{	
//										uiDuration = (bprewash_selected == TRUE)? (uint8_t)MIXED_TumbleTXResetTime_ui8[uiLoadRange][uiTempSel]: 
//													  	  (uint8_t)MIXED_TumbleTXResetTime_ui8[uiLoadRange][uiTempSel];
										uiDuration = (bprewash_selected == TRUE)? (uint8_t)MIXED_TumbleTXResetTime_ui8[uiLoadRange][uiheat_temp]: 
													  	  (uint8_t)MIXED_TumbleTXResetTime_ui8[uiLoadRange][uiheat_temp];
									}
									break;
							case COTTON:
									if(bTimeSaver_Selected == TRUE)
									{
										//uiDuration = (uint8_t)COTTON_TimeSaverTumbleTXTime_ui8[uiTempSel];
										//uiDuration = (uint8_t)COTTON_TimeSaverTumbleTXTime_ui8[uiLoadRange][uiTempSel];
										uiDuration = (uint8_t)COTTON_TimeSaverTumbleTXTime_ui8[uiLoadRange][uiheat_temp];
										uiOperationSelected = DUTY_C22;		// All Temp Duty is same when Timesaver selected.
									}
									else
									{	
//										uiDuration = (bprewash_selected == TRUE)? (uint8_t)COTTON_TumbleTXResetTime_ui8[uiLoadRange][uiTempSel]: 
//													  	  (uint8_t)COTTON_TumbleTXResetTime_ui8[uiLoadRange][uiTempSel];
										uiDuration = (bprewash_selected == TRUE)? (uint8_t)COTTON_TumbleTXResetTime_ui8[uiLoadRange][uiheat_temp]: 
													  	  (uint8_t)COTTON_TumbleTXResetTime_ui8[uiLoadRange][uiheat_temp];
														  
										if(uiTempSel == Wash95 || uiTempSel == P_95)
										{	
											uiOperationSelected = DUTY_C24; 
										}
									}
									break;
							case SYNTHETIC:	
									if(bTimeSaver_Selected == TRUE)
									{
										//uiDuration = (uint8_t)SYNTHETIC_TimeSaverTumbleTXTime_ui8[uiLoadRange][uiTempSel];
										uiDuration = (uint8_t)SYNTHETIC_TimeSaverTumbleTXTime_ui8[uiLoadRange][uiheat_temp];
										uiOperationSelected = DUTY_C23;
									}
									else
									{	
//									uiDuration = (bprewash_selected == TRUE)? (uint8_t)SYNTHETIC_TumbleTXResetTime_ui8[SYNTHETIC_RESET_LIMIT(uiLoadRange)][uiTempSel]: 
//													  	  (uint8_t)SYNTHETIC_TumbleTXResetTime_ui8[SYNTHETIC_RESET_LIMIT(uiLoadRange)][uiTempSel];

//									uiDuration = (bprewash_selected == TRUE)? (uint8_t)SYNTHETIC_TumbleTXResetTime_ui8[uiLoadRange][uiTempSel]: 
//													  	  (uint8_t)SYNTHETIC_TumbleTXResetTime_ui8[uiLoadRange][uiTempSel];
									uiDuration = (bprewash_selected == TRUE)? (uint8_t)SYNTHETIC_TumbleTXResetTime_ui8[uiLoadRange][uiheat_temp]: 
													  	  (uint8_t)SYNTHETIC_TumbleTXResetTime_ui8[uiLoadRange][uiheat_temp];
									
									}
									break;
							default:	break;
								
						}
						if(step_start == FALSE)
						{
							uiCurrent_step_time = uiDuration;
								if(uiCurrent_step_time > RESET_VALUE)
								step_start = TRUE;
						}
						StepTime =  uiDuration;
						//if(WashOperation_Last30secDrain())
						//{
						//	bDrainPump = SWITCH_ON;
						//}
						//else
						//{
							if(Tumbling(uiDuration,uiOperationSelected,iResetLevel,iWater_Washlevel,iTumble_Waterlevel,MAIN_WASH))  
							{
								bStep_Finished = YES;
							}
						//}
#if ENABLE_IOT
						if(uiTumblingCount == TUMBLING_TX)
						{
							set_IOTState(STATE_IOT_MAINWASH);
						}
#endif						
						break;						
			case TUMBLING: 
			case RINSE_TUMBLING:    
						uiTumblingState = 1;
						uiDuration = OperationSelected->eDuration;
						
						switch(GET_PROGRAM)
						{
							case MIXEDSOIL:
							case COTTON:
							case SYNTHETIC:
							//case EXPRESS_30:
									uiDuration = WashOperation_TimeSaverTime(uiDuration);
									break;
									
							case SMARTPLUS:
								switch(uiSmartProgram_Selected)
								{	
									case EXPRESS_30:	
										uiDuration = WashOperation_TimeSaverTime(uiDuration);
										break;
										
									default:
										break;
								}
								break;
							default: break;
									
						}
						
						if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == INNERWEAR)&&(uiPresent_Step == 4))
						{
							if(bTimeSaver_Selected == TRUE)
							{
								uiDuration = FOURTEEN_MINUTES;
							}
							else
							{
								uiDuration = EIGHTEEN_MINUTES;
							}
							
						}
						else if((uiSelected_Position == CURTAINS)&&(uiPresent_Step == 7))
						{
							if(bTimeSaver_Selected == TRUE)
							{
								
								uiDuration = FIVE_MINUTES;
							}
							else
							{
								if((uiTempSel == Wash60)||(uiTempSel == Wash40)||(uiTempSel == P_40)||(uiTempSel == P_60))
								{
//									if((uidisp_select_option & WarmSoak_Yes) == WarmSoak_Yes)
//									{
//										uiDuration = THREE_MINUTES;
//									}
//									else
//									{
										uiDuration = EIGHTEEN_MINUTES;
									//}
								}
								else if((uiTempSel == WashCold) || (uiTempSel == P_COLD))
								{
//									if(bCold_Soak_Selected == TRUE)
//									{
//										uiDuration = TWENTY_THREE_MINUTES;
//									}
//									else
//									{
										uiDuration = TWENTY_EIGHT_MINUTES;
									//}
								}
							}
						}
						else if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == LINENWASH))
						{
							if(uiTempSel == WashCold)
							{	
								uiDuration = TWO_MINUTES;
							}
							else if((uiTempSel == Wash30)||(uiTempSel == Wash40))
							{
								uiDuration = ONE_MINUTES;
							}
							
							
						}
						else if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == SHIRTS)&&(uiPresent_Step == 8))
						{
							if(bTimeSaver_Selected == TRUE)
							{
								uiDuration = TWELVE_MINUTES;
							}
							else
							{
//								if((bCold_Soak_Selected == TRUE)&&(uiTempSel == WashCold))
//								{
//									uiDuration = TWENTY_MINUTES;
//								}
//								else if(((uidisp_select_option & WarmSoak_Yes) == WarmSoak_Yes)&&(uiTempSel > WashCold))
//								{
//									uiDuration = TEN_MINUTES;
//								}
//								else
//								{
									uiDuration = TWENTY_FIVE_MINUTES;
								//}
							}
						}
						else if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == DARKWASH)&&(uiPresent_Step == 8))
						{
							if(bTimeSaver_Selected == TRUE)
							{
								uiDuration = FIFTEEN_MINUTES;
							}
							else
							{
								uiDuration = TWENTY_TWO_MINUTES;
							}
						}
						else if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == UNIFORM)&&(uiPresent_Step == 10))
						{
							if((uidisp_select_option & Prewash_Yes) == Prewash_Yes)
							{
								if(uiPrewashWaterResetCount <= 1)
								{
//									if(bCold_Soak_Selected == TRUE)
//									{
//										uiDuration = NINE_MINUTES;
//									}
//									else
									
									uiDuration = FIFTEEN_MINUTES;
									
									
								}
								else 
								{
//									if(bCold_Soak_Selected == TRUE)
//									{
//										uiDuration = TWENTY_FOUR_MINUTES;
//									}
//									else
									//{
									uiDuration = THIRTY_MINUTES;
									//}
								}
							}
							else
							{
								if(uiWaterResetCount <= 1)
								{
//									if(bCold_Soak_Selected == TRUE)
//									{
//										uiDuration = NINE_MINUTES;
//									}
//									else
//									{
									uiDuration = FIFTEEN_MINUTES;
									//}
								}
								else 
								{
//									if(bCold_Soak_Selected == TRUE)
//									{
//										uiDuration = TWENTY_FOUR_MINUTES;
//									}
//									else
									//{
									uiDuration = THIRTY_MINUTES;
									//}
								}
							}
						}
						else if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == JEANS)&&(uiPresent_Step == 24))
						{
							if((uidisp_select_option & Prewash_Yes) == Prewash_Yes)
							{
								if(uiPrewashWaterResetCount <= 2)
								{
//									if((uidisp_select_option & WarmSoak_Yes) == WarmSoak_Yes)
//									{
//										uiDuration = TWENTY_NINE_MINUTES;
//									}
//									else
//									{
										uiDuration = FOURTY_FIVE_MINUTES;
									//}
								}
								else if((uiPrewashWaterResetCount > 2)&&(uiPrewashWaterResetCount <= 5))
								{
//									if((uidisp_select_option & WarmSoak_Yes) == WarmSoak_Yes)
//									{
//										uiDuration = THIRTY_NINE_MINUTES;
//									}
//									else
//									{
										uiDuration = FIFTY_FIVE_MINUTES;
									//}
								}
								else 
								{
//									if((uidisp_select_option & WarmSoak_Yes) == WarmSoak_Yes)
//									{
//										uiDuration = FOURTY_NINE_MINUTES;
//									}
//									else
//									{
										uiDuration = SIXTY_FIVE_MINUTES;
									//}
								}
							}
							else
							{
								if(uiWaterResetCount <= 2)
								{
//									if((uidisp_select_option & WarmSoak_Yes) == WarmSoak_Yes)
//									{
//										uiDuration = TWENTY_NINE_MINUTES;
//									}
//									else
//									{
										uiDuration = FOURTY_FIVE_MINUTES;
									//}
								}
								else if((uiWaterResetCount > 2)&&(uiWaterResetCount <= 5))
								{
//									if((uidisp_select_option & WarmSoak_Yes) == WarmSoak_Yes)
//									{
//										uiDuration = THIRTY_NINE_MINUTES;
//									}
//									else
//									{
										uiDuration = FIFTY_FIVE_MINUTES;
									//}
								}
								else 
								{
//									if((uidisp_select_option & WarmSoak_Yes) == WarmSoak_Yes)
//									{
//										uiDuration = FOURTY_NINE_MINUTES;
//									}
//									else
//									{
										uiDuration = SIXTY_FIVE_MINUTES;
									//}
								}
							}
						}
						else if((uiSelected_Position == EXPRESS)&&(uiPresent_Step == 4)/* && (bExpress_30_Activatted == FALSE)*/)
						{
							if((uiTempSel == WashCold)||(uiTempSel == Wash40))
							{
								uiDuration = FOUR_MINUTES;
							}
							else if(uiTempSel == Wash30)
							{
								uiDuration = ONE_MINUTES;
							}
						}
						else if(((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == WOOLLENS))&&(uiPresent_Step == 4))
						{
							if(uiTempSel == Wash30)
							{
								uiDuration = TEN_MINUTES;
							}
							else if(uiTempSel == Wash40)
							{
								uiDuration = ONE_MINUTES;
							}
							else if(uiTempSel == WashCold)
							{
								uiDuration = FIFTEEN_MINUTES;
							}
						}
						else if(uiSelected_Position == COTTON)
						{
							//if((uiTempSel >= Wash40E)&&(uiPresent_Step == 6))
							if(((uiTempSel == Wash40E)||(uiTempSel == Wash60E))&&(uiPresent_Step == 6))
							{
								if(uiTempSel == Wash40E)
								{
									if((uidisp_select_option & Prewash_Yes) == Prewash_Yes)
									{
										if(uiPrewashWaterResetCount <= 1)
										{
											uiDuration = RESET_VALUE;
										}
										else if(uiPrewashWaterResetCount >= 2)
										{
											uiDuration = FOURTY_MINUTES;
										}
									}
									else
									{
										if(uiWaterResetCount <= 1)
										{
											uiDuration = RESET_VALUE;
										}
										else if(uiWaterResetCount >= 2)
										{
											uiDuration = FOURTY_MINUTES;
										}
									}
									
								}
								else if(uiTempSel == Wash60E)
								{
									if((uidisp_select_option & Prewash_Yes) == Prewash_Yes)
									{
										if(uiPrewashWaterResetCount <= 1)
										{
											uiDuration = THIRTY_THREE_MINUTES;
										}
										else if(uiPrewashWaterResetCount >= 2)
										{
											uiDuration = SIXTY_MINUTES;
										}
									}
									else
									{
										if(uiWaterResetCount <= 1)
										{
											uiDuration = THIRTY_THREE_MINUTES;
										}
										else if(uiWaterResetCount >= 2)
										{
											uiDuration = SIXTY_MINUTES;
										}
									}
								}
							}
							
						}
						else if((uiSelected_Position == BABYWEAR)&&(uiPresent_Step == 8))
						{
							if(bTimeSaver_Selected == TRUE)
							{
								if((uiTempSel == WashCold) || (uiTempSel == P_COLD))
								{
									uiDuration = TWENTY_EIGHT_MINUTES;
								}
								else if((uiTempSel == Wash40) || (uiTempSel == P_40))
								{
									uiDuration = TWELVE_MINUTES;
								}
								else if((uiTempSel == Wash60) || (uiTempSel == P_60))
								{
									uiDuration = FOUR_MINUTES;
								}
							}
							else
							{
								if((uiTempSel == WashCold)||(uiTempSel == Wash40)|| (uiTempSel == P_COLD) || (uiTempSel == P_40))
								{
									if((uiTempSel == WashCold) || (uiTempSel == P_COLD))
									{
										uiDuration = 58;//FIFTY_EIGHT_MINUTES;
									}
									else if((uiTempSel == Wash40) || (uiTempSel == P_40))
									{
										uiDuration = FOURTY_EIGHT_MINUTES;
									}
									
									
								}
								else if((uiTempSel == Wash60) || (uiTempSel == P_60))
								{
									//if((uidisp_select_option & WarmSoak_Yes) == WarmSoak_Yes)
									{
										uiDuration = TWENTY_EIGHT_MINUTES;
									}
									/*else
									{
										uiDuration = FOURTY_MINUTES;
									}*/
								}
							}
						}
						else if((uiSelected_Position == CRADLEWASH)&&(uiPresent_Step == 5))
						{
							if((uiTempSel == WashCold)||(uiTempSel == Wash40))
							{
								uiDuration = THREE_MINUTES;
							}
							else if(uiTempSel == Wash30)
							{
								uiDuration = FIVE_MINUTES;
							}
						}
//						else if((uiSelected_Position == CRADLEWASH)&&(uiPresent_Step == 5))
//						{
//							if((uiTempSel == WashCold)||(uiTempSel == Wash40))
//							{
//								uiDuration = THREE_MINUTES;
//							}
//							else if(uiTempSel == Wash30)
//							{
//								uiDuration = FIVE_MINUTES;
//							}
//						}
						else if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == SPORTWEAR)&&(uiPresent_Step == 9))
						{
							if(bTimeSaver_Selected == TRUE)
							{
								if((uiTempSel == WashCold) || (uiTempSel == P_COLD))
								{
									uiDuration = FIFTEEN_MINUTES;
								}
								else if((uiTempSel == Wash40) || (uiTempSel == P_40))
								{
									uiDuration = FIVE_MINUTES;
								}
							}
							else
							{
								if((uiTempSel == WashCold) || (uiTempSel == P_COLD))
								{
//									if(bCold_Soak_Selected == TRUE)
//									{
//										uiDuration = TWENTY_FIVE_MINUTES;
//									}
//									else
//									{
										uiDuration = THIRTY_MINUTES;
									//}
								}
								else if((uiTempSel == Wash40) || (uiTempSel == P_40))
								{
//									if((uidisp_select_option & WarmSoak_Yes) == WarmSoak_Yes)
//									{
//										uiDuration = THREE_MINUTES;
//									}
//									else
//									{
										uiDuration = EIGHTEEN_MINUTES;
									//}
								}
							}
						}
						else if(TubClean_Activated == TRUE)
						{
							if((uiTempSel == Wash60)||(uiTempSel == Wash95))
							{
								if(uiPresent_Step == 4)
								{
									uiDuration = THREE_MINUTES;
									uiOperationSelected = DUTY_C13;
								}
								else if(uiPresent_Step == 5)
								{
									uiDuration = THREE_MINUTES;
									uiOperationSelected = DUTY_C14;
								}
							}
							else if(uiTempSel == WashCold)
							{
								if((uiPresent_Step == 4)||(uiPresent_Step == 5))
								{
									uiDuration = FIFTEEN_MINUTES;
									uiOperationSelected = DUTY_C22;
								}
								
							}
						}
						
						
						if(step_start == FALSE)
						{
							uiCurrent_step_time = uiDuration;
								if(uiCurrent_step_time > RESET_VALUE)
								step_start = TRUE;
						}
						StepTime =  uiDuration;
						//-----BELOW CODE IS COMMENTED SINCE NO LAST 30 SEC DRAIN LOGIC IN NEO-----//
//						if(WashOperation_Last30secDrain()) /*TODO: Added for Last 30sec Drain*/
//						{
//							//Below condition  is added to avoid the drain of water at >60 degC temp
//							if((uiSelected_Position == COTTON) && 
//							(uiPresent_Step == 27) && 
//							(uiTempSel == Wash95))
//							{
//								bDrainPump = SWITCH_OFF;
//							}
//							else
//							{
//								bDrainPump = SWITCH_ON;
//							}
//						}
//						else
//						{
							if(Tumbling(uiDuration,uiOperationSelected,iResetLevel,iWater_Washlevel,iTumble_Waterlevel,MAIN_WASH))  
							{
								bStep_Finished = YES;
								Time_Update_Basedon_Set_Reset_Count();
							}
//						}
#if ENABLE_IOT
						if(uiTumblingCount == TUMBLING)
						{
							set_IOTState(STATE_IOT_MAINWASH);
						}
#endif						

						break;
			
			case TUMBLINGSPECIAL: 
						if((uiSelected_Position == COTTON)&&/*(uiTempSel >= Wash40E)*/((uiTempSel == Wash40E) || (uiTempSel == Wash60E)))
						{
							uiTumblingState = TUMBLE_MOTION;
						}
						else
						{
							uiTumblingState = SPECIAL_MOTION;
						}
						if(step_start == FALSE)
						{
							uiCurrent_step_time = OperationSelected->eDuration;
							if(uiCurrent_step_time > RESET_VALUE)
							{
								step_start = TRUE;
								
								if(uiCurrent_step_time <= 2)  
								{
									uiCurrent_step_time = 1;
								}
								else if(uiCurrent_step_time > 2) 
								{
									uiCurrent_step_time = uiCurrent_step_time - 1 ;
								}
							}
						}
						StepTime = 1;
						if(TumblingSpecial(OperationSelected->eDuration,uiOperationSelected,iResetLevel,iWater_Washlevel,iTumble_Waterlevel,MAIN_WASH))
						{
							bStep_Finished = YES;
							Time_Update_Basedon_Set_Reset_Count();
						}
#if ENABLE_IOT
						set_IOTState(STATE_IOT_MAINWASH);
#endif
						break;
			
			case REHEAT:
						uiTumblingState = 1;
						uiDuration = OperationSelected->eDuration;
						switch(GET_PROGRAM)
						{
							case MIXEDSOIL:
							case COTTON:
							case SYNTHETIC:
									uiDuration = WashOperation_TimeSaverTime(uiDuration);
									break;
							default:
									break;
						}

						if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == ANTIALLERGEN)&&(uiPresent_Step == 8))
						{
							if(bTimeSaver_Selected == TRUE)
							{
								if((uiTempSel == Wash60) || (uiTempSel == P_60))
								{
									uiDuration = EIGHT_MINUTES;
								}
								else if((uiTempSel == Wash40) || (uiTempSel == P_40))
								{
									uiDuration = FIFTEEN_MINUTES;
								}
								else if((uiTempSel == Wash95) || (uiTempSel == P_95))
								{
									uiDuration = SIX_MINUTES;
								}
							}
							else
							{
								if((uiTempSel == Wash60) || (uiTempSel == P_60))
								{
									uiDuration = 34;//THIRTY_FOUR_MINUTES;
								}
								else if((uiTempSel == Wash40) || (uiTempSel == P_40))
								{
									uiDuration = FIFTY_FOUR_MINUTES;
								}
								else if((uiTempSel == Wash95) || (uiTempSel == P_95))
								{
									uiDuration = TEN_MINUTES;
								}
							}
						}
						else if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == UNIFORM)&&(uiPresent_Step == 9))
						{
							if(bTimeSaver_Selected == TRUE)
							{
								uiDuration = TEN_MINUTES;
								uiOperationSelected = DUTY_C22;
							}
							else
							{
								if((uidisp_select_option & Prewash_Yes) == Prewash_Yes)
								{
									if(uiPrewashWaterResetCount <= 1)
									{
										uiDuration = FIFTEEN_MINUTES;
									}
									else 
									{
										/*if(((uidisp_select_option & WarmSoak_Yes) == WarmSoak_Yes)&&(uiTempSel > WashCold))
										{
											uiDuration = FOURTEEN_MINUTES;
										}
										else
										{*/
											uiDuration = THIRTY_MINUTES;
										//}
									}
								}
								else
								{
									if(uiWaterResetCount <= 1)
									{
										uiDuration = FIFTEEN_MINUTES;
									}
									else 
									{
										/*if(((uidisp_select_option & WarmSoak_Yes) == WarmSoak_Yes)&&(uiTempSel > WashCold))
										{
											uiDuration = FOURTEEN_MINUTES;
										}
										else
										{*/
											uiDuration = THIRTY_MINUTES;
										//}
									}
								}
								uiOperationSelected = DUTY_C28;
							}
						}
						if((TubClean_Activated == TRUE)&&(uiPresent_Step == 6))
						{
							if(uiTempSel == Wash60)
							{
								uiDuration = THIRTY_MINUTES;
							}
							else if(uiTempSel == Wash95)
							{
								uiDuration = FIVE_MINUTES;
							}
						}
						
						if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == ANTIALLERGEN)&&((uiTempSel == Wash40)|| (uiTempSel == P_40)))
						{
							uiCutIn_Temp = 3;
						}
						else
						{
							uiCutIn_Temp = 4;
						}
						
						if(step_start == FALSE)
						{
							uiCurrent_step_time = uiDuration;
							if(uiCurrent_step_time > RESET_VALUE)
							step_start = TRUE;
						}
						StepTime =  uiDuration;
						if(TumblingSpecialHeat(uiDuration,uiOperationSelected,iResetLevel,iWater_Washlevel,uiCutIn_Temp,MAIN_WASH))
						{
							bStep_Finished = YES;
						}
#if ENABLE_IOT
						set_IOTState(STATE_IOT_HEATING);
#endif						
						break;
			case COOL_DOWN:
					uiTumblingState = 1;
					if((((uiTemperature > 60)&&(uiTemperature < 100))||(uiTempSel == Wash95 || uiTempSel == P_95))&&(bNtcFailed == FALSE))
					{
						if(step_start == FALSE)
						{
							//if(uiTempSel != Wash95)
							//BalanceTime = BalanceTime + 6; //( RINSE1 = 5 MINS, DRAIN = 1 MIN, SPIN = 4 MINS)
							
							step_start = TRUE;
							uiCurrent_step_time = 6;
							StepTime =  6;
						}
						if(cooldown(OperationSelected->eDuration,uiOperationSelected,RESET_LEVEL,iWater_Washlevel,iTumble_Waterlevel))
						{
							bMainWashValve = SWITCH_OFF;
		 					bStep_Finished = YES;
						}
#if ENABLE_IOT
						set_IOTState(STATE_IOT_COOL_DOWN);
#endif						
					}
					else
					{
						bMainWashValve = SWITCH_OFF;
						bStep_Finished = YES;
					}
					break;
			case INITIAL_DRAIN:	/*TODO:Added to differentiate start of cycle drain*/ 
			case DRAIN:
						
					bTimeSaver_Start = TRUE;
					bDelay_Start1 = TRUE;
					
					if( bTimeSaver_Selected == TRUE)
					{
						uiPSER_Option_Avbl = uiPSER_Option_Avbl &  (~Steam_Yes);
						uiPSER_Option_Avbl = uiPSER_Option_Avbl & (~WarmSoak_Yes);
						bHotRinse_Selected = FALSE;
						bCold_Soak_Selected = FALSE;
						bSteam_Start = TRUE;
					}
					
					if(uiProgram_Position == ADDITIVESRINSE)
					{
						bExtraRinse_Start = TRUE;
					}
					if(step_start == FALSE)
					{
						uiCurrent_step_time = OperationSelected->eDuration; // reset due to balance time is already take care
							if(uiCurrent_step_time > RESET_VALUE)
								step_start = TRUE;
					}
					StepTime =  OperationSelected->eDuration;
					
					if(bLoadSensing_Completed == FALSE) //to exit LS execution once timeout has reached and wait to go to next step untill motor fault is cleared
					{
						if(draining(OperationSelected->eDuration,TUMBLING_NOT_REQUIRED,NO_DUTY_CYCLE,EMPTYLEVEL))
						{
							if((((uiSelected_Position == COTTON) && ((uiTempSel != Wash40E)&&(uiTempSel != Wash60E)))|| (uiSelected_Position == SYNTHETIC) || (uiSelected_Position == MIXEDSOIL)) && (uiPresent_Step == 1))
							{
								//LS_flag = TRUE;
								if(bUnbalance_Detected == FALSE) //start LS display and timer only when motor fault is cleared
								{
									LS_flag = TRUE;
								}
								  step_start = 0;
								  //uiDuration = 1;
								 
								if(step_start == FALSE)
								{
									MachineErrors.EBits.HighUnbalance = 0;
									bMotorError_Unbalnce = FALSE;
									bUnbalanceError = FALSE;	//reset unbalance before starting spin
										
									//bRinsehold_Finished = TRUE;
									//uiCurrent_step_time = uiDuration;
									uiCurrent_step_time = OperationSelected->eDuration;
									if(uiCurrent_step_time > RESET_VALUE)
										step_start = TRUE;	
								}
								StepTime =  uiDuration;
								bMotor_Start = TRUE;
								bSpinStarted = TRUE;
								bDrainPump = TRUE;
								uiTumblingState = 3;
								motor(SPIN_PROFILE_LS);

						      		if((bSpin_Status == TRUE)||((step_start == TRUE)&&(uiCurrent_step_time == RESET_VALUE)))
								{
									LS_flag = FALSE;
									bStep_Finished = YES;
									bDrain_Reset_Seconds = NO;
									bSpinDelay = YES;
									Load_Sensing_Calculation();
									After_LS_Update_Para();
									uiCurrent_step_time = RESET_VALUE; //added because after load sensing extra step seconds were getting subtracted leading to timing issue
									//BalanceTime += 2;
									uiLS_Seconds = 240;//180;//120;
								}
								else if(uiLS_Seconds == RESET_VALUE)
								{
										bLoadSensing_Completed = TRUE; //to stop LS execution after timeout
										LS_flag = FALSE;
										bDrainPump = FALSE;
									
										switch(GET_PROGRAM)
										{
											case COTTON:
													uiLoadRange = 3;
													break;
											case MIXEDSOIL:
											case SYNTHETIC:
													uiLoadRange = 2;
													break;
													
											default:
												break;
										}
												
										After_LS_Update_Para();
										//bStep_Finished = YES;
										uiCurrent_step_time = RESET_VALUE;
										bDrain_Reset_Seconds = NO;
										bSpinDelay = YES;
									
								}
							}	
							else{
							bStep_Finished = YES;
							bDrain_Reset_Seconds = NO;}
						}
						else
						{
							bDrain_Reset_Seconds = YES;
						}
					}
					else
					{
						if(bUnbalance_Detected == FALSE) //Go to next step only when motor fault is cleared
						{
							bStep_Finished = YES;
							uiCurrent_step_time = RESET_VALUE; //added because after load sensing extra step seconds were getting subtracted leading to timing issue
							bLoadSensing_Completed = RESET_VALUE;
							bDrain_Reset_Seconds = NO;
							bSpinDelay = YES;
							uiLS_Seconds = 240;//180;//120;
						}
					}
#if ENABLE_IOT
					set_IOTState(STATE_IOT_DRAIN);
#endif
					break;
			
						
			case MAINWASH_INTERMIDIATE_SPIN:
					if(Conditional_drain(EMPTYLEVEL))
					{
						if(step_start == FALSE)
						{
							MachineErrors.EBits.HighUnbalance = 0;
							bMotorError_Unbalnce = FALSE;
							bUnbalanceError = FALSE;	//reset unbalance before starting spin
									
							uiCurrent_step_time = OperationSelected->eDuration;	
								if(uiCurrent_step_time > RESET_VALUE)
									step_start = TRUE;
						}
						StepTime =  OperationSelected->eDuration;
						
						if(bFoamDetection == FALSE)
						{
							uiTumblingState = 3;
							bMotor_Start = TRUE;
							bSpinStarted = TRUE;
							if((Spin_Final_Profile_Index == 17) 
							&& (uiSelected_Position == COTTON) 
							&& ((uiTempSel == Wash60E)||(uiTempSel == Wash40E)))
							{
								bDrainPump = FALSE;
							}
							else
							{
								bDrainPump = TRUE;
							}
							
							
							motor(uiOperationSelected);
						}
						else if((uiFoamState != FOAM_SENSING_TUMBLE)&&(bFoamDetection == TRUE))
						{
							bMotor_Start = FALSE;
							bSpinStarted = TRUE;
							//motor(uiOperationSelected);
						}
						FoamCheck();
						if((bSpin_Status == TRUE)||((step_start == TRUE)&&(uiCurrent_step_time == RESET_VALUE)))
						{
							bStep_Finished = YES;
							bDrainPump = SWITCH_OFF;
							uiTest_drain = RESET_VALUE;
							bSpinDelay = YES;
						}
					}
#if ENABLE_IOT
					set_IOTState(STATE_IOT_INTERMEDIATE_SPIN);					
#endif
						
						break;								
			//******************************END OF MAINWASH OPERATION****************************//	
			
			//***************** RINSE OPERTION ***************************//
			case OPTION_EXTRA_RINSE_MAINWASH:
			case OPTION_EXTRA_RINSE_PREWASH:
			case OPTION_EXTRA_ADDITIVE_RINSE:
			 		uiDuration = OperationSelected->eDuration;
					bExtraRinse_Start = TRUE;
					uiTumblingState = 1;
					if(uiSelected_Position == BABYWEAR)
					{
						iRinselevel = BABY_EXTRARINSELEVEL;
					}
					
					if(uiExtraRinse_Press_count > RESET_VALUE)
					{
						
						switch(uiTumblingCount)
						{
							case OPTION_EXTRA_RINSE_MAINWASH:
									if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == WOOLLENS))
									{
										iRinseAdditive = TUMBLE_SPECIAL;
										uiValve_Select = RINSE_MAINWASH ;
									}
									else
									{
										iRinseAdditive = TUMBLE_NORMAL;
										uiValve_Select = RINSE_MAINWASH;
									}
									break;
							case OPTION_EXTRA_RINSE_PREWASH:
									if(uiSelected_Position == CRADLEWASH)
									{
										iRinseAdditive = TUMBLE_SPECIAL;
										uiValve_Select = RINSE_PREWASH ;
									}
									else
									{
										iRinseAdditive = TUMBLE_NORMAL;
										uiValve_Select = RINSE_PREWASH ;
									}
									break;
							case OPTION_EXTRA_ADDITIVE_RINSE:
									iRinseAdditive = FIVE_LITER;
									uiValve_Select = RINSE_BOTH ;
									break;		
						}
						switch(uiSelected_Position)
						{
//							case EXPRESS_30: uiDuration = WashOperation_TimeSaverTime(uiDuration);
//								break;
//							default: break;
							case SMARTPLUS:
								switch(uiSmartProgram_Selected)
								{	
									case EXPRESS_30:	
										uiDuration = WashOperation_TimeSaverTime(uiDuration);
										break;
										
									default:
										break;
								}
								break;
							default:
								break;
						}
						if(step_start == FALSE)
						{
							uiCurrent_step_time = uiDuration;
									if(uiCurrent_step_time > RESET_VALUE)
									step_start = TRUE;
						}
						StepTime =  uiDuration;
						if(bStep_Flag == FALSE)
						{
							step_start = FALSE;
							bStep_Flag = TRUE;
						}
						if(WashOperation_Last30secDrain())
						{
							bDrainPump = SWITCH_ON;
						}
						else
						{
							if(RinseFilling(uiValve_Select,uiDuration,uiOperationSelected,iRinselevel,iRinseAdditive))
							{
								if((uiSelected_Position == ADDITIVESRINSE)&&(uiTumblingCount ==OPTION_EXTRA_ADDITIVE_RINSE)) 
								{
									uiExtraRinse_Press_count--;
								}
								bStep_Finished = YES;
							}
						}
#if ENABLE_IOT
						if(uiTumblingCount == OPTION_EXTRA_RINSE_MAINWASH)
						{
							if((uiSelected_Position == SMARTPLUS)&&((uiSmartProgram_Selected == JEANS)||(uiSmartProgram_Selected == WOOLLENS)))
								set_IOTState(STATE_IOT_EXTRA_RINSE_1);
							else
								set_IOTState(STATE_IOT_EXTRA_RINSE_2);	
						}
						else if (uiTumblingCount == OPTION_EXTRA_RINSE_PREWASH)
						{
							if(((uiSelected_Position == COTTON)&&((uiPresent_Step == 41)||(uiPresent_Step == 47)))||((uiSelected_Position == MIXEDSOIL)&&(uiPresent_Step == 33)))							
								set_IOTState(STATE_IOT_EXTRA_RINSE_3);							
							else
								set_IOTState(STATE_IOT_EXTRA_RINSE_1);
						}
						else
						{
							set_IOTState(STATE_IOT_EXTRA_RINSE_2);
						}
#endif
					}
					else
					{
						bStep_Finished = YES;
					}	
					break;
							
			case OPTIONAL_DRAIN :
					if(uiExtraRinse_Press_count > RESET_VALUE) 
					{
						if(step_start == FALSE)
						{
							uiCurrent_step_time = OperationSelected->eDuration;
								if(uiCurrent_step_time > RESET_VALUE)
								step_start = TRUE;
						}
						StepTime =  OperationSelected->eDuration;
						if(draining(OperationSelected->eDuration,TUMBLING_NOT_REQUIRED,NO_DUTY_CYCLE,EMPTYLEVEL))
						{
							if(uiSelected_Position == EXPRESS)
							{
								//BalanceTime = (BalanceTime - 1);
								//uicycle_mincompleted++;
								uiCurrent_step_time--;
							}
							if(bprewash_selected == TRUE)
							{
								bprewash_selected = FALSE;
							}
							if(((uiSelected_Position == SMARTPLUS)&&
							(/*(uiSmartProgram_Selected == CURTAINS)||*/
							(uiSmartProgram_Selected == WOOLLENS)/*||
							(uiSmartProgram_Selected == LINENWASH)*/))||
							(uiSelected_Position == EXPRESS)||
							(uiSelected_Position == CRADLEWASH)||
							(uiSelected_Position == CURTAINS))
							//||(uiSelected_Position == WETCLEAN))//added wc
							{
								uiExtraRinse_Press_count--;
							}
							bStep_Finished = YES;
							bDrain_Reset_Seconds = NO;
						}
						else
						{
							bDrain_Reset_Seconds = YES;
						}
#if ENABLE_IOT
						set_IOTState(STATE_IOT_DRAIN);			
#endif						
					}
					else
					{
						bStep_Finished = YES;
					}
					break;
					
			case OPTIONAL_SPIN_TUMBLE:
					uiTumblingState = 1;
					if(uiExtraRinse_Press_count > RESET_VALUE)
					{
						if(step_start == FALSE)
						{
							step_start = TRUE;
							uiCurrent_step_time = OperationSelected->eDuration;
						}
						StepTime =  OperationSelected->eDuration;
						if(draining(OperationSelected->eDuration,TUMBLING_REQUIRED,uiOperationSelected,EMPTYLEVEL))
						{
							bStep_Finished = YES;
							bDrain_Reset_Seconds = NO;
							if((uiSelected_Position == SMARTPLUS)&&((uiSmartProgram_Selected == DARKWASH)||(uiSmartProgram_Selected == INNERWEAR)))
							uiExtraRinse_Press_count--;
						}
						else
						{
							bDrain_Reset_Seconds = YES;
						}
#if ENABLE_IOT
					set_IOTState(STATE_IOT_INTERMEDIATE_SPIN);
#endif
					}
					else
					{
						bStep_Finished = YES;
					}
					
					break;
					
			case OPTIONAL_SPIN:

					if(uiExtraRinse_Press_count > RESET_VALUE)
					{
						if(Conditional_drain(EMPTYLEVEL))
						{
							if(step_start == FALSE)
							{
								MachineErrors.EBits.HighUnbalance = 0;
								bMotorError_Unbalnce = FALSE;
								bUnbalanceError = FALSE;	//reset unbalance before starting spin
									
								uiCurrent_step_time = OperationSelected->eDuration;
								if(uiCurrent_step_time > RESET_VALUE)
									step_start = TRUE;
								
							}
							StepTime =  OperationSelected->eDuration;
							if(bFoamDetection == FALSE)
							{
								uiTumblingState = 3;
								bMotor_Start = TRUE;
								bSpinStarted = TRUE;
								bDrainPump = TRUE;
								motor(uiOperationSelected);
							}
							else if((uiFoamState != FOAM_SENSING_TUMBLE)&&(bFoamDetection == TRUE))
							{
								bMotor_Start = FALSE;
								bSpinStarted = TRUE;
								//motor(uiOperationSelected);
							}
							FoamCheck();
							if((bSpin_Status == TRUE)||((step_start == TRUE)&&(uiCurrent_step_time == RESET_VALUE)))
							{
								bStep_Finished = YES;
								bDrainPump = SWITCH_OFF;
								uiTest_drain = RESET_VALUE;
								bSpinDelay = YES;
								uiExtraRinse_Press_count--;	
							}
#if ENABLE_IOT
							set_IOTState(STATE_IOT_INTERMEDIATE_SPIN);						
#endif						
						}
					}
					else
					{
						bStep_Finished = YES;
					}
					break;
			 			
			case RINSE_PREWASH_FILL:
			case RINSE_MAINWASH_FILL:
					uiTumblingState = 1;
					uiDuration = OperationSelected->eDuration;
					switch(uiTumblingCount)
					{
						case RINSE_MAINWASH_FILL:
								uiValve_Select = RINSE_MAINWASH;
								break;
						case RINSE_PREWASH_FILL:
								uiValve_Select = RINSE_PREWASH ;
								break;
					}
					/*switch(uiSelected_Position)
					{
							case EXPRESS_30: uiDuration = WashOperation_TimeSaverTime(uiDuration);
								break;
							default: break;
					}*/
					switch(GET_PROGRAM)
						{
							//case MIXEDSOIL:
							//case COTTON:
							//case SYNTHETIC:
//							case EXPRESS_30:
//									uiDuration = WashOperation_TimeSaverTime(uiDuration);
//									break;

							case SMARTPLUS:
								switch(uiSmartProgram_Selected)
								{	
									case EXPRESS_30:	
										uiDuration = WashOperation_TimeSaverTime(uiDuration);
										break;
										
									default:
										break;
								}
								break;
									
							case MIXEDSOIL:
									if(uiPresent_Step == 38)
									{
										uiDuration = (bTimeSaver_Selected == TRUE)? FOUR_MINUTES : FIVE_MINUTES; break;
										
									}
									break;
									
							case SYNTHETIC:
									if(uiPresent_Step == 44)
									{
										if(((uiTempSel==Wash60)||(uiTempSel==P_60)||(uiTempSel==WashCold)||(uiTempSel==P_COLD))&&(uiLoadRange==0))
										{
											uiDuration = FOUR_MINUTES;
										}
										else
										{
											uiDuration = FIVE_MINUTES;
										}
									}
									break;
									
							case COTTON:
//									if(((uiTempSel<=Wash40)&&(uiPresent_Step == 51))||(((uiTempSel==Wash60)||(uiTempSel==Wash95))&&(uiPresent_Step == 45)))
//									{
//										if(uiLoadRange==0)
//										{
//											uiDuration = THREE_MINUTES;
//										}
//										else
//										{
//											uiDuration = FIVE_MINUTES;
//										}
//										break;
//									}
									if((uiLoadRange==0)&&
									(((uiPresent_Step == 55)&&((uiTempSel==WashCold)||(uiTempSel==Wash40)||(uiTempSel==P_40)||(uiTempSel==P_COLD)))
									||((uiPresent_Step == 48)&&((uiTempSel==Wash60)||(uiTempSel==Wash95)||(uiTempSel==P_60)||(uiTempSel==P_95)))))
									{
										uiDuration = THREE_MINUTES;
									}
									else
									{
										if((uiTempSel!=Wash60E) && (uiTempSel!=Wash40E))
										{
											uiDuration = SIX_MINUTES;
										}
									}
									break;
							
							default: break;
									
						}
					if(step_start == FALSE)
					{
						uiCurrent_step_time = uiDuration;
						if(uiCurrent_step_time > RESET_VALUE)
								step_start = TRUE;
						
						bExtraRinse_Start = TRUE;
					}
					StepTime =  uiDuration;
					if(bStep_Flag == FALSE)
					{
						step_start = FALSE;
						bStep_Flag = TRUE;
					}
					if(WashOperation_Last30secDrain())
					{
						bDrainPump = SWITCH_ON;
					}
					else
					{
						if(RinseFilling(uiValve_Select,uiDuration,uiOperationSelected,iRinselevel,iTumble_Waterlevel))
						{
							bStep_Finished = YES;	
						}
					}
#if ENABLE_IOT				
					if(uiTumblingCount == RINSE_MAINWASH_FILL)
					{
						set_IOTState(STATE_IOT_RINSE_2);
					}
					else //RINSE_PREWASH_FILL
					{	
						if((uiSelected_Position == CRADLEWASH)&&(uiPresent_Step == 12))
							set_IOTState(STATE_IOT_RINSE_2);
						else
							set_IOTState(STATE_IOT_RINSE_1);
					}
#endif					
					break;
			case RINSE_DRAIN:
			case STEAM_DRAIN:
					if((((uidisp_select_option & Steam_Yes) == Steam_Yes)&&(uiTumblingCount == STEAM_DRAIN))||(uiTumblingCount == RINSE_DRAIN))
					{
						bTimeSaver_Start = TRUE;
						bDelay_Start1 = TRUE;
						if(uiProgram_Position == ADDITIVESRINSE)
						{
							bExtraRinse_Start = TRUE;
						}
						if(step_start == FALSE)
						{
							uiCurrent_step_time = OperationSelected->eDuration; // reset due to balance time is already take care
							if(uiCurrent_step_time > RESET_VALUE)
									step_start = TRUE;
							
						}
						StepTime =  OperationSelected->eDuration;
						if(((uidisp_select_option & Steam_Yes) == Steam_Yes)&&(uiTumblingCount == STEAM_DRAIN))
						{
							if(draining(OperationSelected->eDuration,TUMBLING_REQUIRED,uiOperationSelected,EMPTYLEVEL))
							{
								bStep_Finished = YES;
								uiCurrent_step_time = RESET_VALUE;
								bDrain_Reset_Seconds = NO;
							}
						}
						else
						{
							if(draining(OperationSelected->eDuration,TUMBLING_NOT_REQUIRED,NO_DUTY_CYCLE,EMPTYLEVEL))
							{
								bStep_Finished = YES;
								uiCurrent_step_time = RESET_VALUE;
								bDrain_Reset_Seconds = NO;
							}
						}
#if ENABLE_IOT
						set_IOTState(STATE_IOT_DRAIN);					
#endif	
					}
					else
					{
						bStep_Finished = YES;
					}
						
					break;
					
			case MAINWASH_HALFDRAIN:
			case RINSE_HALFDRAIN:
					bDelay_Start1 = TRUE;
					if(step_start == FALSE)
					{
						uiCurrent_step_time = OperationSelected->eDuration; // reset due to balance time is already take care	
						if(uiCurrent_step_time > RESET_VALUE)
								step_start = TRUE;
					}
					StepTime =  OperationSelected->eDuration;
					
					//if(((uiSelected_Position == MIXEDSOIL)||(uiAISelected_Position == MIXEDSOIL)) && (uiPresent_Step == 24)) //added 09-02-22
					if(uiTumblingCount==MAINWASH_HALFDRAIN)
					{
						if(draining(OperationSelected->eDuration,TUMBLING_NOT_REQUIRED,NO_DUTY_CYCLE,HYGINE_DRAIN_1))
						{
							bStep_Finished = YES;
							uiCurrent_step_time = RESET_VALUE;
							bDrain_Reset_Seconds = NO;
						}
						else
						{
							bDrain_Reset_Seconds = YES;	
						}
					}
					else 
					{
						if(draining(OperationSelected->eDuration,TUMBLING_NOT_REQUIRED,NO_DUTY_CYCLE,HYGINE_DRAIN))
						{
							bStep_Finished = YES;
							uiCurrent_step_time = RESET_VALUE;
							bDrain_Reset_Seconds = NO;
						}
						else
						{
							bDrain_Reset_Seconds = YES;	
						}
					}
					break;
						
			case RINSE_INTERMIDIATE_SPIN:
					if(Conditional_drain(EMPTYLEVEL))
					{
						if(step_start == FALSE)
						{
							MachineErrors.EBits.HighUnbalance = 0;
							bMotorError_Unbalnce = FALSE;
							bUnbalanceError = FALSE;	//reset unbalance before starting spin
									
							uiCurrent_step_time = OperationSelected->eDuration;
							if(uiCurrent_step_time > RESET_VALUE)
									step_start = TRUE;
						}
						StepTime =  OperationSelected -> eDuration;
						
						if(bFoamDetection == FALSE)
						{
							uiTumblingState = 3;
							bMotor_Start = TRUE;
							bSpinStarted = TRUE;
							//bDrainPump = TRUE;
							
							if((Spin_Final_Profile_Index == 17) 
							&& (uiSelected_Position == COTTON) 
							&& ((uiTempSel == Wash60E)||(uiTempSel == Wash40E)))
							{
								bDrainPump = FALSE;
							}
							else
							{
								bDrainPump = TRUE;
							}
							
							motor(uiOperationSelected);
						}
						else if((uiFoamState != FOAM_SENSING_TUMBLE)&&(bFoamDetection == TRUE))
						{
							bMotor_Start = FALSE;
							bSpinStarted = TRUE;
							//motor(uiOperationSelected);
						}
						FoamCheck();	
						if((bSpin_Status == TRUE)||((step_start == TRUE)&&(uiCurrent_step_time == RESET_VALUE)))
						{
							//BalanceTime = BalanceTime - (OperationSelected->eDuration);
							bStep_Finished = YES;
							bDrainPump = SWITCH_OFF;
							uiTest_drain = RESET_VALUE;
							bSpinDelay = YES;
						}
					}
#if ENABLE_IOT
					set_IOTState(STATE_IOT_INTERMEDIATE_SPIN);
#endif					
					break;			
						
			case HOTRINSE:		
					uiTumblingState = 1;
					if(bTimeSaver_Selected == FALSE)
					{
						if(bHotRinse_Selected == TRUE)
						{
							bHotRinse_Start = TRUE;
							if(step_start == FALSE)
							{
								uiCurrent_step_time = OperationSelected->eDuration;	
								if(uiCurrent_step_time > RESET_VALUE)
								step_start = TRUE;
							}
							StepTime =  OperationSelected->eDuration;
							uiTemperature_Required = TEMP_40;
							if(WashOperation_Last30secDrain())
							{
								bDrainPump = SWITCH_ON;
							}
							else
							{
								if(heating(OperationSelected->eDuration,uiOperationSelected,uiTemperature_Required,iWater_Washlevel,iTumble_Waterlevel,iResetLevel)) 
								{
									bStep_Finished = YES;
								}
							}
#if ENABLE_IOT
							set_IOTState(STATE_IOT_HOT_RINSE);
#endif							
							
						}
						else
						{
							bStep_Finished = YES;
							bHotRinse_Start = TRUE;
							
						}
					}
					else
					{
						bHotRinse_Start = TRUE;
						bStep_Finished = YES;
					}
					break;
			case RINSE3_ADDITIVES: 
					uiTumblingState = 1;
					uiDuration = OperationSelected->eDuration;
					/*switch(uiSelected_Position)
					{
						case EXPRESS_30: uiDuration = WashOperation_TimeSaverTime(uiDuration);
							break;
						default: break;
					}*/
					switch(GET_PROGRAM)
						{
							//case MIXEDSOIL:
							//case COTTON:
							//case SYNTHETIC:
//							case EXPRESS_30:
//									uiDuration = WashOperation_TimeSaverTime(uiDuration);
//									break;

							case SMARTPLUS:
								switch(uiSmartProgram_Selected)
								{	
									case EXPRESS_30:	
										uiDuration = WashOperation_TimeSaverTime(uiDuration);
										break;
										
									default:
										break;
								}
								break;
									
							case MIXEDSOIL:
									if(uiPresent_Step == 44)
									{
										uiDuration = (bTimeSaver_Selected == TRUE)? FOUR_MINUTES : FIVE_MINUTES; break;
									}
									break;
							
							case SYNTHETIC:
									
									if(uiPresent_Step == 50)
									{
										if(((uiTempSel==Wash60)||(uiTempSel==P_60)||(uiTempSel==WashCold)||(uiTempSel==P_COLD))&&(uiLoadRange==0))
										{
											uiDuration = FOUR_MINUTES;
										}
										else
										{
											uiDuration = SIX_MINUTES;
										}
									}
									break;
									
							case COTTON:
//									if(((uiTempSel<=Wash40)&&(uiPresent_Step == 57))||(((uiTempSel==Wash60)||(uiTempSel==Wash95))&&(uiPresent_Step == 51)))
//									{
//										if(uiLoadRange==0)
//										{
//											uiDuration = THREE_MINUTES;
//										}
//										else
//										{
//											uiDuration = SIX_MINUTES;
//										}
//									}
									if((uiLoadRange==0)&&
									(((uiPresent_Step == 60)&&((uiTempSel==WashCold)||(uiTempSel==Wash40)||(uiTempSel==P_COLD)||(uiTempSel==P_40)))
									||((uiPresent_Step == 53)&&((uiTempSel==Wash60)||(uiTempSel==Wash95)||(uiTempSel==P_60)||(uiTempSel==P_95)))))
									{
										uiDuration = FOUR_MINUTES;
									}
									else
									{
										if((uiTempSel!=Wash60E) && (uiTempSel!=Wash40E))
										{
											uiDuration = SIX_MINUTES;
										}
									}
									
									break;
									
							default: break;
									
						}
					if(step_start == FALSE)
					{
						uiCurrent_step_time = uiDuration;
						if(uiCurrent_step_time > RESET_VALUE)
								step_start = TRUE;
						
					}
					StepTime =  uiDuration;
					if(bStep_Flag == FALSE)
					{
						step_start = FALSE;
						bStep_Flag = TRUE;
					}
					if(((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == WOOLLENS))||(uiSelected_Position == CRADLEWASH))//||(uiSelected_Position == WETCLEAN))
					{
						iRinseAdditive = TUMBLE_SPECIAL;
					}
					else if((uiSelected_Position == COTTON)&&(uiTempSel == Wash60E))
					{
						iRinseAdditive = TUMBLE_NORMAL;
					}
					else
					{
						iRinseAdditive = FIVE_LITER;	
					}
					if(WashOperation_Last30secDrain())
					{
						bDrainPump = SWITCH_ON;
					}
					else
					{
						if(RinseFilling(RINSE_BOTH,uiDuration,uiOperationSelected,iRinselevel,iRinseAdditive))
						{
							//BalanceTime = BalanceTime - (OperationSelected->eDuration);
							bStep_Finished = YES;
						}
					}
#if ENABLE_IOT
					set_IOTState(STATE_IOT_FINAL_RINSE);
#endif					
					break;
			case RINSE_HOLD:
					if(step_start == FALSE)
					{
						uiCurrent_step_time = OperationSelected->eDuration;
						if(uiCurrent_step_time > RESET_VALUE)
								step_start = TRUE;
						
					}
					StepTime =  OperationSelected->eDuration;
					if(uiRpmSel == Rinsehold )
					{
						if(RinseHold())
						{
							bStep_Finished = YES;
							bRinsehold_Finished = TRUE;
						}
#if ENABLE_IOT
						set_IOTState(STATE_IOT_RINSEHOLD);						
#endif						
					}
					else
					{
						bStep_Finished = YES;
						bRinsehold_Finished = TRUE;
					}
					break;
						
			//***************************** END OF RINSE OPERATION *****************************//	
			
			//****************************** FINAL SPIN OPERATION *******************************//
       			case SPIN:
					if(Conditional_drain(EMPTYLEVEL))
					{
						uiDuration = OperationSelected->eDuration;
						if((uiSelected_Position == EXPRESS) /*&& (bExpress_30_Activatted == FALSE)*/)
						{
							if (((Foam_Removal_Complete & 0x02) != 0x02)&&(bFoamRm_Step == 0))	//(uiPresent_Step == 16) &&
							{
								Foam_Removal_Enable = TRUE;	//for spin program
							}
							
							Foam_Removal_prewash_gui8();	//SPIN step
							if(uiRpmSel == Rpm1000)
							{
								 uiDuration = SIX_MINUTES;
								 uiOperationSelected = SPIN_PROFILE_FS9;
							}
							else if(uiRpmSel == Rpm1200)
							{
								 uiDuration = EIGHT_MINUTES;
								 uiOperationSelected = SPIN_PROFILE_FS10;
							}
							else
							{
								uiDuration = THREE_MINUTES;
								uiOperationSelected = SPIN_PROFILE_FS4;
							}
						}
						//if(uiSelected_Position == EXPRESS_30)
						if((uiSelected_Position == SMARTPLUS) && (uiSmartProgram_Selected == EXPRESS_30))
						{
							if(uiRpmSel == Rpm1200)
							{
								 uiDuration = EIGHT_MINUTES;
								 uiOperationSelected = SPIN_PROFILE_FS10;
							}
							else if(uiRpmSel == Rpm1000)
							{
								 uiDuration = SIX_MINUTES;
								 uiOperationSelected = SPIN_PROFILE_FS9;
							}
						}
						//if((uiRpmSel == Rpm000)&&((uiOperationSelected >= SPIN_PROFILE_FS2)&&(uiOperationSelected <= SPIN_PROFILE_FS12)))
						if((uiRpmSel == Rpm000)&&(((uiOperationSelected >= SPIN_PROFILE_FS2)&&(uiOperationSelected <= SPIN_PROFILE_FS16)) || (uiOperationSelected == SPIN_PROFILE_FS1)))
						{
							bRinsehold_Finished = TRUE;
							bStep_Finished = YES;
							bDrainPump = SWITCH_OFF;
							uiTest_drain = RESET_VALUE;
							bPrewashValve = SWITCH_OFF;	// TODO: Express Issue At end water filling resolved.
						}
						else
						{
							if(step_start == FALSE)
							{
								MachineErrors.EBits.HighUnbalance = 0;
								bMotorError_Unbalnce = FALSE;
								bUnbalanceError = FALSE;	//reset unbalance before starting spin
									
								bRinsehold_Finished = TRUE;
								uiCurrent_step_time = uiDuration;
								if(uiCurrent_step_time > RESET_VALUE)
									step_start = TRUE;	
							}
							StepTime =  uiDuration;
							if(bFoamDetection == FALSE)
							{
								bMotor_Start = TRUE;
								bSpinStarted = TRUE;
								bDrainPump = TRUE;
								uiTumblingState = 3;
								motor(uiOperationSelected);
							}
							else if((uiFoamState != FOAM_SENSING_TUMBLE)&&(bFoamDetection == TRUE))
							{
								bMotor_Start = FALSE;
								bSpinStarted = TRUE;
								//motor(uiOperationSelected);
							}
							FoamCheck();
							if((bSpin_Status == TRUE)||((step_start == TRUE)&&(uiCurrent_step_time == RESET_VALUE)))
							{
								//BalanceTime = BalanceTime - (OperationSelected->eDuration);
								//uiCurrent_step_time
								bStep_Finished = YES;
								bDrainPump = SWITCH_OFF;
								uiTest_drain = RESET_VALUE;
								bSpinDelay = YES;
								
							}
#if ENABLE_IOT
							set_IOTState(STATE_IOT_FINAL_SPIN);
#endif						
						}
					}
					//added to skip declaring unbalance error when it is detected for intermediate spin but 0rpm is selected due to which final spin is skipped 
					//if((uiRpmSel == Rpm000)&&((uiOperationSelected >= SPIN_PROFILE_FS2)&&(uiOperationSelected <= SPIN_PROFILE_FS12)))
					if((uiRpmSel == Rpm000)&&(((uiOperationSelected >= SPIN_PROFILE_FS2)&&(uiOperationSelected <= SPIN_PROFILE_FS16)) || (uiOperationSelected == SPIN_PROFILE_FS1)))
					{
						bMotorError_Unbalnce = FALSE;
				                bUnbalanceError = FALSE;
					}
					else
					{
						if(uiUnbalance_Level == 3)
						{
							bMotorError_Unbalnce = TRUE;
					                bUnbalanceError = TRUE;
						}
						else
						{
							bMotorError_Unbalnce = FALSE;
					                bUnbalanceError = FALSE;
						}
					}
//					if(uiUnbalance_Level == 3)
//					{
//						bMotorError_Unbalnce = TRUE;
//				                bUnbalanceError = TRUE;
//					}
//					else
//					{
//						bMotorError_Unbalnce = FALSE;
//				                bUnbalanceError = FALSE;
//					}
					
					break;

			case ANTI_CREASE:
					uiTumblingState = 1;
					
					if((bTimeSaver_Selected == FALSE)&&(bUnbalanceError == FALSE)&&(uiRpmSel != Rpm000)&&(uiTumblingCount == ANTI_CREASE))
					{
						if((uiSelected_Position == COTTON)||(uiSelected_Position == SYNTHETIC) || (uiSelected_Position == MIXEDSOIL))
						{
							if(uiLoadRange >= 1)
							{
								uiDuration = OperationSelected->eDuration;
							}
							else
							{
								bStep_Finished = YES;
							}
						}
						else
						{
							uiDuration = OperationSelected->eDuration;	
						}
						
						if(step_start == FALSE)
						{
							uiCurrent_step_time = uiDuration;
							if(uiCurrent_step_time > RESET_VALUE)
							step_start = TRUE;	
						}
						StepTime = uiDuration;
						if(bStep_Flag == FALSE)
						{
							step_start = FALSE;
							bStep_Flag = TRUE;
						}
						if(anticrease(uiDuration,uiOperationSelected))
						{
			 				bStep_Finished = YES;
							
						}
#if ENABLE_IOT
						if(uiTumblingCount == ANTI_CREASE)
						{
							set_IOTState(STATE_IOT_ANTI_CREASE);
						}						
#endif					
					}
					else
					{
						bStep_Finished = YES;	
					}
					break;
					
			case STEAM_CREASE:
					uiTumblingState = 1;
					if(((bTimeSaver_Selected == FALSE)&&(bUnbalanceError == FALSE)&&(uiRpmSel != Rpm000)&&(uiTumblingCount == ANTI_CREASE)||
					(uiSelected_Position == STEAMFRESH)||
					((uidisp_select_option & Steam_Yes) == Steam_Yes)&&(bTimeSaver_Selected == FALSE)&&(bUnbalanceError == FALSE)&&(uiRpmSel != Rpm000)&&(uiTumblingCount == STEAM_CREASE)))
					{
						
						if(step_start == FALSE)
						{
							uiCurrent_step_time = OperationSelected->eDuration;
							if(uiCurrent_step_time > RESET_VALUE)
							step_start = TRUE;
							
						}
						StepTime =  OperationSelected->eDuration;
						if(bStep_Flag == FALSE)
						{
							step_start = FALSE;
							bStep_Flag = TRUE;
						}
						if(anticrease(OperationSelected->eDuration,uiOperationSelected))
						{
							//BalanceTime = BalanceTime - (OperationSelected->eDuration);
			 				bStep_Finished = YES;
							
						}
#if ENABLE_IOT
						if(uiTumblingCount == ANTI_CREASE)
						{
							set_IOTState(STATE_IOT_ANTI_CREASE);
						}						
#endif					
					}
					else
					{
						bStep_Finished = YES;	
					}
					break;
				
			case DRAIN_TUMBLE:
			case SPIN_TUMBLE:
			case MAIN_TUMBLE:
					uiTumblingState = 1;
					if(step_start == FALSE)
					{
						uiCurrent_step_time = OperationSelected->eDuration;
						if(uiCurrent_step_time > RESET_VALUE)
								step_start = TRUE;
					}
					StepTime =  OperationSelected->eDuration;
					if(draining(OperationSelected->eDuration,TUMBLING_REQUIRED,uiOperationSelected,EMPTYLEVEL))
					{
						bStep_Finished = YES;
						bDrain_Reset_Seconds = NO;
					}
					else
					{
						
						bDrain_Reset_Seconds = YES;
					}
#if ENABLE_IOT	
					if(uiTumblingCount == SPIN_TUMBLE)
					{
						set_IOTState(STATE_IOT_INTERMEDIATE_SPIN);	
					}						
					else if(uiTumblingCount == DRAIN_TUMBLE)
					{												
						set_IOTState(STATE_IOT_DRAIN);												
					}				
					else
					{
						set_IOTState(STATE_IOT_MAINWASH);	
					}											
#endif			
					break;
					
			case STEAM_FILL:
					uiTumblingState = 1;
					if((( uidisp_select_option & Steam_Yes) ==  Steam_Yes)&&(((bTimeSaver_Selected == FALSE)&&(bUnbalanceError == FALSE)/*&&(uiRpmSel != Rpm000)*/)||(uiSelected_Position == STEAMFRESH)) /*|| (bExpress_30_Activatted == TRUE)*/)
					{
						if(step_start == FALSE)
						{
							uiCurrent_step_time = OperationSelected->eDuration;
							if(uiCurrent_step_time > RESET_VALUE)
									step_start = TRUE;
						}
						StepTime =  OperationSelected->eDuration;
						if(SteamFill(RINSE_MAINWASH,41419))
						{
							bStep_Finished = YES;
						}
					}
					else
					{
						bStep_Finished = YES;
					}					
					break;
					
			case STEAM:
			
					if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == STEAMWASH /*|| bExpress_30_Activatted == TRUE*/)&&(uiPresent_Step==4)&&(uiCurrent_step_time==RESET_VALUE))
					{
						Delay_ms(1000);Delay_ms(1000);Delay_ms(1000);
					}
					uiTumblingState = 1;
					if((( uidisp_select_option & Steam_Yes) ==  Steam_Yes)&&(((bTimeSaver_Selected == FALSE)&&(bUnbalanceError == FALSE)/*&&(uiRpmSel != Rpm000)*/)||(uiSelected_Position == STEAMFRESH))
					||(((uiSelected_Position == MIXEDSOIL)||(uiAISelected_Position == MIXEDSOIL))&&(uiPresent_Step == 25)) /*|| (bExpress_30_Activatted == TRUE)*/)
					{
						uiDuration = OperationSelected->eDuration;
						bSteam_Start = YES;
						if(step_start == FALSE)
						{
							uiCurrent_step_time = uiDuration;
							if(uiCurrent_step_time > RESET_VALUE)
							step_start = TRUE;
						}
						StepTime =  uiDuration;
						uiTemperature_Required = TEMP_90;
						if(SteamHeat(uiDuration,uiOperationSelected,41460,41419,10,MAIN_WASH))
						{
							bStep_Finished = YES;
							bSteam_Start = YES;
						}
					}
					else
					{
						bStep_Finished = YES;
						bSteam_Start = YES;
					}
					break;
					
			case STEAM_CP:
					
					uiDuration = OperationSelected->eDuration;
					
					if(step_start == FALSE)
					{
						uiCurrent_step_time = uiDuration;
						if(uiCurrent_step_time > RESET_VALUE)
							step_start = TRUE;
					}
					StepTime =  uiDuration;
					if(uiStep_Minutes_Completed < StepTime)
					{
						bCirculation_Pump = SWITCH_ON;
					}
					else
					{
						bStep_Finished = YES;
						bCirculation_Pump = SWITCH_OFF;
						Delay_ms(1000);Delay_ms(1000);Delay_ms(1000);
					}
					
					break;
					
			case IDLE_STAGE:
					bStep_Finished = YES;
					break;
					
			//****************************** END OF FINAL SPIN OPERATION *******************************//			

			default: 			//No operation required
					break;
							
		
	}
	//StepTime =  OperationSelected->eDuration;
	if(bFoamDetection == TRUE)
	{
		FoamSensing();
	}
	if(bStep_Finished == YES)
	{
		StepFinished();	
	}
	
	}

}
void StepFinished(void)
{
	bStep_Finished = RESET_VALUE;
	bStop_Minutes_Count = RESET_VALUE;
	bCirculation_Pump = SWITCH_OFF;
 	uiStep_Minutes_Completed = RESET_VALUE;
	uiheat_Minutes_Completed = RESET_VALUE;
	uiheat_Seconds_Completed = RESET_VALUE;
	bHalfMinute_Process_Start = RESET_VALUE;
	uiHalfMinutes_Seconds = RESET_VALUE;
	uiHalfMinute_Completed = RESET_VALUE;
	bFoamDetection = FALSE;
	uiTest_drain =  RESET_VALUE;
	ucCount = RESET_VALUE;
	//bMotor_Start = NO;
	bDrainPump = FALSE;
	bSpinStarted = FALSE;
	uiFoamState = FALSE;
	MotorStop();
	//ucMotor_Rotate_Seconds = RESET_VALUE;
	WashOperationLast30sDrain_state_ui8 = RESET_VALUE;
	bSpinDelay = RESET_VALUE;
	bStep_Flag = FALSE;
	
	//uiSilkTumble = RESET_VALUE;
	uiReheatTime = RESET_VALUE;
	uiUnbalance_Value = RESET_VALUE;
	bWater_Fill_Completed = RESET_VALUE;
	uiError_Mode=RESET_VALUE;
	uiLowPressureTime = RESET_VALUE;
	uiNoWaterTime = RESET_VALUE;
	bSpin_Start = RESET_VALUE;
	bWaterLevel_Read_Required = YES;
	uiHeater_TurnOn_Delay_Value = 3;
	bHeater_Delay_Start = RESET_VALUE;
	bTumbleSpecial = RESET_VALUE;
	bWaterlevel_Desired = RESET_VALUE;
	bWater_ResetLevel = RESET_VALUE;
	CP_State = RESET_VALUE;
	LS_flag = FALSE;//to come out from displaying LS pattern
	if(Foam_Removal_Enable)
	{
		//Foam_Removal_Complete = 0x01;
	//	Foam_Removal_Complete |= (uint8_t)(0x01 << Foam_Removal_Complete);//1
		Foam_Removal_Enable = 0x00;	//step reset
	}
	
	if(step_start == TRUE )
	{
		step_start = FALSE;
		//if((uiCurrent_step_time > RESET_VALUE)&&(uiTumblingCount !=TUMBLINGSPECIAL))
		
		if(uiTumblingCount == COOL_DOWN) //TODO: Added for hold the timing at the phase of CoolDown
		{
			;
		}
		else if(((uiSelected_Position == MIXEDSOIL)||(uiAISelected_Position == MIXEDSOIL))&&(uiTumblingCount == SOAK)&&(uiPresent_Step < 15))//||(uiPresent_Step == 12)||(uiPresent_Step == 14))) //TODO: Added for hold the timing at the phase of Soak in Mixed for 30 sec logic(6min=3min)
		{
			;
		}
//		else if((uiSelected_Position == WETCLEAN)&&(uiTumblingCount == SOAK))//&&((uiPresent_Step == 4)||(uiPresent_Step == 6)||(uiPresent_Step == 8)||(uiPresent_Step == 10)||(uiPresent_Step == 12))) //TODO: Added for hold the timing at the phase of Soak in Wet Clean
//		{
//			;
//		}
		else if((uiCurrent_step_time > RESET_VALUE)&&(uiCurrent_step_time < 254 ))
		{
			BalanceTime = BalanceTime - uiCurrent_step_time;
			uicycle_mincompleted = uicycle_mincompleted + uiCurrent_step_time ;
			uiCurrent_step_time = RESET_VALUE; // Added to reset current step
		}
		else
		{
			uiCurrent_step_time = RESET_VALUE;
		}
	}
	
	if(bTumbleSpecial == NO)
	{	
		if(uiPresent_Step < uiProgramLast_step)
		{
			if(BalanceTime <= 0)
				BalanceTime = 1;
		}
		else if(uiPresent_Step == uiProgramLast_step)
		{
			BalanceTime = 1;
		}
		uiNextStep_Start_Delay = TWO_SECONDS;
	}
	else
	{
		uiCurrent_step_time = RESET_VALUE;
		uiNextStep_Start_Delay = TWO_SECONDS;	
	}
	
	uiStep_Seconds = RESET_VALUE;
	uiLS_Seconds = 240;//180;//120;
	bLoadSensing_Completed = FALSE;
	
	uiTime_Seconds = RESET_VALUE;
	uiTime_Minutes_Completed = RESET_VALUE;
	
	uiPresent_Step++;
	bFoamRm_Step = 0;
	MemoryMirror();
}

/***********************************************************************
* Function_Name : WashOperation_TimeSaverTime(uint8_t ).
* Ret_type	: uint8_t.
* Working	: To fetch the step time.
* Developed By	: Prashant_H
* Date		: 11/02/2022
************************************************************************/
uint8_t WashOperation_TimeSaverTime(uint8_t uiDuration)
{
	//uint8_t RetTime;
	switch(GET_PROGRAM)
	{
		case MIXEDSOIL:
				switch(uiTempSel)
				{
					case P_COLD:
					case WashCold:
							switch(uiPresent_Step)
							{	
								case 16: RetTime = SIX_MINUTES; 
									 break;	
								case 21: case 22: RetTime = IDLE; 
									 break;	
								default: RetTime = uiDuration;	
									 break;
							}
							break;
					case P_30:
					case P_60:
					case Wash30:
					case Wash60:
							switch(uiPresent_Step)
							{	
								case 16: 	RetTime = TWO_MINUTES; 
										break;
								case 21:	RetTime = (bTimeSaver_Selected == TRUE)?TWO_MINUTES: IDLE; break;
								case 22:	RetTime = (bTimeSaver_Selected == TRUE)?ONE_MINUTES: IDLE; break;
								default:	RetTime = uiDuration;break;
							}
							break;
					case P_40:
					case Wash40:
							switch(uiPresent_Step)
							{	
								case 16: 	RetTime = TWO_MINUTES; 
										break;
								case 21:	RetTime = (bTimeSaver_Selected == TRUE)?ONE_MINUTES: IDLE; break;
								case 22:	RetTime = (bTimeSaver_Selected == TRUE)?ONE_MINUTES: IDLE; break;
								default:	RetTime = uiDuration;break;
							}
							break;
							
					default:	RetTime = uiDuration;
							break;
					
				}
				break;
		case COTTON:
				switch(uiTempSel)
				{
					case Wash95:
					case P_95:/*In COTTON for both-temp programs time and duty is same but incase is changed in future make other case for both*/
							switch(uiPresent_Step)
							{
								//case 11	: RetTime = (bTimeSaver_Selected == TRUE)?IDLE : ONE_MINUTES; break;
								
								case 18 : case 19 :
								case 20	: case 21 :
								 		//RetTime = (bTimeSaver_Selected == TRUE)?ONE_MINUTES : IDLE; break;
										RetTime = (bTimeSaver_Selected == TRUE)?IDLE : IDLE; break;
										
//								case 27	: 
//									if(uiLoadRange==1)
//									{
//										RetTime = (bTimeSaver_Selected == TRUE)?TWO_MINUTES : NINE_MINUTES; 
//									  	uiOperationSelected = (bTimeSaver_Selected == TRUE)? DUTY_C23 : DUTY_C24;
//									}
//									else
//									{
//										RetTime = (bTimeSaver_Selected == TRUE)?TWO_MINUTES : TEN_MINUTES; 
//									  	uiOperationSelected = (bTimeSaver_Selected == TRUE)? DUTY_C23 : DUTY_C24;	
//									}
									break;
								default	: RetTime = uiDuration; break;
							}
							break;
					case Wash60:
					case P_60:
							switch(uiPresent_Step)
							{
								//case 11	: RetTime = (bTimeSaver_Selected == TRUE)?IDLE : ONE_MINUTES; break;
								
								case 18 : case 19 :
								case 20	: case 21 :
								 		RetTime = (bTimeSaver_Selected == TRUE)?ONE_MINUTES : IDLE; break;
										
//								case 27	: RetTime = (bTimeSaver_Selected == TRUE)?TWO_MINUTES : TEN_MINUTES; 
//									  uiOperationSelected = (bTimeSaver_Selected == TRUE)? DUTY_C23 : DUTY_C24;
//										break;
								default	: RetTime = uiDuration; break;
							}
							break;
					case Wash40:
					case P_40:
							switch(uiPresent_Step)
							{
								case 9:RetTime = (uiLoadRange == 0)?TWO_MINUTES :TWO_MINUTES; //FOUR_MINUTES
									break;
								case 11: RetTime = (uiLoadRange <= 1)?SIX_MINUTES : SIX_MINUTES;
									break;
//								case 9: RetTime = TWO_MINUTES;
//										break;
//								case 11: RetTime = ONE_MINUTES;
//										 break;
//								case 15 : case 16 :
//								case 17 : case 18 :
//								
//										RetTime = (bTimeSaver_Selected == TRUE)?ONE_MINUTES : IDLE; 
//										break;
										
								case 19: case 20: case 21: 
								case 22: case 23: case 24:
								case 25: case 26: case 27:case 28 :
										//if(uiLoadRange<=1)//if(uiFinalWt>=5)
										if( (uiLoadRange==1) || (uiLoadRange==2) || (uiLoadRange==3))
										{
											RetTime = (bTimeSaver_Selected == TRUE)?ONE_MINUTES : IDLE;
										}
										else
										{
											RetTime = IDLE;	
										}	
										break;		
								
//								case 33	: RetTime = (bTimeSaver_Selected == TRUE)?TWO_MINUTES : TEN_MINUTES;
//									  uiOperationSelected = (bTimeSaver_Selected == TRUE)? DUTY_C23 : DUTY_C24;
//										break;
								default	: RetTime = uiDuration; break;
							}
							break;
					case Wash30:
					case P_30:
							switch(uiPresent_Step)
							{
								case 9:RetTime = TWO_MINUTES;//RetTime = (uiLoadRange == 0)?TWO_MINUTES :THREE_MINUTES; //FOUR_MINUTES
									break;
								case 11: RetTime = (uiLoadRange <= 1)?SIX_MINUTES : SIX_MINUTES;
									break;
//								case 11:RetTime = FOUR_MINUTES;
//									break;
//								case 15 : case 16 :
//								case 17 : case 18 :
//								
//										RetTime = (bTimeSaver_Selected == TRUE)?ONE_MINUTES : IDLE; 
//										break;
//										
										
								case 19: case 20: case 21: 
								case 22: case 23: case 24:
								case 25: case 26: case 27:case 28 :
										//if(uiLoadRange<=1)//if(uiFinalWt>=5)
										//if( (uiLoadRange==1) || (uiLoadRange==2) || (uiLoadRange==3))
										//{
											RetTime = (bTimeSaver_Selected == TRUE)?ONE_MINUTES : IDLE;
										//}
										//else
										//{
										//	RetTime = IDLE;	
										//}	
										break;	
										
//								case 33	: RetTime = (bTimeSaver_Selected == TRUE)?FIVE_MINUTES : TEN_MINUTES;
//									  uiOperationSelected = (bTimeSaver_Selected == TRUE)? DUTY_C23 : DUTY_C24;
//										break;
								default	: RetTime = uiDuration; break;
							}

							break;
					case WashCold:
					case P_COLD:
							switch(uiPresent_Step)
							{
								case 9:RetTime = (uiLoadRange == 0)?THREE_MINUTES :TWO_MINUTES; //FOUR_MINUTES
									break;
								case 11: RetTime = (uiLoadRange <= 1)?TWELVE_MINUTES : SIX_MINUTES;
									break;
//								case 11:RetTime = (uiLoadRange == 2)?NINE_MINUTES :FOUR_MINUTES; //FOUR_MINUTES
//									break;
//								case 15 : case 20 : case 16 : case 21 : 
//								case 17 : case 22 : case 18 : case 23 :
//								case 19	: case 24 :	
//										RetTime = IDLE; /*For Cold all phase are skiped whether timesaver selected or not*/
//										break;
//								case 33	: RetTime = (bTimeSaver_Selected == TRUE)?FIVE_MINUTES : TEN_MINUTES;
//									  uiOperationSelected = (bTimeSaver_Selected == TRUE)? DUTY_C23 : DUTY_C24;
//										break;
										
								case 19: case 20: case 21: 
								case 22: case 23: case 24:
								case 25: case 26: case 27:case 28 :
									RetTime = IDLE; /*For Cold all phase are skiped whether timesaver selected or not*/
										break;
								default	: RetTime = uiDuration; break;
							}
							break;
					default:	RetTime = uiDuration;	
							break;
				}
				break;
		case SYNTHETIC:
				switch(uiTempSel)
				{
					case P_60:
					case Wash60:
						switch(uiPresent_Step)
							{
								case 11:RetTime = TWO_MINUTES;break;
								//case 11:RetTime = (bTimeSaver_Selected == TRUE)?THREE_MINUTES : ONE_MINUTES;break;
								case 17:case 19:RetTime = (bTimeSaver_Selected == TRUE)?ONE_MINUTES : IDLE;break;
								case 18:case 20:RetTime = (bTimeSaver_Selected == TRUE)?ONE_MINUTES : IDLE;break;
								case 22:RetTime = (bTimeSaver_Selected == TRUE)?IDLE : THREE_MINUTES;break;
								
								default	: RetTime = uiDuration; break;
							}
						
							break;
					case P_30:
					case P_40:
					case Wash30:
					case Wash40:
							switch(uiPresent_Step)
							{
								case 11:RetTime = TWO_MINUTES;break;
								case 17:RetTime = (bTimeSaver_Selected == TRUE)?TWO_MINUTES : IDLE;break;
								case 18:RetTime = (bTimeSaver_Selected == TRUE)?ONE_MINUTES : IDLE;break;
								case 19:
								case 20:RetTime = (bTimeSaver_Selected == TRUE)?IDLE : IDLE;break;
								case 22:RetTime = (bTimeSaver_Selected == TRUE)?IDLE : THREE_MINUTES;break;
								
								default	: RetTime = uiDuration; break;
							}
							break;
					case P_COLD:
					case WashCold:
							switch(uiPresent_Step)
							{
								case 11:
									if((uiLoadRange == 0))//||  || (uiLoadRange==3))
									{
										RetTime = NINETEEN_MINUTES;
									}
									else if((uiLoadRange == 1))
									{
										RetTime = TWENTY_ONE_MINUTES;	
									}
									else
									{
										RetTime = TEN_MINUTES;	
									}
									break;	
								case 17:
								case 18:
								case 19:
								case 20:RetTime = IDLE;break;//RetTime = (bTimeSaver_Selected == TRUE)?IDLE : IDLE;break;
								
								
								case 22:RetTime = (bTimeSaver_Selected == TRUE)?IDLE : THREE_MINUTES;break;
								
								default	: RetTime = uiDuration; break;
							}
						
							break;	
					
					default:RetTime = uiDuration;
						break;
				
				}
				break;
			//case EXPRESS_30:
			case SMARTPLUS:
				switch(uiSmartProgram_Selected)
				{	
					case EXPRESS_30:
						switch(uiTempSel)
						{
							case Wash40:
								switch(uiPresent_Step)
								{
									case 7: RetTime = ONE_MINUTES;break;
									case 10: //case 9:
									case 12: //case 11: 
									case 14: //case 13:
									case 16: //case 15:
										RetTime = THREE_MINUTES;break;
									default:RetTime = uiDuration;break;
								}
								break;
							case Wash60:
								switch(uiPresent_Step)
								{
									case 7: RetTime = THREE_MINUTES;//--vish
										break;
									case 10: //case 9: 
									case 12: //case 11:
									case 14: //case 13:
									case 16: //case 15:
										RetTime = FOUR_MINUTES;
										break;
									default:RetTime = uiDuration;break;
								}
								break;
							default: RetTime = uiDuration;
								break;
						}
						break;
					default: 
						break;
				}
				break;
				
		default: RetTime = uiDuration;	break;
	}
	return RetTime;
}
/***********************************************************************
* Function_Name : WashOperation_Last30secDrain(void).
* Ret_type	: Boolean.
* Working	: To execute last 30 sec drain at step time.
* Developed By	: Prashant_H
* Date		: 11/02/2022.
************************************************************************/
boolean WashOperation_Last30secDrain(void)
{
	static boolean ret_b;
	static boolean execution_b;
	
	switch(WashOperationLast30sDrain_state_ui8)
	{
		case ENTRY:/*ENTRY*/
			switch(GET_PROGRAM)
			{
				case MIXEDSOIL:
					switch(uiTempSel)
					{
						case WashCold:
						case P_COLD:
							switch(uiPresent_Step)
							{	/*These are follwoing phases who has last 30 sec drain required. */
								//case 5:
								case 22: case 27: case 30: case 33:
									if(uiCurrent_step_time == ONE_MINUTE)
									{
										if(uiStep_Seconds >= LAST_30S_DRAIN_COUNT)
										{
											WashOperationLast30sDrain_state_ui8 = DO;
											execution_b = TRUE;
											ret_b  = TRUE;
										}
									}
									else 
									{	WashOperationLast30sDrain_state_ui8 = ENTRY;
										ret_b  = FALSE;
										execution_b = FALSE;
									}
									break;
								default:
									WashOperationLast30sDrain_state_ui8 = ENTRY;
									ret_b  = FALSE;
									execution_b = FALSE;
									break;
							}
							break;
						case Wash30:
						case P_30:
						case Wash40:
						case P_40:
						case Wash60:
						case P_60:
							switch(uiPresent_Step)
							{	/*These are follwoing phases who has last 30 sec drain required. */
								//case 5:
								case 27: case 30: case 33:
									if(uiCurrent_step_time == ONE_MINUTE)
									{
										if(uiStep_Seconds >= LAST_30S_DRAIN_COUNT)
										{
											WashOperationLast30sDrain_state_ui8 = DO;
											execution_b = TRUE;
											ret_b  = TRUE;
										}
									}
									else 
									{	WashOperationLast30sDrain_state_ui8 = ENTRY;
										ret_b  = FALSE;
										execution_b = FALSE;
									}
									break;
								default:
									WashOperationLast30sDrain_state_ui8 = ENTRY;
									ret_b  = FALSE;
									execution_b = FALSE;
									break;
							}
							break;
					}
					break;
				case COTTON:
					switch(uiTempSel)
					{	/*These are follwoing phases who has last 30 sec drain required. */
						case Wash95:
						case P_95:
						case Wash60:
						case P_60:
							switch(uiPresent_Step)
							{
								case 5:case 27: case 30: case 33: case 36:
									if(uiCurrent_step_time == ONE_MINUTE)
									{
										if(uiStep_Seconds >= LAST_30S_DRAIN_COUNT)
										{
											WashOperationLast30sDrain_state_ui8 = DO;
											execution_b = TRUE;
											ret_b  = TRUE;
										}
									}
									else 
									{	WashOperationLast30sDrain_state_ui8 = ENTRY;
										ret_b  = FALSE;
										execution_b = FALSE;
									}
									break;
								default:
									WashOperationLast30sDrain_state_ui8 = ENTRY;
									ret_b  = FALSE;
									execution_b = FALSE;
									break;
							}
							break;
						case WashCold:
						case P_COLD:
						case Wash30:
						case P_30:
						case Wash40:
						case P_40:
							switch(uiPresent_Step)
							{
								case 5:case 33: case 36: case 39: case 42: /*These are the phases who has last 30 sec drain applicable*/
									if(uiCurrent_step_time == ONE_MINUTE)
									{
										if(uiStep_Seconds >= LAST_30S_DRAIN_COUNT)
										{
											WashOperationLast30sDrain_state_ui8 = DO;
											execution_b = TRUE;
											ret_b  = TRUE;
										}
									}
									else 
									{	WashOperationLast30sDrain_state_ui8 = ENTRY;
										ret_b  = FALSE;
										execution_b = FALSE;
									}
									break;
								default:
									WashOperationLast30sDrain_state_ui8 = ENTRY;
									ret_b  = FALSE;
									execution_b = FALSE;
									break;
							}
							break;
					}
					break;
				case SYNTHETIC:
					switch(uiTempSel)
					{
						case WashCold:
						case P_COLD:
						case Wash30:
						case P_30:
						case Wash40:
						case P_40:
						case Wash60:
						case P_60:
							switch(uiPresent_Step)
							{	/*These are follwoing phases who has last 30 sec drain required. */
								//case 5:
								case 21: case 24: case 27: case 30:
									if(uiCurrent_step_time == ONE_MINUTE)
									{
										if(uiStep_Seconds >= LAST_30S_DRAIN_COUNT)
										{
											WashOperationLast30sDrain_state_ui8 = DO;
											execution_b = TRUE;
											ret_b  = TRUE;
										}
									}
									else 
									{	WashOperationLast30sDrain_state_ui8 = ENTRY;
										ret_b  = FALSE;
										execution_b = FALSE;
									}
									break;
								default:
									WashOperationLast30sDrain_state_ui8 = ENTRY;
									ret_b  = FALSE;
									execution_b = FALSE;
									break;
							}
							break;
					}
					break;
				//case EXPRESS_30:
				case SMARTPLUS:
					switch(uiSmartProgram_Selected)
					{	
						case EXPRESS_30:
							switch(uiTempSel)
							{
								case Wash40:
								case Wash60:
									switch(uiPresent_Step)
									{	/*These are follwoing phases who has last 30 sec drain required. */
										//case 7:case 9: case 11: case 13: case 15:
										case 7:case 10: case 12: case 14: case 16:
											if(uiCurrent_step_time == ONE_MINUTE)
											{
												if(uiStep_Seconds >= LAST_30S_DRAIN_COUNT)
												{
													WashOperationLast30sDrain_state_ui8 = DO;
													execution_b = TRUE;
													ret_b  = TRUE;
												}
											}
											else 
											{	WashOperationLast30sDrain_state_ui8 = ENTRY;
												ret_b  = FALSE;
												execution_b = FALSE;
											}
											break;
										default:
											WashOperationLast30sDrain_state_ui8 = ENTRY;
											ret_b  = FALSE;
											break;
									}
									break;
								default:
									break;
							}
							break;
						default:
							break;
					}
					break;

				default:
					WashOperationLast30sDrain_state_ui8 = ENTRY;
					ret_b  = FALSE;
					execution_b = FALSE;
					break;
			}
			break;
		case DO:	if(execution_b)
				{
						if(uiStep_Minutes_Completed >= StepTime)  
						{
							WashOperationLast30sDrain_state_ui8 = EXIT;
							ret_b = FALSE;
						}
						else
						{	
							ret_b = TRUE;	
						}
				}
				else
				{
					WashOperationLast30sDrain_state_ui8 = ENTRY;
					ret_b = FALSE;
				}
				
				if((uiMachineState != START_PRESSED) || bPrewashValve || bMainWashValve)
				{
					WashOperationLast30sDrain_state_ui8 = LOAD_OPERATION;
				}
				
			break;
		case EXIT:/*EXIT*/
			WashOperationLast30sDrain_state_ui8 = ENTRY;
			bDrainPump = SWITCH_OFF;
			ret_b = FALSE;
			execution_b = FALSE;
			break;
		case LOAD_OPERATION:/*Load off/TimerPause*/
			if(bPrewashValve || bMainWashValve)
			{
				WashOperationLast30sDrain_state_ui8 = LOAD_OPERATION;
				bDrainPump = SWITCH_OFF;
				ret_b = FALSE;
			}
			else if(uiMachineState == START_PRESSED)
			{	
				if(uiCurrent_step_time == ONE_MINUTE)
				{
					if(uiStep_Seconds >= LAST_30S_DRAIN_COUNT)
					{
						WashOperationLast30sDrain_state_ui8 = DO;
						execution_b = TRUE;
						ret_b  = TRUE;
					}
					else
					{
						WashOperationLast30sDrain_state_ui8 = ENTRY;
						execution_b = FALSE;
						ret_b  = FALSE;
					}
					
				}
				else
				{	
					WashOperationLast30sDrain_state_ui8 = ENTRY;
					ret_b = FALSE;
				}
			}
			else
			{
				WashOperationLast30sDrain_state_ui8 = LOAD_OPERATION;
				ret_b = FALSE;
			}
			
			break;
	}
	return ret_b;
}

/***********************************************************************
* Function_Name : CirculationPump_Execution(Struct *ptr).
* Ret_type	: void.
* Working	: Execute Circulation pump
* State		: ENTRY(Time Init)---->DO(CP Execution)---->LOAD_OPERATION----->EXIT
* Developed By	: Prashant_H
* Date		: 11/03/2022.
************************************************************************/
void CirculationPump_Execution(Operation *ProgramObject)
{
//	static uint8_t cp_profile;
//	cp_profile = ProgramObject->eCircPump_Exec;
//	if(uiCurrent_step_time != RESET_VALUE)
//	{
//		switch(CP_State)
//		{
//			case ENTRY/*ENTRY*/:
//				CPTimeInSecCounter_ui32 = CP20MsTo1MsCounter_ui8 = RESET_VALUE;
//				CpOnTimer_b = CpOffTimer_b = RESET_VALUE;
//				if(uiMachineState != START_PRESSED)
//				{
//					CP_State = ENTRY;
//				}
//				else
//				{
//					switch(cp_profile)
//					{
//						case CP_ON:
//						case CP_OFF:
//							CP_State = LOAD_OPERATION;
//							break;
//						case CP10MinON_2MinOFF:
//							CPExec_objects.CPON_ui16 = TEN_MINUTES*60;
//							CPExec_objects.CPOFF_ui16 = TWO_MINUTES*60;
//							CP_State = LOAD_OPERATION;
//							break;
//						case CP5MinOFF_10MinON:
//							CPExec_objects.CPON_ui16 = TEN_MINUTES*60;
//							CPExec_objects.CPOFF_ui16 = FIVE_MINUTES*60;
//							CP_State = LOAD_OPERATION;
//							break;
//						case CP5MinOFF_5MinON:
//							CPExec_objects.CPON_ui16 = FIVE_MINUTES*60;
//							CPExec_objects.CPOFF_ui16 = FIVE_MINUTES*60;
//							CP_State = LOAD_OPERATION;
//							break;
//						case CP_1MinON_1MinOFF:
//							if(uiStep_Minutes_Completed >= FIVE_MINUTES)
//							{
//								CPExec_objects.CPON_ui16 = ONE_MINUTES*60;
//								CPExec_objects.CPOFF_ui16 = ONE_MINUTES*60;
//								CP_State = LOAD_OPERATION;
//							}
//							break;
//						case CP_5MinON:
//							if(uiStep_Minutes_Completed <= FIVE_MINUTES)
//							{
//							CPExec_objects.CPON_ui16 = FIVE_MINUTES*60;
//							CP_State = LOAD_OPERATION;
//							}
//							break;
//						
//						default:
//							CP_State = ENTRY;
//							break;
//					}
//				}
//				break;
//				
//			case DO/*DO*/:
//				if(bPrewashValve || bMainWashValve || bDrainPump || (uiMachineState != START_PRESSED)) 
//				{
//					CP_State = LOAD_OPERATION;
//				}
//				else
//				{
//					switch(cp_profile)
//					{
//						case CP_ON:
//							bCirculation_Pump = SWITCH_ON;
//							break;
//						case CP_OFF:
//							bCirculation_Pump = SWITCH_OFF;
//							break;
//						case CP_5MinON:
//							if(CpOnTimer_b)
//							{
//								bCirculation_Pump = SWITCH_ON;
//								if( CPTimeInSecCounter_ui32 >= CPExec_objects.CPON_ui16)
//								{
//									CpOnTimer_b = FALSE;
//									CPTimeInSecCounter_ui32 = 0;
//									bCirculation_Pump = SWITCH_OFF;
//								}
//							}
//							break;
//						case CP10MinON_2MinOFF:	
//						case CP5MinOFF_10MinON:
//						case CP5MinOFF_5MinON:
//						case CP_1MinON_1MinOFF:
//							if(CpOnTimer_b)
//							{
//								bCirculation_Pump = SWITCH_ON;
//								if( CPTimeInSecCounter_ui32 >= CPExec_objects.CPON_ui16)
//								{
//									CpOnTimer_b = FALSE;
//									CPTimeInSecCounter_ui32 = 0;
//									CpOffTimer_b = TRUE;
//								}
//							}
//							else if(CpOffTimer_b)
//							{
//								bCirculation_Pump = SWITCH_OFF;
//								if(CPTimeInSecCounter_ui32 >= CPExec_objects.CPOFF_ui16)
//								{
//									CpOnTimer_b = TRUE;
//									CPTimeInSecCounter_ui32 = 0;
//									CpOffTimer_b = FALSE;	
//								}
//							}
//							
//							break;
//						default: 
//							break;
//					}
//				}
//				break;
//				
//			case EXIT/*EXIT*/:
//				CPTimeInSecCounter_ui32 = CP20MsTo1MsCounter_ui8 = RESET_VALUE;
//				CpOnTimer_b = CpOffTimer_b = RESET_VALUE;
//				bCirculation_Pump = SWITCH_OFF;
//				CP_State = ENTRY;
//				break;			
//				
//			case LOAD_OPERATION/*CP off when loads on or Machinestate not start_pressed */:
//				if(bPrewashValve || bMainWashValve || bDrainPump || (uiMachineState != START_PRESSED)) 
//				{
//					CP_State = LOAD_OPERATION;
//					CpOffTimer_b = FALSE;
//					CpOnTimer_b = FALSE;
//					bCirculation_Pump = SWITCH_OFF;
//				}
//				else
//				{
//					if(uiNextStep_Start_Delay == COMPLETED)	//TODO:  This is added to avoid malfunction of CP profile .
//					{
//						switch(cp_profile)
//						{
//							case CP_ON:
//							case CP_OFF:
//								CP_State = DO;
//								break;
//							case CP10MinON_2MinOFF:	CpOnTimer_b = TRUE;	CpOffTimer_b = FALSE; CP_State = DO;
//								break;
//							case CP5MinOFF_10MinON:	CpOffTimer_b = TRUE;	CpOnTimer_b = FALSE; CP_State = DO;
//								break;
//							case CP5MinOFF_5MinON:	CpOffTimer_b = TRUE;	CpOnTimer_b = FALSE; CP_State = DO;
//								break;
//							case CP_1MinON_1MinOFF:	if(uiStep_Minutes_Completed >= FIVE_MINUTES)
//										{	CpOffTimer_b = FALSE;	CpOnTimer_b = TRUE; CP_State = DO;}
//								break;
//							case CP_5MinON:
//								if(uiStep_Minutes_Completed <= FIVE_MINUTES)
//								{
//									CpOffTimer_b = FALSE;	CpOnTimer_b = TRUE; CP_State = DO;
//								}
//								else
//								{
//									CpOffTimer_b = FALSE;	CpOnTimer_b = FALSE; CP_State = DO;
//								}
//								break;
//							default:
//								CP_State = ENTRY;
//								break;
//						}
//					}
//					
//				}
//				break;
//			default:
//				CP_State = ENTRY;
//				break;
//		}
//	}
//	else
//	{
//		CPTimeInSecCounter_ui32 = CP20MsTo1MsCounter_ui8 = RESET_VALUE;
//		CpOnTimer_b = CpOffTimer_b = RESET_VALUE;
//		bCirculation_Pump = SWITCH_OFF;
//		CP_State = 0;	
//	}
}
/***********************************************************************
* Function_Name : WashOperations_HeatTime(uint8_t step_time).
* Ret_type	: uint8_t.
* Working	: Functional working depend on uiPresent of program. It returns the step time.
* Developed By	: Prashant_H
* Date		: 11/03/2022
************************************************************************/
uint8_t WashOperations_HeatTime(uint8_t step_time)
{
	uint8_t RetTime;
	switch(uiSelected_Position)
	{
		//case EXPRESS_30:
		case SMARTPLUS:
				switch(uiSmartProgram_Selected)
				{	
					case EXPRESS_30:
						switch(uiTempSel)
						{
							case Wash40:
								switch(uiPresent_Step)
								{ 	
									case 6: RetTime = IDLE;break;
									case 3: RetTime = ONE_MINUTES;break;
									case 4: RetTime = THREE_MINUTES;
										break;
									default: RetTime = step_time; 
										break;
								}
								break;
							case Wash60:
								
								switch(uiPresent_Step)
								{
									case 3: RetTime = TEN_MINUTES;
										break; 
									case 6: RetTime = TEN_MINUTES;
										break;
									default: RetTime = step_time; break;
								}
								break;
							default:break;
						}
						break;
						
					default:
					break;
				}
				break;
				
				
		default:
			break;
	}
	return RetTime;
}


void SetWashOperationLast30sDrain_state_ui8(uint8_t state)
{
	WashOperationLast30sDrain_state_ui8 = state;
	
}

/******************************************************************************************************************************************************/	
							
void Time_Update_Basedon_Set_Reset_Count(void){
	
	if((uiSelected_Position == COTTON) || (uiSelected_Position == MIXEDSOIL) || (uiSelected_Position == SYNTHETIC) || (uiSelected_Position == SMARTPLUS))
	{

		if((uidisp_select_option & Prewash_Yes) == Prewash_Yes)
			{
								if(((uiSelected_Position == COTTON || uiAISelected_Position == COTTON ) && (uiPresent_Step == 5))/*||
								    ((uiSelected_Position == MIXEDSOIL || uiAISelected_Position == MIXEDSOIL) && (uiPresent_Step == 5)) ||
								    ((uiSelected_Position == SYNTHETIC || uiAISelected_Position == SYNTHETIC) && (uiPresent_Step == 5))*/ ||
								    ((uiSelected_Position == SMARTPLUS) && (uiPresent_Step == 3)))
								 {
							
								if((uiPrewashWaterResetCount <= 1)&&(bTimeSaver_Selected == FALSE)&&(uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == UNIFORM))
								{
									BalanceTime = BalanceTime - 15; 
									uicycle_mincompleted = uicycle_mincompleted + 15;
								}
								else if((uiSelected_Position == COTTON)&&(bTimeSaver_Selected == FALSE)) 
								{
									if((uiPrewashWaterResetCount <= 1)&&((uiTempSel == Wash40E) || (uiTempSel == Wash60E)))
									{
										if(uiTempSel == Wash40E)
										{
											BalanceTime = BalanceTime - 40; 
											uicycle_mincompleted = uicycle_mincompleted + 40;
										}
										else if(uiTempSel == Wash60E)
										{
											BalanceTime = BalanceTime - 27; 
											uicycle_mincompleted = uicycle_mincompleted + 27;
										}
									}
								}
								else if((bTimeSaver_Selected == FALSE)&&(uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == JEANS))
								{
									if(uiPrewashWaterResetCount <= 2)
									{
										BalanceTime = BalanceTime - 20; 
										uicycle_mincompleted = uicycle_mincompleted + 20;
									}
									else if((uiPrewashWaterResetCount > 2)&&(uiPrewashWaterResetCount <= 5))
									{
										BalanceTime = BalanceTime - 10; 
										uicycle_mincompleted = uicycle_mincompleted + 10;
									}
								}
							}}
							else
							{   
								if(((uiSelected_Position == COTTON && (uiTempSel == Wash40E || uiTempSel == Wash60E)) && (uiPresent_Step == 3))/* ||
								    ((uiSelected_Position == MIXEDSOIL || uiAISelected_Position == MIXEDSOIL) && (uiPresent_Step == 11)) */||
								    ((uiSelected_Position == SMARTPLUS) && ((uiSmartProgram_Selected == JEANS && uiPresent_Step == 6) ||
								    (uiSmartProgram_Selected == UNIFORM && uiPresent_Step == 5))))/*smart program uniform cold doesnt have tumbling in mainwash after filling so TX time we are updating in filling step for smart program,*/
								 {  
			
								
								
								if((uiWaterResetCount <= 1)&&(bTimeSaver_Selected == FALSE)&&(uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == UNIFORM))
								{
									BalanceTime = BalanceTime - 15; 
									uicycle_mincompleted = uicycle_mincompleted + 15;
								}
								
								else if((uiSelected_Position == COTTON)&&(bTimeSaver_Selected == FALSE)) 
								{
									if((uiWaterResetCount <= 1)&&((uiTempSel == Wash40E) || (uiTempSel == Wash60E)))
									{
										if(uiTempSel == Wash40E)
										{
											BalanceTime = BalanceTime - 40; 
											uicycle_mincompleted = uicycle_mincompleted + 40;
										}
										else if(uiTempSel == Wash60E)
										{
											BalanceTime = BalanceTime - 27; 
											uicycle_mincompleted = uicycle_mincompleted + 27;
										}
									}
								}
								else if((bTimeSaver_Selected == FALSE)&&(uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == JEANS))
								{
									if(uiWaterResetCount <= 2)
									{
										BalanceTime = BalanceTime - 20; 
										uicycle_mincompleted = uicycle_mincompleted + 20;
									}
									else if((uiWaterResetCount > 2)&&(uiWaterResetCount <= 5))
									{
										BalanceTime = BalanceTime - 10; 
										uicycle_mincompleted = uicycle_mincompleted + 10;
									}
								}
							}}
	}

}
/********************************************************************Load Sensing Management******************************************************************************/

void After_LS_Update_Para(void)
{
	//to deduct rinse1 time from default time in synthetic and cotton based on load
	if((uiLoadRange<=1) && (((GET_PROGRAM) == SYNTHETIC)||((GET_PROGRAM) == COTTON)||((GET_PROGRAM) == MIXEDSOIL)))
	{
		if((GET_PROGRAM) == SYNTHETIC)
		{
			BalanceTime = BalanceTime - SYNTH_RINSE1_TIME;
			uicycle_mincompleted = uicycle_mincompleted + SYNTH_RINSE1_TIME;
			uiLS_Total_subtraction = uiLS_Total_subtraction + SYNTH_RINSE1_TIME;
			
			//additional subtraction of rinse2 and additive rinse filling time difference for 1,2,3kg load...
			//rinse2 filling time difference is 1mins; additive rinse filling time difference is 2 mins
			if((uiLoadRange==0)&&((uiTempSel==WashCold)||(uiTempSel==Wash60)||(uiTempSel==P_60)||(uiTempSel==P_COLD)))
			{
				BalanceTime = BalanceTime - LS_SYNTH_RINSE_FILL_DIFFERENCE;
				uicycle_mincompleted = uicycle_mincompleted + LS_SYNTH_RINSE_FILL_DIFFERENCE;
				uiLS_Total_subtraction = uiLS_Total_subtraction + LS_SYNTH_RINSE_FILL_DIFFERENCE;
			}
			if((uiLoadRange==1)&&((uiTempSel==WashCold) || (uiTempSel==P_COLD) ))
			{
				BalanceTime = BalanceTime + 11;
				uicycle_mincompleted = uicycle_mincompleted - 11;
				uiLS_Total_subtraction = uiLS_Total_subtraction - 11;
			}
			else if((uiLoadRange==0)&&((uiTempSel==WashCold) || (uiTempSel==P_COLD) ))
			{
				BalanceTime = BalanceTime + 9;
				uicycle_mincompleted = uicycle_mincompleted - 9;
				uiLS_Total_subtraction = uiLS_Total_subtraction - 9;
			}
		}
		else if((GET_PROGRAM) == COTTON)
		{
			BalanceTime = BalanceTime - COTTON_RINSE1_TIME;
			uicycle_mincompleted = uicycle_mincompleted + COTTON_RINSE1_TIME;
			uiLS_Total_subtraction = uiLS_Total_subtraction + COTTON_RINSE1_TIME;
			
			//additional subtraction of rinse2 and additive rinse filling time difference for 1,2,3kg load...
			//rinse2 filling time difference is 3mins; additive rinse filling time difference is 2mins
			//if((uiLoadRange==0)&&((uiTempSel!=Wash30) || (uiTempSel!=P_30)))
			if((uiLoadRange==0)&&((uiTempSel!=Wash30) && (uiTempSel!=P_30)))
			{
				BalanceTime = BalanceTime - LS_COTTON_RINSE_FILL_DIFFERENCE;
				uicycle_mincompleted = uicycle_mincompleted + LS_COTTON_RINSE_FILL_DIFFERENCE;
				uiLS_Total_subtraction = uiLS_Total_subtraction + LS_COTTON_RINSE_FILL_DIFFERENCE;
				
				if((bTimeSaver_Selected == TRUE)&&(uiTempSel==Wash40 || uiTempSel==P_40)) //to deduct the time of tumbling+reheat steps which are to be skippd for this condition
				{
					BalanceTime = BalanceTime - 10;
					uicycle_mincompleted = uicycle_mincompleted + 10;
					uiLS_Total_subtraction = uiLS_Total_subtraction + 10;
				}
			}
			if((uiLoadRange==0)&&(uiTempSel == WashCold || uiTempSel == P_COLD))
			{
				BalanceTime = BalanceTime + 7;
				uicycle_mincompleted = uicycle_mincompleted - 7;
				uiLS_Total_subtraction = uiLS_Total_subtraction - 7;	
			}
			else if((uiLoadRange==1)&&(uiTempSel == WashCold || uiTempSel == P_COLD))
			{
				BalanceTime = BalanceTime + 6;
				uicycle_mincompleted = uicycle_mincompleted - 6;
				uiLS_Total_subtraction = uiLS_Total_subtraction - 6;	
			}
			//To subtract heat time in cotton 95 program only --as per MP specs
			if(((uiTempSel == Wash95) || (uiTempSel == P_95)) )
			{
				if(uiLoadRange==0)
				{
					BalanceTime = BalanceTime - 30;
					uicycle_mincompleted = uicycle_mincompleted + 30;
					uiLS_Total_subtraction = uiLS_Total_subtraction + 30;	
				}
				else
				{
					BalanceTime = BalanceTime - 20;
					uicycle_mincompleted = uicycle_mincompleted + 20;
					uiLS_Total_subtraction = uiLS_Total_subtraction + 20;	
				}
			}
			
		}
		else if(((GET_PROGRAM) == MIXEDSOIL)&&(uiTempSel == Wash60 || uiTempSel == P_60))
		{
//			if(uiFinalWt<=3)
//			{
//				BalanceTime = BalanceTime - LS_MIX_RINSE_FILL_DIFFERENCE;
//				uicycle_mincompleted = uicycle_mincompleted + LS_MIX_RINSE_FILL_DIFFERENCE;
//				uiLS_Total_subtraction = uiLS_Total_subtraction + LS_MIX_RINSE_FILL_DIFFERENCE;
//			}
			if(uiLoadRange==0)
			{
				BalanceTime = BalanceTime - 4;
				uicycle_mincompleted = uicycle_mincompleted + 4;
				uiLS_Total_subtraction = uiLS_Total_subtraction + 4;
			}
			if((uiLoadRange==1)&&(bTimeSaver_Selected == TRUE))
			{
				BalanceTime = BalanceTime - 2;
				uicycle_mincompleted = uicycle_mincompleted + 2;
				uiLS_Total_subtraction = uiLS_Total_subtraction + 2;
			}

		}
		
		
	}

			
	
	//to deduct anticrease time from cotton, mix and synthetic based on load
	if((uiLoadRange==0) && (((GET_PROGRAM) == SYNTHETIC)||((GET_PROGRAM) == COTTON)||((GET_PROGRAM) == MIXEDSOIL)))
	{
		if((bTimeSaver_Selected == FALSE)&&(uiRpmSel != Rpm000))
		{ 
//			if((GET_PROGRAM) == COTTON)
//			{
//				BalanceTime = BalanceTime - COTTON_ANTICREASE_TIME;
//				uicycle_mincompleted = uicycle_mincompleted + COTTON_ANTICREASE_TIME;
//				uiLS_Total_subtraction = uiLS_Total_subtraction + COTTON_ANTICREASE_TIME;
//			}
//			else if(((GET_PROGRAM) == MIXEDSOIL) || ((GET_PROGRAM) == SYNTHETIC))
//			{
//				BalanceTime = BalanceTime - SYNTHETIC_ANTICREASE_TIME; //synthetic and mix have same anticrease time
//				uicycle_mincompleted = uicycle_mincompleted + SYNTHETIC_ANTICREASE_TIME;
//				uiLS_Total_subtraction = uiLS_Total_subtraction + SYNTHETIC_ANTICREASE_TIME;
//			}
			BalanceTime = BalanceTime - ANTICREASE_TIME;
			uicycle_mincompleted = uicycle_mincompleted + ANTICREASE_TIME;
			uiLS_Total_subtraction = uiLS_Total_subtraction + ANTICREASE_TIME;
		}
	}
	
	
   	if(bTimeSaver_Selected == FALSE)
	{
		switch(GET_PROGRAM)
		{
			case SYNTHETIC:/*MaxTX_Time - ResetCountTX_Time*/
//					uiLS_Tx_subtraction = ((uint8_t)(SYNTHETIC_TumbleTXResetTime_ui8[MAX_LOADSENSE_COUNT_SYNTH_MIX][uiTempSel]) - 
//							(uint8_t)(SYNTHETIC_TumbleTXResetTime_ui8[uiLoadRange][uiTempSel]));
					uiLS_Tx_subtraction = ((uint8_t)(SYNTHETIC_TumbleTXResetTime_ui8[MAX_LOADSENSE_COUNT_SYNTH_MIX][uiheat_temp]) - 
							(uint8_t)(SYNTHETIC_TumbleTXResetTime_ui8[uiLoadRange][uiheat_temp]));
					BalanceTime = BalanceTime - uiLS_Tx_subtraction;
					uicycle_mincompleted = uicycle_mincompleted + uiLS_Tx_subtraction;
					uiLS_Total_subtraction = uiLS_Total_subtraction + uiLS_Tx_subtraction;
					break;
					
			case MIXEDSOIL:/*MaxTX_Time - ResetCountTX_Time*/
//					uiLS_Tx_subtraction = ((uint8_t)(MIXED_TumbleTXResetTime_ui8[MAX_LOADSENSE_COUNT_SYNTH_MIX][uiTempSel]) - 
//							(uint8_t)(MIXED_TumbleTXResetTime_ui8[uiLoadRange][uiTempSel]));
					uiLS_Tx_subtraction = ((uint8_t)(MIXED_TumbleTXResetTime_ui8[MAX_LOADSENSE_COUNT_SYNTH_MIX][uiheat_temp]) - 
							(uint8_t)(MIXED_TumbleTXResetTime_ui8[uiLoadRange][uiheat_temp]));
					BalanceTime = BalanceTime - uiLS_Tx_subtraction;
					uicycle_mincompleted = uicycle_mincompleted + uiLS_Tx_subtraction;
					uiLS_Total_subtraction = uiLS_Total_subtraction + uiLS_Tx_subtraction;
					break;
					
			case COTTON:	/*MaxTX_Time - ResetCountTX_Time*/ //Exclude 40E & 60E Temp 
					if(uiTempSel != Wash40E && uiTempSel != Wash60E)
					{
//						uiLS_Tx_subtraction = ((uint8_t)(COTTON_TumbleTXResetTime_ui8[MAX_LOADSENSE_COUNT_COTTON][uiTempSel]) - 
//						     		(uint8_t)(COTTON_TumbleTXResetTime_ui8[uiLoadRange][uiTempSel]));
//						BalanceTime = BalanceTime - uiLS_Tx_subtraction;
						uiLS_Tx_subtraction = ((uint8_t)(COTTON_TumbleTXResetTime_ui8[MAX_LOADSENSE_COUNT_COTTON][uiheat_temp]) - 
						     		(uint8_t)(COTTON_TumbleTXResetTime_ui8[uiLoadRange][uiheat_temp]));
						BalanceTime = BalanceTime - uiLS_Tx_subtraction;
						uicycle_mincompleted = uicycle_mincompleted +uiLS_Tx_subtraction;
						uiLS_Total_subtraction = uiLS_Total_subtraction + uiLS_Tx_subtraction;
					}
					break;
					
			default: break;
		}
	}
	else  //added timesaver dependent time subtraction because previously there was no load dependency for Tx step with Timesaver selected
	{
		switch(GET_PROGRAM)
		{
			case SYNTHETIC:/*MaxTX_Time - ResetCountTX_Time*/
//					uiLS_Tx_subtraction = ((uint8_t)(SYNTHETIC_TimeSaverTumbleTXTime_ui8[MAX_LOADSENSE_COUNT_SYNTH_MIX][uiTempSel]) - 
//							(uint8_t)(SYNTHETIC_TimeSaverTumbleTXTime_ui8[uiLoadRange][uiTempSel]));
					uiLS_Tx_subtraction = ((uint8_t)(SYNTHETIC_TimeSaverTumbleTXTime_ui8[MAX_LOADSENSE_COUNT_SYNTH_MIX][uiheat_temp]) - 
							(uint8_t)(SYNTHETIC_TimeSaverTumbleTXTime_ui8[uiLoadRange][uiheat_temp]));
					BalanceTime = BalanceTime - uiLS_Tx_subtraction;
					uicycle_mincompleted = uicycle_mincompleted + uiLS_Tx_subtraction;
					uiLS_Total_subtraction = uiLS_Total_subtraction + uiLS_Tx_subtraction;
					break;
					
			case MIXEDSOIL:/*MaxTX_Time - ResetCountTX_Time*/
//					uiLS_Tx_subtraction = ((uint8_t)(MIXED_TimeSaverTumbleTXTime_ui8[MAX_LOADSENSE_COUNT_SYNTH_MIX][uiTempSel]) - 
//							(uint8_t)(MIXED_TimeSaverTumbleTXTime_ui8[uiLoadRange][uiTempSel]));
					uiLS_Tx_subtraction = ((uint8_t)(MIXED_TimeSaverTumbleTXTime_ui8[MAX_LOADSENSE_COUNT_SYNTH_MIX][uiheat_temp]) - 
							(uint8_t)(MIXED_TimeSaverTumbleTXTime_ui8[uiLoadRange][uiheat_temp]));
					BalanceTime = BalanceTime - uiLS_Tx_subtraction;
					uicycle_mincompleted = uicycle_mincompleted + uiLS_Tx_subtraction;
					uiLS_Total_subtraction = uiLS_Total_subtraction + uiLS_Tx_subtraction;
					
					break;
			case COTTON:	/*MaxTX_Time - ResetCountTX_Time*/ //Exclude 40E & 60E Temp 
					if((uiTempSel != Wash40E) && (uiTempSel != Wash60E))
					{
//						uiLS_Tx_subtraction = ((uint8_t)(COTTON_TimeSaverTumbleTXTime_ui8[MAX_LOADSENSE_COUNT_COTTON][uiTempSel]) - 
//						     		(uint8_t)(COTTON_TimeSaverTumbleTXTime_ui8[uiLoadRange][uiTempSel]));
						uiLS_Tx_subtraction = ((uint8_t)(COTTON_TimeSaverTumbleTXTime_ui8[MAX_LOADSENSE_COUNT_COTTON][uiheat_temp]) - 
						     		(uint8_t)(COTTON_TimeSaverTumbleTXTime_ui8[uiLoadRange][uiheat_temp]));
						BalanceTime = BalanceTime - uiLS_Tx_subtraction;
						uicycle_mincompleted = uicycle_mincompleted +uiLS_Tx_subtraction;
						uiLS_Total_subtraction = uiLS_Total_subtraction + uiLS_Tx_subtraction;
					}
					break;
			default: break;
		}
	}
}

//void After_LS_Update_Para(void)
//{
//
////	if((uiLoadRange < 1)&& ((GET_PROGRAM) == SYNTHETIC))
////	{	
////		BalanceTime = BalanceTime - SYNTH_RINSE2_TIME; /*Rinse 2 total time is 13min so deduct the 13 time from Main time */
////		uicycle_mincompleted = uicycle_mincompleted + SYNTH_RINSE2_TIME;
////	}
//
//	//to deduct rinse1 time from default time in synthetic and cotton based on load
//	//if((uiFinalWt<=7) && (((GET_PROGRAM) == SYNTHETIC)||((GET_PROGRAM) == COTTON)||((GET_PROGRAM) == MIXEDSOIL)))
//	if((uiLoadRange<=2) && (((GET_PROGRAM) == SYNTHETIC)||((GET_PROGRAM) == COTTON)||((GET_PROGRAM) == MIXEDSOIL)))
//	{
//		if((GET_PROGRAM) == SYNTHETIC)
//		{
//			BalanceTime = BalanceTime - SYNTH_RINSE1_TIME;
//			uicycle_mincompleted = uicycle_mincompleted + SYNTH_RINSE1_TIME;
//			uiLS_Total_subtraction = uiLS_Total_subtraction + SYNTH_RINSE1_TIME;
//			
//			//additional subtraction of rinse2 and additive rinse filling time difference for 1,2kg load...
//			//rinse2 filling time difference is 2mins; additive rinse filling time difference is 2 mins
//			if(uiLoadRange==0)
//			{
//				BalanceTime = BalanceTime - LS_SYNTH_RINSE_FILL_DIFFERENCE;
//				uicycle_mincompleted = uicycle_mincompleted + LS_SYNTH_RINSE_FILL_DIFFERENCE;
//				uiLS_Total_subtraction = uiLS_Total_subtraction + LS_SYNTH_RINSE_FILL_DIFFERENCE;
//			}
//		}
//		else if((GET_PROGRAM) == COTTON)
//		{
//			BalanceTime = BalanceTime - COTTON_RINSE1_TIME;
//			uicycle_mincompleted = uicycle_mincompleted + COTTON_RINSE1_TIME;
//			uiLS_Total_subtraction = uiLS_Total_subtraction + COTTON_RINSE1_TIME;
//			
//			//additional subtraction of rinse2 and additive rinse filling time difference for 1,2,3kg load...
//			//rinse2 filling time difference is 2mins; additive rinse filling time difference is 3mins
//			if(uiLoadRange==0)
//			{
//				BalanceTime = BalanceTime - LS_COTTON_RINSE_FILL_DIFFERENCE;
//				uicycle_mincompleted = uicycle_mincompleted + LS_COTTON_RINSE_FILL_DIFFERENCE;
//				uiLS_Total_subtraction = uiLS_Total_subtraction + LS_COTTON_RINSE_FILL_DIFFERENCE;
//			}
//			if((bTimeSaver_Selected == TRUE)&&(uiLoadRange<=1)&&((uiTempSel==Wash30)||(uiTempSel==Wash40))) //to deduct the time of tumbling+reheat steps which are to be skippd for this condition
//			{
//				BalanceTime = BalanceTime - 6;
//				uicycle_mincompleted = uicycle_mincompleted + 6;
//				uiLS_Total_subtraction = uiLS_Total_subtraction + 6;
//			}
//			if((uiTempSel==WashCold)&&(uiLoadRange==2)) //added as per new change in step times
//			{
//				BalanceTime = BalanceTime + 11;
//				uicycle_mincompleted = uicycle_mincompleted - 11;
//				uiLS_Total_subtraction = uiLS_Total_subtraction - 11;
//			}
//			if((uiTempSel==Wash95)&&(uiLoadRange==1)&&(bTimeSaver_Selected == FALSE))
//			{
//				BalanceTime = BalanceTime - 1;
//				uicycle_mincompleted = uicycle_mincompleted + 1;
//				uiLS_Total_subtraction = uiLS_Total_subtraction + 1;
//			}
//		}
//		else if((GET_PROGRAM) == MIXEDSOIL)
//		{
//			//if((uiFinalWt<=3) && (bTimeSaver_Selected == FALSE))
//			if(uiLoadRange<=1)
//			{
//				if((bTimeSaver_Selected == FALSE)||((bTimeSaver_Selected == TRUE)&&((uiTempSel==WashCold)||(uiTempSel==Wash30))))
//				{
//					BalanceTime = BalanceTime - LS_MIX_RINSE_FILL_DIFFERENCE;
//					uicycle_mincompleted = uicycle_mincompleted + LS_MIX_RINSE_FILL_DIFFERENCE;
//					uiLS_Total_subtraction = uiLS_Total_subtraction + LS_MIX_RINSE_FILL_DIFFERENCE;
//				}
//			}
//		}	
//		
//	}
//	
//	//to deduct anticrease time from cotton, mix and synthetic based on load
//	//if((uiFinalWt<=3) && (((GET_PROGRAM) == SYNTHETIC)||((GET_PROGRAM) == COTTON)||((GET_PROGRAM) == MIXEDSOIL)))
//	if(((((GET_PROGRAM) == SYNTHETIC)||((GET_PROGRAM) == COTTON))&&(uiLoadRange==0))||(((GET_PROGRAM) == MIXEDSOIL)&&(uiLoadRange<=1)))
//	{
//		if((bTimeSaver_Selected == FALSE)&&(uiRpmSel != Rpm000))
//		{
//			BalanceTime = BalanceTime - ANTICREASE_TIME;//LS_ANTICREASE_TIME;
//			uicycle_mincompleted = uicycle_mincompleted + ANTICREASE_TIME;//LS_ANTICREASE_TIME;
//			uiLS_Total_subtraction = uiLS_Total_subtraction + ANTICREASE_TIME;//LS_ANTICREASE_TIME;
//		}
//	}
//	
//	
//   	if(bTimeSaver_Selected == FALSE)
//	{
//		switch(GET_PROGRAM)
//		{
//			case SYNTHETIC:/*MaxTX_Time - ResetCountTX_Time*/
//					uiLS_Tx_subtraction = ((uint8_t)(SYNTHETIC_TumbleTXResetTime_ui8[MAX_LOADSENSE_COUNT][uiTempSel]) - 
//							(uint8_t)(SYNTHETIC_TumbleTXResetTime_ui8[uiLoadRange][uiTempSel]));
//					BalanceTime = BalanceTime - uiLS_Tx_subtraction;
//					uicycle_mincompleted = uicycle_mincompleted + uiLS_Tx_subtraction;
//					uiLS_Total_subtraction = uiLS_Total_subtraction + uiLS_Tx_subtraction;
//					break;
//					
//			case MIXEDSOIL:/*MaxTX_Time - ResetCountTX_Time*/
//					uiLS_Tx_subtraction = ((uint8_t)(MIXED_TumbleTXResetTime_ui8[MAX_LOADSENSE_COUNT][uiTempSel]) - 
//							(uint8_t)(MIXED_TumbleTXResetTime_ui8[uiLoadRange][uiTempSel]));
//					BalanceTime = BalanceTime - uiLS_Tx_subtraction;
//					uicycle_mincompleted = uicycle_mincompleted + uiLS_Tx_subtraction;
//					uiLS_Total_subtraction = uiLS_Total_subtraction + uiLS_Tx_subtraction;
//					break;
//					
//			case COTTON:	/*MaxTX_Time - ResetCountTX_Time*/ //Exclude 40E & 60E Temp 
//					if(uiTempSel <= Wash95)
//					{
//						uiLS_Tx_subtraction = ((uint8_t)(COTTON_TumbleTXResetTime_ui8[MAX_LOADSENSE_COUNT][uiTempSel]) - 
//						     		(uint8_t)(COTTON_TumbleTXResetTime_ui8[uiLoadRange][uiTempSel]));
//						BalanceTime = BalanceTime - uiLS_Tx_subtraction;
//						uicycle_mincompleted = uicycle_mincompleted +uiLS_Tx_subtraction;
//						uiLS_Total_subtraction = uiLS_Total_subtraction + uiLS_Tx_subtraction;
//					}
//					break;
//					
//			default: break;
//		}
//	}
//	else  //added timesaver dependent time subtraction because previously there was no load dependency for Tx step with Timesaver selected
//	{
//		switch(GET_PROGRAM)
//		{
//			case SYNTHETIC:/*MaxTX_Time - ResetCountTX_Time*/
//					uiLS_Tx_subtraction = ((uint8_t)(SYNTHETIC_TimeSaverTumbleTXTime_ui8[MAX_LOADSENSE_COUNT][uiTempSel]) - 
//							(uint8_t)(SYNTHETIC_TimeSaverTumbleTXTime_ui8[uiLoadRange][uiTempSel]));
//					BalanceTime = BalanceTime - uiLS_Tx_subtraction;
//					uicycle_mincompleted = uicycle_mincompleted + uiLS_Tx_subtraction;
//					uiLS_Total_subtraction = uiLS_Total_subtraction + uiLS_Tx_subtraction;
//					break;
//					
//			case MIXEDSOIL:/*MaxTX_Time - ResetCountTX_Time*/
//					uiLS_Tx_subtraction = ((uint8_t)(MIXED_TimeSaverTumbleTXTime_ui8[MAX_LOADSENSE_COUNT][uiTempSel]) - 
//							(uint8_t)(MIXED_TimeSaverTumbleTXTime_ui8[uiLoadRange][uiTempSel]));
//					BalanceTime = BalanceTime - uiLS_Tx_subtraction;
//					uicycle_mincompleted = uicycle_mincompleted + uiLS_Tx_subtraction;
//					uiLS_Total_subtraction = uiLS_Total_subtraction + uiLS_Tx_subtraction;
//					
//					//mix heat time dependency on load removed
////					uiMix_Heat_subtraction = ((uint8_t)(MIXED_HeatTempTime_ui8[MAX_LOADSENSE_COUNT][uiTempSel]) - 
////							(uint8_t)(MIXED_HeatTempTime_ui8[uiLoadRange][uiTempSel]));
////					BalanceTime = BalanceTime - uiMix_Heat_subtraction;						     
////					uicycle_mincompleted = uicycle_mincompleted + uiMix_Heat_subtraction;
////					
////					uiLS_subtraction = uiLS_subtraction + uiMix_Heat_subtraction;
//					
//					break;
//			case COTTON:	/*MaxTX_Time - ResetCountTX_Time*/ //Exclude 40E & 60E Temp 
//					if(uiTempSel <= Wash95)
//					{
//						uiLS_Tx_subtraction = ((uint8_t)(COTTON_TimeSaverTumbleTXTime_ui8[MAX_LOADSENSE_COUNT][uiTempSel]) - 
//						     		(uint8_t)(COTTON_TimeSaverTumbleTXTime_ui8[uiLoadRange][uiTempSel]));
//						BalanceTime = BalanceTime - uiLS_Tx_subtraction;
//						uicycle_mincompleted = uicycle_mincompleted +uiLS_Tx_subtraction;
//						uiLS_Total_subtraction = uiLS_Total_subtraction + uiLS_Tx_subtraction;
//					}
//					break;
//			default: break;
//		}
//	}
//}

/*void Load_Sensing_Calculation(void)
{
  uint16_t RPMVAL;
  
  RPMVAL = (uiLS_RPMValue & 0x000f) + (uiLS_RPMValue & 0x00f0) + (uiLS_RPMValue & 0x0f00) + (uiLS_RPMValue & 0xf000);
   
  if(RPMVAL > 4000)
  {
	RPMVAL = 3500;  
  }
  else if(RPMVAL < 1500)
   {
	RPMVAL = 1000;   
   }
  
  uiN_RPMValue =  (RPMVAL - 1000) / 2500;
  
  uiFinalWt = (10.94 - (29.5 * uiN_RPMValue )+(20 * uiN_RPMValue * uiN_RPMValue));
   
    if(uiFinalWt < 50)
     {uiLoadRange = 0;}
    else if(uiFinalWt > 50 && uiFinalWt < 60)
     {uiLoadRange = 1;}
    else if(uiFinalWt > 60)
     {uiLoadRange = 2;}

 //    return (uiLoadRange);
}*/

void Load_Sensing_Calculation(void)
{
	uint16_t RPMVAL;
	float M_coeff, C_intercept, fN_RPMval;
  
	RPMVAL = (uiLS_RPMValue & 0x000f) + (uiLS_RPMValue & 0x00f0) + (uiLS_RPMValue & 0x0f00) + (uiLS_RPMValue & 0xf000);
	
	//new calculation
	//limiting boundaries
	if(uiSW_VF_MD == 1) //For IFB motor div bottom board software with IFB 35mm motor
	{
		if(RPMVAL > 4770)
	  	{
			RPMVAL = 4770;
	  	}
		else if(RPMVAL < 2654)
		{
			RPMVAL = 2654;
		}
		else
		{
			RPMVAL = RPMVAL;
		}
		
		RPMtest = RPMVAL;
		
		//RPM normalization
		//uiN_RPMValue = (float)((RPMVAL - 669) / 2796);
		//fN_RPMval = (float)(((float)(RPMVAL) - (float)(669)) / (float)(2796));
		fN_RPMval = (float)(((float)(RPMVAL) - (float)(2654)) / (float)(2116));
		
		//final weight calculation
		//M_coeff = (-12.40173431);
		//C_intercept = 10.054720006564107;
		M_coeff = (-12.678);//(-8.92);//(-7.84885999);//(-9.381295244);
		C_intercept = 10.543;//8.67;//8.65179154717476;//8.912953920969958;
		
		Y_weight = (float)((M_coeff * fN_RPMval) + C_intercept);
		
		
		if(Y_weight <= 0)
		{
			uiFinalWt = 1;
		}
		else if((Y_weight > 0) && (Y_weight < 8))
		{
			uiFinalWt = (uint8_t)(Y_weight);
			
			if((Y_weight-uiFinalWt)>=0.01)
			{
				uiFinalWt = uiFinalWt+1;
			}
			else
			{
				uiFinalWt = uiFinalWt;
			}
			//--- Finalizing the predicted weight ---
			if(uiFinalWt==6)
			{
				uiFinalWt = uiFinalWt + 1;
				
			}
			else 
			{
				uiFinalWt = uiFinalWt;
				
			}
			
			
		}
		else if(Y_weight >= 8)
		{
			uiFinalWt = 8;
		}
	}
	else
	{
		// VF SOFTWARE NANYANG - 32MM
		//--- Limiting Boundaries ---
		if(RPMVAL > 4487)
	  	{
			RPMVAL =4487;
	  	}
		else if(RPMVAL < 3472)
		{
			RPMVAL = 3472;
		}
		else
		{
			RPMVAL = RPMVAL;
		}
		
		RPMtest = RPMVAL;
		
		//RPM normalization

		fN_RPMval = (float)(((float)(RPMVAL) - (float)(3472)) / (float)(1015));
		
		M_coeff = (-8.66);//(-8.92);
		C_intercept = 9.03;//8.67;
		
		Y_weight = (float)((M_coeff * fN_RPMval) + C_intercept);
		
		
		if(Y_weight <= 0)
		{
			uiFinalWt = 1;
		}
		else if((Y_weight > 0) && (Y_weight < 8))
		{
			uiFinalWt = (uint8_t)(Y_weight);
			//---round off  logic---
			if((Y_weight-uiFinalWt)>=0.01)
			{
				uiFinalWt = uiFinalWt+1;
			}
			else
			{
				uiFinalWt = uiFinalWt;
			}
			//In this case rounded and predicted weights are same
			
		}
		else if(Y_weight >= 8)
		{
			uiFinalWt = 8;
		}
	}
	
	//Categorizing in 4 levels
	if((GET_PROGRAM) == COTTON)
	{
		
		if(uiFinalWt<=2)
		{
			uiLoadRange = 0;	
		}
		else if((uiFinalWt==3)||(uiFinalWt==4))
		{
			uiLoadRange = 1;
		}
		else if((uiFinalWt==5)||(uiFinalWt==6))
		{
			uiLoadRange = 2;
		}
		else if((uiFinalWt==7)||(uiFinalWt==8))
		{
			uiLoadRange = 3;
		}
		else
		{
			uiLoadRange = 3;
		}
	}
	else if(((GET_PROGRAM) == SYNTHETIC)||((GET_PROGRAM) == MIXEDSOIL))
	{
		if(uiFinalWt<=2)
		{
			uiLoadRange = 0;	
		}
		else if((uiFinalWt==3)||(uiFinalWt==4))
		{
			uiLoadRange = 1;
		}
		else if(uiFinalWt==5)
		{
			uiLoadRange = 2;
		}
		else
		{
			uiLoadRange = 2;
		}
	}
}	
