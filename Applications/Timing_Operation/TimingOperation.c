#include "TimingOperations.h"
#include "r_cg_userdefine.h"


//**************** global variables *****************************//
extern boolean bDoorLockStatus,bZcdDetection;
extern boolean bMotor_Start,bSpecialEndurance_Finished;
extern uint8_t uiDoorLockCheck,uiDoorPhysicalCheck,uiZcd_Count;
extern uint8_t uiTime_Minutes_Completed,uiTime_Seconds;
extern uint8_t uiDoorLockFalseCount,uiNextStep_Start_Delay;
extern uint16_t BalanceTime,uiCurrent_step_time;

//***** global function prototype *****//
extern void MotorStop();

//***** local Variables *****//
boolean bDoorGone,bSpinStarted,bPhysical_DoorLock_Status;


/*****************************************************************************************************
* Function Name: Time_DoorTriacCheck
* Description  : This function is to check weather door is locked or unlocked.
* Arguments    : None
* Return Value : char
******************************************************************************************************/
void Time_DoorTriacCheck()
{
	if(uiDoorLockCheck > 8)
	{
		bDoorLockStatus = TRUE;
		bDoorGone = FALSE;
	}
	else
	{
		if((uiDoorLockCheck == RESET_VALUE)&&(bZcdDetection == TRUE))
		{
			bDoorGone = TRUE;	
		}
		if((uiDoorLockFalseCount > 0)&&(bZcdDetection == TRUE))
		{
			bDoorLockStatus = FALSE;
			bDoorGone = FALSE;
		}
		//bDoorLockStatus = FALSE;
		MotorStop();
		bMotor_Start = FALSE;
	
		if(bSpinStarted == TRUE)
		{
			BalanceTime = BalanceTime + uiTime_Minutes_Completed;
			uiCurrent_step_time = uiCurrent_step_time + uiTime_Minutes_Completed;
			uiTime_Minutes_Completed = RESET_VALUE;
			uiTime_Seconds = RESET_VALUE;
		}
		//cSpinProfileIndex = 0;
		//cRedistribution_Count = RESET_VALUE;
		if(bSpecialEndurance_Finished == FALSE)
		{
			uiNextStep_Start_Delay = 4;
		}
	}
	uiDoorLockCheck = RESET_VALUE;
	Time_DoorPhysicalLockCheck();
	//*************************************
	
}


void Time_DoorPhysicalLockCheck()
{
	if(uiDoorPhysicalCheck > 12)
	{
		bPhysical_DoorLock_Status = TRUE;
	}
	else
	{
		bPhysical_DoorLock_Status = FALSE;
	}
	uiDoorPhysicalCheck = RESET_VALUE;
	//*************************************
	
}