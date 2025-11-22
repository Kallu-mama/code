#include "ErrorHandling.h"
#include "r_cg_userdefine.h"
#include "STLED316S.h" 
#include "Selection.h"
#include "WashOperations_Global.h"
#include "Version.h"

//***********************Gobal Function*************************//
extern void Read_Errors(uint8_t );

//************************************
//******************* Gobal Variables********************
extern boolean bProgramStart,bSpin_Start,bWaterLevel_Read_Required,bDisplay_Flag,chip_select;

extern uint8_t cdigits_data_array[6],uiUnbalance_Value,cdigits_data[7];
extern uint8_t cdigits_data[7],uiStep_Minutes_Completed,uiTime_Minutes_Completed;
extern uint8_t uiMachineState,ckey_digit_data[6];
extern uint8_t Error_Storage[7],uiDebugState,uiIPM_Temperature,uiWaterResetCount; 

extern uint8_t ii,ucDbgSelectKey_count,ucDbgTempKey_count,uiUnbalance_Value,uiBot_test_Version,uiBot_Ldf_Version;
extern uint8_t ucDbgRpmKey_count,uidisp_errors_count,ucDbgOptionKey_count,uiBot_Prod_Version,ucDbgTimesaverKey_count;


extern uint16_t uiprev_delaymin,ProgramLifeCycle,uiLS_RPMValue;
extern uint16_t uidebug_key,uiCurrent_step_time;

extern int cSwitch_Pressed;

extern uint16_t iWaterLevel;

extern uint8_t uiTemperature,uiUnbalance_Level,uiSW_VF_MD;
//extern uint16_t uiRPMValue;

extern uint16_t RPMtest;//, Neg_debug_weight, debug_weight;
extern uint8_t uiFinalWt,uiLoadRange;

//******************** Local Variables *******************
uint8_t test_shift;
uint8_t bottom_Array[2];
uint16_t watervalue,uiRPMValue, uiTEMPValue;

#if ENABLE_IOT
extern uint16_t uiMacAddress;
#endif
/***********************************************************************************************************************
* Function Name: DebugMode
* Description  : This function is display various parameter Values for service person.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void DebugMode()
{
   //uint16_t RPMVAL;
   
	if(bDisplay_Flag == TRUE)
	{
		switch(uiDebugState)
	    	{
	    	     case step1:
				//Romove "dbg"
				
				turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
				
				mystrcpy((char *)cdigits_data_array, " dbg");
			        update_display_data(&cdigits_data_array[0] );
				
				
				//if((cSwitch_Pressed == OPTION_KEY)||(cSwitch_Pressed == TEMPERATURE_KEY) || (cSwitch_Pressed == SPIN_KEY)|| (cSwitch_Pressed == SELECT_KEY))
				if( cSwitch_Pressed == 0)
				{
					uiDebugState = step2;
				}
				
				break;
		     case step2:
		     		switch(uidebug_key)
				{
					case TIMESAVER_KEY:
							turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
							
							//Read_Errors(Error_Storage[ucDbgOptionKey_count - 1]);
							Read_Errors(Error_Storage[ucDbgTimesaverKey_count - 1]);
							
							
						break;
						
					case TEMPERATURE_KEY:
						switch(ucDbgTempKey_count)
						{
							case 1:
							
								//1.	First press it should display Pressostat Frequency.
									//if((uiMachineState == PAUSE_PRESSED)||(uiMachineState == IDLE_START))
										//bWaterLevel_Read_Required = TRUE; 		
									watervalue = (iWaterLevel) / 10;
									SevenSegPrint(watervalue);
									
									turnon_display_led(TIMEDOTS_OFF,SEMI_ON,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
									
								break;
							case 2:
							
									//2.	Second press it should display the Step time
									
									turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
									
									DisplayTime(uiCurrent_step_time);
									
									
									
								break;
							
							case 3:
							
								//3.	Third press  it should display Unbalance Value UL-
								
									turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
									
									if(uiUnbalance_Level == 0)
									{
										mystrcpy((char *)cdigits_data_array, " UL0");
									}
									else if(uiUnbalance_Level == 1)
									{
										mystrcpy((char *)cdigits_data_array, " UL1");
									}
									else if(uiUnbalance_Level == 2)
									{
										mystrcpy((char *)cdigits_data_array, " UL2");
									}
									else if(uiUnbalance_Level == 3)
									{
										mystrcpy((char *)cdigits_data_array, " UL3");
									}
							        	
							       		update_display_data(&cdigits_data_array[0] );
								break;
							case 4:
									//4. Fourth Press Show the Unbalance no
									if(uiUnbalance_Value > RESET_VALUE)
									{
										SevenSegPrint((uint8_t)uiUnbalance_Value);
									}
									else
									{
										mystrcpy((char *)cdigits_data_array, " 000");
										update_display_data(&cdigits_data_array[0] );
									}
										
								
								break;
//							case 5:
//								//5. Fifth Press show the Mnufacturer code
//								
//								turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
////								mystrcpy((char *)cdigits_data_array, "  80");
////								update_display_data(&cdigits_data_array[0] );
//								test_shift = (uint8_t)(RPMtest >> 8);
//								SevenSegPrint(test_shift);// / 100);
//								cdigits_data[0] = (cdigits_data[0] & 0x80);//|(0x6D);
//								cdigits_data[1] = (cdigits_data[1] & 0x80);//|(0x6D);//|(0x71);
//								for(ii = 0xC0; ii < 0xC3; ii= ii + 0x02)
//								{
//									STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // update the display
//								}
//								break;
//								
//							case 6:
//								//6. Fifth Press show the Mnufacturer code
//								
//								turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
//								//mystrcpy((char *)cdigits_data_array, "  14");
//								//update_display_data(&cdigits_data_array[0] );
//								//SevenSegPrint(uiLoadRange);
//								SevenSegPrint(RPMtest);
//								cdigits_data[0] = (cdigits_data[0] & 0x80);//|(0x6D);
//								cdigits_data[1] = (cdigits_data[1] & 0x80);//|(0x6D);//|(0x71);
//								for(ii = 0xC0; ii < 0xC3; ii= ii + 0x02)
//								{
//									STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // update the display
//								}
//								break;
							case 5:
								//5. Fifth Press show the Mnufacturer code
								
								turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
								mystrcpy((char *)cdigits_data_array, "  80");
								update_display_data(&cdigits_data_array[0] );
								//SevenSegPrint(RPMtest);
								
								break;
								
							case 6:
								//6. Fifth Press show the Mnufacturer code
								
								turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
								mystrcpy((char *)cdigits_data_array, "  12");
								update_display_data(&cdigits_data_array[0] );
								//SevenSegPrint(uiLoadRange);
								break;
							case 7:
								//6. IPM Temperature
								turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);								
								SevenSegPrint(uiIPM_Temperature);
//								RPMVAL = (uiLS_RPMValue & 0x000f) + (uiLS_RPMValue & 0x00f0) + (uiLS_RPMValue & 0x0f00) + (uiLS_RPMValue & 0xf000);
//								SevenSegPrint(RPMVAL); //SevenSegPrint(uiLS_RPMValue);
								//SevenSegPrint(Neg_debug_weight);
								
								break;	
							case 8:
								//7. AI Program Selection
								turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);								
								SevenSegPrint(getAIComm_Program_ui8());
								break;	
							
							case 9: 
								//8. Final weight of the load detected after load sensing
								//SevenSegPrint(uiFinalWt);
								SevenSegPrint(uiLoadRange);
								break;
								
						}
					break;
					case SPIN_KEY:
						switch(ucDbgRpmKey_count)
						{
							case 1:
									//1.	First press it should display the Temperature of water --C  insteps of 5°C  
									uiTEMPValue = uiTemperature;
									
									//SevenSegPrint(  uiTEMPValue * 100);
									SevenSegPrint(  uiTEMPValue * 10 );
									//---------- for display o symbol ----------------
//									cdigits_data[2] = cdigits_data[2] & 0x80;
//									cdigits_data[2] = cdigits_data[2] | 0x63;
									cdigits_data[0] = (cdigits_data[0] & 0x80);//|(0x6D);
									cdigits_data[3] = cdigits_data[3] & 0x80;
									cdigits_data[3] = cdigits_data[3] | 0x39;
									//cdigits_data[1] = (cdigits_data[1] & 0x80);//|(0x6D);//|(0x71);
									for(ii = 0xC0; ii < 0xC8; ii= ii + 0x02)
									{
										STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // update the display
									}
									//------------------------------------------------
									turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_ON,TIMELEFT_OFF);
									// need to 0c 
								break;
							case 2:
									//2.	Second press it should display NTC Value
									turnon_display_led(TIMEDOTS_OFF,SEMI_ON,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
									
									if((uiTemperature >= 15)&&(uiTemperature <= 20))
									{
										mystrcpy((char *)cdigits_data_array, "1872");
									}
									else if((uiTemperature > 20)&&(uiTemperature <= 25))
									{
										mystrcpy((char *)cdigits_data_array, "1477");
									}
									else if((uiTemperature > 25)&&(uiTemperature <= 30))
									{
										mystrcpy((char *)cdigits_data_array, "1198");
									}
									else if((uiTemperature > 30)&&(uiTemperature <= 35))
									{
										mystrcpy((char *)cdigits_data_array, "0978");
									}
									else if((uiTemperature > 35)&&(uiTemperature <= 40))
									{
										mystrcpy((char *)cdigits_data_array, "0804");
									}
									else if((uiTemperature > 40)&&(uiTemperature <= 45))
									{
										mystrcpy((char *)cdigits_data_array, "0665");
									}
									else if((uiTemperature > 45)&&(uiTemperature <= 50))
									{
										mystrcpy((char *)cdigits_data_array, "0552");
									}
									else if((uiTemperature > 50)&&(uiTemperature <= 52))
									{
										mystrcpy((char *)cdigits_data_array, "0460");
									}
									else if((uiTemperature > 52)&&(uiTemperature <= 53))
									{
										mystrcpy((char *)cdigits_data_array, "0423");
									}
									else if((uiTemperature > 53)&&(uiTemperature <= 55))
									{
										mystrcpy((char *)cdigits_data_array, "0405");
									}
									else if((uiTemperature > 55)&&(uiTemperature <= 60))
									{
										mystrcpy((char *)cdigits_data_array, "0385");
									}
									else if((uiTemperature > 60)&&(uiTemperature <= 65))
									{
										mystrcpy((char *)cdigits_data_array, "0324");
									}
									else if((uiTemperature > 65)&&(uiTemperature <= 70))
									{
										mystrcpy((char *)cdigits_data_array, "0274");
									}
									else if((uiTemperature > 70)&&(uiTemperature <= 75))
									{
										mystrcpy((char *)cdigits_data_array, "0233");
									}
									else if((uiTemperature > 75)&&(uiTemperature <= 80))
									{
										mystrcpy((char *)cdigits_data_array, "0199");
									}
									else if((uiTemperature > 80)&&(uiTemperature <= 85))
									{
										mystrcpy((char *)cdigits_data_array, "0170");
									}
									else if((uiTemperature > 85)&&(uiTemperature <= 90))
									{
										mystrcpy((char *)cdigits_data_array, "0146");
									}
									else if((uiTemperature > 90)&&(uiTemperature <= 95))
									{
										mystrcpy((char *)cdigits_data_array, "0126");
									}
									else if((uiTemperature > 95)&&(uiTemperature <= 100))
									{
										mystrcpy((char *)cdigits_data_array, "0109");
									}
									//chip_select = FALSE ; // select the 2nd chip 
						       			update_display_data(&cdigits_data_array[0] );
								break;
							case 3:
									if((uiMachineState == PAUSE_PRESSED)||(uiMachineState == IDLE_START))
										bWaterLevel_Read_Required = TRUE; 
										
									turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
									
									if(iWaterLevel >=42300)
									{
										mystrcpy((char *)cdigits_data_array, " 00L");	
									}
									else if((iWaterLevel < 42300 )&&( iWaterLevel >= 41540))
									{
										mystrcpy((char *)cdigits_data_array, " 01L");
									}
									else if((iWaterLevel < 41540)&&(iWaterLevel >= 41200))
									{
										mystrcpy((char *)cdigits_data_array, " 02L");
									}
									else if((iWaterLevel < 41200)&&(iWaterLevel >= 40910))
									{
										mystrcpy((char *)cdigits_data_array, " 03L");
									}
									else if((iWaterLevel < 40910)&&(iWaterLevel >= 40640))
									{
										mystrcpy((char *)cdigits_data_array, " 04L");
									}
									else if((iWaterLevel <40640)&&(iWaterLevel >= 40400))
									{
										mystrcpy((char *)cdigits_data_array, " 05L");
									}
									else if((iWaterLevel <40400)&&(iWaterLevel >= 40180))
									{
										mystrcpy((char *)cdigits_data_array, " 06L");
									}
									else if((iWaterLevel < 40180)&&(iWaterLevel >= 39980))
									{
										mystrcpy((char *)cdigits_data_array, " 07L");
									}
									else if((iWaterLevel < 39980)&&(iWaterLevel >= 39790))
									{
										mystrcpy((char *)cdigits_data_array, " 08L");
									}
									else if((iWaterLevel < 39790)&&(iWaterLevel >= 39620))
									{
										mystrcpy((char *)cdigits_data_array, " 09L");
									}
									else if((iWaterLevel < 39620)&&(iWaterLevel >= 39440))
									{
										mystrcpy((char *)cdigits_data_array, " 10L");
									}
									else if((iWaterLevel < 39440)&&(iWaterLevel >= 39290))
									{
										mystrcpy((char *)cdigits_data_array, " 11L");
									}
									else if((iWaterLevel < 39290)&&(iWaterLevel >= 39130))
									{
										mystrcpy((char *)cdigits_data_array, " 12L");
									}
									else if((iWaterLevel < 39130)&&(iWaterLevel >= 38980))
									{
										mystrcpy((char *)cdigits_data_array, " 13L");
									}
									else if((iWaterLevel < 38980)&&(iWaterLevel >= 38830))
									{
										mystrcpy((char *)cdigits_data_array, " 14L");
									}
									else if((iWaterLevel < 38830)&&(iWaterLevel >= 38670))
									{
										mystrcpy((char *)cdigits_data_array, " 15L");
									}
									else if((iWaterLevel < 38670)&&(iWaterLevel >= 38560))
									{
										mystrcpy((char *)cdigits_data_array, " 16L");
									}
									else if((iWaterLevel < 38560)&&(iWaterLevel >= 38430))
									{
										mystrcpy((char *)cdigits_data_array, " 17L");
									}
									else if((iWaterLevel < 38430)&&(iWaterLevel >= 38290))
									{
										mystrcpy((char *)cdigits_data_array, " 18L");
									}
									else if((iWaterLevel < 38290)&& (iWaterLevel >= 38170))
									{
										mystrcpy((char *)cdigits_data_array, " 19L");
									}
									else if((iWaterLevel < 38170)&&(iWaterLevel >= 38050))
									{
										mystrcpy((char *)cdigits_data_array, " 20L");
									}
									else if((iWaterLevel < 38050)&&(iWaterLevel >= 37910))
									{
										mystrcpy((char *)cdigits_data_array, " 21L");
									}
									else if((iWaterLevel < 37910)&&(iWaterLevel >= 37790))
									{
										mystrcpy((char *)cdigits_data_array, " 22L");
									}
									else if((iWaterLevel < 37790)&&(iWaterLevel >= 37660))
									{
										mystrcpy((char *)cdigits_data_array, " 23L");
									}
									else if((iWaterLevel < 37660)&&(iWaterLevel >= 37530))
									{
										mystrcpy((char *)cdigits_data_array, " 24L");
									}
									else if((iWaterLevel < 37530)&&(iWaterLevel >= 37430))
									{
										mystrcpy((char *)cdigits_data_array, " 25L");
									}
									else if((iWaterLevel < 37430)&&(iWaterLevel >= 37330))
									{
										mystrcpy((char *)cdigits_data_array, " 26L");
									}
									else if((iWaterLevel < 37330)&&(iWaterLevel >= 37210))
									{
										mystrcpy((char *)cdigits_data_array, " 27L");
									}
									else if((iWaterLevel < 37210)&&(iWaterLevel >= 37090))
									{
										mystrcpy((char *)cdigits_data_array, " 28L");
									}
									else if((iWaterLevel < 37090)&&(iWaterLevel >= 36980))
									{
										mystrcpy((char *)cdigits_data_array, " 29L");
									}
									else if((iWaterLevel < 36980)&&(iWaterLevel >= 36860))
									{
										mystrcpy((char *)cdigits_data_array, " 30L");
									}
									else if((iWaterLevel < 36860)&&(iWaterLevel >= 36760))
									{
										mystrcpy((char *)cdigits_data_array, " 31L");
									}
									else if((iWaterLevel < 36760)&&(iWaterLevel >= 36670))
									{
										mystrcpy((char *)cdigits_data_array, " 32L");
									}
									else if((iWaterLevel < 36670)&&(iWaterLevel >= 36560))
									{
										mystrcpy((char *)cdigits_data_array, " 33L");
									}
									else if((iWaterLevel < 36560)&&(iWaterLevel >= 36480))
									{
										mystrcpy((char *)cdigits_data_array, " 34L");
									}
									else if((iWaterLevel < 36480)&&(iWaterLevel >= 36370))
									{
										mystrcpy((char *)cdigits_data_array, " 35L");
									}
									else if((iWaterLevel < 36370)&&(iWaterLevel >= 36290))
									{
										mystrcpy((char *)cdigits_data_array, " 36L");
									}
									else if((iWaterLevel < 36290)&&(iWaterLevel >= 36210))
									{
										mystrcpy((char *)cdigits_data_array, " 37L");
									}
									else if((iWaterLevel < 36210)&&(iWaterLevel >= 36100))
									{
										mystrcpy((char *)cdigits_data_array, " 38L");
									}
									else if((iWaterLevel < 36100)&&(iWaterLevel >= 36020))
									{
										mystrcpy((char *)cdigits_data_array, " 39L");
									}
									else if((iWaterLevel < 36020)&&(iWaterLevel >= 35950))
									{
										mystrcpy((char *)cdigits_data_array, " 40L");
									}
									else if((iWaterLevel < 35950)&&(iWaterLevel >= 35860))
									{
										mystrcpy((char *)cdigits_data_array, " 41L");
									}
									else if((iWaterLevel < 35860)&&(iWaterLevel >= 35780))
									{
										mystrcpy((char *)cdigits_data_array, " 42L");
									}
									else if((iWaterLevel < 35780)&&(iWaterLevel >= 35700))
									{
										mystrcpy((char *)cdigits_data_array, " 43L");
									}
									else if((iWaterLevel < 35700)&&(iWaterLevel >= 35640))
									{
										mystrcpy((char *)cdigits_data_array, " 44L");
									}
									else if((iWaterLevel < 35640)&&(iWaterLevel >= 35560))
									{
										mystrcpy((char *)cdigits_data_array, " 45L");
									}
									else if((iWaterLevel < 35560)&&(iWaterLevel >= 35490))
									{
										mystrcpy((char *)cdigits_data_array, " 46L");
									}
									else if((iWaterLevel < 35490)&&(iWaterLevel >= 35440))
									{
										mystrcpy((char *)cdigits_data_array, " 47L");
									}
									else if((iWaterLevel < 35440)&&(iWaterLevel >= 35380))
									{
										mystrcpy((char *)cdigits_data_array, " 48L");
									}
									else if((iWaterLevel < 35380)&&(iWaterLevel >= 35350))
									{
										mystrcpy((char *)cdigits_data_array, " 49L");
									}
									else if(iWaterLevel < 35340)
									{
										mystrcpy((char *)cdigits_data_array, " 50L");
									}
									//chip_select = FALSE ; // select the 2nd chip 
						       			update_display_data(&cdigits_data_array[0] );
									
								break;
							case 4:		
									turnon_display_led(TIMEDOTS_ON,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
									if(uiMacAddress > 0)
									{
										MacAddressDisplay(uiMacAddress); // To display 4 digits of MAC address.	
									}
									else
									{
										mystrcpy((char *)cdigits_data_array, " 000");
										update_display_data(&cdigits_data_array[0] );
									}	
								break;
							case 5:		
									turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_ON,TIMELEFT_OFF);
									mystrcpy((char *)cdigits_data_array, " lFb");
								       	update_display_data(&cdigits_data_array[0] );	
								break;
							case 6:		
									turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_ON,TIMELEFT_OFF);
									if(uiSW_VF_MD == 1)
									{
										mystrcpy((char *)cdigits_data_array, "  lF");
									}
									else
									{
										mystrcpy((char *)cdigits_data_array, "  UF");
									}
								       	update_display_data(&cdigits_data_array[0] );	
								break;
								
							}
						break;
						case SELECT_KEY:
						
								switch(ucDbgSelectKey_count)
								{
									case 1:
										//1.	First press it should display Software Version S--.
										turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
										
										mystrcpy((char *)cdigits_data_array, IB_PRODUCTION_VER);
								       		update_display_data(&cdigits_data_array[0] );
										
									break;
									case 2:
										turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
										bottom_Array[0]=(uiBot_Prod_Version/10)+0x30;
										bottom_Array[1]=(uiBot_Prod_Version%10)+0x30;
										
										mystrcpy((char *)cdigits_data_array, " P03");
										cdigits_data_array[2]=bottom_Array[0];
										cdigits_data_array[3]=bottom_Array[1];
								       		update_display_data(&cdigits_data_array[0] );
										//SevenSegPrint(ProgramLifeCycle);
									break;
									case 3:
										//3. Third press it should show LDF file version
										turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
										bottom_Array[0]=(uiBot_Ldf_Version/10)+0x30;
										bottom_Array[1]=(uiBot_Ldf_Version%10)+0x30;
										
										mystrcpy((char *)cdigits_data_array, "tL02");
										cdigits_data_array[2]=bottom_Array[0];
										cdigits_data_array[3]=bottom_Array[1];
								       		update_display_data(&cdigits_data_array[0] );
										//SevenSegPrint(ProgramLifeCycle);
									break;
									case 4:
										//4. Fourth press it should  display the No. of programs completed or used by the customer 
										if( ProgramLifeCycle > RESET_VALUE)
										{
											SevenSegPrint(ProgramLifeCycle);
										}
										else
										{
											mystrcpy((char *)cdigits_data_array, " 000");
								       			update_display_data(&cdigits_data_array[0] );
										}
										break;
									case 5:
										//5. Fifth press it should display approximate drum RPM (Indicating Either wash or Spin mode)
										//uiRPMValue = (uint16_t)fProcessValue;
										if(uiRPMValue > RESET_VALUE)
										{
											if(uiRPMValue < 405)
											{
												SevenSegPrint(uiRPMValue);
												//condition added to avoid display of 1000 in debug --> 000
												cdigits_data[0] = (cdigits_data[0] & 0x80);//|(0x6D);
												STLED316_7Seg_Write(0xC0,cdigits_data[(ii-0xC0)/2]); // update the display
												
											}
											else if((uiRPMValue > 405)&&(uiRPMValue < 650))
											{
												SevenSegPrint((uiRPMValue-10));
												cdigits_data[0] = (cdigits_data[0] & 0x80);//|(0x6D);
												STLED316_7Seg_Write(0xC0,cdigits_data[(ii-0xC0)/2]); // update the display
											}
											else if((uiRPMValue >= 650)&&(uiRPMValue < 999))
											{
												SevenSegPrint(uiRPMValue);
												cdigits_data[0] = (cdigits_data[0] & 0x80);//|(0x6D);
												STLED316_7Seg_Write(0xC0,cdigits_data[(ii-0xC0)/2]); // update the display
											}
											else if((uiRPMValue >= 1000)&&(uiRPMValue < 1399))
											{
												SevenSegPrint(uiRPMValue);
											}
											else if(uiRPMValue >= 1400)
											{
												SevenSegPrint((uiRPMValue-12));
											}
											
										}
										else
										{
											mystrcpy((char *)cdigits_data_array, " 000");
								       			update_display_data(&cdigits_data_array[0] );
										}
										turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_ON,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
										break;
										
									case 6:
										//1.	First press it should display Software Version S--.
										turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
										
										mystrcpy((char *)cdigits_data_array, IB_TEST_VER);
								       		update_display_data(&cdigits_data_array[0] );
										
									break;
									case 7:
										turnon_display_led(TIMEDOTS_OFF,SEMI_OFF,KG_OFF,RPM_OFF,NOSPIN_OFF,CENTI_OFF,TIMELEFT_OFF);
										turnon_display_OxyJetled(OXYJET_OFF);	//TODO: Turn OFF OxyJet led
										bottom_Array[0]=(uiBot_test_Version/10)+0x30;
										bottom_Array[1]=(uiBot_test_Version%10)+0x30;
										
										mystrcpy((char *)cdigits_data_array, "tP02");
										cdigits_data_array[2]=bottom_Array[0];
										cdigits_data_array[3]=bottom_Array[1];
								       		update_display_data(&cdigits_data_array[0] );
										//SevenSegPrint(ProgramLifeCycle);
									break;
									
								}
						break;
				}
		     break;
			
	    	}
		bDisplay_Flag = FALSE;
	}
}
