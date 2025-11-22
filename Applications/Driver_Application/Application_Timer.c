#include "Application_Timer.h"
#include "r_cg_timer.h"
#include "keyevent.h"
#include "STLED316S.h"
#include "Selection.h"
#include "WashOperations_Global.h"
//***********************Gobal Function*************************//
extern void Store_errors(uint8_t );
extern void time(void);
extern void Time_DoorTriacCheck();
extern void ProgramSelector(void);
extern void MachineStartUp(void);
extern void ComponentSwitchOff1(void);
extern void Reinit_Stleddevice(void);
extern void Alldispled_oFF(void);
extern void SendCommand(void);
extern void DataRecieved();
extern void Application_Interrupt2();
//***************************************************************
//******************* Gobal Variables ***************************

TimerObjectType TimerObject_Init[NO_USER_TIMER] = {
				{AI200Sec_TIMER,		320,		0,	FALSE},
				{AI_1Sec_Interveval_TMER, 	1,		0,	FALSE},
				{EXTRA_DRAIN_1MIN_TIMER,	0,		0,	FALSE},
				{AI_DATA_RecieveTimeout_Status,	20,		0,	FALSE}
};
uint32_t CPTimeInSecCounter_ui32;
uint8_t Timer_CountdownTimer(uint8_t option, uint8_t name);
uint8_t CP20MsTo1MsCounter_ui8,MS_count_key;
extern boolean CpOffTimer_b,CpOnTimer_b;
extern boolean bDoorLockStatus,bTestMode_Selection,Switch_Reset,bPhysical_DoorLock_Status;
extern boolean bStartButton,bChildLockButton,bBuzzer_On;
extern boolean bDoorOpenError,bSpin_Start,bKeyDetected;
extern boolean bFCTMode_Selection,bZcdDetection,bCommunicationError;
extern boolean bBuzzerStart,bPressureError,bWaterOverFLow,bNoWater;
extern boolean buzzerTone11,buzzerTone10,bComm_MB,bMotor_Start;
extern boolean buzzerTone,buzzerTone1,buzzerTone2,bMotorError_E1;
extern boolean buzzerTone3,buzzerTone4,buzzerTone5,buzzerTone6;
extern boolean buzzerTone7,buzzerTone8,bMotorError_E3,bMotorError_E2;
extern boolean bDebugMode_Selection,uiover_heat,bDoorLockError,bDataRecieved;
extern boolean bNtcFailed,bPressureFailure,bSpinStarted,chip_select,bCirculation_Pump;
extern boolean bWaterLevel_Read_Required,bDisplay_Flag,bDelay_Finished,bZCD_Display;

extern boolean bHPrs,bLPrs;

extern boolean bDisp_write; // added for neo

extern boolean bExtraRinse_Flag; //AK

extern uint8_t uiMachineState,uiProgram_Position;
extern uint8_t uiError_Occured;
extern uint8_t cFlash_Count,uiZcd_Count;
extern uint8_t uiNextStep_Start_Delay;
extern uint8_t cdigits_data_array[6],ckey_digit_data[6],cdigits_data[7];
extern uint8_t uiTone_Selected;
extern uint8_t UartRxBuf[30];
extern uint8_t high_vtg_count,low_vtg_count;

//extern uint8_t uiSelectedTone,uiDISP_MCount,uidisp_select_option;
extern uint8_t uiDISP_MCount,uidisp_select_option;
extern volatile uint8_t uiSelectedTone;
extern uint8_t uikey_debounce_delay,uiopt_check,sleep_mode,uiMotorError;
extern uint8_t uiLowCount,uioption_on,uiZCD_Missing_Count,uiStateCheck;
extern uint8_t uiZcdCheck,uiIDLE_COUNT,uiallon_count,uiprev_error;
extern uint8_t SatrtUpDelay,uiMachineStart_count,uimachine_Off,uiRESET;
extern uint8_t uihigh_low_vtlg,uimachine_shutdown,uisnooze_mode_pos;
extern uint8_t uLedKeyData[3],snooze_display,snoozestart_onoff,emergency_on;
extern uint8_t TEST_FCT_Active,temp_first_press,spin_first_press,power_count;

extern uint16_t snoozemode_10minutes_counter,uikeypress_20mscount,uiSupplyVoltage;
extern uint16_t PressoCount,FiveHundredMilli,BalanceTime;
extern uint16_t mybuzzerdelay,uiTriacDelay,uikeypress_20mscount,uiRPMValue;
extern uint16_t uizcd_usec_count,uiprev_key_data,uidelaymin;

extern uint16_t iWaterLevel,iWaterFreqSample;
extern uint16_t iMilliSeconds;

extern uint8_t uiTemperature,WiFi_Comn_Test,WiFi_FCT_Comn_Test;
extern int cSwitch_Pressed;
int key_value; //ak
//******************** Local Variables/Function *******************
boolean bBuzzerTime_Start,bkeys_display_flag,Send_flag;
uint16_t uidisplay_flash_count=0;
uint8_t uiSerialCount,uiZcd_Count_Check,uiData_Recieve_Count;
uint8_t Timer_CountdownTimer(uint8_t , uint8_t );
void Software_Timer20ms();
//uint8_t cLoadDelay,Display_Write_Flag;
uint8_t uiTimer500us,uiTimer1ms,uiTimer1msFlag,uiTimer2ms,uiTimer2msFlag;
extern uint16_t uiBuzzer_Delay,iWaterFreqSample;
extern uint16_t iWaterLevel;
uint16_t uiTimer1msFreq;
volatile uint16_t gu16WaterFreq;
uint8_t Freq_No_Read = 0x00;
void Frequency_Calculation();
void Error_Declaration();
//************************************************************************************
uint8_t AI_WaterFreqRead_b;
uint16_t AI_WaterFreqSample;
uint16_t AI_PressureCount;
uint16_t AI_WaterLevel;
boolean AI_WaterFreqGet_b;
extern uint16_t Display_off_testing;


#if ENABLE_IOT
#include "iot_config.h"
#include "iot_control.h"
extern boolean buzzerTone14;
#endif

/***********************************************************************************************************************
* Function Name: r_tau0_channel2_interrupt(20Msec)
* Description  : This function is INTTM02 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
 void Application_Timer0()
 {
	 static uint16_t  AI_Time_Value;
	 
	 if(bWaterLevel_Read_Required == YES)//&&(uiRPMValue == RESET_VALUE))
	 {
		if(++uiTimer1msFreq >= 998)//500	
		{	
			if(Freq_No_Read == 0x00)
			{
				
			uiTimer1msFreq = 0;
			//gu16WaterFreq = 0xFFFF - TCR00;
			gu16WaterFreq = 0xFFFF - TCR00;
			iWaterLevel = gu16WaterFreq;
			TT0 |= _0001_TAU_CH0_STOP_TRG_ON;
		
			TS0 |= _0001_TAU_CH0_START_TRG_ON;
			}else
			{
				Freq_No_Read = 0x00;
				uiTimer1msFreq = 0;
				TT0 |= _0001_TAU_CH0_STOP_TRG_ON;
		
				TS0 |= _0001_TAU_CH0_START_TRG_ON;
			}
		}
		
	 }
	 else{
		 Freq_No_Read = 0x01;
		uiTimer1msFreq = 0x00;
		 gu16WaterFreq = 0x00;
		TT0 |= _0001_TAU_CH0_STOP_TRG_ON;
	 }
	 if(AI_WaterFreqRead_b == TRUE)
	{
		AI_Time_Value = _1SEC_COUNT;
			
		if(AI_PressureCount >= AI_Time_Value)
		{
			AI_WaterLevel = AI_WaterFreqSample;
			AI_WaterFreqGet_b = TRUE;
			AI_WaterFreqSample = RESET_VALUE;
			AI_PressureCount = RESET_VALUE;
		}
		else
		{
			AI_PressureCount++;
		}
	}
	else
	{
		AI_WaterFreqSample = RESET_VALUE;
		AI_PressureCount = RESET_VALUE;
		AI_WaterLevel = iWaterLevel;
	}
 }
/***********************************************************************************************************************
* Function Name: r_tau0_channel2_interrupt(20Msec)
* Description  : This function is INTTM02 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Application_Timer2()
{
	static uint8_t ii,uiFeedBackSec;

	if(uiMachineStart_count >RESET_VALUE)
	{
		uiMachineStart_count--;
	}
	if(bZcdDetection == FALSE)
	{
		bMotor_Start = NO;
		//bWaterLevel_Read_Required = FALSE;
		uiStateCheck = 3;
		WiFi_Comn_Test = 15;
		WiFi_FCT_Comn_Test = 8;//6;
		uiNextStep_Start_Delay = 2;
		
	}
	MachineStartUp(); // StartUp Operation Machine

   //**************************************************************//
  //*************************************************************
  //**************** Display/Key detection Code ****************//
	if(uiDISP_MCount >= 1)
	{
		uiDISP_MCount--;
		//cdigits_data[5] =  cdigits_data[5] &  (~0x40); ; // // turn off the time dots
	}
	else
	{
		bKeyDetected = FALSE;
		temp_first_press = RESET_VALUE;
		spin_first_press = RESET_VALUE;
		bExtraRinse_Flag = RESET_VALUE; //AK
	}
	//if(TEST_FCT_Active >=1)
	TEST_FCT_Active--;
	
    //---------------------- Keys Detection --------------------------
	if(bStartButton == TRUE)
	{
		if(bZCD_Display == TRUE)
		{
		 STLED316_Read(0x42, &uLedKeyData[0], 3); // read the STLED reg data
		 cSwitch_Pressed = RESET_VALUE;
		 uLedKeyData[0] = 0x12 & uLedKeyData[0];
		 uLedKeyData[1] = 0x12 & uLedKeyData[1];//0x12 acc to data sheet rightshift values b0 & b3 we are using remaining 0
		 uLedKeyData[2] = 0x12 & uLedKeyData[2];
		if((uLedKeyData[2] != RESET_VALUE )|| (uLedKeyData[1] != RESET_VALUE) || (uLedKeyData[0] != RESET_VALUE))
		{
			cSwitch_Pressed = ((cSwitch_Pressed | uLedKeyData[0])<< 4);
			cSwitch_Pressed = ((cSwitch_Pressed | uLedKeyData[1])<< 4);
			cSwitch_Pressed = cSwitch_Pressed |uLedKeyData[2];		
		}
	#if ENABLE_IOT
		check_IOTRebootFlag();
	#endif
		
		if(POWER_TOUCH_KEY == 0)
		{
			cSwitch_Pressed = cSwitch_Pressed + POWER_KEY;
			//Delay_ms(10); //added for test purpose
		}
		if((cSwitch_Pressed != RESET_VALUE)&&(cSwitch_Pressed != 0x1332))
		{
			if(uiprev_key_data == RESET_VALUE)
			{
				if(uikey_debounce_delay >= 1)
				{
					uikey_debounce_delay--; //decrement of debounce time 
					//uikey_debounce_delay = RESET_VALUE;
				}
				
				uikeypress_20mscount++; 
				
				if(uikey_debounce_delay == RESET_VALUE)
				{
						if((uiMachineState == PROGRAM_END)&&(bDoorLockStatus == TRUE)&&(bChildLockButton == FALSE)&&(bDoorLockError == FALSE))
						{
							
							uiTone_Selected = 2;
							uikey_debounce_delay = 10;
							Key_Press_Tone();
							uiprev_key_data = cSwitch_Pressed ;
							
						}
						else
						{
							bKeyDetected = 1;
							//ButtonPressed();
							if((cSwitch_Pressed != SPIN_KEY)||(bExtraRinse_Flag == TRUE)||(uikeypress_20mscount > 100))
							{
								ButtonPressed();
								key_value = RESET_VALUE ;
							}
							else if(cSwitch_Pressed == SPIN_KEY)
							{
								key_value = cSwitch_Pressed ;
							}
								
							if(bKeyDetected == RESET_VALUE)
							{
								if((cSwitch_Pressed == SELECT_KEY)&&((bDebugMode_Selection == FALSE)&&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE)))
								{
									uiprev_key_data = RESET_VALUE ;
									uikey_debounce_delay = 10;
								}
								else
								{
									uiprev_key_data = cSwitch_Pressed ;
									uikey_debounce_delay = KEY_DEBOUNCE_TIME; // commented for time being 
								}
							}
						}
						if((uiMachineState == PROGRAM_END)||(uiMachineState == SNOOZEMODE))
						{
							//snoozemode_10minutes_counter = 1000;
						}
						else{ 
							
							snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT;
							uimachine_shutdown = RESET_VALUE;
						}
					}	
				}
				
	//			if(cFlash_Count == RESET_VALUE)
	//			{
	//				 for(ii = 0xC0; ii < 0xCB; ii= ii + 0x02)
	//				{
	//					STLED316_7Seg_Write(ii,ckey_digit_data[(ii-0xC0)/2]); // this is should not be 0x00 , default array have to written
	//				}
	//			}	
				chip_select = FALSE;
				
				if(bDisp_write == 0)
				{
					for(ii = 0xC0; ii < 0xCD; ii= ii + 0x02)
					{
						STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // update the display
					}
					STLED316_7Seg_Write(0XCD,ckey_digit_data[5]);
					for(ii = 0xC1; ii < 0xCA; ii= ii + 0x02)
					{
						STLED316_7Seg_Write(ii,ckey_digit_data[(ii-0xC1)/2]); // this is should not be 0x00 , default array have to written
						
					}
				}
				
				//chip_select = TRUE;
				//--------------------------
				
		}
		else if((key_value == SPIN_KEY)&&(bExtraRinse_Flag == FALSE)&&(cSwitch_Pressed != 0x1332))
		{
			cSwitch_Pressed = SPIN_KEY;
			ButtonPressed();
			key_value = RESET_VALUE;
		}
		else
		{
			//uiprev_key_data = cSwitch_Pressed;
			if(cSwitch_Pressed != 0x1332)
			{
				uiprev_key_data = cSwitch_Pressed;
			}
			uikey_debounce_delay = KEY_DEBOUNCE_TIME;
			
			if((cFlash_Count > 0) && (bTestMode_Selection == TRUE))
			{	
			}
			
			else if((bDisp_write ==0)&&(MachineErrors.EReg == 0)&&(uiMachineState !=PROGRAM_END)) //program end condition is added for "end" flashing -AK
			{
				//commented to avoid overlapping -- display going off
				//Uncommented for testing the display going off during debug mode
				for(ii = 0xC0; ii < 0xCD; ii= ii + 0x02)
				{
					STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // this is should not be 0x00 , default array have to written
					
				} 
				STLED316_7Seg_Write(0XCD,ckey_digit_data[5]);
				for(ii = 0xC1; ii < 0xCA; ii= ii + 0x02)
					{
						STLED316_7Seg_Write(ii,ckey_digit_data[(ii-0xC1)/2]); // this is should not be 0x00 , default array have to written
							
					}
			}
			
			uikeypress_20mscount = 0;
			if(MachineErrors.EReg == 0)
			{
				if(((uiMachineState != PAUSE_PRESSED)&&(bStartButton == FALSE)&&(bTestMode_Selection == FALSE))||((uiMachineState == PROGRAM_END)&&(bTestMode_Selection == FALSE))||((uiMachineState == SNOOZEMODE)))
				{
					if(MachineErrors.EReg == 0)
					{
						if(snoozemode_10minutes_counter > 0)
							snoozemode_10minutes_counter--;
							
						if(snoozemode_10minutes_counter == 0)
						{
							
							uiMachineState = SNOOZEMODE;
							uisnooze_mode_pos = uiProgram_Position;
							snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT; // reload the 10 minutes counter
							
						}	
					}
				}
			}
			uikey_debounce_delay = KEY_DEBOUNCE_TIME;
			bBuzzer_On = TRUE;
		}
		}
		//----------------------- Display update -------------------------------------------------------
		chip_select = FALSE; // for selecting the 1st STLED
		
		if(uiIDLE_COUNT > RESET_VALUE)
		{
			uiIDLE_COUNT--;
			if(uiMachineState == PAUSE_PRESSED)
			{
				uioption_on =   uioption_on | uiopt_check;
			}
			else
			{
				uioption_on =  uiopt_check;
			}
			if((uiMachineState == STARTUP)||((uiMachineState == IDLE_START)))
			{
				uioption_on =   uioption_on | uidisp_select_option;
			}
			if(uiIDLE_COUNT == RESET_VALUE)
			{
				if(uiMachineState == PAUSE_PRESSED)
				{
					uioption_on =  uioption_on &(~uiopt_check);
				}
				else
				{
					//uioption_on = RESET_VALUE;
					uioption_on =   uidisp_select_option;//uioption_on &(~uiopt_check);
				}
			}
		}
		else
		{
			uiopt_check = 0x08;//0x01;
		}	
		bkeys_display_flag = 1;
	}
	uidisplay_flash_count++;
//	bkeys_display_flag = 1;
	//KEYS_DISPLAY();
	
    //---------------------- END OF Keys  --------------------------
    //**************************************************************
    //*************** DoorLocked FeedBack Check **************//
	if(uiFeedBackSec >= 10)
	{
		uiFeedBackSec = RESET_VALUE;
		Time_DoorTriacCheck();
	}
	else
	{
		uiFeedBackSec++;
	}
	
	if(bDelay_Finished == TRUE)
	{
		uidelaymin = RESET_VALUE;
	}

	if(uiSerialCount >= 45)
	{
		uiSerialCount = 0;		
		Send_flag = TRUE;
		/*if((bDataRecieved == TRUE)&&(bComm_MB == TRUE))
		{
			
			bCommunicationError = FALSE;
			uiData_Recieve_Count = RESET_VALUE;
			//bDataRecieved = FALSE;
			
		}
		else*/
		{
			 if((UartRxBuf[0] != 0x80)||(bComm_MB == FALSE))
			 {
				if((uiMachineState !=PROGRAM_END)&&(uiMachineState !=SNOOZEMODE)&&(uiMachineState !=POWEROFF))
				{
					 if(uiData_Recieve_Count >= 17)
					 {
						 uiData_Recieve_Count = RESET_VALUE;
						bCommunicationError = TRUE;
						 MachineErrors.EBits.DoorError = 1;
					 }
					 else
					 {
						 uiData_Recieve_Count ++;
					 }
				 }
				else
				{
					uiData_Recieve_Count = RESET_VALUE;
				}
				
			}
			 else
			 {
				 if(bZcdDetection == TRUE)
				 bCommunicationError = FALSE;
				 UartRxBuf[0]= 0x00;
				 uiData_Recieve_Count = RESET_VALUE;
				 if((bDoorOpenError == FALSE)&&(bDoorLockError == FALSE))
				 MachineErrors.EBits.DoorError = RESET_VALUE;
			 }
		}
	}
	else
	{
		uiSerialCount++;
	}
	// *********************************************************
}
/***********************************************************************************************************************
* Function Name: r_tau0_channel1_interrupt(1 Msec)
* Description  : This function is INTTM01 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Application_Timer1()
{
	static uint8_t ii;
       static uint8_t rot_sec,ZcdSignal,uiDoorRelayOn;
    
     iMilliSeconds++;
    // Frequency_Calculation();
     if(bZcdDetection == FALSE)
     {
	     Display_off_testing++;
     }
	time(); // Time Calculation	

     if(sleep_mode>=1)
	 sleep_mode--;
     //**************************************
     //********** Door Protection Logic *************//
     if((P1.1 == 1)&&(uiDoorRelayOn < 28))
     {
	 uiDoorRelayOn++;
     }
     else if((P1.1 == 1)&&(uiDoorRelayOn >= 28))
     {
	     P1.1 = 0;
     }
     else
     {
	 uiDoorRelayOn = RESET_VALUE;
     }
	 
    //*************** Program Selection *********//
	if(rot_sec >= 5)
	{
		rot_sec = 0;
		ProgramSelector();
		ComponentSwitchOff1();// turn off the all actuators
	}
	else
	{
		rot_sec++;
	}
	
	//---------------------------------------------------
     //********* Buzzer Timmimng**************	
	if(bBuzzerStart == 1)
	{
   		mybuzzerdelay++;
	}
	else
	{
		mybuzzerdelay = 0;
	}
	//******************************
	
	
	//**********************************/
	if((ZcdSignal >=5)&&(bZcdDetection == TRUE))
	{
		ZcdSignal = RESET_VALUE;
		Application_Interrupt2();
	}
	else
	{
		ZcdSignal ++;
	}
	
	//DISPLAY TEST CHANGES///
	//if((bfillingDuty_enabled == FALSE)&&(LS_flag != TRUE))
	if(bStartButton == FALSE)
	{
		if(MS_count_key == 7)
		{
			MS_count_key = 0;
		if(bZCD_Display == TRUE)
		{
		 STLED316_Read(0x42, &uLedKeyData[0], 3); // read the STLED reg data
		 cSwitch_Pressed = RESET_VALUE;
		 uLedKeyData[0] = 0x12 & uLedKeyData[0];
		 uLedKeyData[1] = 0x12 & uLedKeyData[1];//0x12 acc to data sheet rightshift values b0 & b3 we are using remaining 0
		 uLedKeyData[2] = 0x12 & uLedKeyData[2];
		if((uLedKeyData[2] != RESET_VALUE )|| (uLedKeyData[1] != RESET_VALUE) || (uLedKeyData[0] != RESET_VALUE))
		{
			cSwitch_Pressed = ((cSwitch_Pressed | uLedKeyData[0])<< 4);
			cSwitch_Pressed = ((cSwitch_Pressed | uLedKeyData[1])<< 4);
			cSwitch_Pressed = cSwitch_Pressed |uLedKeyData[2];		
		}
	#if ENABLE_IOT
		check_IOTRebootFlag();
	#endif
		
		if(POWER_TOUCH_KEY == 0)
		{
			cSwitch_Pressed = cSwitch_Pressed + POWER_KEY;
			//Delay_ms(10); //added for test purpose
		}
		if((cSwitch_Pressed != RESET_VALUE)&&(cSwitch_Pressed != 0x1332))
		{
			if(uiprev_key_data == RESET_VALUE)
			{
				if(uikey_debounce_delay >= 1)
				{
					uikey_debounce_delay--; //decrement of debounce time 
					//uikey_debounce_delay = RESET_VALUE;
				}
				
				uikeypress_20mscount++; 
				
				if(uikey_debounce_delay == RESET_VALUE)
				{
						if((uiMachineState == PROGRAM_END)&&(bDoorLockStatus == TRUE)&&(bChildLockButton == FALSE)&&(bDoorLockError == FALSE))
						{
							
							uiTone_Selected = 2;
							uikey_debounce_delay = 10;
							Key_Press_Tone();
							uiprev_key_data = cSwitch_Pressed ;
							
						}
						else
						{
							bKeyDetected = 1;
							//ButtonPressed();
							if((cSwitch_Pressed != SPIN_KEY)||(bExtraRinse_Flag == TRUE)||(uikeypress_20mscount > 200)/*100*/)
							{
								ButtonPressed();
								key_value = RESET_VALUE ;
							}
							else if(cSwitch_Pressed == SPIN_KEY)
							{
								key_value = cSwitch_Pressed ;
							}
								
							if(bKeyDetected == RESET_VALUE)
							{
								if((cSwitch_Pressed == SELECT_KEY)&&((bDebugMode_Selection == FALSE)&&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE)))
								{
									uiprev_key_data = RESET_VALUE ;
									uikey_debounce_delay = 10;
								}
								else
								{
									uiprev_key_data = cSwitch_Pressed ;
									uikey_debounce_delay = KEY_DEBOUNCE_TIME; // commented for time being 
								}
							}
						}
						if((uiMachineState == PROGRAM_END)||(uiMachineState == SNOOZEMODE))
						{
							//snoozemode_10minutes_counter = 1000;
						}
						else{ 
							
							snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT;
							uimachine_shutdown = RESET_VALUE;
						}
					}	
				}
				
	//			if(cFlash_Count == RESET_VALUE)
	//			{
	//				 for(ii = 0xC0; ii < 0xCB; ii= ii + 0x02)
	//				{
	//					STLED316_7Seg_Write(ii,ckey_digit_data[(ii-0xC0)/2]); // this is should not be 0x00 , default array have to written
	//				}
	//			}	
				chip_select = FALSE;
				
				if(bDisp_write == 0)
				{
					for(ii = 0xC0; ii < 0xCD; ii= ii + 0x02)
					{
						STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // update the display
					}
					STLED316_7Seg_Write(0XCD,ckey_digit_data[5]);
					for(ii = 0xC1; ii < 0xCA; ii= ii + 0x02)
					{
						STLED316_7Seg_Write(ii,ckey_digit_data[(ii-0xC1)/2]); // this is should not be 0x00 , default array have to written
						
					}
				}
				
				//chip_select = TRUE;
				//--------------------------
				
		}
		else if((key_value == SPIN_KEY)&&(bExtraRinse_Flag == FALSE)&&(cSwitch_Pressed != 0x1332))
		{
			cSwitch_Pressed = SPIN_KEY;
			ButtonPressed();
			key_value = RESET_VALUE;
		}
		else
		{
			//uiprev_key_data = cSwitch_Pressed;
			if(cSwitch_Pressed != 0x1332)
			{
				uiprev_key_data = cSwitch_Pressed;
			}
			uikey_debounce_delay = KEY_DEBOUNCE_TIME;
			
			if((cFlash_Count > 0) && (bTestMode_Selection == TRUE))
			{	
			}
			
			else if((bDisp_write ==0)&&(MachineErrors.EReg == 0)&&(uiMachineState !=PROGRAM_END)) //program end condition is added for "end" flashing -AK
			{
				//commented to avoid overlapping -- display going off
				//uncommented to avoid encoder led skipping issue
				for(ii = 0xC0; ii < 0xCD; ii= ii + 0x02)
				{
					STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // this is should not be 0x00 , default array have to written
					
				} 
				STLED316_7Seg_Write(0XCD,ckey_digit_data[5]);
				for(ii = 0xC1; ii < 0xCA; ii= ii + 0x02)
					{
						STLED316_7Seg_Write(ii,ckey_digit_data[(ii-0xC1)/2]); // this is should not be 0x00 , default array have to written
							
					}
			}
			
			uikeypress_20mscount = 0;
			if(MachineErrors.EReg == 0)
			{
				if(((uiMachineState != PAUSE_PRESSED)&&(bStartButton == FALSE)&&(bTestMode_Selection == FALSE))||((uiMachineState == PROGRAM_END)&&(bTestMode_Selection == FALSE))||((uiMachineState == SNOOZEMODE)))
				{
					if(MachineErrors.EReg == 0)
					{
						if(snoozemode_10minutes_counter > 0)
							snoozemode_10minutes_counter--;
							
						if(snoozemode_10minutes_counter == 0)
						{
							
							uiMachineState = SNOOZEMODE;
							uisnooze_mode_pos = uiProgram_Position;
							snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT; // reload the 10 minutes counter
							
						}	
					}
				}
			}
			uikey_debounce_delay = KEY_DEBOUNCE_TIME;
			bBuzzer_On = TRUE;
		}
		}
		chip_select = FALSE; // for selecting the 1st STLED
		
		if(uiIDLE_COUNT > RESET_VALUE)
		{
			uiIDLE_COUNT--;
			if(uiMachineState == PAUSE_PRESSED)
			{
				uioption_on =   uioption_on | uiopt_check;
			}
			else
			{
				uioption_on =  uiopt_check;
			}
			if((uiMachineState == STARTUP)||((uiMachineState == IDLE_START)))
			{
				uioption_on =   uioption_on | uidisp_select_option;
			}
			if(uiIDLE_COUNT == RESET_VALUE)
			{
				if(uiMachineState == PAUSE_PRESSED)
				{
					uioption_on =  uioption_on &(~uiopt_check);
				}
				else
				{
					//uioption_on = RESET_VALUE;
					uioption_on =   uidisp_select_option;//uioption_on &(~uiopt_check);
				}
			}
		}
		else
		{
			uiopt_check = 0x08;//0x01;
		}
	/*	ak_count2++;
		if(ak_count2 == 2)
		{
			ak_count2 = 0;
			uidisplay_flash_count++;
			
		}*/
		if(uidisplay_flash_count<13)	//added for removing display fliker after key pressed -AKs
		{
			uidisplay_flash_count++;
		}
	//	uidisplay_flash_count++;
		bkeys_display_flag = 1;
		//KEYS_DISPLAY();
		}
		else
		{
			MS_count_key++;
		}
	}
}


//************* WaterLevel Frequency Check ***********//
/*
void Frequency_Calculation()
{
	static uint32_t ui_AvgCount;
	static uint8_t ui_Count ;
	//static uint16_t Time_Value, AI_Time_Value;
	if((bWaterLevel_Read_Required == YES)&&(cSwitch_Pressed == RESET_VALUE))
	{
		if((MachineErrors.EReg != 0)&&(bTestMode_Selection == TRUE))  
		{
			Time_Value = 1004;
		}
		else
		{
			Time_Value = 1009;
		}	
		if(PressoCount >= Time_Value)
		//if(PressoCount >= 504)//504
		{
			/*if(ui_Count <= 3)
			{
				ui_AvgCount += iWaterFreqSample;
				ui_Count ++;
			}
			else
			{
				//iWaterLevel = ((ui_AvgCount/2)*2)+ 10 ;
				iWaterLevel = (ui_AvgCount* 2) + 8 ;
				 
				ui_AvgCount = 0;
				ui_Count = 0;
			}
			iWaterLevel = iWaterFreqSample ;	
			iWaterFreqSample = RESET_VALUE;
			PressoCount = RESET_VALUE;
		}
		else
		{
			PressoCount++;
		}
	}
	else
	{
		PressoCount = RESET_VALUE;
		iWaterFreqSample = RESET_VALUE;	
	}
}
*/

void Error_Declaration()
{
	 //*****************Error Display***************//
  
       if((bCommunicationError == TRUE) || ((MachineErrors.EReg == 1))) //&& (bPhysical_DoorLock_Status  == FALSE))) // door error //taken from latest SL9kg
       {
	       
	       if(buzzerTone == 0)
	       {
		       uiSelectedTone = ERRORTONE;	
		 	buzzerTone = TRUE;
			if(bDoorLockError == TRUE)
		       {
			       cdigits_data[5] =  cdigits_data[5] & (~0x40); 		// // turn off the time dots
			      // mystrcpy((char *)cdigits_data_array,"dLEr");
			      mystrcpy((char *)cdigits_data_array," dLE");
			       Store_errors(E8);
#if ENABLE_IOT				
				set_IOTAlarm(ALARM_1, ALARM_1_DOOR_LOCK_FAILED);
#endif
		       }
		       else if(((bDoorOpenError == TRUE)||(bTestMode_Selection == TRUE))&&(bCommunicationError == FALSE))
		       {
			       cdigits_data[5] =  cdigits_data[5] & (~0x40); 		// // turn off the time dots
			       //mystrcpy((char *)cdigits_data_array,"door");
			       mystrcpy((char *)cdigits_data_array,"  do");
				Store_errors(E6);
#if ENABLE_IOT				
				set_IOTAlarm(ALARM_1, ALARM_1_DOOR_UNLOCK);
#endif
		       }
		       else if((bCommunicationError == TRUE)&&((bDoorOpenError == FALSE)||(bTestMode_Selection == TRUE)))
		       {
			       cdigits_data[5] =  cdigits_data[5] & (~0x40); 		// // turn off the time dots
			      // mystrcpy((char *)cdigits_data_array,"Conn");
			       mystrcpy((char *)cdigits_data_array," Con");
				Store_errors(E16);
#if ENABLE_IOT				
				set_IOTAlarm(ALARM_3, ALARM_3_COMM_ERROR);
#endif				
		       }
	       }
	       if(bDoorLockError == TRUE)
	       {
		      // mystrcpy((char *)cdigits_data_array,"dLEr");
		       mystrcpy((char *)cdigits_data_array," dLE");
#if ENABLE_IOT				
				set_IOTAlarm(ALARM_1, ALARM_1_DOOR_LOCK_FAILED);			
#endif
	       }
	        else if(((bDoorOpenError == TRUE)||(bTestMode_Selection == TRUE))&&(bCommunicationError == FALSE))
	       {
		       //mystrcpy((char *)cdigits_data_array,"door");
		       mystrcpy((char *)cdigits_data_array,"  do");
#if ENABLE_IOT				
				set_IOTAlarm(ALARM_1, ALARM_1_DOOR_UNLOCK);		
#endif
	       }
	         else if((bCommunicationError == TRUE)&&((bDoorOpenError == FALSE)||(bTestMode_Selection == TRUE)))
		{
			//mystrcpy((char *)cdigits_data_array,"Conn");
			mystrcpy((char *)cdigits_data_array," Con");
#if ENABLE_IOT				
				set_IOTAlarm(ALARM_3, ALARM_3_COMM_ERROR);
#endif	
		}
		cFlash_Count = 1;
	       	uiError_Occured = 1; 
		bDebugMode_Selection = FALSE;
       }
       else  if(MachineErrors.EReg == 2)// no water error
       {
	       if(buzzerTone1 == 0)  
	       {    
	       		uiSelectedTone = ERRORTONE;
			buzzerTone1= TRUE;
	       }
	       mystrcpy((char *)cdigits_data_array, " tAP");
	       Store_errors(E7);
	       cFlash_Count = 1;
	       uiError_Occured = 2; 
	       bDebugMode_Selection = FALSE;
#if ENABLE_IOT		
		set_IOTAlarm(ALARM_2, ALARM_2_NO_WATER);
#endif
       }
       else if(MachineErrors.EReg == 4) // drain pump error
       {	       
	       if(buzzerTone2 == 0)  
	       {  
	       		uiSelectedTone = ERRORTONE;
			buzzerTone2= TRUE;
	       }
	       //	mystrcpy((char *)cdigits_data_array, "dPEr");
	       mystrcpy((char *)cdigits_data_array, "  dP");
		Store_errors(E5);
		cFlash_Count = 1;
	       	uiError_Occured = 3; 
		bDebugMode_Selection = FALSE;
#if ENABLE_IOT		
		set_IOTAlarm(ALARM_2, ALARM_2_DRAIN_PUMP_FAILED);
#endif
       }
        else if(MachineErrors.EReg == 8) // high unbalance
       {
		Store_errors(E15);
		//cFlash_Count = 1;
	        uiError_Occured = 4; 
		bDebugMode_Selection = FALSE;
#if ENABLE_IOT		
		set_IOTAlarm(ALARM_2, ALARM_2_UNBALANCE);
#endif
       }
        else if(MachineErrors.EReg == 16)// volatge error
       {
	       if(buzzerTone3 == 0)  
	       {
	       		uiSelectedTone = ERRORTONE;
			buzzerTone3= TRUE;
			
	       }
	       if(uihigh_low_vtlg == 0x01)
		{
			if( high_vtg_count < 1)
			{
				//mystrcpy((char *)cdigits_data_array, "z260");
				mystrcpy((char *)cdigits_data_array, "  HR");
				Store_errors(E13);
#if ENABLE_IOT			
				set_IOTAlarm(ALARM_2, ALARM_2_HIGH_VOLTAGE);
#endif
			}
			else
			{
				mystrcpy((char *)cdigits_data_array, " 260");
				Store_errors(E13);
#if ENABLE_IOT			
				set_IOTAlarm(ALARM_2, ALARM_2_HIGH_VOLTAGE);
#endif	
			}
		}
		else if(uihigh_low_vtlg == 0x02)
		{
			if( low_vtg_count < 1)
			{
				//mystrcpy((char *)cdigits_data_array, "z260");
				mystrcpy((char *)cdigits_data_array, "  Lo");
				Store_errors(E9);
#if ENABLE_IOT			
				set_IOTAlarm(ALARM_2, ALARM_2_LOW_VOLTAGE);
#endif
			}
			else
			{
				mystrcpy((char *)cdigits_data_array, " 175");
				Store_errors(E9);
#if ENABLE_IOT			
				set_IOTAlarm(ALARM_2, ALARM_2_LOW_VOLTAGE);
#endif
			}
//			mystrcpy((char *)cdigits_data_array, "L175");
//			Store_errors(E9);
//#if ENABLE_IOT			
//			set_IOTAlarm(ALARM_2, ALARM_2_LOW_VOLTAGE);
//#endif
		}
		cFlash_Count = 1;
	        uiError_Occured = 5; 
		bDebugMode_Selection = FALSE;
       }
        else if(MachineErrors.EReg == 32)//motorerror
       {
	       if(buzzerTone8 == 0)
	       {	
	       	uiSelectedTone = ERRORTONE;
		buzzerTone8= TRUE;
	       }
		if(bMotorError_E1 == TRUE) //Block rotor or Hall Sensor or Connector Open
		{
		       mystrcpy((char *)cdigits_data_array, "  E1");
		       Store_errors(E10);
#if ENABLE_IOT			
			set_IOTAlarm(ALARM_1, ALARM_1_MOTOR_FAILED);
#endif		       
		}
		else if(bMotorError_E2 == TRUE) //OverCurrent
       		{
	               mystrcpy((char *)cdigits_data_array, "  E2");
		       Store_errors(E11);
#if ENABLE_IOT			
			set_IOTAlarm(ALARM_3, ALARM_3_IPM_OVERCURRENT);
#endif		       
		}
		else if(bMotorError_E3 == TRUE) //IPM overheat
       		{
	               mystrcpy((char *)cdigits_data_array, "  E3");
		       Store_errors(E12);
#if ENABLE_IOT			
			set_IOTAlarm(ALARM_3, ALARM_3_IPM_OVERHEAT);			
#endif		       
		}
	       
	       cFlash_Count = 1;
	       bDebugMode_Selection = FALSE;
	       
	       uiError_Occured = 6; 
       }
       else if(MachineErrors.EReg == 64)//heater error
       {
	       bDebugMode_Selection = FALSE;
	       if((bNtcFailed == TRUE)) // ntc error
	       {
		       if(buzzerTone10 == 0)  
		       {
			        uiSelectedTone = ERRORTONE;
				buzzerTone10 = TRUE;
				
		       }
		       Store_errors(E3);
		       mystrcpy((char *)cdigits_data_array, " ntc");
		       cFlash_Count = 1;
#if ENABLE_IOT			
			set_IOTAlarm(ALARM_2, ALARM_2_NTC_ERROR);
#endif
		      
	       }
	       else
	       {
		       if(buzzerTone4 == 0)  
		       {
		       		uiSelectedTone = ERRORTONE;
				buzzerTone4= TRUE;
		       }
		       
		       if(uiover_heat == TRUE)
		       {
				//mystrcpy((char *)cdigits_data_array, "HEAt");//over heat Hot
				mystrcpy((char *)cdigits_data_array, " Hot");
				Store_errors(E2);
#if ENABLE_IOT				
				set_IOTAlarm(ALARM_1, ALARM_1_OVER_HEAT);
#endif
			}
		       else
		       {
		        	mystrcpy((char *)cdigits_data_array, " Htr");//heater error
				Store_errors(E1);
#if ENABLE_IOT				
				set_IOTAlarm(ALARM_2, ALARM_2_HEATER_FAILURE);
#endif
		       }
			cFlash_Count = 1;
	       }
	       
	       uiError_Occured = 7; 
       }
        else if(MachineErrors.EReg == 128)//pressure error
       {
	       bDebugMode_Selection = FALSE;
	       if(buzzerTone5 == 0)  
	       {
	       		uiSelectedTone = ERRORTONE;
			buzzerTone5= TRUE;
	       }
	       if(bWaterOverFLow == TRUE)
	       {
		       //mystrcpy((char *)cdigits_data_array, "OFEr");
		       mystrcpy((char *)cdigits_data_array, " OFL");
		       Store_errors(E4);
#if ENABLE_IOT			
			set_IOTAlarm(ALARM_1, ALARM_1_OVERFLOW);
#endif
		}
		else if((bPressureError == TRUE)||(bPressureFailure == TRUE))
		{
			 if(bHPrs == TRUE)
			 {
				//mystrcpy((char *)cdigits_data_array, "zPrS");
				mystrcpy((char *)cdigits_data_array, " zPr");
				Store_errors(E17); 
			 }
			 else if(bLPrs == TRUE)
			 {
				//mystrcpy((char *)cdigits_data_array, "LPrS"); 
				mystrcpy((char *)cdigits_data_array, " LPr");
				Store_errors(E18);
			 }
			 else if((bHPrs == FALSE)&&(bLPrs == FALSE))
			 {
				mystrcpy((char *)cdigits_data_array, " PrS"); 
				Store_errors(E14);
			 }
			
#if ENABLE_IOT			
			set_IOTAlarm(ALARM_1, ALARM_1_PRESSURE_SW_FAILED);
#endif
		}
		
		cFlash_Count = 1;
	       uiError_Occured = 8; 
       }
#if ENABLE_IOT
	else  if(MachineErrors.EReg == 256)// WiFi communication error
	{
		if(buzzerTone14 == 0)
		{
			uiSelectedTone = ERRORTONE;
			buzzerTone14= TRUE;
		}
		mystrcpy((char *)cdigits_data_array, " lot");
		Store_errors(E19);
		cFlash_Count = 1;
		uiError_Occured = 9;
		bDebugMode_Selection = FALSE;
	}
#endif
       else 
       {	
	       if(((bDoorLockStatus == TRUE)&&(bCommunicationError == FALSE))&& (MachineErrors.EBits.DoorError == 0)&&(uiError_Occured == 0))
	       {
		       //mystrcpy((char *)cdigits_data_array, "    ");
		       buzzerTone = 0;
		       bDoorOpenError = FALSE;
#if ENABLE_IOT
			clr_IOTAlarm(ALARM_1,ALARM_1_DOOR_LOCK_FAILED);
			clr_IOTAlarm(ALARM_1,ALARM_1_DOOR_UNLOCK);		
			clr_IOTAlarm(ALARM_3, ALARM_3_COMM_ERROR);
#endif
	       }
	       
	       if((MachineErrors.EBits.NoWaterError == 0)&&(bNoWater == NO)&&(uiError_Occured == 0))
	       {
		       //mystrcpy((char *)cdigits_data_array, "    ");
		       buzzerTone1 = 0;
#if ENABLE_IOT
			clr_IOTAlarm(ALARM_2,ALARM_2_NO_WATER);
#endif
	       }
	       if(MachineErrors.EBits.DrainPump == 0)
	       {
		      // mystrcpy((char *)cdigits_data_array, "    ");
		       buzzerTone2 = 0;
#if ENABLE_IOT
			clr_IOTAlarm(ALARM_2,ALARM_2_DRAIN_PUMP_FAILED);
#endif
	       }
	       if((MachineErrors.EBits.VoltageError == 0)&&(uiSupplyVoltage > 160)&&(uiSupplyVoltage < 281)&&(uiError_Occured == 0))
	       {
		      // mystrcpy((char *)cdigits_data_array, "    ");
		       buzzerTone3 = 0;
#if ENABLE_IOT
			clr_IOTAlarm(ALARM_2,ALARM_2_HIGH_VOLTAGE);
			clr_IOTAlarm(ALARM_2,ALARM_2_LOW_VOLTAGE);
#endif
	       }
	       if((MachineErrors.EBits.HeaterError == 0)&& (uiTemperature < 57)&&(uiError_Occured == 0))
	       {
		       //mystrcpy((char *)cdigits_data_array, "    ");
		       buzzerTone4 = 0;
#if ENABLE_IOT
			clr_IOTAlarm(ALARM_2,ALARM_2_HEATER_FAILURE);
			clr_IOTAlarm(ALARM_1, ALARM_1_OVER_HEAT);
#endif
	       }
	       if((bPressureError ==0)&&(bWaterOverFLow == 0)&&(iWaterLevel > 36000)&&(iWaterLevel < 43100)&&(uiError_Occured == 0))
	       {
		      // mystrcpy((char *)cdigits_data_array, "    ");
		       buzzerTone5 = 0;
#if ENABLE_IOT
			clr_IOTAlarm(ALARM_1,ALARM_1_PRESSURE_SW_FAILED);
			clr_IOTAlarm(ALARM_1,ALARM_1_OVERFLOW);
#endif
	       }
	       if((bMotorError_E1 == NO)&&(bMotorError_E2 == NO)&&(bMotorError_E3 == NO)&&(uiError_Occured == 0))
	       {
		      // mystrcpy((char *)cdigits_data_array, "    ");
		        buzzerTone8 = 0;
#if ENABLE_IOT
			clr_IOTAlarm(ALARM_3, ALARM_3_IPM_OVERHEAT);
			clr_IOTAlarm(ALARM_3, ALARM_3_IPM_OVERCURRENT);
			clr_IOTAlarm(ALARM_1, ALARM_1_MOTOR_FAILED);
#endif
		}
		if( bNtcFailed == FALSE)
		{
			// mystrcpy((char *)cdigits_data_array, "    ");
			buzzerTone10 = 0;
#if ENABLE_IOT
			clr_IOTAlarm(ALARM_2,ALARM_2_NTC_ERROR);
#endif
		}
#if ENABLE_IOT
		if((MachineErrors.EBits.WifiComnError == 0))
		{
			buzzerTone14 = FALSE;
		}		
		if(MachineErrors.EBits.HighUnbalance == 0)
		{
			clr_IOTAlarm(ALARM_2,ALARM_2_UNBALANCE);
		}
#endif

			uiError_Occured = 0;
	}
	
	if(MachineErrors.EReg != FALSE)
	{	
		ckey_digit_data[0] =  ckey_digit_data[1] =  ckey_digit_data[2] = ckey_digit_data[3] = 0x00; // clear the selected options & navigation mark in error condition
	
		turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
		turnon_display_OxyJetled(OXYJET_OFF);	//TODO: Turn OFF OxyJet at start of program.
		update_display_data(&cdigits_data_array[0]);
		bCirculation_Pump = SWITCH_OFF;
		// ckey_digit_data[0] = ckey_digit_data[1] = ckey_digit_data[2] = RESET_VALUE; // clear the  option led
	}

	 //Buzzer Sound after every 5mins
	if(uiError_Occured != RESET_VALUE)
	{
		bBuzzerTime_Start = YES;
	}
	else
	{
		bBuzzerTime_Start = NO;
	}
}	

void Software_Timer20ms()
{
	uint8_t iter_ui8;
	for(iter_ui8 = 0; iter_ui8<NO_USER_TIMER; iter_ui8++)
	{
		if(TimerObject_Init[iter_ui8].countdownTimer_b == TRUE)
		{
			if(TimerObject_Init[iter_ui8].counter_ui32 == 0)
			{
				TimerObject_Init[iter_ui8].countdownTimer_b = FALSE;
			}
			else
			{
				TimerObject_Init[iter_ui8].counter_ui32--;
			}
		}
	}
/*----------------------------CP Timer Execution----------*/	
	if(CpOnTimer_b)
	{
		CP20MsTo1MsCounter_ui8++;
		if(CP20MsTo1MsCounter_ui8 == _20MS_TO_1MS_FACTOR)
		{	
			CPTimeInSecCounter_ui32++;
			CP20MsTo1MsCounter_ui8 = 0;
		}
		
	}
	else if(CpOffTimer_b)
	{
		CP20MsTo1MsCounter_ui8++;
		if(CP20MsTo1MsCounter_ui8 == _20MS_TO_1MS_FACTOR)
		{	
			CPTimeInSecCounter_ui32++;
			CP20MsTo1MsCounter_ui8 = 0;
		}
	}
	
}

/***********************************************************************
* Function_Name : Timer_CountdownTimer(uint8_t,uint8_t).
* Ret_type	: uint8_t.
* Working	: Software Timer.
* Developed By	: 
* Date		: 11/03/2022
************************************************************************/
uint8_t Timer_CountdownTimer(uint8_t option, uint8_t name)
{
	static uint8_t T_Case;
	
	T_Case = RESET_VALUE;
	switch(option)
	{
		case SET_TIME:
			TimerObject_Init[name].counter_ui32 = TimerObject_Init[name].total_ui32;// * _20MS_TO_1MS_FACTOR;
			T_Case = TRUE;
			break;
		case START_TIMER:
			TimerObject_Init[name].counter_ui32 = TimerObject_Init[name].total_ui32 * _20MS_TO_1MS_FACTOR;
			TimerObject_Init[name].countdownTimer_b = TRUE;
			T_Case = TRUE;
			break;
		case PAUSE_TIMER:
			TimerObject_Init[name].countdownTimer_b = FALSE;
			T_Case = TRUE;
			break;
		case RESUME_TIMER:
			TimerObject_Init[name].countdownTimer_b = TRUE;
			T_Case = TRUE;
			break;
		case CHECK_TIMER:
			T_Case  = (TimerObject_Init[name].counter_ui32 == 0)? TRUE : FALSE;
			break;
		/*case LAST_30S_DRAIN_CHECK_TIMER:
			T_Case  = (TimerObject_Init[name].counter_ui32 <= LAST_30S_DRAIN_COUNT)? TRUE : FALSE;
			break;*/
		default:
			T_Case = FALSE;
			break;
	}
	
	return T_Case;
}

/***********************************************************************
* Function_Name : Timer_SetCountdownTimer(uint8_t,uint8_t).
* Ret_type	: boolean.
* Working	: Set of Software Timer value in sec.
* Developed By	: 
* Date		: 11/03/2022
************************************************************************/
boolean Timer_SetCountdownTimer(uint8_t name, uint16_t steptime)
{
	static boolean ret;
	if(TimerObject_Init[name].counter_ui32 == 0)
	{
		TimerObject_Init[name].total_ui32 = steptime;
		ret = TRUE;
	}
	else
	{
		ret = FALSE;
	}
	return ret;
}
/***********************************************************************
* Function_Name : setAIWaterFreq_ReadEnable(uint8_t).
* Ret_type	: void.
* Working	: Set AI Water Frequency Read bit.
* Developed By	: 
* Date		: 19/04/2022
************************************************************************/
void setAIWaterFreq_ReadEnable(uint8_t Read_En)
{
	AI_WaterFreqRead_b = Read_En;
}
