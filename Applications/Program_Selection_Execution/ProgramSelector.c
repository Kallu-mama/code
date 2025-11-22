#include "keyevent.h"
#include "r_cg_userdefine.h"
//#include "Program.h"
#include "ProgramSelector.h"

//**************************** global variables ***********************//

extern boolean bProgramStart,bEnduranceStarted,bDoorLockStatus,bprewash_selected; 
extern boolean bPrewash_Start,bSoak_Start,bHotRinse_Start,bExtraRinse_Start;
extern boolean bTestMode_Selection,bChildLockButton,bTimeSaver_Start,bStartButton;
extern boolean bTEST_COMPLETED,TubClean_Activated,bProgram_Start,bDelay_Start1;
extern boolean bDebugMode_Selection,bFCTMode_Selection,bFaciaTest,bDetectionCompleted;
extern boolean bRinsehold_Finished,bKeyDetected,bRapidWashButton,bSteam_Start;
extern boolean b_option_key_pressed_flag;//bExpress_30_Activatted;
//extern boolean PROG_SW_1_5_flag,PROG_SW_6_10_flag; // added for neo without encoder
//extern uint8_t PROG_SW_1_5_count,PROG_SW_6_10_count; //COMMENTED BCZ THESE NOT USED FOR WDR
extern boolean bTub_deselect;//neo

//extern uint8_t encodercofirm,uiSelectedTone,uiOption_closed;
extern uint8_t encodercofirm,uiOption_closed;
extern volatile uint8_t uiSelectedTone;
extern uint8_t cFlash_Count,uiEncoderValue,uiMachineState;
extern uint8_t uiProgram_Position,uiEncoderDirection,uiSelected_Position;
extern uint8_t ckey_digit_data[6];
extern uint8_t cdigits_data[7];
extern uint8_t uiPSER_Option_Avbl,uilast_selected_option; //
extern uint8_t uimaxspeed_temp,uichild_flash_count,prev_program_pos;
extern uint8_t uioption_on,uiopt_check,uiSoak_Press_count;
extern uint8_t cdigits_data[7],uiTestModeState,uiBefore_Snooze;
extern uint8_t uiTempSel,rpm_Selection,uiDISP_MCount;
extern uint8_t uiExtraRinse_Press_count,uiExtraRinse_Max;
extern uint8_t uiRpmSel,uidisp_select_option,uiIDLE_COUNT;
extern uint8_t uiExtraRinse_Press_Select,uiRinseholdState,uiTumblingCount;
extern uint8_t uiExtraRinse_Press_count,uiExtraRinse_Max;

extern uint16_t uidelaymin,mybuzzerdelay;
extern uint16_t uicycle_mincompleted,snoozemode_10minutes_counter;
extern uint16_t uiCurrent_step_time,uidisplay_flash_count;

extern uint8_t uiLS_Tx_subtraction, uiLS_Total_subtraction,uiLoadRange;
extern void After_LS_Update_Para();
extern void ToneGeneration();
//**************************** Local variables ***********************//

boolean uiReset_TempRPM,bHotRinse_Selected,bCold_Soak_Selected;
boolean bPrev_RPM_ZERO,bSpin_Available,uiUB_decidation;

uint8_t uiRMP_Default,cFlash_prog_led,uiSmartProgram_Selected;
uint8_t uidefault_kg=0,PREWASH_TIME;
uint8_t spin_first_press,temp_first_press;
uint8_t EXTRARINSE1_TIME;
uint8_t EXTRARINSE2_TIME;
uint8_t EXTRARINSE3_TIME;
uint8_t uiTIMESAVE_ON_TIME;
uint8_t uiTIMESAVE_OFF_TIME;
uint8_t ANTICREASE_TIME;
uint8_t FINALSPIN_TIME;
uint16_t BalanceTime;
#if ENABLE_IOT
uint16_t ProgramTime,ProgressTime;
extern uint16_t uikeypress_20mscount;
#include "iot_config.h"
#endif

/*******************************************************************************************************
* Function Name: ProgramTimming
* Description  : This function implements to show execution time for different programs.
* Arguments    : None
* Return Value : None
********************************************************************************************************/
void ProgramTimming()
{
	if(TubClean_Activated == FALSE)
	{
		switch(GET_PROGRAM)
		{		
			case MIXEDSOIL : 
					switch(uiTempSel)
					{
						case WashCold:
						case P_COLD:
							uiTIMESAVE_OFF_TIME 	= MIXEDCLD_TIME;
							uiTIMESAVE_ON_TIME 	= TIME_SAVE_MIXEDCLD_TIME;
							break;
						case Wash60:
						case P_60:
							uiTIMESAVE_OFF_TIME  	= MIXED60_TIME;
							uiTIMESAVE_ON_TIME  	= TIME_SAVE_MIXED60_TIME;	
							break;	
						case Wash30:
						case P_30:
							uiTIMESAVE_OFF_TIME  	= MIXED30_TIME;
							uiTIMESAVE_ON_TIME  	= TIME_SAVE_MIXED30_TIME;
							break;
						case Wash40:
						case P_40:
							uiTIMESAVE_OFF_TIME 	= MIXED40_TIME;
							uiTIMESAVE_ON_TIME  	= TIME_SAVE_MIEXD40_TIME;
							break;
						default:
							break;
					}
					PREWASH_TIME 		= MIXED_PREWASH_TIME;
					EXTRARINSE1_TIME 	= MIXED_ER_TIME;
					EXTRARINSE2_TIME 	= MIXED_ER_TIME;
					EXTRARINSE3_TIME 	= MIXED_ER_TIME;
					ANTICREASE_TIME 	= MIXED_ANTICREEASE_TIME;
					FINALSPIN_TIME 		= MIXED_FINALSPIN_TIME;	
					break;
						
			case COTTON : //Cotton  Normal 
			
				switch(uiTempSel)
				{
						case WashCold:
						case P_COLD:
							uiTIMESAVE_OFF_TIME = COTTONCLD_TIME;
							uiTIMESAVE_ON_TIME  = TIME_SAVE_COTTONCLD_TIME;
							break;
						case Wash30:
						case P_30:
							uiTIMESAVE_OFF_TIME = COTTON30_TIME;
							uiTIMESAVE_ON_TIME  = TIME_SAVE_COTTON30_TIME;
							break;
						case Wash40:
						case P_40:
							uiTIMESAVE_OFF_TIME = COTTON40_TIME;
							uiTIMESAVE_ON_TIME  = TIME_SAVE_COTTON40_TIME;
							break;
						case Wash60:
						case P_60:
							uiTIMESAVE_OFF_TIME = COTTON60_TIME;
							uiTIMESAVE_ON_TIME  = TIME_SAVE_COTTON60_TIME;
							break;
						case Wash95:
						case P_95:
							uiTIMESAVE_OFF_TIME = COTTON90_TIME;
							uiTIMESAVE_ON_TIME  = TIME_SAVE_COTTON90_TIME;
							break;
						//Eco program	
						case Wash40E:
							uiTIMESAVE_OFF_TIME = COTTONECO40_TIME;
							break;
						case Wash60E:
							uiTIMESAVE_OFF_TIME = COTTONECO60_TIME;
						break;
						
					
				}
				PREWASH_TIME 		= COTTON_PREWASH_TIME;
				EXTRARINSE1_TIME 	= COTTON_ER_TIME;	
				EXTRARINSE2_TIME 	= COTTON_ER_TIME;	
				EXTRARINSE3_TIME 	= COTTON_ER_TIME;
				
				if((uiTempSel==Wash40E) || (uiTempSel==Wash60E))
				{
					EXTRARINSE1_TIME = 11;//13;(4F,1D,6S)	
					EXTRARINSE2_TIME = RESET_VALUE;	
					EXTRARINSE3_TIME = RESET_VALUE;	
					ANTICREASE_TIME = 0;
					FINALSPIN_TIME = COTTON_ECO_FINALSPIN;//11;//13;
				}
				else
				{
					ANTICREASE_TIME = COTTON_ANTICREASE_TIME;	
					FINALSPIN_TIME 	= COTTON_FINALSPIN;
				}
				
				break;
			
			case BABYWEAR:
					switch(uiTempSel)
					{
						case WashCold:
						case P_COLD:
							uiTIMESAVE_OFF_TIME = BABYWEARCLD_TIME;
							uiTIMESAVE_ON_TIME  = TIME_SAVE_BABYWEARCLD_TIME;
							break;
						
						case Wash40:
						case P_40:
							uiTIMESAVE_OFF_TIME = BABYWEAR40_TIME;
							uiTIMESAVE_ON_TIME  = TIME_SAVE_BABYWEAR40_TIME;
							break;
							
						case Wash60:
						case P_60:
							uiTIMESAVE_OFF_TIME = BABYWEAR60_TIME;
							uiTIMESAVE_ON_TIME  = TIME_SAVE_BABYWEAR60_TIME;
							break;
						
						default:
							break;
					}
					PREWASH_TIME 	= BABY_PREWASH_TIME;
					EXTRARINSE1_TIME = 9;
					EXTRARINSE2_TIME = 9;
					EXTRARINSE3_TIME = RESET_VALUE;
					ANTICREASE_TIME = 3;
					FINALSPIN_TIME = 3;//4;
				break;
				
	//		case ANTIALLERGEN :
	//				switch(uiTempSel)
	//				{
	//				case Wash40:
	//					uiTIMESAVE_OFF_TIME =  ANTIALG40_TIME;
	//					uiTIMESAVE_ON_TIME  = TIME_SAVE_ANTIALG40_TIME;
	//					break;
	//				case Wash95:
	//					uiTIMESAVE_OFF_TIME = ANTIALG95_TIME;
	//					uiTIMESAVE_ON_TIME = TIME_SAVE_ANTIALG95_TIME;
	//					break;
	//				case Wash60:
	//					uiTIMESAVE_OFF_TIME=  ANTIALG60_TIME;
	//					uiTIMESAVE_ON_TIME  = TIME_SAVE_ANTIALG60_TIME;
	//					break;
	//				default:
	//					break;
	//				}
	//				EXTRARINSE1_TIME = 8;
	//				EXTRARINSE2_TIME = 8;
	//				EXTRARINSE3_TIME = RESET_VALUE;
	//				ANTICREASE_TIME = 0;
	//				FINALSPIN_TIME = 7;//6;
	//				break;	
	//		case EXPRESS_30:
	//				switch(uiTempSel)
	//				{
	//					case Wash40:
	//						if(uiRpmSel == Rpm1200)
	//						{
	//							uiTIMESAVE_OFF_TIME =  EXPRESS_40_TIME+EXPRESS_30_RPM1200_TIME;
	//						}
	//						else 
	//						{
	//							uiTIMESAVE_OFF_TIME =  EXPRESS_40_TIME;
	//						}
	//						break;
	//					case Wash60:
	//						if(uiRpmSel == Rpm1200)
	//						{
	//							uiTIMESAVE_OFF_TIME =  EXPRESS_60_TIME+EXPRESS_30_RPM1200_TIME;
	//						}
	//						else
	//						{
	//							uiTIMESAVE_OFF_TIME =  EXPRESS_60_TIME;
	//						}
	//						break;
	//					default:
	//						break;
	//				}
	//				EXTRARINSE1_TIME 	= EXPRESS_30_ER_TIME;
	//				EXTRARINSE2_TIME 	= EXPRESS_30_ER_TIME;
	//				EXTRARINSE3_TIME 	= RESET_VALUE;
	//				ANTICREASE_TIME 	= EXPRESS_30_ANTICREASE_TIME;
	//				FINALSPIN_TIME 		= EXPRESS_30_FINALSPIN;
	//				break;
	//				
			case STEAMFRESH :
					
					uiTIMESAVE_OFF_TIME = STEAMFRESH_TIME;
					EXTRARINSE1_TIME = RESET_VALUE;
					EXTRARINSE2_TIME = RESET_VALUE;
					EXTRARINSE3_TIME = RESET_VALUE;
					ANTICREASE_TIME = RESET_VALUE;
					FINALSPIN_TIME = RESET_VALUE;
					break;
					
			case ADDITIVESRINSE : 
						
							uiTIMESAVE_OFF_TIME =  RINSESPIN_TIME;
							uiTIMESAVE_ON_TIME = TIME_SAVE_RINSESPIN_TIME;
							PREWASH_TIME = RESET_VALUE;
							EXTRARINSE1_TIME = 4;
							EXTRARINSE2_TIME = 10;
							EXTRARINSE3_TIME = RESET_VALUE;
							
							ANTICREASE_TIME = 5;
							FINALSPIN_TIME = 10;//11;//14;//13;
					break;
						
			/*case DRAINSPIN : // Spin Dry/Drain 
					uiTIMESAVE_OFF_TIME =  SPINDRY_TIME;
					uiTIMESAVE_ON_TIME = TIME_SAVE_SPINDRY_TIME;
					PREWASH_TIME = RESET_VALUE;
					EXTRARINSE1_TIME = RESET_VALUE;
					EXTRARINSE2_TIME = RESET_VALUE;
					EXTRARINSE3_TIME = RESET_VALUE;
					
					ANTICREASE_TIME = 5;
					FINALSPIN_TIME = 10;
					break;*/
					
	//		case WOOLLENS: //Woollens
	//		
	//				switch(uiTempSel)
	//				{
	//					case WashCold:
	//						uiTIMESAVE_OFF_TIME =  WOOLLENSCLD_TIME;
	//						break;
	//						
	//					case Wash30:
	//						uiTIMESAVE_OFF_TIME  =  WOOLLENS30_TIME;
	//						break;	
	//						
	//					case Wash40:
	//						uiTIMESAVE_OFF_TIME =  WOOLLENS40_TIME;
	//						break;
	//						
	//					default:
	//						break;
	//				}
	//				PREWASH_TIME = RESET_VALUE;
	//				EXTRARINSE1_TIME = 7;
	//				EXTRARINSE2_TIME = RESET_VALUE;
	//				EXTRARINSE3_TIME = RESET_VALUE;
	//				ANTICREASE_TIME = 0;
	//				FINALSPIN_TIME = 6;//5;	
	//				break;			
			
//			case WETCLEAN :
//					switch(uiTempSel)
//					{
//						case WashCold:
//							uiTIMESAVE_OFF_TIME =  WETCLEANCLD_TIME;
//							break;
//						case Wash30:
//							uiTIMESAVE_OFF_TIME  =  WETCLEAN30_TIME;
//							break;
//						case Wash40:
//							uiTIMESAVE_OFF_TIME =  WETCLEAN40_TIME;	
//							break;
//						default:
//						break;
//					}
//					PREWASH_TIME = RESET_VALUE;
//					EXTRARINSE1_TIME = 8;
//					EXTRARINSE2_TIME = 8;
//					EXTRARINSE3_TIME = RESET_VALUE;
//					ANTICREASE_TIME = 0;
//					FINALSPIN_TIME = 4;//3;
//					break;
			case CRADLEWASH : // CradleWash
			
					switch(uiTempSel)
					{
						case WashCold:
							uiTIMESAVE_OFF_TIME =  CRADLEWASHCLD_TIME;
							break;
						case Wash30:
							uiTIMESAVE_OFF_TIME  =  CRADLEWASH30_TIME;
							break;
						case Wash40:
							uiTIMESAVE_OFF_TIME =  CRADLEWASH40_TIME;	
							break;
						default:
						break;
					}
					PREWASH_TIME = RESET_VALUE;
					EXTRARINSE1_TIME = 8;
					EXTRARINSE2_TIME = RESET_VALUE;
					EXTRARINSE3_TIME = RESET_VALUE;
					ANTICREASE_TIME = 0;
					FINALSPIN_TIME = 4;//3;
					break;
					
			case SYNTHETIC : //Synthetic/ Daily
					switch(uiTempSel)
					{
						case WashCold:
						case P_COLD:
							uiTIMESAVE_OFF_TIME =  SYNTHETICCLD_TIME;
							uiTIMESAVE_ON_TIME = TIME_SAVE_SYNTHETICCLD_TIME;
							break;
						case Wash30:
						case P_30:
							uiTIMESAVE_OFF_TIME  =  SYNTHETIC30_TIME;
							uiTIMESAVE_ON_TIME  = TIME_SAVE_SYNTHETIC30_TIME;
							break;
						case Wash40:
						case P_40:
							uiTIMESAVE_OFF_TIME =  SYNTHETIC40_TIME;
							uiTIMESAVE_ON_TIME  = TIME_SAVE_SYNTHETIC40_TIME;
							break;
						case Wash60:
						case P_60:
							uiTIMESAVE_OFF_TIME =  SYNTHETIC60_TIME;
							uiTIMESAVE_ON_TIME  = TIME_SAVE_SYNTHETIC60_TIME;
							break;
						default:
							break;
					}
					PREWASH_TIME 		= SYNTH_PREWASH_TIME;
					EXTRARINSE1_TIME 	= SYNTH_ER_TIME;
					EXTRARINSE2_TIME 	= SYNTH_ER_TIME;
					EXTRARINSE3_TIME 	= SYNTH_ER_TIME;
					ANTICREASE_TIME 	= SYNTH_ANTICREAS_TIME;
					FINALSPIN_TIME 		= SYNTH_FINALSPIN_TIME;
					break;
					
			case EXPRESS :  
//					if(bExpress_30_Activatted == TRUE)
//					{
//						uiTIMESAVE_OFF_TIME = STEAMWASH_TIME;
//						EXTRARINSE1_TIME = 7;
//						EXTRARINSE2_TIME = RESET_VALUE;
//						EXTRARINSE3_TIME = RESET_VALUE;
//						ANTICREASE_TIME = RESET_VALUE;
//						FINALSPIN_TIME = 4;//3;
//						break;	
//					}
//					else
//					{
						switch(uiTempSel)
						{
							
							case WashCold:
								if(uiRpmSel == Rpm1200)
								{
									uiTIMESAVE_OFF_TIME =  EXPRESSCLD_TIME + 5;//4;
								}
								else if(uiRpmSel == Rpm1000)
								{
									uiTIMESAVE_OFF_TIME =  EXPRESSCLD_TIME + 3;//2;
								}
								else if(uiRpmSel == Rpm000)
								{
									uiTIMESAVE_OFF_TIME =  EXPRESSCLD_TIME + 1;
								}	
								else
								{
									uiTIMESAVE_OFF_TIME =  EXPRESSCLD_TIME;
								}
								break;
								
							case Wash30:
								if(uiRpmSel == Rpm1200)
								{
									uiTIMESAVE_OFF_TIME =  EXPRESS30_TIME + 5;//4;
								}
								else if(uiRpmSel == Rpm1000)
								{
									uiTIMESAVE_OFF_TIME =  EXPRESS30_TIME + 3;//2;
								}
								else if(uiRpmSel == Rpm000)
								{
									uiTIMESAVE_OFF_TIME =  EXPRESS30_TIME + 1;
								}
								else
								{
									uiTIMESAVE_OFF_TIME =  EXPRESS30_TIME;
								}
								break;
								
							case Wash40:
								if(uiRpmSel == Rpm1200)
								{
									uiTIMESAVE_OFF_TIME =  EXPRESS40_TIME + 5;//4;
								}
								else if(uiRpmSel == Rpm1000)
								{
									uiTIMESAVE_OFF_TIME =  EXPRESS40_TIME + 3;//2;
								}
								else if(uiRpmSel == Rpm000)
								{
									uiTIMESAVE_OFF_TIME =  EXPRESS40_TIME + 1;
								}
								else
								{
									uiTIMESAVE_OFF_TIME =  EXPRESS40_TIME;
								}
								break;					
							default:
								break;
						}
						PREWASH_TIME = RESET_VALUE;
						EXTRARINSE1_TIME = 3;
						EXTRARINSE2_TIME = RESET_VALUE;
						EXTRARINSE3_TIME = RESET_VALUE;
						ANTICREASE_TIME = 0;
						if(uiRpmSel == Rpm1200)
						{
							FINALSPIN_TIME = 8;//7;
						}
						else if(uiRpmSel == Rpm1000)
						{
							FINALSPIN_TIME = 6;//5;
						}
						else
						{
							FINALSPIN_TIME = 3;
						}
//					}
					break;
			case CURTAINS :
					switch(uiTempSel)
					{
					case WashCold:
					case P_COLD:
						uiTIMESAVE_OFF_TIME =  CURTAINCLD_TIME;
						uiTIMESAVE_ON_TIME = TIME_SAVE_CURTAINCLD_TIME;
						break;

					case Wash40:
					case P_40:
						uiTIMESAVE_OFF_TIME  =  CURTAIN40_TIME;
						uiTIMESAVE_ON_TIME  = TIME_SAVE_CURTAIN40_TIME;
						break;
					case Wash60:
					case P_60:
						uiTIMESAVE_OFF_TIME =  CURTAIN60_TIME;
						uiTIMESAVE_ON_TIME  = TIME_SAVE_CURTAIN60_TIME;
						break;

					default:
						break;
					}
					PREWASH_TIME = 18;
					EXTRARINSE1_TIME = 6;
					EXTRARINSE2_TIME = 6;
					EXTRARINSE3_TIME = RESET_VALUE;
					ANTICREASE_TIME = 5;
					FINALSPIN_TIME = 4;//3;
					break;
				
			case SMARTPLUS:

				switch(uiSmartProgram_Selected)
				{
				case STEAMWASH :
						uiTIMESAVE_OFF_TIME = STEAMWASH_TIME;
						EXTRARINSE1_TIME = 7;
						EXTRARINSE2_TIME = RESET_VALUE;
						EXTRARINSE3_TIME = RESET_VALUE;
						ANTICREASE_TIME = RESET_VALUE;
						FINALSPIN_TIME = 4;//3;
						break;	

				case INNERWEAR :
						switch(uiTempSel)
						{
						case Wash40:
							uiTIMESAVE_OFF_TIME = INNERWEAR_TIME;
							uiTIMESAVE_ON_TIME  = TIME_SAVE_INNERWEAR_TIME;
							break;
						default:
							break;
						}
						EXTRARINSE1_TIME = 7;
						EXTRARINSE2_TIME = RESET_VALUE;
						EXTRARINSE3_TIME = RESET_VALUE;
						ANTICREASE_TIME = 2;
						FINALSPIN_TIME = 4;//3;
						break;
//				case CURTAINS :
//						switch(uiTempSel)
//						{
//						case WashCold:
//						case P_COLD:
//							uiTIMESAVE_OFF_TIME =  CURTAINCLD_TIME;
//							uiTIMESAVE_ON_TIME = TIME_SAVE_CURTAINCLD_TIME;
//							break;
//
//						case Wash40:
//						case P_40:
//							uiTIMESAVE_OFF_TIME  =  CURTAIN40_TIME;
//							uiTIMESAVE_ON_TIME  = TIME_SAVE_CURTAIN40_TIME;
//							break;
//						case Wash60:
//						case P_60:
//							uiTIMESAVE_OFF_TIME =  CURTAIN60_TIME;
//							uiTIMESAVE_ON_TIME  = TIME_SAVE_CURTAIN60_TIME;
//							break;
//
//						default:
//							break;
//						}
//						PREWASH_TIME = 18;
//						EXTRARINSE1_TIME = 6;
//						EXTRARINSE2_TIME = 6;
//						EXTRARINSE3_TIME = RESET_VALUE;
//						ANTICREASE_TIME = 5;
//						FINALSPIN_TIME = 4;//3;
//						break;
//						
				case DARKWASH :
						switch(uiTempSel)
						{
						case Wash40:
						case P_40:
							uiTIMESAVE_OFF_TIME =  DARKWASH40_TIME;
							uiTIMESAVE_ON_TIME  = TIME_SAVE_DARKWASH40_TIME;
							break;
						default:
							break;
						}
						PREWASH_TIME = 13;
						EXTRARINSE1_TIME = 8;
						EXTRARINSE2_TIME = 8;
						EXTRARINSE3_TIME = RESET_VALUE;
						ANTICREASE_TIME = 2;
						FINALSPIN_TIME = 7;//6;
						break;
						
				case TUBCLEAN:
						switch(uiTempSel)
						{
						case WashCold:
							uiTIMESAVE_OFF_TIME =  TUBCLEANCLD_TIME;
							break;
						case Wash60:
							uiTIMESAVE_OFF_TIME  =  TUBCLEAN60_TIME;
							break;
						case Wash95:
							uiTIMESAVE_OFF_TIME =  TUBCLEAN90_TIME;
							break;
						default:
							break;
						}
						EXTRARINSE1_TIME = RESET_VALUE;
						EXTRARINSE2_TIME = RESET_VALUE;
						EXTRARINSE3_TIME = RESET_VALUE;
						ANTICREASE_TIME = RESET_VALUE;
						FINALSPIN_TIME = RESET_VALUE;
						break;
				case SPORTWEAR:
						switch(uiTempSel)
						{
							case WashCold:
							case P_COLD:
								uiTIMESAVE_OFF_TIME =  SPORTWAERCLD_TIME;
								uiTIMESAVE_ON_TIME = TIME_SAVE_SPORTWAERCLD_TIME;
								break;
								
							case Wash40:
							case P_40:
								uiTIMESAVE_OFF_TIME  =  SPORTWAER40_TIME;
								uiTIMESAVE_ON_TIME  = TIME_SAVE_SPORTWAER40_TIME;
								break;	
								
							default:
								break;
						}
						PREWASH_TIME = 17;
						EXTRARINSE1_TIME = 8;
						EXTRARINSE2_TIME = 8;
						EXTRARINSE3_TIME = RESET_VALUE;
						ANTICREASE_TIME = 2;
						FINALSPIN_TIME = 4;//3;
						break;	
				case UNIFORM :
						switch(uiTempSel)
						{
						case WashCold:
						case P_COLD:
							uiTIMESAVE_OFF_TIME = UNIFORMCLD_TIME;
							uiTIMESAVE_ON_TIME  = TIME_SAVE_UNIFORMCLD_TIME;
							break;
						case Wash40:
						case P_40:
							uiTIMESAVE_OFF_TIME = UNIFORM40_TIME;
							uiTIMESAVE_ON_TIME  = TIME_SAVE_UNIFORM40_TIME;
							break;
						case Wash60:
						case P_60:
							uiTIMESAVE_OFF_TIME = UNIFORM60_TIME;
							uiTIMESAVE_ON_TIME  = TIME_SAVE_UNIFORM60_TIME;
							break;
						}
						PREWASH_TIME = 16;
						EXTRARINSE1_TIME = 8;
						EXTRARINSE2_TIME = 8;
						EXTRARINSE3_TIME = RESET_VALUE;
						ANTICREASE_TIME = 3;
						FINALSPIN_TIME = 7;//6;
						break;
						
				case JEANS :
						uiTIMESAVE_OFF_TIME = JEANS60_TIME;
						uiTIMESAVE_ON_TIME  = TIME_SAVE_JEANS60_TIME;
			
						PREWASH_TIME = 18;
						EXTRARINSE1_TIME = 11;
						EXTRARINSE2_TIME = RESET_VALUE;
						EXTRARINSE3_TIME = RESET_VALUE;
						ANTICREASE_TIME = 5;
						FINALSPIN_TIME = 10;//11;//14;
						break;
						
				case SHIRTS :
						switch(uiTempSel)
						{
						case WashCold:
						case P_COLD:
							uiTIMESAVE_OFF_TIME = SHIRTCLD_TIME;
							uiTIMESAVE_ON_TIME = TIME_SAVE_SHIRTCLD_TIME;
							break;
						case Wash30:
						case P_30:
							uiTIMESAVE_OFF_TIME = SHIRT30_TIME;
							uiTIMESAVE_ON_TIME = TIME_SAVE_SHIRT30_TIME;
							break;
						case Wash40:
						case P_40:
							uiTIMESAVE_OFF_TIME = SHIRT40_TIME;
							uiTIMESAVE_ON_TIME = TIME_SAVE_SHIRT40_TIME;
							break;
						case Wash60:
						case P_60:
							uiTIMESAVE_OFF_TIME = SHIRT60_TIME;
							uiTIMESAVE_ON_TIME = TIME_SAVE_SHIRT60_TIME;
							break;
						}
						PREWASH_TIME = 12;
						EXTRARINSE1_TIME = 13;
						EXTRARINSE2_TIME = 13;
						EXTRARINSE3_TIME = RESET_VALUE;
						ANTICREASE_TIME = 2;
						FINALSPIN_TIME = 6;//5;
						break;
				case LINENWASH :
						switch(uiTempSel)
						{
							case WashCold:
								uiTIMESAVE_OFF_TIME =  LINENWASHCLD_TIME;
								break;
							case Wash30:
								uiTIMESAVE_OFF_TIME  =  LINENWASH30_TIME;
								break;
							case Wash40:
								uiTIMESAVE_OFF_TIME =  LINENWASH40_TIME;	
								break;
							default:
							break;
						}
						PREWASH_TIME = RESET_VALUE;
						EXTRARINSE1_TIME = 8;
						EXTRARINSE2_TIME = RESET_VALUE;
						EXTRARINSE3_TIME = RESET_VALUE;
						ANTICREASE_TIME = 0;
						FINALSPIN_TIME = 3;//4;
						break;
				case WOOLLENS: //Woollens
			
					switch(uiTempSel)
					{
						case WashCold:
							uiTIMESAVE_OFF_TIME =  WOOLLENSCLD_TIME;
							break;
							
						case Wash30:
							uiTIMESAVE_OFF_TIME  =  WOOLLENS30_TIME;
							break;	
							
						case Wash40:
							uiTIMESAVE_OFF_TIME =  WOOLLENS40_TIME;
							break;
							
						default:
							break;
					}
					PREWASH_TIME = RESET_VALUE;
					EXTRARINSE1_TIME = 7;
					EXTRARINSE2_TIME = RESET_VALUE;
					EXTRARINSE3_TIME = RESET_VALUE;
					ANTICREASE_TIME = 0;
					FINALSPIN_TIME = 6;//5;	
					break;
		
				case ANTIALLERGEN :
					switch(uiTempSel)
					{
					case Wash40:
					case P_40:
						uiTIMESAVE_OFF_TIME =  ANTIALG40_TIME;
						uiTIMESAVE_ON_TIME  = TIME_SAVE_ANTIALG40_TIME;
						break;
					case Wash95:
					case P_95:
						uiTIMESAVE_OFF_TIME = ANTIALG95_TIME;
						uiTIMESAVE_ON_TIME = TIME_SAVE_ANTIALG95_TIME;
						break;
					case Wash60:
					case P_60:
						uiTIMESAVE_OFF_TIME=  ANTIALG60_TIME;
						uiTIMESAVE_ON_TIME  = TIME_SAVE_ANTIALG60_TIME;
						break;
					default:
						break;
					}
					PREWASH_TIME = ANTI_PREWASH_TIME;
					EXTRARINSE1_TIME = 8;
					EXTRARINSE2_TIME = 8;
					EXTRARINSE3_TIME = RESET_VALUE;
					ANTICREASE_TIME = 0;
					FINALSPIN_TIME = 7;//6;
					break;	
			case EXPRESS_30:
					switch(uiTempSel)
					{
						case Wash40:
							if(uiRpmSel == Rpm1200)
							{
								uiTIMESAVE_OFF_TIME =  EXPRESS_40_TIME+EXPRESS_30_RPM1200_TIME;
							}
							else 
							{
								uiTIMESAVE_OFF_TIME =  EXPRESS_40_TIME;
							}
							break;
						case Wash60:
							if(uiRpmSel == Rpm1200)
							{
								uiTIMESAVE_OFF_TIME =  EXPRESS_60_TIME+EXPRESS_30_RPM1200_TIME;
							}
							else
							{
								uiTIMESAVE_OFF_TIME =  EXPRESS_60_TIME;
							}
							break;
						default:
							break;
					}
					EXTRARINSE1_TIME 	= EXPRESS_30_ER_TIME;
					EXTRARINSE2_TIME 	= EXPRESS_30_ER_TIME;
					EXTRARINSE3_TIME 	= RESET_VALUE;
					ANTICREASE_TIME 	= EXPRESS_30_ANTICREASE_TIME;
					FINALSPIN_TIME 		= EXPRESS_30_FINALSPIN;
					break;
				default :
					break;	
				}
				break;
			default : 
			 	break;
		}
	}
	else
	{
		switch(uiTempSel)
		{
			case WashCold:
									
				uiTIMESAVE_OFF_TIME =  TUBCLEANCLD_TIME;						
			break;
										
			case Wash60:
										
				uiTIMESAVE_OFF_TIME  =  TUBCLEAN60_TIME;
			break;
										
			case Wash95:
									
				uiTIMESAVE_OFF_TIME =  TUBCLEAN90_TIME;
			break;
										
			default:
			break;
		}
								
			ANTICREASE_TIME = RESET_VALUE;
			FINALSPIN_TIME = RESET_VALUE;
	}
	
	if((uiRinseholdState == 0)||((uiRinseholdState == 2)&&(uiRpmSel == Rpm000))) //?
	{
		if(uiMachineState == PAUSE_PRESSED)
		{
			
			if( uidisp_select_option & TimeSaver_Yes)
			{
				BalanceTime =  uiTIMESAVE_ON_TIME - uicycle_mincompleted;
			}
			else
			{
				BalanceTime =  uiTIMESAVE_OFF_TIME - uicycle_mincompleted;
			}
		
			if((uidisp_select_option & Prewash_Yes)==Prewash_Yes)
			{
				BalanceTime = BalanceTime + PREWASH_TIME; 
			}
			/* Added As per Spec EL-PROG-286 Rev31  - Warm Soak timming */
			switch(GET_PROGRAM)
			{
				case COTTON:
				case MIXEDSOIL:
				case SYNTHETIC:
						if(((uidisp_select_option & WarmSoak_Yes)== WarmSoak_Yes) && (uiTempSel != WashCold && uiTempSel != P_COLD))
						{
							BalanceTime = BalanceTime + EIGHTEEN_MINUTES;
						}
						else if((bCold_Soak_Selected == TRUE) && ((uiTempSel == WashCold) ||(uiTempSel == P_COLD)))
						{
							BalanceTime = BalanceTime + EIGHT_MINUTES;
						}
						else
						{
							;
						}
					break;
				//case ANTIALLERGEN:
				case BABYWEAR:
						if(((uidisp_select_option & WarmSoak_Yes)== WarmSoak_Yes) && ((uiTempSel != WashCold) &&(uiTempSel != P_COLD)))
						{
							BalanceTime = BalanceTime + SIXTEEN_MINUTES;
						}
						else if((bCold_Soak_Selected == TRUE) && ((uiTempSel == WashCold) || (uiTempSel == P_COLD)))
						{
							BalanceTime = BalanceTime + SIX_MINUTES;
						}
						else
						{
							;
						}
					break;
				case CURTAINS:	
						if(((uidisp_select_option & WarmSoak_Yes)== WarmSoak_Yes) && ((uiTempSel != WashCold) && (uiTempSel != P_COLD)))//&& (uiTempSel > WashCold))
						{
							BalanceTime = BalanceTime + FIFTEEN_MINUTES;
						}
						else if((bCold_Soak_Selected == TRUE) && ((uiTempSel == WashCold) || (uiTempSel == P_COLD)))//&& (uiTempSel == WashCold))
						{
							BalanceTime = BalanceTime + FIVE_MINUTES;
						}
						else
						{
							;
						}
					break;
				case SMARTPLUS:
					switch(uiSmartProgram_Selected)
					{
						//case CURTAINS:
						case SPORTWEAR:
						case SHIRTS:
								if(((uidisp_select_option & WarmSoak_Yes)== WarmSoak_Yes) && ((uiTempSel != WashCold) && (uiTempSel != P_COLD)))//&& (uiTempSel > WashCold))
								{
									BalanceTime = BalanceTime + FIFTEEN_MINUTES;
								}
								else if((bCold_Soak_Selected == TRUE) && ((uiTempSel == WashCold) || (uiTempSel == P_COLD)))//&& (uiTempSel == WashCold))
								{
									BalanceTime = BalanceTime + FIVE_MINUTES;
								}
								else
								{
									;
								}
							break;
						case JEANS:
						case UNIFORM:
								if(((uidisp_select_option & WarmSoak_Yes)== WarmSoak_Yes) && ((uiTempSel != WashCold) && (uiTempSel != P_COLD)))//(uiTempSel > WashCold))
								{
									BalanceTime = BalanceTime + SIXTEEN_MINUTES;
								}
								else if((bCold_Soak_Selected == TRUE) && ((uiTempSel == WashCold) || (uiTempSel == P_COLD)))//(uiTempSel == WashCold))
								{
									BalanceTime = BalanceTime + SIX_MINUTES;
								}
								else
								{
									;
								}
							break;
						case ANTIALLERGEN:
						//case BABYWEAR:
								if(((uidisp_select_option & WarmSoak_Yes)== WarmSoak_Yes) && ((uiTempSel != WashCold) &&(uiTempSel != P_COLD)))
								{
									BalanceTime = BalanceTime + SIXTEEN_MINUTES;
								}
								else if((bCold_Soak_Selected == TRUE) && ((uiTempSel == WashCold) || (uiTempSel == P_COLD)))
								{
									BalanceTime = BalanceTime + SIX_MINUTES;
								}
								else
								{
									;
								}
							break;
						default: break;
					}
					break;
				default:
					break;
				
						
			}
			
			if((uidisp_select_option & ExtraRinse_Yes)== ExtraRinse_Yes)
			{
				switch(uiExtraRinse_Press_Select)
				{
					case 0x01:
						BalanceTime = BalanceTime + EXTRARINSE1_TIME;
					break;
					case 0x02:
						BalanceTime = BalanceTime + EXTRARINSE1_TIME;
						BalanceTime = BalanceTime + EXTRARINSE2_TIME;
					break;
					case 0x03:
						BalanceTime = BalanceTime + EXTRARINSE1_TIME;
						BalanceTime = BalanceTime + EXTRARINSE2_TIME;
						BalanceTime = BalanceTime + EXTRARINSE3_TIME;
					break;
				}
					
			}
			
			if(bHotRinse_Selected == TRUE)
			{
				 BalanceTime = BalanceTime + HOTRINSE_TIME; 
			}
			if(((uidisp_select_option & Steam_Yes)== Steam_Yes)&&((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == LINENWASH)))
			{
				BalanceTime = BalanceTime + STEAM_LINEN;
			}
			else if(((uidisp_select_option & Steam_Yes)== Steam_Yes)&&(/*(uiSelected_Position == SMARTPLUS)&&*/(uiSmartProgram_Selected != STEAMWASH /*&& bExpress_30_Activatted == FALSE*/)))
			{
				 BalanceTime = BalanceTime + STEAMFRESH_TIME; 
			}
			if(uiRpmSel == Rpm000)
			{
				 if(bRapidWashButton == TRUE)
				{
					 BalanceTime = BalanceTime - (FINALSPIN_TIME);
					 if((BalanceTime == 0)||(BalanceTime > 350))  //safety condition when canceling spin at rinse hold
					 {
						BalanceTime =  1;
					 }
				}
				else
				{
					if(((uidisp_select_option & Steam_Yes)== Steam_Yes) 
						&& (uiTumblingCount != STEAM_FILL 
							&& uiTumblingCount != STEAM 
							&& uiTumblingCount != STEAM_CREASE 
							&& uiTumblingCount != STEAM_DRAIN)) // TODO: If steam phases are executing so at that time time should not be deduct.
					{
						BalanceTime = BalanceTime - (ANTICREASE_TIME + FINALSPIN_TIME + STEAMFRESH_TIME); 
					}
					else
					{
						if((((GET_PROGRAM) == MIXEDSOIL)&&(uiLoadRange<=1))||((((GET_PROGRAM) == COTTON)||((GET_PROGRAM) == SYNTHETIC))&&(uiLoadRange==0))) //to subtract anticrease time based on load if rpm0 is selected in between program execution or at rinsehold
						{
							BalanceTime = BalanceTime - FINALSPIN_TIME;
						}
						else
						{
							BalanceTime = BalanceTime -(ANTICREASE_TIME + FINALSPIN_TIME);
						}
						
//						if((uiFinalWt>=1)&&(uiFinalWt<=3)) //to subtract anticrease time based on load if rpm0 is selected in between program execution or at rinsehold
//						{
//							BalanceTime = BalanceTime - FINALSPIN_TIME;
//						}
//						else
//						{
//							BalanceTime = BalanceTime -(ANTICREASE_TIME + FINALSPIN_TIME);
//						}
						//BalanceTime = BalanceTime - (ANTICREASE_TIME + FINALSPIN_TIME);
						if((BalanceTime == 0)||(BalanceTime > 350))
						{
							if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == LINENWASH))
							{
								BalanceTime =  2;
							}
							else
							{
								BalanceTime =  1;
							}
						}
					}
					
				}
					
				 if(uiRinseholdState == 2)
				 	bPrev_RPM_ZERO = TRUE;
   				 
			}
		}
		else 		
		{
			if( uidisp_select_option & TimeSaver_Yes)
			{
				BalanceTime =  uiTIMESAVE_ON_TIME;
			}
			else
			{
				BalanceTime =  uiTIMESAVE_OFF_TIME;
			}
		
			if((uidisp_select_option & Prewash_Yes)==Prewash_Yes)
			{
				BalanceTime = BalanceTime + PREWASH_TIME; 
			}
			
			/* Added As per Spec EL-PROG-286 Rev31  - Soak timming */
			switch(GET_PROGRAM)
			{
				case COTTON:
				case SYNTHETIC:
				case MIXEDSOIL:
						if(((uidisp_select_option & WarmSoak_Yes)== WarmSoak_Yes) && /*(uiTempSel > WashCold)*/((uiTempSel != WashCold) && (uiTempSel != P_COLD)))
						{
							BalanceTime = BalanceTime + EIGHTEEN_MINUTES;
						}
						else if((bCold_Soak_Selected == TRUE) && (uiTempSel == WashCold || uiTempSel == P_COLD))
						{
							BalanceTime = BalanceTime + EIGHT_MINUTES;
						}
						else
						{
							;
						}
					break;
				//case ANTIALLERGEN:
				case BABYWEAR:
						if(((uidisp_select_option & WarmSoak_Yes)== WarmSoak_Yes) && ((uiTempSel != WashCold) &&(uiTempSel != P_COLD)))
						{
							BalanceTime = BalanceTime + SIXTEEN_MINUTES;
						}
						else if((bCold_Soak_Selected == TRUE) && ((uiTempSel == WashCold) || (uiTempSel == P_COLD)))
						{
							BalanceTime = BalanceTime + SIX_MINUTES;
						}
						else
						{
							;
						}
					break;
				case CURTAINS:
						if(((uidisp_select_option & WarmSoak_Yes)== WarmSoak_Yes) && ((uiTempSel != WashCold) && (uiTempSel != P_COLD)))//&& (uiTempSel > WashCold))
						{
							BalanceTime = BalanceTime + FIFTEEN_MINUTES;
						}
						else if((bCold_Soak_Selected == TRUE) && ((uiTempSel == WashCold) || (uiTempSel == P_COLD)))//&& (uiTempSel == WashCold))
						{
							BalanceTime = BalanceTime + FIVE_MINUTES;
						}
						else
						{
							;
						}
					break;
				case SMARTPLUS:
					switch(uiSmartProgram_Selected)
					{
						//case CURTAINS:
						case SPORTWEAR:
						case SHIRTS:
								if(((uidisp_select_option & WarmSoak_Yes)== WarmSoak_Yes) && ((uiTempSel != WashCold) && (uiTempSel != P_COLD)))//&& (uiTempSel > WashCold))
								{
									BalanceTime = BalanceTime + FIFTEEN_MINUTES;
								}
								else if((bCold_Soak_Selected == TRUE) && ((uiTempSel == WashCold) || (uiTempSel == P_COLD)))//&& (uiTempSel == WashCold))
								{
									BalanceTime = BalanceTime + FIVE_MINUTES;
								}
								else
								{
									;
								}
							break;
						case JEANS:
						case UNIFORM:
								if(((uidisp_select_option & WarmSoak_Yes)== WarmSoak_Yes) && ((uiTempSel != WashCold) && (uiTempSel != P_COLD)))//&& (uiTempSel > WashCold))
								{
									BalanceTime = BalanceTime + SIXTEEN_MINUTES;
								}
								else if((bCold_Soak_Selected == TRUE)  && ((uiTempSel == WashCold) || (uiTempSel == P_COLD)))//&& (uiTempSel == WashCold))
								{
									BalanceTime = BalanceTime + SIX_MINUTES;
								}
								else
								{
									;
								}
							break;
						case ANTIALLERGEN:
								if(((uidisp_select_option & WarmSoak_Yes)== WarmSoak_Yes) && ((uiTempSel != WashCold) &&(uiTempSel != P_COLD)))
								{
									BalanceTime = BalanceTime + SIXTEEN_MINUTES;
								}
								else if((bCold_Soak_Selected == TRUE) && (uiTempSel == WashCold || uiTempSel == P_COLD))
								{
									BalanceTime = BalanceTime + SIX_MINUTES;
								}
								else
								{
									;
								}
							break;
						default: break;
					}
					break;
				default:
					break;
				
						
			}
			if((uidisp_select_option & ExtraRinse_Yes)== ExtraRinse_Yes)
			{
				
				switch(uiExtraRinse_Press_Select)
				{
					case 0x01:
						BalanceTime = BalanceTime + EXTRARINSE1_TIME;
					break;
					case 0x02:
						BalanceTime = BalanceTime + EXTRARINSE1_TIME;
						BalanceTime = BalanceTime + EXTRARINSE2_TIME;
					break;
					case 0x03:
						BalanceTime = BalanceTime + EXTRARINSE1_TIME;
						BalanceTime = BalanceTime + EXTRARINSE2_TIME;
						BalanceTime = BalanceTime + EXTRARINSE3_TIME;
					break;
				}
			}
			
			if(bHotRinse_Selected == TRUE)
			{
				 BalanceTime = BalanceTime + HOTRINSE_TIME; 
			}
			if(((uidisp_select_option & Steam_Yes)== Steam_Yes)&&((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == LINENWASH)))
			{
				BalanceTime = BalanceTime + STEAM_LINEN;
			}
			else if(((uidisp_select_option & Steam_Yes)== Steam_Yes)&&(/*(uiSelected_Position == SMARTPLUS)&&*/(uiSmartProgram_Selected != STEAMWASH /*&& bExpress_30_Activatted == FALSE*/)))
			{
				BalanceTime = BalanceTime + STEAMFRESH_TIME; 
			}
			if(uiRpmSel == Rpm000)
			{
				 if(bRapidWashButton == TRUE)
				{
					 BalanceTime = BalanceTime - (FINALSPIN_TIME);
					 if((BalanceTime == 0)||(BalanceTime > 350))
					 {
						BalanceTime =  1;
					 }
				}
				else
				{
					if(((uidisp_select_option & Steam_Yes)== Steam_Yes) &&(/*(uiSelected_Position == SMARTPLUS)&&*/(uiSmartProgram_Selected != STEAMWASH /*&& bExpress_30_Activatted == FALSE*/)))// TODO: Chnages taken from prathmesh for resolving steam time display issue. - 23/11/21
					{
						BalanceTime = BalanceTime - (ANTICREASE_TIME + FINALSPIN_TIME + STEAMFRESH_TIME); 
					}
					else
					{
						BalanceTime = BalanceTime - (ANTICREASE_TIME + FINALSPIN_TIME);
						if((BalanceTime == 0)||(BalanceTime > 350)) //safety condition when canceling spin at rinse hold
						{
							if((uiSelected_Position == SMARTPLUS)&&(uiSmartProgram_Selected == LINENWASH))
							{
								BalanceTime =  2;
							}
							else
							{
								BalanceTime =  1;
							}
						}
					}
				}
			}
			
			uicycle_mincompleted = RESET_VALUE;
			uiCurrent_step_time =  RESET_VALUE;
		}
	}
	else
	{
		if((uiRinseholdState == 2)&&(bPrev_RPM_ZERO == TRUE))
		{
			
			if(bRapidWashButton == TRUE)
			{
				BalanceTime = BalanceTime  + FINALSPIN_TIME;
				
			}
			else
			{
				if((uidisp_select_option & Steam_Yes)== Steam_Yes)
				{
					BalanceTime = BalanceTime + ANTICREASE_TIME + FINALSPIN_TIME + STEAMFRESH_TIME; 
				}
				else
				{
					//if((uiFinalWt>=1)&&(uiFinalWt<=3)) //to add spin and anticrease time depending on load when higher rpm is selected at rinse hold 
					if((((GET_PROGRAM) == MIXEDSOIL)&&(uiLoadRange<=1))||((((GET_PROGRAM) == COTTON)||((GET_PROGRAM) == SYNTHETIC))&&(uiLoadRange==0)))
					{
						BalanceTime = BalanceTime + FINALSPIN_TIME;
					}
					else
					{
						BalanceTime = BalanceTime + ANTICREASE_TIME + FINALSPIN_TIME;
					}
				}
			}
			bPrev_RPM_ZERO = FALSE;
		}
	}
	if((uiSelected_Position == EXPRESS)&&(uiRinseholdState == 2))
	{
		if((uiRpmSel != Rpm1200)&&(uiRpmSel != Rpm1000)&&(uiRpmSel != Rpm000))
		BalanceTime = 4;//3;
		else if(uiRpmSel == Rpm1000)
		BalanceTime = 7;//5;
		else if(uiRpmSel == Rpm1200)
		BalanceTime = 9;//7;
		else if(uiRpmSel == Rpm000)
		BalanceTime = 1;
	}
	//else if((uiSelected_Position == EXPRESS_30)&&(uiRinseholdState == 2))
	else if((uiSelected_Position == SMARTPLUS) && (uiSmartProgram_Selected == EXPRESS_30) && (uiRinseholdState == 2))
	{
		if((uidisp_select_option & Steam_Yes)== Steam_Yes)
		{
			
			//Final spin time wasn't adequate so time manipulated as 25(anti crease)+1(drain)+6(spin)+2(anti crease)
			if((uiRpmSel != Rpm1200)&&(uiRpmSel != Rpm1000)&&(uiRpmSel != Rpm000))
			BalanceTime = 9 + STEAMFRESH_TIME;//3;
			else if(uiRpmSel == Rpm1000)
			BalanceTime = 9 + STEAMFRESH_TIME;
			else if(uiRpmSel == Rpm1200)
			BalanceTime = 11 + STEAMFRESH_TIME;
			else if(uiRpmSel == Rpm000)
			BalanceTime = 1;
		}
		else
		{
			if((uiRpmSel != Rpm1200)&&(uiRpmSel != Rpm1000)&&(uiRpmSel != Rpm000))
			BalanceTime = 9;//3;
			else if(uiRpmSel == Rpm1000)
			BalanceTime = 9;
			else if(uiRpmSel == Rpm1200)
			BalanceTime = 11;
			else if(uiRpmSel == Rpm000)
			BalanceTime = 1;
			
		}
	}
#if ENABLE_IOT	
	set_IOTProgramTime(BalanceTime);	
#endif
}
//--------------------------------------------------------------------------------
void ProgramSelector(void)
{
	if(uiMachineState != POWEROFF)
	{
		uiEncoderValue = ENCODER_VALUE;
		switch(uiEncoderValue)
		{
			case 0:
				if(uiEncoderDirection == CLKWISE)
				{	
					encodercofirm = 1; //change for BLDC original 2
					bDetectionCompleted = YES;
				}
				else if(uiEncoderDirection == ACLKWISE)
				{
					encodercofirm = 2 ;//change for BLDC original 1
					bDetectionCompleted = YES;
						
				}
				break;
			case 1:
				if(bDetectionCompleted == NO)
					uiEncoderDirection = CLKWISE;
				break;
			case 4:
				if(bDetectionCompleted == NO)
					uiEncoderDirection = ACLKWISE;
				break;
			case 5: 
				uiEncoderDirection = NO_INPUT;
				bDetectionCompleted = NO;
				if(encodercofirm == 1) 
				{
					encodercofirm = 0;
						if((uiMachineState != PAUSE_PRESSED)&&(MachineErrors.EReg == 0))
						{
							
							if((bFCTMode_Selection == FALSE)&& (bChildLockButton == FALSE)) // in test mode reverse selection disable not available
							{
								if((bStartButton == NO)||((bTestMode_Selection== TRUE)&&(bDebugMode_Selection == FALSE)&&(bStartButton == NO))||((uiMachineState == PROGRAM_END)&&(bDoorLockStatus == FALSE))||(uiMachineState == SNOOZEMODE)||((bTestMode_Selection== TRUE)&&((uiTestModeState == SF01)||(uiTestModeState == SF02))))
								{
									uiProgram_Position--;
									
									if(uiProgram_Position <= 0)
									{
										if((bTestMode_Selection== TRUE)&&(uiTestModeState == SF01))
											uiProgram_Position = 1;
										else
											//uiProgram_Position = 12;
											uiProgram_Position = 10;
									}
									Reset_ProgParamter();
#if ENABLE_IOT
									reset_IOTProgramOption();
#endif
								}
							}
						}
						if((bStartButton == TRUE)||(uiMachineState == PAUSE_PRESSED))
						{
							cFlash_prog_led = 2;
							if(bChildLockButton == TRUE)
								uichild_flash_count = 2;
							
						}
					if(mybuzzerdelay == 0)
					{
						if((bProgramStart == NO)&&(bTestMode_Selection== FALSE))
						{
							uiSelectedTone = ENCODERTONE;//KEYPRESSTONE;
						}
						else if((bTestMode_Selection == TRUE)&&(bFaciaTest != TRUE)&&(bEnduranceStarted == FALSE)&&(uiProgram_Position < 12)) // else if((bTestMode_Selection == TRUE)&&(bFaciaTest != TRUE)&&(bEnduranceMode == FALSE)&&(uiMachineTestState < step2))				
						{
							uiSelectedTone = ENCODERTONE;//KEYPRESSTONE;
						}
						else if((bEnduranceStarted == TRUE)&&(bDoorLockStatus == FALSE)&&(uiSelected_Position < 12))
						{
							uiSelectedTone = ENCODERTONE;//KEYPRESSTONE;
						}
						else if((uiMachineState == PROGRAM_END)&& (bStartButton == TRUE)&&(bChildLockButton == FALSE)&&(MachineErrors.EReg == 0))
						{
							uiSelectedTone = ENCODERTONE;//KEYPRESSTONE;
						}
						else
						{
							uiSelectedTone = INAPPLICABLE;
						}
					}
					
				}
				else if(encodercofirm == 2)
				{
					encodercofirm = 0;
					
					//if(uiProgram_Position <= 12)
					if(uiProgram_Position <= 10)
					{
						if((uiMachineState != PAUSE_PRESSED)&&(bChildLockButton == FALSE)&&(MachineErrors.EReg == 0))
						{
							if((bStartButton == NO)||((bTestMode_Selection== TRUE)&&(bDebugMode_Selection == FALSE)&&(bStartButton == NO))||((uiMachineState == PROGRAM_END)&&(bDoorLockStatus == FALSE))||(uiMachineState == SNOOZEMODE)||((bTestMode_Selection== TRUE)&&((uiTestModeState == SF01)||(uiTestModeState == SF02))))
							{	uiProgram_Position++;
							
								//if(uiProgram_Position > 12)
								if(uiProgram_Position > 10)
								{
									if((bFCTMode_Selection == TRUE)||((bTestMode_Selection == TRUE)&&( bTEST_COMPLETED == FALSE)))
									{
										//uiProgram_Position = 12;
										uiProgram_Position = 10;
									}
									else
									{
										uiProgram_Position = 1;
									}
									
								}
								Reset_ProgParamter();
#if ENABLE_IOT
								reset_IOTProgramOption();

#endif
							}
							
						}
						if((bStartButton == TRUE)||(uiMachineState == PAUSE_PRESSED))
						{
							cFlash_prog_led = 2;
							if(bChildLockButton == TRUE)
								uichild_flash_count = 2;
							
						}
					}//BELOW SAFETY ADDED TO AVOID PROG POSTION >10
					else
					{
						uiProgram_Position = 1;
						Reset_ProgParamter();	//added for  to update the parameters.
					}
					if(mybuzzerdelay == 0)
					{
						if((bProgramStart == NO)&&(bTestMode_Selection== FALSE))
						{
							uiSelectedTone = ENCODERTONE;//KEYPRESSTONE;
						}
						else if((bTestMode_Selection == TRUE)&&(bFaciaTest != TRUE)&&(bEnduranceStarted == FALSE)&&(uiProgram_Position < 12))
						{
							uiSelectedTone = ENCODERTONE;//KEYPRESSTONE;
						}
						else if((bEnduranceStarted == TRUE)&&(bDoorLockStatus == FALSE)&&(uiSelected_Position < 12))
						{
							uiSelectedTone = ENCODERTONE;//KEYPRESSTONE;
						}
						else if(uiMachineState == FCTMODE)
						{
							uiSelectedTone = ENCODERTONE;//KEYPRESSTONE;
						}
						else if((uiMachineState == PROGRAM_END)&& (bStartButton == TRUE)&&(bChildLockButton == FALSE)&&(MachineErrors.EReg == 0))
						{
							uiSelectedTone = ENCODERTONE;//KEYPRESSTONE;
						}
						else
						{
							uiSelectedTone = INAPPLICABLE;
						}
					}
				}
				break;
			default:
				break;
		}
		
	}	
	
}
//--------------------------------------------------------------------------------
void ProgSel_led()
{
	ckey_digit_data[5] = 0x00;
	if(TubClean_Activated == FALSE)
	{
		switch(uiProgram_Position)
		{
			case SMARTPLUS 	:  			
					//cdigits_data[6] = 0x00;
					cdigits_data[6] = 0x01;
				break;
			case EXPRESS : 				//WOOLLENS
					cdigits_data[6] = 0x02;
				break;
//			case WETCLEAN  : 				//SYNTHETIC				
//					cdigits_data[6] = 0x04;						
//				break;
			case CURTAINS  : 				//SYNTHETIC				
					cdigits_data[6] = 0x04;						
				break;
			case CRADLEWASH : 
	//				cdigits_data[6] = 0x00;
	//				ckey_digit_data[5] = 0x01;
					cdigits_data[6] = 0x00;
					ckey_digit_data[5] = 0x02;
				break;

			case ADDITIVESRINSE  :  
	//				cdigits_data[6] = 0x00;
	//				ckey_digit_data[5] = 0x02;
					cdigits_data[6] = 0x00;
					ckey_digit_data[5] = 0x01;
				break;
			case STEAMFRESH	: 
					cdigits_data[6] = 0x08;
				break;
			case BABYWEAR 	:  			
					//cdigits_data[6] = 0x01;
					cdigits_data[6] = 0x10;
					//cdigits_data[6] = 0x08;
				break;	
			case SYNTHETIC 	: 
					cdigits_data[6] = 0x20;
				break;		
			case COTTON 	:	
					cdigits_data[6] = 0x40;
				break;
			case MIXEDSOIL 	:  
					cdigits_data[6] = 0x80;
				break;
				
			default : 
			 	break;
		}
	}
	else// tubclean true then off all program led's
	{
		cdigits_data[6] = 0x00;
		ckey_digit_data[5] = 0x00;
	}
	
}
void RESET_Prog()
{
	uimaxspeed_temp =0x00;
#if ENABLE_IOT
	if((uiSelected_Position == SMARTPLUS)&&(TubClean_Activated == TRUE)){
		uimaxspeed_temp =  Wash95 | Rpm000;
		//uiTempSel = Wash95;
		//uiRpmSel = Rpm000;
	}
#endif
	cdigits_data[5] = RESET_VALUE ;
	if(TubClean_Activated == FALSE)
	{
		switch(uiProgram_Position)
		{
			
			case EXPRESS :   //Express 15........steam long press steamwash
//					if(bExpress_30_Activatted == FALSE)
//					{
						if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
						{
							 uiPSER_Option_Avbl =  ExtraRinse_Yes|  DelayStart_Yes;// |Steam_Yes ;
							 bprewash_selected = FALSE;
							 bHotRinse_Selected = FALSE;
							 bCold_Soak_Selected = NO;
						}
						uimaxspeed_temp = Wash40 | Rpm1200 ;
						
						if(uiMachineState != PAUSE_PRESSED)
						{
							if(uiReset_TempRPM == TRUE){
							uiRMP_Default = uiRpmSel = Rpm800; 
							uiTempSel = Wash30;}
						}
						uidefault_kg = EXPRESS_DEFAULT_KG;//30;
						uiUB_decidation = 1;
						uiExtraRinse_Max = 1;
//					}
//					else
//					{
//						if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
//						{
//							uiPSER_Option_Avbl = Steam_Yes;
//							bprewash_selected = FALSE;
//							bHotRinse_Selected = FALSE;
//							bCold_Soak_Selected = NO;
//							if((uiBefore_Snooze != PROGRAM_END)&&(uiMachineState != PROGRAM_END))
//							{
//								uidisp_select_option |= Steam_Yes;
//								ckey_digit_data[3] = ckey_digit_data[3] | (0x02);
//							}
//									 
//						}
//							uimaxspeed_temp =  Rpm800;
//							if(uiMachineState != PAUSE_PRESSED)
//							{
//								if(uiReset_TempRPM == TRUE)
//									{
//										uiRMP_Default = uiRpmSel = Rpm800; 
//										uiTempSel = 0x00;
//									}
//							}
//							uidefault_kg = 30;
//							uiUB_decidation = 1;
//							uiExtraRinse_Max = 1;
//							break;	
//
//					}
				break;
			case SYNTHETIC :  
					
					if((uiMachineState != PAUSE_PRESSED) && (bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
					{
					 	uiPSER_Option_Avbl = Prewash_Yes | WarmSoak_Yes | ExtraRinse_Yes| TimeSaver_Yes | DelayStart_Yes /*|Steam_Yes*/ ;
						if((uiMachineState != PAUSE_PRESSED)&&(uiMachineState != PROGRAM_END))
						{
							if(uiReset_TempRPM == TRUE)
							{
								if(bTestMode_Selection== TRUE){
									uiRpmSel = Rpm800; 
								}else{
									uiRMP_Default = uiRpmSel = Rpm1000; }				 
								uiTempSel = Wash40;
							}
						}						
						 if((uiBefore_Snooze != PROGRAM_END)&&(uiMachineState != PROGRAM_END))
						 {
							 if((uiTempSel != WashCold) && (uiTempSel != P_COLD))
							 {
								  if(( uidisp_select_option & TimeSaver_Yes)!=TimeSaver_Yes)
								  {	
								  	//uidisp_select_option |= WarmSoak_Yes;							
									//ckey_digit_data[0] |= WarmSoak_Yes;
									uidisp_select_option &= ~WarmSoak_Yes;	//TODO: Default to selectable.
								  }
								  bCold_Soak_Selected = NO;
	#if ENABLE_IOT							 
						//set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);					
	#endif
							 }
							 else
							 {
								 uiPSER_Option_Avbl = uiPSER_Option_Avbl & (~WarmSoak_Yes);
								ckey_digit_data[0] &= ~WarmSoak_Yes;
								uidisp_select_option &= ~WarmSoak_Yes;
								//bCold_Soak_Selected = YES;
							 }						
						 }					
						bprewash_selected = FALSE;
						//bHotRinse_Selected = FALSE;
					}
					
					uimaxspeed_temp = P_60 | Rpm1200;	//BTRA
					uidefault_kg 		= SYNTH_DEFAULT_KG;
					uiUB_decidation = 0;
					uiExtraRinse_Max 	= SYNTH_MAX_ER;
					
				break;
//
//			case WETCLEAN :
//					if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
//					{
//						 uiPSER_Option_Avbl =   ExtraRinse_Yes|  DelayStart_Yes ;
//						 bprewash_selected = FALSE;
//						bHotRinse_Selected = FALSE;
//						 bCold_Soak_Selected = NO;
//					}
//					uimaxspeed_temp = (Wash40 | Rpm600);
//					if(uiMachineState != PAUSE_PRESSED)
//					{
//						if(uiReset_TempRPM == TRUE){
//						uiRMP_Default = uiRpmSel = Rpm400; 
//						uiTempSel = WashCold;}
//					}
//					uidefault_kg = WETCLEAN_DEFAULT_KG;//20;
//					 uiUB_decidation = 1; //changed
//					uiExtraRinse_Max = 2;
//					
//				break;	
				
			case CRADLEWASH : // CradleWash
					if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
					{
						 uiPSER_Option_Avbl =   ExtraRinse_Yes|  DelayStart_Yes ;
						 bprewash_selected = FALSE;
						 bHotRinse_Selected = FALSE;
						 bCold_Soak_Selected = NO;
					}
					uimaxspeed_temp = (Wash40 | Rpm600);
					if(uiMachineState != PAUSE_PRESSED)
					{
						if(uiReset_TempRPM == TRUE){
						uiRMP_Default = uiRpmSel = Rpm400; 
						uiTempSel = Wash30;}
					}
					 uidefault_kg = CRADLE_DEFAULT_KG;//20;
					uiUB_decidation = 1; 
					uiExtraRinse_Max = 1;
					
				break;	
				
	//		case WOOLLENS : //Woollens			
	//				
	//				if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
	//				{
	//					 uiPSER_Option_Avbl =  ExtraRinse_Yes | DelayStart_Yes ;
	//					 bprewash_selected = FALSE;
	//					 bHotRinse_Selected = FALSE;
	//					 bCold_Soak_Selected = NO;
	//				}
	//				
	//				uimaxspeed_temp = Wash40 | Rpm800;
	//				
	//				if(uiMachineState != PAUSE_PRESSED)
	//				{
	//					if(uiReset_TempRPM == TRUE)
	//					{
	//						uiRMP_Default = uiRpmSel = Rpm800; 
	//						uiTempSel = Wash30;
	//					}
	//				}
	//				uidefault_kg = 20;
	//				uiUB_decidation = 1;
	//				uiExtraRinse_Max = 1;
	//				
	//			break;
				
			/*case DRAINSPIN : // Spin Dry/Drain
					if((uiMachineState != PAUSE_PRESSED) && (bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
					{
						 uiPSER_Option_Avbl =  DelayStart_Yes | TimeSaver_Yes ;
						 bprewash_selected = FALSE;
					}
					uimaxspeed_temp =  Rpm1400;
					if(uiMachineState != PAUSE_PRESSED)
					{
						if(uiReset_TempRPM == TRUE){
							uiRMP_Default = uiRpmSel = Rpm1400; 
							uiTempSel = 0x00;
						}
					}
					uidefault_kg = 00;
					uiExtraRinse_Max = 0;
					
				break;*/	
				
			case ADDITIVESRINSE : //Additives Rinse+Spin
						if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
						{
							 uiPSER_Option_Avbl = ExtraRinse_Yes|  TimeSaver_Yes | DelayStart_Yes | Steam_Yes ;
							 bprewash_selected = FALSE;
							 bHotRinse_Selected = FALSE;
							 bCold_Soak_Selected = NO;
						}
						uimaxspeed_temp =  Rpm1200;//Rpm1400;
						if(uiMachineState != PAUSE_PRESSED)
						{
							if(uiReset_TempRPM == TRUE){
							uiRMP_Default = uiRpmSel = Rpm1000; 
							uiTempSel = 0x00;}
						}
						uidefault_kg = ADDITIVESRINSE_DEF_KG;//90;
						uiUB_decidation = 0;
						uiExtraRinse_Max = 2;
				break;
				
			case STEAMFRESH:
					if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
					{
						 uiPSER_Option_Avbl =   Steam_Yes;
						 bprewash_selected = FALSE;
						 bHotRinse_Selected = FALSE;
						 bCold_Soak_Selected = NO;
						 if((uiBefore_Snooze != PROGRAM_END)&&(uiMachineState != PROGRAM_END))
						 {
							 uidisp_select_option |= Steam_Yes;
							 //ckey_digit_data[0] |= Steam_Yes;
							ckey_digit_data[3] = ckey_digit_data[3] | (0x02);
							uioption_on = (uioption_on | 0x08); //added to keep by default option led glowing
						 }
					}
					uimaxspeed_temp = 0x10;
					if(uiMachineState != PAUSE_PRESSED)
					{
						if(uiReset_TempRPM == TRUE)
						{
						uiRMP_Default = uiRpmSel = Rpm000; 
						uiTempSel = 0x00;
						}
					}
					uidefault_kg = STEAMFRESH_DEFAULT_KG;//10;
					uiUB_decidation = 0;
				break;
	//		case EXPRESS_30:
	//				if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
	//				{
	//					 uiPSER_Option_Avbl =  ExtraRinse_Yes|  DelayStart_Yes | Steam_Yes ;
	//					 bprewash_selected = FALSE;
	//					 bHotRinse_Selected = FALSE;
	//					 bCold_Soak_Selected = NO;
	//				}
	//				uimaxspeed_temp = Wash60 | Rpm1200 ;
	//				if(uiMachineState != PAUSE_PRESSED)
	//				{
	//					if(uiReset_TempRPM == TRUE)
	//					{
	//						uiRMP_Default = uiRpmSel = Rpm1000; 
	//						uiTempSel = Wash40;
	//					}
	//				}
	//				uidefault_kg 		= EXPRESS_30_DEFAULT_KG;
	//				uiExtraRinse_Max 	= EXPRESS_30_MAX_ER;
	//				uiUB_decidation = 1;
	//				break;
	//			
	//		case ANTIALLERGEN : // HYGIENE
	//				if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
	//				{					 				
	//				 	 uiPSER_Option_Avbl =   WarmSoak_Yes | ExtraRinse_Yes | TimeSaver_Yes | DelayStart_Yes | Steam_Yes;					
	//					 if((uiBefore_Snooze != PROGRAM_END)&&(uiMachineState != PROGRAM_END))
	//					 {
	//						 if((( uidisp_select_option & TimeSaver_Yes)!=TimeSaver_Yes))
	//						 {	
	//						 	//uidisp_select_option |= WarmSoak_Yes;
	//						 	//ckey_digit_data[0] |= WarmSoak_Yes;
	//							uidisp_select_option &= ~WarmSoak_Yes;	//TODO: Default to selectable.
	//#if ENABLE_IOT							 
	//					//set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);					
	//#endif							
	//						 }
	//					 }					
	//					 bprewash_selected = FALSE;
	//					 bCold_Soak_Selected = NO;
	//					 //bHotRinse_Selected = FALSE;
	//					 //below code added due to default selection is available
	//					 	if((prev_program_pos == 4)||(prev_program_pos == 0))
	//						{
	//						 	uiExtraRinse_Press_count = 2;
	//						 	uiExtraRinse_Press_Select = 2;
	//						}
	//						 if((uiBefore_Snooze != PROGRAM_END)&&(uiMachineState != PROGRAM_END))
	//						 {
	//							 uidisp_select_option |= ExtraRinse_Yes;
	//							 ckey_digit_data[0] |= ExtraRinse_Yes;
	//				
	//						 }
	//#if ENABLE_IOT
	//			set_IOTExtraRinse(uiExtraRinse_Press_Select);			
	//#endif
	//				}				
	//				uimaxspeed_temp = Wash95 | Rpm1000 ;
	//				
	//				if(uiMachineState != PAUSE_PRESSED)
	//				{
	//					if(uiReset_TempRPM == TRUE){
	//					uiRMP_Default = uiRpmSel = Rpm800; 
	//					uiTempSel = Wash60;}
	//				}
	//				uidefault_kg = 30;
	//				uiUB_decidation = 0;
	//				uiExtraRinse_Max = 2;				
	//				break;
	//			
			case BABYWEAR:
					if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
					{					 
						
					 	uiPSER_Option_Avbl =  Prewash_Yes | WarmSoak_Yes | ExtraRinse_Yes| TimeSaver_Yes | DelayStart_Yes  | Steam_Yes;						
						 if((uiBefore_Snooze != PROGRAM_END)&&(uiMachineState != PROGRAM_END))
						 {
							 if((uiTempSel != WashCold) && (uiTempSel != P_COLD))
							 {
								  if(( uidisp_select_option & TimeSaver_Yes)!=TimeSaver_Yes)
								  {	
								  	//uidisp_select_option |= WarmSoak_Yes;							
									//ckey_digit_data[0] |= WarmSoak_Yes;
									uidisp_select_option &= ~WarmSoak_Yes;	//TODO: Default to selectable.
								  }
								  bCold_Soak_Selected = NO;
	#if ENABLE_IOT							 
						//set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);					
	#endif							 
							 }
							 else
							 {
								 uiPSER_Option_Avbl = uiPSER_Option_Avbl & (~WarmSoak_Yes);
								ckey_digit_data[0] &= ~WarmSoak_Yes;
								uidisp_select_option &= ~WarmSoak_Yes;
								//bCold_Soak_Selected = YES;
							 }
						 }
							
						 bprewash_selected = FALSE;
						// bHotRinse_Selected = FALSE;
					}
					//uimaxspeed_temp = Wash60 | Rpm800;
					uimaxspeed_temp = P_60 | Rpm800;
					if(uiMachineState != PAUSE_PRESSED){
						if(uiReset_TempRPM == TRUE){
						uiRMP_Default = uiRpmSel = Rpm800; 
						uiTempSel = Wash60;}
					}
					uidefault_kg = BABYWEAR_DEFAUL_KG;//30;
					uiExtraRinse_Max = 2;
					uiUB_decidation = 1;
					
				break;
			
			case COTTON : //Cotton  Normal 
					
					if(uiMachineState != PAUSE_PRESSED)
					{
						if(uiReset_TempRPM == TRUE)
						{
							uiRMP_Default = uiRpmSel = Rpm1000; 
							uiTempSel = Wash40;
						}
					}
					if((uiMachineState != PAUSE_PRESSED) && (bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
					{
						if((uiTempSel == Wash60E)||(uiTempSel == Wash40E))
						{
							uiPSER_Option_Avbl =  WarmSoak_Yes | ExtraRinse_Yes| DelayStart_Yes ;
							uiExtraRinse_Max = 1;
						}
						else
						{
						 	uiPSER_Option_Avbl = Prewash_Yes | WarmSoak_Yes | ExtraRinse_Yes| TimeSaver_Yes | DelayStart_Yes | Steam_Yes;
							uiExtraRinse_Max = COTTON_MAX_ER;
							 if((uiBefore_Snooze != PROGRAM_END)&&(uiMachineState != PROGRAM_END))
							 {
								if((uiTempSel != WashCold) && (uiTempSel != P_COLD))
								 {
									  if(( uidisp_select_option & TimeSaver_Yes)!=TimeSaver_Yes)
									  {	
									  	//uidisp_select_option |= WarmSoak_Yes;							
										//ckey_digit_data[0] |= WarmSoak_Yes;
										uidisp_select_option &= ~WarmSoak_Yes;	//TODO: Default to selectable.
									  }
									  bCold_Soak_Selected = NO;
	#if ENABLE_IOT							 
						//set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);					
	#endif								 
								 }
								 else
								 {
									 uiPSER_Option_Avbl = uiPSER_Option_Avbl & (~WarmSoak_Yes);
									ckey_digit_data[0] &= ~WarmSoak_Yes;
									uidisp_select_option &= ~WarmSoak_Yes;
									//bCold_Soak_Selected = YES;
								 }
							 }
							
						}
						 bprewash_selected = FALSE;
						// bHotRinse_Selected = FALSE;
							 
					}
					
					uimaxspeed_temp = P_95 | Rpm1200;//Rpm1400;
					uiUB_decidation = 0;
					uidefault_kg = COTTON_DEFAULT_KG;
					
					
				break;	
			
			case MIXEDSOIL : //MIXED SOIL
					
					if((uiMachineState != PAUSE_PRESSED) && (bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
					{
					 	 uiPSER_Option_Avbl = Prewash_Yes | WarmSoak_Yes | ExtraRinse_Yes| TimeSaver_Yes | DelayStart_Yes  | Steam_Yes;					
						 if((uiBefore_Snooze != PROGRAM_END)&&(uiMachineState != PROGRAM_END))
						 {
							 if((uiTempSel != WashCold) && (uiTempSel != P_COLD))
							 {
								 if(( uidisp_select_option & TimeSaver_Yes)!=TimeSaver_Yes)
								  {	
								  	//uidisp_select_option |= WarmSoak_Yes;	
									uidisp_select_option &= ~WarmSoak_Yes;	//TODO: Default to selectable.
									//ckey_digit_data[0] |= WarmSoak_Yes;
								  }
								  bCold_Soak_Selected = NO;
	#if ENABLE_IOT							 
						//set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);					
	#endif							 
							 }
							 else
							 {
								 uiPSER_Option_Avbl = uiPSER_Option_Avbl & (~WarmSoak_Yes);
								ckey_digit_data[0] &= ~WarmSoak_Yes;
								uidisp_select_option &= ~WarmSoak_Yes;
								//bCold_Soak_Selected = YES;
							 }
								 
						 }
						bprewash_selected = FALSE;
						//bHotRinse_Selected = FALSE;
						
					}
					
					uimaxspeed_temp = P_60 | Rpm1200;	//BTRA
					if(uiMachineState != PAUSE_PRESSED)
					{
						if(uiReset_TempRPM == TRUE)
						{
							if(bTestMode_Selection== TRUE)
							{
								uiRpmSel = Rpm800; 
							}
							else
							{
								uiRMP_Default = uiRpmSel = Rpm800;//Rpm1000; 
								uiTempSel = Wash40;
							}
						}
					}
					uidefault_kg 		= MIXED_DEFAULT_KG;
					uiUB_decidation = 0;
					uiExtraRinse_Max 	= MIXED_MAX_ER;
					
				break;	
			case CURTAINS :
					// curtain
					if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
					{						
					 	 uiPSER_Option_Avbl = Prewash_Yes | WarmSoak_Yes | ExtraRinse_Yes| TimeSaver_Yes | DelayStart_Yes ;					
						 if((uiBefore_Snooze != PROGRAM_END)&&(uiMachineState != PROGRAM_END))
						 {
							 if((uiTempSel != WashCold) && (uiTempSel != P_COLD))
							 {
								  if(( uidisp_select_option & TimeSaver_Yes)!=TimeSaver_Yes)
								  {	
								  	//uidisp_select_option |= WarmSoak_Yes;							
									//ckey_digit_data[0] |= WarmSoak_Yes;	//TODO: Default to selectable.
									uidisp_select_option &= ~WarmSoak_Yes;
								  }
								  bCold_Soak_Selected = NO;
#if ENABLE_IOT							 
					//set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);					
#endif								 
							 }
							 else
							 {
								uiPSER_Option_Avbl = uiPSER_Option_Avbl & (~WarmSoak_Yes);
								ckey_digit_data[0] &= ~WarmSoak_Yes;
								uidisp_select_option &= ~WarmSoak_Yes;
								//bCold_Soak_Selected = YES;
							 }
						 }
						bprewash_selected = FALSE;
						//bHotRinse_Selected = FALSE;
						
					}

					//uimaxspeed_temp = Wash60 | Rpm800;
					uimaxspeed_temp = P_60 | Rpm800;

					if(uiMachineState != PAUSE_PRESSED)
					{
						if(uiReset_TempRPM == TRUE){
							uiRMP_Default = uiRpmSel = Rpm800;
							uiTempSel = Wash40;}
					}

					uidefault_kg = CURTAINS_DEFAULT_KG;//20;
					uiUB_decidation = 1;
					uiExtraRinse_Max = 2;

					break;
				
			case SMARTPLUS :

					switch(uiSmartProgram_Selected)
					{
						case STEAMWASH:
								if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
								{
									 uiPSER_Option_Avbl =   ExtraRinse_Yes | DelayStart_Yes | Steam_Yes;
									 bprewash_selected = FALSE;
									 bHotRinse_Selected = FALSE;
									 bCold_Soak_Selected = NO;
									 if((uiBefore_Snooze != PROGRAM_END)&&(uiMachineState != PROGRAM_END))
									 {
	//									 uidisp_select_option |= Steam_Yes;
	//									 ckey_digit_data[0] |= Steam_Yes;
										uidisp_select_option |= Steam_Yes;
										ckey_digit_data[3] = ckey_digit_data[3] | (0x02);
										uioption_on = (uioption_on | 0x08); //added to keep by default option led glowing
									 }
									 /*else if(uiMachineState == PROGRAM_END)	//TODO: Steam should be selected after program end as well.
									 {
										 uidisp_select_option |= Steam_Yes;
										 ckey_digit_data[0] |= Steam_Yes;				 	
									 }*/
								}
								uimaxspeed_temp =  Rpm800;
								if(uiMachineState != PAUSE_PRESSED)
								{
									if(uiReset_TempRPM == TRUE)
									{
										uiRMP_Default = uiRpmSel = Rpm800; 
										uiTempSel = 0x00;
									}
								}
								uidefault_kg = STEAMWASH_DEFAULT_KG;//30;
								uiUB_decidation = 1;
								uiExtraRinse_Max = 1;
							break;	

					case INNERWEAR:
						if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
						{
							 uiPSER_Option_Avbl =   ExtraRinse_Yes| TimeSaver_Yes | Steam_Yes |  DelayStart_Yes;
							 bprewash_selected = FALSE;
							 bHotRinse_Selected = FALSE;
							 bCold_Soak_Selected = FALSE;
						}
						uimaxspeed_temp = Wash40 | Rpm600;
						if(uiMachineState != PAUSE_PRESSED)
						{
							if(uiReset_TempRPM == TRUE)
							{
								uiRMP_Default = uiRpmSel = Rpm600; 
								uiTempSel = Wash40;
							}
						}
						uidefault_kg = INNERWEAR_DEFAULT_KG;//15;
						uiUB_decidation = 1;
						uiExtraRinse_Max = 1;
					break;	
//					case CURTAINS :
//						// curtain
//						if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
//						{						
//						 	 uiPSER_Option_Avbl = Prewash_Yes | WarmSoak_Yes | ExtraRinse_Yes| TimeSaver_Yes | DelayStart_Yes ;					
//							 if((uiBefore_Snooze != PROGRAM_END)&&(uiMachineState != PROGRAM_END))
//							 {
//								 if((uiTempSel != WashCold) && (uiTempSel != P_COLD))
//								 {
//									  if(( uidisp_select_option & TimeSaver_Yes)!=TimeSaver_Yes)
//									  {	
//									  	//uidisp_select_option |= WarmSoak_Yes;							
//										//ckey_digit_data[0] |= WarmSoak_Yes;	//TODO: Default to selectable.
//										uidisp_select_option &= ~WarmSoak_Yes;
//									  }
//									  bCold_Soak_Selected = NO;
//	#if ENABLE_IOT							 
//						//set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);					
//	#endif								 
//								 }
//								 else
//								 {
//									uiPSER_Option_Avbl = uiPSER_Option_Avbl & (~WarmSoak_Yes);
//									ckey_digit_data[0] &= ~WarmSoak_Yes;
//									uidisp_select_option &= ~WarmSoak_Yes;
//									//bCold_Soak_Selected = YES;
//								 }
//							 }
//							bprewash_selected = FALSE;
//							//bHotRinse_Selected = FALSE;
//							
//						}
//
//						//uimaxspeed_temp = Wash60 | Rpm800;
//						uimaxspeed_temp = P_60 | Rpm800;
//
//						if(uiMachineState != PAUSE_PRESSED)
//						{
//							if(uiReset_TempRPM == TRUE){
//								uiRMP_Default = uiRpmSel = Rpm800;
//								uiTempSel = Wash40;}
//						}
//
//						uidefault_kg = CURTAINS_DEFAULT_KG;//20;
//						uiUB_decidation = 1;
//						uiExtraRinse_Max = 2;
//
//						break;
					case DARKWASH :
						// Darkwash
						if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
						{
							uiPSER_Option_Avbl = Prewash_Yes | ExtraRinse_Yes| TimeSaver_Yes | DelayStart_Yes ;
							bprewash_selected = FALSE;
							bHotRinse_Selected = FALSE;
							bCold_Soak_Selected = FALSE;
						}


						//uimaxspeed_temp = Wash40 | Rpm1000;
						uimaxspeed_temp = P_40 | Rpm1000;

						if(uiMachineState != PAUSE_PRESSED)
						{
							if(uiReset_TempRPM == TRUE){
								uiRMP_Default = uiRpmSel = Rpm1000; 
								uiTempSel = Wash40;}
						}

						uidefault_kg = DARKWASH_DEFAULT_KG;//30;
						uiUB_decidation = 0;
						uiExtraRinse_Max = 2;
						break;
					case TUBCLEAN :
						if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
						{
							uiPSER_Option_Avbl = 0x00 ;
							bprewash_selected = FALSE;
							bHotRinse_Selected = FALSE;
							bCold_Soak_Selected = FALSE;
						}

						uimaxspeed_temp =  Wash95 | Rpm000;
						if(uiMachineState != PAUSE_PRESSED)
						{
							if(uiReset_TempRPM == TRUE){
								uiRMP_Default = uiRpmSel = Rpm000;
								uiTempSel = Wash60;//Wash95; //default temp changed to 60 as per spec rev 11
							}
						}
						uidefault_kg = 00;
						uiUB_decidation = 0;
						break;
						
					case SPORTWEAR : //sport wear
						if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
						{
						 	 uiPSER_Option_Avbl = Prewash_Yes | WarmSoak_Yes | ExtraRinse_Yes | TimeSaver_Yes | DelayStart_Yes ;				
							 if((uiBefore_Snooze != PROGRAM_END)&&(uiMachineState != PROGRAM_END))
							 {
								 if((uiTempSel != WashCold) && (uiTempSel != P_COLD))
								 {
									  if(( uidisp_select_option & TimeSaver_Yes)!=TimeSaver_Yes)
									  {	
									  	//uidisp_select_option |= WarmSoak_Yes;							
										//ckey_digit_data[0] |= WarmSoak_Yes;	//TODO: Default to selectable.
										uidisp_select_option &= ~WarmSoak_Yes;
									  }
									  bCold_Soak_Selected = NO;
	#if ENABLE_IOT							 
						//set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);					
	#endif								 
								 }
								 else
								 {
									 uiPSER_Option_Avbl = uiPSER_Option_Avbl & (~WarmSoak_Yes);
									ckey_digit_data[0] &= ~WarmSoak_Yes;
									uidisp_select_option &= ~WarmSoak_Yes;
									//bCold_Soak_Selected = YES;
								 }
							 }
								
							bprewash_selected = FALSE;
							bHotRinse_Selected = FALSE;
							
						}
						//uimaxspeed_temp = Wash40 | Rpm800;
						uimaxspeed_temp = P_40 | Rpm800;
						if(uiMachineState != PAUSE_PRESSED)
						{
							if(uiReset_TempRPM == TRUE){
							uiRMP_Default = uiRpmSel = Rpm800; 
							uiTempSel = Wash40;}
						}
						uidefault_kg = SPORTSWEAR_DEFAULT_KG;//20;
						uiUB_decidation = 1;
						uiExtraRinse_Max = 2;
						break;
					case JEANS : 
						if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
						{						
						 	 uiPSER_Option_Avbl = Prewash_Yes | WarmSoak_Yes | ExtraRinse_Yes| TimeSaver_Yes | DelayStart_Yes ;				
							 if((uiBefore_Snooze != PROGRAM_END)&&(uiMachineState != PROGRAM_END))
							 {
								  if(( uidisp_select_option & TimeSaver_Yes)!=TimeSaver_Yes)
								  {	
								  	//uidisp_select_option |= WarmSoak_Yes;							
									//ckey_digit_data[0] |= WarmSoak_Yes;	//TODO: Default to selectable.
									uidisp_select_option &= ~WarmSoak_Yes;
	#if ENABLE_IOT							 
						//set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);					
	#endif								
								  }
							 }													
							 bprewash_selected = FALSE;
							 bCold_Soak_Selected = FALSE;
							// bHotRinse_Selected = FALSE;
						}
						//uimaxspeed_temp = Wash60 | Rpm1400;
						uimaxspeed_temp = P_60 | Rpm1200;//Rpm1400;
						if(uiMachineState != PAUSE_PRESSED)
						{
							if(uiReset_TempRPM == TRUE){
							uiRMP_Default = uiRpmSel = Rpm1000; 
							uiTempSel = Wash60;}
						}
						uidefault_kg = JEANS_DEFAULT_KG;//35;
						uiUB_decidation = 0;
						uiExtraRinse_Max = 1;
						break;
					case UNIFORM : 
						if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
						{						
						 	uiPSER_Option_Avbl = Prewash_Yes | WarmSoak_Yes | ExtraRinse_Yes| TimeSaver_Yes | DelayStart_Yes | Steam_Yes;				
							 if((uiBefore_Snooze != PROGRAM_END)&&(uiMachineState != PROGRAM_END))
							 {
								 if((uiTempSel != WashCold)||(uiTempSel != P_COLD))
								 {
									  if(( uidisp_select_option & TimeSaver_Yes)!=TimeSaver_Yes)
									  {	
									  	//uidisp_select_option |= WarmSoak_Yes;							
										//ckey_digit_data[0] |= WarmSoak_Yes;	//TODO: Default to selectable.
										uidisp_select_option &= ~WarmSoak_Yes;
									  }
									  bCold_Soak_Selected = NO;
	#if ENABLE_IOT							 
						//set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);					
	#endif								 
								 }
								 else
								 {
									 uiPSER_Option_Avbl = uiPSER_Option_Avbl & (~WarmSoak_Yes);
									ckey_digit_data[0] &= ~WarmSoak_Yes;
									uidisp_select_option &= ~WarmSoak_Yes;
									//bCold_Soak_Selected = YES;
								 }
							 }
								
							 bprewash_selected = FALSE;
							 bHotRinse_Selected = FALSE;
						 
						}
						//uimaxspeed_temp = Wash60 | Rpm1000;
						uimaxspeed_temp = P_60 | Rpm1000;
						if(uiMachineState != PAUSE_PRESSED)
						{
							if(uiReset_TempRPM == TRUE){
							uiRMP_Default = uiRpmSel = Rpm800; 
							uiTempSel = Wash40;}
						}
						uidefault_kg = UNIFORM_DEFAULT_KG;//30;
						uiUB_decidation = 0;
						uiExtraRinse_Max = 2;
					
						break;
					case SHIRTS : 
						if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
						{
						 	 uiPSER_Option_Avbl = Prewash_Yes | WarmSoak_Yes | ExtraRinse_Yes | Steam_Yes | TimeSaver_Yes | DelayStart_Yes;				
							 if((uiBefore_Snooze != PROGRAM_END)&&(uiMachineState != PROGRAM_END))
							 {
								 if((uiTempSel != WashCold)&&(uiTempSel != P_COLD))
								 {
									  if(( uidisp_select_option & TimeSaver_Yes)!=TimeSaver_Yes)
									  {	
									  	//uidisp_select_option |= WarmSoak_Yes;							
										//ckey_digit_data[0] |= WarmSoak_Yes;
										uidisp_select_option &= ~WarmSoak_Yes;	//TODO: Default to selectable.
									  }
									  bCold_Soak_Selected = NO;
	#if ENABLE_IOT							 
						//set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);					
	#endif								 
								 }
								 else
								 {
									uiPSER_Option_Avbl = uiPSER_Option_Avbl & (~WarmSoak_Yes);
									ckey_digit_data[0] &= ~WarmSoak_Yes;
									uidisp_select_option &= ~WarmSoak_Yes;
									//bCold_Soak_Selected = YES;
								 }
							 }
								
							 bprewash_selected = FALSE;
							 bHotRinse_Selected = FALSE;
							 
						}
						uimaxspeed_temp = P_60 | Rpm800; //Wash60 | Rpm800;
						if(uiMachineState != PAUSE_PRESSED)
						{
							if(uiReset_TempRPM == TRUE){
							uiRMP_Default = uiRpmSel = Rpm800; 
							uiTempSel = Wash40;}
						}
						uidefault_kg = SHIRTS_DEFAULT_KG;//20;
						uiUB_decidation = 1;
						uiExtraRinse_Max = 2;
					
						break;

					case LINENWASH:
						if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
						{
							uiPSER_Option_Avbl =   ExtraRinse_Yes|  DelayStart_Yes | Steam_Yes;
						 	bprewash_selected = FALSE;
						 	bHotRinse_Selected = FALSE;
						 	bCold_Soak_Selected = NO;
	//						if((uiTempSel == Wash30)||(uiTempSel == Wash40))
	//						{
								if((uiBefore_Snooze != PROGRAM_END)&&(uiMachineState != PROGRAM_END))
								{
								 	//uidisp_select_option |= Steam_Yes;
								 	//ckey_digit_data[0] |= Steam_Yes;
									uidisp_select_option |= Steam_Yes;
									ckey_digit_data[3] = ckey_digit_data[3] | (0x02);
									uioption_on = (uioption_on | 0x08); //added to keep by default option led glowing
	#if ENABLE_IOT
									set_IOTOptionEnable(OPTION_ENABLE_2, OPTION_2_STEAM);
	#endif
					
								}
	//						}
							
							
						}
						uimaxspeed_temp = (Wash40 | Rpm600);
						if(uiMachineState != PAUSE_PRESSED)
						{
							if(uiReset_TempRPM == TRUE){
							uiRMP_Default = uiRpmSel = Rpm400; 
							uiTempSel = Wash30;}
						}
					 	uidefault_kg = LINENWASH_DEFAULT_KG;//30;
					 	uiUB_decidation = 1;//changed
						uiExtraRinse_Max = 1;
					
						break;
					
					case WOOLLENS : //Woollens			
					
						if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
						{
						 	uiPSER_Option_Avbl =  ExtraRinse_Yes | DelayStart_Yes ;
						 	bprewash_selected = FALSE;
						 	bHotRinse_Selected = FALSE;
						 	bCold_Soak_Selected = NO;
						}
					
						uimaxspeed_temp = Wash40 | Rpm800;
					
						if(uiMachineState != PAUSE_PRESSED)
						{
							if(uiReset_TempRPM == TRUE)
							{
								uiRMP_Default = uiRpmSel = Rpm800; 
								uiTempSel = Wash30;
							}
						}
						uidefault_kg = WOOLENS_DEFAULT_KG;//20;
						uiUB_decidation = 1;//changed
						uiExtraRinse_Max = 1;
					
						break;
					
						
					case ANTIALLERGEN : // HYGIENE
						if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
						{					 				
						 	 uiPSER_Option_Avbl =   Prewash_Yes | WarmSoak_Yes | ExtraRinse_Yes | TimeSaver_Yes | DelayStart_Yes | Steam_Yes;					
							 if((uiBefore_Snooze != PROGRAM_END)&&(uiMachineState != PROGRAM_END))
							 {
								 if((( uidisp_select_option & TimeSaver_Yes)!=TimeSaver_Yes))
								 {	
								 	//uidisp_select_option |= WarmSoak_Yes;
								 	//ckey_digit_data[0] |= WarmSoak_Yes;
									uidisp_select_option &= ~WarmSoak_Yes;	//TODO: Default to selectable.
		#if ENABLE_IOT							 
							//set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);					
		#endif							
								 }
							 }					
							 bprewash_selected = FALSE;
							 bCold_Soak_Selected = NO;
							 //bHotRinse_Selected = FALSE;
							 //below code added due to default selection is available
							 	if((prev_program_pos == 4)||(prev_program_pos == 0))
								{
								 	uiExtraRinse_Press_count = 2;
								 	uiExtraRinse_Press_Select = 2;
								}
								 if((uiBefore_Snooze != PROGRAM_END)&&(uiMachineState != PROGRAM_END))
								 {
									 uidisp_select_option |= ExtraRinse_Yes;
									 ckey_digit_data[0] |= ExtraRinse_Yes;
									 uioption_on = (uioption_on | 0x04); //added to keep by default option led glowing
						
								 }
		#if ENABLE_IOT
					set_IOTExtraRinse(uiExtraRinse_Press_Select);			
		#endif
						}				
						//uimaxspeed_temp = Wash95 | Rpm1000 ;
						uimaxspeed_temp = P_95 | Rpm1000 ;
						
						if(uiMachineState != PAUSE_PRESSED)
						{
							if(uiReset_TempRPM == TRUE){
							uiRMP_Default = uiRpmSel = Rpm800; 
							uiTempSel = Wash60;}
						}
						uidefault_kg = ANTIALLERGEN_DEFAULT_KG;//30;
						uiUB_decidation = 0;
						uiExtraRinse_Max = 2;				
						break;
					
					case EXPRESS_30:
						if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
						{
							 uiPSER_Option_Avbl =  ExtraRinse_Yes|  DelayStart_Yes | Steam_Yes ;
							 bprewash_selected = FALSE;
							 bHotRinse_Selected = FALSE;
							 bCold_Soak_Selected = NO;
						}
						uimaxspeed_temp = Wash60 | Rpm1200 ;
						if(uiMachineState != PAUSE_PRESSED)
						{
							if(uiReset_TempRPM == TRUE)
							{
								uiRMP_Default = uiRpmSel = Rpm1000; 
								uiTempSel = Wash40;
							}
						}
						uidefault_kg 		= EXPRESS_30_DEFAULT_KG;
						uiExtraRinse_Max 	= EXPRESS_30_MAX_ER;
						uiUB_decidation = 1;
						break;
						
					default :
						break;
					}
				break;
			default : 
			 	break;
			
			
		}
	}
	else// tubclean true for neo
	{
		if((uiMachineState != PAUSE_PRESSED) &&(bTestMode_Selection == FALSE)&&(bFCTMode_Selection == FALSE))
		{
			uiPSER_Option_Avbl = 0x00 ;
			bprewash_selected = FALSE;
			bHotRinse_Selected = FALSE;
			bCold_Soak_Selected = FALSE;
		}

			uimaxspeed_temp =  Wash95 | Rpm000;
			if(uiMachineState != PAUSE_PRESSED)
			{
				if(uiReset_TempRPM == TRUE){
					uiRMP_Default = uiRpmSel = Rpm000;
					uiTempSel = Wash60;
				}
			}
			uidefault_kg = 00;
			uiUB_decidation = 0;
	}
	
	uiopt_check = RESET_VALUE;	
	if(uiMachineState != PAUSE_PRESSED)
		bTimeSaver_Start = bSteam_Start = bDelay_Start1 = bProgram_Start = bPrewash_Start = bSoak_Start = bHotRinse_Start = bExtraRinse_Start = FALSE;
	cFlash_Count = 0x00;
}
void ResProg_led()
{
	ckey_digit_data[3] = 0x00;
	ckey_digit_data[4] = 0x00;
	ckey_digit_data[5] = 0x00;
}

void Reset_ProgParamter()
{
	if((uiBefore_Snooze != PROGRAM_END))
	{
		uilast_selected_option = RESET_VALUE;
		uidisp_select_option = RESET_VALUE ;
		uiExtraRinse_Press_count = uiExtraRinse_Press_Select = RESET_VALUE;
		uiSoak_Press_count = RESET_VALUE;
		uiReset_TempRPM = TRUE;
		bRapidWashButton = FALSE;
		uiRinseholdState = RESET_VALUE;
		ckey_digit_data[0] = RESET_VALUE;
#if ENABLE_IOT
		bHotRinse_Selected = FALSE;
#endif
	}
	else
	{	
		if(uiSelected_Position == ADDITIVESRINSE)
		{
			//TubClean_Activated = RESET_VALUE;
//After END on knob rotation, extra rinse 1 gets deselected but 1400rpm remains selected;issue fixed
//			uimaxspeed_temp =  Rpm1000;
//			uiRpmSel = Rpm1000; 
//			uiTempSel = 0x00;
//			uidefault_kg = 00;
//			uiExtraRinse_Max = 2;
//			
//			uiPSER_Option_Avbl = ExtraRinse_Yes|  TimeSaver_Yes | DelayStart_Yes;
		}
	}
	bRinsehold_Finished = FALSE;
	uiSelected_Position = uiProgram_Position; 
	uicycle_mincompleted = RESET_VALUE;
	uiOption_closed  = RESET_VALUE;
	uicycle_mincompleted = RESET_VALUE;
	bCold_Soak_Selected = RESET_VALUE;//vish
	snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT;
	spin_first_press = RESET_VALUE;
	//TubClean_Activated = FALSE;
	temp_first_press = RESET_VALUE;
	//bExpress_30_Activatted = FALSE;
//	if(uiSelected_Position != EXPRESS)
//	{
//		bExpress_30_Activatted = FALSE;
//	}
#if ENABLE_IOT
	 if(uiSelected_Position != SMARTPLUS)
	 // if((uiSelected_Position != SMARTPLUS)||(uiSelected_Position != ADDITIVESRINSE))  // DECISION NEED TO BE TAKEN BASED WIFI CONFIG
	{
		if((uiBefore_Snooze!=PROGRAM_END)&&(uiMachineState!=PROGRAM_END))
			uiSmartProgram_Selected = FALSE;
			TubClean_Activated = FALSE;
	}
	else if(uiSmartProgram_Selected != TUBCLEAN)
	{
		TubClean_Activated = FALSE;	
	}
#endif
	uidisplay_flash_count = RESET_VALUE;
	uidelaymin = RESET_VALUE;
	bKeyDetected = RESET_VALUE;
	uiIDLE_COUNT = uiDISP_MCount = RESET_VALUE;
	rpm_Selection = RESET_VALUE;
	uioption_on = RESET_VALUE;
	b_option_key_pressed_flag = RESET_VALUE;
	bSpin_Available = TRUE;
	ProgSel_led();  // update the led sattus
	RESET_Prog();
	ProgramTimming(); // select & show the programming timing 
	bTub_deselect = RESET_VALUE;
	
	if(uiPSER_Option_Avbl != 0)
	{
		while(!(uiopt_check & uiPSER_Option_Avbl))
		{
			uiopt_check = uiopt_check >> 1;
			
			if(uiopt_check < 1)
			{
				uiopt_check = 0x20;
			}
		}
	}
}

/***********************************************************************
* Function_Name : AI_ProgramTimeUpdate(void).
* Ret_type	: void.
* Working	: To update balance time of AI selected program.
* Developed By	: Prashant_H
* Date		: 22/04/2022
************************************************************************/
void AI_ProgramTimeUpdate()
{	static uint16_t CompletedMin_bkup_ui16;
	switch(uiSelected_Position)
	{
		case COTTON:
			switch(GET_PROGRAM)
			{
				case COTTON: break;
				case SYNTHETIC:
					/*Taking bakup of cycle completed time caus in ProgramTimming() funcion it getting reset.*/
					CompletedMin_bkup_ui16 = uicycle_mincompleted; 
					ProgramTimming();
					
					//Added for the new requirement of Algo team to disable the Steam option in Synthetic
					if((uidisp_select_option & (Steam_Yes)) == Steam_Yes)
					{
						uidisp_select_option = uidisp_select_option & (~Steam_Yes);
						uilast_selected_option = uidisp_select_option & (~Steam_Yes);
						uioption_on = (uioption_on & (~1<<3));
						BalanceTime = BalanceTime - STEAMFRESH_TIME;
					}
					//time subtracted during load sensing timing update is added back here because after switching the LS time update has to be re done for the switched program
					//cycle minutes completed in the first program before switching are subtracted 
					BalanceTime  = (BalanceTime - CompletedMin_bkup_ui16) + uiLS_Total_subtraction;
					uicycle_mincompleted = CompletedMin_bkup_ui16 - uiLS_Total_subtraction;
					After_LS_Update_Para();
					break;
				case MIXEDSOIL:
					/*Taking bakup of cycle completed time caus in ProgramTimming() funcion it getting reset.*/
					CompletedMin_bkup_ui16 = uicycle_mincompleted; 
					ProgramTimming();
					BalanceTime  = (BalanceTime - CompletedMin_bkup_ui16) + uiLS_Total_subtraction;
					uicycle_mincompleted = CompletedMin_bkup_ui16 - uiLS_Total_subtraction;
					After_LS_Update_Para();
					break;
			}
			break;
		case SYNTHETIC:
			switch(GET_PROGRAM)
			{
				case SYNTHETIC: break;
				case COTTON:
					
					CompletedMin_bkup_ui16 = uicycle_mincompleted;
					ProgramTimming();
					if((uidisp_select_option & Prewash_Yes) == Prewash_Yes)
					{
						BalanceTime  = (BalanceTime - (CompletedMin_bkup_ui16+1)) + uiLS_Total_subtraction;
						uicycle_mincompleted = (CompletedMin_bkup_ui16+1) - uiLS_Total_subtraction;	
					}
					else
					{
						BalanceTime  = (BalanceTime - CompletedMin_bkup_ui16) + uiLS_Total_subtraction;
						uicycle_mincompleted = CompletedMin_bkup_ui16 - uiLS_Total_subtraction;	
					
					}
					After_LS_Update_Para();
					
				
				break;
				case MIXEDSOIL:
					CompletedMin_bkup_ui16 = uicycle_mincompleted;
					ProgramTimming();
					BalanceTime  = (BalanceTime - CompletedMin_bkup_ui16) + uiLS_Total_subtraction;
					uicycle_mincompleted = CompletedMin_bkup_ui16 - uiLS_Total_subtraction;
					After_LS_Update_Para();
					break;
			}
			break;
		default:
			break;
	}
}
