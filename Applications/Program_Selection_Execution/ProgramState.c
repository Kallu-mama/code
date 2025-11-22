
#include "ProgramState.h"
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_userdefine.h"
#include "STLED316S.h"
#include "ErrorHandling.h"
#include "Selection.h"
#include "WashOperations_Global.h"


//*********** global Functions************//
extern void WaterOverFlowCondition();
//extern void MotorTriacShortCondition();
extern void PressureSensorError();
extern void MotorFail();
extern void PowerOnRead();
extern void FCTMode();
extern void RESET_Prog();
extern void ResProg_led();
extern void MotorStop(void);
extern void Programs();
extern void ProgramTimming();
extern void VoltageControl();
extern boolean TestMode();
extern void DebugMode();
extern void ProgSel_led();
extern void Delay_ms(uint16_t);
extern void LowVoltage();
extern void MemoryMirror();
extern void After_LS_Update_Para(void);

//****************************** global variables****************************************************//
extern boolean bProgram_Ended,bDoorLock,bMotor_Start,bHeater,bDrainPump,bWashProgram_Selected,bDoorLockStatus;
extern boolean bPrewashValve,bMainWashValve,bStop_Minutes_Count,bTestMode_Selection,bTestSpinValueRead,bTEST_COMPLETED;
extern boolean bPrewash_Start,bSoak_Start,bHotRinse_Start,bExtraRinse_Start,bDelay_Start1,bTimeSaver_Start;
extern boolean step_start,bRecheck_Count_Start,bDisplay_Flag,bChildLockButton,bStartButton,bDelay_Start;
extern boolean bNtcFailed,bNoWater,uiReset_TempRPM,bHeatCompleted,bProgram_Start,bCommunicationError,bSteam_Filling_Start;
extern boolean bProgramStart,bWaterOverFLow,bPressureSwitchFail,bWaterFillStage,bEco60Reset,b2min_Drin_On;
extern boolean bDrain_Reset_Seconds,bSpinTUmbleStart,bDebugMode_Selection,buzzerTone,bSpin_Status,bEcoLevelReached;
extern boolean bWaterLevel_Read_Required,bHeaterOpen,bDebugMode_Selection,bFoamDetection,bFoamCOuntStart;
extern boolean bTwoLitreReached,bDebugMode_Selection,buzzerTone6,buzzerTone7,buzzerTone3,bUnbalance_Detected;
extern boolean bPressureError,bSpin_Start,bSpinStarted,bRinsehold_Finished,bLevelReached,bBuzzerStart,bSteam_Start;
extern boolean bEnduranceMode,bTESTMODE_START,bSnooze_Flag,bDrainLevelReach,bFaciaTest,bSpecialEnduranceSelected;
extern boolean bHRecheck_Count_Start,bReheatHeater,bSpin_Available,bDelay_Finished,bCoolDown_Process_Start,bCirculation_Pump;
extern boolean bIdle,bSpinTumble,bFoamCOuntStart,bPressureFailure,bSpecialEndurance_Finished,bRapidWashButton;
extern boolean bDisplay_Flag,bDoorLockError,bEnduranceStarted,TubClean_Activated,bFCTMode_Selection,bDispalyoff_flag;
extern boolean Test_mode_Active,Fct_mode_Active,bPhysical_DoorLock_Status,bMotorError_E1,bZCD_Display,bFaciaTestStep;
extern boolean Switch_Reset,bFctCountStart,chip_select,bMotorError_E3,bMotorError_E2,bZcdDetection,bMotorError_Unbalnce;
extern boolean b_option_key_pressed_flag,bDrainTimerStart;

extern uint8_t uiSelected_Position,uiRecheck_Seconds,uiTime_Seconds,uiUnbalance_Value,uiTest_drain;
extern uint8_t uiNextStep_Start_Delay,uiNonFatal_Error,uiMachineState,Step_pos;
extern uint8_t uiTime_Minutes_Completed,uiCoolDown_Seconds,ucCount,uiFoamState;
extern uint8_t ucCount,uiRinseholdState,uiLowPressureTime,uiNoWaterTime,uiLowPressureState,uiNoWaterState;
extern uint8_t uiProgram_Position,uiStep_Seconds,uiDrainState,uiFillingState,uiHeatState;
extern uint8_t uiPresent_Step,uiTestModeState,uiMachineTestState,FaciaState,uiSteamFill_Seconds;
extern uint8_t cdigits_data_array[6],cFlash_Count,cdigits_data[6];
extern uint8_t uiFCTMode,uiBeforeState,ckey_digit_data[6],SFState;
extern uint8_t uiAdditional_Drain_TurnOn_Mins,uiSelection_testmode,uiTimesaver_testmode;
extern uint8_t uiRinseFillState,uiTumbleState,uiAdditional_Heater_TurnOn_Mins;
extern uint8_t uiFCT_SW_Test,uiOption_testmode,uiRpm_testmode,uiTemp_testmode;
extern uint8_t uiStartUpCount,PressureSwitchMinutes,HeatMinutes,uiStartUpdelay,uiOption_closed;
extern uint8_t uidisp_select_option,uiprev_error,uiDrainOffSeconds;
extern uint8_t uiOverFlowOccurs,uikey_debounce_delay;
extern uint8_t uiReheatTime,uiDoor_Pulse_Sec,uiDoorLOckReleaseSeconds;
//extern uint8_t uiSelectedTone,spin_first_press,temp_first_press;
extern volatile uint8_t uiSelectedTone,uiToneFreqSate;
extern uint8_t spin_first_press,temp_first_press;
extern uint8_t uichild_flash_count,uihigh_low_vtlg,power_count;//uiToneFreqSate;
extern uint8_t uiEupTime,uiDoorUnlockPulseCount,uiMotorError;
extern uint8_t uiMachineBefore_Poweroff,uiZcd_Count;
extern uint8_t uiStartUpdelay,SatrtUpDelay,uiStep_Minutes_Completed;
extern uint8_t uiHRecheck_Seconds,uiWaterResetCount,uiSilkTumble,sleep_mode,uiPrewashWaterResetCount;
extern uint8_t uiLowCount,uiTubClean_MSG_Count,uiIDLE_COUNT,uiDISP_MCount,uiDrainTimer;
extern uint8_t uiSleepModeSeconds,ui_prev_door_status,uiEmergencySeconds;
extern uint8_t uiRpmSel,uiTempSel,uimaxspeed_temp,uiPSER_Option_Avbl,uiExtraRinse_Max,uidefault_kg;
extern uint8_t uiExtraRinse_Press_Select,uiExtraRinse_Press_count,uiDISP_MCount;

extern uint16_t mybuzzerdelay,avg,BalanceTime,uiSupplyVoltage,ProgressTime,ProgramTime,iMilliSeconds;
extern uint16_t uicycle_mincompleted,uiCurrent_step_time,uiRPMValue;
extern uint16_t ucMotor_Rotate_Seconds,uidelaymin,uiRPMValue;
extern uint16_t ProgramLifeCycle,uidisplay_flash_count;
extern uint16_t iWaterLevel,iMotor_Spinning_Seconds,uiTubclean_MSG_TMOUT_Count,uiFoamSensingCount;

extern int cSwitch_Pressed;

extern uint8_t uioption_on,uilast_selected_option,uiMachineStart_count,uiMachineBefore_Poweroff;
extern uint8_t uiTemperature,AI_WaterFreq_State_ui8,WashOperationLast30sDrain_state_ui8,CP_State;

extern SystemError MachineErrors;
extern uint8_t uiheat_Minutes_Completed,uiheat_Seconds_Completed;		
//***********************************************************************
//*********************Local Variable ********************************//

boolean buzzerTone12,buzzerTone13,bDoorPulse,bDoorLocked,bEmegency_door_pull,bPowerOff_Door;
boolean uiover_heat,bProgram_End,bOverHeat,bEmptyLevelReached,bEmegency_Lever_pull;
boolean disp_brightness_control,bUnbalanceError,bDoorOpenError,bDoorClosed,bDoorUnlocked;
boolean bHPrs,bLPrs,bWakeup_F;

uint8_t uiBefore_Snooze,prev_program_pos;
uint8_t uimachine_shutdown,uimachine_Off,uiDoorPulseCount,tt;
uint8_t snooze_offtime,uitemp_onoff,snoozestart_onoff,uisnooze_mode_pos;
uint8_t snooze_display = 2;
uint8_t uiTubClean_req ,uiTubClean_Disp_count, uiTubClean_Disp,uitubclean_cycle;
uint8_t uiProgramCount,uiAdd,uiHeaterCheck,uiRecoverCount;
uint8_t L175_count;

uint16_t snoozemode_10minutes_counter,change_display_bright;
uint16_t bright,dull,uiDullness,uiBrightness;
#if ENABLE_IOT

#include"iot_config.h" 
#include "iot_control.h"
extern int uDiagnostic_Time_Value;
extern boolean bDiagnosticMode;
extern boolean bDiagnostic_Set_Flag;
extern char cMachineDiagnosticState;
extern uint8_t BkMultiFuncSetting;
extern void Reset_ProgParamter();
#endif
//******************************************************************************************************//


extern boolean bFoamRm_Step,bLoadSensing_Completed;
extern uint8_t Foam_Removal_Complete;
extern boolean Foam_Removal_Enable, LS_flag, bStep_Finished;
extern uint8_t uiFinalWt, uiLS_Tx_subtraction, uiLS_Total_subtraction,uiLoadRange,uiLS_Seconds; //uiResetFill_Count;
/*************************************************************************************************************
* Function Name: CurrentState
* Description  : This function is to determine the current state of Machine.
* Arguments    : None
* Return Value : None
**************************************************************************************************************/
void CurrentState()
{
	static uint8_t ii;
	
	switch(uiMachineState)
	{		
		
		case STARTUP:
#if ENABLE_IOT
		set_IOTState(STATE_IOT_START);
#endif
			WashOperationLast30sDrain_state_ui8 = RESET_VALUE; // Added to reset the state.
			b_option_key_pressed_flag = RESET_VALUE;
			if(bStartButton == TRUE)
			{
				bWaterLevel_Read_Required = 1;
				bStop_Minutes_Count = NO;
				uiNonFatal_Error = RESET_VALUE;
				//cDoor_Lock_sec = 15;
				uiMachineState = START_PRESSED;
				bIdle = FALSE;
				AI_WaterFreq_State_ui8 = 0;
				setAIComm_State_ui8(AI_Idle);
			}
			MachineStartState();
			//Reset_ProgParamter();
			Programs();
			/*if(bCommunicationError == TRUE)
			{
				MachineErrors.EBits.DoorError = 1;
				bStartButton = FALSE;
				//uiMachineState = PAUSE_PRESSED;
			}*/
			if(bTestMode_Selection == FALSE)
			{
				bDoorLocked = FALSE;
			}
			uiWaterResetCount = RESET_VALUE;
			
			uiPrewashWaterResetCount = RESET_VALUE;//TODO: Added for Prewash Reset. 
			CP_State = RESET_VALUE;			// TODO: Added PSB_BTRA
			setAIComm_Program_ui8(RESET_VALUE);	// TODO: Added PSB_BTRA
			WashOperationLast30sDrain_state_ui8 = RESET_VALUE; // TODO: Added PSB_BTRA
			setAIComm_State_ui8(AI_Idle);	// TODO: Added PSB_BTRA
			
			bEco60Reset = FALSE;
			bUnbalance_Detected = FALSE;
			uiBeforeState = RESET_VALUE;
			bEmegency_door_pull = RESET_VALUE;
			Switch_Reset = TRUE;
			bDoorOpenError = RESET_VALUE;
			break;
			
		case IDLE_START: 
#if ENABLE_IOT
		set_IOTState(STATE_IOT_IDEAL);
#endif
			Programs();
			uiWaterResetCount = RESET_VALUE;
			WashOperationLast30sDrain_state_ui8 = RESET_VALUE; // Added to reset the state.
			CP_State = RESET_VALUE;
			setAIComm_Program_ui8(RESET_VALUE);
			uiPrewashWaterResetCount = RESET_VALUE; //TODO:
			setAIComm_State_ui8(AI_Idle);
			if(bTestMode_Selection == FALSE)
			{
				DoorCondition_Check();
			}
			uiBefore_Snooze = 0x00;
			bEco60Reset = FALSE;
			bWaterLevel_Read_Required = TRUE;
			bStop_Minutes_Count = NO;
			bDoorLocked = FALSE;
			bUnbalance_Detected = FALSE;
			bMotorError_Unbalnce = FALSE;
			bUnbalanceError = FALSE;
			bEmegency_door_pull = RESET_VALUE;
			uiNonFatal_Error = RESET_VALUE;
			prev_program_pos = RESET_VALUE;
			MachineErrors.EBits.HighUnbalance = 0;

			if(bStartButton == TRUE)
			{
				uiMachineState = START_PRESSED;
				uiSelectedTone = STARTPRESSTONE;
				MemoryMirror();
				bMotorError_Unbalnce = FALSE;
				bUnbalanceError = FALSE;
				
				bFoamRm_Step = 0;
				Foam_Removal_Complete =0;
				Foam_Removal_Enable = 0; //start pressed //Changed from 1 to 0 to stop filling in refresh program and it should only be enabled in express cycles
				bIdle = FALSE;
				AI_WaterFreq_State_ui8 = 0;
				setAIComm_State_ui8(AI_Idle);
			}
			else if(bTestMode_Selection == TRUE)
			{
				uiMachineState = TESTMODE;
				uiSelectedTone = KEYPRESSTONE;
				bIdle = FALSE;
			}
			else if(bDebugMode_Selection == TRUE)
			{
				DebugMode();
				bIdle = FALSE;
			}
			if(bCommunicationError == TRUE)
			{
				MachineErrors.EBits.DoorError = 1;
				uiMachineState = COMMUNICATIONERROR;
			}
			else if(bFCTMode_Selection == TRUE)
			{
				 uiMachineState = FCTMODE;
				 bIdle = FALSE;
			}
			else
			{
				bIdle = TRUE;
				
			}
#if ENABLE_IOT
			uiSelected_Position = uiProgram_Position;
#endif
			bProgramStart = FALSE;
			uiBeforeState = IDLE_START;
			//added as a safety condition to deselect warmsoak for cold temperatures
			if((uiTempSel == WashCold) || (uiTempSel == P_COLD))
			 {
				 uiPSER_Option_Avbl =  uiPSER_Option_Avbl &(~ WarmSoak_Yes);
				 uidisp_select_option =  uidisp_select_option &(~ WarmSoak_Yes);
			 }
			break;
		
		case PROGRAM_END:
		Foam_Removal_Enable = FALSE;	//program end
		Foam_Removal_Complete = FALSE;	//program end
				ProgramEnd();
#if ENABLE_IOT	
		if((bDoorLockStatus == FALSE)&&(bDoorLockError == FALSE))
		{
			set_IOTState(STATE_IOT_END);	
		}				
#endif
			uiBeforeState = PROGRAM_END;
			break;
		
		case START_PRESSED:
#if ENABLE_IOT
		if(get_IOTState() < 0x02)
		{
			set_IOTState(STATE_IOT_START);
		}
#endif
	
				if(bSpecialEndurance_Finished == TRUE)
				{
					if(bChildLockButton == FALSE)
					{
						//bDoorLock = SWITCH_OFF;
						//bDoorPulse = FALSE;
					}
					bEmegency_Lever_pull = FALSE;
				}
				else
				{
					if(bCommunicationError == TRUE)
					{
						MachineErrors.EBits.DoorError = 1;
						uiMachineState = COMMUNICATIONERROR;
					}
					else if((bDoorPulse == FALSE)&&(bDoorLockStatus == FALSE)&&(bDoorLock == FALSE)&&(bPhysical_DoorLock_Status  == TRUE))
					{
						bDoorLock = SWITCH_ON;
						bDoorPulse = TRUE;
					}
					else if((bDoorLockStatus == TRUE)&&(bPhysical_DoorLock_Status  == TRUE))
					{
						bDoorPulse = TRUE;
						bDoorLocked = TRUE;
						bPowerOff_Door = FALSE;
					}
					/*else if((bSpecialEnduranceSelected == TRUE)&&(bDoorLockStatus == FALSE)&&(bSpecialEndurance_Finished == FALSE))
					{
						bEmegency_Lever_pull = TRUE;
						if(uiEmergencySeconds >= 3)
						{
							DoorError_Check();
						}
					}*/
					else if(((bDoorLocked == FALSE)||(bPhysical_DoorLock_Status == FALSE))&&(bZcdDetection == TRUE))
					{
						if (iMilliSeconds > 990 )//MILLI_FIVE_HUNDRED) //issue observed during spin power interrupt machine displays door error
						{
							if(bZcdDetection == TRUE)// solution delay added to get zcd signal from motor board
							{
								bEmegency_Lever_pull = TRUE;
								if(uiEmergencySeconds >= 3)
								{
									DoorError_Check();
								}
							}
						}
					}
					else if((bDoorLocked == TRUE)&&(bDoorLock == FALSE)&&(bDoorPulse == TRUE))
					{
						bDoorLock = TRUE;
					}
					else
					{
						if(bZcdDetection == TRUE)
						{
							bEmegency_Lever_pull = TRUE;
							if(uiEmergencySeconds >= 4)
							{
								bDoorLocked = FALSE;
								uiDoorPulseCount = RESET_VALUE;
								//Emergency_DoorOpen();
								DoorError_Check();
							}
						}
						else
						{
							bEmegency_Lever_pull = FALSE;
						}
					}
				}
					
				bWashProgram_Selected = YES;
				uiDoorLOckReleaseSeconds = RESET_VALUE;
				uiDoorUnlockPulseCount = RESET_VALUE;
				bProgramStart = TRUE;
				uiAdd = RESET_VALUE;
				if(((bDoorLockStatus == TRUE)||(bSpecialEndurance_Finished == TRUE))&&(bCommunicationError == FALSE))
				{
					MachineErrors.EBits.DoorError = 0;
					//cDoor_Lock_sec = 15;
					uiEmergencySeconds = RESET_VALUE;
					uiDoorPulseCount = RESET_VALUE;
					bEmegency_Lever_pull = RESET_VALUE;
					bDoorOpenError = FALSE;
					if(bZcdDetection == TRUE)
					{
						PressureSwitchFailure();
						HeaterFailure();
					}
				}
				else
				{
					if((bDoorOpenError == TRUE)&&(uiDoorPulseCount > RESET_VALUE))
					{
						if(uiDoorPulseCount < 2)
						{
							uiprev_error = 0xFF;
							buzzerTone = RESET_VALUE;
						}
					}
					ComponentSwitchOff();
					MotorStop();
				}
				if((bStartButton == PAUSEPRESSED)&&(bDoorLockStatus == TRUE))
				{
					if(mybuzzerdelay == 0)
					{
						uiMachineState = PAUSE_PRESSED;
						uiSelectedTone = PAUSETONE;
					}
				}
				if(bTestMode_Selection == TRUE)
				{
					if(bFaciaTest != 1)
					{
						uiSelectedTone = KEYPRESSTONE;
					}
					else
					{
						uiSelectedTone = STARTPRESSTONE;
					}
					uiMachineState = TESTMODE;
				}
				else if((bMotorError_E1 == TRUE)||(bMotorError_E2 == TRUE)||(bMotorError_E3 == TRUE))
				{
					uiMachineState = MOTORFAILURE;
				}
				else if(bDebugMode_Selection == TRUE)
				{
					//uiSelectedTone = KEYPRESSTONE;
					DebugMode();
					Programs();
				}
				else
				{
					Programs();
				}
				
				uiBeforeState = RESET_VALUE;
			break;
	
		case PAUSE_PRESSED:
#if ENABLE_IOT		
		if((uiRinseholdState == 2) && (bChildLockButton == 1) && (uiMachineState != POWEROFF))
		{
			set_IOTState(STATE_IOT_RINSE_HOLD_AND_PAUSE);	
		}
		else
		{
			set_IOTState(STATE_IOT_PAUSE);	
		}	
#endif
			bWaterLevel_Read_Required = TRUE;
			bUnbalance_Detected = FALSE;
//			bStart_Traic_Fire = FALSE;
			bSpin_Start = RESET_VALUE;
			//bSpinTumble = FALSE;
			uiheat_Minutes_Completed = RESET_VALUE;
			uiheat_Seconds_Completed = RESET_VALUE;
			if(bZcdDetection == TRUE)
			{
				PauseState();
			}
			if(bDebugMode_Selection == TRUE)
			{
				DebugMode();
				Programs();
			}
			
			uiBeforeState = PAUSE_PRESSED;
			
			if((uiRinseholdState == 2)&&(bChildLockButton == 1))
			{
				uichild_flash_count = 1;
			}
			
			break;
	
		case PROGRAM_RESET:
	
			StopAllEvents();
#if ENABLE_IOT
		ckey_digit_data[3] = cdigits_data[5]= 0x0;
		uiBefore_Snooze = RESET_VALUE;
		Reset_ProgParamter();
		uiMachineState = IDLE_START;
		
#endif
			break;
	
		case TESTMODE:
			bTestMode_Selection = TRUE;
			//bChildLockButton = FALSE;
			TestMode();
			if((bDebugMode_Selection == TRUE)&&((uiTestModeState != SF01)&&(uiTestModeState != SF02)))
			{
				//uiSelectedTone = KEYPRESSTONE;
				DebugMode();
				//Programs();
			}
			if(bCommunicationError == TRUE)
			{
				MachineErrors.EBits.DoorError = 1;
				uiMachineState = COMMUNICATIONERROR;
				//bStartButton = FALSE;
				//uiMachineState = PAUSE_PRESSED;
			}
			if(((bStartButton == TRUE)&&(bFaciaTest == TRUE)&&(uiMachineTestState != step7))||((bStartButton == TRUE)&&((uiTestModeState != SF01)&&(uiTestModeState != SF02)))&&(bZcdDetection == TRUE))
			//if(((bStartButton == TRUE)&&(bFaciaTest == TRUE)&&(uiMachineTestState != step8))||((bStartButton == TRUE)&&((uiTestModeState != SF01)&&(uiTestModeState != SF02)))&&(bZcdDetection == TRUE))
			{
				if((bDoorLockStatus == TRUE)&&(bCommunicationError == FALSE))
				{
					MachineErrors.EBits.DoorError = 0;
					uiDoorPulseCount = RESET_VALUE;
					bEmegency_Lever_pull = FALSE;
					bDoorLocked = TRUE;
					bDoorPulse = TRUE;
					bPowerOff_Door = FALSE;
					uiEmergencySeconds = RESET_VALUE;
				}
				else if((bDoorLocked == TRUE)&&(bDoorLock == FALSE)&&(bDoorPulse == TRUE))
				{
					bDoorLock = TRUE;
				}
				else
				{
					bEmegency_Lever_pull = TRUE;
					if(uiEmergencySeconds >= 3)
					{
				 		DoorError_Check();
					}
				}
			}
			 if((bMotorError_E1 == TRUE)||(bMotorError_E2 == TRUE)||(bMotorError_E3 == TRUE))
			{
				uiMachineState = MOTORFAILURE;
			}
			uiBeforeState = RESET_VALUE;
			bWashProgram_Selected = YES;
			break;
	
		case SNOOZEMODE:
#if ENABLE_IOT
		if(get_IOTState() != STATE_IOT_END)
		{
			set_IOTState(STATE_IOT_STANDBY);
		}
#endif		
			if(uiTubClean_Disp == FALSE)
			{
				change_display_bright++;
				buzzerTone6 = RESET_VALUE;
				
			     	if((uiSleepModeSeconds > 7) && (uiMachineState = SNOOZEMODE))
				{
					uiDullness++; // 
					if(uiDullness < dull)
					{
					  START_LED_ON;
					}
					else if(uiDullness < 400) 
					{
					   START_LED_OFF;
					}
					else
					{
					   uiDullness = RESET_VALUE;
					   tt++;
					   if(tt > 2)
					   {
						   if(dull > 1)
						   dull--;
						   tt = 0;
					   }
					}
				}
		    	else if((uiSleepModeSeconds > 5) && (uiMachineState = SNOOZEMODE))
			{
				uiBrightness++;
				if(uiBrightness < bright)
				{
				  START_LED_OFF;
				}
				else if(uiBrightness < 500) 
				{
				  START_LED_ON;
				}
				else
				{
				   uiBrightness = RESET_VALUE;
				   tt++;
				   if(tt > 1)
				   {
					   if(bright > 2)
					   bright--;
					   tt = 0;
				   }
				}
			}
			else
			{
				uiBrightness = RESET_VALUE; 
				bright = 500;
				dull = 350;
			}
		     //------------------
				uiBefore_Snooze = uiBeforeState ;// = SNOOZEMODE;
				ResProg_led();
				sleep_mode = 200;
				bIdle = FALSE;
				/*
				if(uiBefore_Snooze == PROGRAM_END)
				{
					if(uimachine_shutdown >= 7) // 60 minutes & 20 sec of sleep mode conter 
					{
							//RESET_HIGH;
							Switch_Reset = FALSE;
							uimachine_Off = TRUE;
						}
					}
					else
					{
						if(uimachine_shutdown >= 6) // 60 minutes conter in idle
						{
							//RESET_HIGH;
							Switch_Reset = FALSE;
						uimachine_Off = TRUE;
						}
					}
				*/
				if((((cSwitch_Pressed != RESET_VALUE)||(bWakeup_F == TRUE))&&(uiBefore_Snooze == IDLE_START)) ||(uisnooze_mode_pos != uiProgram_Position)||( bSnooze_Flag == TRUE))
				{
					bWakeup_F = FALSE;
					chip_select = FALSE; 
//					STLED316_SetBrghtCtrlModeAndDigitCnt(STLED316_BRGHT_MODE_VAR, 0, 6);
//					
//					for(ii = 0; ii<6;ii++)
//					{
//						STLED316_7Seg_SetBrghtInd(ii, 3, 3);
//						STLED316_7Seg_Write(ii,0x00); // this is should not be 0x00 , default array have to written
//					}
//					STLED316_DisplayOn(); 
//					//chip_select = TRUE; 				
//					for(ii = 0; ii<6;ii++)
//					{
//						STLED316_7Seg_SetBrghtInd(ii, 3, 3);
//						STLED316_7Seg_Write(ii,0x00); // this is should not be 0x00 , default array have to written
//					}
					for(ii = 0xC0; ii <= 0xCD; ii= ii + 0x01)  //ADDED FOR NEO - AK
					{
						//STLED316_7Seg_SetBrghtInd(ii, 3, 3);
						STLED316_7Seg_Write(ii,0x00); // this is should not be 0x00 , default array have to written
					}

					STLED316_DisplayOn(); 
					chip_select = FALSE; 				
					
					if(uiBefore_Snooze == PROGRAM_END)
					{
						if(uisnooze_mode_pos != uiProgram_Position)
						{
							uiSelected_Position = uiProgram_Position = prev_program_pos  ;
						}
						ProgSel_led();
						uiMachineState = PROGRAM_END;
						//uiBefore_Snooze = RESET_VALUE;
						buzzerTone6 = RESET_VALUE;
						uimachine_shutdown = RESET_VALUE;
					}
					else{
						if(uisnooze_mode_pos != uiProgram_Position)
						{
							prev_program_pos= uiProgram_Position ;
							uimachine_shutdown = RESET_VALUE;
						}
						
						StopAllEvents();
						bStartButton = PAUSEPRESSED;
						uiMachineState = IDLE_START;
						uiSelected_Position = uiProgram_Position;
						prev_program_pos = RESET_VALUE;
						uiPresent_Step = RESET_VALUE;
						bProgramStart = RESET_VALUE;
						uikey_debounce_delay = 200;
						ProgSel_led();
						RESET_Prog();
						ProgramTimming();					
						}
					}
			}
			else
			{
				//uiTubClean_MSG_Count;
				//uiTubclean_MSG_TMOUT_Count;
				ckey_digit_data[0] =  ckey_digit_data[1] =  ckey_digit_data[2] = 0x00; // clear the selected options & navigation mark in error condition
				if(uiTubClean_MSG_Count < 2)
				{
					if(bDisplay_Flag == TRUE)
					{
						mystrcpy((char *)cdigits_data_array, " run");
						
						update_display_data(&cdigits_data_array[0]);
						bDisplay_Flag = FALSE;
						uidisp_select_option = 0x00;
					}
				}
				else if(uiTubClean_MSG_Count < 4)
						  					 
				{
					if(bDisplay_Flag == TRUE)
					{
						//mystrcpy((char *)cdigits_data_array, " End");
						mystrcpy((char *)cdigits_data_array, " tUb");
						
						update_display_data(&cdigits_data_array[0]);
						bDisplay_Flag = FALSE;
						uidisp_select_option = 0x00;
					}
				   
				}
				else if(uiTubClean_MSG_Count < 6)
				{
					if(bDisplay_Flag == TRUE)
					{
						//mystrcpy((char *)cdigits_data_array, " End");
						mystrcpy((char *)cdigits_data_array, " CLn");
						update_display_data(&cdigits_data_array[0]);
						bDisplay_Flag = FALSE;
						uidisp_select_option = 0x00;
					}			  					   
				}
			}
			break;
		case POWEROFF:
				if(bDispalyoff_flag == FALSE)
				{	
					bTEST_COMPLETED = RESET_VALUE;
					chip_select = FALSE; 		
					STLED316_DisplayOff();
					//chip_select = TRUE;
					uiIDLE_COUNT = RESET_VALUE;
					uiDISP_MCount = RESET_VALUE;
					bZCD_Display = TRUE;
					bDispalyoff_flag = TRUE;
					STLED316_DisplayOff();
					ckey_digit_data[0] = ckey_digit_data[1] = ckey_digit_data[2] = ckey_digit_data[3] = ckey_digit_data[4] = ckey_digit_data[5] = 0x00;
					cdigits_data[0] = cdigits_data[1] = cdigits_data[2] = cdigits_data[3] = cdigits_data[4] = cdigits_data[5] = cdigits_data[6] = 0x00;
					//ResetMemoryValues(); 
					
					if(bFCTMode_Selection == TRUE)
					{
						bFctCountStart = NO; // reset the FCT counter
						uiSelected_Position = uiProgram_Position = 1;						
						//uiTFCTSeconds = RESET_VALUE;
						//uiMachineState = STARTUP;
						bStartButton = FALSE;
						uiFCT_SW_Test = RESET_VALUE; // reset key pressed variable 
					}
					else if(bTestMode_Selection == TRUE)
					{
						uiSelection_testmode = TRUE;
						uiOption_testmode = TRUE;
						uiRpm_testmode = TRUE;
						uiTemp_testmode = TRUE;
						uiTimesaver_testmode = TRUE;
						FaciaState = RESET_VALUE;
						//cdisp_count = FALSE;
						//bTest_Forward = TRUE;
					}
					bFCTMode_Selection = FALSE;
					bTestMode_Selection = FALSE;
					bDebugMode_Selection = FALSE;
				}
				
				START_LED_OFF;
				
				if((bChildLockButton == FALSE)|| (uiDoorLOckReleaseSeconds == 0xFF))
				{
					bEmegency_door_pull = FALSE;
					uidisp_select_option = uilast_selected_option;
					if( uiExtraRinse_Press_count ==  0)
					{
						;//uidisp_select_option = uidisp_select_option & (~ExtraRinse_Yes);
						//uilast_selected_option &= (~ExtraRinse_Yes);
					}
					uiProgram_Position = uiSelected_Position;									
					uioption_on = RESET_VALUE;					
					StopAllEvents();						
					ProgSel_led();  // update the led sattus
					RESET_Prog();
					ProgramTimming(); // select & show the programming timing 
					MachineState_LED(FALSE,FALSE,FALSE,FALSE);
					
					 if((bDoorLockStatus == TRUE)&&(iWaterLevel > EMPTYLEVEL1)&&(uiTemperature < ON_PAUSE_DOOR_OPEN_TEMPERATURE)&&(uiRPMValue == RESET_VALUE))
					 {
						 bDoorPulse = FALSE;
						 bDoorLock = FALSE;
					 }
					
				}
				else
				{
					ComponentSwitchOff();
					//SpinProfileReset();
					bTestSpinValueRead = RESET_VALUE;
				}
				uiSelectedTone = RESET_VALUE;
				uiToneFreqSate = RESET_VALUE;
				bBuzzerStart = FALSE;
				P6.3 = RESET_VALUE;
				//added as a safety condition to deselect warmsoak for cold temperatures
				if((uiTempSel == WashCold)||(uiTempSel == P_COLD))
				 {
					 uiPSER_Option_Avbl =  uiPSER_Option_Avbl &(~ WarmSoak_Yes);
					 uidisp_select_option =  uidisp_select_option &(~ WarmSoak_Yes);
				 }
			break;
			
		case RESTARTPOWER:
				 	 
				 if(bChildLockButton == FALSE)
				{
					uiMachineState = STARTUP;
					Test_mode_Active = FALSE;
					Fct_mode_Active = FALSE;
					uiMachineStart_count = 150;
					uiStartUpdelay = 4;
					SatrtUpDelay = 0;//3;	//commented to remove the delay btwn buzzer ad display
					uiStartUpCount = 4;
					uiSelectedTone = STARTUPTONE;
					uidisplay_flash_count = RESET_VALUE;
					ProgSel_led();  // update the led sattus
					RESET_Prog();
					ProgramTimming(); // select & show the programming timing 
					TurnOnSelected_Option();
					
					
				}
				else
				{
					uiSelectedTone = STARTUPTONE;
					uidisplay_flash_count = RESET_VALUE;						
					//TurnOnSelected_Option();
					//Switch_Reset = TRUE;
					uiMachineState = uiMachineBefore_Poweroff ;	
				}
			break;
		
		case HIGHLOWVOLTAGE:
		
			 if((uiRecoverCount > 8)&&(uiSupplyVoltage >= SUPPLY160)&&(uiSupplyVoltage <= SUPPLY260))
			 {
				 uiLowCount = RESET_VALUE;
				 buzzerTone3 = FALSE;
				 uiprev_error = 0xFF;
				if(bProgramStart == YES)
				{
					if(bFaciaTest == TRUE)
					{
						bStop_Minutes_Count = FALSE;
					}
					if(uiBeforeState != RESET_VALUE)
					{
						uiMachineState = uiBeforeState;
						uiBeforeState = RESET_VALUE;
					}
					else if(bStartButton == PAUSEPRESSED)
					{
						uiMachineState = PAUSE_PRESSED;
					}
					else if((bStartButton == STARTPRESSED) && (bTestMode_Selection == TRUE))
					{
						uiMachineState = TESTMODE;
					}
					else if(uiBeforeState == SNOOZEMODE)
					{
						uiMachineState = IDLE_START;
						bProgramStart = RESET_VALUE;
					}
					else if(uiBeforeState == HEATERROR)
					{
						uiMachineState = HEATERROR;
					}
					else if(uiBeforeState == PROGRAM_END)
					{
						uiMachineState = PROGRAM_END;						
					}
					else
					{
				 		uiMachineState = START_PRESSED;
						//uiMototor_Start = 4;
						if(bSpinStarted == TRUE)
						{
//							BalanceTime = BalanceTime + uiTime_Minutes_Completed;
//							uiCurrent_step_time = uiCurrent_step_time + uiTime_Minutes_Completed;
//							uiTime_Minutes_Completed = RESET_VALUE;
//							uiTime_Seconds = RESET_VALUE;
							
							L175_count++;
							if(L175_count<3)
							{
								BalanceTime = BalanceTime + uiTime_Minutes_Completed;
								uiCurrent_step_time = uiCurrent_step_time + uiTime_Minutes_Completed;
								uiTime_Minutes_Completed = RESET_VALUE;
								uiTime_Seconds = RESET_VALUE;
							}
							else
							{
								L175_count = RESET_VALUE; //to reset the count after every logic execution so that the logic restarts for next spin
								
								//Below condition added to skip LS for 3times HV/LV case and take default load category
								if((((uiSelected_Position == COTTON) && ((uiTempSel != Wash40E)&&(uiTempSel != Wash60E)))|| (uiSelected_Position == SYNTHETIC) || (uiSelected_Position == MIXEDSOIL)) && (uiPresent_Step == 1))
								{
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
									
									LS_flag = FALSE; //to clear LS display
									bLoadSensing_Completed = TRUE; //to exit LS execution and wait to go to next step untill motor fault is cleared
									After_LS_Update_Para();
									MotorStop();
									bSpin_Status = TRUE;
									//bStep_Finished = YES;
									bDrainPump = SWITCH_OFF;
								}
								else
								{
									MotorStop();
									bSpin_Status = TRUE;
									bStep_Finished = YES;
									bDrainPump = SWITCH_OFF;
								}
							}
						}
					}
					MachineErrors.EBits.VoltageError = 0;
					uihigh_low_vtlg = RESET_VALUE;
				}
				else
				{
					uiMachineState = IDLE_START;
					MachineErrors.EBits.VoltageError = 0;
				}
			 }
			 else
			 {
				 //if(uiSupplyVoltage > 12)
				 {
					MachineErrors.EBits.VoltageError = 1;
				 }
				 if((uiSupplyVoltage >= SUPPLY160)&&(uiSupplyVoltage <= SUPPLY260))
				 {
					uiRecoverCount ++;
				 }
				 else
				 {
					uiRecoverCount = RESET_VALUE;
				 }
			 }
			VoltageControl();
			
			break;
	
		case WATERERROR:
	
			 WaterOverFlowCondition();
			 Programs();
			 if((bDebugMode_Selection == TRUE)&&(uiOverFlowOccurs <= 2))
			{
				//uiSelectedTone = KEYPRESSTONE;
				DebugMode();
				//Programs();
			}
		
			 break;
	
		case PRESSUREERROR:
				PressureSensorError();
			Programs();
			break;
		case COMMUNICATIONERROR:
				
				if(bZCD_Display == FALSE)
				{
					bZCD_Display = TRUE;
				}
					
				if(bCommunicationError == FALSE)
			 	{
					 uiprev_error = 0xFF;
					if(bProgramStart == YES)
					{
						if(bFaciaTest == TRUE)
						{
							bStop_Minutes_Count = FALSE;
						}
						if(uiBeforeState != RESET_VALUE)
						{
							uiMachineState = uiBeforeState;
							uiBeforeState = RESET_VALUE;
						}
						else if(bStartButton == PAUSEPRESSED)
						{
							uiMachineState = PAUSE_PRESSED;
						}
						else if((bStartButton == STARTPRESSED) && (bTestMode_Selection == TRUE))
						{
							uiMachineState = TESTMODE;
						}
						else if(uiBeforeState == SNOOZEMODE)
						{
							uiMachineState = IDLE_START;
							bProgramStart = RESET_VALUE;
						}
						else if(uiBeforeState == HEATERROR)
						{
							uiMachineState = HEATERROR;
						}
						else
						{
					 		uiMachineState = START_PRESSED;
							//uiMototor_Start = 4;
							if(bSpinStarted == TRUE)
							{
								BalanceTime = BalanceTime + uiTime_Minutes_Completed;
								uiCurrent_step_time = uiCurrent_step_time + uiTime_Minutes_Completed;
								uiTime_Minutes_Completed = RESET_VALUE;
								uiTime_Seconds = RESET_VALUE;
							}
						}
						 MachineErrors.EBits.DoorError = 0;
					}
					else
					{
						uiMachineState = IDLE_START;
						 MachineErrors.EBits.DoorError = 0;
					}
				 }
				 else
				 {
					 MachineErrors.EBits.DoorError = 1;	
				 }
				VoltageControl();
			break;
		case HEATERROR:
		
				if(bStartButton == TRUE)
				{
					bHeater = SWITCH_OFF;
					uiBeforeState = HEATERROR;
					MachineErrors.EBits.HeaterError = 1;
					bWaterLevel_Read_Required = 1;  
					if((HeatMinutes != RESET_VALUE) && (iWaterLevel > 37170)&&(uiHeaterCheck <= 1)&&(uiTemperature > 50))
					//if((HeatMinutes != RESET_VALUE) && (iWaterLevel > 22570)&&(uiTemperature > 50))
					{
						bDoorLock = SWITCH_ON;
						if(bDoorLockStatus == TRUE)
						{
							if((bTestMode_Selection == TRUE)&&((uiTestModeState == SF01)||(uiTestModeState == SF02)))
							{
								bMainWashValve = SWITCH_OFF;
							}
							else
							{
								bMainWashValve = SWITCH_ON;
							}
							bDrainPump = SWITCH_OFF;
						}
					}
					else if((iWaterLevel < 39740)&&(uiTemperature > 50)&&(uiHeaterCheck <= 1))
					//else if((iWaterLevel < 24770)&&(uiTemperature > 50))
					//else if(iWaterLevel < 24770)
					{
						bDoorLock = SWITCH_ON;
						bDrainPump = SWITCH_ON;
						bMainWashValve = SWITCH_OFF;
					}
					else if((iWaterLevel < 39740)&&(uiTemperature < 50))
					{
						bDoorLock = SWITCH_ON;
						bDrainPump = SWITCH_ON;
						bMainWashValve = SWITCH_OFF;
					}
					else if((uiHeaterCheck <= 1)&&(uiTemperature > 50))
					//else if(uiTemperature > 50)
					{
						uiHeaterCheck ++;
						HeatMinutes = 120;
					}
					else if(iWaterLevel > 40610)
					{
						bDrainPump = SWITCH_OFF;
						bDoorLock = SWITCH_OFF;
						bOverHeat = FALSE;
						ComponentSwitchOff();
					}
				}
				Programs();
				if(bZcdDetection == TRUE)
				PressureSwitchFailure();
			break;
	
	
		case MOTORFAILURE:
				Programs();
	
				if((bStartButton == TRUE)||(bFCTMode_Selection == TRUE))
				{
					MotorFail();
				}
				else
				{
					if(mybuzzerdelay == 0)
					{
						uiMachineState = PAUSE_PRESSED;
						uiSelectedTone = PAUSETONE;
					}
				}
				
				if(bDebugMode_Selection == TRUE)
				{
					DebugMode();
				}
				if(bZcdDetection == TRUE)
				PressureSwitchFailure();
				HeaterFailure();
				
			break;
		
		case FCTMODE:
				bFCTMode_Selection = TRUE;
				bChildLockButton = FALSE;
				FCTMode();
				//below condition is added to avoid E1,E2 & E3 error recovery in FCT mode
				if((bMotorError_E1 == TRUE)||(bMotorError_E2 == TRUE)||(bMotorError_E3 == TRUE))
				{
					uiMachineState = MOTORFAILURE;
				}
			break;
		case DEBUG_MODE:
		
				DebugMode();	
					break;
		case 255 :
			if(bStartButton == TRUE)
				PowerOnRead(); // restore the prevous values
			else
				uiMachineState= IDLE_START;
			break;
				
		default :		//No operation required
					break;
				
	}
}


/*************************************************************************************************************
* Function Name: StopAllEvents
* Description  : This function is to clear the Machine parameters.
* Arguments    : None
* Return Value : None
**************************************************************************************************************/
void StopAllEvents()
{
	
		bWashProgram_Selected = RESET_VALUE;
		bDrainLevelReach = FALSE;
		//bDoorLock = SWITCH_OFF;
		ComponentSwitchOff();
		uiReheatTime = RESET_VALUE;
		bMotor_Start = NO;
		if(bEmegency_door_pull == FALSE)
		{
			bStartButton = FALSE;
			//if(Switch_Reset == TRUE)
			 //  uiMachineState =  STARTUP;		
		}
		//bSpinTumble = RESET_VALUE;
		bEmptyLevelReached = FALSE;
		iMotor_Spinning_Seconds = RESET_VALUE;
		MotorStop();		
		//bSpinTUmbleStart = RESET_VALUE;
		uiStartUpCount = RESET_VALUE;
		bFCTMode_Selection = FALSE;
		bFaciaTestStep = FALSE;
		bProgramStart = RESET_VALUE;
		uiStep_Seconds = RESET_VALUE;
		uiLS_Seconds = 240;//180;//120; //2mins dec timer reset
		uiPresent_Step = RESET_VALUE;
		bSpinStarted = RESET_VALUE;
//		cSpinProfileIndex = RESET_VALUE;
		MachineErrors.EReg = 0;
		uiOption_closed = RESET_VALUE;
		bSpecialEnduranceSelected = FALSE;
		bWaterLevel_Read_Required = TRUE; 
		uiRinseholdState = RESET_VALUE;
		//SpinProfileReset();
		uiprev_error = 0xFF;
		uiTestModeState = RESET_VALUE;
		uiMachineTestState = RESET_VALUE;
		FaciaState = RESET_VALUE;
		if(bTestMode_Selection == TRUE)
		 	uiReset_TempRPM = TRUE;
			
		bTestMode_Selection = FALSE;
		uiTest_drain = RESET_VALUE;
		bFaciaTest = RESET_VALUE;
//		bMotorTachoShort = FALSE;
//		bMotorTriacShort= FALSE;
		bSpin_Available = TRUE;
		bEnduranceMode = RESET_VALUE;
		bHRecheck_Count_Start = NO;
		bRecheck_Count_Start = NO;
		bDebugMode_Selection = FALSE;
		bPressureError = RESET_VALUE;
//		bMotorTraicFail = RESET_VALUE;
//		bMotorCountStart = RESET_VALUE;
//		bMotorTriacShort = RESET_VALUE;
//		uimotorindex = RESET_VALUE;  
		ucMotor_Rotate_Seconds = RESET_VALUE;
		uiFCTMode = RESET_VALUE;
		bSpecialEndurance_Finished = FALSE;
		uiBeforeState = RESET_VALUE;
		uiNonFatal_Error = RESET_VALUE;
		uiWaterResetCount = RESET_VALUE;
		bReheatHeater = RESET_VALUE;
//		bTachoOpen = RESET_VALUE;
		bNtcFailed = RESET_VALUE;
		bPressureFailure = RESET_VALUE;
		//uiSilkTumble = RESET_VALUE;
		bHeaterOpen = RESET_VALUE;
		bWaterFillStage = RESET_VALUE;
		bTwoLitreReached = RESET_VALUE;
//		uiMotorTriacState = RESET_VALUE;
//		cMotorTriacCount = RESET_VALUE;
//		cTriacCountSeconds = RESET_VALUE;
		buzzerTone = RESET_VALUE;
		uiOverFlowOccurs = RESET_VALUE;
		uidelaymin = RESET_VALUE;
		bChildLockButton = RESET_VALUE;
		uiCurrent_step_time = uicycle_mincompleted = RESET_VALUE;
		step_start = FALSE;
		uidisp_select_option = uidisp_select_option & (~DelayStart_Yes);
		bTimeSaver_Start = bDelay_Start1  = bProgram_Start= bPrewash_Start= bSteam_Start = FALSE; 
		bDelay_Finished = bDelay_Start = bSoak_Start = bHotRinse_Start = bExtraRinse_Start = FALSE;
		
		bFoamCOuntStart = RESET_VALUE;
		uiFoamState = RESET_VALUE;
		uiFoamSensingCount = RESET_VALUE;
		uiDrainTimer = RESET_VALUE;
		bDrainTimerStart = RESET_VALUE;
		
		bFoamDetection = FALSE;
		ucCount = RESET_VALUE;
		
		bHeatCompleted = RESET_VALUE;
		spin_first_press = RESET_VALUE;
		temp_first_press = RESET_VALUE;
		bWaterOverFLow = RESET_VALUE;
		bUnbalanceError = RESET_VALUE;
		bDoorOpenError = FALSE;
		bRapidWashButton = FALSE;
		uiDoorLOckReleaseSeconds = RESET_VALUE;
		bEnduranceStarted = RESET_VALUE;
		bProgram_End = FALSE;
		uiFillingState = RESET_VALUE;
		uiRinseFillState = RESET_VALUE;
		uiHeatState = RESET_VALUE;
		uiDrainState = RESET_VALUE;
		uiTumbleState = RESET_VALUE;
		uiover_heat = FALSE;
		bOverHeat = FALSE;
		PressureSwitchMinutes = 120;
		HeatMinutes = 120;
//		uiMotorFailState = RESET_VALUE;
		//uiMotorCount = RESET_VALUE;
		//uiMotorDrainMinutes = RESET_VALUE;
		bLevelReached = FALSE;
		uihigh_low_vtlg = RESET_VALUE;
		bEcoLevelReached = FALSE;
		bSteam_Filling_Start = FALSE;
		uiSteamFill_Seconds = RESET_VALUE;
		bDoorOpenError = RESET_VALUE;
		uiLowPressureTime = RESET_VALUE;
		uiLowPressureState = RESET_VALUE;
		uiNoWaterTime = RESET_VALUE;
		uiNoWaterState = RESET_VALUE;
		bNoWater = FALSE;
		bEco60Reset = FALSE;
		bSpin_Status = FALSE;
//		cMotor_Tumble_State = RESET_VALUE;
		uiAdditional_Drain_TurnOn_Mins = RESET_VALUE;
		uiAdditional_Heater_TurnOn_Mins = RESET_VALUE;
		bRinsehold_Finished = FALSE;
		bDoorLockError = FALSE;
		uiDoorUnlockPulseCount = RESET_VALUE;
		uiHeaterCheck = RESET_VALUE;
		bCoolDown_Process_Start = NO;
		b2min_Drin_On = FALSE;
		uiCoolDown_Seconds = RESET_VALUE;
		//cStepState = RESET_VALUE;
		SFState = RESET_VALUE;
		bUnbalance_Detected = FALSE;
		uiDrainOffSeconds = 10;
		bCommunicationError = FALSE;
		Step_pos= FALSE;
	 	bMotorError_E1 = FALSE;
	 	bMotorError_E2 = FALSE;
	 	bMotorError_E3 = FALSE;
		bMotorError_Unbalnce = FALSE;
		bUnbalanceError = FALSE;
		snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT;
		
		bHPrs = bLPrs = FALSE;
		uiFinalWt = RESET_VALUE;
		uiLoadRange = RESET_VALUE;
		uiLS_Total_subtraction = RESET_VALUE;
		uiLS_Tx_subtraction = RESET_VALUE;
		LS_flag = RESET_VALUE;
		L175_count = RESET_VALUE;
		//uiResetFill_Count = RESET_VALUE;
		uiheat_Minutes_Completed = RESET_VALUE;
		uiheat_Seconds_Completed = RESET_VALUE;
		bLoadSensing_Completed = RESET_VALUE;
#if ENABLE_IOT
		ProgramTime = 0;
		ProgressTime = 0;
		reset_IOTparam();
#endif
		if((uiExtraRinse_Press_Select > 0)&&((uilast_selected_option & ExtraRinse_Yes) == ExtraRinse_Yes))
			uiExtraRinse_Press_count = uiExtraRinse_Press_Select;
		else
		uiExtraRinse_Press_count = uiExtraRinse_Press_Select = 0;
}

/*************************************************************************************************************
* Function Name: PressureSwitchFailure
* Description  : This function is to detect the Pressure Sensor Failure.
* Arguments    : None
* Return Value : None
**************************************************************************************************************/
void PressureSwitchFailure()
{
	if(((((bSpinStarted == FALSE)||(uiMachineTestState != step5))&&(iWaterLevel >= 43200))||(iWaterLevel < 35000))&&(uiRPMValue < 65))
	//if((((((bSpinStarted == FALSE)||(uiMachineTestState != step5))&&(iWaterLevel >= 43200))||(iWaterLevel < 35000))||(uiResetFill_Count > 15))&&(uiRPMValue < 65))
	{
		if(uiRecheck_Seconds == RESET_VALUE)
		{		
				MotorStop();
				bHeater =SWITCH_OFF ;
				bMainWashValve = SWITCH_OFF;
				bPrewashValve = SWITCH_OFF;
				bMotor_Start = NO;
				bPressureError = 1;
				uiMachineState = PRESSUREERROR;
				MachineErrors.EBits.PressureError = 1;
				//PressureSwitchMinutes = 120;
		}
		else
		{
				bRecheck_Count_Start = YES;
				if(iWaterLevel >= 43200)
				{
					bHPrs = TRUE;
				}
				else if(iWaterLevel <= 35000)
				{
					bLPrs = TRUE; // disable 
				}

//				if(uiResetFill_Count>15)
//				{
//					bHPrs = bLPrs = FALSE;
//				}
		}
	}
	else
	{
		bRecheck_Count_Start = NO;
		PressureSwitchMinutes = 120;
		//uiRecheck_Seconds = 90;
		bHPrs = FALSE;
		bLPrs = FALSE;
	}
	
}


/*************************************************************************************************************
* Function Name: HeaterFailure
* Description  : This function is to detect the different Heater Failure condition.
* Arguments    : None
* Return Value : None
**************************************************************************************************************/
void HeaterFailure()
{
	if((uiTemperature > 91)&&(uiTemperature < 105)&&(bFaciaTest == FALSE)&&(bNtcFailed == FALSE))
	{
		if(uiHRecheck_Seconds == RESET_VALUE)
		{
			MotorStop();
			ComponentSwitchOff();
			bMotor_Start = NO;
			HeatMinutes = 120;
			uiMachineState = HEATERROR;
			bWaterLevel_Read_Required = 1;  
			MachineErrors.EBits.HeaterError = 1;
			uiover_heat = TRUE;
		}
		else
		{
			bHRecheck_Count_Start = YES;
			uiover_heat = FALSE;
		}
	}
	else if((uiTemperature > 59)&&(uiTemperature < 105)&&(bFaciaTest == TRUE)&&(bNtcFailed == FALSE))
	{
		if(uiHRecheck_Seconds == RESET_VALUE)
		{
			MotorStop();
			ComponentSwitchOff();
			bMotor_Start = NO;
			HeatMinutes = 120;
			uiMachineState = HEATERROR;
			bWaterLevel_Read_Required = 1;  
			MachineErrors.EBits.HeaterError = 1;
			uiover_heat = TRUE;
		}
		else
		{
			bHRecheck_Count_Start = YES;
			uiover_heat = FALSE;
		}
	}
	else if((uiTemperature <= 5) ||(uiTemperature > 105))
	{
		if(uiHRecheck_Seconds == RESET_VALUE)
		{
			if((bTestMode_Selection == TRUE)&&((uiTestModeState == SF01)||(uiTestModeState == SF02)))
			{
				MachineErrors.EBits.HeaterError = 1;
			}
			else
			{
				MachineErrors.EBits.HeaterError = 0;
			}
			bNtcFailed = TRUE;	
		}
		else
		{
			bHRecheck_Count_Start = YES;
		}
		
	}
	else
	{
		bHRecheck_Count_Start = NO;
		bNtcFailed = FALSE;
	}
}


/*************************************************************************************************************
* Function Name: MachineStartUp
* Description  : This function is to give initial Startup time to controller.
* Arguments    : None
* Return Value : None
**************************************************************************************************************/
void MachineStartUp()
{	
	if(uiStartUpdelay >= 8)
	{
		if(uiStartUpCount < 8)
		{
			uiStartUpCount ++;
			uiStartUpdelay = RESET_VALUE;
		}
		else
		{	
			uiStartUpdelay = RESET_VALUE;
		}
	}
	else
	{
		uiStartUpdelay++;
	}
	
}


/*************************************************************************************************************
* Function Name: MachineStartState
* Description  : This function is to give initial state to controller.
* Arguments    : None
* Return Value : None
**************************************************************************************************************/
void MachineStartState()
{
	
	 if (uiStartUpCount > 4)
	{
		 if((bTestMode_Selection == TRUE)&&(uiProgram_Position != 0))
		{
			uiMachineState = TESTMODE;
			uiSelectedTone = KEYPRESSTONE;
			bIdle = FALSE;
		}
		else if((uiProgram_Position != 0)&&(bTestMode_Selection == FALSE))
		{
			buzzerTone7 = RESET_VALUE;
			uiMachineState = IDLE_START;
		}
		if(uiProgram_Position == RESET_VALUE)
		{
			uiProgram_Position = 1;
			uiMachineState = IDLE_START;
			ProgramTimming();
		}
			
		//bStartButton = RESET_VALUE;
	}
	bWaterOverFLow = FALSE;
	bProgramStart = FALSE;
	
}



/*************************************************************************************************************
* Function Name: PauseState
* Description  : This function is to perform Pause opeartion.
* Arguments    : None
* Return Value : None
**************************************************************************************************************/
void PauseState()
{
	static uint16_t uiEmergencyConditionCheck;
	
	SetWashOperationLast30sDrain_state_ui8(LOAD_OPERATION);
	ComponentSwitchOff();
	SpinProfileReset();
	bTestSpinValueRead = RESET_VALUE;
	MotorStop();
	uiLowPressureTime = RESET_VALUE;
	uiLowPressureState = RESET_VALUE;
	uiNoWaterTime = RESET_VALUE;
	uiNoWaterState = RESET_VALUE;
	bNoWater = FALSE;
	uiDoor_Pulse_Sec = RESET_VALUE;	
	//bDoorPulse = FALSE;
	//bDoorLocked = FALSE;
	bWashProgram_Selected = TRUE;
	uiLS_Seconds = 240;//180;//120;
	bLoadSensing_Completed = RESET_VALUE;
	LS_flag = FALSE;
	if(uiDrainState ==STAE_ERROR)
	{
		//bStop_Minutes_Count = NO;
		uiDrainState = STATE_DRAIN;
	}
	
	if(bDoorOpenError == TRUE)
	{
		MachineErrors.EBits.DoorError = 1;
	}
	if(bEmegency_door_pull == FALSE)
	{
		if((iWaterLevel > ON_PAUSE_DOOR_OPEN_WATERLEVEL)&&(uiTemperature < ON_PAUSE_DOOR_OPEN_TEMPERATURE)&&(uiRPMValue == RESET_VALUE)&&(bZcdDetection == TRUE))
		{				
			if((bPowerOff_Door == FALSE)&&(bDoorOpenError == FALSE))
			{
				bDoorLock = SWITCH_OFF;
			}
			bDoorPulse = FALSE;
			bDoorLocked = FALSE;
			bDoorUnlocked = TRUE;
			uiEmergencySeconds = RESET_VALUE;
			uiEmergencyConditionCheck = RESET_VALUE;
			bEmegency_Lever_pull = RESET_VALUE;
		}
		else if((bDoorUnlocked == FALSE)&&(bZcdDetection == TRUE))
		{	
			bDoorLock = SWITCH_ON;
			bDoorUnlocked = FALSE;
			if((bDoorLockStatus == FALSE)&&(bDoorOpenError == FALSE)&&(bPowerOff_Door == FALSE)&&(bZcdDetection == TRUE))
			{
				if(uiEmergencyConditionCheck > 65000)
				{
					bEmegency_Lever_pull = TRUE;
					if(uiEmergencySeconds >= 3)
					{
						Emergency_DoorOpen();
					}
				}
				else
				{
					uiEmergencyConditionCheck++;
				}
			}
			else if((bDoorLockStatus == TRUE)&&(bDoorOpenError == FALSE)&&(bPowerOff_Door == FALSE)&&(bEmegency_Lever_pull == TRUE)&&(bZcdDetection == TRUE))
			{
				bStartButton = TRUE;
				uiEmergencyConditionCheck = RESET_VALUE;
				Emergency_DoorOpen();
			}
			else
			{
				uiEmergencySeconds = RESET_VALUE;
				uiEmergencyConditionCheck = RESET_VALUE;
				bEmegency_Lever_pull = RESET_VALUE;
			}
		}
	}
	else
	{
		if(((bDoorLockStatus == TRUE)||(bPhysical_DoorLock_Status == FALSE))&&(bDoorOpenError == FALSE)&&(bPowerOff_Door == FALSE)&&(bEmegency_Lever_pull == TRUE))
		{
			bStartButton = TRUE;
			Emergency_DoorOpen();
		}
	}
	
	uiNextStep_Start_Delay = 3;
	Programs();
	if((bStartButton == STARTPRESSED)&&(bEmegency_Lever_pull == FALSE))
	{
		if(mybuzzerdelay == 0)
		{
			if(uiBeforeState ==  PAUSE_PRESSED)
			{
				
				uiMachineState = START_PRESSED;
				
			}
			uiDoorPulseCount = RESET_VALUE;
			bDoorUnlocked = FALSE;
			if(bEmegency_Lever_pull == FALSE)
			{
				uiSelectedTone = STARTPRESSTONE;
			}
		}
	}
	bProgramStart = TRUE;
}


/*************************************************************************************************************
* Function Name: ComponentSwitchOff1
* Description  : This function is to cut the Loads off during Pause/Reset condition.
* Arguments    : None
* Return Value : None
**************************************************************************************************************/
void ComponentSwitchOff1(void)
{
	if(bDrainPump == SWITCH_OFF)
	{
		DRAINPUMP_OFF;
	}
	if(bHeater == SWITCH_OFF)
	{
		HEATER_OFF;
	}
	if(bPrewashValve == SWITCH_OFF)
	{
		PREWASH_VALVE_OFF;
	}
	if(bMainWashValve == SWITCH_OFF)
	{
		MAINWASH_VALVE_OFF;
	}
	if(bCirculation_Pump == SWITCH_OFF)
	{
		C_B_PUMP_OFF;
	}
}


/*************************************************************************************************************
* Function Name: ComponentSwitchOff
* Description  : This function is to cut the Loads off during Pause/Reset condition.
* Arguments    : None
* Return Value : None
**************************************************************************************************************/
void ComponentSwitchOff(void)
{
	if(bEmegency_door_pull == FALSE)
	{
		bDrainPump = SWITCH_OFF;
	}
	bHeater = SWITCH_OFF;
	bPrewashValve = SWITCH_OFF;
	bCirculation_Pump = SWITCH_OFF;
	bMainWashValve = SWITCH_OFF;
}


/*************************************************************************************************************
* Function Name: SpinProfileReset
* Description  : This function is to reset all spin parameters for machine Poweroff/Reset condition.
* Arguments    : None
* Return Value : None
**************************************************************************************************************/
void SpinProfileReset()
{
	
	//cSpinState = RESET_VALUE;
	//bStart_Traic_Fire = FALSE;
	//bSpin_Start = RESET_VALUE;
	//bSpinTumble = FALSE;
	//uiUnbalance_Value = RESET_VALUE;
	//cRedistribution_Count = RESET_VALUE;
	//cSpinProfileIndex = RESET_VALUE;
	//cFoamState = RESET_VALUE;
	//FoamReturn = RESET_VALUE;
	//bFoamCOuntStart = NO;
	//cFoamSensingCount = RESET_VALUE;
	if((uiAdd == RESET_VALUE)&&(bSpinStarted == TRUE)&&(uiMachineState == PAUSE_PRESSED)&&(bSpin_Status == FALSE)&&(bZcdDetection == TRUE))
	{
		uiAdd ++;
		BalanceTime = (BalanceTime + uiTime_Minutes_Completed);
		uiCurrent_step_time = uiCurrent_step_time + uiTime_Minutes_Completed;
		uicycle_mincompleted = uicycle_mincompleted - uiTime_Minutes_Completed;
		uiTime_Minutes_Completed = uiStep_Minutes_Completed = RESET_VALUE;
	}
	//iMotor_Spinning_Seconds = RESET_VALUE;
	//cFSISState = RESET_VALUE;
}


/*************************************************************************************************************
* Function Name: ProgramEnd
* Description  : This function is to perform Machine End state opeartion.
* Arguments    : None
* Return Value : None
**************************************************************************************************************/
void ProgramEnd()
{
	bDebugMode_Selection = FALSE;
	b_option_key_pressed_flag = FALSE;
	WashOperationLast30sDrain_state_ui8 = RESET_VALUE; // Added to reset the state.
	CP_State = RESET_VALUE;
	setAIComm_State_ui8(AI_Idle);
	PressureSwitchMinutes=120;
	//bProgramReset = FALSE;
	bDoorPulse = FALSE;
	bUnbalance_Detected = FALSE;
	uiOverFlowOccurs = RESET_VALUE;
	bDrainPump = SWITCH_OFF;
	bHeater = SWITCH_OFF;
	uiCurrent_step_time = RESET_VALUE;
	uiReheatTime = RESET_VALUE;
	//uiSilkTumble = RESET_VALUE;
	bEco60Reset = FALSE;
	MachineState_LED(FALSE,FALSE,FALSE,FALSE);
	turnon_display_OxyJetled(OXYJET_OFF); // TODO: Turn OFF OxyJet Led at Program End
	MotorStop();
	//bProgramStart = FALSE;
	bDelay_Finished = FALSE;
	bStop_Minutes_Count = YES;
	uiWaterResetCount = RESET_VALUE;
	uiPrewashWaterResetCount = RESET_VALUE; //TODO: Added To reset the prewash reset count after end of the program. to avoid overflow
	if((uiNonFatal_Error == HIGH_UNBALANCE)||( bUnbalanceError == TRUE))
	{
		if(buzzerTone12 == FALSE)
		{
			uiSelectedTone = ERRORTONE;
			buzzerTone12 = TRUE;
		}
	}
	if((bChildLockButton == 0)&&(uiRPMValue == RESET_VALUE))
	{
		bDoorLock = SWITCH_OFF;
		if(bDoorLockStatus == TRUE)
		{
			
			BalanceTime = 1;
			snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT;
			
			buzzerTone6 = RESET_VALUE;		
			prev_program_pos = uiProgram_Position;
			
			if((bDisplay_Flag == TRUE)&&( MachineErrors.EReg == FALSE))
			{
				mystrcpy((char *)cdigits_data_array, "0001");
				update_display_data(&cdigits_data_array[0]);				
				bDisplay_Flag = FALSE;
				//cdigits_data[4] =  cdigits_data[4] & (~0x10); // //turn off the time left led
				cdigits_data[5] =  cdigits_data[5] &  (~0x40);//neo changes for comma
			}
			
			if(uiProgramCount == RESET_VALUE)
			{	
				ProgramLifeCycle++;
				uiProgramCount++;
				
				if(ProgramLifeCycle < 40)
				{
					uitubclean_cycle = (uint8_t)ProgramLifeCycle;
				}
				else
				{
					uitubclean_cycle ++;
				}
				
				if((uitubclean_cycle % 40) == 0x00)
				{
					uiTubClean_req = TRUE;
					uitubclean_cycle = FALSE;
#if ENABLE_IOT
					set_IOTMultiFuncSetting(TUBCLEAN_REMINDER);
					BkMultiFuncSetting |= TUBCLEAN_REMINDER;
#endif
				}
				if((TubClean_Activated == TRUE)&&(uiTubClean_req == TRUE))
				{
					uitubclean_cycle = FALSE;
					uiTubClean_req = FALSE;
					uiTubClean_Disp_count = RESET_VALUE;
#if ENABLE_IOT
					clr_IOTMultiFuncSetting(TUBCLEAN_REMINDER);
					BkMultiFuncSetting &= ~TUBCLEAN_REMINDER;
#endif
				}
				if(uiTubClean_req == TRUE)
				{
					uiTubClean_Disp_count++;
					uiTubClean_Disp = TRUE;
					 uiTubclean_MSG_TMOUT_Count = 0x00;
					if(uiTubClean_Disp_count > 7)
					{
						uiTubClean_req = FALSE ; 
						uiTubClean_Disp_count = FALSE;
						uiTubClean_Disp = FALSE;
#if ENABLE_IOT
					clr_IOTMultiFuncSetting(TUBCLEAN_REMINDER);
					BkMultiFuncSetting &= ~TUBCLEAN_REMINDER;
#endif
					}
					 
				}
				   
				if(ProgramLifeCycle > MAX_CYCLE)
				{
					
					ProgramLifeCycle = RESET_VALUE;
				}
			}
			
		}
		else if((bDoorLockStatus == FALSE)&&(bDoorLockError == FALSE))
		{
			
			uicycle_mincompleted = RESET_VALUE;
			step_start = FALSE;
			bTimeSaver_Start = bDelay_Start1 = bPrewash_Start = FALSE;
			bSoak_Start = bHotRinse_Start = bExtraRinse_Start = bSteam_Start = FALSE;
			uiRinseholdState = RESET_VALUE;
			bRinsehold_Finished = FALSE;
			spin_first_press = RESET_VALUE;
			temp_first_press = RESET_VALUE;
			bTESTMODE_START = FALSE;
			bSpin_Available = TRUE;
			if(buzzerTone6 == FALSE)
			{
				prev_program_pos = uiProgram_Position;	//added for safety to display end properly if door gets opened during ramp down-- production issue
				if(mybuzzerdelay == 0)
				{
					uiSelectedTone = ENDTONE;
					buzzerTone6 = TRUE;
				}
				//buzzerTone6 = TRUE;
				if(bDisplay_Flag == TRUE)
				{
					mystrcpy((char *)cdigits_data_array, " End");
					
					update_display_data(&cdigits_data_array[0]);
				}
				bProgram_End = TRUE;
				bSnooze_Flag = FALSE;
#if ENABLE_IOT
				uiMachineState = PROGRAM_END;//IDLE_START
				BalanceTime = 0;
				ProgressTime = 0;
#endif
				if(MachineErrors.EReg == 0) 
				{
					snoozemode_10minutes_counter = 1000;
				}	
			}
			
			buzzerTone13 = RESET_VALUE;
			
			if((uiNonFatal_Error == HIGH_UNBALANCE)||( bUnbalanceError == TRUE))
			{
				 MachineErrors.EBits.HighUnbalance = 1;
				  bUnbalanceError = TRUE;
				 if(cFlash_Count == RESET_VALUE)
				 {
				 	cFlash_Count = 4;
				 }
			}
			else
			{
				MachineErrors.EBits.HighUnbalance = 0;
				 bUnbalanceError = FALSE;
				if(cFlash_Count == RESET_VALUE)
				 {
					cFlash_Count = 3;
				 }
			}
			uiProgramCount = RESET_VALUE;
			if(bDisplay_Flag == TRUE)
			{
				bProgram_End = TRUE;
				if((MachineErrors.EReg == 8)&&(cFlash_Count == 1) )// high unbalance
				{
					//mystrcpy((char *)cdigits_data_array, "UnbL");
					mystrcpy((char *)cdigits_data_array, " UbL");
					update_display_data(&cdigits_data_array[0] );
				}
				else 
				{
					mystrcpy((char *)cdigits_data_array, " End");
				
					ckey_digit_data[0] = ckey_digit_data[0] & ~0x01 ; // turn off the time dots
				}
				cdigits_data[0] = cdigits_data[1]= cdigits_data[2]= cdigits_data[3]= cdigits_data[4]/*= cdigits_data[5]*/= 0x00; //turn off the all led //commented cdigits[5] for top LEDs flickering issue
				// cdigits_data[4] =  cdigits_data[4] | (0x02); ////turn on the doorlock led
				ckey_digit_data[0] = ckey_digit_data[1] = ckey_digit_data[2]= ckey_digit_data[3] = ckey_digit_data[4] /*= ckey_digit_data[5]*/ = RESET_VALUE; // clear the  option led //commented ckey_digit_data[5] for program LEDs flickering issue
				//cdigits_data[4] =  cdigits_data[4] & (~0x10); 		//  // turn off the time left led
				cdigits_data[5] =  cdigits_data[5] &  (~0x40);//neo changes for comma
				if((bDoorLockError == FALSE))
					cdigits_data[4] =  cdigits_data[4] | (0x02); ////turn on the doorlock led
				
				update_display_data(&cdigits_data_array[0] );
				turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
				//ClearOpt_InError();    // clear all selected navigation/mark of option
				bDisplay_Flag = FALSE;
			}
			if((uiProgram_Position != prev_program_pos)||(cSwitch_Pressed == START_KEY) )
			{
				uikey_debounce_delay = 20;
				bStartButton = FALSE;
				bUnbalanceError = FALSE;
				bMotorError_Unbalnce = FALSE;
				uichild_flash_count = RESET_VALUE;
				MachineErrors.EReg = RESET_VALUE;
				bProgram_End = FALSE;
				bDebugMode_Selection = FALSE;
				uiPresent_Step = RESET_VALUE;
				if(bEmegency_door_pull == TRUE)
				{
					uiSelected_Position = uiProgram_Position = prev_program_pos;
					prev_program_pos = RESET_VALUE;
					
				}
				else
				{
					uiSelected_Position = uiProgram_Position = prev_program_pos;
					prev_program_pos = RESET_VALUE;
				}
				
				//uiReset_TempRPM = TRUE;
				if(uiExtraRinse_Press_Select >0)
				{
					uiExtraRinse_Press_count = uiExtraRinse_Press_Select;
				}
					
				ProgSel_led();
				//ProgramTimming();
				//RESET_Prog();
				//TurnOnSelected_Option();
				uiMachineState = PROGRAM_RESET;//STARTUP;
				snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT;
			}	
				
		}
	}
	else if((bChildLockButton == TRUE)&&(uiRPMValue == RESET_VALUE))
	{		
		bDoorLock = SWITCH_ON;
		prev_program_pos = uiSelected_Position;
		if(bDisplay_Flag == TRUE)
		{
			if((MachineErrors.EReg == 8)&&(cFlash_Count == 1) )// high unbalance
			{
				//mystrcpy((char *)cdigits_data_array, "UnbL");
				mystrcpy((char *)cdigits_data_array, " UbL");
			}
			else 
			{
				mystrcpy((char *)cdigits_data_array, " End");
				cdigits_data[5] =  cdigits_data[5] &  (~0x40); ; // // turn off the time dots
				bSnooze_Flag = RESET_VALUE;
#if ENABLE_IOT				
				set_IOTState(STATE_IOT_END);
#endif				
			}
			cdigits_data[0]= cdigits_data[1]= cdigits_data[2]= cdigits_data[3]= cdigits_data[4]/*= cdigits_data[5]*/= 0x00; //turn off the all led //commented cdigits[5] for top LEDs flickering issue
			turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
			if((bChildLockButton == 0)&& (bDoorLockError == FALSE))
				cdigits_data[4] =  cdigits_data[4] | (0x02); ////turn on the doorlock led
				
			if(buzzerTone13 == FALSE)
			{
				if(mybuzzerdelay == 0)
				{
					uiSelectedTone = PAUSETONE;
					buzzerTone13 = TRUE;
				}
			}
			//cdigits_data[4] =  cdigits_data[4] & (~0x10); //  // turn off the time left led
			cdigits_data[5] =  cdigits_data[5] &  (~0x40);//neo changes for comma
			//cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
			update_display_data(&cdigits_data_array[0] );
			snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT;
			bDisplay_Flag = FALSE;
		}
	}	
		
}



/*************************************************************************************************************
* Function Name: Emergency_DoorOpen
* Description  : This function is to perform Emergency Door opeartion.
* Arguments    : None
* Return Value : None
**************************************************************************************************************/
void Emergency_DoorOpen()
{
	if((bDoorLockStatus == FALSE)&&(ui_prev_door_status == TRUE)&&(bDoorLock == TRUE)&&(bEmegency_door_pull == FALSE)&&(bPhysical_DoorLock_Status  == TRUE)&&(bZcdDetection == TRUE))
	{
		if(uiEmergencySeconds == 4)
		{
			DOORLOCK_ON;
			Delay_ms(20);
			DOORLOCK_OFF;
			bEmegency_door_pull = TRUE;
		}
	}
	else if(((bDoorLockStatus == TRUE)||(bPhysical_DoorLock_Status == RESET_VALUE))&&(ui_prev_door_status == TRUE)&&(bDoorLock == TRUE)&&(bZcdDetection == TRUE))
	{
		bEmegency_door_pull = TRUE;
		bDebugMode_Selection = FALSE;
		
		if((iWaterLevel > EMPTYLEVEL)&&(uiTemperature < ON_PAUSE_DOOR_OPEN_TEMPERATURE)&&(uiRPMValue == RESET_VALUE)&&(bZcdDetection == TRUE))
		{
			ui_prev_door_status = bDoorLock = FALSE;
			bDrainPump = SWITCH_OFF;
			
			
			bDoorPulse = FALSE;
			prev_program_pos = uiProgram_Position;
			
			uiBefore_Snooze = uiMachineState = PROGRAM_END;
			uiMachineBefore_Poweroff = uiMachineState;
			uiEmergencySeconds = RESET_VALUE;
			bEmegency_Lever_pull = RESET_VALUE;
			MotorStop();
			//SpinProfileReset();
			StopAllEvents();
			snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT;
			buzzerTone6 = RESET_VALUE;			
		}
		else
		{
			cFlash_Count = 1;
			uiDISP_MCount = 10;
			if(iWaterLevel < EMPTYLEVEL)
			{	
				bDrainPump = SWITCH_ON;
			}
			else if(iWaterLevel >= (EMPTYLEVEL+80))
			{
				bDrainPump = SWITCH_OFF;
			}
			 if(bDisplay_Flag == TRUE)
			 {
				turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
				
				cdigits_data[0]= cdigits_data[1]= cdigits_data[2]= cdigits_data[3]= cdigits_data[4]= cdigits_data[5]= 0x00; //turn off the all led 
					
				//mystrcpy((char *)cdigits_data_array, "StoP");
				mystrcpy((char *)cdigits_data_array, " StP");
				cdigits_data[5] =  cdigits_data[5] &  (~0x40); ; // // turn off the time dots
				//cdigits_data[4] =  cdigits_data[4] & (~0x10); //  // turn off the time left led
				cdigits_data[5] =  cdigits_data[5] &  (~0x40);//neo changes for comma
				//cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
				update_display_data(&cdigits_data_array[0] );
				snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT;
				bDisplay_Flag = FALSE;
			}
		}
	}
	else
	{
		bEmegency_door_pull = FALSE;
		ui_prev_door_status = FALSE;
	}
	
}


/*************************************************************************************************************
* Function Name: DoorCondition_Check
* Description  : This function is to perform door open safety opeartion after program reset.
* Arguments    : None
* Return Value : boolean
**************************************************************************************************************/
boolean DoorCondition_Check()
{
	uint8_t result;
	
	if((iWaterLevel >= EMPTYLEVEL1)&&(uiRPMValue == RESET_VALUE))
	{
		bDoorLock = SWITCH_OFF;
		bEmptyLevelReached = TRUE;
		bDoorPulse = FALSE;
		bPowerOff_Door = FALSE;
		bDrainPump = SWITCH_OFF;
		result = TRUE;
	}
	else if(bEmptyLevelReached == FALSE)
	{	
		//bDoorLock = SWITCH_ON;
			
		if((iWaterLevel < EMPTYLEVEL1)&&(iWaterLevel > 35000))
		{
			bDrainPump = SWITCH_ON;
		}
		else
		{
			bDrainPump = SWITCH_OFF;
		}
		result = FALSE;
	}
	return result;
}


/*************************************************************************************************************
* Function Name: DoorError_Check
* Description  : This function is to detect Door error condition.
* Arguments    : None
* Return Value : None
**************************************************************************************************************/
void DoorError_Check()
{
	//if((uiDoor_Pulse_Sec == 3)&&(uiDoorPulseCount < 2)&&(bDoorOpenError == FALSE))
	if((bPhysical_DoorLock_Status == TRUE)&&(bZcdDetection == TRUE))
	{
		if((uiDoor_Pulse_Sec >= 4)&&(uiDoorPulseCount < 2))
		{
			DOORLOCK_ON;
			Delay_ms(20);
			DOORLOCK_OFF;
			uiDoor_Pulse_Sec = RESET_VALUE;	
			uiDoorPulseCount ++;
		}
		/*else if(uiDoor_Pulse_Sec > 3)
		{
			if((uiMachineState == START_PRESSED)||(uiMachineState == MOTORFAILURE))
			{
				bStartButton = FALSE;
				uiMachineState = PAUSE_PRESSED;
			}
			bPowerOff_Door = TRUE;
		}*/
		//else if((uiDoorPulseCount == 2)||(bDoorOpenError == TRUE))
		else if(uiDoorPulseCount == 2)
		{
			MachineErrors.EBits.DoorError = 1;
			uiDoor_Pulse_Sec = RESET_VALUE;
			bDoorOpenError = TRUE;
			bPowerOff_Door = FALSE;
			if((uiMachineState == START_PRESSED)||(uiMachineState == MOTORFAILURE))
			{
				bStartButton = FALSE;
				uiMachineState = PAUSE_PRESSED;
			}
		}
	}
	else if(bZcdDetection == TRUE)
	{
		MachineErrors.EBits.DoorError = 1;
		uiDoor_Pulse_Sec = RESET_VALUE;
		bDoorOpenError = TRUE;
		bPowerOff_Door = FALSE;
		if((uiMachineState == START_PRESSED)||(uiMachineState == MOTORFAILURE))
		{
			bStartButton = FALSE;
			uiMachineState = PAUSE_PRESSED;
		}
	}
}


/************************************************************************************************************************
****************************************** END OF FILE *****************************************************************/
