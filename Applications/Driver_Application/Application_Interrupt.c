#include "Application_Interrupt.h"
#include "r_cg_intc.h"
#include "r_cg_port.h"
#include "STLED316S.h"
#include "r_cg_timer.h"
#include "r_cg_userdefine.h"

//******************Global functions*****************//
extern void Averaging(void);

//******************Global variables*****************//
extern boolean bEmegency_door_pull,bPrewashValve,bPhysical_DoorLock_Status;
extern boolean bWaterLevel_Read_Required,bCirculation_Pump,bZCD_Display;
extern boolean bMainWashValve,bDrainPump,bDoorLock,bHeater;
				

extern uint8_t ckey_digit_data[6],cdigits_data[7] ;
extern uint8_t uiDoorLockCheck,uiNTCDigital,IB_ADC_Value;
extern uint16_t PressoCount,iWaterFreqSample;
extern uint16_t input3,input4;


//************************Local variables*************//
uint8_t ui_door_pulse_count,ui_prev_door_status,uiDoorPhysicalCheck;

/***********************************************************************************************************************
* Function Name: Application_Interrupt2
* Description  : This function execute ZCD interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Application_Interrupt2()
{
   static uint8_t AnalogSample;
     //*********Averaging of Adc I/P's****************************//
   	if(AnalogSample >= 32) //update after every (32x10 msec)
	{
		Averaging();	
		AnalogSample = RESET_VALUE;
	}
	else
	{ 
		input3 += uiNTCDigital;
		input4 += IB_ADC_Value;
		AnalogSample++;
	}
     //**************************************************************************//	
	
    //************************************************//
    //**********Loads Switch On***********//
    if(bDrainPump == TRUE)
    {
    	DRAINPUMP_ON;
    }
    
    
//***********************************************************//
//**********Door Switch On/Off*******************************//
    if((ui_prev_door_status != bDoorLock)&&(bEmegency_door_pull == FALSE)&&(bPhysical_DoorLock_Status == TRUE)&&(bZCD_Display == TRUE))
    {
	    ui_door_pulse_count++;
	    
	    if(bDoorLock == TRUE)
	    {     
		    if(ui_door_pulse_count <= 2) 
		    {
			    DOORLOCK_ON;    	   
		    }
		    else
		    {
			    DOORLOCK_OFF;
			    ui_prev_door_status = bDoorLock;
			    ui_door_pulse_count = 0x00;
		    }
	    }
	    else if(bDoorLock == FALSE)
	    {
		    if((ui_door_pulse_count <= 2) )
		    {
			    DOORLOCK_ON;    
		    }
		    else
		    {
			DOORLOCK_OFF;   
			ui_door_pulse_count = 0x00;
			ui_prev_door_status = bDoorLock;	     
		    }    
	    }
    }    
//*******************************************************************//
		

}

/***********************************************************************************************************************
* Function Name: Application_Interrupt3
* Description  : This function is INTP3 interrupt to get DoorFeedback.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Application_Interrupt3()
{
	uiDoorLockCheck++;
}

/***********************************************************************************************************************
* Function Name: Application_Interrupt1
* Description  : This function is INTP1 interrupt to get Physical feedback of Door.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Application_Interrupt0()
{
	uiDoorPhysicalCheck++;
	
	 //************************************************//
    	//**********Loads Switch On***********//
	if(bHeater == TRUE)
	{
		HEATER_ON;
	}
	if(bPrewashValve == TRUE)
	{
		PREWASH_VALVE_ON;
	}
	if(bMainWashValve == TRUE)
	{
		MAINWASH_VALVE_ON;
	}
	if(bCirculation_Pump == TRUE)
	{
		C_B_PUMP_ON;
	}
}


/***********************************************************************************************************************
* Function Name: Application_Interrupt0
* Description  : This function is INTP0 interrupt to get WaterLevel.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
/*void Application_Interrupt0()
{
    if(bWaterLevel_Read_Required == YES)
    {
	iWaterFreqSample++;
    }
    else
    {
	iWaterFreqSample = RESET_VALUE;
	PressoCount = RESET_VALUE;
    }
}*/

//**********************************

