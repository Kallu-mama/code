#include "TestMode.h"
#include "r_cg_userdefine.h"
#include "STLED316S.h"
#include "Selection.h"


//*************************************************************//
//********************global variables************************//
extern boolean bStart_SfMinutes_Count,bDoorLock,bDoorLockStatus,bMotor_Start;
extern boolean  bDebugMode_Selection,bWaterLevel_Read_Required,bStartButton;

extern uint8_t uiStep_Minutes_Completed,uiPresent_Step,uiNextStep_Start_Delay;
extern uint8_t uiPresent_Step,ckey_digit_data[6],cdigits_data[7],cdigits_data_array[6];
extern uint8_t cFlash_Count,SFState;

//extern uint8_t uiSelectedTone;
extern volatile uint8_t uiSelectedTone;
extern uint8_t uiTempSel;
extern uint8_t uiRpmSel;

extern uint16_t BalanceTime;
extern int cSwitch_Pressed;

//******** global function prototype ******//
extern boolean ProgramAdditivesEndurance();
extern void ProgSel_led();
extern void Display_Message_SF45678();

//**************************************************************//
//********************  Local variables ************************//
uint16_t uiSF6_cyclecompleted;


/*******************************************************************************************************
* Function Name: SpecialFunction6()
* Description  : This function executes SpecialFunction6 Program.
* Arguments    : None
* Return Value : None
*******************************************************************************************************/
void SpecialFunction6()
{
	
	switch(SFState)
		{
		case step1:
			if(bStartButton == STARTPRESSED)
			{
				uiSelectedTone = STARTPRESSTONE;
				bDoorLock = SWITCH_ON;
				SFState = step2;
			}
			break;
		case step2:
			
			if(bStartButton == STARTPRESSED)
			{
				//bDoorLock = SWITCH_ON;
				if(bDoorLockStatus == YES)
				{			
					if(uiNextStep_Start_Delay == 0)
					{
						if(ProgramAdditivesEndurance())
						{
							bMotor_Start = NO;
							bDebugMode_Selection = FALSE;
							MachineState_LED(FALSE,FALSE,FALSE ,FALSE);
							BalanceTime = TRUE;
							if(uiStep_Minutes_Completed >= SIX_MINUTES)
							{
								uiPresent_Step = RESET_VALUE;	
								bStart_SfMinutes_Count = YES;
								bWaterLevel_Read_Required = TRUE;
								uiStep_Minutes_Completed = RESET_VALUE;
								//uiSF6_cyclecompleted++;	
								if(uiSF6_cyclecompleted <= MAX_TEST_MODE_CYCLE)
								{
									uiSF6_cyclecompleted++;	
								}
								else
								{
									uiSF6_cyclecompleted = RESET_VALUE;
								}
								BalanceTime = THIRTY_TWO_MINUTES;//THIRTY_THREE_MINUTES;//THIRTY_SIX_MINUTES;
							}
						}
					}
				}
			}
			else
			{
					
				bStartButton = STARTPRESSED;
				BalanceTime = BalanceTime;
				uiSelectedTone = INAPPLICABLE;
			}
			Display_Message_SF45678();
			break;
		}
		
}
