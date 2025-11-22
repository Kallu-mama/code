#include "backup.h"
#include "pfdl_types.h"
#include "pfdl.h"
#include "r_cg_userdefine.h"
#include "String.h"


//******************Global functions*****************//
extern void ProgSel_led();
extern void ProgramTimming();
extern void StopAllEvents();
//******************Global variables*****************//

extern pfdl_descriptor_t FDL_SysInit;	//CPU clock 8MHz is used & Full fSetSpeed mode selected as programming voltage mode
extern pfdl_status_t FDL_Status ;	// Declaration of FDL_Status with initial value as PFDL_IDLE
extern pfdl_request_t FDL_Request ;	// Declaration of FDL_Request with initial value

extern boolean bWashProgram_Selected,bLevelReached,bDoorLock,bDoorLockStatus,bCommunicationError;//bExpress_30_Activatted;
extern boolean bTESTMODE_START,bStartButton,bSpecialEnduranceSelected,bEmegency_door_pull,bSteam_Start;
extern boolean bProgramStart,bTestMode_Selection,bHeaterOpen,bDelay_Start,bChildLockButton,bSpecialEndurance_Finished;
extern boolean bReheatHeater,bSpinTUmbleStart,bDoorLockError,bRapidWashButton,bDoorUnlocked,bCold_Soak_Selected;
extern boolean bNoWater,bEnduranceMode,bPressureError,bTwoLitreReached,bFCTMode_Selection,bEco60Reset;
extern boolean bPrewash_Start,bSoak_Start,bHotRinse_Start,bExtraRinse_Start,bTimeSaver_Start,bDelay_Finished;
extern boolean step_start,bSpinStarted,bWaterOverFLow,bOverHeat,bStep_Flag,bFaciaTest,bTest_Forward;
extern boolean bDoorLock,bHeatCompleted,uiover_heat,bProgram_Start,bRinsehold_Finished,TubClean_Activated;
extern boolean bDoorOpenError,bUnbalanceError,bEnduranceStarted,bDelay_Start1,uiReset_TempRPM ,Switch_Reset ;
extern boolean bMotorError_E3,bMotorError_E2,bMotorError_E1,bZcdDetection,bHotRinse_Selected,bHPrs,bLPrs,uiUB_decidation;
extern boolean bLoadSensing_Completed;

extern uint8_t uiStep_Minutes_Completed,uiPresent_Step;
extern uint8_t uiSelected_Position,uiStep_Seconds,uiFillingState,uiMachineTestState;
extern uint8_t uiProgram_Position,uiMachineState,uiRinseFillState,uiLowPressureTime,uiAdditional_Drain_TurnOn_Mins;
extern uint8_t uiHeatState,uiDrainState,uiTumbleState,uiAdditional_Heater_TurnOn_Mins,uiNoWaterTime,SFState,uiExtraTime_Seconds;
extern uint8_t uiHalfMinutes_Seconds,uiHalfMinute_Completed,uiTestModeState,FaciaState;
extern uint8_t uiTime_Minutes_Completed,Step_pos;
extern uint8_t uiFCTMode,uiBeforeState,uiDoorUnlockPulseCount;
extern uint8_t cdigits_data[7] ,ckey_digit_data[6];
extern uint8_t Error_Storage[7];
extern uint8_t uiTime_Seconds;
extern uint8_t prev_program_pos; 
extern uint8_t uiRinseholdState;
extern uint8_t uisnooze_mode_pos;
extern uint8_t uiLS_Total_subtraction, uiLS_Tx_subtraction, uiFinalWt,uiLoadRange;

extern unsigned char ucWrite_DataBuffer[MemoryLength]; //Write data buffer array for data flash write operations 	
extern unsigned char ucRead_DataBuffer[MemoryLength] ; //Read data buffer array for Read and store the data
extern unsigned int uiFDL_VersionChk; //buffer used to store the bStartButton address where the FDL version details located.


extern uint8_t uiWaterResetCount,uiReheatTime,HeatMinutes,uimaxspeed_temp,uiPrewashWaterResetCount;//uiResetFill_Count;
extern uint8_t uiOverFlowOccurs,uiStartTemperature,uiHeaterCheck;
extern uint16_t uicycle_mincompleted,uiSF8_cyclecompleted,uiSF6_cyclecompleted;
extern uint8_t uiExtraRinse_Press_count,uiSoak_Press_count ;
extern uint8_t uiMachineBefore_Poweroff,uiSmartProgram_Selected;
extern uint8_t uiExtraRinse_Press_Select,uilast_selected_option,uiprev_error;
extern uint8_t uiDoorLOckReleaseSeconds,uimachine_shutdown;
extern uint8_t uiPSER_Option_Avbl, uidisp_select_option,uiExtraRinse_Max;
extern uint8_t uiRMP_Default,ui_prev_door_status;
extern uint8_t uiRpmSel ,uiTempSel ,uidefault_kg ,uiOption_closed,uiTest_drain,uiheat_temp;
extern uint8_t uioption_on;
extern uint8_t uihigh_low_vtlg;
extern uint16_t ProgramLifeCycle,uispin_cyclecompleted,uiSF7_cyclecompleted;
extern uint16_t snoozemode_10minutes_counter;
extern uint16_t uiIdleNotSelected,uiCurrent_step_time;
extern uint16_t ucMotor_Rotate_Seconds,uiRPMValue;
extern uint16_t BalanceTime,uidelaymin;
extern uint16_t iWaterLevel;
extern uint8_t uiTemperature;
extern uint8_t uiTubClean_req ,uiTubClean_Disp_count, uiTubClean_Disp,uitubclean_cycle;
//************************Local variables*************//
uint8_t uiprev_reset_pos;
uint8_t uiprogram_option_status;
#if ENABLE_IOT
extern uint16_t ProgramTime;
extern uint8_t BkMultiFuncSetting,WiFi_Comn_Test, L175_count;
#include "iot_config.h"
#endif
/***********************************************************************************************************************
* Function Name: LowVoltage
* Description  : This function is Stores the Value in EEPROM .
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void LowVoltage()
{
	DI();

	uiprogram_option_status = 0x00;
	
	if(bPrewash_Start == TRUE) 
	{
		uiprogram_option_status |=0x01;
	}
	if(bSoak_Start == TRUE)
	{
		uiprogram_option_status |=0x02;
	}
	if(bExtraRinse_Start == TRUE)
	{
		uiprogram_option_status |=0x04;
	}
	if( bSteam_Start  == TRUE)
	{
		uiprogram_option_status |=0x08;
	}
	if(bDelay_Start1 == TRUE)
	{
		uiprogram_option_status |=0x10;
	}
	if(bTimeSaver_Start == TRUE)
	{
		uiprogram_option_status |=0x20;	
	}
	
	ucWrite_DataBuffer[0] = bStartButton;
	ucWrite_DataBuffer[1] = bWashProgram_Selected;
	ucWrite_DataBuffer[2] = uiStep_Minutes_Completed;
	ucWrite_DataBuffer[3] = uiPresent_Step;		
	ucWrite_DataBuffer[4] = bRapidWashButton;
	ucWrite_DataBuffer[5] = uiRinseholdState;
	ucWrite_DataBuffer[6] = uiStep_Seconds;
	ucWrite_DataBuffer[7] = uiFillingState;
	ucWrite_DataBuffer[8] = uiRinseFillState;
	ucWrite_DataBuffer[9] = uiHeatState;
	
	ucWrite_DataBuffer[10] = uiDrainState;
	ucWrite_DataBuffer[11] = uiTumbleState;
	ucWrite_DataBuffer[12] = bProgramStart;
	ucWrite_DataBuffer[13] = uiAdditional_Heater_TurnOn_Mins;
	ucWrite_DataBuffer[14] = uiLowPressureTime;
	ucWrite_DataBuffer[15] = uiNoWaterTime;
	ucWrite_DataBuffer[16] = uiAdditional_Drain_TurnOn_Mins;
	ucWrite_DataBuffer[17] = uiWaterResetCount;
	ucWrite_DataBuffer[18] = uiHalfMinutes_Seconds;
	ucWrite_DataBuffer[19] = uiHalfMinute_Completed;
	ucWrite_DataBuffer[20] = uiTestModeState;
	
	ucWrite_DataBuffer[21] = FaciaState;
	ucWrite_DataBuffer[22] = uiMachineTestState;
	ucWrite_DataBuffer[23] = bFaciaTest;
	ucWrite_DataBuffer[24] = bTestMode_Selection;
	ucWrite_DataBuffer[25] = bChildLockButton;
	ucWrite_DataBuffer[26] = bNoWater;
	ucWrite_DataBuffer[27] = bEnduranceMode;
	ucWrite_DataBuffer[28] = uiFCTMode;
	ucWrite_DataBuffer[29] = uiBeforeState;
	ucWrite_DataBuffer[30] = SFState;
	
	ucWrite_DataBuffer[31] = uiExtraTime_Seconds;
	ucWrite_DataBuffer[32] = bSpecialEndurance_Finished;
	ucWrite_DataBuffer[33] = bCommunicationError;
	ucWrite_DataBuffer[34] = uiReheatTime;
	ucWrite_DataBuffer[35] = bPressureError;
	ucWrite_DataBuffer[36] = uiHeaterCheck;
	ucWrite_DataBuffer[37] = uiDoorUnlockPulseCount;
	ucWrite_DataBuffer[38] = bReheatHeater;
	ucWrite_DataBuffer[39] = uiSmartProgram_Selected;
	ucWrite_DataBuffer[40] = HeatMinutes;
	
	ucWrite_DataBuffer[41] = bHeaterOpen;
	ucWrite_DataBuffer[42] = bTwoLitreReached;
	
	if(bSpinStarted == TRUE)
	{
		BalanceTime = BalanceTime + uiTime_Minutes_Completed;
		uiCurrent_step_time = uiCurrent_step_time + uiTime_Minutes_Completed;
		uiTime_Minutes_Completed = RESET_VALUE;
		uiTime_Seconds = RESET_VALUE;
	}
	ucWrite_DataBuffer[43] = (uint8_t)(BalanceTime & 0x00FF);
	ucWrite_DataBuffer[44] = (uint8_t)(BalanceTime >> 8);
	ucWrite_DataBuffer[45] = (uint8_t)(uidelaymin & 0x00FF);
	ucWrite_DataBuffer[46] = (uint8_t)(uidelaymin >> 8);
	ucWrite_DataBuffer[47] = (uint8_t)(uicycle_mincompleted & 0x00FF);
	ucWrite_DataBuffer[48] = (uint8_t)(uicycle_mincompleted >> 8);
	ucWrite_DataBuffer[49] = uiprogram_option_status;
	ucWrite_DataBuffer[50] = bTESTMODE_START;
	
	ucWrite_DataBuffer[51] = uiOverFlowOccurs;
	ucWrite_DataBuffer[52] = (uint8_t) (uiCurrent_step_time & 0x00FF);
	ucWrite_DataBuffer[53] = (uint8_t)(uiCurrent_step_time >> 8);
	ucWrite_DataBuffer[54] = uiStartTemperature;
	ucWrite_DataBuffer[55] = uiTime_Seconds;
	ucWrite_DataBuffer[56] = uiTime_Minutes_Completed;
	ucWrite_DataBuffer[57] = bSpinStarted;	
	ucWrite_DataBuffer[58] = bHeatCompleted;
	ucWrite_DataBuffer[59] = bTest_Forward;
	ucWrite_DataBuffer[60] = uiover_heat;
	
	ucWrite_DataBuffer[61] = uiDoorLOckReleaseSeconds;	
	ucWrite_DataBuffer[62] = bWaterOverFLow;
	ucWrite_DataBuffer[63] = bEnduranceStarted;
	ucWrite_DataBuffer[64] = bUnbalanceError;
	ucWrite_DataBuffer[65] = bDoorOpenError;	
	ucWrite_DataBuffer[66] = (uint8_t)(snoozemode_10minutes_counter & 0x00FF);
	ucWrite_DataBuffer[67] = (uint8_t)(snoozemode_10minutes_counter >> 8);
	ucWrite_DataBuffer[68] = bRinsehold_Finished ;
	ucWrite_DataBuffer[69] = uiExtraRinse_Max ;
	ucWrite_DataBuffer[70] = bOverHeat ;
	
	ucWrite_DataBuffer[71] = uiOption_closed ;
	ucWrite_DataBuffer[72] = bStep_Flag;
	ucWrite_DataBuffer[73] = bLevelReached;
	ucWrite_DataBuffer[74] = uihigh_low_vtlg;
	ucWrite_DataBuffer[75] = bFCTMode_Selection;
	ucWrite_DataBuffer[76] = bDelay_Finished;
	ucWrite_DataBuffer[77] = bSpecialEnduranceSelected;
	ucWrite_DataBuffer[78] = bEmegency_door_pull;
	ucWrite_DataBuffer[79] = bEco60Reset;
	ucWrite_DataBuffer[80] = Step_pos;
	
	ucWrite_DataBuffer[81] = bMotorError_E1;
	ucWrite_DataBuffer[82] = bMotorError_E2;
	ucWrite_DataBuffer[83] = bMotorError_E3;
	
	ucWrite_DataBuffer[84] = (uint8_t)(uispin_cyclecompleted & 0x00FF);
	ucWrite_DataBuffer[85] = (uint8_t)(uispin_cyclecompleted >> 8);
	ucWrite_DataBuffer[86] = (uint8_t)(ProgramLifeCycle & 0x00FF);
	ucWrite_DataBuffer[87] = (uint8_t)(ProgramLifeCycle >> 8);
	ucWrite_DataBuffer[88] = (uint8_t)(uiSF7_cyclecompleted & 0x00FF);
	ucWrite_DataBuffer[89] = (uint8_t)(uiSF7_cyclecompleted >> 8);
	ucWrite_DataBuffer[90] = (uint8_t)(uiSF6_cyclecompleted & 0x00FF);
	ucWrite_DataBuffer[91] = (uint8_t)(uiSF6_cyclecompleted >> 8);
	
	ucWrite_DataBuffer[92] = (uint8_t)(uiSF8_cyclecompleted & 0x00FF);
	ucWrite_DataBuffer[93] = (uint8_t)(uiSF8_cyclecompleted >> 8);
	ucWrite_DataBuffer[94] = uiSelected_Position ;
	ucWrite_DataBuffer[95] = uiMachineState;
	ucWrite_DataBuffer[96] = uiRpmSel;
	ucWrite_DataBuffer[97] = uiTempSel;
	ucWrite_DataBuffer[98] = uiTubClean_req;
	ucWrite_DataBuffer[99] = uiTubClean_Disp_count;
	ucWrite_DataBuffer[100] = uitubclean_cycle;
	
	ucWrite_DataBuffer[101] = uiPSER_Option_Avbl;
	ucWrite_DataBuffer[102] = uilast_selected_option ; // uidisp_select_option;
	ucWrite_DataBuffer[103] = Error_Storage[0];
	ucWrite_DataBuffer[104] = Error_Storage[1];
	ucWrite_DataBuffer[105] = Error_Storage[2];
	ucWrite_DataBuffer[106] = Error_Storage[3];
	ucWrite_DataBuffer[107] = Error_Storage[4];
	ucWrite_DataBuffer[108] = Error_Storage[5];
	ucWrite_DataBuffer[109] = Error_Storage[6];
	ucWrite_DataBuffer[110] = prev_program_pos;
	
	ucWrite_DataBuffer[111] = uiRMP_Default;
	ucWrite_DataBuffer[112] = uimaxspeed_temp;
	ucWrite_DataBuffer[113] = bDoorLockError;
	ucWrite_DataBuffer[114] = uiExtraRinse_Press_Select;
	ucWrite_DataBuffer[115] = TubClean_Activated;
	ucWrite_DataBuffer[116] = uiprev_error;
	ucWrite_DataBuffer[117] = bDoorLock;
	ucWrite_DataBuffer[118] = ui_prev_door_status;
	ucWrite_DataBuffer[119] = uiExtraRinse_Press_count;
	ucWrite_DataBuffer[120] = bDoorUnlocked;
	ucWrite_DataBuffer[121] = uiMachineBefore_Poweroff ;
	ucWrite_DataBuffer[122] = bHotRinse_Selected;

	ucWrite_DataBuffer[123] = Switch_Reset;
	ucWrite_DataBuffer[124] = bHPrs;
	ucWrite_DataBuffer[125] = bLPrs;
	ucWrite_DataBuffer[126] = bCold_Soak_Selected;
	ucWrite_DataBuffer[127] = uidefault_kg;
	ucWrite_DataBuffer[128] = getAIComm_ExecState_ui8();
	ucWrite_DataBuffer[129] = getAIComm_Program_ui8();
/*----------------------------------------------------------------------------------------------------------------------------	
	ucWrite_DataBuffer[130] = Reserved;
*///----------------------------------------------------------------------------------------------------------------------------	
#if ENABLE_IOT
	//ucWrite_DataBuffer[131] is used for "bFirmwareUpgrade" variable to remove 3Sec startup delay in bootloader pgm
	ucWrite_DataBuffer[132] = (uint8_t)(ProgramTime & 0x00FF);
	ucWrite_DataBuffer[133] = (uint8_t)(ProgramTime >> 8);
	ucWrite_DataBuffer[134] = BkMultiFuncSetting;
	//ucWrite_DataBuffer[135] = WiFi_Comn_Test;
#endif
	ucWrite_DataBuffer[135]	= uiPrewashWaterResetCount;
	ucWrite_DataBuffer[136]	= uiFinalWt;
	ucWrite_DataBuffer[137]	= uiLS_Total_subtraction;
	ucWrite_DataBuffer[138]	= uiUB_decidation;
	ucWrite_DataBuffer[139]	= uiLS_Tx_subtraction;
	ucWrite_DataBuffer[140]	= uiTest_drain;
	ucWrite_DataBuffer[141]	= uiLoadRange;
	ucWrite_DataBuffer[142]	= L175_count;
	ucWrite_DataBuffer[143]	= uiheat_temp;
	//ucWrite_DataBuffer[144]	= uiResetFill_Count; //ucWrite_DataBuffer[144]	= bExpress_30_Activatted;
	ucWrite_DataBuffer[144]	= bLoadSensing_Completed;
	MemoryWrite();
}

void MemoryWrite(void)
{
	DI();
	FDL_Status = PFDL_Open(&FDL_SysInit);
    	if (FDL_Status != PFDL_OK)
    	{
    		while(1)
    		{
    		  //FDL initialization failed, user may insert RESET function
    		}

    	}
	FDL_Blank_Chk();
	if (FDL_Status == PFDL_ERR_MARGIN) //Execute Erase comamnd if Blank Check uiError_Occured
	{
		FDL_Erase();	//Execute Erase command
		if (FDL_Status == PFDL_ERR_ERASE) //if Erase uiError_Occured
		{
			while(1)
			{
				//FDL Erase fucntion failed, user may insert RESET function
			}
		}
	}     
	FDL_Write();//Execute Write command  
    	if (FDL_Status == PFDL_ERR_MARGIN) //Execute Erase comamnd if Blank Check uiError_Occured
	{
		FDL_Erase();	//Execute Erase command
		if (FDL_Status == PFDL_ERR_ERASE) //if Erase uiError_Occured
		{
			while(1)
			{
				//FDL Erase fucntion failed, user may insert RESET function
			}
		}
	}    
	FDL_Verify();	//Execute Verify command
    	
	if (FDL_Status == PFDL_ERR_MARGIN) //Execute Erase comamnd if verify uiError_Occured
	{
		FDL_Erase();	//Execute Erase command
		if (FDL_Status == PFDL_ERR_ERASE) //if Erase uiError_Occured
		{
			 while(1)
			 {
				 //FDL Erase fucntion failed, user may insert RESET function
			 }
		}
	}    
	//Execute Read command 
    PFDL_Close(); 
	EI();	
}

//*******************************************************************************************************//
//**************************** For Memory Mirror Operation *********************************************//
void Memory_Mirror_Write(void)
{
	DI();
	FDL_Status = PFDL_Open(&FDL_SysInit);
    	if (FDL_Status != PFDL_OK)
    	{
    		while(1)
    		{
    		  //FDL initialization failed, user may insert RESET function
    		}

    	}
	FDL_Blank_Chk_1();
	if (FDL_Status == PFDL_ERR_MARGIN) //Execute Erase comamnd if Blank Check uiError_Occured
	{
		FDL_Erase_1();	//Execute Erase command
		if (FDL_Status == PFDL_ERR_ERASE) //if Erase uiError_Occured
		{
			while(1)
			{
				//FDL Erase fucntion failed, user may insert RESET function
			}
		}
	}     
	FDL_Write_1();//Execute Write command  
    	if (FDL_Status == PFDL_ERR_MARGIN) //Execute Erase comamnd if Blank Check uiError_Occured
	{
		FDL_Erase_1();	//Execute Erase command
		if (FDL_Status == PFDL_ERR_ERASE) //if Erase uiError_Occured
		{
			while(1)
			{
				//FDL Erase fucntion failed, user may insert RESET function
			}
		}
	}    
	FDL_Verify_1();	//Execute Verify command
    	
	if (FDL_Status == PFDL_ERR_MARGIN) //Execute Erase comamnd if verify uiError_Occured
	{
		FDL_Erase_1();	//Execute Erase command
		if (FDL_Status == PFDL_ERR_ERASE) //if Erase uiError_Occured
		{
			 while(1)
			 {
				 //FDL Erase fucntion failed, user may insert RESET function
			 }
		}
	}    
	//Execute Read command 
    PFDL_Close(); 
	EI();	
}

void Position_Idle()
{
	//memset(ucWrite_DataBuffer,RESET_VALUE,MemoryLength);
	//MemoryWrite();
}

void PowerOnRead()
{
	uint8_t ii;
	 
	FDL_Status=PFDL_Open(&FDL_SysInit);
    	if (FDL_Status!= PFDL_OK)
    	{
    		while(1)
    		{
    			//FDL initialization failed, user may insert RESET function
    		}

    	}
    	FDL_Read();		//Execute Read command 
    	PFDL_Close();
	
	//************************ reset the all byte if any found 0xff *********************************************//
	if((ucRead_DataBuffer[2] == 0xFF)||(ucRead_DataBuffer[3]== 0xFF)||(ucRead_DataBuffer[51] == 0xFF)
	||(ucRead_DataBuffer[61] == 0xFF)||(ucRead_DataBuffer[94] == 0xFF)) // 
	{
		
		FDL_Status=PFDL_Open(&FDL_SysInit);
	    	if (FDL_Status!= PFDL_OK)
	    	{
	    		while(1)
	    		{
	    			//FDL initialization failed, user may insert RESET function
	    		}

	    	}
	    	FDL_Read_1();		//Execute Read command 
		
	    	PFDL_Close();
		
		MemoryValuesRead(); // restore the previous values
		
		if((ucRead_DataBuffer[2] == 0xFF)||(ucRead_DataBuffer[3]== 0xFF)||(ucRead_DataBuffer[51] == 0xFF)
		||(ucRead_DataBuffer[61] == 0xFF)||(ucRead_DataBuffer[94] == 0xFF))
		{
			/*for(ii = 0 ; ii< MemoryLength ; ii++)
			{
				ucWrite_DataBuffer[ii] = 0x00;
			}*/
			memset(ucWrite_DataBuffer,RESET_VALUE,MemoryLength);
			ucWrite_DataBuffer[0] = uiProgram_Position = uiSelected_Position =  1; // to set the default 1 position
		
			snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT;
			MemoryWrite();
			Memory_Mirror_Write();
		}
	}
	else
	{
		uiBeforeState = ucRead_DataBuffer[29];
#if ENABLE_IOT
		BkMultiFuncSetting = ucRead_DataBuffer[134];
		set_IOTMultiFuncSetting(BkMultiFuncSetting);
#endif		   
	    	if((ucRead_DataBuffer[1] == YES)||(uiMachineState == RINSE_HOLD))
	    	{
			MemoryValuesRead(); // restore the previous values
			
	   	}
		else
		{
			ResetMemoryValues(); 
		}
	}
	if(((uiSelected_Position <= 1) && ((uiRpmSel == 0xFF)||(uiRpmSel == 0x00))&& ((uiTempSel == 0xFF)||(uiTempSel == 0x00))) 
	|| (uiSelected_Position == 0xFF))
	{
		ResetMemoryValues();
		uiProgram_Position = uiSelected_Position = 1;
		uiRpmSel = Rpm1000 ;// RapidWashSelected;
		uiTempSel = Wash40;// TemperatureSelected;
		bFCTMode_Selection = FALSE;
		uidisp_select_option = 0x00;
		bDoorLockError = FALSE;
		bChildLockButton = FALSE;
		uiSmartProgram_Selected = RESET_VALUE;
		bStartButton = FALSE; 
		uiWaterResetCount = FALSE;
		uiPrewashWaterResetCount = FALSE;
		//uiResetFill_Count = FALSE;
		uiReset_TempRPM = TRUE;
		
		if(ProgramLifeCycle <= 40)
		{
			uitubclean_cycle = (uint8_t)ProgramLifeCycle;
		}
		uiMachineState = FALSE;
		TubClean_Activated = FALSE;
		//bExpress_30_Activatted = FALSE;
		uiDoorUnlockPulseCount = FALSE;
		BalanceTime = MIXED40_TIME;	
	}
	//-------------------------------------------------------
}

void FDL_Blank_Chk(void)
{
	FDL_Request.index_u16=0x0000;  				//Start Address of Data Flash (For R5F100LE, Data Flash range from (0xF1000 - 0xF1FFF)
	FDL_Request.bytecount_u16= MemoryLength;				//Counter for number of bytes to be check
	FDL_Request.command_enu =PFDL_CMD_BLANKCHECK_BYTES;	//Assign the command to be execute by PFDL_Execute
	PFDL_Execute(&FDL_Request);				//Execute the Blank Check on data flash
	FDL_status_chk();					//Check status of FDL Handler
}

//*****************************************

void FDL_status_chk(void)
{
	FDL_Status=PFDL_BUSY;
	while (FDL_Status==PFDL_BUSY)
	{
		FDL_Status=PFDL_Handler();
	}	
}

//***************************************

void FDL_Erase(void)
{
	FDL_Request.index_u16=0;  				//Assign the block 0 of data flash to be erase Data Flash 
								//0: block 0 (0xF1000 to 0xF13FF) 
								//1: block 1 (0xF1400 to 0xF17FF)						//2: block 2 (0xF1800 to 0xF1BFF)
								//3: block 3 (0xF1C00 to 0xF1FFF)
	FDL_Request.command_enu =PFDL_CMD_ERASE_BLOCK;		//Assign the command to be execute by PFDL_Execute
	PFDL_Execute(&FDL_Request);				//Execute the Blank Check on data flash
	FDL_status_chk();					//Check status of FDL Handler
}

//********************

void FDL_Write(void)
{
	FDL_Request.index_u16=0x0000;  				//Start Address of Data Flash (For R5F100LE, Data Flash range from (0xF1000 - 0xF1FFF)
	FDL_Request.data_pu08=ucWrite_DataBuffer; 		//Assign the address of Write_DataBuffer as data to be stored in Data Flash
	FDL_Request.bytecount_u16=MemoryLength;				//Counter for number of bytes to be write
	FDL_Request.command_enu =PFDL_CMD_WRITE_BYTES;		//Assign the command to be execute by PFDL_Execute
	PFDL_Execute(&FDL_Request);				//Execute the Blank Check on data flash
	FDL_status_chk();					//Check status of FDL Handler	
}

//*****************************
void FDL_Verify(void)
{
	FDL_Request.index_u16=0x0000;  				//Start Address of Data Flash (For R5F100LE, Data Flash range from (0xF1000 - 0xF1FFF)
	FDL_Request.bytecount_u16=MemoryLength;				//Counter for number of bytes to be verify
	FDL_Request.command_enu =PFDL_CMD_IVERIFY_BYTES;	//Assign the command to be execute by PFDL_Execute
	PFDL_Execute(&FDL_Request);				//Execute the data verify on data flash
	FDL_status_chk();
}

//*****************************
void FDL_Read(void)
{
	FDL_Request.index_u16=0x0000;  				//Read from 0xF1000h; 
								//Start Address of Data Flash (For R5F100LE, Data Flash range from (0xF1000 - 0xF1FFF)
	FDL_Request.data_pu08=ucRead_DataBuffer; 			//Assign the address of Read_DataBuffer to stored contents of  dedicated Data Flash area
	FDL_Request.bytecount_u16=MemoryLength;				//Counter for number of bytes to be read
	FDL_Request.command_enu =PFDL_CMD_READ_BYTES;		//Assign the command to be execute by PFDL_Execute
	PFDL_Execute(&FDL_Request);				//Execute the read command on data flash
	FDL_status_chk();
}
//*****************************
//***************************************************************************************************************************************************//
//***************************************************************************************************************************************************//
//******************************************************* For Memory Corruption Operation *********************************************************************//
void FDL_Blank_Chk_1(void)
{
	FDL_Request.index_u16=0x0400;  				//Start Address of Data Flash (For R5F100LE, Data Flash range from (0xF1000 - 0xF1FFF)
	FDL_Request.bytecount_u16= MemoryLength;		//Counter for number of bytes to be check
	FDL_Request.command_enu =PFDL_CMD_BLANKCHECK_BYTES;	//Assign the command to be execute by PFDL_Execute
	PFDL_Execute(&FDL_Request);				//Execute the Blank Check on data flash
	FDL_status_chk_1();					//Check status of FDL Handler
}

//*****************************************

void FDL_status_chk_1(void)
{
	FDL_Status=PFDL_BUSY;
	while (FDL_Status==PFDL_BUSY)
	{
		FDL_Status=PFDL_Handler();
	}	
}

//***************************************

void FDL_Erase_1(void)
{
	FDL_Request.command_enu =PFDL_CMD_ERASE_BLOCK;		//Assign the command to be execute by PFDL_Execute
	FDL_Request.index_u16=1;  				//Assign the block 0 of data flash to be erase Data Flash 
								//0: block 0 (0xF1000 to 0xF13FF) 
								//1: block 1 (0xF1400 to 0xF17FF)		
								//2: block 2 (0xF1800 to 0xF1BFF)
								//3: block 3 (0xF1C00 to 0xF1FFF)
	PFDL_Execute(&FDL_Request);				//Execute the Blank Check on data flash
	FDL_status_chk_1();					//Check status of FDL Handler
}

//********************

void FDL_Write_1(void)
{
	FDL_Request.command_enu =PFDL_CMD_WRITE_BYTES;		//Assign the command to be execute by PFDL_Execute
	FDL_Request.index_u16=0x0400;  				//Start Address of Data Flash (For R5F100LE, Data Flash range from (0xF1000 - 0xF1FFF)
	FDL_Request.data_pu08=ucWrite_DataBuffer; 		//Assign the address of Write_DataBuffer as data to be stored in Data Flash
	FDL_Request.bytecount_u16=MemoryLength;			//Counter for number of bytes to be write
	
	PFDL_Execute(&FDL_Request);				//Execute the Blank Check on data flash
	FDL_status_chk_1();					//Check status of FDL Handler	
}

//*****************************
void FDL_Verify_1(void)
{
	FDL_Request.command_enu =PFDL_CMD_IVERIFY_BYTES;	//Assign the command to be execute by PFDL_Execute
	FDL_Request.index_u16=0x0400;  				//Start Address of Data Flash (For R5F100LE, Data Flash range from (0xF1000 - 0xF1FFF)
	FDL_Request.bytecount_u16=MemoryLength;			//Counter for number of bytes to be verify
	
	PFDL_Execute(&FDL_Request);				//Execute the data verify on data flash
	FDL_status_chk_1();
}

//*****************************
void FDL_Read_1(void)
{
	FDL_Request.command_enu =PFDL_CMD_READ_BYTES;		//Assign the command to be execute by PFDL_Execute
	FDL_Request.index_u16=0x0400;  				//Read from 0xF1000h; 
								//Start Address of Data Flash (For R5F100LE, Data Flash range from (0xF1000 - 0xF1FFF)
	FDL_Request.data_pu08=ucRead_DataBuffer; 		//Assign the address of Read_DataBuffer to stored contents of  dedicated Data Flash area
	FDL_Request.bytecount_u16=MemoryLength;			//Counter for number of bytes to be read
	
	PFDL_Execute(&FDL_Request);				//Execute the read command on data flash
	FDL_status_chk_1();
}
//******************************************************************************************************************************************************//
//******************************************************************************************************************************************************//

void MemoryValuesRead()
{
	bStartButton = ucRead_DataBuffer[0];
	bWashProgram_Selected = ucRead_DataBuffer[1];
	uiStep_Minutes_Completed = ucRead_DataBuffer[2];
	uiPresent_Step = ucRead_DataBuffer[3];		
	bRapidWashButton = ucRead_DataBuffer[4];
	uiRinseholdState = ucRead_DataBuffer[5] ;
	uiStep_Seconds = ucRead_DataBuffer[6];
	uiFillingState = ucRead_DataBuffer[7];
	uiRinseFillState = ucRead_DataBuffer[8];
	uiHeatState = ucRead_DataBuffer[9];
	
	uiDrainState = ucRead_DataBuffer[10] ;
	uiTumbleState = ucRead_DataBuffer[11];
	bProgramStart = ucRead_DataBuffer[12];
	uiAdditional_Heater_TurnOn_Mins = ucRead_DataBuffer[13];
	uiLowPressureTime = ucRead_DataBuffer[14];
	uiNoWaterTime = ucRead_DataBuffer[15];
	uiAdditional_Drain_TurnOn_Mins = ucRead_DataBuffer[16];
	uiWaterResetCount = ucRead_DataBuffer[17];
	uiHalfMinutes_Seconds = ucRead_DataBuffer[18];
	uiHalfMinute_Completed = ucRead_DataBuffer[19];
	uiTestModeState = ucRead_DataBuffer[20];
	
	FaciaState = ucRead_DataBuffer[21];
	uiMachineTestState = ucRead_DataBuffer[22];
	bFaciaTest = ucRead_DataBuffer[23];
	bTestMode_Selection = ucRead_DataBuffer[24];
	bChildLockButton = ucRead_DataBuffer[25];
	bNoWater = ucRead_DataBuffer[26];
	bEnduranceMode = ucRead_DataBuffer[27];
	uiFCTMode = ucRead_DataBuffer[28];
	uiBeforeState = ucRead_DataBuffer[29];
	SFState = ucRead_DataBuffer[30] ;
	
	uiExtraTime_Seconds = ucRead_DataBuffer[31];
	bSpecialEndurance_Finished = ucRead_DataBuffer[32];
	bCommunicationError = ucRead_DataBuffer[33];
	uiReheatTime = ucRead_DataBuffer[34];
	bPressureError = ucRead_DataBuffer[35];
	uiHeaterCheck = ucRead_DataBuffer[36];
	uiDoorUnlockPulseCount = ucRead_DataBuffer[37];
	bReheatHeater = ucRead_DataBuffer[38];
	uiSmartProgram_Selected = ucRead_DataBuffer[39];
	HeatMinutes = ucRead_DataBuffer[40];
	
	bHeaterOpen = ucRead_DataBuffer[41];
	bTwoLitreReached = ucRead_DataBuffer[42];
	BalanceTime = ucRead_DataBuffer[44];
	BalanceTime =( BalanceTime << 8)+ ucRead_DataBuffer[43];
	uidelaymin = ucRead_DataBuffer[46];
	uidelaymin =( uidelaymin << 8)+ ucRead_DataBuffer[45];
	uicycle_mincompleted = ucRead_DataBuffer[48];
	uicycle_mincompleted =( uicycle_mincompleted << 8)+ ucRead_DataBuffer[47]; 
	uiprogram_option_status = ucRead_DataBuffer[49];
	bTESTMODE_START = ucRead_DataBuffer[50];
	
	uiOverFlowOccurs = ucRead_DataBuffer[51] ; 
	uiCurrent_step_time = ucRead_DataBuffer[53];
	uiCurrent_step_time =( uiCurrent_step_time << 8)+ ucRead_DataBuffer[52];
	uiStartTemperature  = ucRead_DataBuffer[54];
	uiTime_Seconds = ucRead_DataBuffer[55]; 
	uiTime_Minutes_Completed = ucRead_DataBuffer[56];
	bSpinStarted = ucRead_DataBuffer[57]; 
	bHeatCompleted = ucRead_DataBuffer[58];
	bTest_Forward =  ucRead_DataBuffer[59];
	uiover_heat =  ucRead_DataBuffer[60];
	
	uiDoorLOckReleaseSeconds =  ucRead_DataBuffer[61];
	bWaterOverFLow  =  ucRead_DataBuffer[62];
	bEnduranceStarted = ucRead_DataBuffer[63];
	bUnbalanceError =  ucRead_DataBuffer[64];
	bDoorOpenError  = ucRead_DataBuffer[65];
	snoozemode_10minutes_counter = ucRead_DataBuffer[67];
	snoozemode_10minutes_counter =( snoozemode_10minutes_counter << 8)+ ucRead_DataBuffer[66];
	bRinsehold_Finished = ucRead_DataBuffer[68];
	uiExtraRinse_Max = ucRead_DataBuffer[69];
	bOverHeat = ucRead_DataBuffer[70];
	
	uiOption_closed = ucRead_DataBuffer[71];
	bStep_Flag =  ucRead_DataBuffer[72];
	bLevelReached = ucRead_DataBuffer[73];
	uihigh_low_vtlg = ucRead_DataBuffer[74];
	bFCTMode_Selection = ucRead_DataBuffer[75];
	bDelay_Finished = ucRead_DataBuffer[76];
	bSpecialEnduranceSelected = ucRead_DataBuffer[77];
	bEmegency_door_pull = ucRead_DataBuffer[78]; 
	bEco60Reset = ucRead_DataBuffer[79];
	Step_pos = ucRead_DataBuffer[80];
	
	bMotorError_E1 = ucRead_DataBuffer[81];
	bMotorError_E2 = ucRead_DataBuffer[82];
	bMotorError_E3 = ucRead_DataBuffer[83];
	
	uispin_cyclecompleted = ucRead_DataBuffer[85];
	uispin_cyclecompleted = (uispin_cyclecompleted << 8)+ ucRead_DataBuffer[84] ;
	ProgramLifeCycle = ucRead_DataBuffer[87];
	ProgramLifeCycle = (ProgramLifeCycle << 8)+ ucRead_DataBuffer[86] ;
	uiSF7_cyclecompleted = ucRead_DataBuffer[89];
	uiSF7_cyclecompleted = (uiSF7_cyclecompleted << 8)+ ucRead_DataBuffer[88] ;
	uiSF6_cyclecompleted = ucRead_DataBuffer[91];
	uiSF6_cyclecompleted = (uiSF6_cyclecompleted << 8)+ ucRead_DataBuffer[90];
	
	uiSF8_cyclecompleted = ucRead_DataBuffer[93];
	uiSF8_cyclecompleted = (uiSF8_cyclecompleted << 8)+ ucRead_DataBuffer[92] ;
	uiSelected_Position  = ucRead_DataBuffer[94];
	uiMachineState = ucRead_DataBuffer[95] ;
	uiRpmSel  = ucRead_DataBuffer[96];
	uiTempSel = ucRead_DataBuffer[97];
	uiTubClean_req = ucRead_DataBuffer[98];
	uiTubClean_Disp_count = ucRead_DataBuffer[99];
	uitubclean_cycle = ucRead_DataBuffer[100];
	
	uiPSER_Option_Avbl = ucRead_DataBuffer[101];
	uidisp_select_option = uilast_selected_option = ucRead_DataBuffer[102];
	Error_Storage[0]= ucRead_DataBuffer[103];
	Error_Storage[1]= ucRead_DataBuffer[104];
	Error_Storage[2]= ucRead_DataBuffer[105];
	Error_Storage[3]= ucRead_DataBuffer[106];
	Error_Storage[4]= ucRead_DataBuffer[107];
	Error_Storage[5]= ucRead_DataBuffer[108];
	Error_Storage[6]= ucRead_DataBuffer[109];
	prev_program_pos = ucRead_DataBuffer[110];
	
	uiRMP_Default = ucRead_DataBuffer[111];
	uimaxspeed_temp = ucRead_DataBuffer[112];
	bDoorLockError = ucRead_DataBuffer[113];
	uiExtraRinse_Press_Select = ucRead_DataBuffer[114]; 
	TubClean_Activated = ucRead_DataBuffer[115];
	uiprev_error = ucRead_DataBuffer[116];
	bDoorLock = ucRead_DataBuffer[117];
	ui_prev_door_status = ucRead_DataBuffer[118];
	uiExtraRinse_Press_count = ucRead_DataBuffer[119]; 
	bDoorUnlocked = ucRead_DataBuffer[120]  ;
	uiMachineBefore_Poweroff =  ucRead_DataBuffer[121];
	bHotRinse_Selected =  ucRead_DataBuffer[122];

	Switch_Reset =  ucRead_DataBuffer[123];
 	bHPrs = ucRead_DataBuffer[124];
	bLPrs = ucRead_DataBuffer[125];
	bCold_Soak_Selected = ucRead_DataBuffer[126];	
	uidefault_kg = ucRead_DataBuffer[127];
	setAIComm_State_ui8(ucRead_DataBuffer[128]);
	setAIComm_Program_ui8(ucRead_DataBuffer[129]);
/*------------------------------------------------------------------------------------------------------------	
	
	
	Reserved = ucRead_DataBuffer[130];
*///----------------------------------------------------------------------------------------------------------
#if ENABLE_IOT
	//ucRead_DataBuffer[131] is used for "bFirmwareUpgrade" variable to remove 3Sec startup delay
	ProgramTime = ucRead_DataBuffer[133];
	ProgramTime =( ProgramTime << 8)+ ucRead_DataBuffer[132];
	//WiFi_Comn_Test = ucRead_DataBuffer[135];
 	set_IOTTemperature(uiTempSel - 1);
#endif
	uiPrewashWaterResetCount = ucRead_DataBuffer[135];
	uiFinalWt = ucRead_DataBuffer[136];
	uiLS_Total_subtraction = ucRead_DataBuffer[137];
	uiUB_decidation = ucRead_DataBuffer[138];
	uiLS_Tx_subtraction = ucRead_DataBuffer[139];
	uiTest_drain = ucRead_DataBuffer[140];
	uiLoadRange = ucRead_DataBuffer[141];
	L175_count = ucRead_DataBuffer[142];
	uiheat_temp = ucRead_DataBuffer[143];
	//uiResetFill_Count = ucRead_DataBuffer[144]; //bExpress_30_Activatted = ucRead_DataBuffer[144];
	bLoadSensing_Completed  = ucRead_DataBuffer[144];
	Display_Backup();
	Option_Backup();
	
	if((bStartButton == TRUE)&&(bZcdDetection == TRUE))
	{
		START_LED_ON;//cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
	}
	else
	{
		START_LED_OFF ;//cdigits_data[4] =  cdigits_data[4] & ~(0x04); 
		// uiprogram_option_status = 0x00;
	}
	if((BalanceTime < 0) ||(BalanceTime > 250))
	{
		if(uiMachineState == START_PRESSED)
		{
			uiMachineState = PAUSE_PRESSED;
			ProgramTimming();
			uiMachineState = START_PRESSED;
		}
		else
		{
			ProgramTimming();
		}
	}
}

void ResetMemoryValues()
{
	uint8_t ii;
	
	for(ii = 0 ; ii < 83 ; ii++)
	{
		if(ii==39)
		{
			uiSmartProgram_Selected = ucRead_DataBuffer[39];
		}
		else
		{
			ucWrite_DataBuffer[ii] = 0x00;
		}
	}
	uispin_cyclecompleted = ucRead_DataBuffer[85];
	uispin_cyclecompleted = (uispin_cyclecompleted << 8)+ ucRead_DataBuffer[84] ;
	ProgramLifeCycle = ucRead_DataBuffer[87];
	ProgramLifeCycle = (ProgramLifeCycle << 8)+ ucRead_DataBuffer[86] ;
	uiSF7_cyclecompleted = ucRead_DataBuffer[89];
	uiSF7_cyclecompleted = (uiSF7_cyclecompleted << 8)+ ucRead_DataBuffer[88] ;
	uiSF6_cyclecompleted = ucRead_DataBuffer[91];
	uiSF6_cyclecompleted = (uiSF6_cyclecompleted << 8)+ ucRead_DataBuffer[90];
	
	uiSF8_cyclecompleted = ucRead_DataBuffer[93];
	uiSF8_cyclecompleted = (uiSF8_cyclecompleted << 8)+ ucRead_DataBuffer[92] ;
	uiSelected_Position  = ucRead_DataBuffer[94];
	uiMachineState = ucRead_DataBuffer[95] ;
	uiRpmSel  = ucRead_DataBuffer[96];
	uiTempSel = ucRead_DataBuffer[97];
	uiTubClean_req = ucRead_DataBuffer[98];
	uiTubClean_Disp_count = ucRead_DataBuffer[99];
	uitubclean_cycle = ucRead_DataBuffer[100];
	
	uiPSER_Option_Avbl = ucRead_DataBuffer[101];
	uidisp_select_option = uilast_selected_option = ucRead_DataBuffer[102];
	Error_Storage[0]= ucRead_DataBuffer[103];
	Error_Storage[1]= ucRead_DataBuffer[104];
	Error_Storage[2]= ucRead_DataBuffer[105];
	Error_Storage[3]= ucRead_DataBuffer[106];
	Error_Storage[4]= ucRead_DataBuffer[107];
	Error_Storage[5]= ucRead_DataBuffer[108];
	Error_Storage[6]= ucRead_DataBuffer[109];
	prev_program_pos = ucRead_DataBuffer[110];
	
	uiRMP_Default = ucRead_DataBuffer[111];
	uimaxspeed_temp = ucRead_DataBuffer[112];
	bDoorLockError = ucRead_DataBuffer[113];
	uiExtraRinse_Press_Select = ucRead_DataBuffer[114]; 
	TubClean_Activated = ucRead_DataBuffer[115];
	uiprev_error = ucRead_DataBuffer[116]= 0xFF;
	
	if((bDoorLockStatus == TRUE)&&(iWaterLevel > EMPTYLEVEL)&&(uiTemperature < ON_PAUSE_DOOR_OPEN_TEMPERATURE)&&(uiRPMValue == RESET_VALUE))
	{
		bDoorLock = ucRead_DataBuffer[117]=RESET_VALUE;
		ui_prev_door_status = ucRead_DataBuffer[118]=RESET_VALUE;
	}
	else
	{
		bDoorLock = ucRead_DataBuffer[117];
		ui_prev_door_status = ucRead_DataBuffer[118];
	}
	 
	uiExtraRinse_Press_count = ucRead_DataBuffer[119]; 
	bDoorUnlocked = ucRead_DataBuffer[120]  ;
	uiMachineBefore_Poweroff =  ucRead_DataBuffer[121];
	bHotRinse_Selected =  ucRead_DataBuffer[122];

	Switch_Reset =  ucRead_DataBuffer[123]; 
	bHPrs = ucRead_DataBuffer[124]= RESET_VALUE;
	bLPrs = ucRead_DataBuffer[125]= RESET_VALUE;
	bCold_Soak_Selected = ucRead_DataBuffer[126];
	uidefault_kg = ucRead_DataBuffer[127];
/*---------------------------------------------------------------------------------------------------
	Reserved = ucRead_DataBuffer[128];
	Reserved = ucRead_DataBuffer[129];
	Reserved = ucRead_DataBuffer[130];
*///---------------------------------------------------------------------------------------------------
#if ENABLE_IOT
	//ucRead_DataBuffer[131] is used for bFirmwareUpgrade variable to remove 3Sec startup delay
	ProgramTime = ucRead_DataBuffer[132]= RESET_VALUE;
	ProgramTime = ucRead_DataBuffer[133] = RESET_VALUE;
	BkMultiFuncSetting = ucRead_DataBuffer[134] = RESET_VALUE;
	//WiFi_Comn_Test = ucRead_DataBuffer[135];
#endif	
	uiFinalWt = ucRead_DataBuffer[136]= RESET_VALUE;
	uiLS_Total_subtraction = ucRead_DataBuffer[137]= RESET_VALUE;
	uiUB_decidation = ucRead_DataBuffer[138] = RESET_VALUE;
	uiLS_Tx_subtraction = ucRead_DataBuffer[139] = RESET_VALUE;
	uiTest_drain = ucRead_DataBuffer[140] = RESET_VALUE;
	uiLoadRange = ucRead_DataBuffer[141] = RESET_VALUE;
	L175_count = ucRead_DataBuffer[142] = RESET_VALUE;
	uiheat_temp = ucRead_DataBuffer[143] = RESET_VALUE;
	//bExpress_30_Activatted = ucRead_DataBuffer[144] = RESET_VALUE;
	bLoadSensing_Completed  = ucRead_DataBuffer[144] = RESET_VALUE;
	
	 if(bZcdDetection == TRUE)
	 {
	 	StopAllEvents();
	 }
	 Display_Backup();
	
	if(((uiMachineState == PAUSE_PRESSED) || (uiMachineState == START_PRESSED))&&(bZcdDetection == TRUE))
	{	
		START_LED_ON ;//cdigits_data[4] =  cdigits_data[4] | 0x08; // //turn on the start/puase led
		//bDoorLock = SWITCH_ON;
		Option_Backup();
	}
	else
	{
		START_LED_OFF; //cdigits_data[4] =  cdigits_data[4] & ~(0x04); 
		uiprogram_option_status = RESET_VALUE;
	}
	 
}
//*********************************************************************************************//
void MemoryMirror()
{
	DI();
	
	uiprogram_option_status = 0x00;
	
	if(bPrewash_Start == TRUE) 
	{
		uiprogram_option_status |=0x01;
	}
	if(bSoak_Start == TRUE)
	{
		uiprogram_option_status |=0x02;
	}
	if(bExtraRinse_Start == TRUE)
	{
		uiprogram_option_status |=0x04;
	}
	if( bSteam_Start  == TRUE)
	{
		uiprogram_option_status |=0x08;
	}
	if(bDelay_Start1 == TRUE)
	{
		uiprogram_option_status |=0x10;
	}
	if(bTimeSaver_Start == TRUE)
	{
		uiprogram_option_status |=0x20;	
	}
	
	ucWrite_DataBuffer[0] = bStartButton;
	ucWrite_DataBuffer[1] = bWashProgram_Selected;
	ucWrite_DataBuffer[2] = uiStep_Minutes_Completed;
	ucWrite_DataBuffer[3] = uiPresent_Step;		
	ucWrite_DataBuffer[4] = bRapidWashButton;
	ucWrite_DataBuffer[5] = uiRinseholdState;
	ucWrite_DataBuffer[6] = uiStep_Seconds;
	ucWrite_DataBuffer[7] = uiFillingState;
	ucWrite_DataBuffer[8] = uiRinseFillState;
	ucWrite_DataBuffer[9] = uiHeatState;
	
	ucWrite_DataBuffer[10] = uiDrainState;
	ucWrite_DataBuffer[11] = uiTumbleState;
	ucWrite_DataBuffer[12] = bProgramStart;
	ucWrite_DataBuffer[13] = uiAdditional_Heater_TurnOn_Mins;
	ucWrite_DataBuffer[14] = uiLowPressureTime;
	ucWrite_DataBuffer[15] = uiNoWaterTime;
	ucWrite_DataBuffer[16] = uiAdditional_Drain_TurnOn_Mins;
	ucWrite_DataBuffer[17] = uiWaterResetCount;
	ucWrite_DataBuffer[18] = uiHalfMinutes_Seconds;
	ucWrite_DataBuffer[19] = uiHalfMinute_Completed;
	ucWrite_DataBuffer[20] = uiTestModeState;
	
	ucWrite_DataBuffer[21] = FaciaState;
	ucWrite_DataBuffer[22] = uiMachineTestState;
	ucWrite_DataBuffer[23] = bFaciaTest;
	ucWrite_DataBuffer[24] = bTestMode_Selection;
	ucWrite_DataBuffer[25] = bChildLockButton;
	ucWrite_DataBuffer[26] = bNoWater;
	ucWrite_DataBuffer[27] = bEnduranceMode;
	ucWrite_DataBuffer[28] = uiFCTMode;
	ucWrite_DataBuffer[29] = uiBeforeState;
	ucWrite_DataBuffer[30] = SFState;
	
	ucWrite_DataBuffer[31] = uiExtraTime_Seconds;
	ucWrite_DataBuffer[32] = bSpecialEndurance_Finished;
	ucWrite_DataBuffer[33] = bCommunicationError;
	ucWrite_DataBuffer[34] = uiReheatTime;
	ucWrite_DataBuffer[35] = bPressureError;
	ucWrite_DataBuffer[36] = uiHeaterCheck;
	ucWrite_DataBuffer[37] = uiDoorUnlockPulseCount;
	ucWrite_DataBuffer[38] = bReheatHeater;
	ucWrite_DataBuffer[39] = uiSmartProgram_Selected;
	ucWrite_DataBuffer[40] = HeatMinutes;
	
	ucWrite_DataBuffer[41] = bHeaterOpen;
	ucWrite_DataBuffer[42] = bTwoLitreReached;
	
	if(bSpinStarted == TRUE)
	{
		BalanceTime = BalanceTime + uiTime_Minutes_Completed;
		uiCurrent_step_time = uiCurrent_step_time + uiTime_Minutes_Completed;
		uiTime_Minutes_Completed = RESET_VALUE;
		uiTime_Seconds = RESET_VALUE;
	}
	ucWrite_DataBuffer[43] = (uint8_t)(BalanceTime & 0x00FF);
	ucWrite_DataBuffer[44] = (uint8_t)(BalanceTime >> 8);
	ucWrite_DataBuffer[45] = (uint8_t)(uidelaymin & 0x00FF);
	ucWrite_DataBuffer[46] = (uint8_t)(uidelaymin >> 8);
	ucWrite_DataBuffer[47] = (uint8_t)(uicycle_mincompleted & 0x00FF);
	ucWrite_DataBuffer[48] = (uint8_t)(uicycle_mincompleted >> 8);
	ucWrite_DataBuffer[49] = uiprogram_option_status;
	ucWrite_DataBuffer[50] = bTESTMODE_START;
	
	ucWrite_DataBuffer[51] = uiOverFlowOccurs;
	ucWrite_DataBuffer[52] = (uint8_t) (uiCurrent_step_time & 0x00FF);
	ucWrite_DataBuffer[53] = (uint8_t)(uiCurrent_step_time >> 8);
	ucWrite_DataBuffer[54] = uiStartTemperature;
	ucWrite_DataBuffer[55] = uiTime_Seconds;
	ucWrite_DataBuffer[56] = uiTime_Minutes_Completed;
	ucWrite_DataBuffer[57] = bSpinStarted;	
	ucWrite_DataBuffer[58] = bHeatCompleted;
	ucWrite_DataBuffer[59] = bTest_Forward;
	ucWrite_DataBuffer[60] = uiover_heat;
	
	ucWrite_DataBuffer[61] = uiDoorLOckReleaseSeconds;	
	ucWrite_DataBuffer[62] = bWaterOverFLow;
	ucWrite_DataBuffer[63] = bEnduranceStarted;
	ucWrite_DataBuffer[64] = bUnbalanceError;
	ucWrite_DataBuffer[65] = bDoorOpenError;	
	ucWrite_DataBuffer[66] = (uint8_t)(snoozemode_10minutes_counter & 0x00FF);
	ucWrite_DataBuffer[67] = (uint8_t)(snoozemode_10minutes_counter >> 8);
	ucWrite_DataBuffer[68] = bRinsehold_Finished ;
	ucWrite_DataBuffer[69] = uiExtraRinse_Max ;
	ucWrite_DataBuffer[70] = bOverHeat ;
	
	ucWrite_DataBuffer[71] = uiOption_closed ;
	ucWrite_DataBuffer[72] = bStep_Flag;
	ucWrite_DataBuffer[73] = bLevelReached;
	ucWrite_DataBuffer[74] = uihigh_low_vtlg;
	ucWrite_DataBuffer[75] = bFCTMode_Selection;
	ucWrite_DataBuffer[76] = bDelay_Finished;
	ucWrite_DataBuffer[77] = bSpecialEnduranceSelected;
	ucWrite_DataBuffer[78] = bEmegency_door_pull;
	ucWrite_DataBuffer[79] = bEco60Reset;
	ucWrite_DataBuffer[80] = Step_pos;
	
	ucWrite_DataBuffer[81] = bMotorError_E1;
	ucWrite_DataBuffer[82] = bMotorError_E2;
	ucWrite_DataBuffer[83] = bMotorError_E3;
	
	ucWrite_DataBuffer[84] = (uint8_t)(uispin_cyclecompleted & 0x00FF);
	ucWrite_DataBuffer[85] = (uint8_t)(uispin_cyclecompleted >> 8);
	ucWrite_DataBuffer[86] = (uint8_t)(ProgramLifeCycle & 0x00FF);
	ucWrite_DataBuffer[87] = (uint8_t)(ProgramLifeCycle >> 8);
	ucWrite_DataBuffer[88] = (uint8_t)(uiSF7_cyclecompleted & 0x00FF);
	ucWrite_DataBuffer[89] = (uint8_t)(uiSF7_cyclecompleted >> 8);
	ucWrite_DataBuffer[90] = (uint8_t)(uiSF6_cyclecompleted & 0x00FF);
	ucWrite_DataBuffer[91] = (uint8_t)(uiSF6_cyclecompleted >> 8);
	
	ucWrite_DataBuffer[92] = (uint8_t)(uiSF8_cyclecompleted & 0x00FF);
	ucWrite_DataBuffer[93] = (uint8_t)(uiSF8_cyclecompleted >> 8);
	ucWrite_DataBuffer[94] = uiSelected_Position ;
	ucWrite_DataBuffer[95] = uiMachineState;
	ucWrite_DataBuffer[96] = uiRpmSel;
	ucWrite_DataBuffer[97] = uiTempSel;
	ucWrite_DataBuffer[98] = uiTubClean_req;
	ucWrite_DataBuffer[99] = uiTubClean_Disp_count;
	ucWrite_DataBuffer[100] = uitubclean_cycle;
	
	ucWrite_DataBuffer[101] = uiPSER_Option_Avbl;
	ucWrite_DataBuffer[102] = uilast_selected_option ; // uidisp_select_option;
	ucWrite_DataBuffer[103] = Error_Storage[0];
	ucWrite_DataBuffer[104] = Error_Storage[1];
	ucWrite_DataBuffer[105] = Error_Storage[2];
	ucWrite_DataBuffer[106] = Error_Storage[3];
	ucWrite_DataBuffer[107] = Error_Storage[4];
	ucWrite_DataBuffer[108] = Error_Storage[5];
	ucWrite_DataBuffer[109] = Error_Storage[6];
	ucWrite_DataBuffer[110] = prev_program_pos;
	
	ucWrite_DataBuffer[111] = uiRMP_Default;
	ucWrite_DataBuffer[112] = uimaxspeed_temp;
	ucWrite_DataBuffer[113] = bDoorLockError;
	ucWrite_DataBuffer[114] = uiExtraRinse_Press_Select;
	ucWrite_DataBuffer[115] = TubClean_Activated;
	ucWrite_DataBuffer[116] = uiprev_error;
	ucWrite_DataBuffer[117] = bDoorLock;
	ucWrite_DataBuffer[118] = ui_prev_door_status;
	ucWrite_DataBuffer[119] = uiExtraRinse_Press_count;
	ucWrite_DataBuffer[120] = bDoorUnlocked;
	ucWrite_DataBuffer[121] = uiMachineBefore_Poweroff ;
	ucWrite_DataBuffer[122] = bHotRinse_Selected;

	ucWrite_DataBuffer[123] = Switch_Reset;
	ucWrite_DataBuffer[124] = bHPrs;
	ucWrite_DataBuffer[125] = bLPrs;
	ucWrite_DataBuffer[126] = bCold_Soak_Selected;
	ucWrite_DataBuffer[127] = uidefault_kg;
	ucWrite_DataBuffer[128] = getAIComm_ExecState_ui8();
	ucWrite_DataBuffer[129] = getAIComm_Program_ui8();
/*----------------------------------------------------------------------------------------------------------------------------	
	
	
	ucWrite_DataBuffer[130] = Reserved;
*///----------------------------------------------------------------------------------------------------------------------------	
#if ENABLE_IOT
	//ucWrite_DataBuffer[131] is used for "bFirmwareUpgrade" variable to remove 3Sec startup delay
	ucWrite_DataBuffer[132] = (uint8_t)(ProgramTime & 0x00FF);
	ucWrite_DataBuffer[133] = (uint8_t)(ProgramTime >> 8);
	ucWrite_DataBuffer[134] = BkMultiFuncSetting;
	//ucWrite_DataBuffer[135] = WiFi_Comn_Test;
#endif	
	ucWrite_DataBuffer[135]	= uiPrewashWaterResetCount;
	ucWrite_DataBuffer[136]	= uiFinalWt;
	ucWrite_DataBuffer[137]	= uiLS_Total_subtraction;
	ucWrite_DataBuffer[138]	= uiUB_decidation;
	ucWrite_DataBuffer[139]	= uiLS_Tx_subtraction;
	ucWrite_DataBuffer[140]	= uiTest_drain;
	ucWrite_DataBuffer[141] = uiLoadRange;
	ucWrite_DataBuffer[142] = L175_count;
	ucWrite_DataBuffer[143] = uiheat_temp;
	//ucWrite_DataBuffer[144]	= uiResetFill_Count; //ucWrite_DataBuffer[144] = bExpress_30_Activatted;
	ucWrite_DataBuffer[144]	= bLoadSensing_Completed;
	Memory_Mirror_Write();
}
//*******************************************************************************************************

void Display_Backup()
{
	 if(bProgramStart == TRUE)
	 	bProgram_Start = TRUE;
	
	 
	 if(uiCurrent_step_time == 0XFFFF)
	 {
		 uiCurrent_step_time = RESET_VALUE;
	 }
	 
	 if((uiCurrent_step_time > RESET_VALUE)&&(uiCurrent_step_time != 0XFFFF))
	 {
		 step_start = TRUE;
	 }
	 
	
	 if(ProgramLifeCycle == 0xFFFF)
	 {
		 ProgramLifeCycle = RESET_VALUE;
	 }
	 
	 if(uitubclean_cycle == 0xFF)
	 {
		 uitubclean_cycle = RESET_VALUE;
	 }
	
	if(uidelaymin > RESET_VALUE)
		bDelay_Start = TRUE;
		
	uiProgram_Position = uiSelected_Position;
	uiprev_reset_pos =  uiSelected_Position;
	
	
	if(uiBeforeState == PROGRAM_END)
	{
		uisnooze_mode_pos = uiProgram_Position;	
	}
	else
	{
		snoozemode_10minutes_counter = SNOOZEMODE_MINUTESCOUNT;
	}
	
	
	cdigits_data[0] = 0x00;cdigits_data[1] = 0x00;cdigits_data[2] = 0x00;
	cdigits_data[3] = 0x00;
	cdigits_data[4] = cdigits_data[4] & (~0x03);
	
	ProgSel_led();
	
	if(bChildLockButton == 1)
	{
		cdigits_data[5] =  cdigits_data[5] | (0x02); // // turn on the child lock led 
	}
	
}

void Option_Backup()
{
	if(bTestMode_Selection == FALSE)
	{
		if((uiprogram_option_status & 0x01) == 0x01)
		{
			bPrewash_Start = TRUE;
#if ENABLE_IOT
			set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_PREWASH);
#endif
			if(( uidisp_select_option & Prewash_Yes) == RESET_VALUE)
			{
				uiPSER_Option_Avbl =  uiPSER_Option_Avbl &(~ Prewash_Yes);
#if ENABLE_IOT
				clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_PREWASH);
#endif
			}
		}
		else
		{
			bPrewash_Start = FALSE;
			if(( uidisp_select_option & Prewash_Yes) == Prewash_Yes)
			{
#if ENABLE_IOT
				set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_PREWASH);
#endif
			}
		}
		
		if((uiprogram_option_status & 0x02) == 0x02)
		{
			bSoak_Start = TRUE;
#if ENABLE_IOT
			set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);
#endif
			if( (uidisp_select_option & WarmSoak_Yes) == RESET_VALUE)
			{
				uiPSER_Option_Avbl =  uiPSER_Option_Avbl &(~ WarmSoak_Yes);
#if ENABLE_IOT
					clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);
					//set_IOTSoakInMin(0x00);
#endif
			}
		}
		else
		{
			bSoak_Start = FALSE;
			if( (uidisp_select_option & WarmSoak_Yes) == WarmSoak_Yes)
			{
#if ENABLE_IOT
				set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);
#endif	
			}
		}
		
		if((uiprogram_option_status & 0x04) == 0x04)
		{
			 bExtraRinse_Start = TRUE;

			 if( (uidisp_select_option & ExtraRinse_Yes) == RESET_VALUE)
			 {
				uiPSER_Option_Avbl =  uiPSER_Option_Avbl &(~ ExtraRinse_Yes);
			 }
		}
		else
		{
			bExtraRinse_Start  = FALSE;			
		}
		
		if((uiprogram_option_status & 0x08) == 0x08)
		{
			bSteam_Start = TRUE;
#if ENABLE_IOT
			set_IOTOptionEnable(OPTION_ENABLE_2, OPTION_2_STEAM);
#endif
			if( (uidisp_select_option & Steam_Yes) == RESET_VALUE)
			{
				uiPSER_Option_Avbl =  uiPSER_Option_Avbl &(~ Steam_Yes);
#if ENABLE_IOT
				clr_IOTOptionEnable(OPTION_ENABLE_2, OPTION_2_STEAM);
#endif
			}
		}
		else
		{
			bSteam_Start  = FALSE;
			if( (uidisp_select_option & Steam_Yes) == Steam_Yes)
			{
#if ENABLE_IOT
				set_IOTOptionEnable(OPTION_ENABLE_2, OPTION_2_STEAM);
#endif	
			}
		}
		
		if((uiprogram_option_status & 0x10) == 0x10)
		{
			bDelay_Start1 = TRUE;
#if ENABLE_IOT
			set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_DELAYSTART);
#endif
			if(( uidisp_select_option & DelayStart_Yes) == RESET_VALUE)
			{
				uiPSER_Option_Avbl =  uiPSER_Option_Avbl &(~ DelayStart_Yes);
#if ENABLE_IOT
				clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_DELAYSTART);
#endif
			}
		}
		else
		{
			bDelay_Start1 = FALSE;
			if(( uidisp_select_option & DelayStart_Yes) == DelayStart_Yes)
			{
#if ENABLE_IOT
				set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_DELAYSTART);
#endif
			}
		}
		
		if((uiprogram_option_status & 0x20) == 0x20)
		{
		 	bTimeSaver_Start = TRUE;
#if ENABLE_IOT
			set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_TIMESAVER);
#endif
			if( (uidisp_select_option & TimeSaver_Yes) == RESET_VALUE)
			{
				uiPSER_Option_Avbl =  uiPSER_Option_Avbl &(~ TimeSaver_Yes);
#if ENABLE_IOT
					clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_TIMESAVER);
#endif
			}
		}
		else
		{
			bTimeSaver_Start = FALSE;
			if( (uidisp_select_option & TimeSaver_Yes) == TimeSaver_Yes)
			{
#if ENABLE_IOT
				set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_TIMESAVER);
#endif
			}
		}
#if ENABLE_IOT		
		if(bCold_Soak_Selected)
		{
			set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_SOAK);
		}
		else
		{
			set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_SOAK);
		}
#endif
	}
}

