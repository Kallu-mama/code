#include "programselection.h"
#include "r_cg_userdefine.h"
#include "Selection.h"

//**************** global variables *****************************//
extern boolean bChildLockButton,bDoorLock,bDebugMode_Selection;//bExpress_30_Activatted;
extern boolean bDoorPulse,bUnbalanceError,bDelay_Finished;
extern boolean bDelay_Process_Start,bRapidWashButton,bStartButton;

extern uint8_t uiSelected_Position,uiPresent_Step;
extern uint8_t uiStep_Seconds,uiStep_Minutes_Completed;
//extern uint8_t uiSelectedTone,PressureSwitchMinutes;
extern uint8_t PressureSwitchMinutes;
extern volatile uint8_t uiSelectedTone;
extern uint8_t uiOverFlowOccurs,uiReheatTime,uiSilkTumble;
extern uint8_t uiMachineState,uiWaterResetCount,uiPrewashWaterResetCount,uiExtraRinse_Press_Select;
extern uint8_t uiTempSel,uiRpmSel,uiSmartProgram_Selected,uiExtraRinse_Press_count;

extern uint16_t uidelaymin,uiCurrent_step_time,snoozemode_10minutes_counter;

//*****global function prototype*****//

extern void ProgramTimming();
extern void RESET_Prog();
extern void Clear_Option_INPause();
extern boolean ProgramMixed();
extern boolean ProgramCottonEco60();
extern boolean ProgramCottonEco40();
extern boolean ProgramAdditivesRinse();
extern boolean ProgramDrainSpin();
extern boolean ProgramWool();
extern boolean Program_ExpressWash();
extern boolean ProgramCotton();
extern boolean ProgramCotton_60_90();
extern boolean ProgramSportsWear();
extern boolean ProgramTubClean();
extern boolean ProgramCradleWash();
extern boolean ProgramCurtains();
extern boolean ProgramDarkWash();
extern boolean ProgramBabyCare();
extern boolean ProgramSynthetic();
extern boolean ProgramAntiAllergen();
extern boolean ProgramUniform();
extern boolean ProgramInnerWear();
extern boolean ProgramSteamFresh();
extern boolean ProgramSteamWash();
extern boolean ProgramJeans();
extern boolean ProgramShirts();
extern boolean Program_ExpressWash_30();
extern boolean TubClean_Activated; // neo 
extern boolean ProgramWetclean();
extern boolean ProgramLinenwash();
//*****local function prototype*****//

boolean bRinsehold_Finished,bSpecialEnduranceSelected,bSpecialEndurance_Finished;
uint8_t  uiRinseholdState;
uint16_t uiSF8_cyclecompleted;


/***********************************************************************************************************************
* Function Name: Programs
* Description  : This function implements execution of Delay option.
* Arguments    : None
* Return Value : char
***********************************************************************************************************************/
boolean DelayStart()
{
	static boolean result;
	
	if(uidelaymin > RESET_VALUE)
	{
		bDelay_Process_Start = YES;
		result = STEP_INPROGRESS;
	}
	else
	{
		bDelay_Process_Start = NO;
		result = STEP_FINISHED;
	}
	return result;
}
/***********************************************************************************************************************
* Function Name: Programs
* Description  : This function implements execution of selected programs.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Programs()
{	
	if(TubClean_Activated == FALSE)
	{
		switch(GET_PROGRAM)
		{
			case COTTON:
					 if((uiTempSel == Wash60)||(uiTempSel == Wash95)||
						(uiTempSel == P_60)||(uiTempSel == P_95))
					{
						if(ProgramCotton_60_90())
						{
							Program_Finished();
						}
					}
					else if((uiTempSel <= Wash40) || (uiTempSel >= P_COLD))
					{
						if(ProgramCotton())
						{
							Program_Finished();
						}
					}
					else if(uiTempSel == Wash40E)
					{
						if(ProgramCottonEco40())
						{
							Program_Finished();
						}
					}
					else if(uiTempSel == Wash60E)
					{
						if(ProgramCottonEco60())
						{
							Program_Finished();
						}
					}
		    	 	break;
			
			case MIXEDSOIL:
					if(ProgramMixed())
					{
						Program_Finished();
					}
				break;
			case CURTAINS :	

					if(ProgramCurtains())
					{
						Program_Finished();
					}
					break;
			case STEAMFRESH:
					if(ProgramSteamFresh())
					{
						Program_Finished();
					}
				break;	
			case BABYWEAR:
					if(ProgramBabyCare())
					{
						Program_Finished();
					}
				break;
			case CRADLEWASH : // CradleWash
					if(ProgramCradleWash())
					{
						Program_Finished();
					}
				break;
			/*case DRAINSPIN:
					if(ProgramDrainSpin())
					{
						Program_Finished();
					}
				break;	*/
			case ADDITIVESRINSE:
				
					if(ProgramAdditivesRinse())
					{
						Program_Finished();		
					}
					break;
				
			case EXPRESS:
//					if(bExpress_30_Activatted == TRUE)
//					{
//						if(ProgramSteamWash())
//						{
//							Program_Finished();
//						}
//					}
//					else
//					{
						if(Program_ExpressWash())
						{
							Program_Finished();
						}
//					}
					
					
				break;
//			case WETCLEAN:
//					if(ProgramWetclean())
//					{
//						Program_Finished();
//					}
//				break;	
			case SYNTHETIC:
					if(ProgramSynthetic())
					{
						Program_Finished();
					}
				break;
			case SMARTPLUS : 
				
					switch(uiSmartProgram_Selected)
					{
						
					case INNERWEAR :

							if(ProgramInnerWear())
							{
								Program_Finished();
							}
							break;
//					case CURTAINS :	
//
//							if(ProgramCurtains())
//							{
//								Program_Finished();
//							}
//							break;
					case DARKWASH :

							if(ProgramDarkWash())
							{
								Program_Finished();
							}
							break;
					case TUBCLEAN :
							if(ProgramTubClean())
							{
								Program_Finished();
							}
							break;
					case SPORTWEAR : 
							if(ProgramSportsWear())
							{
								Program_Finished();	
							}
							break;
					case JEANS : 
							if(ProgramJeans())
							{
								Program_Finished();	
							}
							break;		
					case UNIFORM : 
							if(ProgramUniform())
							{
								Program_Finished();
							}
							break;
					case SHIRTS : 
							if(ProgramShirts())
							{
								Program_Finished();	
							}
							break;	
					case STEAMWASH:
							if(ProgramSteamWash())
							{
								Program_Finished();
							}
							break;
					case LINENWASH:
							if(ProgramLinenwash())
							{
								Program_Finished();
							}
							break;
					case WOOLLENS:
							if(ProgramWool())
							{
								Program_Finished();
							}
							break;
					case ANTIALLERGEN:
							if(ProgramAntiAllergen())
							{
								Program_Finished();
							}
							break;
					case EXPRESS_30 : 
							if(Program_ExpressWash_30())
							{
								Program_Finished();
							}
							break;

					     	default : 
							break;
					} 	
		     	default : 
				break;
				
		}
	}
	else// tubcleAn true
	{
		if(ProgramTubClean())
			{
				Program_Finished();
			}
	}
}

/***********************************************************************************************************************
* Function Name: Program_Finished
* Description  : This function implements program End state .
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Program_Finished()
{
	if(bSpecialEnduranceSelected == TRUE)
	{
		bSpecialEndurance_Finished = TRUE;
		SpecialEnduranceProgram();
	}
	else
	{
		uiMachineState = PROGRAM_END;
	}
}


/***********************************************************************************************************************
* Function Name: RinseHold
* Description  : This function implements execution of RinseHold option.
* Arguments    : None
* Return Value : char
***********************************************************************************************************************/
boolean RinseHold()
{	
	static boolean result;
	switch(uiRinseholdState)
	{
		case 0:			
							
			uiSelectedTone = PAUSETONE;
			bStartButton = PAUSEPRESSED;
			uiMachineState = PAUSE_PRESSED;
			Clear_Option_INPause();
			uiRinseholdState = 2;
			bDebugMode_Selection = FALSE;
			result = STEP_INPROGRESS;
			break;
					
		case 1:
			uiMachineState = START_PRESSED;
			bRinsehold_Finished = TRUE;
			result = STEP_FINISHED;
			break;
		case 2:
			bStartButton = PAUSEPRESSED;
			uiMachineState = PAUSE_PRESSED;
			bDebugMode_Selection = FALSE;
			SpinSelection();
			if((bChildLockButton!=1)&&(bStartButton == STARTPRESSED))
			{
				uiRinseholdState = 1;
			}
			result = STEP_INPROGRESS;
			break;
	}
	return result;
}

/***********************************************************************************************************************
* Function Name: SpecialEnduranceProgram
* Description  : This function implements execution of Special Endurance program.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void SpecialEnduranceProgram()
{
	if(uiStep_Minutes_Completed >= TEN_MINUTES)
	{
		uiPresent_Step = RESET_VALUE;
		bSpecialEndurance_Finished = FALSE;
		uiStep_Minutes_Completed = RESET_VALUE;
		uiWaterResetCount = RESET_VALUE;
		//uiPrewashWaterResetCount = RESET_VALUE;
		if(uiExtraRinse_Press_Select >0)
		{
			uiExtraRinse_Press_count = uiExtraRinse_Press_Select;
		}
		ProgramTimming();
		RESET_Prog();
		TurnOnSelected_Option();
		if(bChildLockButton == FALSE)
		{
			bDoorPulse = FALSE;
		}
		if(uiSF8_cyclecompleted <= MAX_TEST_MODE_CYCLE)
		{
			uiSF8_cyclecompleted++;	
		}
		else
		{
			uiSF8_cyclecompleted = RESET_VALUE;
		}
		//uiSF8_cyclecompleted++;
		//bDoorLock = SWITCH_ON;
	}
	PressureSwitchMinutes=120;
	bUnbalanceError = FALSE;		
	MachineErrors.EReg = RESET_VALUE;
	uiOverFlowOccurs = RESET_VALUE;
	snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT;
	uiCurrent_step_time = RESET_VALUE;
	uiReheatTime = RESET_VALUE;
	//uiSilkTumble = RESET_VALUE;
	bDelay_Finished = FALSE;
}
