#include "ErrorHandling.h"
#include "r_cg_userdefine.h"
#include "STLED316S.h"
//******************Global functions*****************//
//extern void motor();
extern void ComponentSwitchOff();
extern void MotorStop(void);
extern void Emergency_DoorOpen(void);
extern void Delay_ms(uint16_t);
extern void SpinProfileReset();
extern void motor(uint8_t);
extern void DoorError_Check();
extern void SpinProfileReset();
//****************** global variables***********************//
extern boolean bDoorOpenError,bWaterOverFLow,bStart_Traic_Fire,bChildLockButton,bCirculation_Pump;
extern boolean bDoorLock,bMotor_Start,bHeater,bDrainPump,bDoorLockStatus;
extern boolean bPrewashValve,bMainWashValve,bStop_Minutes_Count,bTestMode_Selection;
extern boolean bWaterLevel_Read_Required,bFaciaTest,bFoamDetection;
extern boolean bPressureError,bStartButton,bDoorLocked,bMotorError_E1,bMotorError_E2;
extern boolean bEmegency_Lever_pull,bSpin_Status,bMotorError_E3,bCommunicationError;

extern uint8_t uiSelected_Position,uiNonFatal_Error,uiMachineState,rflag,uiTempSel;
extern uint8_t uiProgram_Position,uiBeforeState,uiTestModeState,Error_Storage[7];
extern uint8_t cdigits_data_array[6],uiError_Mode,uiAdditional_Drain_TurnOn_Mins;
extern uint8_t uiTime_Minutes_Completed,uiTime_Seconds,uihigh_low_vtlg,uiNextStep_Start_Delay;

extern uint8_t PressureSwitchMinutes,uiSilkTumble,uiStep_Minutes_Completed;
extern uint8_t uiEmergencySeconds,uiTumblingState,uiDrain_2min_On;
extern uint16_t BalanceTime,uiCurrent_step_time,uiRPMValue,uicycle_mincompleted;

uint16_t uiFoamSensingCount;
uint8_t uiDrainTimer;
boolean bDrainTimerStart;

extern uint16_t iWater_Washlevel,iWaterLevel,iMotor_Spinning_Seconds;

extern SystemError MachineErrors;

//************************Local variables*************//

uint8_t uiprev_error = 0xFF;
uint8_t uiOverFlowOccurs = 0;
uint8_t ucCount,count1,count2,uiFoamState;

boolean bPressureFailure,bFoamCOuntStart,b2min_Drin_On;

#if ENABLE_IOT
#include "iot_config.h"
#endif

extern uint16_t uiCurrent_step_time;

extern uint8_t FINALSPIN_TIME, ANTICREASE_TIME;
extern boolean bSpinStarted,bFCTMode_Selection;
/****************************************************************************************************
* Function Name: PressureSensorError
* Description  : This function is to perform pressure switch failure Condition and Display Error.
* Arguments    : None
* Return Value : None
********************************************************************************************************/
void PressureSensorError()
{
	uiBeforeState = PRESSUREERROR;
	if(bPressureError == TRUE)
	{
		bDoorLock = SWITCH_ON;
	
		if(bDoorLockStatus == TRUE)
		{
			MachineErrors.EBits.DoorError = 0;
			//cDoor_Lock_sec = 15;
			//bDoorOpenError = FALSE;
			
			if(PressureSwitchMinutes > RESET_VALUE)
			{
				bDrainPump = SWITCH_ON;
				MachineErrors.EBits.PressureError = 1;
			}
			else
			{
				bDrainPump = SWITCH_OFF;
				bPressureError = FALSE; 
			}
		}
		else
		{
			/*if(cDoor_Lock_sec == RESET_VALUE)
			{
				MachineErrors.EBits.DoorError = 1;
				bDoorOpenError = TRUE;
			}*/
			bDrainPump = SWITCH_OFF;
			bPressureError = FALSE;
		}
		uiProgram_Position = uiSelected_Position;
	}
	else
	{
		bDoorLock = SWITCH_ON;
		bDrainPump = SWITCH_OFF;
		bPressureFailure = TRUE;
		bCirculation_Pump = SWITCH_OFF;
		MachineErrors.EBits.PressureError = 1;
	}
}
/*************************************************************************************************
* Function Name: WaterOverFlowCondition
* Description  : This function is to perform WaterOverFlow Condition.
* Arguments    : None
* Return Value : None
*****************************************************************************************************/
void WaterOverFlowCondition()
{
	uiBeforeState = WATERERROR;
		bWaterLevel_Read_Required = 1;
		bHeater =SWITCH_OFF ;
		 if(bWaterOverFLow ==TRUE)
		 {
			bDoorLock = SWITCH_ON;
			if(bDoorLockStatus == TRUE)
			{
				 WaterOverFlow();
				 
				 if((uiOverFlowOccurs > 2)&&(bFaciaTest != TRUE))
				 {
					 MachineErrors.EBits.PressureError = 1;
				 }
				 else if(bFaciaTest == TRUE)
				 {
					 MachineErrors.EBits.PressureError = 1;
				 }
				 else
				 {
					 MachineErrors.EBits.PressureError = RESET_VALUE;
				 }
				 MachineErrors.EBits.DoorError = 0;
				 bDoorOpenError = FALSE;
			}
			else
			{
				
			  /*if(cDoor_Lock_sec == RESET_VALUE)
				{
					MachineErrors.EBits.DoorError = 1;
					bDoorOpenError = TRUE;
					MachineErrors.EBits.PressureError = 0;
				}
				MachineErrors.EBits.PressureError = 1;
				bDrainPump = SWITCH_OFF;
				MachineErrors.EBits.DoorError = 0;
				bDoorOpenError = FALSE;*/
			}
			
		 }
		 else
		 {
			bDoorLock = SWITCH_ON;
			bDrainPump = SWITCH_OFF;
			/*if(bDoorLockStatus == TRUE)
			{
			 	MachineErrors.EBits.DoorError = 0;
			 	bDoorOpenError = FALSE;
			}
			else
			{
				
				if(cDoor_Lock_sec == RESET_VALUE)
				{
					MachineErrors.EBits.DoorError = 1;
					bDoorOpenError = TRUE;
					MachineErrors.EBits.PressureError = 0;
				}
			}*/
			
			
			//if((iWaterLevel < 40400) && (iWaterLevel > 35500))
			if((iWaterLevel < 40400) && (iWaterLevel > 35000)) //for freq betn 35000 to 35500 machine was going in OFEr but seven segment display was blank hence the limit made to 35000
			{
				if((uiOverFlowOccurs >= 2)&&(bFaciaTest == FALSE))
				{
					MachineErrors.EBits.PressureError = 1;
				}
				else if(bFaciaTest == TRUE)
				{
					MachineErrors.EBits.PressureError = 1;
				}
				 else
				{
					MachineErrors.EBits.PressureError = RESET_VALUE;
				}
				bWaterOverFLow = 1;
			}				
		 }
}
/***************************************************************************************************
* Function Name: WaterOverFlow
* Description  : This function is to display WaterOverFlow error.
* Arguments    : None
* Return Value : None
*****************************************************************************************************/
void WaterOverFlow()
{
	bWaterLevel_Read_Required = TRUE;
	if((iWaterLevel >= iWater_Washlevel)&&(uiOverFlowOccurs < 3)&&(bFaciaTest != TRUE))
	{
		bDrainPump = SWITCH_OFF;
		uiOverFlowOccurs ++;
		if(uiOverFlowOccurs <=2 )
		{
			uiMachineState = START_PRESSED;
			bWaterOverFLow = FALSE;
		}
		
	}
	else if((uiOverFlowOccurs > 2)&&(iWaterLevel > EMPTYLEVEL)&&(bFaciaTest != TRUE))
	//if(iWaterLevel > EMPTYLEVEL)
	{
		bDrainPump = SWITCH_OFF;
		uiError_Mode = RESET_VALUE;	
		bWaterOverFLow = 1;
		MachineErrors.EBits.PressureError = 1;
	}
	else if((bFaciaTest == TRUE)&&(iWaterLevel > EMPTYLEVEL))
	{
		bDrainPump = SWITCH_OFF;	
		bWaterOverFLow = 1;
		uiError_Mode = RESET_VALUE;
		MachineErrors.EBits.PressureError = 1;
	}
	
	else
	{
		if(bStartButton == PAUSEPRESSED)
		{
			MachineErrors.EBits.DrainPump = FALSE;
			bDrainPump = SWITCH_OFF;
			uiAdditional_Drain_TurnOn_Mins = RESET_VALUE;
		}
		if(uiAdditional_Drain_TurnOn_Mins >= 7)
		{
			MachineErrors.EBits.DrainPump = 1;
			bDrainPump = SWITCH_OFF;
		}
		else if(bStartButton == STARTPRESSED)
		{
			MotorStop();
			bDrainPump = SWITCH_ON;
			uiError_Mode = EMPTYLEVEL_NOT_ATTAINED;
			bDoorLock = SWITCH_ON;
			bHeater =SWITCH_OFF ;
			bMainWashValve = SWITCH_OFF;
			bPrewashValve = SWITCH_OFF;
			bMotor_Start = NO;
			bWaterOverFLow = TRUE;
			if(bDoorLockStatus == FALSE)
			{
				Emergency_DoorOpen();
			}
		}
		bStop_Minutes_Count = TRUE;
	}
}
/*******************************************************************************************************
* Function Name: MotorFail
* Description  : This function is to detect MotorFail error.
* Arguments    : None
* Return Value : None
*******************************************************************************************************/

void MotorFail()
{	
	uiBeforeState = MOTORFAILURE;
		
	MotorStop();	
	SpinProfileReset();
	bStop_Minutes_Count = YES;
	bHeater = SWITCH_OFF;
	bPrewashValve = SWITCH_OFF;
	bMainWashValve = SWITCH_OFF;
	bMotor_Start = NO;
	bCirculation_Pump = SWITCH_OFF;	// TODO: Addded to avoid cp on in motor error - 23/11/21
	//below condition is added to avoid drain pump on in FCT 
	if(((bMotorError_E1 == TRUE) || (bMotorError_E3 == TRUE)) && (bFCTMode_Selection != TRUE))
	{
		MotorStop();
		if((bDoorLock == SWITCH_ON)&&(uiDrain_2min_On <= 120))
		{	
			bDrainPump = SWITCH_ON;
			b2min_Drin_On = TRUE;
			
		}
		else
		{
			if(bChildLockButton == FALSE)
			{	
				bDoorLock = SWITCH_OFF;
				b2min_Drin_On = FALSE;
			}
			
			bDrainPump = SWITCH_OFF;
		}
	}
	MachineErrors.EBits.MotorError = 1;
}
/**********************************************************************************************************
* Function Name: Store_errors
* Description  : This function is to Store errors in memory .
* Arguments    : uint8_t
* Return Value : None
************************************************************************************************************/
void Store_errors(uint8_t uirecent_error)
{
	uint8_t ii;
	
	
	if(uiprev_error != uirecent_error)
	{
	
		for(ii=6;ii>=1;ii--)
		{
			Error_Storage[ii] = Error_Storage[ii-1];
		}
		Error_Storage[0] = uirecent_error;
		
		uiprev_error = uirecent_error;
	}
}

/****************************************************************************************************
* Function Name: VoltageControl
* Description  : This function is to perform Volatge Error operation.
* Arguments    : None
* Return Value : None
*****************************************************************************************************/
void VoltageControl()
{
	if(bFaciaTest == TRUE)
	{
		bStop_Minutes_Count = TRUE;
	}
	ComponentSwitchOff();
	if(bCommunicationError == FALSE)
	{
		if((uihigh_low_vtlg == 2)&&(bStartButton == FALSE))
		{
			bDoorLock = FALSE;
		}
		else if(bDoorLock == TRUE)
		{
			bDoorLock = TRUE;
		}
	}
		
	bMotor_Start = NO;
	//bSpinTumble = RESET_VALUE;
	//bSpin_Start = RESET_VALUE;
	//cRedistribution_Count = RESET_VALUE;
	//iMotor_Spinning_Seconds = RESET_VALUE;
	MotorStop();
	uiNextStep_Start_Delay = 3;
	SpinProfileReset();
	
}
/**********************************************************************************************************
* Function Name: Read_Errors
* Description  : This function is to Read errors from memory .
* Arguments    : uint8_t
* Return Value : None
************************************************************************************************************/ 
void Read_Errors(uint8_t display_error)
{
	
	switch(display_error)
	{
		case E1: // Heater Error
			mystrcpy((char *)cdigits_data_array, " Htr");
		break;
		case E2: //Over Heat Error
			//mystrcpy((char *)cdigits_data_array, "HEAt");
			mystrcpy((char *)cdigits_data_array, " Hot");
		break;
		case E3: // NTC Error
			mystrcpy((char *)cdigits_data_array, " ntc");
		break;
		case E4: // Overflow Error
			//mystrcpy((char *)cdigits_data_array, "OFEr");
			mystrcpy((char *)cdigits_data_array, " OFL");
		break;
		case E5: // Drain Pump Error
			//mystrcpy((char *)cdigits_data_array, "dPEr");
			mystrcpy((char *)cdigits_data_array, "  dP");
		break;
		case E6: // Doorlock Error
			//mystrcpy((char *)cdigits_data_array, "door");
			mystrcpy((char *)cdigits_data_array, "  do");
		break;
		case E7: // dry tap error
			mystrcpy((char *)cdigits_data_array, " tAP");
		break;
		case E8: // Door locked Error
			//mystrcpy((char *)cdigits_data_array, "dLEr");
			mystrcpy((char *)cdigits_data_array, " dLE");
			break;
		case E9: //Low voltage
			//mystrcpy((char *)cdigits_data_array, "L175");
			mystrcpy((char *)cdigits_data_array, " 175");
			break;
		case E10: //Hall Sensor error
			mystrcpy((char *)cdigits_data_array, "  E1");
			break;
		case E11: //motor OverCurrent
			mystrcpy((char *)cdigits_data_array, "  E2");
			break;
		case E12: //IPM OverHeat
			mystrcpy((char *)cdigits_data_array, "  E3");
			break;
		case E13: //High Voltage
			//mystrcpy((char *)cdigits_data_array, "z260");
			mystrcpy((char *)cdigits_data_array, " 260");
			break;
		case E14: //pressure switch error
			mystrcpy((char *)cdigits_data_array, " PrS");
			break;
		case E15:
			//mystrcpy((char *)cdigits_data_array, "UnbL");
			mystrcpy((char *)cdigits_data_array, " UbL");
		break;
		case E16:
			//mystrcpy((char *)cdigits_data_array, "Conn");
			mystrcpy((char *)cdigits_data_array, " Con");
		break;
		case E17:
			//mystrcpy((char *)cdigits_data_array, "zPrS");
			mystrcpy((char *)cdigits_data_array, " zPr");
		break;
		case E18:
			//mystrcpy((char *)cdigits_data_array, "LPrS");
			mystrcpy((char *)cdigits_data_array, " LPr");
		break;
#if ENABLE_IOT
		case E19:
			mystrcpy((char *)cdigits_data_array, " lot");
		break;
#endif
		case 0x00: 
			mystrcpy((char *)cdigits_data_array, " nEr"); //
			break;
		case 0xFF: 
			mystrcpy((char *)cdigits_data_array, " nEr"); //
			break;
		default: 
			mystrcpy((char *)cdigits_data_array, "    "); // remove this 
			break;
	}
	
	update_display_data(&cdigits_data_array[0]);
	
}

/****************************************************************************************************
* Function Name: FoamSensing
* Description  : This function is to perform Volatge Error operation.
* Arguments    : None
* Return Value : boolean
*****************************************************************************************************/
boolean FoamSensing()
{	static uint16_t iFoamLevel;
	static uint8_t result;
	
	//if((uiSelected_Position == COTTON)&&(uiTempSel >= Wash40E))
	if((uiSelected_Position == COTTON)&&((uiTempSel == Wash40E) || (uiTempSel == Wash60E)))
	{
		iFoamLevel = FOAM_ECOSENSING_FREQUENCY;
	}
	else
	{
		iFoamLevel = FOAM_SENSING_FREQUENCY;
	}	
	switch(uiFoamState)
	{
		case FOAM_SENSING_INITIAL: 
		
				 bFoamCOuntStart = YES;	
			         count1= COUNT_SIX;
				 count2= COUNT_EIGHT;
	      		      
				if ((ucCount < STEP_COUNT)&& (iWaterLevel <= iFoamLevel)&&(iWaterLevel > DANGER_WATERLEVEL)) 
				{	
					if(uiFoamSensingCount > ONE_SECOND)
					{	
						bFoamCOuntStart = YES;
						uiFoamSensingCount = RESET_VALUE;
						MotorStop();
						bMotor_Start = NO;
						uiFoamState = FOAM_SENSING_HALT;	//2509
					//	uiFoamState = FOAM_SENSING_WATER_FILLING;//TBD 2509
						result = 0;
					}
				}
				else if((iWaterLevel < DANGER_WATERLEVEL)||(iWaterLevel > iFoamLevel))
				{
					uiFoamState = FOAM_DELAY;//CONTINUE_SPIN_OPERATION;
					bFoamCOuntStart = YES; //NO; //ADDED TO AVOID STUCK ISSUES IN FOAM SENSE
				}
				else if(ucCount == STEP_COUNT)
				{
					uiFoamState = FOAM_SENSING_SPIN_CANCEL;
					bFoamCOuntStart = NO; 
				}
				result = DECISIONTAKING;
			break;
			
		case FOAM_SENSING_HALT:		//state added by Priyanka 2509
			if(uiRPMValue < 5)
			{ 
			   // Transition to tumbling state
			   uiFoamState = FOAM_SENSING_TUMBLE;
			   uiFoamSensingCount = RESET_VALUE;
			   
			}
			result = FOAM_SENSING_HALT;
		break;
				
		case FOAM_DELAY:          // state added by Pratham bcoz of duty update to motor side 
		      
		        if(uiFoamSensingCount > 4)
			{
				uiFoamSensingCount = RESET_VALUE;
				uiFoamState = CONTINUE_SPIN_OPERATION;
			}
			else
			{
				bDrainPump = SWITCH_OFF;
				bMotor_Start = NO;
			}
		
		break;
		
		case FOAM_SENSING_WATER_FILLING:
			
			bMotor_Start = NO;
			bDrainPump = SWITCH_ON;					
			if((count1 > ZERO_VALUE)&&(iWaterLevel > FOAM_LEVEL_LIMIT))
			{																
				if(uiFoamSensingCount < SIX_SECONDS)
				{
					bMainWashValve = SWITCH_ON;
					bPrewashValve = SWITCH_OFF;																
				}
				else if(uiFoamSensingCount < TEN_SECONDS)
				{
					bPrewashValve = SWITCH_OFF;
					bMainWashValve = SWITCH_OFF;
				}
				else 
				{ 
					count1--;
					uiFoamSensingCount =RESET_VALUE;
				}
									
			}			
			else if(iWaterLevel > FOAM_LEVEL_LIMIT)
			{
				uiFoamSensingCount = RESET_VALUE;
				uiStep_Minutes_Completed = RESET_VALUE;
				uiFoamState = FOAM_SENSING_IDLE;				
			}
			else
			{
				bMainWashValve = SWITCH_OFF;
			}
			result = FOAM_SENSING_WATER_FILLING;
			break;
					
		case FOAM_SENSING_IDLE:				
			
			if(uiFoamSensingCount >60)
			{
				uiFoamSensingCount = RESET_VALUE;
				uiFoamState = FOAM_SENSING_TUMBLE;
			}
			else
			{
				bDrainPump = SWITCH_OFF;
				bMotor_Start = NO;
			}
			result = FOAM_SENSING_IDLE;
			break;
					
		case FOAM_SENSING_TUMBLE:
				
				if(uiFoamSensingCount < FIFTY_ONE_SECONDS)
				{
				    uiTumblingState = 0;
		   		    bMotor_Start = NO;					
					    
				    bDrainTimerStart = TRUE;
				    if(uiDrainTimer < 10)
			            {
					bDrainPump = SWITCH_ON;	
				    }
				    else if(uiDrainTimer < 17)
			            { 
				        bDrainPump = SWITCH_OFF;	
				    }
				    else
				    {
					uiDrainTimer = RESET_VALUE;  
				    }
				}
				else if(uiFoamSensingCount < EIGHTY_ONE_SECONDS)
				{
				    bDrainTimerStart = FALSE;
				    bDrainPump = SWITCH_OFF;
				
				    uiTumblingState = 1;
	   			    bMotor_Start = YES;	
				    motor(DUTY_C14);
				    
				    if(uiFoamSensingCount==80)
				    {
	   			    	bMotor_Start = NO;
					uiTumblingState = 0;
				    }
				}
				else if(uiFoamSensingCount < ONE_HUNDRED_FIFTEEN_SECONDS)
				{
				    //Drain pump control 
				   bDrainTimerStart = TRUE;
				   
				   if(uiDrainTimer < 10)
				   {
					bDrainPump = SWITCH_ON;	
				   }
				   else if(uiDrainTimer < 17)
				   { 
					bDrainPump = SWITCH_OFF;	
				   }
				   else
				   {
					uiDrainTimer = RESET_VALUE;  
				   }
				   
				    uiTumblingState = 1;
	   			    bMotor_Start = YES;	
				    motor(DUTY_W1);
				    
				    if(uiFoamSensingCount==114)
				    {
	   			    	bMotor_Start = NO;
					uiTumblingState = 0;
				    }
				    				    						    
				}
				else if(uiFoamSensingCount < ONE_HUNDRED_FORTY_FIVE_SECONDS)
				{
				     bDrainTimerStart = FALSE;
				     bDrainPump = SWITCH_OFF;
				     
				    uiTumblingState = 1;
	   			    bMotor_Start = YES;	
				    motor(DUTY_C14);
				    
				    if(uiFoamSensingCount==144)
				    {
	   			    	bMotor_Start = NO;
					uiTumblingState = 0;
				    }
				     
				     
				}				
				else if(uiFoamSensingCount < ONE_HUNDRED_SEVENTY_NINE_SECONDS)
				{
				    //Drain pump control 
				   bDrainTimerStart = TRUE;
				   
				   if(uiDrainTimer < 10)
				   {
					bDrainPump = SWITCH_ON;	
				   }
				   else if(uiDrainTimer < 17)
				   { 
					bDrainPump = SWITCH_OFF;	
				   }
				   else
				   {
					uiDrainTimer = RESET_VALUE;  
				   }
				   
				    uiTumblingState = 1;
	   			    bMotor_Start = YES;	
				    motor(DUTY_W1);					  				    
				}				
				else if(uiFoamSensingCount < ONE_HUNDRED_NINETY_FOUR_SECONDS)
				{
			            bDrainTimerStart = FALSE;
				    bDrainPump = SWITCH_ON;
				    uiTumblingState = 0;
	   			    bMotor_Start = NO;
				    
				        // Extra water fill logic...need to verify with spec + Anu
					if(iWaterLevel > FOAM_LEVEL_LIMIT)
					{
					   bMainWashValve = SWITCH_ON;	
					}
					else
					{
						bMainWashValve = SWITCH_OFF;	
						bMotor_Start = NO;
						uiFoamSensingCount = RESET_VALUE;
						ucCount++;
						uiFoamState = FOAM_DELAY;
					
						if(bSpinStarted == TRUE)
						{
							BalanceTime = BalanceTime + uiTime_Minutes_Completed;
							uicycle_mincompleted = uicycle_mincompleted - uiTime_Minutes_Completed;//to avoid subtracting additional time on pause and Power interrupt
							uiCurrent_step_time = uiCurrent_step_time + uiTime_Minutes_Completed;
							uiTime_Minutes_Completed = RESET_VALUE;
							uiTime_Seconds = RESET_VALUE;
						}					
					}
				    
				}
				else if(uiFoamSensingCount < TWO_HUNDRED_TWENTY_FOUR_SECONDS)
				{
					  bMainWashValve = SWITCH_OFF;
					  //Drain pump control 
					   bDrainTimerStart = TRUE;
					   
					   if(uiDrainTimer < 23)
					   {
						bDrainPump = SWITCH_ON;	
					   }
					   else if(uiDrainTimer < 30)
					   { 
						bDrainPump = SWITCH_OFF;	
					   }
					   else
					   {
						uiDrainTimer = RESET_VALUE;  
					   }
					   
					    uiTumblingState = 1;
		   			    bMotor_Start = YES;	
					    motor(DUTY_W1);				
					
				   					
				}
				else if(uiFoamSensingCount < TWO_HUNDRED_SEVENTY_FIVE_SECONDS)
				{
				    bDrainTimerStart = FALSE;
				    bDrainPump = SWITCH_OFF;
				     
				    uiTumblingState = 0;
	   			    bMotor_Start = NO;	
				}
				else if(uiFoamSensingCount < THREE_HUNDRED_NINETY_FIVE_SECONDS)
				{			
					  //Drain pump control 
					   bDrainTimerStart = TRUE;
					   
					   if(uiDrainTimer < 23)
					   {
						bDrainPump = SWITCH_ON;	
					   }
					   else if(uiDrainTimer < 30)
					   { 
						bDrainPump = SWITCH_OFF;	
					   }
					   else
					   {
						uiDrainTimer = RESET_VALUE;  
					   }
					   
					    uiTumblingState = 1;
		   			    bMotor_Start = YES;	
					    motor(DUTY_W1);	
					    
					    if(uiFoamSensingCount==394)
					    {
		   			    	bMotor_Start = NO;
						uiTumblingState = 0;
					    }
				}
				else if(uiFoamSensingCount < FOUR_HUNDRED_TWENTY_FIVE_SECONDS)
				{
					    bDrainTimerStart = FALSE;
				    	    bDrainPump = SWITCH_OFF;
				     
					    uiTumblingState = 1;
		   			    bMotor_Start = YES;	
					    motor(DUTY_C14);
					    
					    if(uiFoamSensingCount==424)
					    {
		   			    	bMotor_Start = NO;
						uiTumblingState = 0;
					    }
					    
					    
					   	
				}
				else if(uiFoamSensingCount < FIVE_HUNDRED_FORTY_FIVE_SECONDS)
				{
					
					  //Drain pump control 
					   bDrainTimerStart = TRUE;
					   
					   if(uiDrainTimer < 23)
					   {
						bDrainPump = SWITCH_ON;	
					   }
					   else if(uiDrainTimer < 30)
					   { 
						bDrainPump = SWITCH_OFF;	
					   }
					   else
					   {
						uiDrainTimer = RESET_VALUE;  
					   }
					   
					    uiTumblingState = 1;
		   			    bMotor_Start = YES;	
					    motor(DUTY_W1);  
				}
				else if(uiFoamSensingCount < FIVE_HUNDRED_SEVENTY_FIVE_SECONDS)
				{
					bDrainTimerStart = FALSE;
				        bDrainPump = SWITCH_OFF;
				     
				        uiTumblingState = 0;
	   			        bMotor_Start = NO;			
				}
				else if(uiFoamSensingCount < SIX_HUNDRED_FIVE_SECONDS)
				{
						     
				        uiTumblingState = 1;
	   			 	bMotor_Start = YES;	
				    	motor(DUTY_C14);
					
					if(uiFoamSensingCount==604)
					    {
		   			    	bMotor_Start = NO;
						uiTumblingState = 0;
					    }
				}
				else if(uiFoamSensingCount < SEVEN_HUNDRED_TWENTY_FIVE_SECONDS)
				{
					//Drain pump control 
					   bDrainTimerStart = TRUE;
					   
					   if(uiDrainTimer < 23)
					   {
						bDrainPump = SWITCH_ON;	
					   }
					   else if(uiDrainTimer < 30)
					   { 
						bDrainPump = SWITCH_OFF;	
					   }
					   else
					   {
						uiDrainTimer = RESET_VALUE;  
					   }
					   
					    uiTumblingState = 1;
		   			    bMotor_Start = YES;	
					    motor(DUTY_W1); 
					    
					    if(uiFoamSensingCount==724)
					    {
		   			    	bMotor_Start = NO;
						uiTumblingState = 0;
					    }
				}
				else if(uiFoamSensingCount < SEVEN_HUNDRED_FIFTY_FIVE_SECONDS)
				{
					bDrainTimerStart = FALSE;
				        bDrainPump = SWITCH_OFF;
					   
					uiTumblingState = 1;
	   			 	bMotor_Start = YES;	
				    	motor(DUTY_C14);	
					
//					if(uiFoamSensingCount==754)
//					    {
//		   			    	bMotor_Start = NO;
//						uiTumblingState = 0;
//					    }
				}
				//else if(uiFoamSensingCount < EIGHT_HUNDRED_SEVENTY_FIVE_SECONDS)
				else if(uiFoamSensingCount < SEVEN_HUNDRED_EIGHTY_FIVE_SECONDS)
				{
					 uiTumblingState = 0;
		   			 bMotor_Start = NO;
					 bDrainTimerStart = FALSE;
				         bDrainPump = SWITCH_ON;
					 
					 if(iWaterLevel > FOAM_LEVEL_LIMIT)
					{
					   bMainWashValve = SWITCH_ON;	
					}
					else
					{
						bMainWashValve = SWITCH_OFF;	
						bMotor_Start = NO;
						uiFoamSensingCount = RESET_VALUE;
						ucCount++;
						uiFoamState = FOAM_DELAY;
					
						if(bSpinStarted == TRUE)
						{
							BalanceTime = BalanceTime + uiTime_Minutes_Completed;
							uicycle_mincompleted = uicycle_mincompleted - uiTime_Minutes_Completed;//to avoid subtracting additional time on pause and Power interrupt
							uiCurrent_step_time = uiCurrent_step_time + uiTime_Minutes_Completed;
							uiTime_Minutes_Completed = RESET_VALUE;
							uiTime_Seconds = RESET_VALUE;
						}					
					}
				}
				else if(uiFoamSensingCount < NINE_HUNDRED_FIVE_SECONDS)
				{
					bMainWashValve = SWITCH_OFF;
					
					bDrainTimerStart = TRUE;
					   
					   if(uiDrainTimer < 23)
					   {
						bDrainPump = SWITCH_ON;	
					   }
					   else if(uiDrainTimer < 30)
					   { 
						bDrainPump = SWITCH_OFF;	
					   }
					   else
					   {
						uiDrainTimer = RESET_VALUE;  
					   }
					   
					    uiTumblingState = 1;
		   			    bMotor_Start = YES;	
					    motor(DUTY_W1);  
					
				}
//				else if(uiFoamSensingCount < NINE_HUNDRED_THIRTY_FIVE_SECONDS)
//				{
//					 bMainWashValve = SWITCH_OFF;
//					 uiTumblingState = 1;
//		   			 bMotor_Start = YES;	
//					 motor(DUTY_W1); 
//					 
//					  //Drain pump control 
//					   bDrainTimerStart = TRUE;
//					   
//					   if(uiDrainTimer < 23)
//					   {
//						bDrainPump = SWITCH_ON;	
//					   }
//					   else if(uiDrainTimer < 30)
//					   { 
//						bDrainPump = SWITCH_OFF;	
//					   }
//					   else
//					   {
//						uiDrainTimer = RESET_VALUE;  
//					   }					   			 
//				}
				else 
				{   
				    bMainWashValve = SWITCH_OFF;
				    bDrainPump = SWITCH_OFF;
				    uiTumblingState = 0;
	   			    bMotor_Start = NO;	
				    bDrainTimerStart = FALSE;
				    uiFoamSensingCount = RESET_VALUE;			
				    ucCount++;
				    uiFoamState = FOAM_DELAY;
				    
				    if(bSpinStarted == TRUE)
				    {
					BalanceTime = BalanceTime + uiTime_Minutes_Completed;
					uicycle_mincompleted = uicycle_mincompleted - uiTime_Minutes_Completed;//to avoid subtracting additional time on pause and Power interrupt
					uiCurrent_step_time = uiCurrent_step_time + uiTime_Minutes_Completed;
					uiTime_Minutes_Completed = RESET_VALUE;
					uiTime_Seconds = RESET_VALUE;
				    }
	 			    				    
				}
		result = FOAM_SENSING_TUMBLE;
		break;
					
		case FOAM_SENSING_SPIN_CANCEL:						
			MotorStop();
			uiFoamState = FOAM_SENSING_INITIAL;
			result = FOAM_SENSING_SPIN_CANCEL;
			bSpin_Status = TRUE;
			bFoamCOuntStart = NO;
			break;
					
		case CONTINUE_SPIN_OPERATION:
			uiFoamSensingCount = RESET_VALUE;
			uiFoamState = FOAM_SENSING_INITIAL;
			bFoamDetection = FALSE;
			SpinProfileReset();
			result = CONTINUE_SPIN_OPERATION;
			bFoamCOuntStart = NO;
			break;
		case STARTSPIN :
			uiFoamState = RESET_VALUE;
			MotorStop();
			SpinProfileReset();
			result = STARTSPIN;
			bFoamCOuntStart = NO;
		default: //No operation required
			break;		
	}
 return result;
}



void FoamCheck()
{
	static uint16_t iFoamLevel;
	//if((uiSelected_Position == COTTON)&&(uiTempSel >= Wash40E))
	if((uiSelected_Position == COTTON)&&((uiTempSel == Wash40E) || (uiTempSel == Wash60E)))
	{
		iFoamLevel = FOAM_ECOSENSING_FREQUENCY;
	}
	else
	{
		iFoamLevel = FOAM_SENSING_FREQUENCY;
	}
	
	// if((uiRPMValue >= 85)&&(uiRPMValue <= 400)&&(iWaterLevel <= iFoamLevel)&&(iWaterLevel > DANGER_WATERLEVEL))
	 if((iWaterLevel <= iFoamLevel)&&(iWaterLevel > DANGER_WATERLEVEL)) //RPM dependent condition removed. made same as Universal and WDR model
	 {
		 bFoamDetection = TRUE;
	 }
	 else if((iWaterLevel > iFoamLevel)&&(uiFoamState == FOAM_SENSING_INITIAL))
	 {
		 bFoamDetection = FALSE;
	 }
}
