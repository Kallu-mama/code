
#include "AI_Comm.h"
#include "AI_Comm_Global.h"



/*----------------------------AI_COMM_PACKET----------------------------------------------------------*/
AI_COMM_PACKET AI_Comm_Object[NO_OF_HEADER]=
			{	/*HeaderType	UnusedByte	Resquest	HeaderType Length*/
				{PRS_HEADER,	0x00,		REQ,		0,	0x85},		
			};

/*---------------------------Extern---------------------------------*/
extern uint8_t uiTempSel,uiRpmSel,uiSelected_Position,uiPresent_Step;
extern uint8_t uiMachineState,uiTumblingCount;
extern uint16_t AI_WaterLevel,iWaterLevel;
extern uint8_t uidisp_select_option;
extern uint8_t uioption_on;

/*-----------------------------Global Variable Declaration--------------------------------*/
/*static*/ uint8_t AI_CurrentState_ui8, AI_HeaderType_ui8,AI_ExecState_ui8,uiAISelected_Position;
/*static*/ uint8_t *ArrRef_Ptr_ui8;
AI_COMM_PACKET *AI_Comm_Object_ptr = NULL;
uint8_t AI_Recieve_Buf[AI_CLOTH_BYTE_HEADER_LENGTH];
uint8_t WaterFreqAvg_Arr_ui8[AI_TRANSMIT_BUF_SIZE];
uint8_t AI_WaterFreq_State_ui8;
uint16_t WaterFreqAvg_ui16 = RESET_VALUE;
//uint16_t WaterFreqData_ui16 = RESET_VALUE;
/*************************************************************************************************/
uint8_t Reset_WaterLevel_Index[20];
uint8_t Set_WaterLevel_Index[20];
extern boolean AI_WaterFreqGet_b,bMotor_Start;
uint8_t reset_time_index;
uint8_t set_time_index;
volatile uint8_t set_flag;
volatile uint8_t reset_flag;
uint8_t previous_state;
uint8_t Motor_Start_Previous_State;

/*--------------------------------Static Declaration-------------------------------------*/
static boolean AI_Idle_Do(void);
static boolean AI_Data_Computation();
static boolean AI_Transmit_Do(void);
static boolean AI_Recieve_Do(void);
static boolean AI_Exit_Do(void);			
/*---------------------------------------------------------*/


boolean (*AI_StatesExecution[])(void)= {
	&AI_Idle_Do,
	&AI_Data_Computation,
	&AI_Transmit_Do,
	&AI_Recieve_Do,
	&AI_Exit_Do,
};


#if ENABLE_IOT

/***********************************************************************
* Function_Name : AI_State(void).
* Ret_type	: void.
* Working	: AI State Change.
* Developed By	: Prashant_H
* Date		: 11/03/2022
************************************************************************/
static void AI_State()
{
	switch(AI_CurrentState_ui8)
	{
		case AI_IDLE:
			AI_CurrentState_ui8 = AI_TRANSMIT;
			AI_Idle_Do();
			break;
		case AI_TRANSMIT:
			AI_CurrentState_ui8 = AI_TRANSMIT;
			AI_Transmit_Do();
			break;
		case AI_RECIEVE:
			AI_CurrentState_ui8 = AI_RECIEVE;
			AI_Recieve_Do();
			break;
		case AI_EXIT:
			AI_CurrentState_ui8 = AI_EXIT;
			AI_Exit_Do();
			break;
		default:
			break;
	}
}

/***********************************************************************
* Function_Name : AI_Comm_Entry(uint8_t header_type).
* Ret_type	: void.
* Working	: AI Entry.
* Developed By	: Prashant_H
* Date		: 11/03/2022
************************************************************************/
void AI_Comm_Entry(uint8_t header_type)
{
	memset(AI_Recieve_Buf, 0, AI_CLOTH_BYTE_HEADER_LENGTH);
	memset(WaterFreqAvg_Arr_ui8,0,AI_TRANSMIT_BUF_SIZE);
	AI_HeaderType_ui8 	= header_type;
	AI_CurrentState_ui8 	= AI_DATA_COMPUTATION;
	AI_WaterFreq_State_ui8  = ENTRY;
	AI_ExecState_ui8 	= AI_Idle;
}

/***********************************************************************
* Function_Name : AI_Idle_Do(void).
* Ret_type	: Boolean.
* Working	: AI Idle.
* Developed By	: Prashant_H
* Date		: 11/03/2022
************************************************************************/
static boolean AI_Idle_Do()
{
	static boolean ret_b;
	AI_Comm_Entry(PRS_HEADER_LOC);
	return ret_b;

}

/***********************************************************************
* Function_Name : AI_Data_Computation().
* Ret_type	: boolean.
* Working	: Get 200SEC data of water frequency for every 1SEC.
* Developed By	: Prashant_H
* Date		: 11/03/2022
************************************************************************/
boolean AI_Data_Computation()
{
	static uint16_t index = 5;
	static boolean ret_b;
	
	switch(AI_WaterFreq_State_ui8)
	{
		case ENTRY:
			WaterFreqAvg_ui16 = ret_b = RESET_VALUE;
			//WaterFreqData_ui16 = RESET_VALUE;
			index = 5;	// TODO: Index is 5 becuase fist 5 payload will fill in AI_Transmit state ---> AI_Comm.c
			setAIWaterFreq_ReadEnable(FALSE);
			AI_WaterFreqGet_b = FALSE;
			if(uiMachineState == START_PRESSED)
			{	
				if((iWaterLevel < EMPTYLEVEL)&& (uiTumblingCount == OPTIONAL_PREWASH || uiTumblingCount == FILLING)) //(iWaterLevel < AI_WATER_LEVEL)
				{
					AI_Comm_Entry(PRS_HEADER_LOC);
					Timer_CountdownTimer(START_TIMER, AI200Sec_TIMER);
					Timer_CountdownTimer(START_TIMER, AI_1Sec_Interveval_TMER);
					AI_WaterFreq_State_ui8 = DO;
					AI_CurrentState_ui8 = AI_DATA_COMPUTATION;
					AI_ExecState_ui8 = AI_Runing;
				}
			}
			else
			{
				Timer_CountdownTimer(PAUSE_TIMER, AI200Sec_TIMER);
				Timer_CountdownTimer(PAUSE_TIMER, AI_1Sec_Interveval_TMER);
				AI_WaterFreq_State_ui8 = ENTRY;
			}
			ret_b = FALSE;
			break;
		case DO:
			if(Timer_CountdownTimer(CHECK_TIMER, AI200Sec_TIMER) == RESET_VALUE)
			{	
				if(Timer_CountdownTimer(CHECK_TIMER, AI_1Sec_Interveval_TMER) == TRUE )
				{
						
						//WaterFreqData_ui16 = (uint16_t)iWaterLevel;
						WaterFreqAvg_Arr_ui8[index++] = (uint8_t)((iWaterLevel & (0xFF00))>>8);
						WaterFreqAvg_Arr_ui8[index++] = (uint8_t)(iWaterLevel & (0xFF));
						//WaterFreqData_ui16 = RESET_VALUE; 
						
						if((bMotor_Start == YES) && (Motor_Start_Previous_State == 0))
						{
							WaterFreqAvg_Arr_ui8[681] =  (uint8_t)((index &(0xFF)));
							Motor_Start_Previous_State = 1;
						}
						//Below ports are the Pewash Valve and Main wash valve to indicate the reset index
						if(((P1.6 == 1) || (P5.1 == 1)) && (previous_state == 0))
						{
							reset_flag = 1;
							previous_state = 1;
						}
						//&&  condition is added to check both the valve should remain close to indicate the set index
						else if(((previous_state == 1)&&((P1.6 == 0)&&(P5.1 == 0))))
						{
							set_flag = 1;
							previous_state = 0;
						}
						
						//Store the index of set and reset for the AI logic build this is to be transmitted
						if(reset_flag == 1)
						{
							Reset_WaterLevel_Index[reset_time_index++] = (uint8_t)((index &(0xFF00))>>8);
							Reset_WaterLevel_Index[reset_time_index++] = (uint8_t)((index &(0xFF)));
							reset_flag = 0;
						}
						
						if(set_flag == 1)
						{
							Set_WaterLevel_Index[set_time_index++] = (uint8_t)((index &(0xFF00))>>8);
							Set_WaterLevel_Index[set_time_index++] = (uint8_t)((index &(0xFF)));
							set_flag = 0;
						}
						
						AI_WaterFreq_State_ui8 = DO;
						AI_WaterFreqGet_b = FALSE;
						AI_CurrentState_ui8 = AI_DATA_COMPUTATION;
						Timer_CountdownTimer(START_TIMER, AI_1Sec_Interveval_TMER);
					
				}
			}
			else
			{
				index = 5;
				setAIWaterFreq_ReadEnable(FALSE);
				AI_CurrentState_ui8 = AI_TRANSMIT;
				AI_WaterFreq_State_ui8 = EXIT;
				ret_b = TRUE;
				reset_time_index = 0;
				set_time_index = 0;
				Motor_Start_Previous_State = 0;
			}
			if(uiMachineState != START_PRESSED)
			{
				AI_WaterFreq_State_ui8 = LOAD_OPERATION;	
			}
			break;
		case EXIT:
			WaterFreqAvg_ui16 = RESET_VALUE;
			//WaterFreqData_ui16 = RESET_VALUE;
			setAIWaterFreq_ReadEnable(FALSE);
			//AI_WaterFreqGet_b = FALSE;
			index = 5;
			ret_b = TRUE;
			reset_time_index = 0;
			set_time_index = 0;
			Motor_Start_Previous_State = 0;
			AI_WaterFreq_State_ui8 = ENTRY;
			break;
			
		case LOAD_OPERATION:/*IF Machine state is not in start Pressed*/
			if(uiMachineState  == START_PRESSED)
			{
				Timer_CountdownTimer(RESUME_TIMER, AI200Sec_TIMER);
				AI_WaterFreq_State_ui8 = DO;
				setAIWaterFreq_ReadEnable(FALSE);
			}
			else
			{
				Timer_CountdownTimer(PAUSE_TIMER, AI200Sec_TIMER);
				AI_WaterFreq_State_ui8 = LOAD_OPERATION;
			}
			break;
		default:
			break;
			
	}
	return ret_b;
}

/***********************************************************************
* Function_Name : AI_Transmit_Do(void).
* Ret_type	: Boolean.
* Working	: AI data transmission.
* Developed By	: Prashant_H
* Date		: 11/03/2022
************************************************************************/
static boolean AI_Transmit_Do()
{
	static uint16_t index ,itr_ui16;
	uint8_t i;
	static boolean ret_b;
	
	AI_Comm_Object_ptr = &AI_Comm_Object[AI_HeaderType_ui8];

	switch(AI_Comm_Object_ptr->AI_Header)
	{
		case PRS_HEADER:				
				WaterFreqAvg_Arr_ui8[0] = AI_Comm_Object_ptr->AI_Header;
				WaterFreqAvg_Arr_ui8[1] = AI_Comm_Object_ptr->AI_DataLength_ui8;
				WaterFreqAvg_Arr_ui8[2] = AI_Comm_Object_ptr->Req_Resp;
				WaterFreqAvg_Arr_ui8[3] = uiSelected_Position;
				WaterFreqAvg_Arr_ui8[4] = AI_Comm_Object_ptr->Machine_Type_ui8;
				for(i=0;i<40;i++)
				{
					/*Logic Used for filling set reset index in the water freq array[]*/
					if(i<20)
					{
						WaterFreqAvg_Arr_ui8[640+i] = Reset_WaterLevel_Index[i];
					}
					else if(i<40)
					{
						WaterFreqAvg_Arr_ui8[640+i] = Set_WaterLevel_Index[i-20];
					}
				}
				R_UART2_Send(&WaterFreqAvg_Arr_ui8[0],AI_TRANSMIT_BUF_SIZE);
				AI_CurrentState_ui8 = AI_RECIEVE;
				ret_b = FALSE;
				Timer_CountdownTimer(START_TIMER,AI_DATA_RecieveTimeout_Status);
			break;
		default:
			break;
	}
	AI_Comm_Object_ptr = NULL;
	index = RESET_VALUE;
	AI_ExecState_ui8 = AI_Runing;
	return ret_b;
}

/***********************************************************************
* Function_Name : AI_Recieve_Do(void).
* Ret_type	: Boolean.
* Working	: AI data reception.
* Developed By	: Prashant_H
* Date		: 11/03/2022
************************************************************************/
static boolean AI_Recieve_Do()
{
	static boolean ret_b;

	switch(AI_Recieve_Buf[0])
	{
		case PRS_HEADER:
			switch(AI_Recieve_Buf[5])
			{
				case AI_CLOTH_COTTON_100:
						uiAISelected_Position = COTTON;
						//if((uiSelected_Position == COTTON) || ((uidisp_select_option & Prewash_Yes) == Prewash_Yes))
						//{
							uiPresent_Step = uiPresent_Step;	
						/*}
						else
						{
							uiPresent_Step = uiPresent_Step + 1;  //Incremented to match the Present step with the balance time.Present step of syn and cotton are having diffetent positions
						}*/
						ret_b = TRUE;
						AI_CurrentState_ui8 = AI_EXIT;
						AI_ExecState_ui8 = AI_Done;
						break;
				case AI_CLOTH_SYNTH_100:
						uiAISelected_Position = SYNTHETIC;
						//if((uiSelected_Position == SYNTHETIC) || ((uidisp_select_option & Prewash_Yes) == Prewash_Yes))
						//{
							uiPresent_Step = uiPresent_Step;
						//}
						//else
						//{	
							//uidisp_select_option = uidisp_select_option & (~Steam_Yes);
							//uioption_on = (uioption_on & (~1<<3));
							
							//uiPresent_Step = uiPresent_Step - 1;  //Decremented to match the Present step with the balance time.Present step of syn and cotton are having diffetent positions
						
						//}
						ret_b = TRUE;
						AI_CurrentState_ui8 = AI_EXIT;
						AI_ExecState_ui8 = AI_Done;
						break;
				case AI_CLOTH_COTTON_50_SYNTH_50:
				case AI_CLOTH_COTTON_25_SYNTH_75:
				case AI_CLOTH_COTTON_75_SYNTH_25:
						
						uiAISelected_Position = MIXEDSOIL;
						//if((uiSelected_Position == COTTON) && ((uidisp_select_option & Prewash_Yes) != Prewash_Yes))
						//{
						//	uiPresent_Step = uiPresent_Step - 1;	
						//}
						//else
						//{
							uiPresent_Step = uiPresent_Step;  //Incremented to match the Present step with the balance time.Present step of syn and cotton are having diffetent positions
						//}
						//uiPresent_Step = uiPresent_Step;
						ret_b = TRUE;
						AI_CurrentState_ui8 = AI_EXIT;
						AI_ExecState_ui8 = AI_Done;
						break;
						
				default:	if(Timer_CountdownTimer(CHECK_TIMER,AI_DATA_RecieveTimeout_Status)) 
						{	
							ret_b = TRUE; 
							AI_CurrentState_ui8 = AI_EXIT;
							AI_ExecState_ui8 = AI_Fail;
							
						}
						else{ret_b = FALSE;}
						
					break;
			}
			break;
		default:
			if(Timer_CountdownTimer(CHECK_TIMER,AI_DATA_RecieveTimeout_Status)) 
			{
				ret_b = TRUE; 
				AI_CurrentState_ui8 = AI_EXIT;
				AI_ExecState_ui8 = AI_Fail;
			}
			else{ret_b = FALSE;}
			
			break;
	}
	if(AI_ExecState_ui8 == AI_Done)
	{
		AI_ProgramTimeUpdate();
	}
	return ret_b;
}

/***********************************************************************
* Function_Name : AI_Comm_Run(void).
* Ret_type	: Boolean.
* Working	: AI state runing.
* Developed By	: Prashant_H
* Date		: 11/03/2022
************************************************************************/
boolean AI_Comm_Run()
{
	return (*AI_StatesExecution[AI_CurrentState_ui8])();
}

/***********************************************************************
* Function_Name : AI_Exit_Do(void).
* Ret_type	: Boolean.
* Working	: AI state exit.
* Developed By	: Prashant_H
* Date		: 11/03/2022
************************************************************************/
static boolean AI_Exit_Do()
{
	static boolean ret_b;
	memset(AI_Recieve_Buf, 0, AI_CLOTH_BYTE_HEADER_LENGTH);
	memset(WaterFreqAvg_Arr_ui8,0,AI_TRANSMIT_BUF_SIZE);
	return ret_b;
}

/***********************************************************************
* Function_Name : AI_Enable_Status(void).
* Ret_type	: Boolean.
* Working	: To check AI is applicable for cycle as well as depend on temperature & rpm.
* Developed By	: Prashant_H
* Date		: 11/03/2022
************************************************************************/
boolean AI_Enable_Status()
{
	static boolean ret_b;
	if(AI_Option_Object[uiSelected_Position].AI_En_b == TRUE)
	{
		if((uiTempSel <= AI_Option_Object[uiSelected_Position].max_option_ui8[AI_TEMP_LOC])
			&& (uiRpmSel <= AI_Option_Object[uiSelected_Position].max_option_ui8[AI_RPM_LOC])) 	
		{
			if((uiTempSel == WashCold) || (uiTempSel == Wash30) || (uiTempSel == Wash40) || 
				(uiTempSel == P_COLD) || (uiTempSel == P_30) || (uiTempSel == P_40))
				{
					ret_b = TRUE;
				}
				else 
				{
					ret_b = FALSE;
				}
			//ret_b = TRUE;
		}
		else
		{
			ret_b = FALSE;
		}
	}
	else
	{
		ret_b = FALSE;
	}
	return ret_b;
}

/***********************************************************************
* Function_Name : getAIComm_ExecState_ui8(void).
* Ret_type	: uint8_t.
* Working	: To get AI communication state.
* Developed By	: Prashant_H
* Date		: 11/03/2022
************************************************************************/
uint8_t getAIComm_ExecState_ui8()
{
	return AI_ExecState_ui8;
}

/***********************************************************************
* Function_Name : getAIComm_Program_ui8(void).
* Ret_type	: uint8_t.
* Working	: To get AI selected program.
* Developed By	: Prashant_H
* Date		: 11/03/2022
************************************************************************/
uint8_t getAIComm_Program_ui8()
{
	return uiAISelected_Position;
}

/***********************************************************************
* Function_Name : setAIComm_State_ui8(void).
* Ret_type	: void.
* Working	: To set the AI execution state.
* Developed By	: Prashant_H
* Date		: 11/03/2022
************************************************************************/
void setAIComm_State_ui8(uint8_t state)
{
	AI_ExecState_ui8 = AI_CurrentState_ui8 = state;
}

/***********************************************************************
* Function_Name : setAIComm_Program_ui8(void).
* Ret_type	: void.
* Working	: To set AI selected program.
* Developed By	: Prashant_H
* Date		: 11/03/2022
************************************************************************/
void setAIComm_Program_ui8(uint8_t AI_SelectedProgram)
{
	uiAISelected_Position = AI_SelectedProgram;
}

/***********************************************************************
* Function_Name : AI_Execution(void).
* Ret_type	: void.
* Working	: To Execute the Function.
* Developed By	: Prashant_H
* Date		: 11/03/2022
************************************************************************/
void AI_Execution()
{
	if(AI_Enable_Status() )
	{
		if(getAIComm_ExecState_ui8() != AI_Done)
		{
			if(AI_Comm_Run())
			{
				;
			}
		}
	}
}

#endif 
