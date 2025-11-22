#include "keyevent.h"
#include "r_cg_userdefine.h"
#include "Selection.h"
#include "STLED316S.h" 
#include "ErrorHandling.h"
#include "WashOperations_Global.h"
#include "r_cg_serial.h" // IOT

//***************************************************************//
//********************* global variables ***********************//

extern boolean bTestMode_Selection,bProgramStart,bDebugMode_Selection,bprewash_selected;
extern boolean uiover_heat,bFaciaTest,bFCTMode_Selection,bPhysical_DoorLock_Status;
extern boolean bPrewash_Start,bSoak_Start,bHotRinse_Start,bExtraRinse_Start;
extern boolean bKeyDetected,bDoorLock,bTimeSaver_Start,bDelay_Start1,bDoorLockStatus;
extern boolean bTESTMODE_START,bDoorLockError,bEnduranceStarted,bSteam_Start;
extern boolean bSpecialEnduranceSelected,bChildLockButton,bStartButton;
extern boolean bNtcFailed,uiReset_TempRPM,chip_select,bTest_Forward;

extern uint8_t ckey_digit_data[6],cdigits_data_array[6] ;
extern uint8_t cdigits_data[7],Error_Storage[7];
extern uint8_t uiError_Occured,uiDebugState,FaciaState,uiTubClean_Disp,ii;
extern uint8_t uiMachineTestState,cdisp_count,uiFCT_SW_Test;
extern uint8_t uiFCTMode,uiTFCTSeconds,uiRinseholdState,uikey_debounce_delay;//,PROG_SW_1_5_count;

extern uint8_t uiopt_check,uiOption_closed,uiSelected_Position,uiSmartProgram_Selected;
extern uint8_t uidisp_select_option,uiRESET,SatrtUpDelay ;
extern uint8_t uiPSER_Option_Avbl,uiMachineState,uioption_on; //
//extern uint8_t uiSelectedTone,uiProgram_Position,uiTestModeState;
extern uint8_t uiProgram_Position,uiTestModeState;
extern volatile uint8_t uiSelectedTone;
extern uint8_t uimaxspeed_temp,uiTempSel,uiRpmSel;
extern uint8_t uiIDLE_COUNT,uiRMP_Default,uiDISP_MCount;

extern uint16_t iDelay_Count,iDelaytime;
extern uint16_t ProgramLifeCycle,BalanceTime,uidelaymin,ProgressTime,ProgramTime;
extern uint16_t uidisplay_flash_count,mybuzzerdelay;
extern uint16_t uikeypress_20mscount,uiSF8_cyclecompleted;
extern int cSwitch_Pressed;
extern uint8_t uiExtraRinse_Press_count,uiExtraRinse_Max; //AK


//***** global function prototype *****//
extern void ProgSel_led();
extern void Power_OnOff(void);
extern void MotorStop(void);
extern void TestMode_Fasia_led();
extern void Reset_ProgParamter();
extern void ProgramTimming();
extern void ToneGeneration(void);

//***** local function prototype *****//
void Clear_Option_INPause();
void ButtonPressed(void);


//*************** Local Variables **************//
boolean bBuzzer_On;
boolean bSnooze_Flag,Lab_mode_Active;
boolean breset_flag,bDispalyoff_flag;
boolean Switch_Reset,TubClean_Activated;//bExpress_30_Activatted;
boolean Test_mode_Active,Fct_mode_Active;
boolean cReset_TestMode_Stage;
boolean b_option_key_pressed_flag;

boolean bDisp_write; // added for neo
boolean PROG_SW_1_5_flag,PROG_SW_6_10_flag; //added for neo 
boolean bTub_Available,bTub_deselect;// added for neo 

uint8_t uiTone_Selected = FALSE;
uint8_t uidoorflash_count,uiMachineBefore_Poweroff;
uint8_t sleep_mode = 1;
uint8_t uiTemp_testmode = 1;
uint8_t uiSelection_testmode = 1;
uint8_t uiOption_testmode = 1;
uint8_t uiRpm_testmode = 1;
uint8_t uiTimesaver_testmode = 1;
uint8_t ucDbgOptionKey_count;
uint8_t ucDbgSelectKey_count;
uint8_t ucDbgTempKey_count;
uint8_t ucDbgRpmKey_count;
uint8_t ucDbgTimesaverKey_count;
uint8_t ucDbgSoakKey_count;
uint8_t TEST_FCT_Active;
uint8_t uichild_flash_count;
uint8_t uilast_selected_option;
boolean Timesaver_3sec_b, bExtraRinse_Flag,prgm_key_bzzr_flag;
uint16_t uidebug_key,uitest_key;
uint8_t uiTub_deselect;
uint8_t Childlock_presstime,Tubclean_presstime,Extra_rinse_presstime,Spin_button_presstime,Debug_mode_presstime,Wifi_resettime;

uint8_t PROG_SW_1_5_count,PROG_SW_6_10_count=5;

#if ENABLE_IOT
#include"iot_config.h"
#include "iot_user_config.h"
#include "iot_control.h" 
extern boolean bDiagnostic_Set_Flag;
extern uint8_t BkMultiFuncSetting;
#endif
//--------------------------------------------------------------------


/***********************************************************************************************************************
* Function Name: ButtonPressed
* Description  : This function implements to get which key is pressed & 
		 execute the steps accordingly .
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void ButtonPressed(void)
{
	switch(cSwitch_Pressed)
	{
		
		case START_KEY: //start key
			
			if((bChildLockButton !=1)&&(uiMachineState != SNOOZEMODE))
			{
					if((bTestMode_Selection == TRUE) && (bStartButton == TRUE)&&(uiError_Occured == RESET_VALUE)&&((uiTestModeState == SF01)||(uiTestModeState == SF02)))
					{
						if((uikeypress_20mscount >= 80)&&(uiMachineTestState > step1))
						{
							cReset_TestMode_Stage = TRUE;
							bKeyDetected = RESET_VALUE;
						}
					}					
					else if((((MachineErrors.EReg != 128)&&(uiover_heat == FALSE)&&
					(bTestMode_Selection==FALSE)&&(bDoorLockError == FALSE)&&
					(MachineErrors.EBits.MotorError == FALSE))||
					((bTestMode_Selection == TRUE)&&(uiError_Occured == RESET_VALUE)&&
					(bEnduranceStarted == FALSE)&&(uiMachineState == TESTMODE)))&&
					(bPhysical_DoorLock_Status == TRUE))
					{
						bStartButton ^=1;
						uidisplay_flash_count = RESET_VALUE;
						bKeyDetected = RESET_VALUE;
						uiDISP_MCount = RESET_VALUE;						
						Switch_Reset = TRUE; // this is doubpt 
						
						if((bStartButton == 1)&&(uiMachineState == STARTUP))
						{
							uiSelectedTone = STARTPRESSTONE;
						}
						else
						{
							uiTone_Selected = 3;
						}
						
					}
					else
					{
						if(bPhysical_DoorLock_Status == FALSE)
						{
							uidoorflash_count= 2;// need flag 
						}
						
						bKeyDetected = RESET_VALUE;
						if((bPhysical_DoorLock_Status == FALSE)&&(uiMachineState == TESTMODE)&&(FaciaState == FALSE)&&(bFaciaTest == FALSE)&&((uiTestModeState == SF01)||(uiTestModeState == SF02)))
						{
							bStartButton ^=1;
							uiTone_Selected = 1;
						}
						else
						{
							uiTone_Selected = 2;
						}
					
					}
					
					if(bStartButton == TRUE)
					{
						uiopt_check = 0x10; // for reset the option 
						uiIDLE_COUNT = RESET_VALUE;
						//cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
						//------------
						turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
						uiReset_TempRPM = FALSE;
						
						//-----------
						if(uiRinseholdState == 2)
						{
							if(uiRpmSel == Rinsehold)
							{
								uiRpmSel = uiRMP_Default;
								uiRinseholdState = RESET_VALUE;
							}
							else
							{
								uiRinseholdState = RESET_VALUE;
							}
							
						}
						if(bTestMode_Selection == TRUE)
						{
							bTESTMODE_START   = TRUE;
						}
						else
						{
							uilast_selected_option = uidisp_select_option;
						}
#if ENABLE_IOT						
						if(BalanceTime >= ProgressTime)
						{						
							ProgramTime += (BalanceTime - ProgressTime);							
						}
						else
						{								
							ProgramTime -= (ProgressTime - BalanceTime);
						}					
#endif						


						
					}
					else if(bStartButton == FALSE)
					{
						//cdigits_data[4] =  cdigits_data[4] &(~ 0x08);	// //turn off the start/puase led
						//cdigits_data[5] =  cdigits_data[5] &  (~0x80);		//  // turn off the semicolan
						 uiopt_check = 0x10; // for reset the option 
						//uioption_on = RESET_VALUE;
						uiIDLE_COUNT = RESET_VALUE;
#if ENABLE_IOT
						ProgressTime = BalanceTime;
#endif						
						if(bTestMode_Selection == FALSE)
						{
							Clear_Option_INPause();
						}
						
					}
					
			   
			}
			else
			{
				if((Switch_Reset != RESET_VALUE))
				{
					if(bChildLockButton == TRUE)
					{
						uichild_flash_count = 2;
					}
					uiTone_Selected = 2;
					bKeyDetected = RESET_VALUE;
				}
			}
			break;
//		case OPTION_KEY ://option key
//			Timesaver_3sec_b = FALSE;
//			b_option_key_pressed_flag = 1;
//			if((MachineErrors.EReg != 128)&&(uiover_heat == FALSE)&&(bDoorLockError == FALSE))
//			{
//				if((bDebugMode_Selection == TRUE)||(bTestMode_Selection == TRUE)|| (bFCTMode_Selection == TRUE))
//				{
//					
//					if(bDebugMode_Selection == TRUE)
//					{
//						uidebug_key = OPTION_KEY; // for selecting switch case in debug mode
//						ucDbgOptionKey_count++;
//						ucDbgSelectKey_count = RESET_VALUE;
//						ucDbgTempKey_count = RESET_VALUE;
//						ucDbgRpmKey_count = RESET_VALUE;
//						
//						if(ucDbgOptionKey_count > 7) //in debug mode we have only 7 different message have to show
//						{
//							ucDbgOptionKey_count = 1;
//						}
//						uiTone_Selected = 1;
//							bKeyDetected = RESET_VALUE;
//					}
//					else if((bTestMode_Selection == TRUE)&&(bStartButton == TRUE))
//					{
//						if(((uiTestModeState == SF01)||(uiTestModeState == SF02))&&(uiProgram_Position == 1)&&(bFaciaTest != 1))
//						{
//							uiopt_check ^= 0x7f;
//							uiPSER_Option_Avbl = uiopt_check;
//							uiOption_testmode ^= 0x01;
//							
//							//-----------------------------------------------
//								TestMode_Fasia_led();
//							//-----------------------------------------------
//							uiTone_Selected = 1;
//								bKeyDetected = RESET_VALUE;
//						}
//						else 
//						{
//							if((uiProgram_Position == STEAMFRESH)&&((uiTestModeState == SF01)||(uiTestModeState == SF02))&&(uiMachineTestState < step2))
//							{
//								if(uikeypress_20mscount >= 150)
//								{
//									Error_Storage[0] = Error_Storage[1] = Error_Storage[2] = Error_Storage[3]= Error_Storage[4] = Error_Storage[5] = Error_Storage[6] = RESET_VALUE;
//									uikeypress_20mscount = RESET_VALUE;
//									uiSelectedTone = KEYPRESSTONE;
//								}
//							}
//							else
//							{
//								uiTone_Selected = 2;
//							}
//							uitest_key = OPTION_KEY; // for selecting switch case in debug mode
//						}
//						
//					}
//					else if(bFCTMode_Selection == TRUE)
//					{
//						uiFCT_SW_Test |=0x01;
//						uiTone_Selected = 1;
//							bKeyDetected = RESET_VALUE;
//					}
//					else 
//					{
//						uiTone_Selected = 2;
//					}
//					
//				}
//				else if((uiMachineState != PROGRAM_END)&&(uiMachineState != SNOOZEMODE)&&(MachineErrors.EReg == 0))
//				{
//					if(uikeypress_20mscount >= 100) //3 sec press count (20msec x 150)
//					{
//#if ENABLE_IOT						
//						uikeypress_20mscount = RESET_VALUE;
//						//uiSelectedTone = KEYPRESSTONE;
//
//						if((get_IOTMultiFuncSetting() & DETERGENT_ORDER) == RESET_VALUE)//((uDetergentKey&(1<<BIT0)) ==RESET_VALUE)
//						{
//
//							set_IOTMultiFuncSetting(DETERGENT_ORDER);//uDetergentKey |= (1<<BIT0);//Used Bit0 for Detergent
//							BkMultiFuncSetting |= DETERGENT_ORDER;
//							uiSelectedTone = KEYPRESSTONE;
//							//uikeypress_20mscount = 0;
//							//bKeyDetected = RESET_VALUE;
//						}
//						else
//						{
//							uiSelectedTone = INAPPLICABLE;
//							//uikeypress_20mscount = RESET_VALUE;
//							//bKeyDetected = RESET_VALUE;
//						}
//						bKeyDetected = RESET_VALUE;
//#endif						
//					}	            
//					else if((bChildLockButton !=1)&&(bStartButton == FALSE)&&(uiMachineState != PROGRAM_END)&&(uiMachineState != SNOOZEMODE))						
//					{
//						if(uikeypress_20mscount < 15)
//						{
//							
//							//bKeyDetected = RESET_VALUE;
//							uiTone_Selected = 1;
//							if((TubClean_Activated == FALSE)&&(uiRinseholdState != 2))
//							{
//								OptionSelection();
//								uikey_debounce_delay = 15;
//							}
//							else
//								uiTone_Selected = 2;
//						}
//						
//					}
//					else if((bChildLockButton !=1)&&(bStartButton == TRUE)) 
//					{
//						
//						if((uiMachineState != PROGRAM_END)&&(uiMachineState != SNOOZEMODE)&&(MachineErrors.EReg == 0))
//						{
//							if(uikeypress_20mscount < 15)
//							{
//								if(bSpecialEnduranceSelected == TRUE)
//								{
//									SevenSegPrint(uiSF8_cyclecompleted);
//								}
//								else
//								{
//									SevenSegPrint(ProgramLifeCycle);
//								}
//
//									uikey_debounce_delay = 15;
//									turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
//									uiDISP_MCount = 50;
//									// bKeyDetected = TRUE; // need to check the impact
//									uiTone_Selected = 1;
//									//bKeyDetected = RESET_VALUE;
//								}
//						}
//						else
//						{
//							
//								uiTone_Selected = 2;
//							//bKeyDetected = RESET_VALUE;
//						}
//						
//						
//					}
//					else
//					{
//						if(bChildLockButton == TRUE)
//						{
//							uichild_flash_count = 2;
//						}
//						if((Switch_Reset != RESET_VALUE))
//							uiTone_Selected = 2;
//					}
//				}
//				else
//				{
//					uiTone_Selected = 2;
//					//bKeyDetected = RESET_VALUE;
//				}
//				
//			}
//			else
//			{
//				if(bChildLockButton == TRUE)
//				{
//					uichild_flash_count = 2;
//				}
//				uiTone_Selected = 2;
//				bKeyDetected = RESET_VALUE;
//			}
//			break;
			
		case TEMPERATURE_KEY : // temperature key
		
			
			if(((bDebugMode_Selection == TRUE)||(bTestMode_Selection == TRUE)||(bFCTMode_Selection == TRUE))&&(bDoorLockError == FALSE))
			{
				
				if(bDebugMode_Selection == TRUE)
				{
					uidebug_key = TEMPERATURE_KEY; // for selecting switch case in debug mode
					ucDbgTempKey_count ++;
					ucDbgSelectKey_count = RESET_VALUE;
					ucDbgOptionKey_count = RESET_VALUE;
					ucDbgRpmKey_count = RESET_VALUE;
					//if(ucDbgTempKey_count > 7)//TODO: 6 to 7 Added AI Program selection in debug mode //in debug mode we have only 6 different message have to show
					if(ucDbgTempKey_count > 9)//added to display load weight for testing purpose
					{
						ucDbgTempKey_count = 1;
					}
					uiTone_Selected = 1;
					bKeyDetected = RESET_VALUE;
				}
				else if((bTestMode_Selection == TRUE)&&(bStartButton == TRUE)&&((uiTestModeState == SF01)||(uiTestModeState == SF02))&&(uiProgram_Position == 1)&&(bFaciaTest != 1))
				{
					//uiTemp_testmode ^= 0x1;   
					uiTemp_testmode = 0x00;  // ADDED TO REMOVE BUTTON TOGGLING IN TRSTMODE   
					//TestMode_Fasia_led();
					uiTone_Selected = 1;
				}
				else if(bFCTMode_Selection == TRUE)
				{
					uiFCT_SW_Test |=0x02;
					uiTone_Selected = 1;
				}
				else
				{
					uiTone_Selected = 2;
				}
				bKeyDetected = RESET_VALUE;
			}
#if ENABLE_IOT
		else if((bChildLockButton !=1)&&(uiMachineState != PROGRAM_END)&&(uiMachineState != SNOOZEMODE)&&(MachineErrors.EReg == 0))
#else			
			else if((bChildLockButton !=1)) 
#endif				
			{
				/*if((uiProgram_Position == DRAINSPIN) && (uikeypress_20mscount >= 150)&&(uiMachineState != SNOOZEMODE))
				{
					
					 Read_Errors(Error_Storage[0]); //show the latest error
					 turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
					 uiTone_Selected = 1;
					//uikeypress_20mscount = RESET_VALUE;
					 bKeyDetected = TRUE;
					 uiDISP_MCount = 80;
					 bBuzzer_On = TRUE;
					 uikeypress_20mscount = RESET_VALUE;
					 bKeyDetected = RESET_VALUE;
					
				}*/
				/*if((uiProgram_Position == ADDITIVESRINSE) &&(TubClean_Activated == FALSE) &&(bProgramStart == FALSE)&&(uiMachineState != SNOOZEMODE))
				{
					if(uikeypress_20mscount >= 120)
					{
						 TubClean_Activated = TRUE;
						 
						 mystrcpy((char *)cdigits_data_array, " tUb");
						 update_display_data(&cdigits_data_array[0] );
						 turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
						 
						 uiSelectedTone = KEYPRESSTONE;
						 uimaxspeed_temp =  Wash95 | Rpm000;
						 uiTempSel = Wash95;
						 uiRpmSel = Rpm000;
						 uidisp_select_option = RESET_VALUE;
						 cdigits_data[5] = cdigits_data[5]& ~(0x3F); // for clear the selected option
						 uikeypress_20mscount = RESET_VALUE;
						 uiPSER_Option_Avbl =  0x00;
						 uiDISP_MCount = 0;
						 BalanceTime = TUBCLEAN90_TIME;
						 uidisplay_flash_count = RESET_VALUE;
						 bBuzzer_On = TRUE;
						 uiTone_Selected = 1;
						 bKeyDetected = RESET_VALUE;
					}
					else
					{
						
						uiTone_Selected = 2;
					}
				
					 
				}*/				
//				if((uikeypress_20mscount >= 100)) //3 sec press count (20msec x 150)
//				{
//#if ENABLE_IOT					
//					if((((BkMultiFuncSetting & AP_MODE_SETTING) == RESET_VALUE)&&(get_WiFiMode() != 0x01))&&((get_IOTMultiFuncSetting() & WIFI_ENABLE_DISABLE)== RESET_VALUE))
//					{
//
//						uiDISP_MCount = 70;
//						set_IOTMultiFuncSetting(AP_MODE_SETTING);
//						BkMultiFuncSetting = get_IOTMultiFuncSetting(); //new added
//						turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
//						mystrcpy((char *)cdigits_data_array, " AP ");
//						update_display_data(&cdigits_data_array[0]);
//						uiSelectedTone = KEYPRESSTONE;
//						uikeypress_20mscount = RESET_VALUE;
//						bKeyDetected = RESET_VALUE;
//					}
//					else
//					{
//						uiSelectedTone = INAPPLICABLE;
//						uikeypress_20mscount = RESET_VALUE;
//						bKeyDetected = RESET_VALUE;
//					}
//#endif					
//				}
				if(uikeypress_20mscount < 15)
				{
					if(((uiProgram_Position != ADDITIVESRINSE)||((uiProgram_Position == ADDITIVESRINSE)&&(TubClean_Activated == TRUE)))&&((uiSmartProgram_Selected != STEAMWASH))&&(uiProgram_Position != STEAMFRESH)/*&& (bExpress_30_Activatted == FALSE)*/)
					{
						if((uiMachineState != PROGRAM_END)&&(uiMachineState != SNOOZEMODE)&&(MachineErrors.EReg == 0))
						{
							TemperatureSelection();
							//bKeyDetected = RESET_VALUE;
							//added to remove mismatch betn buzzer and display change
							if(prgm_key_bzzr_flag == 0)	//ADDED FOR REMOVING BUZZER DELAY - AK
							{
								uiTone_Selected = 1;
								uiSelectedTone = KEYPRESSTONE;
								prgm_key_bzzr_flag = TRUE;
							}
							else		//ADDED FOR REMOVING BUZZER DELAY - AK
							{
								uiTone_Selected = 4;
								uiSelectedTone = KEYPRESSTONE2;
								prgm_key_bzzr_flag = FALSE;
							}
							ToneGeneration();
						}
					}					
					else 
					{
						if((Switch_Reset != RESET_VALUE))
					 		uiTone_Selected = 2;
					}
					uikey_debounce_delay = 15;
				}
				
			}
			else
			{
				if(bChildLockButton == TRUE)
				{
					uichild_flash_count = 2;
				}
				uiTone_Selected = 2;
				bKeyDetected = RESET_VALUE;
			}
			break;
		case SPIN_KEY :// rpm/ spin selection key
			if(((bDebugMode_Selection == TRUE)||(bTestMode_Selection == TRUE)||(bFCTMode_Selection == TRUE))&&(bDoorLockError == FALSE))
			{
				if(bDebugMode_Selection == TRUE)
				{
					uidebug_key = SPIN_KEY; // for selecting switch case in debug mode
					ucDbgRpmKey_count++;
					ucDbgSelectKey_count = RESET_VALUE;
					ucDbgOptionKey_count = RESET_VALUE;
					ucDbgTempKey_count= RESET_VALUE;
					
					if(ucDbgRpmKey_count >6)//4) //in debug mode we have only 3 different message have to show
					{
						ucDbgRpmKey_count = 1;
					}
					uiTone_Selected = 1;
					bKeyDetected = RESET_VALUE;
				}
				else if(bTestMode_Selection == TRUE)
				{
					
					if((bStartButton == TRUE)&&((uiTestModeState == SF01)||(uiTestModeState == SF02))&&(uiProgram_Position == 1)&&(bFaciaTest != 1))
					{
						//uiRpm_testmode ^= 0x01;
						uiRpm_testmode = 0x00; // ADDED TO REMOVE TOGGLE OF BUTTON IN TEST MODE
						//TestMode_Fasia_led();
						uiTone_Selected = 1;
						
					}
					else if((bStartButton == FALSE)&&((uiTestModeState == SF03)||(uiTestModeState == SF04)||(uiTestModeState == SF06)))
					{
						SpinSelection(); // select the desire RPM
						
					}
					else
					{
						uiTone_Selected = 2;
					}
					bKeyDetected = RESET_VALUE;
				}
				else if(bFCTMode_Selection == TRUE)
				{
					
					uiFCT_SW_Test |=0x04;
					uiTone_Selected = 1;
					bKeyDetected = RESET_VALUE;
						
					
				}
				else
				{
					uiTone_Selected = 2;
					bKeyDetected = RESET_VALUE;
				}
			}
			else if((bChildLockButton !=1)&&(uiError_Occured == RESET_VALUE)) 
			{
				if((uiMachineState != PROGRAM_END)&&(uiMachineState != SNOOZEMODE)&&(MachineErrors.EReg == 0))
				{

					Extra_rinse_presstime = EXTRARINSE_LONGPRESS(); 
					Spin_button_presstime = SPIN_BUTTON_PRESS();
					if(uikeypress_20mscount >= Extra_rinse_presstime)//200//100
					{
						//bExtraRinse_Flag = bExtraRinse_Flag^TRUE;
						if(uiPSER_Option_Avbl & ExtraRinse_Yes)
						{
							bExtraRinse_Flag = TRUE;
							
							uikeypress_20mscount = RESET_VALUE;
							uiopt_check = ExtraRinse_Yes;
							SelectOption();	
							uikey_debounce_delay = 15;
							uiSelectedTone = KEYPRESSTONE;
							bKeyDetected = RESET_VALUE;
						}
						else
						{
							uiSelectedTone = INAPPLICABLE;
						}
						
					
						 
					}

					else if(uikeypress_20mscount < Spin_button_presstime)//190//99//25)//15 - ak changed for spin selection
					{
						if(bExtraRinse_Flag==TRUE)
						{
							
							if(uiPSER_Option_Avbl & ExtraRinse_Yes)
							{
								uiopt_check = ExtraRinse_Yes;
								//OptionSelection();
								SelectOption();	
								uikey_debounce_delay = 15;
								//uikeypress_20mscount = RESET_VALUE;
								//uiSelectedTone = KEYPRESSTONE;
								bKeyDetected = RESET_VALUE;
								//added to remove mismatch betn buzzer and display change
								if(prgm_key_bzzr_flag == 0)	//ADDED FOR REMOVING BUZZER DELAY - AK
								{
									uiTone_Selected = 1;
									uiSelectedTone = KEYPRESSTONE;
									prgm_key_bzzr_flag = TRUE;
								}
								else		//ADDED FOR REMOVING BUZZER DELAY - AK
								{
									uiTone_Selected = 4;
									uiSelectedTone = KEYPRESSTONE2;
									prgm_key_bzzr_flag = FALSE;
								}
								ToneGeneration();
							}
							else
							{
								//uikeypress_20mscount = RESET_VALUE;
								uiSelectedTone = INAPPLICABLE;
								//bKeyDetected = RESET_VALUE;
							}
						}
						else
						{
							if(uiSelected_Position != STEAMFRESH)
							{
								SpinSelection(); // select the desire RPM
								uikey_debounce_delay = 15;
								//bKeyDetected = RESET_VALUE;
								//added to remove mismatch betn buzzer and display change
								if(prgm_key_bzzr_flag == 0)	//ADDED FOR REMOVING BUZZER DELAY - AK
								{
									uiTone_Selected = 1;
									uiSelectedTone = KEYPRESSTONE;
									prgm_key_bzzr_flag = TRUE;
								}
								else		//ADDED FOR REMOVING BUZZER DELAY - AK
								{
									uiTone_Selected = 4;
									uiSelectedTone = KEYPRESSTONE2;
									prgm_key_bzzr_flag = FALSE;
								}
								ToneGeneration();
							}
							else
							{
								if((Switch_Reset != RESET_VALUE))
						 		uiTone_Selected = 2;
							}
						}
					}
				}
				else
				{
					uiTone_Selected = 2;
					bKeyDetected = RESET_VALUE;
				}
			}
			else
			{
				if(bChildLockButton == TRUE)
				{
					uichild_flash_count = 2;
				}
				uiTone_Selected = 2;
				
			}
			break;

//		case SELECT_KEY: // select key
//			
//			if(((bDebugMode_Selection == TRUE)||(bTestMode_Selection == TRUE)||(bFCTMode_Selection == TRUE))&&(bDoorLockError == FALSE))
//			{
//				if(bDebugMode_Selection == TRUE)
//				{
//					uidebug_key = SELECT_KEY; // for selecting switch case in debug mode
//					ucDbgSelectKey_count++;
//					ucDbgRpmKey_count= RESET_VALUE;
//					ucDbgOptionKey_count = RESET_VALUE;
//					ucDbgTempKey_count= RESET_VALUE;
//					if(ucDbgSelectKey_count > 6) //in debug mode we have only 3 different message have to show 
//					{
//						ucDbgSelectKey_count = 1;
//					}
//					uiTone_Selected = 1;
//					bKeyDetected = RESET_VALUE;
//				}
//				else if((bTestMode_Selection == TRUE)&&(bStartButton == TRUE)&&((uiTestModeState == SF01)||(uiTestModeState == SF02))&&(uiProgram_Position == 1)&&(uiMachineTestState < step2)&&(bFaciaTest != 1))
//				{
//					
//					uiSelection_testmode ^= 0x01;
//					TestMode_Fasia_led();
//					bKeyDetected = RESET_VALUE;
//					
//					uiTone_Selected = 1;
//				}
//				else if(bFCTMode_Selection == TRUE)
//				{
//					uiTone_Selected = 1;
//				}
//				
//				
//			}
//			else if((bChildLockButton !=1)&&(bStartButton == FALSE)) 
//			{
//				if((uiMachineState != PROGRAM_END)&&(Switch_Reset != FALSE)&&(MachineErrors.EReg == 0))
//				{
//					
//					if((uiIDLE_COUNT > 0)&& (uiopt_check != TimeSaver_Yes)
//						&&(((uiopt_check != WarmSoak_Yes)&&(uiopt_check != Steam_Yes)&&(uiopt_check != Prewash_Yes))|| uikeypress_20mscount <15))
//					{							
//						SelectOption();	// select the available options									
//						bKeyDetected = RESET_VALUE;
//						
//					}
//					else if((uiIDLE_COUNT > 0)&& (Timesaver_3sec_b != TRUE)
//						&&((uiopt_check == TimeSaver_Yes)&& (uikeypress_20mscount <15)))
//					{							
//						SelectOption();	// select the available options									
//						bKeyDetected = RESET_VALUE;
//						
//					}
//					else
//					{
//						if(uikeypress_20mscount >= 150)
//						{
//							if(uiPSER_Option_Avbl & TimeSaver_Yes)
//							{
//								/*********TimeSaver Selection Added**********/							
//								uiopt_check = TimeSaver_Yes;
//								uiIDLE_COUNT = 20;
//								SelectOption();	// select the available options
//								uikeypress_20mscount = RESET_VALUE;
//								uiSelectedTone = KEYPRESSTONE;
//								bKeyDetected = RESET_VALUE;
//								Timesaver_3sec_b = TRUE;
//							}
//							else
//							{
//								uiSelectedTone = INAPPLICABLE;
//								uikeypress_20mscount = RESET_VALUE;
//								bKeyDetected = RESET_VALUE;
//							}
//						}													
//						else
//						{
//							uiTone_Selected = 2;
//						}
//					}
//					
//					
//				}
//				else
//				{	if((Switch_Reset != RESET_VALUE))	
//						uiTone_Selected = 2;
//					
//				}
//				
//			}
//			else if((bChildLockButton !=1)&&(bStartButton == TRUE)) 
//			{
//				bKeyDetected = RESET_VALUE;
//				cSwitch_Pressed = RESET_VALUE;
//				uiTone_Selected = 2;
//			}
//			else
//			{
//				uiDISP_MCount = 0;
//				//bKeyDetected = RESET_VALUE;
//				if((uiMachineState != PROGRAM_END)&&(uiMachineState != SNOOZEMODE)&&(MachineErrors.EReg == 0))
//					DisplayTime(BalanceTime); //display balance time
//					
//				if(bChildLockButton == TRUE)
//				{
//					uichild_flash_count = 2;
//				}
//				uiTone_Selected = 2;
//				
//			}
//			break;
			case SELECT_KEY: // select key// to be used as steam button in neo series //ref: serena
		//	b_option_key_pressed_flag = 1; //to keep glowing the steam led once selected //commented here and added in option selection to solve flag activation during run mode
			if(((bDebugMode_Selection == TRUE)||(bTestMode_Selection == TRUE)||(bFCTMode_Selection == TRUE))&&(bDoorLockError == FALSE))
			{
				if(bDebugMode_Selection == TRUE)
				{
					uidebug_key = SELECT_KEY; // for selecting switch case in debug mode
					ucDbgSelectKey_count++;
					ucDbgRpmKey_count= RESET_VALUE;
					ucDbgOptionKey_count = RESET_VALUE;
					ucDbgTempKey_count= RESET_VALUE;
					if(ucDbgSelectKey_count > 7) //in debug mode we have only 3 different message have to show 
					{
						ucDbgSelectKey_count = 1;
					}
					uiTone_Selected = 1;
					bKeyDetected = RESET_VALUE;
				}
				//added to clear last 7 errors on steam  button 
//				else if((bTestMode_Selection == TRUE)&&(bStartButton == TRUE)&&((uiTestModeState == SF01)||(uiTestModeState == SF02))&&(uiProgram_Position == 1)&&(uiMachineTestState < step2)&&(bFaciaTest != 1))
//				{
//					
//					//uiSelection_testmode ^= 0x01;
//					uiSelection_testmode = 0x00; //ADDED FOR REMOVING TESTMODE TOGGLE OF BUTTON 
//					TestMode_Fasia_led();
//					bKeyDetected = RESET_VALUE;
//					
//					uiTone_Selected = 1;
//				}
				else if((bTestMode_Selection == TRUE)&&(bStartButton == TRUE))
				{
					if(((uiTestModeState == SF01)||(uiTestModeState == SF02))&&(uiProgram_Position == 1)&&(uiMachineTestState < step2)&&(bFaciaTest != 1))
					{
						//uiSelection_testmode ^= 0x01;
						uiSelection_testmode = 0x00; //ADDED FOR REMOVING TESTMODE TOGGLE OF BUTTON 
						TestMode_Fasia_led();
						bKeyDetected = RESET_VALUE;
						uiTone_Selected = 1;
					}
					else 
					{
						if((uiProgram_Position == CRADLEWASH)&&((uiTestModeState == SF01)||(uiTestModeState == SF02))&&(uiMachineTestState < step2))
						{
							if(uikeypress_20mscount >= 150)
							{
								Error_Storage[0] = Error_Storage[1] = Error_Storage[2] = Error_Storage[3]= Error_Storage[4] = Error_Storage[5] = Error_Storage[6] = RESET_VALUE;
								uikeypress_20mscount = RESET_VALUE;
								uiSelectedTone = KEYPRESSTONE;
							}
						}
						else
						{
							uiTone_Selected = 2;
						}
						//uitest_key = SELECT_KEY; // for selecting switch case in debug mode
					}
					
				}
				else if(bFCTMode_Selection == TRUE)
				{
					uiFCT_SW_Test |=0x01;
					uiTone_Selected = 1;
					bKeyDetected = RESET_VALUE;
				}
				
				
			}
			else if(bChildLockButton !=1)//&&(bStartButton == FALSE)) //commented as wifi reset should be available during program run as well
			{
				if((uiMachineState != PROGRAM_END)&&(Switch_Reset != FALSE)&&(MachineErrors.EReg == 0))
				{
					//WIFI reset shout be on every program hence changes implemented below
					Wifi_resettime = WIFIRESET_LONGPRESS();
					if((uikeypress_20mscount >= Wifi_resettime))//200//3 sec press count (20msec x 150)
					{
#if ENABLE_IOT					
						if((((BkMultiFuncSetting & AP_MODE_SETTING) == RESET_VALUE)&&(get_WiFiMode() != 0x01))&&((get_IOTMultiFuncSetting() & WIFI_ENABLE_DISABLE)== RESET_VALUE))
						{

							uiDISP_MCount = 70;
							set_IOTMultiFuncSetting(AP_MODE_SETTING);
							BkMultiFuncSetting = get_IOTMultiFuncSetting(); //new added
							turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
							mystrcpy((char *)cdigits_data_array, "  AP");
							update_display_data(&cdigits_data_array[0]);
							uiSelectedTone = KEYPRESSTONE;
							uikeypress_20mscount = RESET_VALUE;
							bKeyDetected = RESET_VALUE;
						}
						else
						{
							uiSelectedTone = INAPPLICABLE;
							uikeypress_20mscount = RESET_VALUE;
							bKeyDetected = RESET_VALUE;
						}
#endif					
					}
					else
					{
						if(bStartButton == FALSE)
						{
							if((uiProgram_Position == MIXEDSOIL || 
							((uiSelected_Position == SMARTPLUS)&&((uiSmartProgram_Selected == ANTIALLERGEN)||(uiSmartProgram_Selected == INNERWEAR)||(uiSmartProgram_Selected == EXPRESS_30)||(uiSmartProgram_Selected == SHIRTS)||(uiSmartProgram_Selected == UNIFORM)||((uiSmartProgram_Selected == LINENWASH)&&(uiTempSel != WashCold))||(uiSmartProgram_Selected == STEAMWASH)))||
							((uiProgram_Position == COTTON)&&(uiTempSel != Wash40E)&&(uiTempSel != Wash60E)) || (uiProgram_Position == BABYWEAR) || (uiProgram_Position == ADDITIVESRINSE)))//steam selection for dry programs added
							{
								if((/*(uiIDLE_COUNT > 0)&& */(uiopt_check != TimeSaver_Yes)
									&&(((uiopt_check != WarmSoak_Yes)&&(uiopt_check != Steam_Yes)&&(uiopt_check != Prewash_Yes))|| uikeypress_20mscount <15))&&(uiRinseholdState != 2))//steam selection not available in rinsehold state
								{	
									uiopt_check = Steam_Yes; //application timer passes steam 0x08 directly (line no. 387) // pased again bcz of extra rinse
									SelectOption();	// select the available options									
									bKeyDetected = RESET_VALUE;
									//added to remove mismatch betn buzzer and display change
									if(prgm_key_bzzr_flag == 0)	//ADDED FOR REMOVING BUZZER DELAY - AK
									{
										uiTone_Selected = 1;
										uiSelectedTone = KEYPRESSTONE;
										prgm_key_bzzr_flag = TRUE;
									}
									else		//ADDED FOR REMOVING BUZZER DELAY - AK
									{
										uiTone_Selected = 4;
										uiSelectedTone = KEYPRESSTONE2;
										prgm_key_bzzr_flag = FALSE;
									}
									ToneGeneration();
									
								}
								else if((/*(uiIDLE_COUNT > 0)&& */(Timesaver_3sec_b != TRUE)
									&&((uiopt_check == TimeSaver_Yes)&& (uikeypress_20mscount <15)))&&(uiRinseholdState != 2))//steam selection not available in rinsehold state
								{	
									uiopt_check = Steam_Yes;
									SelectOption();	// select the available options									
									bKeyDetected = RESET_VALUE;	
									//added to remove mismatch betn buzzer and display change
									if(prgm_key_bzzr_flag == 0)	//ADDED FOR REMOVING BUZZER DELAY - AK
									{
										uiTone_Selected = 1;
										uiSelectedTone = KEYPRESSTONE;
										prgm_key_bzzr_flag = TRUE;
									}
									else		//ADDED FOR REMOVING BUZZER DELAY - AK
									{
										uiTone_Selected = 4;
										uiSelectedTone = KEYPRESSTONE2;
										prgm_key_bzzr_flag = FALSE;
									}
									ToneGeneration();
								}
								else
								{
									uiTone_Selected = 2;
								}
							}
							else
							{
								uiTone_Selected = 2;
							}
						}
						else
						{
							uiTone_Selected = 2;
						}
						
					}
					
//					if((uiProgram_Position == MIXEDSOIL || 
//					((uiSelected_Position == SMARTPLUS)&&((uiSmartProgram_Selected == ANTIALLERGEN) || (uiSmartProgram_Selected == INNERWEAR)))||
//					((uiProgram_Position == COTTON)&&(uiTempSel != Wash40E)&&(uiTempSel != Wash60E)) || uiProgram_Position == BABYWEAR || uiProgram_Position == ADDITIVESRINSE ))
//					{
//						if(/*(uiIDLE_COUNT > 0)&& */(uiopt_check != TimeSaver_Yes)
//							&&(((uiopt_check != WarmSoak_Yes)&&(uiopt_check != Steam_Yes)&&(uiopt_check != Prewash_Yes))|| uikeypress_20mscount <15))
//						{	
//							uiopt_check = Steam_Yes; //application timer passes steam 0x08 directly (line no. 387) // pased again bcz of extra rinse
//							SelectOption();	// select the available options									
//							bKeyDetected = RESET_VALUE;
//							
//						}
//						else if(/*(uiIDLE_COUNT > 0)&& */(Timesaver_3sec_b != TRUE)
//							&&((uiopt_check == TimeSaver_Yes)&& (uikeypress_20mscount <15)))
//						{	
//							uiopt_check = Steam_Yes;
//							SelectOption();	// select the available options									
//							bKeyDetected = RESET_VALUE;
//							
//						}
//						else
//						{
//							if((uikeypress_20mscount >= 100))//100)) //3 sec press count (20msec x 150)
//							{
//#if ENABLE_IOT					
//								if((((BkMultiFuncSetting & AP_MODE_SETTING) == RESET_VALUE)&&(get_WiFiMode() != 0x01))&&((get_IOTMultiFuncSetting() & WIFI_ENABLE_DISABLE)== RESET_VALUE))
//								{
//
//									uiDISP_MCount = 70;
//									set_IOTMultiFuncSetting(AP_MODE_SETTING);
//									BkMultiFuncSetting = get_IOTMultiFuncSetting(); //new added
//									turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
//									mystrcpy((char *)cdigits_data_array, "  AP");
//									update_display_data(&cdigits_data_array[0]);
//									uiSelectedTone = KEYPRESSTONE;
//									uikeypress_20mscount = RESET_VALUE;
//									bKeyDetected = RESET_VALUE;
//								}
//								else
//								{
//									uiSelectedTone = INAPPLICABLE;
//									uikeypress_20mscount = RESET_VALUE;
//									bKeyDetected = RESET_VALUE;
//								}
//#endif					
//							}
//							
//						}
//					}
//					else
//					{
//						uiTone_Selected = 2;
//					}
						
				}
				else
				{	if((Switch_Reset != RESET_VALUE))	
						uiTone_Selected = 2;
					
				}
				
			}
			else if((bChildLockButton !=1)&&(bStartButton == TRUE)) 
			{
				//following block of code added to display lab mode cycles & program life cycles in run mode 		
				if((uiMachineState != PROGRAM_END)&&(uiMachineState != SNOOZEMODE)&&(MachineErrors.EReg == 0))
				{
					if(uikeypress_20mscount < 15)
					{
						if(bSpecialEnduranceSelected == TRUE)
						{
							if(uiSF8_cyclecompleted == RESET_VALUE || uiSF8_cyclecompleted > MAX_TEST_MODE_CYCLE)
							{
								mystrcpy((char *)cdigits_data_array, " 000");
								update_display_data(&cdigits_data_array[0] );
							}
							else
							{
										
								SevenSegPrint(uiSF8_cyclecompleted);
								cdigits_data[0] = (cdigits_data[0] & 0x80);//|(0x6D);
								for(ii = 0xC0; ii < 0xC3; ii= ii + 0x02)
								{
									STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // update the display
								}
							}
									//SevenSegPrint(uiSF8_cyclecompleted);
						}
						else
							{
								SevenSegPrint(ProgramLifeCycle);
							}

								uikey_debounce_delay = 15;
								turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
								uiDISP_MCount = 50;
								// bKeyDetected = TRUE; // need to check the impact
								uiTone_Selected = 1;
								//bKeyDetected = RESET_VALUE;
					}
				}
				else
				{
					bKeyDetected = RESET_VALUE;
					cSwitch_Pressed = RESET_VALUE;
					uiTone_Selected = 2;
				}
						
						
			}
			else
			{
				uiDISP_MCount = 0;
				//bKeyDetected = RESET_VALUE;
				if((uiMachineState != PROGRAM_END)&&(uiMachineState != SNOOZEMODE)&&(MachineErrors.EReg == 0))
					DisplayTime(BalanceTime); //display balance time
					
				if(bChildLockButton == TRUE)
				{
					uichild_flash_count = 2;
				}
				uiTone_Selected = 2;
				
			}
			break;
		case TIMESAVER_KEY:
				if(((bDebugMode_Selection == TRUE)||(bTestMode_Selection == TRUE)||(bFCTMode_Selection == TRUE))&&(bDoorLockError == FALSE))
				{
					if(bDebugMode_Selection == TRUE)
					{
						uidebug_key = TIMESAVER_KEY; // for selecting switch case in debug mode
						ucDbgTimesaverKey_count++;
						ucDbgSelectKey_count = RESET_VALUE;
						ucDbgRpmKey_count= RESET_VALUE;
						ucDbgOptionKey_count = RESET_VALUE;
						ucDbgTempKey_count= RESET_VALUE;
						if(ucDbgTimesaverKey_count > 7) //in debug mode we have to show last 7 errors
						{
							ucDbgTimesaverKey_count = 1;
						}
						uiTone_Selected = 1;
						bKeyDetected = RESET_VALUE;
					}
					else if((bTestMode_Selection == TRUE)&&(bStartButton == TRUE)&&((uiTestModeState == SF01)||(uiTestModeState == SF02))&&(uiProgram_Position == 1)&&(uiMachineTestState < step2)&&(bFaciaTest != 1))
					{
						
						//uiTimesaver_testmode ^= 0x01;
						uiTimesaver_testmode = 0x00;//testmode for timesave btn led
						TestMode_Fasia_led();
						bKeyDetected = RESET_VALUE;
						
						uiTone_Selected = 1;
					}
					else if(bFCTMode_Selection == TRUE)
					{
						uiFCT_SW_Test |=0x08;
						uiTone_Selected = 1;
						bKeyDetected = RESET_VALUE;
					}
					
					
				}
				else if((bChildLockButton !=1)&&(bStartButton == FALSE)) 
				{
					if((uiMachineState != PROGRAM_END)&&(Switch_Reset != FALSE)&&(MachineErrors.EReg == 0))
					{
						if(uikeypress_20mscount < 15)
						{
							if(uiPSER_Option_Avbl & TimeSaver_Yes)
								{
									/*********TimeSaver Selection Added**********/							
									uiopt_check = TimeSaver_Yes;
									uiIDLE_COUNT = 20;
									SelectOption();	// select the available options
									uikeypress_20mscount = RESET_VALUE;
									uiSelectedTone = KEYPRESSTONE;
									bKeyDetected = RESET_VALUE;
									Timesaver_3sec_b = TRUE;
								}
								else
								{
									uiSelectedTone = INAPPLICABLE;
									uikeypress_20mscount = RESET_VALUE;
									bKeyDetected = RESET_VALUE;
								}
						}
					}
					else
					{	if((Switch_Reset != RESET_VALUE))	
							uiTone_Selected = 2;
						
					}
				}
				else if((bChildLockButton !=1)&&(bStartButton == TRUE)) 
				{
					bKeyDetected = RESET_VALUE;
					cSwitch_Pressed = RESET_VALUE;
					uiTone_Selected = 2;
				}
				else
				{
					uiDISP_MCount = 0;
					//bKeyDetected = RESET_VALUE;
					if((uiMachineState != PROGRAM_END)&&(uiMachineState != SNOOZEMODE)&&(MachineErrors.EReg == 0))
						DisplayTime(BalanceTime); //display balance time
						
					if(bChildLockButton == TRUE)
					{
						uichild_flash_count = 2;
					}
					uiTone_Selected = 2;
					
				}
			break;
		case SOAK_KEY:
				if(((bDebugMode_Selection == TRUE)||(bTestMode_Selection == TRUE)||(bFCTMode_Selection == TRUE))&&(bDoorLockError == FALSE))
				{
					if(bDebugMode_Selection == TRUE)
					{
						uidebug_key = SOAK_KEY; // for selecting switch case in debug mode
						ucDbgSoakKey_count++;
						ucDbgTimesaverKey_count = RESET_VALUE;
						ucDbgSelectKey_count = RESET_VALUE;
						ucDbgRpmKey_count= RESET_VALUE;
						ucDbgOptionKey_count = RESET_VALUE;
						ucDbgTempKey_count= RESET_VALUE;
						if(ucDbgSoakKey_count > 6) //in debug mode we have only 3 different message have to show 
						{
							ucDbgSoakKey_count = 1;
						}
						uiTone_Selected = 1;
						bKeyDetected = RESET_VALUE;
					}
					else if((bTestMode_Selection == TRUE)&&(bStartButton == TRUE)&&((uiTestModeState == SF01)||(uiTestModeState == SF02))&&(uiProgram_Position == 1)&&(uiMachineTestState < step2)&&(bFaciaTest != 1))
					{
						
						//uiSelection_testmode ^= 0x01;
						uiOption_testmode = 0x00;//testmode for soak key
						TestMode_Fasia_led();
						bKeyDetected = RESET_VALUE;
						
						uiTone_Selected = 1;
					}
					else if(bFCTMode_Selection == TRUE)
					{
						uiTone_Selected = 1;
					}
					
					
				}
				else if((bChildLockButton !=1)&&(uiError_Occured == RESET_VALUE)) 
				{
					if((uiMachineState != PROGRAM_END)&&(uiMachineState != SNOOZEMODE)&&(MachineErrors.EReg == 0))
					{
						Tubclean_presstime = TUBCLEAN_LONGPRESS();
						if(uikeypress_20mscount >= Tubclean_presstime)
						{
							//if((uiProgram_Position == ADDITIVESRINSE) &&(TubClean_Activated == FALSE) &&(bProgramStart == FALSE)&&(uiMachineState != SNOOZEMODE))
							
							if((bProgramStart == FALSE)&&(uiMachineState != SNOOZEMODE)&&(bStartButton != TRUE))
							{
								 TubClean_Activated = FALSE;
								 bTub_Available = 1;
								 bTub_deselect = bTub_deselect ^ bTub_Available;
								 if(bTub_Available & bTub_deselect)
								 {
									 TubClean_Activated = TRUE;
									 
									 mystrcpy((char *)cdigits_data_array, " tUb");
									 update_display_data(&cdigits_data_array[0] );
									 turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
									 
									 uiSelectedTone = KEYPRESSTONE;
									 uimaxspeed_temp =  Wash95 | Rpm000;
									 uiTempSel = Wash60;
									 uiRpmSel = Rpm000;
									 uidisp_select_option = RESET_VALUE;
									// cdigits_data[5] = cdigits_data[5]& ~(0x3F); // for clear the selected option
									 uikeypress_20mscount = RESET_VALUE;
									 uiPSER_Option_Avbl =  0x00;
									 uiDISP_MCount = 0;
									 BalanceTime = TUBCLEAN90_TIME;
									 uidisplay_flash_count = RESET_VALUE;
									 bBuzzer_On = TRUE;
									 uiTone_Selected = 1;
									 bKeyDetected = RESET_VALUE;
									 uiProgram_Position = SMARTPLUS;
									 uiSmartProgram_Selected = TUBCLEAN;
								 }
								 else
								 {
									TubClean_Activated = FALSE;
									uiSelectedTone = 1;
									uikeypress_20mscount = RESET_VALUE;
									bKeyDetected = RESET_VALUE;
									//Reset_ProgParamter();
									// added to take back position mixed soil from start 
									uiProgram_Position = MIXEDSOIL;
									PROG_SW_1_5_count = 1;
									PROG_SW_6_10_count = 6;
									Reset_ProgParamter();
								 }
							}
							else
							{
								
								uiTone_Selected = 2;
							}
						
							 
						}
						else if(uikeypress_20mscount < 15)
						{
							if(uiPSER_Option_Avbl & WarmSoak_Yes)
							{
								uiopt_check = WarmSoak_Yes;
								//uiIDLE_COUNT = 20;
								SelectOption();	// select the available options
								//uikeypress_20mscount = RESET_VALUE;
								uikey_debounce_delay = 15;
								uiSelectedTone = KEYPRESSTONE;
								//bKeyDetected = RESET_VALUE;
							}
							else
							{
								uiSelectedTone = INAPPLICABLE;
								//uikeypress_20mscount = RESET_VALUE;
								//bKeyDetected = RESET_VALUE;
								//uikeypress_20mscount = RESET_VALUE;
								//bKeyDetected = RESET_VALUE;
							}
						}
					}
					else
					{	//if((Switch_Reset != RESET_VALUE))	
							uiTone_Selected = 2;
							bKeyDetected = RESET_VALUE;
						
					}
				}
				else
				{
					if(bChildLockButton == TRUE)
					{
						uichild_flash_count = 2;
					}
					uiTone_Selected = 2;
					
				}
				
			break;
//		case PROG_SW_1_5 :
//			if((bChildLockButton !=1)&&(uiMachineState != PROGRAM_END)&&(uiMachineState != SNOOZEMODE)&&(bDebugMode_Selection == FALSE))
//			{
//				if((uikeypress_20mscount >= 100)) //3 sec press count (20msec x 150)
//				{
//					//wifi to be updated
//				}
//				else if(uikeypress_20mscount < 15)
//				{
//					PROG_SW_1_5_flag = TRUE;
//					PROG_SW_1_5_count++;
//					if(PROG_SW_1_5_count > 5)
//					{
//						PROG_SW_1_5_count = 1;
//					}
//					PROG_SW_6_10_count = 5;
//					PROG_SW_6_10_flag = FALSE;
//					ProgramSelector();
//					uiSelectedTone = KEYPRESSTONE;
//				}
//			}
//			break;
//		case PROG_SW_6_10 :
//			if((bChildLockButton !=1)&&(uiMachineState != PROGRAM_END)&&(uiMachineState != SNOOZEMODE)&&(bDebugMode_Selection == FALSE))
//			{
//				if((uikeypress_20mscount >= 100)) //3 sec press count (20msec x 150)
//				{
//					//tubclean to be updated
////					if((uiProgram_Position == ADDITIVESRINSE) &&(TubClean_Activated == FALSE) &&(bProgramStart == FALSE)&&(uiMachineState != SNOOZEMODE))
////					{
////						TubClean_Activated = TRUE;
////						 
////						 mystrcpy((char *)cdigits_data_array, " tUb");
////						 update_display_data(&cdigits_data_array[0] );
////						 turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
////						 
////						 uiSelectedTone = KEYPRESSTONE;
////						 uimaxspeed_temp =  Wash95 | Rpm000;
////						 uiTempSel = Wash95;
////						 uiRpmSel = Rpm000;
////						 uidisp_select_option = RESET_VALUE;
////						 cdigits_data[5] = cdigits_data[5]& ~(0x3F); // for clear the selected option
////						 uikeypress_20mscount = RESET_VALUE;
////						 uiPSER_Option_Avbl =  0x00;
////						 uiDISP_MCount = 0;
////						 BalanceTime = TUBCLEAN90_TIME;
////						 uidisplay_flash_count = RESET_VALUE;
////						 bBuzzer_On = TRUE;
////						 uiTone_Selected = 1;
////						 bKeyDetected = RESET_VALUE;
////					}
////					
////					else
////					{
////						uiTone_Selected = 2;
////					}
//				}
//				else if(uikeypress_20mscount < 15)
//				{
//					PROG_SW_6_10_flag = TRUE;
//					PROG_SW_6_10_count++;
//					if(PROG_SW_6_10_count > 10)
//					{
//						PROG_SW_6_10_count = 6;
//					}
//					PROG_SW_1_5_count = 0;
//					PROG_SW_1_5_flag = FALSE;
//					ProgramSelector();
//					uiSelectedTone = KEYPRESSTONE;
//					
//				}
//			}
//			break;
		case TEMP_OPTION_KEY : // option + temp (debug mode ) //neo 10kg temp+spin
			
			if((bChildLockButton !=1)&&(uidelaymin == RESET_VALUE)&&(uiMachineState != SNOOZEMODE))
			{
				Debug_mode_presstime = DEBUG_LONGPRESS();
				if(uikeypress_20mscount >= Debug_mode_presstime)//200//100
				{	
					if(((bTestMode_Selection == TRUE)&& (uiTestModeState > SF03))||(bTestMode_Selection == FALSE))
					{
						if((MachineErrors.EReg == 0) || ((bNtcFailed == 1)||(MachineErrors.EBits.HeaterError == 1)))
						{
							bDebugMode_Selection ^= TRUE;
							uidisplay_flash_count = RESET_VALUE;
							uikeypress_20mscount = RESET_VALUE;
							uiDebugState = RESET_VALUE;
							uiSelectedTone = KEYPRESSTONE;
							// reset the all default values
							uidebug_key = RESET_VALUE; // for selecting switch case in debug mode
							//below reset added for storing last error first.
							ucDbgOptionKey_count = ucDbgTempKey_count = ucDbgRpmKey_count = ucDbgSelectKey_count =ucDbgTimesaverKey_count= RESET_VALUE;
							ucDbgRpmKey_count= RESET_VALUE;
							uiTFCTSeconds = ucDbgOptionKey_count = RESET_VALUE;
							ucDbgTempKey_count= RESET_VALUE;							
                           				 bKeyDetected = RESET_VALUE;
							bBuzzer_On = TRUE;
						 	uiTone_Selected = 1;
							turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
							turnon_display_OxyJetled(OXYJET_OFF);
							
						}
					}
				}
				
			}
			else
			{
				if((Switch_Reset != RESET_VALUE))
					uiTone_Selected = 2;
					
				bKeyDetected = RESET_VALUE;
				
			}
			break;
		case SPIN_TEMP_KEY: // spin + temp (child lock) //neo 10kg spin+steam
			
			uiDISP_MCount = 0;
			Childlock_presstime = CHILDLOCK_LONGPRESS();
			
			if((((bStartButton == TRUE) && (bDoorLock == SWITCH_ON))||((uiRinseholdState == 2)&&(bChildLockButton == 1))||((MachineErrors.EReg != 0)&&(bChildLockButton == 1)))&&(uiMachineState != POWEROFF)||((bChildLockButton == 1)&&(bDoorLockStatus == FALSE)))
	        	{
				if(uikeypress_20mscount >= Childlock_presstime)//100 during load sensing else 200
		        	{
					if(((bDebugMode_Selection == FALSE)&&(MachineErrors.EReg == 0))||((MachineErrors.EReg != 0)&&(bChildLockButton == 1)))
					{
						if(( bTestMode_Selection == TRUE))
						{
							if((uiTestModeState == SF03)||(uiTestModeState == SF04))
							{
								bChildLockButton ^=1 ; // chlidlock activation 
								bKeyDetected = RESET_VALUE;
								uikeypress_20mscount = 0;
								if(bChildLockButton == 1)
								{
									
									cdigits_data[5] =  cdigits_data[5] | (0x02); // // turn on the child lock led
								}
								else
								{
									cdigits_data[5] =  cdigits_data[5] & (~0x02); // // turn off the child lock led	
								}
								bBuzzer_On = TRUE;
								uiTone_Selected = 1;
							}
						}
						else
						{
							bBuzzer_On = TRUE;
							bChildLockButton ^=1 ; // chlidlock activation 
							uikeypress_20mscount = 0;
							bKeyDetected = RESET_VALUE;
							if(bChildLockButton == 1)
							{
								//EvalenaLeds.ChildLockStatus = LED_ON;	
								
								cdigits_data[5] =  cdigits_data[5] | (0x02); // // turn on the child lock led
							}
							else
							{
								cdigits_data[5] =  cdigits_data[5] & (~0x02); // // turn off the child lock led
								//EvalenaLeds.ChildLockStatus = LED_OFF;
							}
							uiTone_Selected = 1;
						}
						bKeyDetected = RESET_VALUE;
					}
					
					uikeypress_20mscount = 0;
				}
			}
			else
			{
				if((Switch_Reset != RESET_VALUE))
					uiTone_Selected = 2;
					
				bKeyDetected = RESET_VALUE;
			}
			break;
		case TEST_MODE_KEY:
			if((bChildLockButton !=1)&&(uiMachineState == POWEROFF)&&(Switch_Reset == RESET_VALUE))
			{
				Test_mode_Active = TRUE;
				uikeypress_20mscount = RESET_VALUE;
				bKeyDetected = TRUE;
				bBuzzer_On = TRUE;
#if ENABLE_IOT
			clr_IOTMultiFuncSetting(MACHINE_ON_OFF);
			BkMultiFuncSetting &= ~MACHINE_ON_OFF;
#endif				
			}
			
		break;
		case FCT_MODE_KEY:
			if((bChildLockButton !=1)&&(uiMachineState == POWEROFF)&&(Switch_Reset == RESET_VALUE))
			{
				Fct_mode_Active = TRUE;
				uikeypress_20mscount = RESET_VALUE;
				bKeyDetected = TRUE;
				bBuzzer_On = TRUE;
#if ENABLE_IOT
			clr_IOTMultiFuncSetting(MACHINE_ON_OFF);
			BkMultiFuncSetting &= ~MACHINE_ON_OFF;
#endif				
			}
			
		break;
		case LAB_MODE_KEY:
			if((bChildLockButton !=1)&&(uiMachineState == POWEROFF)&&(Switch_Reset == RESET_VALUE))
			{
				Lab_mode_Active = TRUE;
				uikeypress_20mscount = RESET_VALUE;
				bKeyDetected = TRUE;
				bBuzzer_On = TRUE;
#if ENABLE_IOT
			clr_IOTMultiFuncSetting(MACHINE_ON_OFF);
			BkMultiFuncSetting &= ~MACHINE_ON_OFF;
#endif				
			}
			
		break;
		case TEMP_SELECT_KEY: // select + temp (test mode)
		
			if(((uiMachineState== IDLE_START)&&(SatrtUpDelay > RESET_VALUE))||((uiMachineState== POWEROFF)&&(Switch_Reset == RESET_VALUE)&&(Test_mode_Active == TRUE)))
			{
				
				//if(TEST_FCT_Active > RESET_VALUE)
				{
				       if(uikeypress_20mscount >= 200)//100 //3 sec press count (20msec x 150)
				       {
					        //bKeyDetected = RESET_VALUE;
					        uikeypress_20mscount = 0;
					        Switch_Reset = TRUE;
						Test_mode_Active = FALSE;
					        uiSelected_Position = uiProgram_Position = 1;				
						uiTestModeState = SF01;
					     	bTestMode_Selection = TRUE;
						TEST_FCT_Active = RESET_VALUE;
						 bTESTMODE_START = RESET_VALUE;
						 uidisp_select_option = RESET_VALUE;
						  ckey_digit_data[0] = ckey_digit_data[0] & ~(0x3F); // clear the selected option
						  ckey_digit_data[1] = 0x00; // clear the options led
						  ckey_digit_data[2] = 0x00; // clear the options led
						 ProgSel_led();
						 uiMachineState = TESTMODE;
						 cdisp_count = TRUE;
						 bTest_Forward = FALSE;
						 //uiTone_Selected = 1;
						//turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
						 uidisplay_flash_count = 0;//reset the count for display message instantly 
						 bDisp_write = 0; // added for removing SF1 blinking
				       }
				      
				       
				}
				
			}
			/*
			else
			{
				uiTone_Selected = 2;
				 bKeyDetected = RESET_VALUE;
			}
			*/
			break;
		case SPIN_SELECT_KEY :
			
			
			if(((bChildLockButton !=1)&&(uiMachineState== IDLE_START)&&(SatrtUpDelay > RESET_VALUE))||((uiMachineState == POWEROFF)&&(Switch_Reset == RESET_VALUE)&&(Fct_mode_Active == TRUE)))
			{
				
				//if(TEST_FCT_Active > RESET_VALUE)
				{
					if(uikeypress_20mscount >= 130)//70 //3 sec press count (20msec x 150)
				        {
					       uikeypress_20mscount = 0;
					       // Activate the FCT Mode
					       bFCTMode_Selection = TRUE;
					       Fct_mode_Active = FALSE;
					       Switch_Reset = TRUE;
					       uiMachineState = FCTMODE;
					       TEST_FCT_Active = FALSE;
					       uiTone_Selected = 1;
					       uiFCTMode = RESET_VALUE;
					       uiTFCTSeconds = RESET_VALUE;
					       bKeyDetected = RESET_VALUE;
					      
				        }
				}
			}/*
			else
			{
				uiTone_Selected = 2;
				bKeyDetected = RESET_VALUE;
				
			}
			*/
			
		break;
		case LAB_MODE:
			if((bChildLockButton !=1)&&((uiMachineState== POWEROFF)&&(Switch_Reset == RESET_VALUE)&&(Lab_mode_Active == TRUE)))
			{
				if((bFCTMode_Selection == FALSE) &&(bTestMode_Selection == FALSE ))
				{
				       if(uikeypress_20mscount >= 140)//70//110) //3 sec press count (20msec x 150)
				       {
					       uiDISP_MCount = 100;
					       bBuzzer_On = TRUE;
					       bSpecialEnduranceSelected = TRUE;
					       uiTone_Selected = 1;
					       bKeyDetected = RESET_VALUE;
					       mystrcpy((char *)cdigits_data_array, "LAb-");
						update_display_data(&cdigits_data_array[0] );
						Switch_Reset = TRUE;
						Lab_mode_Active = FALSE;
						uiMachineState = RESTARTPOWER;
				       }
				}
				
			}/*
			else
			{
				 uiTone_Selected = 2;
				 bKeyDetected = RESET_VALUE;
			}*/
			
			break;
		case POWER_KEY :
			//uiTone_Selected = 1;
			//uikeypress_20mscount = RESET_VALUE;
			Power_OnOff();			
			
			  
			break;
		
		case 0x00 :
		case 0xFFFF :
			bKeyDetected = RESET_VALUE;
			break;
		
		default:	
			// bPressConfirmStart = YES;
			bKeyDetected = RESET_VALUE;
			
			uiTone_Selected = 2;
			
			break;
	}
	
	
	Key_Press_Tone();
	
}
//--------------------------------------------------------------------
/*****************************************************************************************************************
* Function Name: Clear_Option_INPause
* Description  : This function clears the options which are already executes
		& won't available in pause state.
* Arguments    : None
* Return Value : None
******************************************************************************************************************/
void Clear_Option_INPause()
{
		if((uiMachineState == PAUSE_PRESSED)||(uiMachineState == START_PRESSED)||(uiMachineState == MOTORFAILURE))
		{
			if(bPrewash_Start == TRUE)
			{
				uiOption_closed |=Prewash_Yes;
				if(( uidisp_select_option & Prewash_Yes) == RESET_VALUE)
					uiPSER_Option_Avbl =  uiPSER_Option_Avbl &(~ Prewash_Yes); // // clear the prewash option if stage passed
			}
			else
			{
				
				if(( uiPSER_Option_Avbl & Prewash_Yes) == RESET_VALUE)
					uiOption_closed |=Prewash_Yes;
			}
			
			if(bSoak_Start == TRUE)
			{
				uiOption_closed |=WarmSoak_Yes;
				if( (uidisp_select_option & WarmSoak_Yes) == RESET_VALUE)
					uiPSER_Option_Avbl =  uiPSER_Option_Avbl &(~ WarmSoak_Yes); // // clear the soak option if stage passed
			}
			else
			{
				
				if( (uiPSER_Option_Avbl & WarmSoak_Yes) == RESET_VALUE)
					uiOption_closed |=WarmSoak_Yes;
			}
			
			if(bExtraRinse_Start == TRUE)
			{
				uiOption_closed |=ExtraRinse_Yes;
				if( (uidisp_select_option & ExtraRinse_Yes) == RESET_VALUE)
					uiPSER_Option_Avbl =  uiPSER_Option_Avbl &(~ ExtraRinse_Yes); // // clear the Extrarinse option if stage passed
			}
			else
			{
				
				if( (uiPSER_Option_Avbl & ExtraRinse_Yes) == RESET_VALUE)
					uiOption_closed |=ExtraRinse_Yes;
			}
			
			if(bSteam_Start == TRUE)
			{
				uiOption_closed |=Steam_Yes;
				if( (uidisp_select_option & Steam_Yes) == RESET_VALUE)
					uiPSER_Option_Avbl =  uiPSER_Option_Avbl &(~ Steam_Yes); // // clear the Steam option if stage passed
			}
			else
			{
				
				if( (uiPSER_Option_Avbl & Steam_Yes) == RESET_VALUE)
					uiOption_closed |=Steam_Yes;
			}
			if(bTimeSaver_Start == TRUE)
			{
				uiOption_closed |=TimeSaver_Yes;
				if( (uidisp_select_option & TimeSaver_Yes) == RESET_VALUE)
					uiPSER_Option_Avbl =  uiPSER_Option_Avbl &(~ TimeSaver_Yes); // // clear the timesaver option if stage passed
			}else
			{
				
				if( (uiPSER_Option_Avbl & TimeSaver_Yes) == RESET_VALUE)
					uiOption_closed |=TimeSaver_Yes;
			}
			
			if(bDelay_Start1 == TRUE)
			{
				uiOption_closed |=DelayStart_Yes;
				if(( uidisp_select_option & DelayStart_Yes) == RESET_VALUE)
					uiPSER_Option_Avbl =  uiPSER_Option_Avbl &(~ DelayStart_Yes); // // clear the delay option if stage passed
			}
			else
			{
				if(( uiPSER_Option_Avbl & DelayStart_Yes) == RESET_VALUE)
					uiOption_closed |=DelayStart_Yes;
			}
			 // uioption_on = uiPSER_Option_Avbl & uioption_on;
		}
}


/****************************************************************************************************
* Function Name: Key_Press_Tone
* Description  : This function is to execute the selected buzzer tone.
* Arguments    : None
* Return Value : None
*****************************************************************************************************/
void Key_Press_Tone()
{
     if(mybuzzerdelay == 0)
     {
	if((bBuzzer_On == TRUE))
	{
		if(uiTone_Selected == 1)
		uiSelectedTone = KEYPRESSTONE;
		else if(uiTone_Selected == 2)
		uiSelectedTone = INAPPLICABLE;
		else if(uiTone_Selected == 3)
		uiSelectedTone = STARTPRESSED;
		else if(uiTone_Selected == 4)	//ADDED FOR REMOVING BUZZER DELAY - AK
		uiSelectedTone = KEYPRESSTONE2;
		else if(uiTone_Selected == 5)
		uiSelectedTone = ENCODERTONE;
		
		bBuzzer_On = FALSE;
	}
     }
}

/*********************************************************************************************
* Function Name: Power_OnOff
* Description  : This function implements to make display On/OFF &
		 Reset the program.
* Arguments    : None
* Return Value : None
***********************************************************************************************/
void Power_OnOff(void)
{
	
	static uint8_t ii;
	
	DI();
	if( cSwitch_Pressed  == POWER_KEY)
	{
		//***************** RESET KEY/ Switch OFF Machine Function************************//
		 
		chip_select = FALSE; 
		if((Switch_Reset == TRUE)&&(uiMachineState == SNOOZEMODE)&&(uiTubClean_Disp == FALSE))
		{
			bSnooze_Flag = TRUE;
			breset_flag = FALSE;
			uiMachineBefore_Poweroff = uiMachineState;
			sleep_mode = 200;
			//Switch_Reset = TRUE;
			//Test_count = RESET_VALUE;
			bKeyDetected = RESET_VALUE;
			
		}
		else
		{
			Switch_Reset = Switch_Reset^0x01;
			
			if(Switch_Reset == 0x00)
			{
				bDisp_write = 1; //added for neo//flashing fct or test mode
				bSpecialEnduranceSelected = FALSE;
				uiMachineBefore_Poweroff = uiMachineState;
				bDispalyoff_flag = FALSE;				
				MotorStop();
				uiMachineState = POWEROFF;
#if ENABLE_IOT
				set_IOTMultiFuncSetting(MACHINE_ON_OFF);
				BkMultiFuncSetting |= MACHINE_ON_OFF;
#endif				
			}
			else
			{ 
				uiMachineState = RESTARTPOWER;
#if ENABLE_IOT
				clr_IOTMultiFuncSetting(MACHINE_ON_OFF);
				BkMultiFuncSetting &= ~MACHINE_ON_OFF;
				
				bDisp_write = 0; //added for neo
#endif				
			}
		}
		bKeyDetected = RESET_VALUE;
	}
	
	EI();		    
}

