 
#include "timings.h"
#include "r_cg_userdefine.h"
#include "STLED316S.h"
#include "r_cg_port.h"

//*************************************************************//
//********************global variables************************//
extern boolean bStartButton,bRecheck_Count_Start,bDoorLockStatus,bWaterLevel_Read_Required;//bExpress_30_Activatted;
extern boolean bStop_Minutes_Count,bMotor_Start,bHeater,bHalfMinute_Process_Start,bSpin_Start;
extern boolean bDrainPump,bDoorLock,bCoolDown_Process_Start,bDoorLocked,bCommunicationError,bFCTMode_Selection;
extern boolean bNoWater,bDebugMode_Selection,bUnbalance_Detected,bZcdDetection,b2min_Drin_On;
extern boolean bProgramStart,bTestMode_Selection,bDelay_Start,bHRecheck_Count_Start;
extern boolean buzzerTone,buzzerTone1,buzzerTone2,bBuzzerTime_Start,bFoamDetection;
extern boolean buzzerTone3,buzzerTone4,buzzerTone5,buzzerTone12,bFoamCOuntStart,bSteam_Filling_Start;
extern boolean buzzerTone7,buzzerTone8,Switch_Reset,bFaciaTest,bSpinStarted,bZCD_Display;
extern boolean TubClean_Activated,bDrainLevelReach,bSpecialEndurance_Finished,bSpin_Status;
extern boolean bDrumHalt,bDoorGone,bEmegency_Lever_pull,bPhysical_DoorLock_Status;


extern boolean Foam_Removal_Enable,bDrainTimerStart,LS_flag,bLoadSensing_Completed;
extern uint8_t Foam_Removal_Complete;


extern uint8_t uiHeater_TurnOn_Delay_Value,uiDrain_Off_Seconds;
extern uint8_t uiCoolDown_Seconds,uiNextStep_Start_Delay,uiSteamFill_Seconds,uiFoamDrain_Seconds,uiFoamDrain_TotalSeconds,uiDrainTimer;
extern uint8_t uiError_Mode,uiLowPressureTime,uiNoWaterTime;
extern uint8_t uiExtraTime_Seconds,uiHalfMinutes_Seconds,uiStep_Seconds,uiStep_Minutes_Completed,uiheat_Minutes_Completed,uiheat_Seconds_Completed;
extern uint8_t uiHalfMinute_Completed,uiAdditional_Heater_TurnOn_Mins,uiAdditional_Drain_TurnOn_Mins;
extern uint8_t uiDelay_Seconds,uiPresent_Step,uiBeforeState;
extern uint8_t uiDrainState,uiMachineState,uiMachineTestState;
extern uint8_t uiTestModeState;
extern uint8_t uiLowPressureState,uiNoWaterState,uiSelected_Position,uiMachineTestState;

extern uint8_t PressureSwitchMinutes;
extern uint8_t HeatMinutes;
extern uint8_t uiDoorLOckReleaseSeconds;
extern uint8_t uiReheatTime,uiWaterOverFlowSeconds;
extern uint8_t uiDrainOffSeconds,uiTestModeTumble;
extern uint8_t uiRecheck_Seconds;
extern uint8_t SatrtUpDelay,uiTumblingCount,uiTempSel;
//extern uint8_t uiHRecheck_Seconds,uiSleepModeSeconds,uiSilkTumble,uiSelectedTone;
extern uint8_t uiHRecheck_Seconds,uiSleepModeSeconds,uiSilkTumble;
extern volatile uint8_t uiSelectedTone;


extern uint16_t uidelaymin,BalanceTime,uiRPMValue; 
extern uint16_t uiCurrent_step_time,uiSupplyVoltage;

extern uint16_t iWaterLevel,iMotor_Spinning_Seconds;
extern uint16_t iMilliSeconds,uiFoamSensingCount;

extern uint8_t uiTemperature,uiSelected_Position,uiSmartProgram_Selected,uiAISelected_Position;

//**************************************************************//
//********************  Local variables ************************//
boolean bDoorLockError;

uint8_t uiTime_Minutes_Completed;
uint8_t uiTime_Seconds,uiSilkTumble;
uint8_t uihigh_low_vtlg,uiDoorUnlockPulseCount;
uint8_t uiSleepModeSeconds,uiDrain_2min_On;
uint8_t uiDoor_Pulse_Sec;
uint8_t uiLowCount,uiDoorLockFalseCount,uiStateCheck;
uint8_t IB_Val_missing_count,uiEmergencySeconds;
uint8_t high_vtg_count,low_vtg_count;
uint8_t uiLS_Seconds = 240;//180;

uint16_t input3,input4,ucMotor_Rotate_Seconds;
uint16_t uicycle_mincompleted,cErrorBuzzerSeconds;
uint16_t IB_Value;

//******** global function prototype ******//
extern void TemperatureCalculation(int,uint8_t *);
//extern void MotorValues_Reset(void);
extern void MotorStop();
extern void PressureSwitchFailure();

//******** local function prototype ********//
void MinutesCalculation(void);
void DelayCheck(void);
void ErrorModeTimings(void);
void MotorTimings(void);
void Averaging(void);
void ComponentSwitchOff(void);
void ErrorHandling();

#if ENABLE_IOT
#include "iot_config.h"
extern uint8_t WiFi_Comn_Test,WiFi_FCT_Comn_Test;
 uint8_t WiFi_Comn_Wait;
#endif

/***********************************************************************************************************************
* Function Name: time
* Description  : This function implements Timming releated operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void time(void)
{
	if(iMilliSeconds >= MILLI_THOUSAND)
 	{
    		iMilliSeconds = RESET_VALUE;
		
		//LS step timer; new taken since existing timer is dependent on motor fault from bottom PB
		if((bLoadSensing_Completed == FALSE) && (LS_flag == TRUE) && (uiLS_Seconds > RESET_VALUE))
		{
			uiLS_Seconds--;
		}
		if(((uiRPMValue < 5)||(bSpinStarted == TRUE)))
		{
			bWaterLevel_Read_Required = TRUE;
		}
		else
		{
			bWaterLevel_Read_Required = FALSE;
		}
	
		if(uiMachineState == SNOOZEMODE)
		{
			uiSleepModeSeconds++;
			if(uiSleepModeSeconds < 6)
			{
				START_LED_OFF;
			}
			else if(uiSleepModeSeconds < 11)
			{
				START_LED_OFF;
			}
			else
			{
				uiSleepModeSeconds = 1;
			}
		}
		else
		{
			uiSleepModeSeconds = 0;
		}
   	    	if((bStartButton == STARTPRESSED))//&&(Switch_Reset == TRUE))
    		{
			 if((bDoorLockStatus == TRUE)||(bSpecialEndurance_Finished == TRUE))
			{
				if(uiNextStep_Start_Delay > RESET_VALUE)
				{
					if((uiRPMValue == RESET_VALUE)&&(bZCD_Display == TRUE))
					{
						uiNextStep_Start_Delay --;
						bMotor_Start = NO;
					}
					iWaterLevel = iWaterLevel;
					bWaterLevel_Read_Required = FALSE;
					
				}
				else 
				{
					//MinutesCalculation();
					
					if((uidelaymin == RESET_VALUE)&&(bUnbalance_Detected ==FALSE)&&(bFoamDetection == FALSE))
					{
						MinutesCalculation();	
					}
					if((uidelaymin == RESET_VALUE)&&(BalanceTime > RESET_VALUE)&&(bUnbalance_Detected ==FALSE)&&(bFoamDetection == FALSE)) //if((uidelaymin == RESET_VALUE)&&(bTestMode_Selection == FALSE)&&(BalanceTime > RESET_VALUE)&&(cRedistribution_Count == RESET_VALUE))
					{
						TimeCalculation();
					} 
					else if((bDebugMode_Selection ==FALSE)&&(uidelaymin > RESET_VALUE))
					{
						HoursCalculation();
					}
				}
				uiDoor_Pulse_Sec = RESET_VALUE;
			}
			
			
			if(((bDoorLockStatus == FALSE)&&(bDoorLocked == FALSE)&&(bTestMode_Selection== FALSE))
			||((bDoorLockStatus == FALSE)&&((bFaciaTest == TRUE)||(uiTestModeState !=SF01)||(uiTestModeState !=SF02)))
			&& (bPhysical_DoorLock_Status == TRUE))
			{
				uiDoor_Pulse_Sec ++;
			}
			
			if((uiHeater_TurnOn_Delay_Value > RESET_VALUE)&&(uiNextStep_Start_Delay == RESET_VALUE))
			{
			 	uiHeater_TurnOn_Delay_Value--;
			}
			
			if(bSteam_Filling_Start == YES)
			{
				uiSteamFill_Seconds++;
			}
			else
			{
				uiSteamFill_Seconds = RESET_VALUE;
			}
			////////////////////////////////////////////////////////////////
			// for express spin drain & spin
			if((Foam_Removal_Enable==1)&&(uiSelected_Position == EXPRESS)&&(uiPresent_Step >= 15) /*&&(bExpress_30_Activatted == FALSE)*/)			//second time control	
			{
				uiFoamDrain_Seconds++;	
				uiFoamDrain_TotalSeconds++;
			}
			else
			{
				uiFoamDrain_Seconds = RESET_VALUE;
				uiFoamDrain_TotalSeconds = RESET_VALUE;
			}
			////////////////////////////////////////////////////////////////
			 if(bCoolDown_Process_Start == YES)
			{
				uiCoolDown_Seconds++;
			}
			else
			{
				uiCoolDown_Seconds = RESET_VALUE;
			}
			 if(bFoamCOuntStart == YES)
			 {
				 uiFoamSensingCount++;
			 }
			 else
			 {
				uiFoamSensingCount = RESET_VALUE; 
			 }
			 if(bDrainTimerStart == YES)
			 {
				uiDrainTimer++; 
			 }
			 else
			 {
				uiDrainTimer = RESET_VALUE;  
			 }
			 if(b2min_Drin_On == TRUE)
			 {
				 uiDrain_2min_On ++;
			 }
			 else
			 {
				 uiDrain_2min_On = RESET_VALUE;
			 }
			 
			if((bDrainLevelReach == TRUE)&&(uiDrainOffSeconds > RESET_VALUE))
			{
				if((iWaterLevel < 43000)&&(uiMachineTestState < step6))
				{
					bDrainPump = SWITCH_ON;
				}
				else
				{
					bDrainPump = SWITCH_OFF;
				}
				
				uiDrainOffSeconds --;
			}
			 
			
			  if(bBuzzerTime_Start == YES)
			 { 
				 cErrorBuzzerSeconds++;
			 }
			 else
			 {
				 cErrorBuzzerSeconds = RESET_VALUE;
			 }
			 if(bEmegency_Lever_pull == TRUE)
			 {
				 uiEmergencySeconds ++;
			 }
			 else
			 {
				 uiEmergencySeconds = RESET_VALUE;
			 }
#if ENABLE_IOT
			 if((WiFi_Comn_Test != 0))
			 {
				 WiFi_Comn_Test --;
			 }			 
#endif
				 
		}
		else
		{
			//uiDoor_Lock_sec = DOORLOCK_WAIT_SECONDS;
			uiNextStep_Start_Delay = NEXTSTEP_DELAY;
			if((bEmegency_Lever_pull == TRUE)&&(uiMachineState == PAUSE_PRESSED)&&(bDoorLock == TRUE))
			 {
				 uiEmergencySeconds ++;
			 }
			 else
			 {
				 uiEmergencySeconds = RESET_VALUE;
			 }
#if ENABLE_IOT
			 if((bFCTMode_Selection != TRUE)&&(WiFi_Comn_Test != 0))
			 {				
				WiFi_Comn_Test --;					 
			 }
#endif
		}
		
		DelayCheck();
		Countings();
		DoorLockRelease();
		if((bDoorGone == TRUE)&&(bDoorLockStatus == TRUE))
		 {
			 uiDoorLockFalseCount ++;
		 }
		 else
		 {
			 uiDoorLockFalseCount = RESET_VALUE;
		 }
		 
		if(SatrtUpDelay > RESET_VALUE)
		{
			SatrtUpDelay--;
		}
		if((uiStateCheck > RESET_VALUE)&&(SatrtUpDelay == RESET_VALUE)&&(bZcdDetection == TRUE))
		{
			uiStateCheck --;
		}
#if ENABLE_IOT
//		if((WiFi_Comn_Test != 0)&&(bFCTMode_Selection == TRUE))
//		{
//			WiFi_Comn_Test --;
//		}
		if((WiFi_FCT_Comn_Test != 0)&&(bFCTMode_Selection == TRUE))
		{
			WiFi_FCT_Comn_Test --;
		}
#endif
		if(uihigh_low_vtlg == 0x01)
		{
			if(high_vtg_count >= 1)
			{
				high_vtg_count = RESET_VALUE;
			}
			else
			{
				high_vtg_count++;
			}
		}
		else
		{
			high_vtg_count = RESET_VALUE;
		}
		if(uihigh_low_vtlg == 0x02)
		{
			if(low_vtg_count >= 1)
			{
				low_vtg_count = RESET_VALUE;
			}
			else
			{
				low_vtg_count++;
			}
		}
		else
		{
			low_vtg_count = RESET_VALUE;
		}
		
 	}
}



/***********************************************************************************************************************
* Function Name: MinutesCalculation
* Description  : This function implements Time Calculation Minutewise.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void MinutesCalculation(void)
{
	if(bHalfMinute_Process_Start == YES)
	{
		uiHalfMinutes_Seconds++;
		if(uiHalfMinutes_Seconds > HALF_MINUTE_SECONDS)
		{
			uiHalfMinutes_Seconds = RESET_VALUE;
			uiHalfMinute_Completed ++;	
		}	
		uiStep_Seconds = RESET_VALUE;
	}
	else if(bStop_Minutes_Count == TRUE)
	{
		if(uiError_Mode !=  NO_ERROR)
		{
			uiExtraTime_Seconds++;
			if(uiExtraTime_Seconds > ONE_MINUTE_SECONDS)
			{
				uiExtraTime_Seconds = RESET_VALUE;
				ErrorModeTimings();
			}
			if(uiDrainState == STATE_DRAIN_IDLE)
			{
				uiExtraTime_Seconds = RESET_VALUE;
				if(uiDrain_Off_Seconds >RESET_VALUE)
				{
					uiDrain_Off_Seconds--;	
				}
			}
		}
	}
	else if(bHalfMinute_Process_Start == NO)
	{
		uiStep_Seconds ++;
		uiheat_Seconds_Completed++;
		if(uiStep_Seconds > ONE_MINUTE_SECONDS)
		{
	    		uiStep_Seconds = RESET_VALUE;
			uiheat_Seconds_Completed = RESET_VALUE;
			uiStep_Minutes_Completed++;
			uiheat_Minutes_Completed++;
			if((bHeater == SWITCH_ON)&&(((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == ANTIALLERGEN))||(TubClean_Activated == TRUE)))
			{
				uiReheatTime++;
			}
			uiExtraTime_Seconds = RESET_VALUE;	
		}
	}
	//MotorTimings();
}


/***********************************************************************************************************************
* Function Name: DelayCheck
* Description  : This function implements Error Persisting condition.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void DelayCheck(void)
{
	if(bHRecheck_Count_Start == YES)
    	{
		if(uiHRecheck_Seconds > RESET_VALUE)
		{
			uiHRecheck_Seconds --;
		}
	}
	else
	{
		uiHRecheck_Seconds = 3;
	}
	
	if((bRecheck_Count_Start == YES) && ((uiMachineState == TESTMODE) || (uiMachineState == START_PRESSED)||(uiMachineState == MOTORFAILURE)))
    	{
		if(uiRecheck_Seconds > RESET_VALUE)
		{
			uiRecheck_Seconds --;
			//bDrainPump = SWITCH_OFF;
		}
	}
	else
	{
		/*if(iWaterLevel > 42910)
		{
			uiRecheck_Seconds = 35;
		}
		else if (iWaterLevel < 35300)
		{
			uiRecheck_Seconds = 9;
		}
		else if ((iWaterLevel < 35300) && ((uiMachineState != TESTMODE) || (uiTestModeState == 5)))
		{
			uiRecheck_Seconds = 35;//12;//60; // value changed due to error came in between
		}
		else if((iWaterLevel < 35300) && (uiMachineState == TESTMODE)) // value changed due to error came in between
		{
			uiRecheck_Seconds = 3;//100; // value changed due to error came in between
		}*/
		//to fix the hprs error occurence
		//80-86 seconds delay added before declaration of error
		if((iWaterLevel > 42510)&&(uiMachineState == TESTMODE)&&(uiMachineTestState >= step4))//below condition added as per PR 78793 //HPRS DECLARATION IN TEST MODE @ LP stage
		{
			uiRecheck_Seconds = 15;
		}
		else if(iWaterLevel > 42510)
		{
			uiRecheck_Seconds = 90;//50;
		}
		else if((iWaterLevel < 42910)&& (iWaterLevel > 35300))
		{
			uiRecheck_Seconds = 90;//36;
			if((uiMachineState == TESTMODE)&&(uiMachineTestState >= step4))
			{
				uiRecheck_Seconds = 15;
			}
		}
		else if (iWaterLevel < 35300)
		{
			uiRecheck_Seconds = 36;//9; //existing hprs in test mode 50 sec : prog mode 50 sec // lprs TM 36 sec : PM 36 sec
						// new HPRS TM 15 SEC : PM 90 SEC //LPRS TM 36 SEC : PM 36 SEC
		}
		else if ((iWaterLevel < 35300) && ((uiMachineState != TESTMODE) || (uiTestModeState == 5)))
		{
			uiRecheck_Seconds = 35;//12;//60; // value changed due to error came in between
		}
		else if((iWaterLevel < 35300) && (uiMachineState == TESTMODE)) // value changed due to error came in between
		{
			uiRecheck_Seconds = 36;//3;//100; // value changed due to error came in between
		}
		
	}
	
	if((iWaterLevel > 35300) && (iWaterLevel < DANGER_WATERLEVEL) &&((bProgramStart == TRUE)||(bTestMode_Selection == TRUE)) && ((uiMachineState == TESTMODE) || (uiMachineState == START_PRESSED)||(uiMachineState == MOTORFAILURE)))
	{
		
	   	if(uiWaterOverFlowSeconds > RESET_VALUE)
	   	{
		   	uiWaterOverFlowSeconds--;
	   	}
		else
		{
			uiMachineState = WATERERROR;
			if((bTestMode_Selection == TRUE)&&((uiTestModeState == SF01)||(uiTestModeState == SF02)))
			{
				MachineErrors.EBits.PressureError = 1;
			}
		}
	}
	else
	{
		uiWaterOverFlowSeconds = 6;
		MachineErrors.EBits.PressureError = 0;
	}
	
	if(cErrorBuzzerSeconds == 301)
	{
		buzzerTone=buzzerTone1=buzzerTone2= RESET_VALUE;
		buzzerTone3=buzzerTone4=buzzerTone5=buzzerTone12= RESET_VALUE;
		buzzerTone7=buzzerTone8=RESET_VALUE;
	}
	else if(cErrorBuzzerSeconds > 301)
	{
		cErrorBuzzerSeconds = RESET_VALUE;
	}
}


/***********************************************************************************************************************
* Function Name: ErrorModeTimings
* Description  : This function implements Error Condition Timming related operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void ErrorModeTimings(void)
{
		if(uiError_Mode == LOW_PRESSURE_CONDITION)
		{
			uiLowPressureTime++;
			uiNoWaterTime =RESET_VALUE;
			uiAdditional_Heater_TurnOn_Mins = RESET_VALUE;
			uiAdditional_Drain_TurnOn_Mins = RESET_VALUE;
			uiDrainState = RESET_VALUE;
			bNoWater = 0;
		}
		else if(uiError_Mode == NO_WATER_CONDITION)
		{
			uiNoWaterTime ++;
			uiLowPressureTime = RESET_VALUE;
			uiAdditional_Heater_TurnOn_Mins = RESET_VALUE;
			uiAdditional_Drain_TurnOn_Mins = RESET_VALUE;
			uiDrainState = RESET_VALUE;
		}
		else if(uiError_Mode == TEMPEATURE_NOT_ATTAINED)
		{
			uiAdditional_Heater_TurnOn_Mins++;
			uiNoWaterTime =RESET_VALUE;
			uiLowPressureTime = RESET_VALUE;
			uiAdditional_Drain_TurnOn_Mins = RESET_VALUE;
			uiLowPressureState = RESET_VALUE;
			uiNoWaterState = RESET_VALUE;
			uiDrainState = RESET_VALUE;
		}
		else if(uiError_Mode == EMPTYLEVEL_NOT_ATTAINED)
		{
			uiAdditional_Drain_TurnOn_Mins++;
			uiNoWaterTime =RESET_VALUE;
			uiAdditional_Heater_TurnOn_Mins = RESET_VALUE;
			uiLowPressureTime = RESET_VALUE;
			uiLowPressureState = RESET_VALUE;
			uiNoWaterState = RESET_VALUE;
			
		}
		else
		{
			uiAdditional_Drain_TurnOn_Mins = RESET_VALUE;
			uiNoWaterTime =RESET_VALUE;
			uiAdditional_Heater_TurnOn_Mins = RESET_VALUE;
			uiLowPressureTime = RESET_VALUE;
			uiLowPressureState = RESET_VALUE;
			uiNoWaterState = RESET_VALUE;
			uiDrainState = RESET_VALUE;
		}
}

/***********************************************************************************************************************
* Function Name: MotorTimings
* Description  : This function implements Motor Timming calculation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
/*
void MotorTimings(void)
{
	if( (bMotor_Start == YES) && (bSpin_Start == NO)&&(uiMachineState != MOTORFAILURE))
	{
		ucMotor_Rotate_Seconds++;
	}
	else if(bSpin_Start == YES)
	{
		if(iMotor_Spinning_Seconds > RESET_VALUE)
		{
			iMotor_Spinning_Seconds --;
		}
	}
	
}*/

/***********************************************************************************************************************
* Function Name: Averaging
* Description  : This function implements ADC input averaging & HIGH/Low voltage errorr condition.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Averaging(void)
{
		static uint8_t uiVoltageErrorCheck;
		
		TemperatureCalculation(input3 >>5 , &uiTemperature);
		input3 = RESET_VALUE;
		
		IB_Value = input4 >> 5;		
		if(IB_Value <230)
		{
			IB_Val_missing_count++;
			if((IB_Val_missing_count >5)&&((uiMachineState == TESTMODE)||(uiMachineState == FCTMODE)))
			{
				uiSelectedTone = ERRORTONE;
			}
		}
		else
		{
			IB_Val_missing_count = RESET_VALUE;
		}
	 	//cSamples_index = RESET_VALUE;
		
		if(bZcdDetection == TRUE)
		{
			
			 if((uiSupplyVoltage >= SUPPLY270)&&(uiMachineState !=STARTUP)&&(uiMachineState != FCTMODE)&&(uiMachineState != POWEROFF)&&(bSpin_Status== FALSE))
			 {
				if(uiVoltageErrorCheck > 10)//23
				{
					uiMachineState = HIGHLOWVOLTAGE;
					uiBeforeState = uiMachineState;
					MachineErrors.EBits.HeaterError = 0;
					uihigh_low_vtlg = 0x01;
					uiVoltageErrorCheck = RESET_VALUE;
				}
				else
				{
					uiVoltageErrorCheck++;
				}
				 
			 }
			 else if((uiSupplyVoltage <= SUPPLY150)&&(uiMachineState !=STARTUP)/*&&(uiMachineState !=SNOOZEMODE)*/&&(uiMachineState != FCTMODE)&&(uiMachineState != POWEROFF))
			 {
				//  if(uiLowCount > 2)
				if(uiVoltageErrorCheck > 10)//23) 23-- 4sec delay, 10-- 2 sec delay 
				{
					if(uiRPMValue < 15)
					 {
						 uiMachineState = HIGHLOWVOLTAGE;
						 uiLowCount = RESET_VALUE;
						 MachineErrors.EBits.HeaterError = 0;
						 uihigh_low_vtlg = 0x02;
					 }
					 else if((uiRPMValue > 20)&&(uiSupplyVoltage <= 141))
					 {
						 uiMachineState = HIGHLOWVOLTAGE;
						 uiLowCount = RESET_VALUE;
						 MachineErrors.EBits.HeaterError = 0;
						 uihigh_low_vtlg = 0x02;
					 }
					 uiVoltageErrorCheck = RESET_VALUE;
				}
				else
				{
					uiVoltageErrorCheck++;
				}
			 }
			 else
			 {
				 uiVoltageErrorCheck = RESET_VALUE;
			 }
		}
		else
		{
			uiVoltageErrorCheck = RESET_VALUE;
		}
		
		
		 //---------------- HEATER TRAP ------------------------------------------
	/*	if(bTemperatureRise_Flag == TRUE)
		{
			bTemperatureRise_Flag = FALSE;
			//if(bHeater == SWITCH_ON) // this will check if only if heater is on
			{
				uiCurrent_temperature = (uint8_t)uiTemperature;
				
				if(uiCurrent_temperature ==  uiPrev_temperature)
				{
					uiPrev_temperature = uiCurrent_temperature;
					uiDryHeat_Count = RESET_VALUE;
				}
				else if(uiCurrent_temperature > uiPrev_temperature)
				{
					uiDryHeat_Count++;
					if(uiDryHeat_Count > 5)
					{
						uiDryHeat_Count = RESET_VALUE;
						bHeater = SWITCH_OFF;
					}
					uiPrev_temperature = uiCurrent_temperature;
				}
			}	
			
		}*/ 
		//-------------  END OF HEATER TRAP LOGIC -----------------------------
}


/***********************************************************************************************************************
* Function Name: DoorLockRelease
* Description  : This function implements DoorUnlocked error condition.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void DoorLockRelease()
{
	//if((bDoorLock == SWITCH_OFF)&&(bPhysical_DoorLock_Status  == TRUE)&&(bZcdDetection == TRUE)&&(bCommunicationError == FALSE))
	if((bDoorLock == SWITCH_OFF)&&(bZcdDetection == TRUE)&&(bCommunicationError == FALSE))
	{
		if((bDoorLockStatus == TRUE)||(bDoorLockError == TRUE))
		{
			uiDoorLOckReleaseSeconds++;
		}
		else
		{
			bDoorLockError = FALSE;
			uiDoorUnlockPulseCount = RESET_VALUE;
			uiDoorLOckReleaseSeconds = RESET_VALUE;
			MachineErrors.EBits.DoorError = 0;
		}
		if((uiDoorLOckReleaseSeconds >= 4)&&(uiDoorUnlockPulseCount < 2)&&(bDoorLockError == FALSE))
		{
			DOORLOCK_ON;
			Delay_ms(20);
			DOORLOCK_OFF;
			uiDoorLOckReleaseSeconds = RESET_VALUE;	
			uiDoorUnlockPulseCount ++;
		}
		else if (bDoorLockError == TRUE)
		{
			MachineErrors.EBits.DoorError = 1;
			MachineErrors.EBits.HighUnbalance = 0;
		}
		else if(uiDoorUnlockPulseCount >= 2)
		{
			bDoorLockError = TRUE;
			MachineErrors.EBits.DoorError = 1;
			MachineErrors.EBits.HighUnbalance = 0;
		}
	}
	else if(bZcdDetection == TRUE)
	{
		//bDoorLockError = FALSE;
		uiDoorUnlockPulseCount = RESET_VALUE;
		//MachineErrors.EBits.DoorError = 0;
		//uiDoorLOckReleaseSeconds = RESET_VALUE;
	}
}


/***********************************************************************************************************************
* Function Name: Countings
* Description  : This function implements Pressure & Heater error drain opeartion .
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Countings()
{
	if((uiMachineState ==PRESSUREERROR) && (PressureSwitchMinutes > RESET_VALUE) &&(bDoorLockStatus == TRUE) &&(bStartButton == STARTPRESSED))
	{
		PressureSwitchMinutes--;
	}
	if((uiMachineState == HEATERROR)&& (HeatMinutes > RESET_VALUE)&&(bDoorLockStatus == TRUE)&&(bStartButton == STARTPRESSED))
	{
		HeatMinutes--;
	}
}


/***********************************************************************************************************************
* Function Name: HoursCalculation
* Description  : This function implements Delay Time releated operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void HoursCalculation()
{
	if(bDelay_Start == YES)
	{
		uiDelay_Seconds ++;
	
		if(uiDelay_Seconds > ONE_MINUTE_SECONDS)
		{
	    		uiDelay_Seconds = RESET_VALUE;
			uidelaymin--; // decrement of start delay min 
			
		}
	}	
	else
	{
		uiDelay_Seconds = RESET_VALUE;
		bDelay_Start = FALSE;
	}
}


/***********************************************************************************************************************
* Function Name: TimeCalculation
* Description  : This function implements program time calculation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void TimeCalculation()
{
	if((((bHalfMinute_Process_Start == YES)||(bHalfMinute_Process_Start == NO))&&(bStop_Minutes_Count == NO))||(bSpinStarted == YES))
	{
		uiTime_Seconds++;
		if(uiTime_Seconds > ONE_MINUTE_SECONDS)
		{
			uiTime_Seconds = RESET_VALUE;
			uiTime_Minutes_Completed++;
			if(MachineErrors.EReg == 0)
			{		
				if((uiCurrent_step_time > RESET_VALUE)&&(BalanceTime > TRUE))
				{
					if(uiTumblingCount == INITIAL_DRAIN ||(uiTumblingCount == COOL_DOWN))//||((uiSelected_Position == MIXEDSOIL)&&(uiTumblingCount == SOAK)&&((uiPresent_Step == 10)||(uiPresent_Step == 12)||(uiPresent_Step == 14)))) //TODO: Added for hold the timing at the phase of CoolDown & Start of drain
					{
						;
					}
					else if(((uiSelected_Position == MIXEDSOIL)||(uiAISelected_Position == MIXEDSOIL))&&(uiTumblingCount == SOAK)&&(uiPresent_Step <15))//TODO: Added for hold the timing at the phase of Soak in Mixed as per latest specs 
					{
						;
					}
//					else if((uiSelected_Position == WETCLEAN)&&(uiTumblingCount == SOAK))//TODO: Added for hold the timing at the phase of Soak in Wet Clean
//					{
//						;
//					}
					else if(((uiSelected_Position != EXPRESS)
						&&(/*(uiSelected_Position == SMARTPLUS)&&*/(uiSmartProgram_Selected != STEAMWASH))						)
						||(/*(bExpress_30_Activatted == FALSE)&&*/(uiSelected_Position == EXPRESS)&&((uiPresent_Step != 15)&&(uiPresent_Step != 10)&&(uiPresent_Step != 12)))
						||((/*(uiSelected_Position == SMARTPLUS)&&*/(uiSmartProgram_Selected == STEAMWASH /*|| bExpress_30_Activatted == TRUE*/))&&((uiPresent_Step != 14)&&(uiPresent_Step != 10)&&(uiPresent_Step != 7)&&(uiPresent_Step != 1)))	//present step dependencies for drain step of steamwash corrected 			
						)
					{
						
						BalanceTime = (BalanceTime - 1);	
					
						/*if(((uiTumblingCount !=COOL_DOWN)&&(uiTempSel != Wash95))
						||(((TubClean_Activated == TRUE)||(uiSelected_Position == COTTON))
							&&(uiTempSel == Wash95)))*/
						
						/*if(((uiTumblingCount != COOL_DOWN)
							&&(uiTempSel != Wash95) )
							||((TubClean_Activated == TRUE) &&(uiTempSel == Wash95)))*/
						{
							uicycle_mincompleted = (uicycle_mincompleted + 1) ;
						}
						
					}
					uiCurrent_step_time = (uiCurrent_step_time - 1);
				}
				else
				{
					uiCurrent_step_time = RESET_VALUE;
				}
				
				
			}
			
		}
	}
	
}

