/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/G13 V2.05.06.02 [08 Nov 2021]
* Device(s)    : R5F100JJ
* Tool-Chain   : CA78K0R
* Description  : This file implements main function.
* Creation Date: 02-08-2023
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_intc.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "r_cg_wdt.h"
#include "r_cg_lvd.h"
/* Start user code for include. Do not edit comment generated here */
#include "pfdl_types.h"
#include "pfdl.h"
#include "TemperatureCalculation.h"
#include "STLED316S.h"
#include "Selection.h"
#include "My_ram_vector.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

#if ENABLE_IOT
#include "iot_config.h"
#endif
/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

//******************Global functions*****************//
extern void CurrentState(void);
extern void ToneGeneration(void);
extern void PowerOnRead(void);
extern void ProgSel_led(void);
extern void RESET_Prog(void);
extern void FCTMode(void);
extern void SendCommand(void);
extern void DataRecieved(void);
extern void ProgramTimming(void);
extern void Error_Declaration(void);
//******************Local functions*****************//
void display_Selected_options(void);
void Reinit_Stleddevice(void);
void init_Stleddevice(void);
void Alldispled_on(void);
void Alldispled_oFF(void);
void MotorStop(void);

//******************Local variables**********************//

//*****************Door***************//
boolean bDoorLock,bDoorLockStatus;
uint8_t uiDoorLockCheck,uiDoorLOckReleaseSeconds;

//*************** motor control*************************************************//

boolean bMotor_Start,bSpin_Start,bSkip_R1,bTumbleSpecial;
boolean bDutyCycle_Change;

uint8_t cTumbleState;

uint16_t iMotor_Spinning_Seconds;

//***********Water filling**************//
boolean bWaterlevel_Desired,bWaterLevel_Read_Required,bWater_Fill_Completed;
boolean bMainWashValve,bWater_ResetLevel,bPrewashValve,bWaterFillStage,bTwoLitreReached;

uint8_t uiFillingState,uiRinseFillState;
uint8_t uiLowPressureState,uiNoWaterState;
uint8_t uiResetState,uiLowPressureTime,uiNoWaterTime;

uint8_t uiWaterResetCount,uiPrewashWaterResetCount;
uint16_t iRinselevel,iTumble_Waterlevel,iWater_Washlevel,iWaterLevel;
uint16_t iRinseChnageWaterLevel,iWaterFreqSample;

//****************Timming**************//
boolean bStop_Minutes_Count,bHeater,bHalfMinute_Process_Start,bDelay_Process_Start;
boolean bStart_SfMinutes_Count,bCoolDown_Process_Start,bHRecheck_Count_Start;
boolean bRecheck_Count_Start,bHeater_Delay_Start,bDrain_Reset_Seconds;

uint8_t uiRecheck_Seconds,uiDelay_Seconds;
uint8_t uiCoolDown_Seconds,uiNextStep_Start_Delay,uiDrain_Off_Seconds,SfMinutes;
uint8_t uiExtraTime_Seconds,uiHalfMinutes_Seconds,uiStep_Seconds,uiStep_Minutes_Completed,uiheat_Minutes_Completed,uiheat_Seconds_Completed;
uint8_t uiHalfMinute_Completed,uiAdditional_Heater_TurnOn_Mins,uiHeater_TurnOn_Delay_Value;
uint8_t uiAdditional_Drain_TurnOn_Mins,uiFatal_Minutes_Completed,uiWaterOverFlowSeconds;

uint8_t uiStartUpdelay,uiStartUpCount,SatrtUpDelay = 2;
uint8_t PressureSwitchMinutes = 120,HeatMinutes,uiHRecheck_Seconds;
uint8_t uiMotorDrainMinutes,uiReheatTime,uiMachineStart_count = 150;
uint16_t iMilliSeconds;
uint16_t uidelaymin;

//****************Error Condition****************************//
boolean bWaterOverFLow,bPressureSwitchFail,bPressureError,bNoWater;
boolean bHeaterOpen,bNtcFailed;

uint8_t uiNonFatal_Error,uiError_Mode,uiError_Occured;

SystemError MachineErrors;
//***************Adc Operation********************//
uint8_t uiAveragingSec;
uint8_t uiNTCDigital,IB_ADC_Value;
uint16_t uiSupplyVoltage;
uint8_t uiTemperature;

//******************Program State*************************//
boolean bDrainPump,bCirculation_Pump; //loads

boolean bStep_Finished,bProgram_Ended,bWashProgram_Selected,bProgram_Start;
boolean bProgramStart,bTestSpinValueRead,bTESTMODE_START;
boolean bTestTempRead,bDebugMode_Selection,bTestMode_Selection;
boolean bFctCountStart,bFCTMode_Selection;    // FCT mode

uint8_t uiFCTMode,uiTFCTSeconds,uiFCT_SW_Test;   //Fct Mode
uint8_t uiTumbleState,uiStepState,uiHeatState,uiDrainState,uiFaciaTest,FaciaState,uiBeforeState;
uint8_t uiMachineState,uiDebugState,uiMachineTestState,SFState,uiTestModeState;

uint16_t ProgramLifeCycle;

uint8_t uiTemperature_Required,uiStartTemperature,TempModeTemperature;

//**************Key/Encoder/OptionSelection Operation*************//
boolean bPressConfirmStart,bSwitch_Read_Allow,bDetectionCompleted;
boolean bStartButton,bRapidWashButton,bChildLockButton,bKeyDetected;
boolean bOption_DelayStart_Selected,bExtraRinse_Selection,bPrewash_Selection;
boolean bPrewash_Start,bSoak_Start,bHotRinse_Start,bExtraRinse_Start;
boolean bSteam_Start,bDelay_Start1,bTimeSaver_Start;

uint8_t uiSelected_Position,uiPresent_Step,uiPrevious_Position;
uint8_t uiEncoderValue,uiEncoderDirection,uiButtonPressConfirm;
uint8_t uiProgram_Position,ckey_digit_data[6],cdigits_data_array[6];

uint8_t rot_sec,encodercofirm;
uint8_t uLedKeyData[3];
uint8_t uikey_debounce_delay = 10;
uint16_t uikeypress_20mscount;
uint16_t uiprev_key_data;
int cSwitch_Pressed;

//****************** Display Operation -------------//
boolean bDisplay_Flag;
boolean default_Set,chip_select;

uint8_t cFlash_Count;
uint8_t cdigits_data[7],Error_Storage[7];


//option variable
uint8_t uioption_on,uidisp_select_option;
uint8_t uiPSER_Option_Avbl = 0xFF; //
uint8_t uitimesaver_delay_start; //
uint8_t uimaxspeed_temp;
uint8_t uiallon_count;

//******************Memory BackUp*****************//
boolean bLowVoltage_Detected;

uint8_t ucWrite_DataBuffer[MemoryLength] ;	//Write data buffer array for data flash write operations 
uint8_t ucRead_DataBuffer[MemoryLength] ; //Read data buffer array for Read and store the data

unsigned int uiFDL_VersionChk;
pfdl_descriptor_t FDL_SysInit = {32,0};	//CPU clock 8MHz is used & Full fSetSpeed mode selected as programming voltage mode
pfdl_status_t FDL_Status ;	// Declaration of FDL_Status with initial value as PFDL_IDLE
pfdl_request_t FDL_Request ;

//****************Buzzer Operation*******************//
boolean buzzer_on,bBuzzerStart;
boolean buzzerTone,buzzerTone1,buzzerTone2;
boolean buzzerTone3,buzzerTone4,buzzerTone5,buzzerTone6;
boolean buzzerTone7,buzzerTone8,buzzerTone9,buzzerTone10,buzzerTone11,buzzerTone14;

//uint8_t uiToneFreqSate,uiSelectedTone;
volatile uint8_t uiToneFreqSate,uiSelectedTone;
uint8_t buzzercount,buzzerarray;
uint16_t BuzzerFreq,mybuzzerdelay;
//******************************************************//

uint8_t uiRESET;
uint8_t Rapid;
uint8_t uiRinse_Reset_Count;
uint8_t uiChildLockDelay;
uint16_t PressoCount;
boolean bEnduranceMode;
uint8_t resource,uiVoltageAveraging;
uint16_t uiIdleNotSelected;
boolean bIdle;
uint8_t ii;
uint16_t Display_off_testing;
#if ENABLE_IOT
void  IOT_CommHandler(void);
void initIOTCommuncation(void);
#endif
//-----------------------------------------
extern boolean bZcdDetection,bZCD_Display,Send_flag,bDataRecieved,bkeys_display_flag,bCommunicationError;
extern volatile uint16_t  g_uart1_tx_count;
extern uint8_t uiStateCheck;
uint8_t Rx_Buffer[1];
extern void adc_interrupt(void);
extern void tau0_channel3_interrupt(void);
extern void intc3_interrupt(void);
extern void intc1_interrupt(void);
extern void intc0_interrupt(void);
extern void lvd_interrupt(void);
extern void tau0_channel0_interrupt(void);
extern void tau0_channel1_interrupt(void);
extern void uart1_interrupt_receive(void);
extern void uart1_interrupt_send(void);
extern void uart2_interrupt_receive(void);
extern void uart2_interrupt_send(void);
extern void wdt_interrupt(void);

/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
    RAM_INTTM00_ISR = &r_tau0_channel0_interrupt;
    RAM_INTTM01_ISR = &r_tau0_channel1_interrupt;
    RAM_INTTM02_ISR = &r_tau0_channel2_interrupt;
    RAM_INTTM03_ISR = &r_tau0_channel3_interrupt;
    RAM_INTP0_ISR = &r_intc0_interrupt;
    RAM_INTP6_ISR = &r_intc6_interrupt;
    RAM_INTAD_ISR = &r_adc_interrupt;
    RAM_INTWDTI_ISR= &r_wdt_interrupt;
    RAM_INTSR1_ISR = &r_uart1_interrupt_receive;
    RAM_INTST1_ISR = &r_uart1_interrupt_send;
    RAM_INTSR2_ISR = &r_uart2_interrupt_receive;
    RAM_INTST2_ISR = &r_uart2_interrupt_send;
    RAM_INTLVI_ISR = &r_lvd_interrupt;
    
// 	RAM_INTAD_ISR = &adc_interrupt;
//	RAM_INTTM03_ISR = &tau0_channel3_interrupt;
//	RAM_INTP3_ISR = &intc3_interrupt;
//	RAM_INTP1_ISR = &intc1_interrupt;
//	RAM_INTP0_ISR = &intc0_interrupt;
//	RAM_INTLVI_ISR = &lvd_interrupt;
//	RAM_INTTM00_ISR = &tau0_channel0_interrupt;
//	RAM_INTTM01_ISR = &tau0_channel1_interrupt;
//	RAM_INTSR1_ISR = &uart1_interrupt_receive;
//	RAM_INTST1_ISR = &uart1_interrupt_send;
//	RAM_INTSR2_ISR = &uart2_interrupt_receive;
//	RAM_INTST2_ISR = &uart2_interrupt_send;
//	RAM_INTWDTI_ISR = &wdt_interrupt;
     	R_ADC_Start(); 		//  start the AdC after proper power on
	PowerOnRead(); 		// restore the prevous values
	bWaterLevel_Read_Required = TRUE;
     	R_INTC0_Start(); 	// Door Feedback intp
     	R_INTC6_Start(); 	// Physical door closed intp
	R_TAU0_Channel1_Start(); // 1msec timer
	//STB1_LOW; time being
	Delay_ms(300);	// this delay time due to some problem in the SMPS getting 3.7v after every 7-8 sec (on time 50-60msec) 
    	//bZCD_Display = TRUE;
	R_LVD_InterruptMode_Start(); // on low voltage interrupt on 
	init_Stleddevice(); // initialize the stled ic
	//bSpin_Available = TRUE; // make spin availbale for module 
	if((bStartButton != TRUE)&&(bTestMode_Selection == FALSE))
	{
		Alldispled_oFF(); // power on condition
	}
	else
	{
		if((bTestMode_Selection == FALSE)&&(uiMachineState != PROGRAM_END))
		{
			TurnOnSelected_Option();
		}	
	}
	R_UART1_Start();
	R_UART1_Receive(Rx_Buffer ,1);
	R_TAU0_Channel0_Start(); // 
	R_TAU0_Channel2_Start(); // 20msec timer (key scan , display update , adc calculation)
	
//	R_TAU0_Channel3_Start(); // buzzer 	//commented for intial dual buzzer tone - AK
	Delay_ms(10);
	
	MotorStop();   
	
	//R_UART1_Receive(Rx_Buffer ,1);
	
	
#if ENABLE_IOT
	R_UART2_Start();
	initIOTCommuncation();
#endif	
 	while(SatrtUpDelay != RESET_VALUE)
	{
		//bZCD_Display = TRUE;
		if(buzzerTone7 == FALSE)
		{
			uiSelectedTone = STARTUPTONE;
			buzzerTone7 = TRUE;
		}
		uiStartUpCount = RESET_VALUE;
		ToneGeneration();
	}
	TemperatureCalculation(uiNTCDigital,&uiTemperature);
    	while (1U)
   	{
		
		if(bZCD_Display == TRUE)
		{
       			ToneGeneration();
		}
		else
		{
			P6.3 = RESET_VALUE;
		}
		// data transmit
		if(Send_flag == TRUE)
		{
			if(g_uart1_tx_count == 0x00) // Avoid while transmitting
	    		{
				SendCommand();
				Delay_ms(10);
				Send_flag = FALSE;
			}
		}
		// check data receive
		if(bDataRecieved == TRUE)
		{
			DataRecieved();
			bDataRecieved = FALSE;
		}
		if(((bZcdDetection == TRUE)||(bCommunicationError == TRUE))&&(uiStateCheck == RESET_VALUE))
		{
			CurrentState();
		}
		
#if ENABLE_IOT
       		IOT_CommHandler();
#endif
		if(bkeys_display_flag == TRUE)
		{
			KEYS_DISPLAY();//time being
			//STLED316_7Seg_Write(0xC0,0xFF);
		//	STLED316_7Seg_Write(0xC2,0xFF);
		//	STLED316_7Seg_Write(0xC4,0xFF);
		//	STLED316_7Seg_Write(0xC6,0xFF);

			//STLED316_DisplayOn(); // display on command 
			
			if(bZCD_Display == TRUE)
				Error_Declaration();
		}
		R_WDT_Restart();
   	}
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    bWaterLevel_Read_Required = TRUE;
    EI();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
void init_Stleddevice(void)
{
	chip_select = FALSE; 	
	//6964 initialization function
	Initialise_6964();
}

void Alldispled_oFF(void)
{
	//clear the all display data
	for(ii = 0; ii<7;ii++)
	{
    		ckey_digit_data[ii] = 0x00;
		cdigits_data[ii] = 0x00;
	}
	
	uiProgram_Position = uiSelected_Position;
	if(uiMachineState != POWEROFF)
		ProgSel_led();
		
	RESET_Prog();
	//------------- to show the selected options --------------
	TurnOnSelected_Option();
	//-------------
	ProgramTimming();
}
void Reinit_Stleddevice(void)
{
	chip_select = FALSE; 
//	for(ii = 0xC0; ii<0xC7;ii++)
//	{
//		
//		STLED316_7Seg_Write(ii,cdigits_data[ii]); // this is should not be 0x00 , default array have to written
//	}
	STLED316_setDigits_seg_cnt();
	for(ii = 0xC0; ii < 0xCD; ii= ii + 0x02)
	{
		STLED316_7Seg_Write(ii,cdigits_data[(ii-0xC0)/2]); // update the display
	}
	STLED316_7Seg_Write(0XCD,ckey_digit_data[5]);
	for(ii = 0xC1; ii < 0xCA; ii= ii + 0x02)
	{
		STLED316_7Seg_Write(ii,ckey_digit_data[(ii-0xC1)/2]); // this is should not be 0x00 , default array have to written
					
	}
	//STLED316_7Seg_Write(1,0x03);
	// Send ther display on condition 
	STLED316_DisplayOn(); 
	//------------- End of config ---------------------------	
}

void MotorStop()
{
	bMotor_Start = NO;
}
/* End user code. Do not edit comment generated here */
