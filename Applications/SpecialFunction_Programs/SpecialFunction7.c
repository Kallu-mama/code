#include "TestMode.h"
#include "TestMode.h"
#include "r_cg_userdefine.h"
#include "STLED316S.h"
#include "Selection.h"

//**************************************************************//
//********************  Local variables ************************//
uint16_t uiSF7_cyclecompleted;


//*************************************************************//
//********************global variables************************//
extern boolean bStartButton,bStart_SfMinutes_Count,bMotor_Start;
extern boolean bDoorLock,bDoorLockStatus,bDrainPump,bPrewashValve;
extern boolean bDebugMode_Selection,bMotor_Start;

extern uint8_t SFState,uiStep_Minutes_Completed,uiNextStep_Start_Delay;
extern uint8_t uiPresent_Step,ckey_digit_data[6],cdigits_data[7];
extern uint8_t uiTempSel;
extern uint8_t uiRpmSel,uiWaterResetCount;
extern uint8_t cdigits_data_array[6],uiProgram_Position;
//extern uint8_t uiSelectedTone,uiSelected_Position;
extern volatile uint8_t uiSelectedTone;
extern uint8_t uiSelected_Position;

extern uint16_t BalanceTime;
extern int cSwitch_Pressed;
extern uint8_t cFlash_Count;

//******** global function prototype ******//
extern void ProgSel_led();
extern void Display_Message_SF45678();
extern boolean ProgramCotton_60_90();


/********************************************************************************************************
* Function Name: SpecialFunction7()
* Description  : This function executes SpecialFunction7 Program.
* Arguments    : None
* Return Value : boolean
********************************************************************************************************/
void SpecialFunction7()
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
							uiRpmSel = Rpm1200;//Rpm1400; 
							//uiRpmSel = Rpm1200; 
							uiTempSel = Wash60;
							if(ProgramCotton_60_90())
							{	
								bMotor_Start = NO;
								MachineState_LED(FALSE,FALSE,FALSE ,FALSE);
								bDebugMode_Selection = FALSE;
								if(uiStep_Minutes_Completed >= TEN_MINUTES)
								{
									uiPresent_Step = RESET_VALUE;
									bStart_SfMinutes_Count = YES;
									uiStep_Minutes_Completed = RESET_VALUE;
									BalanceTime = COTTON90_TIME+25;
									uiWaterResetCount = RESET_VALUE;
									//uiSF7_cyclecompleted++;
									if(uiSF7_cyclecompleted <= MAX_TEST_MODE_CYCLE)
									{
										uiSF7_cyclecompleted++;	
									}
									else
									{
										uiSF7_cyclecompleted = RESET_VALUE;
									}
									//bDoorLock = SWITCH_ON;
									//uiRpmSel = Rpm1400; 
									uiRpmSel = Rpm1200; 
									SFState = step3;
								}
								else
								{
									BalanceTime = TRUE;
									bStart_SfMinutes_Count = NO;					
								}
							}
							else
							{
								bStart_SfMinutes_Count = NO;
							}
						}
					}
					else
					{
						uiTempSel = Wash60;	
					}
					
				}
				else
				{
					uiSelectedTone = INAPPLICABLE;
					bStartButton = STARTPRESSED;	
				}
				
				
				break;
		case step3:
				
				if(bStartButton == STARTPRESSED)
				{
					//bDoorLock = SWITCH_ON;
					if(bDoorLockStatus == YES)
					{
						if(uiNextStep_Start_Delay == 0)
						{
							uiTempSel = Wash95;
							//uiRpmSel = Rpm1400; 
							uiRpmSel = Rpm1200; 
							if(ProgramCotton_60_90())
							{
								bMotor_Start = NO;
								MachineState_LED(FALSE,FALSE,FALSE ,FALSE);
								bDebugMode_Selection = FALSE;
								if(uiStep_Minutes_Completed >= TEN_MINUTES)
								{
									BalanceTime = COTTON60_TIME+25;
									uiPresent_Step = RESET_VALUE;
									uiWaterResetCount = RESET_VALUE;
									bStart_SfMinutes_Count = YES;
									uiStep_Minutes_Completed = RESET_VALUE;
									//uiSF7_cyclecompleted++;
									if(uiSF7_cyclecompleted <= MAX_TEST_MODE_CYCLE)
									{
										uiSF7_cyclecompleted++;	
									}
									else
									{
										uiSF7_cyclecompleted = RESET_VALUE;
									}
									uiRpmSel = Rpm1200; 
									//uiRpmSel = Rpm1400; 
									//uiSelected_Position = uiProgram_Position = COTTON;
									SFState = step2;
								}
								else
								{
									BalanceTime = TRUE;
									bStart_SfMinutes_Count = NO;
								}
							}
						}
					}
					
				}
				else
				{
					uiSelectedTone = INAPPLICABLE;
					bStartButton = STARTPRESSED;
				}
				break;
		
		default: 
				break;
	}
	
	Display_Message_SF45678();
	
}
//--------------------------------------------------------------------
