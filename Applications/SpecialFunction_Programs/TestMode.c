
#include "TestMode.h"
#include "r_cg_userdefine.h"
#include "STLED316s.h"
#include "Selection.h"
#include "WashOperations_Global.h"


//**************************************************************//
//********************  Local variables ************************//
boolean bEnduranceStarted,bFaciaTest;

uint8_t testmode_clear,cFlash_C;
uint8_t rpm_Selection,update_display;

//*************************************************************//
//********************global variables************************//

extern boolean bWashProgram_Selected,bEnduranceMode,bProgramStart;
extern boolean bDisplay_Flag,bStartButton,bDebugMode_Selection;
extern boolean bTESTMODE_START,bRapidWashButton;
extern boolean bDebugMode_Selection,bTimeSaver_Start,bDelay_Start1,bprewash_selected;
extern boolean bPrewash_Start,bSoak_Start,bHotRinse_Start,bExtraRinse_Start,bSteam_Start,uiUB_decidation;

//extern uint8_t uiTestModeState,uiMachineState,uiSelectedTone;
extern uint8_t uiTestModeState,uiMachineState;
extern volatile uint8_t uiSelectedTone;
extern uint8_t uiSelected_Position,uiProgram_Position,cdigits_data_array[6],ckey_digit_data[6];//
extern uint8_t uiTempSel,uiExtraRinse_Press_count,uidisp_select_option;
extern uint8_t uiRpmSel,cdigits_data[7];
extern uint8_t uiIDLE_COUNT,uiDISP_MCount;
extern uint8_t uimaxspeed_temp,ii;

extern uint8_t PROG_SW_6_10_count;

extern uint16_t BalanceTime;
extern uint16_t uiSF7_cyclecompleted;
extern int cSwitch_Pressed;
extern uint16_t uiSF6_cyclecompleted;

//******** global function prototype ******//
extern void HeaterFailure();
extern void PressureSwitchFailure();
extern void ProgSel_led();


/***********************************************************************************************************************
* Function Name: TestMode()
* Description  : This function implements for Special Program excution.
* Arguments    : None
* Return Value : boolean
***********************************************************************************************************************/
boolean TestMode()
{	
	
	if(bDisplay_Flag == TRUE)
	{
		if(testmode_clear == RESET_VALUE)
		 {
			 testmode_clear = 0x01;
			 ckey_digit_data[0] = ckey_digit_data[1] = ckey_digit_data[2] =  ckey_digit_data[3] = ckey_digit_data[4] = ckey_digit_data[5] = 0x00; // shu
			 if(MachineErrors.EReg == 0)
			 uiSelectedTone = KEYPRESSTONE;
		 }
		 
		if(bTESTMODE_START == FALSE)
		{
			uiSelected_Position = uiProgram_Position;	
			if(uiSelected_Position >7)
			{
				uiProgram_Position = uiSelected_Position = 7;
				ProgSel_led();
				
			}
			if((uiSelected_Position == 7)&&(bEnduranceStarted == FALSE))
			{	
				uiTestModeState = SF07;
				if(MachineErrors.EReg == 0)
				{
					cdigits_data[0] = (cdigits_data[0] & 0x80);//|(0x6D);
					cdigits_data[1] = (cdigits_data[1] & 0x80)|(0x6D);//|(0x71);
					cdigits_data[2] = (cdigits_data[2] & 0x80)|(0x71);//|(0x3F);
					cdigits_data[3] = (cdigits_data[3] & 0x80)|(0x07);	
					//mystrcpy((char *)cdigits_data_array, "SF07");
				}
				BalanceTime = COTTON60_TIME+25;
				//uiRpmSel = Rpm1400; 
				uiRpmSel = Rpm1200;
				uiTempSel = Wash60;
				uiUB_decidation = 0; //added to select which unbalance table to be followed
				
			}
			else if((uiSelected_Position == 6)&&(bEnduranceStarted == FALSE))
			{	
				uiTestModeState = SF06;
				if(uiDISP_MCount <= RESET_VALUE)
				{
					if(MachineErrors.EReg == 0)
					{
						cdigits_data[0] = (cdigits_data[0] & 0x80);//|(0x6D);
						cdigits_data[1] = (cdigits_data[1] & 0x80)|(0x6D);//|(0x71);
						cdigits_data[2] = (cdigits_data[2] & 0x80)|(0x71);//|(0x3F);
						cdigits_data[3] = (cdigits_data[3] & 0x80)|(0x7D);
						 //mystrcpy((char *)cdigits_data_array, "SF06");
					//	 cdigits_data[5] =  cdigits_data[5] & ~(0x08);  		// // turn off the no spin symbol	
					//	cdigits_data[4] =  cdigits_data[4] & (~0x08); // // turn off the rpm symbol	
					}
				}
				BalanceTime = THIRTY_TWO_MINUTES;//THIRTY_THREE_MINUTES;//THIRTY_SIX_MINUTES;
				if(rpm_Selection == RESET_VALUE)
				{
					rpm_Selection = TRUE;
					//uiRpmSel = Rpm1400;
					uiRpmSel = Rpm1200;
				}
				uimaxspeed_temp = Rpm1200;//Rpm1400; //Rpm1200;	//Rpm1400;
				uiUB_decidation = 0;
			}
			else if(uiSelected_Position == 5)
			{
				if(MachineErrors.EReg == 0)
				{
					cdigits_data[0] = (cdigits_data[0] & 0x80);//|(0x6D);
					cdigits_data[1] = (cdigits_data[1] & 0x80)|(0x6D);//|(0x71);
					cdigits_data[2] = (cdigits_data[2] & 0x80)|(0x71);//|(0x3F);
					cdigits_data[3] = (cdigits_data[3] & 0x80)|(0x6D);	
				//mystrcpy((char *)cdigits_data_array, "SF05");
				}	
			}
			else if(uiSelected_Position == 4)
			{
				uiTestModeState = SF04;
				if(uiDISP_MCount <= RESET_VALUE)
				{
					if(MachineErrors.EReg == 0)
					{
						cdigits_data[0] = (cdigits_data[0] & 0x80);//|(0x6D);
						cdigits_data[1] = (cdigits_data[1] & 0x80)|(0x6D);//|(0x71);
						cdigits_data[2] = (cdigits_data[2] & 0x80)|(0x71);//|(0x3F);
						cdigits_data[3] = (cdigits_data[3] & 0x80)|(0x66);
						//mystrcpy((char *)cdigits_data_array, "SF04");
					//	cdigits_data[5] =  cdigits_data[5] & ~(0x08);  		// // turn off the no spin symbol	
					//	cdigits_data[4] =  cdigits_data[4] & (~0x08); // // turn off the rpm symbol	
					}	
					
					if(rpm_Selection == RESET_VALUE)
					{
						rpm_Selection = TRUE;
						uiRpmSel = Rpm1200;//Rpm1400;
					}
					uimaxspeed_temp =  Rpm1200;//Rpm1400;	
				}
			}
			else if((uiSelected_Position == 3)&&(bEnduranceStarted == FALSE))
			{
				uiTestModeState = SF03;
				if(uiDISP_MCount <= RESET_VALUE)
				{
					if(MachineErrors.EReg == 0)
					{
						//mystrcpy((char *)cdigits_data_array, "SF03");
						cdigits_data[0] = (cdigits_data[0] & 0x80);//|(0x6D);
						cdigits_data[1] = (cdigits_data[1] & 0x80)|(0x6D);//|(0x71);
						cdigits_data[2] = (cdigits_data[2] & 0x80)|(0x71);//|(0x3F);
						cdigits_data[3] = (cdigits_data[3] & 0x80)|(0x4F);
				//		cdigits_data[5] =  cdigits_data[5] & ~(0x08);  		// // turn off the no spin symbol	
				//		cdigits_data[4] =  cdigits_data[4] & (~0x08); // // turn off the rpm symbol	
					}	
					
					if(rpm_Selection == RESET_VALUE)
					{
						rpm_Selection = TRUE;
						uiRpmSel = Rpm1200;//Rpm1400;
					}
					uimaxspeed_temp =  Rpm1200;//Rpm1400;	
				}
				
			}
			else if(uiSelected_Position == 2)
			{
				uiTestModeState = SF02;
				ckey_digit_data[0] =  ckey_digit_data[1] =  /*ckey_digit_data[2] = - by -AK*/ 0x00; // clear the selected options & navigation mark in error condition
				if(MachineErrors.EReg == 0)
				{
					cdigits_data[0] = (cdigits_data[0] & 0x80);//|(0x6D);
					cdigits_data[1] = (cdigits_data[1] & 0x80)|(0x6D);//|(0x71);
					cdigits_data[2] = (cdigits_data[2] & 0x80)|(0x71);//|(0x3F);
					cdigits_data[3] = (cdigits_data[3] & 0x80)|(0x5B);
					//mystrcpy((char *)cdigits_data_array, "SF02");
				}
				turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
				turnon_display_OxyJetled(OXYJET_OFF);	//TODO: Turn OFF OxyJet led.
			}
			else if(uiSelected_Position == 1)
			{
				uiTestModeState = SF01;//shu
				ckey_digit_data[0] =  ckey_digit_data[1] =  /*ckey_digit_data[2] = - by -AK*/  ckey_digit_data[3]= ckey_digit_data[4] = ckey_digit_data[5] = 0x00; // clear the selected options & navigation mark in error condition
				if(MachineErrors.EReg == 0)
				{					
					cdigits_data[0] = (cdigits_data[0] & 0x80);//(0x6D);
					cdigits_data[1] = (cdigits_data[1] & 0x80)|(0x6D);//(0x71);
					cdigits_data[2] = (cdigits_data[2] & 0x80)|(0x71);//(0x3F);
					cdigits_data[3] = (cdigits_data[3] & 0x80)|(0x06);
					//mystrcpy((char *)cdigits_data_array, "SF01");
				}
				turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
				//turnon_display_OxyJetled(OXYJET_OFF);	//TODO: Turn OFF OxyJet led.
				
			}
			
			
			//update_display_data(&cdigits_data_array[0] );
		}
		else 
		{
			switch(uiTestModeState)
			{
				case SF01:
				case SF02:		bRapidWashButton = FALSE;
							SpecialFunction1();
							break;
				case SF04:
				case SF03:
							if(bStartButton == TRUE)
							{
								SpecialFunction3();
								HeaterFailure();
					 			PressureSwitchFailure();
								bEnduranceStarted = TRUE;
							}
							else
							{
								
								bEnduranceStarted = FALSE;
								if(uiDISP_MCount == RESET_VALUE)
								{	if(uiTestModeState == SF03)
									{
										//mystrcpy((char *)cdigits_data_array, "SF03");
										cdigits_data[0] = (cdigits_data[0] & 0x80)|(0x6D);
										cdigits_data[1] = (cdigits_data[1] & 0x80)|(0x71);
										cdigits_data[2] = (cdigits_data[2] & 0x80)|(0x3F);
										cdigits_data[3] = (cdigits_data[3] & 0x80)|(0x4F);
									}
									else
									{
										//mystrcpy((char *)cdigits_data_array, "SF04");
										cdigits_data[0] = (cdigits_data[0] & 0x80)|(0x6D);
										cdigits_data[1] = (cdigits_data[1] & 0x80)|(0x71);
										cdigits_data[2] = (cdigits_data[2] & 0x80)|(0x3F);
										cdigits_data[3] = (cdigits_data[3] & 0x80)|(0x66);
									}	
									//update_display_data(&cdigits_data_array[0] );
									//cdigits_data[5] =  cdigits_data[5] & (~0x80); // turn off the rpm symbol
								}
							}
							break;
				
				case SF05:
							
							break;
				case SF06:
							if(bStartButton == TRUE)
							{
								uiProgram_Position = uiSelected_Position= ADDITIVESRINSE;
								ProgSel_led();
								SpecialFunction6();
								HeaterFailure();
					 			PressureSwitchFailure();
								bEnduranceStarted = TRUE;
							}
							
							break;
				case SF07:
							if(bStartButton == TRUE)
							{
								uiProgram_Position=uiSelected_Position = COTTON;
								ProgSel_led();
								bRapidWashButton = FALSE;
								SpecialFunction7();
								HeaterFailure();
					 			PressureSwitchFailure();
								bEnduranceStarted = TRUE;
							}
							
							break;			
				case SF08:
							
							
							break;						
				default:
							break;

			}
			if(uiTestModeState < SF06)
			{
				bTimeSaver_Start = bDelay_Start1  = bPrewash_Start = bSoak_Start = bHotRinse_Start = bExtraRinse_Start = bSteam_Start = FALSE;
				uidisp_select_option = 0x00;
				uiExtraRinse_Press_count = RESET_VALUE;
				bprewash_selected = RESET_VALUE;
				
			}
			else if((uiTestModeState == SF06)||(uiTestModeState == SF07))
			{
				bTimeSaver_Start = bDelay_Start1  = bPrewash_Start = bSoak_Start = bHotRinse_Start = bExtraRinse_Start =  FALSE;
				uidisp_select_option = 0x08;
				uiExtraRinse_Press_count = RESET_VALUE;
				bprewash_selected = RESET_VALUE;
			}
		}
		if(( bDebugMode_Selection == FALSE)&&(update_display == 10)) // this flag is reseted in debug mode also if we make reset here then debug message not get diplayed
		{
			bDisplay_Flag = FALSE;
		}
		else
		{
			update_display++;
		}
		return 0;
		
	}
	
}


/***********************************************************************************************************************
* Function Name: Display_Message_SF45678()
* Description  : This function displays the number of cycles completed.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void Display_Message_SF45678()
{
	if((MachineErrors.EReg == 0) && (bDebugMode_Selection == FALSE))
	{
		
		//cdigits_data[5] = RESET_VALUE;//water fill led flickering issue
		if(cFlash_C == RESET_VALUE)
		{
			cFlash_C = 9;
			cdigits_data[4] =  cdigits_data[4] & (~0x01); //turn off the time left led
		}
		if(cFlash_C > 6 )
		{
			//mystrcpy((char *)cdigits_data_array, " P07");
			if(uiTestModeState == SF05)
			{
				cdigits_data[0] = (cdigits_data[0] & 0x80);//|(0x6D);
				cdigits_data[1] = (cdigits_data[1] & 0x80)|(0x6D);//(0x71);
				cdigits_data[2] = (cdigits_data[2] & 0x80)|(0x71);//(0x3F);
				cdigits_data[3] = (cdigits_data[3] & 0x80)|(0x6D);
			}
				//mystrcpy((char *)cdigits_data_array, "SF05");
			else if(uiTestModeState == SF06)
			{
				cdigits_data[0] = (cdigits_data[0] & 0x80);//|(0x6D);
				cdigits_data[1] = (cdigits_data[1] & 0x80)|(0x6D);//(0x71);
				cdigits_data[2] = (cdigits_data[2] & 0x80)|(0x71);//(0x3F);
				cdigits_data[3] = (cdigits_data[3] & 0x80)|(0x7D);
			}
				//mystrcpy((char *)cdigits_data_array, "SF06");
			else if(uiTestModeState == SF07)
			{
				cdigits_data[0] = (cdigits_data[0] & 0x80);//|(0x6D);
				cdigits_data[1] = (cdigits_data[1] & 0x80)|(0x6D);//(0x71);
				cdigits_data[2] = (cdigits_data[2] & 0x80)|(0x71);//(0x3F);
				cdigits_data[3] = (cdigits_data[3] & 0x80)|(0x07);
			}
				//mystrcpy((char *)cdigits_data_array, "SF07");
				
			//update_display_data(&cdigits_data_array[0] );
			//cdigits_data[5] =  cdigits_data[5] &  (~0x04); // // turn off the time dots//need to confirm 				
			cdigits_data[4] =  cdigits_data[4] & (~0x0F); 	//  // turn off the time left led 0C symbol, rpm symbol	
		}
		else 
		
		{
			DisplayTime(BalanceTime);
			if((cFlash_C == 1 )||(cFlash_C == 3 )||(cFlash_C == 5 ))
			{
				
				//cdigits_data[5] =  cdigits_data[5] & (~0x04); 		// // turn off the time dots
				cdigits_data[4] =  cdigits_data[4] & (~0x40); //neo P1 dot changes
			}
			else
			{
				//cdigits_data[5] =  cdigits_data[5] |  (0x04); // // turn on the time dots
				cdigits_data[4] =  cdigits_data[4] | (0x40);//neo P1 dot changes
				
			}
			//cdigits_data[4] =  cdigits_data[4] | 0x10; // //turn on the time left led
			cdigits_data[4] =  cdigits_data[4] | (0x01); 		//  // turn on the time left led
		}
		
		//if(cSwitch_Pressed == OPTION_KEY)
		if(cSwitch_Pressed == SELECT_KEY)
		{
//			 if(uiTestModeState == SF06)
//				SevenSegPrint(uiSF6_cyclecompleted);
//			else if(uiTestModeState == SF07)
//				SevenSegPrint(uiSF7_cyclecompleted);
//			//uitest_key = RESET_VALUE;
//			cdigits_data[5] =  cdigits_data[5] & (~0x04); 		// // turn off the time dots
//			cdigits_data[4] =  cdigits_data[4] & (~0x01); 		//  // turn off the time left led

			if(uiTestModeState == SF06)
			{
				//SevenSegPrint(uiSF6_cyclecompleted);
				if(uiSF6_cyclecompleted == RESET_VALUE || uiSF6_cyclecompleted > MAX_TEST_MODE_CYCLE)
				{
					mystrcpy((char *)cdigits_data_array, " 000");
					update_display_data(&cdigits_data_array[0] );
				}
				else
				{
					
					SevenSegPrint(uiSF6_cyclecompleted);	
					cdigits_data[0] = (cdigits_data[0] & 0x80);//|(0x6D);
					for(ii = 0xC0; ii < 0xC3; ii= ii + 0x02)
					{
						STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // update the display
					}
				}
				
			}
			else if(uiTestModeState == SF07)
			{
				//SevenSegPrint(uiSF7_cyclecompleted);
				if(uiSF7_cyclecompleted == RESET_VALUE || uiSF7_cyclecompleted > MAX_TEST_MODE_CYCLE)
				{
					mystrcpy((char *)cdigits_data_array, " 000");
					update_display_data(&cdigits_data_array[0] );
				}
				else
				{
					
					SevenSegPrint(uiSF7_cyclecompleted);	
					cdigits_data[0] = (cdigits_data[0] & 0x80);//|(0x6D);
					for(ii = 0xC0; ii < 0xC3; ii= ii + 0x02)
					{
						STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // update the display
					}
				}
			}
			//uitest_key = RESET_VALUE;
			//cdigits_data[5] =  cdigits_data[5] & (~0x04); 		// // turn off the time dots
			cdigits_data[4] =  cdigits_data[4] & (~0x40);			// turn off the time dots
			//cdigits_data[4] =  cdigits_data[4] & (~0x01); 		//  // turn off the time left led
			cdigits_data[5] =  cdigits_data[5] & (~0x20);			// turn off the time left led
		}
	}
}

