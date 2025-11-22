#include "Selection.h"
#include "STLED316S.h"
#include "r_cg_userdefine.h"
#include "ProgramSelector.h" 
#include "r_cg_port.h"
#include "WashOperations_Global.h"

//*****************************************************************//
//********************* global variables *************************//
extern boolean bStartButton,bChildLockButton,bRapidWashButton;
extern boolean bProgramStart,bKeyDetected,bOverHeat,bDelay_Start1;//bExpress_30_Activatted;
extern boolean bPrewash_Start,bHotRinse_Start,bExtraRinse_Start,bHotRinse_Selected;
extern boolean bDisplay_Flag,bDoorLock,bTimeSaver_Start,bFctCountStart;
extern boolean bSoak_Start,bProgram_Start,TubClean_Activated,bSteam_Start;
extern boolean bDrainPump,bPrewashValve,bMainWashValve,chip_select;
extern boolean bProgram_End,bDoorLockError,bDoorLockStatus,bDisplay_Flag;
extern boolean bTEST_COMPLETED,bTestMode_Selection,bFaciaTestStep;
extern boolean bBuzzer_On,bEmegency_door_pull,uiover_heat,bPhysical_DoorLock_Status;
extern boolean default_Set,bDebugMode_Selection,bProgram_End,uiReset_TempRPM;
extern boolean bFCTMode_Selection,bEnduranceStarted,bSpin_Start,bCold_Soak_Selected;
extern boolean bkeys_display_flag,bRinsehold_Finished,bFaciaTest,bZCD_Display;

extern boolean b_option_key_pressed_flag;
extern boolean bExtraRinse_Flag; //AK

extern uint8_t cdigits_data_array[6],uiMachineState,uiRinseholdState;
extern uint8_t ckey_digit_data[6],uiProgram_Position,uiTestModeState; 
extern uint8_t cdigits_data[7],FaciaState,cFlash_C,uiTFCTSeconds;
extern uint8_t uiSelected_Position,cFlash_Count,uiTone_Selected;
extern uint8_t uiPSER_Option_Avbl,uihigh_low_vtlg,uiTubClean_Disp;
extern uint8_t uimaxspeed_temp,uidisp_select_option,uiSmartProgram_Selected;
extern uint8_t uLedKeyData[3],temp_first_press,spin_first_press;
extern uint8_t uikey_debounce_delay,uidefault_kg,uilast_selected_option;
extern uint8_t uidoorflash_count,uioption_on,uichild_flash_count;
extern uint8_t cFlash_prog_led,uiTumblingCount,snoozestart_onoff;
extern uint8_t snooze_display,uitemp_onoff,uiError_Occured;

extern uint16_t uidisplay_flash_count,BalanceTime;
extern uint16_t uidelaymin,snoozemode_10minutes_counter;
extern uint16_t iWaterLevel,uiprev_delaymin;

extern uint8_t uiTemperature,uiRMP_Default;

extern uint16_t uikeypress_20mscount; //AK

//****************************************************************//
//****************** Global Function Prototype ******************//

extern void ResProg_led();
extern void ProgSel_led();
extern void Reinit_Stleddevice(void);
extern void Alldispled_oFF(void);
extern void Reset_ProgParamter();
//****************************************************************//
//****************** Local Function Prototype ******************//
void OptionSelection();
void TemperatureSelection();
void Check_LaundryAdd();

//*****************************************************************//
//********************* Local variables *************************//
boolean bDelay_Start,bprewash_selected,bTimeSaver_Selected;
boolean bEcoSelected,bFiveHundredMsec_flag;

uint8_t uiOption_closed,uisoak_press_count = 0;
uint8_t uiExtraRinse_Press_count,uiExtraRinse_Max;
uint8_t uiSTART_Led_on,uiDISP_MCount,uiIDLE_COUNT;
uint8_t uiopt_check = 0x10;
uint8_t uiSoak_Press_count;
uint8_t uiTempSel,cTub_CleanCount,uiRpmSel;
uint8_t uiExtraRinse_Press_Select,testing_var;
uint8_t uiTubClean_MSG_Count;
uint16_t uiTubclean_MSG_TMOUT_Count;

extern boolean LS_flag;
uint8_t dig_no,i=1;
extern uint16_t Display_off_testing;
#if ENABLE_IOT
#include"iot_user_config.h"
#include"iot_config.h"
extern boolean bDiagnostic_Set_Flag;
extern uint8_t BkMultiFuncSetting;
void ChkWifi_Symbol_On();
void ChkWifi_Symbol_Off();
#endif

/***********************************************************************************************************************
* Function Name: OptionSelection
* Description  : This function implements to make option available for perticular program.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void OptionSelection()
{
	
	if(uiopt_check ==0)
	{
		uiopt_check = 0x10;
		uiIDLE_COUNT = 200;
	}
	else
	{
		if(uiMachineState == PAUSE_PRESSED)
		{
			//uioption_on =  uioption_on &(~uiopt_check);
			
			if(uiIDLE_COUNT > 0)
				uiopt_check = uiopt_check << 1;			
			if(uiopt_check > 0x20)		//(uiopt_check > 0x10) Added as per PR-42354
			{
				uioption_on = uiopt_check = 0x01;
			}
			if(uiPSER_Option_Avbl != 0)
			{
				if((uiOption_closed & uiPSER_Option_Avbl ) != uiPSER_Option_Avbl){
				while((uiopt_check & uiOption_closed)!=0x00)
				{
					uiopt_check = uiopt_check << 1;
					
					if(uiopt_check > 0x20)		//(uiopt_check > 0x10) Added as per PR-42354
					{
						uiopt_check = 0x01;
					}
				}
				uiIDLE_COUNT = 200;
				uiTone_Selected = 1;
				uioption_on = uiopt_check  ;
				}
				else
				{
					uiIDLE_COUNT = 0;
					uiTone_Selected = 2;
				}
				
			}
			else
			{
				uioption_on = uiopt_check = 0;
				uiIDLE_COUNT = 0;
				uiTone_Selected = 2;
			}
		}
		else
		{
			if(uiIDLE_COUNT > 0)
				uiopt_check = uiopt_check << 1;
			
			if(uiopt_check > 0x20)		//(uiopt_check > 0x10) Added as per PR-42354
			{
				uioption_on = uiopt_check = 0x01;
			}
			uiIDLE_COUNT = 200;
			
			if(uiPSER_Option_Avbl != 0)
			{
				while(!(uiopt_check & uiPSER_Option_Avbl))
				{
					uiopt_check = uiopt_check << 1;
					
					if(uiopt_check > 0x20)		//(uiopt_check > 0x10) Added as per PR-42354
					{
						uiopt_check = 0x01;
					}
				}
			}
			else
			{
				uiopt_check = 0;
				uiIDLE_COUNT = 0;
				uiTone_Selected = 2;
			}
		
		}
	}
        
	if(uiopt_check == DelayStart_Yes)
	{
		uiDISP_MCount = 0x00;	
		if( uidelaymin > 0)
		{
			
			turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
		}
		else
		{
			
			turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
		}
	}
	
	else if(uiopt_check == ExtraRinse_Yes)
	{
		
		uiIDLE_COUNT = 200;
		uiDISP_MCount = 200;
		turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
		if(uiExtraRinse_Press_Select == 1)
		{
			mystrcpy((char *)cdigits_data_array, "0001");
		}
		else if(uiExtraRinse_Press_Select == 2)
		{
			mystrcpy((char *)cdigits_data_array, "0002");
		}
		else if(uiExtraRinse_Press_Select == 3)
		{
			mystrcpy((char *)cdigits_data_array, "0003");
		}
		else
		{
			mystrcpy((char *)cdigits_data_array, "0000");
		}
							
							
		update_display_data(&cdigits_data_array[0] );
						
	}
	else
	{
		uiDISP_MCount = RESET_VALUE;
		uidisplay_flash_count= RESET_VALUE;
	}
}
//--------------------------------------------------------------------

/***********************************************************************************************************************
* Function Name: TemperatureSelection
* Description  : This function implements to make tempearture available for perticular program.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void TemperatureSelection()
{
	if(TubClean_Activated == FALSE)
	{
		bKeyDetected = TRUE;
		if(bStartButton == RESET_VALUE)
		{		
			if(bProgram_Start == FALSE) // if program is not then able to change the temp else not
			{
				
				temp_first_press++;
				uiIDLE_COUNT = 1;
				if(temp_first_press > 0x01)
				{
					uiTempSel++;
					
					if(uiTempSel > (uimaxspeed_temp & 0x0F))
					{
						
						if(((uiProgram_Position == ADDITIVESRINSE)&&(TubClean_Activated == FALSE)) ||(/*(uiSelected_Position == SMARTPLUS)&&*/(uiSmartProgram_Selected == STEAMWASH))||(uiProgram_Position == STEAMFRESH) /*||  (bExpress_30_Activatted == TRUE)*/)
						{
							uiTempSel = RESET_VALUE;
							bKeyDetected = RESET_VALUE;
							uiDISP_MCount = 0;
							uiTone_Selected = 2;
						}
						else
						{
							uiTempSel = WashCold;
						}
					}
					if(uiTempSel == WashCold)
					{
						//if((uiProgram_Position == EXPRESS_30)||((uiProgram_Position == SMARTPLUS)&&((uiSmartProgram_Selected == INNERWEAR)||(uiSmartProgram_Selected == DARKWASH) || (uiSmartProgram_Selected == ANTIALLERGEN))))
						if((uiSelected_Position == SMARTPLUS) && ((uiSmartProgram_Selected == EXPRESS_30) || (uiSmartProgram_Selected == INNERWEAR)||(uiSmartProgram_Selected == DARKWASH) || (uiSmartProgram_Selected == ANTIALLERGEN)))
						{
							uiTempSel = Wash40;
						}
						else if((uiProgram_Position == SMARTPLUS)&&(uiSmartProgram_Selected == JEANS))
						{
							uiTempSel = Wash60;
						}
					}
					/*else if(uiTempSel == Wash20)
					{
						if((uiProgram_Position == BABYWEAR)||((uiProgram_Position == SMARTPLUS)&&((uiSmartProgram_Selected == CURTAINS)||(uiSmartProgram_Selected == SPORTWEAR)||(uiSmartProgram_Selected == UNIFORM))))  
						{
							uiTempSel = Wash40;
						}
						else if((uiProgram_Position == CRADLEWASH)||(uiProgram_Position == WOOLLENS)||(uiProgram_Position == EXPRESS)||((uiProgram_Position == SMARTPLUS)&&(uiSmartProgram_Selected == SHIRTS)))  
						{
							uiTempSel = Wash30;
						}
						else if(TubClean_Activated == TRUE)
						{
							uiTempSel = Wash60;
						}
					}*/
					else if(uiTempSel == Wash95)
					{
						if((uiProgram_Position == SYNTHETIC)|| (uiProgram_Position == MIXEDSOIL) || (uiProgram_Position == CURTAINS)|| (uiProgram_Position == BABYWEAR))
						{
							uiTempSel = P_COLD;
						}
						else if((uiProgram_Position == SMARTPLUS) &&  ((uiSmartProgram_Selected == UNIFORM) || (uiSmartProgram_Selected == SHIRTS)))
						{
							uiTempSel = P_COLD;
						}
						else if((uiProgram_Position == SMARTPLUS) &&  (uiSmartProgram_Selected == JEANS))
						{
							uiTempSel = P_60;
						}
					}
					else if(uiTempSel == Wash30) // BABYWEAR
					{
					if((uiProgram_Position == BABYWEAR)|| 
					(uiProgram_Position == CURTAINS)||
					((uiProgram_Position == SMARTPLUS)&&(/*(uiSmartProgram_Selected == CURTAINS)||*/(uiSmartProgram_Selected == SPORTWEAR)
					||(uiSmartProgram_Selected == DARKWASH)||
					(uiSmartProgram_Selected == UNIFORM))))  
						{
							uiTempSel = Wash40;
						}
					else if(TubClean_Activated == TRUE)
						{
							uiTempSel = Wash60;
						}
					}
					//below code added for smartplus prewash programs.
					else if(((uiTempSel > Wash40) && (uiTempSel < P_40)) && 
					((uiProgram_Position == SMARTPLUS) && (uiSmartProgram_Selected == DARKWASH)))
					{
						uiTempSel = P_40;
					}
					
					else if(((uiTempSel > Wash60) && (uiTempSel < P_60)) && 
					((uiProgram_Position == SMARTPLUS) && (uiSmartProgram_Selected == JEANS)))
					{
						uiTempSel = P_60;
					}
					
					else if(((uiTempSel > Wash40) && (uiTempSel < P_COLD)) && 
					((uiProgram_Position == SMARTPLUS) && (uiSmartProgram_Selected == SPORTWEAR)))
					{
						uiTempSel = P_COLD;
					}
					//PREWASH FOR ANTI-ALLERGEN SKIP PCOLD & P30C
					else if(((uiTempSel > Wash95) && (uiTempSel < P_40)) && 
					((uiProgram_Position == SMARTPLUS) && (uiSmartProgram_Selected == ANTIALLERGEN)))
					{
						uiTempSel = P_40;
					}
					
					else if((uiTempSel == P_30) && 
					((uiProgram_Position == SMARTPLUS) && (uiSmartProgram_Selected == SPORTWEAR)))
					{
						uiTempSel = P_40;
					}
					
					else if(((uiTempSel > Wash60) && (uiTempSel < P_COLD)) && 
					(((uiProgram_Position == SMARTPLUS) &&  (uiSmartProgram_Selected == UNIFORM))|| (uiProgram_Position == CURTAINS) || (uiProgram_Position == BABYWEAR) ))
					{
						uiTempSel = P_COLD;
					}
					else if((uiTempSel == P_30) && 
					(((uiProgram_Position == SMARTPLUS) &&  (uiSmartProgram_Selected == UNIFORM))|| (uiProgram_Position == CURTAINS)|| (uiProgram_Position == BABYWEAR)))
					{
						uiTempSel = P_40;
					}
					
					if((uiTempSel == Wash60)&&(uiProgram_Position == EXPRESS))
					{
						uiExtraRinse_Max = 2; // Added for BTRA
					}
					else if((uiTempSel != Wash60)&&(uiProgram_Position == EXPRESS))
					{
						uiExtraRinse_Max = 1; // Added for BTRA
					}
					if(((uiTempSel == Wash40E)||(uiTempSel == Wash60E))&&(uiProgram_Position == COTTON))
					{
						uiPSER_Option_Avbl =  WarmSoak_Yes | ExtraRinse_Yes| DelayStart_Yes ;
						uiExtraRinse_Max = 1;
						
						if(uidisp_select_option != DelayStart_Yes)
						{
							uidisp_select_option = RESET_VALUE ;
						}
						uilast_selected_option = RESET_VALUE;
						uiExtraRinse_Press_count = uiExtraRinse_Press_Select = RESET_VALUE;
						
						bCold_Soak_Selected = FALSE;
#if ENABLE_IOT						
						clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);
						clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_PREWASH);
						clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_TIMESAVER);
						clr_IOTOptionEnable(OPTION_ENABLE_2, OPTION_2_STEAM);
						clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_HOT_RINSE);
						bHotRinse_Selected = FALSE;
#endif
						
						//uiSoak_Press_count = RESET_VALUE;
						TurnOnSelected_Option();
					}
					else if((uiTempSel != Wash40E)&&(uiTempSel != Wash60E)&&(uiProgram_Position == COTTON))
					{
						uiPSER_Option_Avbl = Prewash_Yes | WarmSoak_Yes | ExtraRinse_Yes| TimeSaver_Yes | DelayStart_Yes | Steam_Yes;
						uiExtraRinse_Max = 3;
						//uiReset_TempRPM = FALSE;
						//RESET_Prog();
					}
					if((uiTempSel == WashCold)&&((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == LINENWASH)))
					{	
						uiPSER_Option_Avbl =   ExtraRinse_Yes |  DelayStart_Yes;
						ckey_digit_data[0] &= ~Steam_Yes;
						uidisp_select_option &= ~Steam_Yes;
#if ENABLE_IOT
						clr_IOTOptionEnable(OPTION_ENABLE_2, OPTION_2_STEAM);
#endif
						
					}
					else if((uiTempSel != WashCold)&&((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == LINENWASH)))
					{
						uiPSER_Option_Avbl =   ExtraRinse_Yes |  DelayStart_Yes | Steam_Yes;
						uidisp_select_option |= Steam_Yes;
						ckey_digit_data[0] |= Steam_Yes;
#if ENABLE_IOT
						set_IOTOptionEnable(OPTION_ENABLE_2, OPTION_2_STEAM);
#endif
					}
					
					else if(uiTempSel == WashCold)
					{
						uiPSER_Option_Avbl = uiPSER_Option_Avbl & (~WarmSoak_Yes);
						
						if(((uidisp_select_option & WarmSoak_Yes) == WarmSoak_Yes))//||(uiProgram_Position == COTTON))//default soak for cotton cold removed as per spec rev10 //taken from SL9kg
						{
							if((( uidisp_select_option & TimeSaver_Yes)!=TimeSaver_Yes))
							{
								bCold_Soak_Selected = TRUE;
#if ENABLE_IOT						
								set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_SOAK);
#endif					
							}
						}											
						ckey_digit_data[0] &= ~WarmSoak_Yes;
						uidisp_select_option &= ~WarmSoak_Yes;
#if ENABLE_IOT						
						clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);
#endif
					}
					else if((uiTempSel != WashCold)&&(uiTempSel != P_COLD)&&/*(uiTempSel <= Wash95)*/((uiTempSel != Wash40E) && (uiTempSel != Wash60E) )&&
					((uiProgram_Position == COTTON)||(uiProgram_Position == MIXEDSOIL)||(uiProgram_Position == SYNTHETIC)||(uiProgram_Position == BABYWEAR)|| (uiProgram_Position == CURTAINS)||
					(uiSmartProgram_Selected == SPORTWEAR)||(uiSmartProgram_Selected == UNIFORM)||(uiSmartProgram_Selected == SHIRTS)))//||(uiSmartProgram_Selected == CURTAINS)))
					{
						uiPSER_Option_Avbl = uiPSER_Option_Avbl | WarmSoak_Yes;
											
						if(bCold_Soak_Selected == TRUE)
						{
							//uidisp_select_option |= WarmSoak_Yes;
							uidisp_select_option &= ~WarmSoak_Yes;
							//ckey_digit_data[0] |= WarmSoak_Yes;
							bCold_Soak_Selected = FALSE;
#if ENABLE_IOT					
							//set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);
							clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_SOAK);
#endif						
						}
						
					}
#if ENABLE_IOT
					Display_TEMP(uiTempSel);
#endif				
				}
				
//				if((bExpress_30_Activatted == TRUE) && (uiSelected_Position == EXPRESS))
//					{
//						uiPSER_Option_Avbl = Steam_Yes;
//						uidisp_select_option |= Steam_Yes;
//						ckey_digit_data[3] = ckey_digit_data[3] | (0x02);
//					}
				
				if(((uiProgram_Position == ADDITIVESRINSE)&&(TubClean_Activated == FALSE))||(/*(uiSelected_Position == SMARTPLUS)&&*/(uiSmartProgram_Selected == STEAMWASH))||(uiProgram_Position == STEAMFRESH) /*|| (bExpress_30_Activatted == TRUE)*/)
				{
					uiTempSel = RESET_VALUE;
					bKeyDetected = RESET_VALUE;
					uiDISP_MCount = 0;
					uiTone_Selected = 2;
				}
				else
				{
					uiDISP_MCount = 200;
					uiTone_Selected = 1;
					Display_TEMP(uiTempSel); // show the temp on display 
					ProgramTimming(); // update the programming timing as per selected program
				}
				
			}
			else
			{
				if(((uiProgram_Position != ADDITIVESRINSE)||((uiProgram_Position == ADDITIVESRINSE)&&(TubClean_Activated == TRUE)))&&(/*(uiSelected_Position == SMARTPLUS)&&*/(uiSmartProgram_Selected != STEAMWASH))&&(uiProgram_Position != STEAMFRESH)/*&& (bExpress_30_Activatted == FALSE)*/)
				{
					Display_TEMP(uiTempSel); // show the temp on display 
					uiDISP_MCount = 200;
				}
				uiTone_Selected = 2;
			}
		}
		else
		{	
			if((uiMachineState != PROGRAM_END)&&(MachineErrors.EReg == 0))
			{
				if(((uiProgram_Position != ADDITIVESRINSE)||((uiProgram_Position == ADDITIVESRINSE)&&(TubClean_Activated == TRUE)))&&(/*(uiSelected_Position == SMARTPLUS)&&*/(uiSmartProgram_Selected != STEAMWASH))&&(uiProgram_Position != STEAMFRESH) /*&& (bExpress_30_Activatted == FALSE)*/)
				{
					uiDISP_MCount = 200;//50
					bKeyDetected = TRUE;
					Display_TEMP(uiTempSel); // show the temp on display while program running 
					
					// cdigits_data[5] =  cdigits_data[5] & (~0x40); 		// // turn off the time dots//
					
					uiTone_Selected = 1;
				}
				else
					uiTone_Selected = 2;
			}
			else
			{
				uiTone_Selected = 2;
			}
			
			
		}
	}
	else //TUBCLEAN  ACTIVATED
	{
		bKeyDetected = TRUE;
		if(bStartButton == RESET_VALUE)
		{		
			if(bProgram_Start == FALSE) // if program is not then able to change the temp else not
			{
				
				temp_first_press++;
				uiIDLE_COUNT = 1;
				if(temp_first_press > 0x01)
				{
					uiTempSel++;
					
					if(uiTempSel > (uimaxspeed_temp & 0x0F))
					{
						
						uiTempSel = WashCold;
						
					}
					if(uiTempSel == Wash30) 
					{
							uiTempSel = Wash60;
					}
					
#if ENABLE_IOT
					Display_TEMP(uiTempSel);
#endif				
				}
				
				
					uiDISP_MCount = 200;
					uiTone_Selected = 1;
					Display_TEMP(uiTempSel); // show the temp on display 
					ProgramTimming(); // update the programming timing as per selected program
				
				
			}
			else
			{
				//if(((uiProgram_Position != ADDITIVESRINSE)||((uiProgram_Position == ADDITIVESRINSE)&&(TubClean_Activated == TRUE)))&&(/*(uiSelected_Position == SMARTPLUS)&&*/(uiSmartProgram_Selected != STEAMWASH))&&(uiProgram_Position != STEAMFRESH)&& (bExpress_30_Activatted == FALSE))
				//{
					Display_TEMP(uiTempSel); // show the temp on display 
					uiDISP_MCount = 200;
				//}
				uiTone_Selected = 2;
			}
		}
		else
		{	
			if((uiMachineState != PROGRAM_END)&&(MachineErrors.EReg == 0))
			{
				//if(((uiProgram_Position != ADDITIVESRINSE)||((uiProgram_Position == ADDITIVESRINSE)&&(TubClean_Activated == TRUE)))&&(/*(uiSelected_Position == SMARTPLUS)&&*/(uiSmartProgram_Selected != STEAMWASH))&&(uiProgram_Position != STEAMFRESH) && (bExpress_30_Activatted == FALSE))
				//{
					uiDISP_MCount = 200;//50
					bKeyDetected = TRUE;
					Display_TEMP(uiTempSel); // show the temp on display while program running 
					
					// cdigits_data[5] =  cdigits_data[5] & (~0x40); 		// // turn off the time dots//
					
					uiTone_Selected = 1;
				//}
				//else
					//uiTone_Selected = 2;
			}
			else
			{
				uiTone_Selected = 2;
			}
			
			
		}
	}
	
}
/***********************************************************************************************************************
* Function Name: SpinSelection
* Description  : This function implements to make Spin available for perticular program.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void SpinSelection()
{
	if(((bStartButton!=STARTPRESSED)/*||(uiTumblingCount == RINSE_HOLD)*/)&&(uiTumblingCount != SPIN)&&(uiTumblingCount != ANTI_CREASE))
	{
		
	
		bKeyDetected = TRUE;
		spin_first_press++;
		uiIDLE_COUNT = 1;
		if(spin_first_press > 1)
		{
			uiRpmSel = uiRpmSel + 0x10;
			
			if(uiRpmSel > ( 0xF0 & uimaxspeed_temp))
			{
				if(bRinsehold_Finished == TRUE)
					uiRpmSel = Rpm000;
				else
					uiRpmSel = Rinsehold;
			}
			if((uiRpmSel == Rinsehold) && ((TubClean_Activated == TRUE)||(uiProgram_Position == STEAMFRESH)||(((uiProgram_Position == ADDITIVESRINSE)&&(TubClean_Activated == FALSE))&&(uiExtraRinse_Press_Select == RESET_VALUE)))) // spin dry program dont have rinse hold option
			{
				uiRpmSel = Rpm000;
			}
			if(( bTestMode_Selection == TRUE)&&((uiTestModeState == SF03)||(uiTestModeState == SF04)||(uiTestModeState == SF06)))
			{
				if(uiRpmSel == Rinsehold)
				{
					uiRpmSel = Rpm400;
				}
				
			}
			if((uiRinseholdState == 2) && (uiRpmSel == Rinsehold) )
			{
				uiRpmSel = Rpm000;
			}
			if((uiRpmSel == Rpm000) && (/*(uiSelected_Position == SMARTPLUS)&&*/((uiSmartProgram_Selected == STEAMWASH)/*||(bExpress_30_Activatted == TRUE)*/))) // steam wash program dont have no spin option
			{
				uiRpmSel = Rpm400;
			}
			/*if((uiSelected_Position == DRAINSPIN) && (uiRpmSel == Rpm000))
			{
				if(( uidisp_select_option & TimeSaver_Yes) ==  TimeSaver_Yes)
				{
					bTimeSaver_Selected = FALSE;
					bRapidWashButton = FALSE;
					uidisp_select_option = uidisp_select_option & (~TimeSaver_Yes);
					ckey_digit_data[0] = ckey_digit_data[0] &  (~TimeSaver_Yes); // deselcet the time saver option
#if ENABLE_IOT
						clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_TIMESAVER);
#endif
					
				}
			}*/
			if((uiProgram_Position != STEAMFRESH) && (uiRpmSel == Rpm000)
				&& (uiTumblingCount != STEAM_FILL  //TODO: If steam started then steam should not be deselct.
				&& uiTumblingCount != STEAM 
				&& uiTumblingCount != STEAM_CREASE 
				&& uiTumblingCount != STEAM_DRAIN) /*&& (bExpress_30_Activatted == FALSE)*/ /*&&(uiTumblingCount !=RINSE_HOLD)*/)//TODO: Added changes from prathmesh to deselect steam when at spin select 0
			{
				
				if(( uidisp_select_option & Steam_Yes) ==  Steam_Yes)
				{
					uidisp_select_option = uidisp_select_option & (~Steam_Yes);
					ckey_digit_data[0] = ckey_digit_data[0] &  (~Steam_Yes); // deselcet the steam option
#if ENABLE_IOT
					clr_IOTOptionEnable(OPTION_ENABLE_2, OPTION_2_STEAM);
					if(uiRinseholdState == 2)
					{
						//Below condition is added to remove the steam option if the rpm select is 000 or crosses it
						//The Steam_Yes = 0x08-->0000|1000-->to remove the steam optio  and it with 1111 | 0111
						uiPSER_Option_Avbl  = uiPSER_Option_Avbl & (~(1<<3)) ;
					}
					else
					{
						uiPSER_Option_Avbl = uiPSER_Option_Avbl;
						
					}
					
#endif					
				}
			}
		}
		uiDISP_MCount = 200;
		Display_RPM(uiRpmSel); // show selected RPM 
		ProgramTimming(); // update the time if spin cancle is selected 
		
		uiTone_Selected = 1;
		
		
	}
	else
	{
		if((uiMachineState != PROGRAM_END)&&(MachineErrors.EReg == 0))
		{
			bKeyDetected = TRUE;
			uiDISP_MCount = 200;//50
			Display_RPM(uiRpmSel); // display selected RPM while program running
			// cdigits_data[5] =  cdigits_data[5] & (~0x40); 		// // turn off the time dots
			uiTone_Selected = 1;	
		}
		else
		{
			uiTone_Selected = 2;
		}
	}
	
}
/***********************************************************************************************************************
* Function Name: SelectOption
* Description  : This function implements to make Select available option for perticular program.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void SelectOption()
{
	
	if((bStartButton!=STARTPRESSED)/*&&(uiIDLE_COUNT > 0)*/&&(TubClean_Activated == FALSE))
	{
		switch(uiopt_check)
		{
			case Prewash_Yes: // Prewash_Yes = 0x01
				uidisplay_flash_count= RESET_VALUE;
				if(bPrewash_Start == FALSE)
				{
					//reduced for fast deselection of option
					uiIDLE_COUNT = 20;//200;
					uidisp_select_option = uidisp_select_option^Prewash_Yes; 
					if( uidisp_select_option & Prewash_Yes)
					{
						
//						ckey_digit_data[0] = ckey_digit_data[0] | Prewash_Yes;
//						ckey_digit_data[1] = ckey_digit_data[1] | Prewash_Yes; // turn on prewash option led
//						ckey_digit_data[2] = ckey_digit_data[2] | Prewash_Yes; // turn on prewash option led
						//ckey_digit_data[0] = ckey_digit_data[0] | 0x02;
						bprewash_selected = TRUE;
#if ENABLE_IOT
						set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_PREWASH);
#endif
						uiTempSel = uiTempSel + APP_PREWASH_ON_OFF;
						Display_TEMP(uiTempSel);
					}
					else
					{	
//						ckey_digit_data[0] = ckey_digit_data[0] &  (~Prewash_Yes);
//						ckey_digit_data[1] = ckey_digit_data[1] & (~Prewash_Yes); // turn off prewash option led
//						ckey_digit_data[2] = ckey_digit_data[2] & (~Prewash_Yes); // turn off prewash option led
						//ckey_digit_data[0] = ckey_digit_data[0] & (~0x02);
						bprewash_selected = FALSE;
#if ENABLE_IOT
						clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_PREWASH);
#endif	
						uiTempSel = uiTempSel - APP_PREWASH_ON_OFF;
						Display_TEMP(uiTempSel);
					}
					uiTone_Selected = 1;
					
				}
				else
				{
					uiTone_Selected = 2;
				}
				
				
			break;
			case WarmSoak_Yes:
					uidisplay_flash_count= RESET_VALUE;
					b_option_key_pressed_flag = 1; //ADDED FOR ':AI' ISSUE 
					if(bSoak_Start == FALSE)
					{
						uiIDLE_COUNT = 20;//200;						
						//uiSoak_Press_count ++;
						/*if(uiSoak_Press_count > SOAK_MAX_COUNT)
						{
							uiSoak_Press_count = 0x00;
							uidisp_select_option = uidisp_select_option &(~WarmSoak_Yes);
						}
						else
						{*/
							uidisp_select_option = uidisp_select_option ^ WarmSoak_Yes;
						//}
						
						
						if( uidisp_select_option & WarmSoak_Yes)
						{
							 //cdigits_data[1] =  cdigits_data[1] | 0x80;
//							ckey_digit_data[0] = ckey_digit_data[0] | WarmSoak_Yes; // turn on navigation of Saok
//							ckey_digit_data[1] = ckey_digit_data[1] | WarmSoak_Yes; // turn on Soak option led
//							ckey_digit_data[2] = ckey_digit_data[2] | WarmSoak_Yes; // turn on Soak option led
//							ckey_digit_data[0] = ckey_digit_data[0] &  (~TimeSaver_Yes); // turn off navigation of Steam 
							uidisp_select_option = uidisp_select_option & (~TimeSaver_Yes);
#if ENABLE_IOT
							set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);
							//set_IOTSoakInMin(0x01);
							clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_TIMESAVER);
#endif
						}
						else
						{
							//cdigits_data[1] =  cdigits_data[1] & ~(0x80);
//							ckey_digit_data[0] = ckey_digit_data[0] &  (~WarmSoak_Yes); // turn off navigation of Soak
//							ckey_digit_data[1] = ckey_digit_data[1] & (~WarmSoak_Yes); // turn off Soak option led
//							ckey_digit_data[2] = ckey_digit_data[2] & (~WarmSoak_Yes); // turn off Soak option led
#if ENABLE_IOT
							clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);
							//set_IOTSoakInMin(0x00);
#endif
						}
						
						
					//	update_display_data(&cdigits_data_array[0] );	//ADDED FOR ':AI' ISSUE
						
						uiTone_Selected = 1;
					}
					else
					{
						uiTone_Selected = 2;
						
					}
				
			break;
			case ExtraRinse_Yes:
			
					if(bExtraRinse_Start == FALSE)
					{
						uiIDLE_COUNT = 200;
						uiDISP_MCount = 200;
						if(bExtraRinse_Flag == TRUE) //added to select on long press
						{
							uiExtraRinse_Press_count++;
						}
						if(uiExtraRinse_Press_count > uiExtraRinse_Max)
						{
							uiExtraRinse_Press_count = RESET_VALUE;
							uiExtraRinse_Press_Select = RESET_VALUE;
						}
						
						
						
						uiExtraRinse_Press_Select = uiExtraRinse_Press_count ;
						if( uiExtraRinse_Press_count >  0)
						{
							uidisp_select_option |=ExtraRinse_Yes;
//							ckey_digit_data[0] = ckey_digit_data[0] | ExtraRinse_Yes; // turn on navigation of ExtraRinse
//							ckey_digit_data[1] = ckey_digit_data[1] | ExtraRinse_Yes; // turn on ExtraRinse option led
//							ckey_digit_data[2] = ckey_digit_data[2] | ExtraRinse_Yes; // turn on ExtraRinse option led
							ckey_digit_data[1] = ckey_digit_data[1] | 0x02;
							
						}
						else
						{
							if(((uiProgram_Position == ADDITIVESRINSE)&&(TubClean_Activated == FALSE)) && (uiRpmSel == Rinsehold))
							{
								uiRpmSel = uiRMP_Default;
							}
							uidisp_select_option = uidisp_select_option & (~ExtraRinse_Yes);
							//cdigits_data[2] =  cdigits_data[2] & ~(0x80); 
//							ckey_digit_data[0] = ckey_digit_data[0] &  (~ExtraRinse_Yes); // turn off navigation of ExtraRinse
//							ckey_digit_data[1] = ckey_digit_data[1] & (~ExtraRinse_Yes); // turn off ExtraRinse option led
//							ckey_digit_data[2] = ckey_digit_data[2] & (~ExtraRinse_Yes); // turn off ExtraRinse option led
							ckey_digit_data[1] = ckey_digit_data[1] & (~0x02);
						}
						if(uiExtraRinse_Press_count == 1)
						{
							mystrcpy((char *)cdigits_data_array, " 001");
						}
						else if(uiExtraRinse_Press_count == 2)
						{
							mystrcpy((char *)cdigits_data_array, " 002");
						}
						else if(uiExtraRinse_Press_count == 3)
						{
							mystrcpy((char *)cdigits_data_array, " 003");
						}
						else
						{
							mystrcpy((char *)cdigits_data_array, " 000");
						}
						
						update_display_data(&cdigits_data_array[0] );
						turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
						uiTone_Selected = 1;
#if ENABLE_IOT
						set_IOTExtraRinse(uiExtraRinse_Press_count);
#endif
					}
					else
					{
						uiTone_Selected = 2;
					}
			
				
			break;
			case Steam_Yes:
				    	uidisplay_flash_count= RESET_VALUE;
					if((uiSelected_Position != STEAMFRESH)&&(/*(uiSelected_Position == SMARTPLUS)&&*/(uiSmartProgram_Selected != STEAMWASH )/*&& (bExpress_30_Activatted != TRUE)*/))
					{
				
						if(bSteam_Start == FALSE)
						{
							uiIDLE_COUNT = 20;//200;
							uidisp_select_option = uidisp_select_option^Steam_Yes;
							
							if( uidisp_select_option & Steam_Yes)
							{
								//cdigits_data[3] =  cdigits_data[3] | 0x80;
//								ckey_digit_data[0] = ckey_digit_data[0] | Steam_Yes; // turn on navigation of Steam
//								ckey_digit_data[1] = ckey_digit_data[1] | Steam_Yes; // turn on Steam option led
//								ckey_digit_data[2] = ckey_digit_data[2] | Steam_Yes; // turn on Steam option led
								ckey_digit_data[3] = ckey_digit_data[3] | 0x02;
								
								ckey_digit_data[4] = ckey_digit_data[4] &  (~0x02); // turn off navigation of Steam 
								uidisp_select_option = uidisp_select_option & (~TimeSaver_Yes);
								bTimeSaver_Selected = FALSE;
								b_option_key_pressed_flag = 1; //ENABLE WHEN OPTION IS SELECTED ONLY.
								if(uiRpmSel == Rpm000)
								{
									uiRpmSel = uiRMP_Default;
								}
#if ENABLE_IOT
								set_IOTOptionEnable(OPTION_ENABLE_2, OPTION_2_STEAM);
								clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_TIMESAVER);
#endif
							}
							else
							{
								
								//cdigits_data[3] =  cdigits_data[3] & ~(0x80);
//								ckey_digit_data[0] = ckey_digit_data[0] &  (~Steam_Yes); // turn off navigation of Steam
//								ckey_digit_data[1] = ckey_digit_data[1] & (~Steam_Yes); // turn off Steam option led
//								ckey_digit_data[2] = ckey_digit_data[2] & (~Steam_Yes); // turn off Steam option led
								ckey_digit_data[3] = ckey_digit_data[3] & (~0x02);
								
#if ENABLE_IOT
								clr_IOTOptionEnable(OPTION_ENABLE_2, OPTION_2_STEAM);
#endif
								
							}
							uiTone_Selected = 1;
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
					
			break;
			case TimeSaver_Yes:
					
					uidisplay_flash_count= RESET_VALUE;
					if(bTimeSaver_Start == FALSE)
					{
						/*if((uiSelected_Position == DRAINSPIN) && (uiRpmSel == Rpm000)){
							uiTone_Selected = 2;
							uiDISP_MCount = 0;
						}
						else*/{
							
							uiIDLE_COUNT = 20;//200;
							uidisp_select_option = uidisp_select_option^TimeSaver_Yes;
							uiDISP_MCount = 200;
							if( uidisp_select_option & TimeSaver_Yes)
							{
								
								bTimeSaver_Selected = TRUE;
								bRapidWashButton = TRUE;
								 
//								ckey_digit_data[0] = ckey_digit_data[0] | TimeSaver_Yes; // turn on navigation of HotRinse
//								ckey_digit_data[1] = ckey_digit_data[1] | TimeSaver_Yes; // turn on HotRinse option led
//								ckey_digit_data[2] = ckey_digit_data[2] | TimeSaver_Yes; // turn on HotRinse option led
								ckey_digit_data[4] = ckey_digit_data[4] |  (0x02);
								// timesaver is activated then deselect the hotrinse option
								ckey_digit_data[3] = ckey_digit_data[3] &  (~0x02); // turn off navigation of Steam 
								uidisp_select_option = uidisp_select_option & (~Steam_Yes);
								uidisp_select_option &= ~WarmSoak_Yes;
								ckey_digit_data[0] &= ~WarmSoak_Yes;
								bCold_Soak_Selected = FALSE;
								//cdigits_data[4] =  cdigits_data[4] | 0x01 ;
#if ENABLE_IOT								
								set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_TIMESAVER);
								clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_HOT_RINSE);
								clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);
								clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_SOAK);								
								bHotRinse_Selected = FALSE;
								clr_IOTOptionEnable(OPTION_ENABLE_2, OPTION_2_STEAM);									
#endif
								
							}
							else
							{
								bTimeSaver_Selected = FALSE;
								bRapidWashButton = FALSE;
//								ckey_digit_data[0] = ckey_digit_data[0] & (~TimeSaver_Yes); // turn off navigation of HotRinse
//								ckey_digit_data[1] = ckey_digit_data[1] & (~TimeSaver_Yes); // turn off HotRinse option led
//								ckey_digit_data[2] = ckey_digit_data[2] & (~TimeSaver_Yes); // turn off HotRinse option led
								ckey_digit_data[4] = ckey_digit_data[4] &  (~0x02);
#if ENABLE_IOT
								clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_TIMESAVER);
#endif
							}
							uiTone_Selected = 1;
							uiDISP_MCount = 0;
						}
					}
					else
					{
						uiTone_Selected = 2;
					}
			break;
			case DelayStart_Yes:
				
				if(bDelay_Start1 == FALSE)
				{
					uiIDLE_COUNT = 200;
					uidelaymin = AddDelay(uidelaymin);
					DisplayTime(uidelaymin);
					uiDISP_MCount = 100;
					//ckey_digit_data[0] = ckey_digit_data[0] | 0x01 ; // turn on the time dots
					if(uidelaymin > 0)
					{
						bDelay_Start =  TRUE;
						//cdigits_data[4] =  cdigits_data[4]  | 0x02;
						uidisp_select_option = uidisp_select_option | DelayStart_Yes;
//						ckey_digit_data[0] = ckey_digit_data[0] | DelayStart_Yes; // turn on navigation of DelayStart
//						ckey_digit_data[1] = ckey_digit_data[1] | DelayStart_Yes; // turn on DelayStart option led
//						ckey_digit_data[2] = ckey_digit_data[2] | DelayStart_Yes; // turn on DelayStart option led
						turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
#if ENABLE_IOT
						set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_DELAYSTART);
#endif
					}
					else
					{
						bDelay_Start =  FALSE;
						//cdigits_data[4] =  cdigits_data[4] & ~(0x02);
						
						uidisp_select_option = uidisp_select_option &(~DelayStart_Yes);
//						ckey_digit_data[0] = ckey_digit_data[0] & (~DelayStart_Yes); // turn off navigation of HotRinse
//						ckey_digit_data[1] = ckey_digit_data[1] & (~DelayStart_Yes); // turn off HotRinse option led
//						ckey_digit_data[2] = ckey_digit_data[2] & (~DelayStart_Yes); // turn off HotRinse option led
						turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
#if ENABLE_IOT
						clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_DELAYSTART);
#endif
					}
					uiTone_Selected = 1;
					bKeyDetected = RESET_VALUE;
				}
				else
				{
					uiTone_Selected = 2;
					
				}
				
			break;
		}
		TurnOnSelected_Option();
		ProgramTimming();
	}
	else
	{
		uiTone_Selected = 2;
		
	}
}
/*******************************************************************************************************
* Function Name: AddDelay
* Description  : This function implements to set the amount of delay.
* Arguments    : uint16_t
* Return Value : uint16_t
********************************************************************************************************/
uint16_t AddDelay(uint16_t uiprev_delaymin)
{
	if(uiprev_delaymin < 300)
	{
		uiprev_delaymin = uiprev_delaymin + 30;
	}
	else if((uiprev_delaymin >= 300)&&(uiprev_delaymin < 1140)) //720
	{
		uiprev_delaymin = uiprev_delaymin + 60;
	}
//	else if((uiprev_delaymin >= 720)&&(uiprev_delaymin <= 1440))
//	{
//		uiprev_delaymin = uiprev_delaymin + 120;
//	}
	
	if(uiprev_delaymin > 1440)
	{
		uiprev_delaymin = 0;
	}
	
	
	return uiprev_delaymin;
}

/*******************************************************************************************************
* Function Name: Display_TEMP
* Description  : This function implements to show available temperature for different programs.
* Arguments    : uint8_t
* Return Value : None
********************************************************************************************************/
//void Display_TEMP(uint8_t selected_temp)
//{
//	switch(selected_temp)
//	{
//		case  WashCold:
//			mystrcpy((char *)cdigits_data_array, " CLd");
//			break;
//			
//		/*case  Wash20:
//			mystrcpy((char *)cdigits_data_array, " 20C");
//			break;*/	
//			
//		case  Wash30:
//			mystrcpy((char *)cdigits_data_array, " 30C");
//			break;
//			
//		case  Wash40:
//			mystrcpy((char *)cdigits_data_array, " 40C");
//			break;
//			
//		case  Wash60:
//			mystrcpy((char *)cdigits_data_array, " 60C");
//			break;
//			
//		case Wash95:
//			mystrcpy((char *)cdigits_data_array, " 95C");
//			break;
//			
//		case Wash40E:
//			mystrcpy((char *)cdigits_data_array, " 40E");
//			break;
//		
//		case Wash60E:
//			mystrcpy((char *)cdigits_data_array, " 60E");
//			break;	
//			
//		case 0x00:
//			
//			if(bStartButton == FALSE)
//			{
//				uiDISP_MCount = 0;
//				bKeyDetected = FALSE;
//				
//			}
//			else{
//				uiDISP_MCount = 0;
//				bKeyDetected = FALSE;
//				uidisplay_flash_count = 200;
//				DisplayTime(BalanceTime); //display balance time
//			}
//			break;
//	}
//	
//	update_display_data(&cdigits_data_array[0] );	
//	
//	if(selected_temp != 0x00)
//		turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_ON,TIMELEFT_OFF);
//}

void Display_TEMP(uint8_t selected_temp)
{
	switch(selected_temp)
	{
		case  WashCold:
			uidisp_select_option = uidisp_select_option& (~0x01);
			ckey_digit_data[0] = ckey_digit_data[0] & (~0x02);
			mystrcpy((char *)cdigits_data_array, " CLd"); //strcpy(cdigits_data_array, "CoLd");
			bprewash_selected = FALSE;
			break;
		case  Wash30:
			ckey_digit_data[0] = ckey_digit_data[0] & (~0x02);
			uidisp_select_option = uidisp_select_option & (~0x01);
			mystrcpy((char *)cdigits_data_array, " 30C"); //strcpy(cdigits_data_array, " 30C");
			bprewash_selected = FALSE;
			break;
		case  Wash40:
			ckey_digit_data[0] = ckey_digit_data[0] & (~0x02);
			uidisp_select_option = uidisp_select_option& (~0x01);
			mystrcpy((char *)cdigits_data_array, " 40C"); //strcpy(cdigits_data_array, " 40C");
			bprewash_selected = FALSE;
			break;
		case  Wash60:
			ckey_digit_data[0] = ckey_digit_data[0] & (~0x02);
			uidisp_select_option = uidisp_select_option& (~0x01);
			mystrcpy((char *)cdigits_data_array, " 60C"); //strcpy(cdigits_data_array, " 60C");
			bprewash_selected = FALSE;
			break;
		case Wash95:
			ckey_digit_data[0] = ckey_digit_data[0] & (~0x02);
			uidisp_select_option = uidisp_select_option& (~0x01);
			mystrcpy((char *)cdigits_data_array, " 95C"); //strcpy(cdigits_data_array, " 95C");
			bprewash_selected = FALSE;
			break;
		case  Wash40E:
			ckey_digit_data[0] = ckey_digit_data[0] & (~0x02);
			uidisp_select_option = uidisp_select_option& (~0x01);
			mystrcpy((char *)cdigits_data_array, " 40E"); //strcpy(cdigits_data_array, " 40C");
			bprewash_selected = FALSE;
			break;
		case  Wash60E:
			ckey_digit_data[0] = ckey_digit_data[0] & (~0x02);
			uidisp_select_option = uidisp_select_option& (~0x01);
			mystrcpy((char *)cdigits_data_array, " 60E"); //strcpy(cdigits_data_array, " 40C");
			bprewash_selected = FALSE;
			break;
		case P_COLD:
			uidisp_select_option = uidisp_select_option|0x01;
			ckey_digit_data[0] = ckey_digit_data[0] | (0x02);
			mystrcpy((char *)cdigits_data_array, " P0C"); //strcpy(cdigits_data_array, "P-0C");
			bprewash_selected = TRUE;
			b_option_key_pressed_flag = 1;
			break;
		case P_30:
			ckey_digit_data[0] = ckey_digit_data[0] | (0x02);
			uidisp_select_option = uidisp_select_option|0x01;
			mystrcpy((char *)cdigits_data_array, " P30"); //strcpy(cdigits_data_array, "P30C");
			bprewash_selected = TRUE;
			b_option_key_pressed_flag = 1;
			break;
		case P_40:
			ckey_digit_data[0] = ckey_digit_data[0] | (0x02);
			uidisp_select_option = uidisp_select_option|0x01;
			mystrcpy((char *)cdigits_data_array, " P40"); //strcpy(cdigits_data_array, "P40C");
			bprewash_selected = TRUE;
			b_option_key_pressed_flag = 1;
			break;
		case P_60:
			ckey_digit_data[0] = ckey_digit_data[0] | (0x02);
			uidisp_select_option = uidisp_select_option|0x01;
			mystrcpy((char *)cdigits_data_array, " P60"); //strcpy(cdigits_data_array, "P60C");
			bprewash_selected = TRUE;
			b_option_key_pressed_flag = 1;
			break;
		case P_95:
			ckey_digit_data[0] = ckey_digit_data[0] | (0x02);
			uidisp_select_option = uidisp_select_option|0x01;
			mystrcpy((char *)cdigits_data_array, " P95"); //strcpy(cdigits_data_array, "P95C");
			bprewash_selected = TRUE;
			b_option_key_pressed_flag = 1;
			break;
		case 0x00:
			
			if(bStartButton == FALSE)
			{
				uiDISP_MCount = 0;
				bKeyDetected = FALSE;
				
			}
			else
			{
				uiDISP_MCount = 0;
				bKeyDetected = FALSE;
				uidisplay_flash_count = 200;
				DisplayTime(BalanceTime); //display balance time
			}
			break;
	}
#if ENABLE_IOT
	if(selected_temp < P_COLD)	//added for updating prewash flag  on app either selected on app or keys
	{
		clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_PREWASH);
	}
	else
	{
		set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_PREWASH);
	}
#endif
	
	update_display_data(&cdigits_data_array[0] );	
	
	if(selected_temp != 0x00)
		turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_ON,TIMELEFT_OFF);
}

/*******************************************************************************************************
* Function Name: Display_RPM
* Description  : This function implements to show available RPM for different programs.
* Arguments    : uint8_t
* Return Value : None
********************************************************************************************************/
void Display_RPM(uint8_t selected_rpm)
{
	
	uint8_t Rpm_zero;

	switch(selected_rpm)
	{
		
		case Rinsehold:
			//mystrcpy((char *)cdigits_data_array, "t==#");
			mystrcpy((char *)cdigits_data_array, " t=#");
			Rpm_zero = FALSE;
			
		break;
		case  Rpm000:
			mystrcpy((char *)cdigits_data_array, "   0");
			Rpm_zero = TRUE;

			
		break;
		case  Rpm400:
			mystrcpy((char *)cdigits_data_array, " 400");
			Rpm_zero = FALSE;

			
		break;
		case  Rpm600:
			mystrcpy((char *)cdigits_data_array, " 600");
			Rpm_zero = FALSE;

			
		break;
		case  Rpm800:
			mystrcpy((char *)cdigits_data_array, " 800");
			Rpm_zero = FALSE;

			
		break;
		case Rpm1000:
			mystrcpy((char *)cdigits_data_array, "1000");
			Rpm_zero = FALSE;

		break;
		case Rpm1200:
			mystrcpy((char *)cdigits_data_array, "1200");
			Rpm_zero = FALSE;

		break;
		
//		case Rpm1400:
//			mystrcpy((char *)cdigits_data_array, "1400");
//			Rpm_zero = FALSE;
//
//		break;
		
	}
	if(Rpm_zero == TRUE)
	{
		turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_OFF);
	}else{
		turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_ON,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);}
	
	update_display_data(&cdigits_data_array[0] );
}

/*******************************************************************************************************
* Function Name: Display_KG
* Description  : This function implements to show available capacity for different programs.
* Arguments    : uint8_t
* Return Value : None
********************************************************************************************************/
void Display_KG(uint8_t avaible_kg)
{
	uint8_t ii;
	switch(avaible_kg)
	{
		case  10:
			mystrcpy((char *)cdigits_data_array, " 10 ");					
			break;
		case  15:
			mystrcpy((char *)cdigits_data_array, " 15 ");					
			break;
		case  20:
			mystrcpy((char *)cdigits_data_array, " 20 ");					
			break;
		case  25:
			mystrcpy((char *)cdigits_data_array, " 25 ");					
			break;
		case  30:
			mystrcpy((char *)cdigits_data_array, " 30 ");					
			break;
		case  35:
			mystrcpy((char *)cdigits_data_array, " 35 ");					
			break;
		case  40:
			mystrcpy((char *)cdigits_data_array, " 40 ");					
			break;
		case  45:
			mystrcpy((char *)cdigits_data_array, " 45 ");					
			break;
		case  50:
			mystrcpy((char *)cdigits_data_array, " 50 ");					
			break;
		case  55:
			mystrcpy((char *)cdigits_data_array, " 55 ");					
			break;
		case  60:
			mystrcpy((char *)cdigits_data_array, " 60 ");
			break;
		case  65:
			mystrcpy((char *)cdigits_data_array, " 65 ");
			break;
		case  70:
			mystrcpy((char *)cdigits_data_array, " 70 ");					
			break;
		case  75:
			mystrcpy((char *)cdigits_data_array, " 75 ");
			break;
		case  80:
			mystrcpy((char *)cdigits_data_array, " 80 ");
			break;
		case  85:
			mystrcpy((char *)cdigits_data_array, " 85 ");
			break;
		case  90:
			mystrcpy((char *)cdigits_data_array, " 90 ");
			break;
		case  100:
			mystrcpy((char *)cdigits_data_array, "100 ");
			break;
		case 00:
			mystrcpy((char *)cdigits_data_array, " 80 ");
			//uidisplay_flash_count = 201;
			break;
		
	}
	
	update_display_data(&cdigits_data_array[0] );
	turnon_display_led(TIMEDOTS_OFF,SEMI_ON,KG_ON,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
	//update_display_data(&cdigits_data_array[0] );
	//THE BELOW LINES ARE ADDED FOR TGESTING
//	for(ii = 0xC0; ii < 0xCD; ii= ii + 0x02)
//		{
//			STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // this is should not be 0x00 , default array have to written
//			
//		}
}

void display_availoption(uint8_t uiOption_Avbl)
{
	testing_var = uiOption_Avbl;
	// EXPRESS 30 STEAM LED SHOULD GLOW AND NOT BLINK HENCE ADDED SAME FOR REFERESH(STEAMFRESH) PROGRAMD
	if((bDisplay_Flag == TRUE)&&(bZCD_Display == TRUE))// /*&& (bExpress_30_Activatted == FALSE)*/ && (uiProgram_Position != STEAMFRESH) )
	{
		
		if(uiOption_Avbl & Prewash_Yes)
		{
			ckey_digit_data[0] = ckey_digit_data[0] | (0x02);
			
		}
		else
		{
			ckey_digit_data[0] = ckey_digit_data[0] & (~0x02);
		}
		
		if( uiOption_Avbl & WarmSoak_Yes)
		{
//			 ckey_digit_data[1] = ckey_digit_data[1] | WarmSoak_Yes;
//			ckey_digit_data[2] = ckey_digit_data[2] | WarmSoak_Yes;
		}
		else
		{
//			 ckey_digit_data[1] = ckey_digit_data[1] & (~WarmSoak_Yes); // 
//			ckey_digit_data[2] = ckey_digit_data[2] & (~WarmSoak_Yes); // 
		}
		
		if( uiOption_Avbl & ExtraRinse_Yes)
		{
			 ckey_digit_data[1] = ckey_digit_data[1] | (0x02);
		}
		else
		{
			
			ckey_digit_data[1] = ckey_digit_data[1] & (~0x02);
		}
		
		if( uiOption_Avbl & Steam_Yes)
		{
			 ckey_digit_data[3] = ckey_digit_data[3] | (0x02);
		}
		else
		{
			 ckey_digit_data[3] = ckey_digit_data[3] & (~0x02);
		}
		
		if( uiOption_Avbl & TimeSaver_Yes)
		{
			 ckey_digit_data[4] = ckey_digit_data[4] | (0x02);
		}
		else
		{
			 ckey_digit_data[4] = ckey_digit_data[4] & (~0x02);
		}
		
		if( uiOption_Avbl & DelayStart_Yes) 
		{
//			ckey_digit_data[1] = ckey_digit_data[1] | DelayStart_Yes;
//			ckey_digit_data[2] = ckey_digit_data[2] | DelayStart_Yes;
		}
		else
		{
//			ckey_digit_data[1] = ckey_digit_data[1] & (~DelayStart_Yes); //
//			ckey_digit_data[2] = ckey_digit_data[2] & (~DelayStart_Yes); //
		}
		
		
		
		
		if((uiMachineState != PROGRAM_END)&& (uiMachineState == START_PRESSED)&& (MachineErrors.EReg == 0))
			TurnOnSelected_Option();
			
		bDisplay_Flag = FALSE;
	}
}

void turnon_display_led(char time_dots,char semicolan,char kg,char rpm,char no_spin,char centigrade,char time_left)
{
	if(time_dots == 1)
	{
		//cdigits_data[5] =  cdigits_data[5] |  (0x04); // // turn on the time dots
		cdigits_data[4] =  cdigits_data[4] | (0x40);//neo P1 dot changes
	}
	else
	{
		//cdigits_data[5] =  cdigits_data[5] & (~0x04); 		// // turn off the time dots
		cdigits_data[4] =  cdigits_data[4] & (~0x40); //neo P1 dot changes
	}
	//if(semicolan == 1){cdigits_data[4] =  cdigits_data[4] |  (0x10);		//  // turn on the semicolan
	//}else{cdigits_data[4] =  cdigits_data[4] &  (~0x10);		//  // turn off the semicolan
	//}
	if(semicolan == 1)
	{
		cdigits_data[5] =  cdigits_data[5] |  (0x40);//neo changes for comma
	}
	else
	{
		cdigits_data[5] =  cdigits_data[5] &  (~0x40);//neo changes for comma
	}
//	if(kg == 1){cdigits_data[4] =  cdigits_data[4] | (0x02); // // turn on the kg symbol led
//	}else{cdigits_data[4] =  cdigits_data[4] & (~0x02); // // turn off the kg symbol led
//	}
	if(kg == 1){cdigits_data[5] =  cdigits_data[5] | (0x10); // // turn on the kg symbol led
	}else{cdigits_data[5] =  cdigits_data[5] & (~0x10); // // turn off the kg symbol led
	}
//	if(rpm == 1){cdigits_data[4] =  cdigits_data[4] | (0x08); // // turn on the rpm symbol	
//	}
//	else
//	{
//	 	cdigits_data[4] =  cdigits_data[4] & (~0x08); // // turn off the rpm symbol	
//	}
	/*  TODO: Commented for BTRA Oxy Jet should glow instead of spin cancel led. 12-04-2022 Prashant_H */
	/*if(no_spin == 1){cdigits_data[5] =  cdigits_data[5] |(0x08);  		// // turn on the no spin symbol
	}else{cdigits_data[5] =  cdigits_data[5] & ~(0x08);  		// // turn off the no spin symbol	
	}*/
//	if(centigrade == 1){cdigits_data[4] =  cdigits_data[4] | (0x04); 		// // turn on the 0C symbol
//	}else{cdigits_data[4] =  cdigits_data[4] & (~0x04); 		// // turn off the 0C symbol
//	}
	if(time_left == 1){cdigits_data[5] =  cdigits_data[5] | (0x20); 		//  // turn on the time left led
	}else{cdigits_data[5] =  cdigits_data[5] & (~0x20); 		//  // turn off the time left led
	
	}
}

void check_doorlockstatus()
{
	
		if((bOverHeat == FALSE))// if water level below the door level then doodr can open 
		{
			 // bPhysical_DoorLock_Status
			
			if(bDoorLockStatus == TRUE)
			{
				if((bStartButton == FALSE) )
				{
					if((iWaterLevel > ON_PAUSE_DOOR_OPEN_WATERLEVEL)&& (uiTemperature < 50)&&((uiMachineState == PAUSE_PRESSED)||(uiMachineState == HIGHLOWVOLTAGE)||(uiMachineState == IDLE_START)))
					{
						if((bProgram_End == FALSE)&& (bTEST_COMPLETED == FALSE)&&(bDoorLockError == 0)&&(bFiveHundredMsec_flag == TRUE)&&(uihigh_low_vtlg !=1))
						{
							cdigits_data[5] =  cdigits_data[5] | (0x01); ////turn on the doorlock led	
						}
						else 
						{
							if((bFiveHundredMsec_flag == TRUE) && (bChildLockButton == FALSE) &&(bDoorLockError == FALSE)&&(uihigh_low_vtlg !=1))
							{
								cdigits_data[5] =  cdigits_data[5] | (0x01); ////turn on the doorlock led
							}
							else
							{
								cdigits_data[5] =  cdigits_data[5] & (~0x01); 	// //turn off the doorlock led
								
							}
						}
					}
					else					
					{
						cdigits_data[5] =  cdigits_data[5] & (~0x01); 	// //turn off the doorlock led
					}
					
				}
				else
				{
					if(((bProgram_End == TRUE)|| (bTEST_COMPLETED == TRUE))&&(bStartButton == TRUE))
					{
						cdigits_data[5] =  cdigits_data[5] & (~0x01); 	// //turn off the doorlock led
						
					}
					else
					{
						if((bStartButton == TRUE)&&(uiMachineState != PROGRAM_END)&&(bDoorLock == SWITCH_ON)) 
						{
							cdigits_data[5] =  cdigits_data[5] & (~0x01); 	// //turn off the doorlock led
						}
						else
						{
						
							if((bFiveHundredMsec_flag == TRUE) && (bChildLockButton == FALSE) &&(bDoorLockError == FALSE)&&(uihigh_low_vtlg !=1)&&(bDoorLock == SWITCH_OFF)&&(bPhysical_DoorLock_Status == FALSE))
							{
								cdigits_data[5] =  cdigits_data[5] | (0x01); ////turn on the doorlock led
							}
							else
							{
								cdigits_data[5] =  cdigits_data[5] & (~0x01); 	// //turn off the doorlock led
							}
							
						}
					}
				}
					
			}
			else
			{
				if((bDoorLockError == 1)&&(uihigh_low_vtlg != 2))
				{
					cdigits_data[5] =  cdigits_data[5] & (~0x01); 	// //turn off the doorlock led
					//cdigits_data[5] =  cdigits_data[5] & (~0x08);LA led not required in error
				}
				else
				{
					
					cdigits_data[5] =  cdigits_data[5] | (0x01); ////turn on the doorlock led
					//cdigits_data[5] =  cdigits_data[5] | (0x08);//LA led not required in error
						
				}
			}
		}
		else // dooor will not open keep locked
		{
			cdigits_data[5] =  cdigits_data[5] & (~0x01); 	// //turn off the doorlock led
			cdigits_data[5] =  cdigits_data[5] & (~0x08);
		}
}
#if ENABLE_IOT
void ChkWifi_Symbol_On()
{
	if((bFaciaTestStep == FALSE)&&(bFCTMode_Selection != TRUE)&&(get_WiFiMode() != 0x00))
	{

		if(((get_IOTMultiFuncSetting() & WIFI_ENABLE_DISABLE)== RESET_VALUE) && (((BkMultiFuncSetting & AP_MODE_SETTING)== RESET_VALUE)&&(get_WiFiMode() != 0x01)))
		{
			//cdigits_data[4] = cdigits_data[4] | 0x20;
			ckey_digit_data[2] = ckey_digit_data[2] | (0x02);
		}
		else if(((get_IOTMultiFuncSetting() & WIFI_ENABLE_DISABLE)== RESET_VALUE) && (((BkMultiFuncSetting & AP_MODE_SETTING)== AP_MODE_SETTING)||(get_WiFiMode() == 0x01)))
		{
			//cdigits_data[4] = cdigits_data[4] | 0x20;
			ckey_digit_data[2] = ckey_digit_data[2] | (0x02);
		}
		else if(((bTestMode_Selection == TRUE)||(bFCTMode_Selection == TRUE)) && (MachineErrors.EBits.WifiComnError == 1))
		{
			//cdigits_data[4] = cdigits_data[4] | 0x20;
			ckey_digit_data[2] = ckey_digit_data[2] | (0x02);
		}
		else
		{
			//cdigits_data[4] = cdigits_data[4] & (~0x20);
			ckey_digit_data[2] = ckey_digit_data[2] & (~0x02);
		}
	}
}
void ChkWifi_Symbol_Off()
{
	if((bFaciaTestStep == FALSE)&&(bFCTMode_Selection != TRUE))//&&((get_WiFiMode() == 0x00)))
	{

		if(((get_IOTMultiFuncSetting() & WIFI_ENABLE_DISABLE)== RESET_VALUE) && (((BkMultiFuncSetting & AP_MODE_SETTING)== RESET_VALUE)&&(get_WiFiMode() > 0x01)))
		{
			//cdigits_data[4] = cdigits_data[4] | 0x20;
			ckey_digit_data[2] = ckey_digit_data[2] | 0x02;//& (~0x02); //| 0x02;
		}
		else if(((get_IOTMultiFuncSetting() & WIFI_ENABLE_DISABLE)== RESET_VALUE) && (((BkMultiFuncSetting & AP_MODE_SETTING)== AP_MODE_SETTING)||(get_WiFiMode() == 0x01)))
		{
			//cdigits_data[4] = cdigits_data[4] & (~0x20);
			ckey_digit_data[2] = ckey_digit_data[2] & (~0x02);
		}
		else if(((bTestMode_Selection == TRUE)||(bFCTMode_Selection == TRUE)) && (MachineErrors.EBits.WifiComnError == 1))
		{
			//cdigits_data[4] = cdigits_data[4] & (~0x20);
			ckey_digit_data[2] = ckey_digit_data[2] & (~0x02);
		}
		else
		{
			//cdigits_data[4] = cdigits_data[4] & (~0x20);
			ckey_digit_data[2] = ckey_digit_data[2] & (~0x02);
		}
	}
}
#endif
void CheckDrain_PVMVSymbol()
{
	  
	if((bFaciaTestStep == FALSE)&&(bFCTMode_Selection != TRUE))
	{				
		if((bPrewashValve == SWITCH_ON)||(bMainWashValve == SWITCH_ON) || (MachineErrors.EReg == 2)) //TODO: Changes taken from prathmesh as per Spec Rev 9. - 17/11/21
		{
		//	cdigits_data[4] = cdigits_data[4] | (0x40); // // turn on the waterfill symbol
			cdigits_data[5] =  cdigits_data[5] | (0x04);
		}
		else
		{
		//	cdigits_data[4] = cdigits_data[4] & (~0x40); // // turn on the waterfill symbol	
			cdigits_data[5] = cdigits_data[5] & (~0x04);
		}
	}
}

void MachineState_LED(char Prewash_LED,char Wash_LED,char Rinse_LED,char Spin_LED)
{
	if(Prewash_LED == 1)
	{
		//ckey_digit_data[3] =  ckey_digit_data[3] | (0x03); // // turn on the Prewash TEXT
	}
	else
	{
		//------------------------TURN OFF THE PREVIOUS STATE LED -----------------
		//ckey_digit_data[3] =  ckey_digit_data[3] & (~0x03); // // turn off the Prewash TEXT
	}
	if(Wash_LED == 1)
	{
		//ckey_digit_data[3] =  ckey_digit_data[3] | (0x04); // // turn on the Wash TEXT
		//cdigits_data[5] = cdigits_data[5] | 0x10; // // turn on the arrow of the Wash TEXT
	}
	else
	{
	//	ckey_digit_data[3] =  ckey_digit_data[3] & (~0x04); // // turn off the Wash TEXT
	//	cdigits_data[5] = cdigits_data[5] & (~0x10); // // turn off the arrow of the Wash TEXT
	}
	if(Rinse_LED == 1)
	{
	//	ckey_digit_data[3] =  ckey_digit_data[3] | (0x08); // // turn on the Rinse TEXT
	//	cdigits_data[5] = cdigits_data[5] | 0x20; // // turn on the arrow of the Rinse TEXT
	}
	else
	{
	//	ckey_digit_data[3] =  ckey_digit_data[3] & (~0x08); // // turn off the Rinse TEXT
	//	cdigits_data[5] = cdigits_data[5] & (~0x20); // // turn off the arrow of the Rinse TEXT	
	}
	if(Spin_LED == 1)
	{
	//	ckey_digit_data[3] =  ckey_digit_data[3] | (0x10); // // turn on the Spin  TEXT
	//	cdigits_data[5] = cdigits_data[5] | 0x40; // // turn on the arrow of the Spin TEXT
	}else{
	//	ckey_digit_data[3] =  ckey_digit_data[3] & (~0x10); // // turn on the Spin  TEXT
	//	cdigits_data[5] = cdigits_data[5] & (~0x40); // // turn on the arrow of the Spin TEXT
	}
}


void TurnOnSelected_Option()
{
	//ckey_digit_data[0] = ckey_digit_data[0] | uidisp_select_option;
	if( uidelaymin > 0)
	{
		//ckey_digit_data[0] = ckey_digit_data[0] | DelayStart_Yes;// 	// turn on the delay selction led
	}
	if( (uidisp_select_option & TimeSaver_Yes) == TimeSaver_Yes)
	{
		ckey_digit_data[4] = ckey_digit_data[4] | (0x02);
		//ckey_digit_data[0] = ckey_digit_data[0] | TimeSaver_Yes;// 	// turn on the time saver selction led
		bTimeSaver_Selected = TRUE;
		bRapidWashButton = TRUE;
	}
	else
	{
		ckey_digit_data[4] = ckey_digit_data[4] & (~0x02);
		//cdigits_data[0] =  cdigits_data[0] & ~(0x40);
		//ckey_digit_data[0] = ckey_digit_data[0] &(~ TimeSaver_Yes);// 	// turn on the time saver selction led
		bTimeSaver_Selected = FALSE;
		bRapidWashButton = FALSE;
	}
		
	if( (uidisp_select_option & Steam_Yes) == Steam_Yes)
	{
		ckey_digit_data[3] = ckey_digit_data[3] | (0x02);
		//cdigits_data[5] =  cdigits_data[5] | (0x80);
		//ckey_digit_data[0] = ckey_digit_data[0] | Steam_Yes;//	// turn on the Steam selction led
	}
	else
	{
		ckey_digit_data[3] = ckey_digit_data[3] & (~0x02);
		//cdigits_data[5] =  cdigits_data[5] & ~(0x80);
		//ckey_digit_data[0] = ckey_digit_data[0] &(~ Steam_Yes);//	// turn on the hot rinse selction led
	}
	if( (uidisp_select_option & ExtraRinse_Yes) == ExtraRinse_Yes) 
	{
		ckey_digit_data[1] = ckey_digit_data[1] | (0x02);
		//ckey_digit_data[0] = ckey_digit_data[0] | ExtraRinse_Yes;// 	// turn on the Extra rinse selction led
	}
	else
	{
		ckey_digit_data[1] = ckey_digit_data[1] & (~0x02);
		//ckey_digit_data[0] = ckey_digit_data[0] & (~ExtraRinse_Yes);// 	// turn on the Extra rinse selction led
	}
//	if((uidisp_select_option & WarmSoak_Yes) == WarmSoak_Yes)
//	{
//		 ckey_digit_data[0] = ckey_digit_data[0] | WarmSoak_Yes;// 	// turn on the soak selction led
//	}
//	else
//	{
//		ckey_digit_data[0] = ckey_digit_data[0] & (~WarmSoak_Yes);// 	// turn on the soak selction led
//	}
	if( (uidisp_select_option & Prewash_Yes) == Prewash_Yes)
	{
		ckey_digit_data[0] = ckey_digit_data[0] | (0x02);
		//ckey_digit_data[0] = ckey_digit_data[0] | Prewash_Yes;// // turn on the Prewash selction led
		bprewash_selected = TRUE;
	}
	else
	{
		ckey_digit_data[0] = ckey_digit_data[0] & (~0x02);
		//ckey_digit_data[0] = ckey_digit_data[0] & (~Prewash_Yes);// // turn on the Prewash selction led
		
	}
	//-------------  
	if((bProgramStart == FALSE)&&(bTestMode_Selection == FALSE))
	{
		ProgramTimming();
	}
	
}

void KEYS_DISPLAY()
{
	uint8_t ii;
	if((bkeys_display_flag == TRUE)&&(bZCD_Display == TRUE))
	{
		
	
		if((bTestMode_Selection== FALSE)&&(bDebugMode_Selection == FALSE)&& (MachineErrors.EReg == 0)&& (bFCTMode_Selection == FALSE)&&(uiMachineState != SNOOZEMODE)&&(uiMachineState != POWEROFF))
		{
			
		if(b_option_key_pressed_flag == 1)
		{      
			//b_option_key_pressed_flag = 0;
			
			switch(uidisplay_flash_count)
			{
				case 1:
				case 51:
				case 101:
				case 151:
				case 201:
				case 251:
				case 301:
				case 351:
				case 401:	//added for AI display
				case 451:
				case 501:
					
					
					bDisplay_Flag = TRUE;
					bFiveHundredMsec_flag = TRUE;
					
					Check_LaundryAdd();//if option is selected + irrespective of start/pause led should indicate
					check_doorlockstatus();
					ProgSel_led(); // 
#if ENABLE_IOT
					if((uiMachineState != PROGRAM_END) && (uiMachineState != SNOOZEMODE))
						ChkWifi_Symbol_On();
#endif
					if(bStartButton == FALSE)
					{
						
						if(bChildLockButton == TRUE)
						{
							cdigits_data[5] =  cdigits_data[5] | (0x02); // // turn on the child lock led
						}
						
						//display_availoption(uiPSER_Option_Avbl);
						if(uiRinseholdState != 2)
						{
							display_availoption(uiPSER_Option_Avbl);
						}
						else
						{
							//Rinsehold state no option selection is possible, only spin selection
							//previously selected option only should glow
							display_availoption(uidisp_select_option);
						}
						
						if(bPhysical_DoorLock_Status == TRUE)
							START_LED_ON;//		cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
						
						if((bKeyDetected == RESET_VALUE) && (uiDISP_MCount ==0))// && (bEmegency_door_pull !=1) if((cSwitch_Pressed  != TEMPERATURE_KEY )&&(cSwitch_Pressed  != SPIN_KEY ))
						{
							if(TubClean_Activated == FALSE)
							{
								if(uiRinseholdState == 2)
								{
									//mystrcpy((char *)cdigits_data_array, "rinS");
									mystrcpy((char *)cdigits_data_array, " t=#");
									update_display_data(&cdigits_data_array[0] );
									turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_ON,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
								}
								else
								{
									if(((uiopt_check & DelayStart_Yes) == DelayStart_Yes)&&(uiIDLE_COUNT > 0))
									{	
										DisplayTime(uidelaymin);										
									}
									else
									{

										if((uiSelected_Position==COTTON)||(uiSelected_Position==MIXEDSOIL)||(uiSelected_Position==SYNTHETIC))
										{
											if( uidisplay_flash_count >= 401)
											{
												Display_RPM(uiRpmSel); // display RPM
												
											}
											else if(( uidisplay_flash_count >= 301)&&(uidisplay_flash_count < 401))
											{	
												if(uiTempSel == 0x00)
												{
													uidisplay_flash_count = 402;//317;//302;
													Display_RPM(uiRpmSel); // display RPM
													
												}
												else
												{
													Display_TEMP(uiTempSel); // display selected temp 
													//turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_ON,TIMELEFT_OFF);
												}
											}
											else if(( uidisplay_flash_count >= 201)&&(uidisplay_flash_count < 301))
											{
												if((uiMachineState == PAUSE_PRESSED))
												{
													//uidisplay_flash_count = 402;//317;//302;
													if(( uidisplay_flash_count >= 201)&&(uidisplay_flash_count < 250))
													{
														Display_RPM(uiRpmSel); // display RPM
													}
													else if(( uidisplay_flash_count >= 251)&&(uidisplay_flash_count < 301))
													{
														Display_TEMP(uiTempSel);// TODO: Added from Prathmesh  as per sepc rev 9- 17/11/21 
													}
													
												}
												else
												{
													Display_KG(uidefault_kg); // display kg 
													//turnon_display_led(TIMEDOTS_OFF,SEMI_ON,KG_ON,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
												}
											}

											else if((uidisplay_flash_count >=101)&&(uidisplay_flash_count <201))
											{
												DisplayTime(BalanceTime);
												turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
											}
											else if(uidisplay_flash_count <101)
											{
												mystrcpy((char *)cdigits_data_array, "  Ae");
												update_display_data(&cdigits_data_array[0] );
												turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
											}
										}
										else
										{
											if( uidisplay_flash_count >= 375)
											{
												Display_RPM(uiRpmSel); // display RPM
												
											}
											else if(( uidisplay_flash_count >= 250)&&(uidisplay_flash_count < 375))
											{	
												if(uiTempSel == 0x00)
												{
													uidisplay_flash_count = 376;
													Display_RPM(uiRpmSel); // display RPM
													
												}
												else
												{
													Display_TEMP(uiTempSel); // display selected temp 
													//turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_ON,TIMELEFT_OFF);
												}
											}
											else if(( uidisplay_flash_count >= 125)&&(uidisplay_flash_count < 250))
											{
												if((uiMachineState == PAUSE_PRESSED))
												{
													//uidisplay_flash_count = 376;
//													if(( uidisplay_flash_count >= 150)&&(uidisplay_flash_count < 225))
//													{
//														Display_RPM(uiRpmSel); // display RPM
//													}
//													else if(( uidisplay_flash_count >= 225)&&(uidisplay_flash_count < 300))
//													{
//														Display_TEMP(uiTempSel);// TODO: Added from Prathmesh  as per sepc rev 9- 17/11/21 
//													}
													Display_RPM(uiRpmSel); // display RPM
													Display_TEMP(uiTempSel);// TODO: Added from Prathmesh  as per sepc rev 9- 17/11/21 
													
												}
												else
												{
													Display_KG(uidefault_kg); // display kg 
													//turnon_display_led(TIMEDOTS_OFF,SEMI_ON,KG_ON,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
												}
											}
											else if(uidisplay_flash_count < 125)
											{
												DisplayTime(BalanceTime);
												turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
											}
										}
//										if( uidisplay_flash_count >= 301)
//										{
//											Display_RPM(uiRpmSel); // display RPM
//											
//										}
//										else if(( uidisplay_flash_count >= 201)&&(uidisplay_flash_count < 300))
//										{	
//											if(uiTempSel == 0x00)
//											{
//												uidisplay_flash_count = 302;
//												Display_RPM(uiRpmSel); // display RPM
//												
//											}
//											else
//											{
//												Display_TEMP(uiTempSel); // display selected temp 
//												//turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_ON,TIMELEFT_OFF);
//											}
//										}
//										else if(( uidisplay_flash_count >= 101)&&(uidisplay_flash_count < 200))
//										{
//											if((uiMachineState == PAUSE_PRESSED))
//											{
//												uidisplay_flash_count = 302;
//												Display_RPM(uiRpmSel); // display RPM
//												Display_TEMP(uiTempSel);// TODO: Added from Prathmesh  as per sepc rev 9- 17/11/21 
//												
//											}
//											else
//											{
//												Display_KG(uidefault_kg); // display kg 
//												//turnon_display_led(TIMEDOTS_OFF,SEMI_ON,KG_ON,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
//											}
//										}
//										else if(uidisplay_flash_count < 101)
//										{
//											DisplayTime(BalanceTime);
//											turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
//										}
									}
								}
							}
							else // if tub clean is activated 
							{
								bDisplay_Flag = TRUE;
								
								TurnOnSelected_Option();// for clear the option
								if( uidisplay_flash_count >= 151)
								{
									uidisplay_flash_count = 0;
									turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
								}
								else if(( uidisplay_flash_count >= 101)&&(uidisplay_flash_count < 150))
								{	
									DisplayTime(BalanceTime);
									    if(uiRpmSel == Rpm000)
									    {
										   turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
									    }
									    else
									    {
										    turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
									    }
								}
								else if(( uidisplay_flash_count >= 51)&&(uidisplay_flash_count < 100))
								{
									mystrcpy((char *)cdigits_data_array, " CLn");
									update_display_data(&cdigits_data_array[0] );
									if(uiRpmSel == Rpm000)
									    {
										   turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_OFF);
									    }
									    else
									    {
										    turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
									    }
								}
								else if(uidisplay_flash_count < 51)
								{
									mystrcpy((char *)cdigits_data_array, " tUb");
									update_display_data(&cdigits_data_array[0] );
									if(uiRpmSel == Rpm000)
									    {
										   turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_OFF);
									    }
									    else
									    {
										    turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
									    }
								} 
							}
						}
					} 
					else if(bStartButton == TRUE)
					{
						if(bChildLockButton == TRUE)
						{
							cdigits_data[5] =  cdigits_data[5] | (0x02); // // turn on the child lock led
						}
						CheckDrain_PVMVSymbol();
						if(bPhysical_DoorLock_Status == TRUE)
						{
							START_LED_ON;//cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
						}
						if((bKeyDetected == RESET_VALUE) && (uiDISP_MCount == 0x00))//if((cSwitch_Pressed  != TEMPERATURE_KEY )&&(cSwitch_Pressed  != SPIN_KEY ))
						{
							if(TubClean_Activated == FALSE)
							{
								if((uidisp_select_option & DelayStart_Yes)&&(uidelaymin > RESET_VALUE))
								{ 
									if(uidisplay_flash_count == 151) // show delay message  for 1 sec after 3 sec completed
									{
										mystrcpy((char *)cdigits_data_array, " dLy");
										update_display_data(&cdigits_data_array[0] );									
										turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
										
									}
									else
									{
								    		DisplayTime(uidelaymin);// display delaly time
											
										turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
									}
								}
								else
								{
									if(((BalanceTime >= RESET_VALUE)&&(uiMachineState != PROGRAM_END)) || ((bDoorLockStatus == TRUE)&&(uiMachineState == PROGRAM_END)&&(bChildLockButton == FALSE)))// if(BalanceTime > RESET_VALUE)
									{
										if(LS_flag!=TRUE)
										{
											i=0; // used for LS display : reseted to start from 1 digit
											if((uiSelected_Position==COTTON)||(uiSelected_Position==MIXEDSOIL)||(uiSelected_Position==SYNTHETIC))
											{
												if(uidisplay_flash_count<101)
												{
													mystrcpy((char *)cdigits_data_array, "  Ae");
													update_display_data(&cdigits_data_array[0] );
													turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
												}
												else if(uidisplay_flash_count>=101)
												{
													DisplayTime(BalanceTime);
													if(bPhysical_DoorLock_Status  == TRUE)
													{
														START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led									    
													}
													snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT; // this is added for the
												    if(uiRpmSel == Rpm000)
												    {
													   turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
												    }
												    else
												    {
													    turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
												    }
												}
												
											}
											else
											{
												DisplayTime(BalanceTime); //display balance time	
												if(bPhysical_DoorLock_Status  == TRUE)
												{
													START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led									    
												}
												snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT; // this is added for the
											    if(uiRpmSel == Rpm000)
											    {
												   turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
											    }
											    else
											    {
												    turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
											    }
											}
											
										}
										else
										{
											LoadSensing_display();
											Delay_ms(140);
											uidisplay_flash_count=0;	
										}
										
//										    DisplayTime(BalanceTime); //display balance time	
//											if(bPhysical_DoorLock_Status  == TRUE)
//											{
//												START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led									    
//											}
//											snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT; // this is added for the
//										    if(uiRpmSel == Rpm000)
//										    {
//											   turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
//										    }
//										    else
//										    {
//											    turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
//										    }
										    
									}
									else
									{
										if(bChildLockButton == 0)
											START_LED_OFF; 
									}
								}
							}
							else  // tub clean is true
							{
								cTub_CleanCount++;
								if(((BalanceTime >= RESET_VALUE)&&(uiMachineState != PROGRAM_END)) || ((bDoorLockStatus == TRUE)&&(uiMachineState == PROGRAM_END)&&(bChildLockButton == FALSE)))// if(BalanceTime > RESET_VALUE)
								{
									if(cTub_CleanCount < 5)
									{
										if(bPhysical_DoorLock_Status  == TRUE)
										{
											START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
										}
										//if(BalanceTime > RESET_VALUE)
										{
										    DisplayTime(BalanceTime); //display balance time
										    if(uiRpmSel == Rpm000)
											{
												turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
											}
											else
											{
												turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
											}
										}
									}
									else
									{
										if(uiMachineState != PROGRAM_END)
										{
											
										mystrcpy((char *)cdigits_data_array, " CLn");
										update_display_data(&cdigits_data_array[0] );
											if(uiRpmSel == Rpm000)
											{
												turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_OFF);
											}
											else
											{
												turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
											}
										}
										else
										{
											if(bPhysical_DoorLock_Status  == TRUE)
											{
												START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
											}
										
											DisplayTime(BalanceTime); //display balance time
											    	if(uiRpmSel == Rpm000)
												{
													turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
												}
												else
												{
													turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
												}
										}
									}
								}
								if(cTub_CleanCount>8)
								{
									cTub_CleanCount = RESET_VALUE;
								}
							}
						
						}
						
						if((uiMachineState != PROGRAM_END)||((uiMachineState == PROGRAM_END)&&(bChildLockButton == TRUE)))
							display_availoption(uidisp_select_option);// //cdigits_data[5] = cdigits_data[5] | uidisp_select_option; // keep on the selected options
							
						
						//if(uidisplay_flash_count == 201)
						//	uidisplay_flash_count = RESET_VALUE; // reset the timer in start mode after 4 sec completed ( 3 sec delay timer  & 1sec delay message cycle completed) 
					}
					
//					for(ii = 0; ii < 6;ii++)
//					{
//						STLED316_7Seg_Write(ii,cdigits_data[ii]); // update the display
//					}

					for(ii = 0xC0; ii < 0xCD; ii= ii + 0x02)
					{
						STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // this is should not be 0x00 , default array have to written
						
					}
					STLED316_7Seg_Write(0XCD,ckey_digit_data[5]); //ADDED BY - AK FOR PROGRAM LEDS
					
					if(bSpin_Start == FALSE)
						Reinit_Stleddevice();
					
					break;//--------------------------------------------------------------------
					
				case 27: // here display get off if any blinking condition is true (every 500 msec within 8 sec))
				case 77:
				case 127:
				case 177:
				case 227:
				case 277:
				case 327:
				case 377:
				case 427: 	//added for AI display
				case 477:
				case 527:
					bFiveHundredMsec_flag = FALSE;
					
					if(cFlash_prog_led >0)
					{
						cFlash_prog_led--;
						
						ResProg_led();
					}
					if(uidoorflash_count>0)
					{
						uidoorflash_count--;
						cdigits_data[5] =  cdigits_data[5] & (~0x01); 	// //turn off the doorlock led
					}
					else
					{
						check_doorlockstatus();
					}
					if(cFlash_Count > 0)
					{
						cFlash_Count--;
						if((bStartButton == FALSE)&&(bProgram_End == FALSE)&& (bTEST_COMPLETED == FALSE))
						{
							if(bPhysical_DoorLock_Status  == TRUE)
							{
								START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
							}
						}
						cdigits_data[0] = cdigits_data[1] = cdigits_data[2] = cdigits_data[3] = 0x00;
						
//						for(ii = 0; ii < 4;ii++)
//						{
//							STLED316_7Seg_Write(ii,cdigits_data[ii]); // NO(blank) display 
//						}

						for(ii = 0xC0; ii < 0xCD; ii= ii + 0x02)
						{
							STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // this is should not be 0x00 , default array have to written
							
						}
						Check_LaundryAdd();
						CheckDrain_PVMVSymbol();
#if ENABLE_IOT
					if((uiMachineState != PROGRAM_END) && (uiMachineState != SNOOZEMODE))
						ChkWifi_Symbol_Off();
#endif
						//chip_select = TRUE; 
					
							if(bStartButton == FALSE)
							{
								//display_availoption(uiPSER_Option_Avbl);
								if(uiRinseholdState != 2)
								{
									display_availoption(uiPSER_Option_Avbl);
								}
								else
								{
									//Rinsehold state no option selection is possible, only spin selection
									//previously selected option only should glow
									display_availoption(uidisp_select_option);
								}
							}
							else
							{
								display_availoption((uidisp_select_option));////cdigits_data[5] = cdigits_data[5] | uidisp_select_option; // countious on led selected option 
							}
						
						chip_select = FALSE; // for selecting the 1st STLED
						
						
					}
					else
					{
						Check_LaundryAdd();
						CheckDrain_PVMVSymbol();
#if ENABLE_IOT
						if((uiMachineState != PROGRAM_END) && (uiMachineState != SNOOZEMODE))
							ChkWifi_Symbol_Off();
						//cdigits_data[4] = cdigits_data[4] & (~0x20);
#endif
						bDisplay_Flag = TRUE;
						
						if(bStartButton == FALSE)
						{
							if((uiMachineState == PAUSE_PRESSED))
							{
								if(uiRinseholdState != 2)
								{
									//display_availoption(uiPSER_Option_Avbl);
									if(uiIDLE_COUNT == RESET_VALUE)
									{
										display_availoption((uidisp_select_option));////cdigits_data[5] = cdigits_data[5] | uidisp_select_option; // countious on led selected option 
									}
									else
									{
										display_availoption((uiOption_closed   & uidisp_select_option ) | uioption_on);
									}
								}
								else
								{
									//Rinsehold state no option selection is possible, only spin selection
									//previously selected option only should glow
									display_availoption(uidisp_select_option);
								}
								/*if(uiIDLE_COUNT == RESET_VALUE)
								{
									display_availoption((uidisp_select_option));////cdigits_data[5] = cdigits_data[5] | uidisp_select_option; // countious on led selected option 
								}
								else
								{
									display_availoption((uiOption_closed   & uidisp_select_option ) | uioption_on);
								}*/
							}
							else
							{
								display_availoption((uiPSER_Option_Avbl & uioption_on));
							}
							
							//------------------------------------------------------------------ 
							//----------------------------
							//drainvalve
							//-------------------------
							//---------------------------------------------- 
							if(( bChildLockButton == TRUE ) &&( uichild_flash_count > RESET_VALUE))
							{
								uichild_flash_count--;
								cdigits_data[5] =  cdigits_data[5] & (~0x02); // // turn off the child lock led
							}
							else if(bChildLockButton == TRUE)
							{
								cdigits_data[5] =  cdigits_data[5] | (0x02); // // turn on the child lock led
							}
							else
							{
								cdigits_data[5] =  cdigits_data[5] & (~0x02); // // turn off the child lock led
								START_LED_OFF; // cdigits_data[4] =  cdigits_data[4] & ~(0x08);	// //turn off the start/puase led
								
							}
							
							//---------------------------------------------- 
							
							
							
							if((bKeyDetected == RESET_VALUE) && (uiDISP_MCount ==0) )// && (bEmegency_door_pull !=1)if((cSwitch_Pressed  != TEMPERATURE_KEY )&&(cSwitch_Pressed  != SPIN_KEY ))
							{
								if(TubClean_Activated == FALSE)
								{
									if(uiRinseholdState == 2)
									{
										//mystrcpy((char *)cdigits_data_array, "H0Ld");
										mystrcpy((char *)cdigits_data_array, " t=#");
										update_display_data(&cdigits_data_array[0] );
										//cdigits_data[4] =  cdigits_data[4] |( 0x08);	// //turn on the start/puase led
										turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_OFF);
									}
									else
									{
										if(((uiopt_check & DelayStart_Yes) == DelayStart_Yes)&&(uiIDLE_COUNT > 0))
										{
											DisplayTime(uidelaymin); 
											
										}
										else
										{	
											if((uiSelected_Position==COTTON)||(uiSelected_Position==MIXEDSOIL)||(uiSelected_Position==SYNTHETIC))
											{
												if(uidisplay_flash_count<101)
												{
													mystrcpy((char *)cdigits_data_array, "  Ae");
													update_display_data(&cdigits_data_array[0] );
													turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
												}
												else if((uidisplay_flash_count >= 101)&&(uidisplay_flash_count <201))
												{
													
													DisplayTime(BalanceTime);
													turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
												}
											}
											else
											{
												if(uidisplay_flash_count < 125)
												{
													DisplayTime(BalanceTime);
													turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
												}
											}									
//											if(uidisplay_flash_count < 101)
//											{
//												
//												DisplayTime(BalanceTime);
//												turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
//											}
										}
									}
								}
								
							}
							
						} 
						else if(bStartButton == TRUE)
						{
							//----------------------------
							//drainvalve
							//-------------------------
							if((bKeyDetected == RESET_VALUE) && (uiDISP_MCount ==0))	//if((cSwitch_Pressed  != TEMPERATURE_KEY )&&(cSwitch_Pressed  != SPIN_KEY ))
							{
								if(TubClean_Activated == FALSE)
								{
									if((uidisp_select_option & DelayStart_Yes)&&(uidelaymin > RESET_VALUE))
									{ 
										if(uidisplay_flash_count == 177)
										{
											mystrcpy((char *)cdigits_data_array, " dLy");
											update_display_data(&cdigits_data_array[0] );
											 turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
										}
										else
										{
									    		
											DisplayTime(uidelaymin);
											if(bPhysical_DoorLock_Status  == TRUE)
											{
												START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
											}
											turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
										}
									}
									else
									{
						
										if(((BalanceTime >= RESET_VALUE)&&(uiMachineState != PROGRAM_END)) || ((bDoorLockStatus == TRUE)&&(uiMachineState == PROGRAM_END)&&(bChildLockButton == FALSE)))// if(BalanceTime > RESET_VALUE)
										{
											if(LS_flag!=TRUE)
											{
												i=0; // used for LS display : reseted to start from 1 digit
												
												if((uiSelected_Position==COTTON)||(uiSelected_Position==MIXEDSOIL)||(uiSelected_Position==SYNTHETIC))
												{
													if(uidisplay_flash_count<101)
													{
														mystrcpy((char *)cdigits_data_array, "  Ae");
														update_display_data(&cdigits_data_array[0] );
														turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
													}
													else if(uidisplay_flash_count>=101)
													{
														 DisplayTime(BalanceTime);
														if(bPhysical_DoorLock_Status  == TRUE)
														{
															START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
														}
													    if(uiRpmSel == Rpm000)
													    {
														   turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
													    }
													    else
													    {   
														   turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
													   	if((GET_PROGRAM) != STEAMFRESH && uiSelected_Position != ADDITIVESRINSE)
														{
															if(uiMachineState != PROGRAM_END)
															{
																turnon_display_OxyJetled(OXYJET_ON);	//TODO: Turn On OxyJet at start of program.
															}
														}
													    }
													    snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT;
													
													}
												}
												else
												{
													DisplayTime(BalanceTime);
														if(bPhysical_DoorLock_Status  == TRUE)
														{
															START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
														}
													    if(uiRpmSel == Rpm000)
													    {
														   turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
													    }
													    else
													    {   
														   turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
														   	if((GET_PROGRAM) != STEAMFRESH && uiSelected_Position != ADDITIVESRINSE)
															{
																if(uiMachineState != PROGRAM_END)
																{
																	turnon_display_OxyJetled(OXYJET_ON);	//TODO: Turn On OxyJet at start of program.
																}
															}
													    }
													    snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT;	
												}
											}
											else
											{
												LoadSensing_display();
												Delay_ms(140);
												uidisplay_flash_count=0;	
											}
//										    DisplayTime(BalanceTime);
//											if(bPhysical_DoorLock_Status  == TRUE)
//											{
//												START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
//											}
//										    if(uiRpmSel == Rpm000)
//										    {
//											   turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
//										    }
//										    else
//										    {   
//											   turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
//											   	if((GET_PROGRAM) != STEAMFRESH && uiSelected_Position != ADDITIVESRINSE)
//												{
//													if(uiMachineState != PROGRAM_END)
//													{
//														turnon_display_OxyJetled(OXYJET_ON);	//TODO: Turn On OxyJet at start of program.
//													}
//												}
//										    }
//										    snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT;
										}
									}
								}
								else
								{
									if(((BalanceTime >= RESET_VALUE)&&(uiMachineState != PROGRAM_END)) || ((bDoorLockStatus == TRUE)&&(uiMachineState == PROGRAM_END)&&(bChildLockButton == FALSE)))// if(BalanceTime > RESET_VALUE)
									{
										if(cTub_CleanCount < 5)
										{
											
											    DisplayTime(BalanceTime); //display balance time
												if(bPhysical_DoorLock_Status  == TRUE)
												{
													START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
												}
											    	if(uiRpmSel == Rpm000)
											    	{
													turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
												}
												else
												{
													turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
												}
											
										}
										else
										{
											if(uiMachineState != PROGRAM_END)
											{
												mystrcpy((char *)cdigits_data_array, " CLn");
												update_display_data(&cdigits_data_array[0] );
												if(uiRpmSel == Rpm000)
												{
													turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_OFF);
												}
												else
												{
													turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
												}
											}
											else
											{
												DisplayTime(BalanceTime); //display balance time
												if(bPhysical_DoorLock_Status  == TRUE)
												{
											    	START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
												}
											    if(uiRpmSel == Rpm000)
												{
													turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
												}
												else
												{
													turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
												}
												
											}
										}
									}
								
								
								
								}
								
								if((uiMachineState != PROGRAM_END)||((uiMachineState == PROGRAM_END)&&(bChildLockButton == TRUE)))
									display_availoption((uidisp_select_option));////cdigits_data[5] = cdigits_data[5] | uidisp_select_option; // countious on led selected option 
							}
							
							if(((bChildLockButton == TRUE) &&(uichild_flash_count > RESET_VALUE))||((bChildLockButton == TRUE) && (uiMachineState == PROGRAM_END)))
							{
								uichild_flash_count--;
								cdigits_data[5] =  cdigits_data[5] & ~(0x02);// // turn off the child lock led
							}
							else if(bChildLockButton == TRUE)
							{
								cdigits_data[5] =  cdigits_data[5] |(0x02); // // turn off the child lock led
							}
						}
					
					}
//					for(ii = 0; ii < 6;ii++)
//					{
//						STLED316_7Seg_Write(ii,cdigits_data[ii]); // update the display
//					}

					for(ii = 0xC0; ii < 0xCD; ii= ii + 0x02)
					{
						STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // this is should not be 0x00 , default array have to written
						
					}
					STLED316_7Seg_Write(0XCD,ckey_digit_data[5]); //ADDED BY - AK FOR PROGRAM LEDS
					
					//chip_select = TRUE; // for selecting the 1st STLED
//					for(ii = 0; ii < 6;ii++)
//					{
//						STLED316_7Seg_Write(ii,ckey_digit_data[ii]); // this is should not be 0x00 , default array have to written
//						
//					}

					for(ii = 0xC1; ii < 0xCA; ii= ii + 0x02)
					{
						STLED316_7Seg_Write(ii,ckey_digit_data[(ii-0xC1)/2]); // this is should not be 0x00 , default array have to written
						
					}
					chip_select = FALSE; // for selecting the 1st STLED
					
					break; ///--------------------------------------------------------------------
					
				
					
				case 550:
					uidisplay_flash_count = 0;
					break;
				default :
				 break;
			}
			if(uidisplay_flash_count > 551)
				uidisplay_flash_count= RESET_VALUE;
				
			bDisplay_Flag = TRUE ;
			
			
			
		}
		else{
			switch(uidisplay_flash_count)
			{
				case 1:
				case 88:
				case 174:
				case 261:
				case 348:
				case 435:	//added for AI display
				case 522:
					bDisplay_Flag = TRUE;
					bFiveHundredMsec_flag = TRUE;
					
					Check_LaundryAdd();//to indicate if no option is selected
					check_doorlockstatus();
					ProgSel_led(); // 
#if ENABLE_IOT
					if((uiMachineState != PROGRAM_END) && (uiMachineState != SNOOZEMODE))
						ChkWifi_Symbol_On();
#endif
					if(bStartButton == FALSE)
					{
						
						if(bChildLockButton == TRUE)
						{
							cdigits_data[5] =  cdigits_data[5] | (0x02); // // turn on the child lock led
						}
						
						//display_availoption(uiPSER_Option_Avbl);
						
						
						if(uiRinseholdState != 2)
						{
							display_availoption(uiPSER_Option_Avbl);
						}
						else
						{
							//Rinsehold state no option selection is possible, only spin selection
							//previously selected option only should glow
							display_availoption(uidisp_select_option);
						}
						
						
						
						
						if(bPhysical_DoorLock_Status == TRUE)
							START_LED_ON;//		cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
						
						if((bKeyDetected == RESET_VALUE) && (uiDISP_MCount ==0))// && (bEmegency_door_pull !=1) if((cSwitch_Pressed  != TEMPERATURE_KEY )&&(cSwitch_Pressed  != SPIN_KEY ))
						{
							if(TubClean_Activated == FALSE)
							{
								if(uiRinseholdState == 2)
								{
									//mystrcpy((char *)cdigits_data_array, "rinS");
									mystrcpy((char *)cdigits_data_array, " t=#");
									update_display_data(&cdigits_data_array[0] );
									turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_ON,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
								}
								else
								{
									if(((uiopt_check & DelayStart_Yes) == DelayStart_Yes)&&(uiIDLE_COUNT > 0))
									{	
										DisplayTime(uidelaymin);										
									}
									else
									{
										if((uiSelected_Position==COTTON)||(uiSelected_Position==MIXEDSOIL)||(uiSelected_Position==SYNTHETIC))
										{
											if( uidisplay_flash_count >= 401)
											{
												Display_RPM(uiRpmSel); // display RPM
											
											}
											else if(( uidisplay_flash_count >= 301)&&(uidisplay_flash_count < 401))
											{	
												if(uiTempSel == 0x00)
												{
													uidisplay_flash_count = 402;//301;//317;//302;
													Display_RPM(uiRpmSel); // display RPM
												
												}
												else
												{
													Display_TEMP(uiTempSel); // display selected temp 
													//turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_ON,TIMELEFT_OFF);
												}
											}
											else if(( uidisplay_flash_count >= 201)&&(uidisplay_flash_count < 301))
											{
												if((uiMachineState == PAUSE_PRESSED))
												{
													//uidisplay_flash_count = 402;//317;//302;
													if(( uidisplay_flash_count >= 201)&&(uidisplay_flash_count < 250))
													{
														Display_RPM(uiRpmSel); // display RPM
													}
													else if(( uidisplay_flash_count >= 251)&&(uidisplay_flash_count < 301))
													{
														Display_TEMP(uiTempSel);// TODO: Added from Prathmesh  as per sepc rev 9- 17/11/21 
													}
												
												}
												else
												{
													Display_KG(uidefault_kg); // display kg 
													//turnon_display_led(TIMEDOTS_OFF,SEMI_ON,KG_ON,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
												}
											}
											else if((uidisplay_flash_count >=101)&&(uidisplay_flash_count <201))
											{
												DisplayTime(BalanceTime);
												turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
											}
											else if(uidisplay_flash_count <101)
											{
												mystrcpy((char *)cdigits_data_array, "  Ae");
												update_display_data(&cdigits_data_array[0] );
												turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
											}
										}
										else
										{
											if( uidisplay_flash_count >= 375)
											{
												Display_RPM(uiRpmSel); // display RPM
											
											}
											else if(( uidisplay_flash_count >= 250)&&(uidisplay_flash_count < 375))
											{	
												if(uiTempSel == 0x00)
												{
													uidisplay_flash_count = 376;
													Display_RPM(uiRpmSel); // display RPM
												
												}
												else
												{
													Display_TEMP(uiTempSel); // display selected temp 
													//turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_ON,TIMELEFT_OFF);
												}
											}
											else if(( uidisplay_flash_count >= 125)&&(uidisplay_flash_count < 250))
											{
												if((uiMachineState == PAUSE_PRESSED))
												{
													//uidisplay_flash_count = 376;
//													if(( uidisplay_flash_count >= 150)&&(uidisplay_flash_count < 225))
//													{
//														Display_RPM(uiRpmSel); // display RPM
//													}
//													else if(( uidisplay_flash_count >= 225)&&(uidisplay_flash_count < 300))
//													{
//														Display_TEMP(uiTempSel);// TODO: Added from Prathmesh  as per sepc rev 9- 17/11/21 
//													}
													Display_RPM(uiRpmSel); // display RPM
													Display_TEMP(uiTempSel);// TODO: Added from Prathmesh  as per sepc rev 9- 17/11/21 
												}
												else
												{
													Display_KG(uidefault_kg); // display kg 
													//turnon_display_led(TIMEDOTS_OFF,SEMI_ON,KG_ON,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
												}
											}
											else if(uidisplay_flash_count < 125)
											{
												DisplayTime(BalanceTime);
												turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
											}
										}
//										if( uidisplay_flash_count >= 301)
//										{
//											Display_RPM(uiRpmSel); // display RPM
//											
//										}
//										else if(( uidisplay_flash_count >= 201)&&(uidisplay_flash_count < 300))
//										{	
//											if(uiTempSel == 0x00)
//											{
//												uidisplay_flash_count = 302;
//												Display_RPM(uiRpmSel); // display RPM
//												
//											}
//											else
//											{
//												Display_TEMP(uiTempSel); // display selected temp 
//												//turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_ON,TIMELEFT_OFF);
//											}
//										}
//										else if(( uidisplay_flash_count >= 101)&&(uidisplay_flash_count < 200))
//										{
//											if((uiMachineState == PAUSE_PRESSED))
//											{
//												uidisplay_flash_count = 302;
//												Display_RPM(uiRpmSel); // display RPM
//												Display_TEMP(uiTempSel);// TODO: Added from Prathmesh  as per sepc rev 9- 17/11/21 
//												
//											}
//											else
//											{
//												Display_KG(uidefault_kg); // display kg 
//												//turnon_display_led(TIMEDOTS_OFF,SEMI_ON,KG_ON,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
//											}
//										}
//										else if(uidisplay_flash_count < 101)
//										{
//											DisplayTime(BalanceTime);
//											turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
//										}
									}
								}
							}
							else // if tub clean is activated 
							{
								bDisplay_Flag = TRUE;
								
								TurnOnSelected_Option();// for clear the option
								if( uidisplay_flash_count >= 151)
								{
									uidisplay_flash_count = 0;
									turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
								}
								else if(( uidisplay_flash_count >= 101)&&(uidisplay_flash_count < 150))
								{	
									DisplayTime(BalanceTime);
									    if(uiRpmSel == Rpm000)
									    {
										   turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
									    }
									    else
									    {
										    turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
									    }
								}
								else if(( uidisplay_flash_count >= 51)&&(uidisplay_flash_count < 100))
								{
									mystrcpy((char *)cdigits_data_array, " CLn");
									update_display_data(&cdigits_data_array[0] );
									if(uiRpmSel == Rpm000)
									    {
										   turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_OFF);
									    }
									    else
									    {
										    turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
									    }
								}
								else if(uidisplay_flash_count < 51)
								{
									mystrcpy((char *)cdigits_data_array, " tUb");
									update_display_data(&cdigits_data_array[0] );
									if(uiRpmSel == Rpm000)
									    {
										   turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_OFF);
									    }
									    else
									    {
										    turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
									    }
								} 
							}
						}
					} 
					else if(bStartButton == TRUE)
					{
						if(bChildLockButton == TRUE)
						{
							cdigits_data[5] =  cdigits_data[5] | (0x02); // // turn on the child lock led
						}
						CheckDrain_PVMVSymbol();
						if(bPhysical_DoorLock_Status == TRUE)
						{
							START_LED_ON;//cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
						}
						if((bKeyDetected == RESET_VALUE) && (uiDISP_MCount == 0x00))//if((cSwitch_Pressed  != TEMPERATURE_KEY )&&(cSwitch_Pressed  != SPIN_KEY ))
						{
							if(TubClean_Activated == FALSE)
							{
								if((uidisp_select_option & DelayStart_Yes)&&(uidelaymin > RESET_VALUE))
								{ 
									if(uidisplay_flash_count == 174/*151*/) // show delay message  for 1 sec after 3 sec completed
									{
										mystrcpy((char *)cdigits_data_array, " dLy");
										update_display_data(&cdigits_data_array[0] );									
										turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
										
									}
									else
									{
								    		DisplayTime(uidelaymin);// display delaly time
											
										turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
									}
								}
								else
								{
									if(((BalanceTime >= RESET_VALUE)&&(uiMachineState != PROGRAM_END)) || ((bDoorLockStatus == TRUE)&&(uiMachineState == PROGRAM_END)&&(bChildLockButton == FALSE)))// if(BalanceTime > RESET_VALUE)
									{
										if(LS_flag!=TRUE)
										{
											i=0; // used for LS display : reseted to start from 1 digit
										    if((uiSelected_Position==COTTON)||(uiSelected_Position==MIXEDSOIL)||(uiSelected_Position==SYNTHETIC))
											{
												if(uidisplay_flash_count<101)
												{
													mystrcpy((char *)cdigits_data_array, "  Ae");
													update_display_data(&cdigits_data_array[0] );
													turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
												}
												else if(uidisplay_flash_count>=101)
												{
													DisplayTime(BalanceTime);
													if(bPhysical_DoorLock_Status  == TRUE)
													{
														START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led									    
													}
													snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT; // this is added for the
												    if(uiRpmSel == Rpm000)
												    {
													   turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
												    }
												    else
												    {
													    turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
												    }
												}
												
											}
											else
											{
												DisplayTime(BalanceTime); //display balance time	
												if(bPhysical_DoorLock_Status  == TRUE)
												{
													START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led									    
												}
												snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT; // this is added for the
											    if(uiRpmSel == Rpm000)
											    {
												   turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
											    }
											    else
											    {
												    turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
											    }
											}
										}
										else
										{
											LoadSensing_display();
											Delay_ms(140);
											uidisplay_flash_count=0;	
										}
//										    DisplayTime(BalanceTime); //display balance time	
//											if(bPhysical_DoorLock_Status  == TRUE)
//											{
//												START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led									    
//											}
//											snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT; // this is added for the
//										    if(uiRpmSel == Rpm000)
//										    {
//											   turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
//										    }
//										    else
//										    {
//											    turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
//										    }
										    
									}
									else
									{
										if(bChildLockButton == 0)
											START_LED_OFF; 
									}
								}
							}
							else  // tub clean is true
							{
								cTub_CleanCount++;
								if(((BalanceTime >= RESET_VALUE)&&(uiMachineState != PROGRAM_END)) || ((bDoorLockStatus == TRUE)&&(uiMachineState == PROGRAM_END)&&(bChildLockButton == FALSE)))// if(BalanceTime > RESET_VALUE)
								{
									if(cTub_CleanCount < 5)
									{
										if(bPhysical_DoorLock_Status  == TRUE)
										{
											START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
										}
										//if(BalanceTime > RESET_VALUE)
										{
										    DisplayTime(BalanceTime); //display balance time
										    if(uiRpmSel == Rpm000)
											{
												turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
											}
											else
											{
												turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
											}
										}
									}
									else
									{
										if(uiMachineState != PROGRAM_END)
										{
											
										mystrcpy((char *)cdigits_data_array, " CLn");
										update_display_data(&cdigits_data_array[0] );
											if(uiRpmSel == Rpm000)
											{
												turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_OFF);
											}
											else
											{
												turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
											}
										}
										else
										{
											if(bPhysical_DoorLock_Status  == TRUE)
											{
												START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
											}
										
											DisplayTime(BalanceTime); //display balance time
											    	if(uiRpmSel == Rpm000)
												{
													turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
												}
												else
												{
													turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
												}
										}
									}
								}
								if(cTub_CleanCount>8)
								{
									cTub_CleanCount = RESET_VALUE;
								}
							}
						
						}
						
						if((uiMachineState != PROGRAM_END)||((uiMachineState == PROGRAM_END)&&(bChildLockButton == TRUE)))
							display_availoption(uidisp_select_option);// //cdigits_data[5] = cdigits_data[5] | uidisp_select_option; // keep on the selected options
							
						
						//if(uidisplay_flash_count == 201)
						//	uidisplay_flash_count = RESET_VALUE; // reset the timer in start mode after 4 sec completed ( 3 sec delay timer  & 1sec delay message cycle completed) 
					}
					
//					for(ii = 0; ii < 6;ii++)
//					{
//						STLED316_7Seg_Write(ii,cdigits_data[ii]); // update the display
//					}

					for(ii = 0xC0; ii < 0xCD; ii= ii + 0x02)
					{
						STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // this is should not be 0x00 , default array have to written
						
					}
					STLED316_7Seg_Write(0XCD,ckey_digit_data[5]);	//ADDED BY -AK FOR PROGRAM LEDS
//					
					
					if(bSpin_Start == FALSE)
						Reinit_Stleddevice();
					
					break;//--------------------------------------------------------------------
					
				case 62: // here display get off if any blinking condition is true (every 500 msec within 8 sec))
				case 150:
				case 236:
				case 323:
				case 410:	//added for AI display
				case 497:
					bFiveHundredMsec_flag = FALSE;
					
					if(cFlash_prog_led >0)
					{
						cFlash_prog_led--;
						
						ResProg_led();
					}
					if(uidoorflash_count>0)
					{
						uidoorflash_count--;
						cdigits_data[5] =  cdigits_data[5] & (~0x01); 	// //turn off the doorlock led
					}
					else
					{
						check_doorlockstatus();
					}
					if(cFlash_Count > 0)
					{
						cFlash_Count--;
						if((bStartButton == FALSE)&&(bProgram_End == FALSE)&& (bTEST_COMPLETED == FALSE))
						{
							if(bPhysical_DoorLock_Status  == TRUE)
							{
								START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
							}
						}
						cdigits_data[0] = cdigits_data[1] = cdigits_data[2] = cdigits_data[3] = 0x00;
						
//						for(ii = 0; ii < 4;ii++)
//						{
//							STLED316_7Seg_Write(ii,cdigits_data[ii]); // NO(blank) display 
//						}

						for(ii = 0xC0; ii < 0xCD; ii= ii + 0x02)
						{
							STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // this is should not be 0x00 , default array have to written
							
						}
						Check_LaundryAdd();
						CheckDrain_PVMVSymbol();
#if ENABLE_IOT
					if((uiMachineState != PROGRAM_END) && (uiMachineState != SNOOZEMODE))
						ChkWifi_Symbol_Off();
#endif
						//chip_select = TRUE; 
					
							if(bStartButton == FALSE)
							{
								//display_availoption(uiPSER_Option_Avbl);
								if(uiRinseholdState != 2)
								{
									display_availoption(uiPSER_Option_Avbl);
								}
								else
								{
									//Rinsehold state no option selection is possible, only spin selection
									//previously selected option only should glow
									display_availoption(uidisp_select_option);
								}
							}
							else
							{
								display_availoption((uidisp_select_option));////cdigits_data[5] = cdigits_data[5] | uidisp_select_option; // countious on led selected option 
							}
						
						chip_select = FALSE; // for selecting the 1st STLED
						
						
					}
					else
					{
						Check_LaundryAdd();
						CheckDrain_PVMVSymbol();
#if ENABLE_IOT
						if((uiMachineState != PROGRAM_END) && (uiMachineState != SNOOZEMODE))
							ChkWifi_Symbol_Off();
						//cdigits_data[4] = cdigits_data[4] & (~0x20);
#endif
						bDisplay_Flag = TRUE;
						
						if(bStartButton == FALSE)
						{
							if((uiMachineState == PAUSE_PRESSED))
							{
								if(uiRinseholdState != 2)
								{
									//display_availoption(uiPSER_Option_Avbl);
									if(uiIDLE_COUNT == RESET_VALUE)
									{
										display_availoption((uidisp_select_option));////cdigits_data[5] = cdigits_data[5] | uidisp_select_option; // countious on led selected option 
									}
									else
									{
										display_availoption((uiOption_closed   & uidisp_select_option ) | uioption_on);
									}
								}
								else
								{
									//Rinsehold state no option selection is possible, only spin selection
									//previously selected option only should glow
									display_availoption(uidisp_select_option);
								}
								/*if(uiIDLE_COUNT == RESET_VALUE)
								{
									display_availoption((uidisp_select_option));////cdigits_data[5] = cdigits_data[5] | uidisp_select_option; // countious on led selected option 
								}
								else
								{
									display_availoption((uiOption_closed   & uidisp_select_option ) | uioption_on);
								}*/
							}
							else
							{
								display_availoption((uiPSER_Option_Avbl & uioption_on));
							}
							
							//------------------------------------------------------------------ 
							//----------------------------
							//drainvalve
							//-------------------------
							//---------------------------------------------- 
							if(( bChildLockButton == TRUE ) &&( uichild_flash_count > RESET_VALUE))
							{
								uichild_flash_count--;
								cdigits_data[5] =  cdigits_data[5] & (~0x02); // // turn off the child lock led
							}
							else if(bChildLockButton == TRUE)
							{
								cdigits_data[5] =  cdigits_data[5] | (0x02); // // turn on the child lock led
							}
							else
							{
								cdigits_data[5] =  cdigits_data[5] & (~0x02); // // turn off the child lock led
								START_LED_OFF; // cdigits_data[4] =  cdigits_data[4] & ~(0x08);	// //turn off the start/puase led
								
							}
							
							//---------------------------------------------- 
							
							
							
							if((bKeyDetected == RESET_VALUE) && (uiDISP_MCount ==0) )// && (bEmegency_door_pull !=1)if((cSwitch_Pressed  != TEMPERATURE_KEY )&&(cSwitch_Pressed  != SPIN_KEY ))
							{
								if(TubClean_Activated == FALSE)
								{
									if(uiRinseholdState == 2)
									{
										//mystrcpy((char *)cdigits_data_array, "H0Ld");
										mystrcpy((char *)cdigits_data_array, " t=#");
										update_display_data(&cdigits_data_array[0] );
										//cdigits_data[4] =  cdigits_data[4] |( 0x08);	// //turn on the start/puase led
										turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_OFF);
									}
									else
									{
										if(((uiopt_check & DelayStart_Yes) == DelayStart_Yes)&&(uiIDLE_COUNT > 0))
										{
											DisplayTime(uidelaymin); 
											
										}
										else
										{	
											if((uiSelected_Position==COTTON)||(uiSelected_Position==MIXEDSOIL)||(uiSelected_Position==SYNTHETIC))
											{
												if(uidisplay_flash_count <101)
												{
													mystrcpy((char *)cdigits_data_array, "  Ae");
													update_display_data(&cdigits_data_array[0] );
													turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
												}
												
												else if((uidisplay_flash_count >=101)&&(uidisplay_flash_count <201))
												{
													DisplayTime(BalanceTime);
													turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
												}
											}
											else
											{
												if(uidisplay_flash_count < 125)
												{
													DisplayTime(BalanceTime);
													turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
												}
											}									
//											if(uidisplay_flash_count < 101)
//											{
//												
//												DisplayTime(BalanceTime);
//												turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
//											}
										}
									}
								}
								
							}
							
						} 
						else if(bStartButton == TRUE)
						{
							//----------------------------
							//drainvalve
							//-------------------------
							if((bKeyDetected == RESET_VALUE) && (uiDISP_MCount ==0))	//if((cSwitch_Pressed  != TEMPERATURE_KEY )&&(cSwitch_Pressed  != SPIN_KEY ))
							{
								if(TubClean_Activated == FALSE)
								{
									if((uidisp_select_option & DelayStart_Yes)&&(uidelaymin > RESET_VALUE))
									{ 
										if(uidisplay_flash_count == 177)
										{
											mystrcpy((char *)cdigits_data_array, " dLy");
											update_display_data(&cdigits_data_array[0] );
											 turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
										}
										else
										{
									    		
											DisplayTime(uidelaymin);
											if(bPhysical_DoorLock_Status  == TRUE)
											{
												START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
											}
											turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
										}
									}
									else
									{
						
										if(((BalanceTime >= RESET_VALUE)&&(uiMachineState != PROGRAM_END)) || ((bDoorLockStatus == TRUE)&&(uiMachineState == PROGRAM_END)&&(bChildLockButton == FALSE)))// if(BalanceTime > RESET_VALUE)
										{
											if(LS_flag!=TRUE)
											{
												i=0; // used for LS display : reseted to start from 1 digit
												if((uiSelected_Position==COTTON)||(uiSelected_Position==MIXEDSOIL)||(uiSelected_Position==SYNTHETIC))
												{
													if(uidisplay_flash_count<101)
													{
														mystrcpy((char *)cdigits_data_array, "  Ae");
														update_display_data(&cdigits_data_array[0] );
														turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
													}
													else if(uidisplay_flash_count>=101)
													{
															DisplayTime(BalanceTime);
														if(bPhysical_DoorLock_Status  == TRUE)
														{
															START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
														}
													    if(uiRpmSel == Rpm000)
													    {
														   turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
													    }
													    else
													    {   
														   turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
														   	if(GET_PROGRAM != STEAMFRESH && uiSelected_Position != ADDITIVESRINSE)
															{
																if(uiMachineState != PROGRAM_END)
																{
																 	turnon_display_OxyJetled(OXYJET_ON);	//TODO: Turn On OxyJet at start of program.
																}
															}
													    }
													    snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT;
													}
												}
												else
												{
													DisplayTime(BalanceTime);
													if(bPhysical_DoorLock_Status  == TRUE)
													{
														START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
													}
												    if(uiRpmSel == Rpm000)
												    {
													   turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
												    }
												    else
												    {   
													   turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
													   	if(GET_PROGRAM != STEAMFRESH && uiSelected_Position != ADDITIVESRINSE)
														{
															if(uiMachineState != PROGRAM_END)
															{
															 	turnon_display_OxyJetled(OXYJET_ON);	//TODO: Turn On OxyJet at start of program.
															}
														}
												    }
												    snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT;
													
												}
											}
											else
											{
												LoadSensing_display();
												Delay_ms(140);
												uidisplay_flash_count=497;	
											}
												
//										    DisplayTime(BalanceTime);
//											if(bPhysical_DoorLock_Status  == TRUE)
//											{
//												START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
//											}
//										    if(uiRpmSel == Rpm000)
//										    {
//											   turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
//										    }
//										    else
//										    {   
//											   turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
//											   	if(GET_PROGRAM != STEAMFRESH && uiSelected_Position != ADDITIVESRINSE)
//												{
//													if(uiMachineState != PROGRAM_END)
//													{
//													 	turnon_display_OxyJetled(OXYJET_ON);	//TODO: Turn On OxyJet at start of program.
//													}
//												}
//										    }
//										    snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT;
										}
									}
								}
								else
								{
									if(((BalanceTime >= RESET_VALUE)&&(uiMachineState != PROGRAM_END)) || ((bDoorLockStatus == TRUE)&&(uiMachineState == PROGRAM_END)&&(bChildLockButton == FALSE)))// if(BalanceTime > RESET_VALUE)
									{
										if(cTub_CleanCount < 5)
										{
											
											    DisplayTime(BalanceTime); //display balance time
												if(bPhysical_DoorLock_Status  == TRUE)
												{
													START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
												}
											    	if(uiRpmSel == Rpm000)
											    	{
													turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
												}
												else
												{
													turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
												}
											
										}
										else
										{
											if(uiMachineState != PROGRAM_END)
											{
												mystrcpy((char *)cdigits_data_array, " CLn");
												update_display_data(&cdigits_data_array[0] );
												if(uiRpmSel == Rpm000)
												{
													turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_OFF);
												}
												else
												{
													turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
												}
											}
											else
											{
												DisplayTime(BalanceTime); //display balance time
												if(bPhysical_DoorLock_Status  == TRUE)
												{
											    	START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
												}
											    if(uiRpmSel == Rpm000)
												{
													turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_ON,CENTI_OFF,TIMELEFT_ON);
												}
												else
												{
													turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_ON);
												}
												
											}
										}
									}
								
								
								
								}
								
								if((uiMachineState != PROGRAM_END)||((uiMachineState == PROGRAM_END)&&(bChildLockButton == TRUE)))
									display_availoption((uidisp_select_option));////cdigits_data[5] = cdigits_data[5] | uidisp_select_option; // countious on led selected option 
							}
							
							if(((bChildLockButton == TRUE) &&(uichild_flash_count > RESET_VALUE))||((bChildLockButton == TRUE) && (uiMachineState == PROGRAM_END)))
							{
								uichild_flash_count--;
								cdigits_data[5] =  cdigits_data[5] & ~(0x02);// // turn off the child lock led
							}
							else if(bChildLockButton == TRUE)
							{
								cdigits_data[5] =  cdigits_data[5] |(0x02); // // turn off the child lock led
							}
						}
					
					}
//					for(ii = 0; ii < 6;ii++)
//					{
//						STLED316_7Seg_Write(ii,cdigits_data[ii]); // update the display
//					}

					for(ii = 0xC0; ii < 0xCD; ii= ii + 0x02)
					{
						STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // this is should not be 0x00 , default array have to written
						
					}
					STLED316_7Seg_Write(0XCD,ckey_digit_data[5]);//ADDED BY -AK FOR PROGRAM LEDS
					//chip_select = TRUE; // for selecting the 1st STLED
//					for(ii = 0; ii < 6;ii++)
//					{
//						STLED316_7Seg_Write(ii,ckey_digit_data[ii]); // this is should not be 0x00 , default array have to written
//						
//					}

					for(ii = 0xC1; ii < 0xCA; ii= ii + 0x02)
					{
						STLED316_7Seg_Write(ii,ckey_digit_data[(ii-0xC1)/2]); // this is should not be 0x00 , default array have to written
						
					}
					chip_select = FALSE; // for selecting the 1st STLED
					
					break; ///--------------------------------------------------------------------
					
				
					
				case 550:
					uidisplay_flash_count = 0;
					break;
				default :
				 break;
			}
			if(uidisplay_flash_count > 551)
				uidisplay_flash_count= RESET_VALUE;
				
			bDisplay_Flag = TRUE ;
			
		}
			
		}
		else
		{
			switch(uidisplay_flash_count)
			{
				case 0x01:
					if(cFlash_C >=1)				
						cFlash_C--;
					
					bFiveHundredMsec_flag = TRUE;
					
					if((bTestMode_Selection== TRUE)&&(bFaciaTest == FALSE)&&(FaciaState == step1)&& (cFlash_Count == 1))
					{
						
						cFlash_Count = 0;
					}
					if(cFlash_C<=0)
					{
						cFlash_C=0;
					}
					
					bDisplay_Flag = TRUE;
					Check_LaundryAdd();
					CheckDrain_PVMVSymbol();
					check_doorlockstatus();
#if ENABLE_IOT
					if((uiMachineState != PROGRAM_END) && (uiMachineState != SNOOZEMODE))
						ChkWifi_Symbol_On();	
#endif
					if((uiMachineState != SNOOZEMODE)&&(uiMachineState != POWEROFF))
					{
						if(bSpin_Start == FALSE)
							Reinit_Stleddevice();
						
						if((bChildLockButton == 1)) 
						{
							cdigits_data[5] =  cdigits_data[5] | (0x02); // // turn on the child lock led
						}
						
						if(((MachineErrors.EReg != 128)&&(uiover_heat == FALSE)&&(bTestMode_Selection==FALSE)&&(bDoorLockError == FALSE))||((bTestMode_Selection == TRUE)&&(uiError_Occured == RESET_VALUE)&&(bEnduranceStarted == FALSE)&&(uiMachineState == TESTMODE)))
						{
							
							if(bTEST_COMPLETED == TRUE)
							{
								START_LED_OFF; // cdigits_data[4] =  cdigits_data[4] &(~ 0x08);	// //turn off the start/puase led
							}
							else
							{
								if(bPhysical_DoorLock_Status  == TRUE)
								{
									START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
								}
							}
						}
						else
						{
							//if((MachineErrors.EReg != 128)&&(uiover_heat == FALSE)&&(bTestMode_Selection==TRUE)&&(bDoorLockError == FALSE))
							if((uiError_Occured == RESET_VALUE)&&(uiover_heat == FALSE)&&(bTestMode_Selection==TRUE)&&(bDoorLockError == FALSE))
							{
								if(bStartButton == FALSE)
								{
									START_LED_OFF; //cdigits_data[4] =  cdigits_data[4] & (~0x08); // //turn off the start/puase led
								}
								else
								{
									if(bPhysical_DoorLock_Status  == TRUE)
									{
										START_LED_ON; //cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
									}
								}
							}
							else
							{
								START_LED_OFF; //cdigits_data[4] =  cdigits_data[4] & (~0x08); // //turn off the start/puase led
							}
								
						}
						
						
						
						
						if(bFCTMode_Selection == TRUE)
						{
							if(bFctCountStart == YES)
							{
								uiTFCTSeconds++;
							}
						}
						else if( bDebugMode_Selection == TRUE)
						{
							if(MachineErrors.EReg == 0)
								display_availoption((uidisp_select_option));			
						}
						
						
						
					}
					else
					{
						
						 if(uiTubClean_Disp == FALSE)
						{
							cdigits_data[0] = cdigits_data[1] = cdigits_data[2] = cdigits_data[3] = cdigits_data[4] = cdigits_data[5] = cdigits_data[6] = 0x00;
							//if(snooze_display == 0)
							{
								//ckey_digit_data[0] = ckey_digit_data[0] &(~ 0x03); // turn off the dots & semicolon
								//cdigits_data[5] =  cdigits_data[5] &  (~0x80);		//  // turn off the semicolan
								//cdigits_data[5] =  cdigits_data[5] & (~0x40); 		// // turn off the time dots
						//AK SNOOZE		STLED316_7Seg_SetBrghtInd(4, snooze_display, 3);
								ckey_digit_data[0] = ckey_digit_data[1] = ckey_digit_data[2] = ckey_digit_data[3] = ckey_digit_data[4] = 0x00; 
								
									
							}
							//uidisplay_flash_count = 0;
						}
					}
//					for(ii = 0; ii < 6;ii++)
//					{
//						STLED316_7Seg_Write(ii,cdigits_data[ii]); // update the display
//					}
					for(ii = 0xC0; ii < 0xCD; ii= ii + 0x02)
					{
						STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // this is should not be 0x00 , default array have to written
						
					}
					STLED316_7Seg_Write(0XCD,ckey_digit_data[5]); //ADDED BY - AK FOR TSTMODE LED
					
					//chip_select = TRUE; // for selecting the 1st STLED
//					for(ii = 0; ii < 6;ii++)
//					{
//						STLED316_7Seg_Write(ii,ckey_digit_data[ii]); // this is should not be 0x00 , default array have to written
//						
//					}
					for(ii = 0xC1; ii < 0xCA; ii= ii + 0x02)
					{
						STLED316_7Seg_Write(ii,ckey_digit_data[(ii-0xC1)/2]); // this is should not be 0x00 , default array have to written
						
					}
					chip_select = FALSE; // for selecting the 1st STLED
					
					if(cFlash_Count >= 0) 	
					{
						//chip_select = TRUE; // for selecting the 1st STLED
//						for(ii = 0; ii < 6;ii++)
//						{
//							STLED316_7Seg_Write(ii,ckey_digit_data[ii]); // this is should not be 0x00 , default array have to written
//							
//						}
//					for(ii = 0xC0; ii < 0xCD; ii= ii + 0x02)//shu
//					{
//						STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // this is should not be 0x00 , default array have to written
//									
//					}
					for(ii = 0xC1; ii < 0xCA; ii= ii + 0x02)
					{
						STLED316_7Seg_Write(ii,ckey_digit_data[(ii-0xC1)/2]); // this is should not be 0x00 , default array have to written
						
					}
						chip_select = FALSE; // for selecting the 1st STLED
					}
				break;
				case 25:
					bFiveHundredMsec_flag = FALSE;
					
					if(cFlash_C >=1)
						cFlash_C--;
					
					if(cFlash_C<=0)
					{
						cFlash_C=0;
					}
					if(uidoorflash_count>0)
					{
						uidoorflash_count--;
						cdigits_data[5] =  cdigits_data[5] & (~0x01); 	// //turn off the doorlock led
					}
					else
					{
						check_doorlockstatus();
					}
					bDisplay_Flag = TRUE;
						
					if(bFCTMode_Selection == TRUE)
					{
						if(bFctCountStart == YES)
						{
							uiTFCTSeconds++;
						}
					}
					
					
					if((uiMachineState != SNOOZEMODE)&&(uiMachineState != POWEROFF))
					{
						if( bDebugMode_Selection == TRUE)
						{
							if(MachineErrors.EReg == 0)
								display_availoption((uidisp_select_option));	
						}
						if(((MachineErrors.EReg != 128)&&(uiover_heat == FALSE)&&(bTestMode_Selection==FALSE)&&(bDoorLockError == FALSE))||((bTestMode_Selection == TRUE)&&(uiError_Occured == RESET_VALUE)&&(bEnduranceStarted == FALSE)&&(uiMachineState == TESTMODE)))
						{
							if((bProgram_End == FALSE)&& (bTEST_COMPLETED == FALSE))
							{
								if(bStartButton == FALSE)
								{
									START_LED_OFF; //cdigits_data[4] =  cdigits_data[4] &(~ 0x08);	// //turn off the start/puase led
								}
								else
								{
									if(bPhysical_DoorLock_Status  == TRUE)
									{
										START_LED_ON; //cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
									}
								}
							}
							else
							{
								START_LED_OFF; //cdigits_data[4] =  cdigits_data[4] &(~ 0x08);	// //turn off the start/puase led
							}
						}
						else
						{
							if((MachineErrors.EReg != 128)&&(uiover_heat == FALSE)&&(bTestMode_Selection==TRUE)&&(bDoorLockError == FALSE))
							{
								if(bStartButton == FALSE)
								{
									START_LED_OFF; //cdigits_data[4] =  cdigits_data[4] & (~0x08); // //turn off the start/puase led
								}
								else
								{
									if(bPhysical_DoorLock_Status  == TRUE)
									{
										START_LED_ON; //cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
									}
								}
							}
							else
								START_LED_OFF; //cdigits_data[4] =  cdigits_data[4] & (~0x08); // //turn off the start/puase led
						}
						
						//-------------------------------------------------------------------------
						// check the doorlock
						Check_LaundryAdd();//need to verify!!
						CheckDrain_PVMVSymbol();
#if ENABLE_IOT
						if((uiMachineState != PROGRAM_END) && (uiMachineState != SNOOZEMODE))
							ChkWifi_Symbol_Off();
#endif
						//-------------------------------------------------------------------------
						
						if(cFlash_Count > 0)
						{
							
							cFlash_Count--;
							if(((bChildLockButton == TRUE) &&(uichild_flash_count > RESET_VALUE))||((bChildLockButton == TRUE) && (uiMachineState == PROGRAM_END)))
							{
								uichild_flash_count--;
								cdigits_data[5] =  cdigits_data[5] & ~(0x02);// // turn off the child lock led
							}
							else if(bChildLockButton == TRUE)
							{
								cdigits_data[5] =  cdigits_data[5] |(0x02); // // turn off the child lock led
							}
							
							if((MachineErrors.EReg == 0)&& (bTestMode_Selection== TRUE)&&(bFaciaTest == TRUE))
							{
								mystrcpy((char *)cdigits_data_array, "    ");
								update_display_data(&cdigits_data_array[0] );
								//cdigits_data[5] =  cdigits_data[5] &  (~0x40);//  // turn off the time dots				
								
//								for(ii = 0; ii < 6;ii++)
//								{
//									STLED316_7Seg_Write(ii,cdigits_data[ii]); // update the display
//								}

								for(ii = 0xC0; ii < 0xCD; ii= ii + 0x02)
								{
									STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // this is should not be 0x00 , default array have to written
									
								}
								//shu-1
								for(ii = 0xC1; ii < 0xCA; ii= ii + 0x02)
								{
									STLED316_7Seg_Write(ii,ckey_digit_data[(ii-0xC1)/2]); // this is should not be 0x00 , default array have to written
									
								}
							}
							else if(MachineErrors.EReg != 0)
							{
								
								if(((MachineErrors.EReg != 128)&&(uiover_heat == FALSE)&&(bTestMode_Selection==FALSE)&&(bDoorLockError == FALSE))||((bTestMode_Selection == TRUE)&&(uiError_Occured == RESET_VALUE)&&(bEnduranceStarted == FALSE)&&(uiMachineState == TESTMODE)))
								{
									if((bStartButton == TRUE)&&(bPhysical_DoorLock_Status  == TRUE))
									{
										START_LED_ON;//cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
									}
								}
								else
								{
									START_LED_OFF; // cdigits_data[4] =  cdigits_data[4] & (~0x08); // //turn on the start/puase led
								}
								
								//following point modified to avoid door led blinking during error
								cdigits_data[0] = cdigits_data[1] = cdigits_data[2] = cdigits_data[3] = cdigits_data[4] = 0x00;cdigits_data[5] &= 0x05; //ADDED FOR BLINKING OF LEDS DURING ERROR - AK
								ckey_digit_data[0] = ckey_digit_data[1] = ckey_digit_data[2] = ckey_digit_data[3] =ckey_digit_data[4] =/*ckey_digit_data[5] =*/ 0X00;
								
//								for(ii = 0; ii < 6;ii++)
//								{
//									STLED316_7Seg_Write(ii,cdigits_data[ii]); // NO(blank) display 
//								}
								
								for(ii = 0xC0; ii < 0xCD; ii= ii + 0x02)
								{
									STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // this is should not be 0x00 , default array have to written
									
								}
								STLED316_7Seg_Write(0XCD,ckey_digit_data[5]); //ADDED BY - AK FOR TSTMODE LED
								
								//shu-1
								for(ii = 0xC1; ii < 0xCA; ii= ii + 0x02)
								{
									STLED316_7Seg_Write(ii,ckey_digit_data[(ii-0xC1)/2]); // this is should not be 0x00 , default array have to written
									
								}
								
								//chip_select = TRUE; 
								if((uiMachineState == PAUSE_PRESSED))
								{
									if(uiIDLE_COUNT == RESET_VALUE)
										display_availoption((uidisp_select_option));////cdigits_data[5] = cdigits_data[5] | uidisp_select_option; // countious on led selected option 
									else
										display_availoption((uiPSER_Option_Avbl & uioption_on));
								}
								else{
									display_availoption((uiPSER_Option_Avbl & uioption_on));
								}
								chip_select = FALSE; // for selecting the 1st STLED
							}
							else
							{
//								for(ii = 0; ii < 6;ii++)
//								{
//									STLED316_7Seg_Write(ii,0x00); // NO(blank) display 
//								}
								for(ii = 0xC0; ii <= 0xCD; ii= ii + 0x01/*0x02*/)// this is for blinking in test mode
								{
									STLED316_7Seg_Write(ii,0x00); // this is should not be 0x00 , default array have to written
									
								}
								
								//chip_select = TRUE; 
//								for(ii = 0; ii < 6;ii++)
//								{
//									STLED316_7Seg_Write(ii,0x00); // this is should not be 0x00 , default array have to written
//									
//								}
//								for(ii = 0xC0; ii < 0xCD; ii= ii + 0x02)  //AK
//								{
//									STLED316_7Seg_Write(ii,0x00);
//								}
								
								chip_select = FALSE; // for selecting the 1st STLED
								START_LED_OFF
							}
							
							
						}
						else
						{
//							for(ii = 0; ii < 6;ii++)
//							{
//								STLED316_7Seg_Write(ii,cdigits_data[ii]); // update the display
//							}

							for(ii = 0xC0; ii < 0xCD; ii= ii + 0x02)
							{
								STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // this is should not be 0x00 , default array have to written
								
							}
							//shu-1
								for(ii = 0xC1; ii < 0xCA; ii= ii + 0x02)
								{
									STLED316_7Seg_Write(ii,ckey_digit_data[(ii-0xC1)/2]); // this is should not be 0x00 , default array have to written
									
								}
						}
					}
						
					break;
				case 50:
						uidisplay_flash_count = 0;
						// add count for display tub clean 
						if(uiTubClean_Disp == TRUE)
						{
							uiTubClean_MSG_Count++;
							if(uiTubClean_MSG_Count > 6)
							{
								uiTubClean_MSG_Count = FALSE;
							}
							
							uiTubclean_MSG_TMOUT_Count++;
							
							if( uiTubclean_MSG_TMOUT_Count >= 300)
							{
								uiTubClean_Disp = FALSE; 	
							}
							chip_select = FALSE;
//							for(ii = 0; ii < 6;ii++)
//							{
//								STLED316_7Seg_Write(ii,cdigits_data[ii]); // update the display
//							}

							for(ii = 0xC0; ii < 0xCD; ii= ii + 0x02)
							{
								STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // this is should not be 0x00 , default array have to written
								
							}
						}
						
					break;
			}
			
			if(uiMachineState == SNOOZEMODE)
			{
				if((snoozestart_onoff == 0x01))
				{
					uiSTART_Led_on = 20; //	START_LED_ON; // cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
					uitemp_onoff = 0x00;
					if(uiSTART_Led_on >20)
						uiSTART_Led_on = RESET_VALUE;
				}
				if((uidisplay_flash_count > 1)&&(uiTubClean_Disp == FALSE))
				{
					uidisplay_flash_count = RESET_VALUE;
					bDisplay_Flag = TRUE;
				}
				else if(uidisplay_flash_count>50)
				{
					uidisplay_flash_count = RESET_VALUE;
					bDisplay_Flag = TRUE;
				}
			}
			else
			{
				if(uidisplay_flash_count > 50)
				{
					uidisplay_flash_count = RESET_VALUE;
				}
			}
			bDisplay_Flag = TRUE;
		}
	bDisplay_Flag = TRUE;
	Display_off_testing = 0x00;
	//chip_select = TRUE; // for selecting the 1st STLED 
	bkeys_display_flag = FALSE;
	}
	else
	{
		
		if(Display_off_testing >= 1200)
		{
			Display_off_testing = 0x00;
			START_LED_OFF;
			//chip_select = TRUE; 	
			//STLED316_DisplayOff();//shu
			chip_select = FALSE; 	
			STLED316_DisplayOff();
		}
	}
    //------------------------------------------------
}

void display_Selected_options()
{
	if( uidelaymin > 0)
	{
		ckey_digit_data[0] = ckey_digit_data[0] | DelayStart_Yes;// 	// turn on the delay selction led
	}
	if( (uidisp_select_option & TimeSaver_Yes) == TimeSaver_Yes)
	{
		ckey_digit_data[0] = ckey_digit_data[0] | TimeSaver_Yes;// 	// turn on the time saver selction led
		bTimeSaver_Selected = TRUE;
	}
	if( (uidisp_select_option & Steam_Yes) == Steam_Yes)
	{
		ckey_digit_data[0] = ckey_digit_data[0] | Steam_Yes;//	// turn on the hot rinse selction led
	}
	if( (uidisp_select_option & ExtraRinse_Yes) == ExtraRinse_Yes)
	{
		ckey_digit_data[0] = ckey_digit_data[0] | ExtraRinse_Yes;// 	// turn on the Extra rinse selction led
	}
	if((uidisp_select_option & WarmSoak_Yes) == WarmSoak_Yes)
	{
		 ckey_digit_data[0] = ckey_digit_data[0] | WarmSoak_Yes;// 	// turn on the soak selction led
	}
	if( (uidisp_select_option & Prewash_Yes) == Prewash_Yes)
	{
		ckey_digit_data[0] = ckey_digit_data[0] | Prewash_Yes;// // turn on the Prewash selction led
		bprewash_selected = TRUE;
	}
	//-------------
	if((bProgramStart == FALSE)&&(bTestMode_Selection == FALSE))
	{
		ProgramTimming();
	}
}

/***********************************************************************
* Function_Name : turnon_display_OxyJetled(char ).
* Ret_type	: void.
* Working	: To turn & off oxyjet led.
* Developed By	: Prashant_H
* Date		: 12/04/2022
************************************************************************/

void turnon_display_OxyJetled(uint8_t oxy_jet_led)
{
	if(oxy_jet_led)
	{	
		//cdigits_data[5] = cdigits_data[5] | (0x08);
	}
	else
	{
		//cdigits_data[5] = cdigits_data[5] & ~(0x08);
	}
}

void LoadSensing_display()
{
	uint8_t j;
	for(j = 0;j<6;j++) cdigits_data[j] = (cdigits_data[j] & 0x80);
	turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
	if((i==0)||(i==4))
	{
		i=1;//0
	}
			switch(dig_no)
			{
				case 0:
					//for(j = 0;j<6;j++) cdigits_data[j] = (cdigits_data[j] & 0x80);
					cdigits_data[i] = (cdigits_data[i] & 0x80)|(0x01);
					dig_no = 1;
					break;
				case 1:
					//for(j = 0;j<6;j++) cdigits_data[j] = (cdigits_data[j] & 0x80);
					cdigits_data[i] = (cdigits_data[i] & 0x80)|(0x03);
					dig_no = 2;
					break;
				case 2:
					//for(j = 0;j<6;j++) cdigits_data[j] = (cdigits_data[j] & 0x80);
					cdigits_data[i] = (cdigits_data[i] & 0x80)|(0x06);
					dig_no = 3;
					break;
				case 3:
					//for(j = 0;j<6;j++) cdigits_data[j] = (cdigits_data[j] & 0x80);
					cdigits_data[i] = (cdigits_data[i] & 0x80)|(0x0C);
					dig_no = 4;
					break;
				case 4:
					//for(j = 0;j<6;j++) cdigits_data[j] = (cdigits_data[j] & 0x80);
					cdigits_data[i] = (cdigits_data[i] & 0x80)|(0x18);
					dig_no = 5;
					break;
				case 5:
					//for(j = 0;j<6;j++) cdigits_data[j] = (cdigits_data[j] & 0x80);
					cdigits_data[i] = (cdigits_data[i] & 0x80)|(0x30);
					dig_no = 6;
					break;
				case 6:
					//for(j = 0;j<6;j++) cdigits_data[j] = (cdigits_data[j] & 0x80);
					cdigits_data[i] = (cdigits_data[i] & 0x80)|(0x21);
					dig_no = 0;
					i=i+1;
					break;
			}
//	uidisplay_flash_count=0;
//	for(j=0;j<6;j++)
//	{
//		STLED316_7Seg_Write(j,cdigits_data[j]); // update the display
//	}
	for(j = 0xC0; j < 0xCD; j= j + 0x02)
	{
		STLED316_7Seg_Write(j,cdigits_data[(j-0xC0)/2]); // update the display
	}
	if(i==4)
	{
		i=1;//0
	}
}

/***********************************************************************************************************************
* Function Name: LaundryAdd
* Description  : This function decides laundry Add led status
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Check_LaundryAdd()
{
	if((bTestMode_Selection== FALSE)&&(bDebugMode_Selection == FALSE)&& (bFCTMode_Selection == FALSE) && (MachineErrors.EReg == 0)&&(TubClean_Activated == FALSE))
	{
		if((iWaterLevel > ON_PAUSE_DOOR_OPEN_WATERLEVEL) && (uiTemperature < 50) 
		&& (bOverHeat == FALSE)&&(uiTumblingCount <= DRAIN))
		{
			cdigits_data[5] =  cdigits_data[5] | (0x08); //turn on laundry add LED
		}
		else
		{
			cdigits_data[5] =  cdigits_data[5] & (~0x08); //turn off laundry add LED
		}
	}
	else if ((bFaciaTestStep == TRUE) || (bFCTMode_Selection == TRUE))
	{
		cdigits_data[5] =  cdigits_data[5] | (0x08); //turn on laundry add LED
	}
	else
	{
		cdigits_data[5] =  cdigits_data[5] & (~0x08); //turn off laundry add LED
	}
}
