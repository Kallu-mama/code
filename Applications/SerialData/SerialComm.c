#include "SerialComm.h"
#include "r_cg_serial.h"

extern boolean bMotor_Start,bDoorLockStatus,bUnbalanceError,uiUB_decidation;
extern uint8_t uiMachineState,uiTumblingState,uiOperationSelected,uiRpmSel,uiTempSel;
extern uint8_t uiSelected_Position,StepTime,uiStep_Seconds,uiTestModeState,uiSmartProgram_Selected;
extern uint16_t iWaterLevel,uiSupplyVoltage,uiRPMValue;

extern uint8_t UartRxBuf[30];
extern SystemError MachineErrors;
uint8_t test_error,uiFinalWt;
uint16_t uiLS_RPMValue,uiN_RPMValue;
//********* Local Variables ***********//
boolean bSpin_Finished,bSpecialMotion_Finished,bUnbalance_Detected;
boolean bZcdDetection,bZCD_Display;
boolean bFoamDetection,bMotorError_E1,bMotorError_E2,bMotorError_E3,bComm_MB;
boolean bSpin_Status,bCommunicationError,bMotorError_Unbalnce;

uint8_t uiMotorState_Special,uiMotorState_Tumble,uiMotorState_Spin,uiIPM_Temperature;
uint8_t MachineStatus[30],uiMotor_minutes,uiUnbalance_Level,uiMotorError,uiUnbalance_Value;
uint8_t uiBot_Prod_Version,uiBot_test_Version,uiSelected_Duty,uiBot_Ldf_Version;
uint8_t uiMotorState_Run_Stop,uiMotorState_Washstate,uiMotorState_Duty_MB;
uint8_t Spin_Final_Profile_Index,uiSW_VF_MD;
//***************Serial Data*************************//
void SendCommand()
{
	
	uint8_t index = 0,i=0;
	uint8_t checksum1=0,checksum2=0;
	uint8_t WaterLevelHigh,WaterLevelLow;
	uint16_t WaterLevel;
	uint16_t Check_sum16;
	
	WaterLevel = iWaterLevel;
	WaterLevelHigh = (uint8_t)((WaterLevel >> 8)& 0xff);
	WaterLevelLow = (uint8_t)((WaterLevel >> 0)& 0xff);
	
	MachineStatus[index++]= 0x80;//HEADER;	            //0
	MachineStatus[index++]= DEST_NODE;          // 1
	MachineStatus[index++]= MESSAGE_TYPE;       // 2
	MachineStatus[index++]= LENGTH;             // 3
	MachineStatus[index++]= uiMachineState;     // 4
	MachineStatus[index++]= uiSelected_Position; // 5
	MachineStatus[index++]= uiTestModeState;    // 6
	MachineStatus[index++]= bMotor_Start;       // 7
	MachineStatus[index++]= uiTumblingState;    // 8
	MachineStatus[index++]= uiSelected_Duty;    // 9
	MachineStatus[index++]= StepTime;           // 10
	MachineStatus[index++]= uiRpmSel;           // 11
	MachineStatus[index++]= uiTempSel;          // 12
	MachineStatus[index++]= bComm_MB;           // 13
	MachineStatus[index++]= uiUB_decidation;    // 14
	MachineStatus[index++]= 0xFF;	   	    // 15
	MachineStatus[index++]= 0xFF;	   	    // 16
	MachineStatus[index++]= 0xFF;	   	    // 17
	MachineStatus[index++]= 0xFF;	   	    // 18
	MachineStatus[index++]= 0xFF;	   	    // 19
	MachineStatus[index++]= 0xFF;	   	    // 20
	MachineStatus[index++]= 0xFF;	   	    // 21
	MachineStatus[index++]= 0xFF;	   	    // 22
	MachineStatus[index++]= 0xFF;	   	    // 23
	MachineStatus[index++]= 0xFF;	   	    // 24
	MachineStatus[index++]= 0xFF;	   	    // 25
	MachineStatus[index++]= 0xFF;	   	    // 26
	MachineStatus[index++]= 0xFF;	   	    // 27

	Check_sum16 = Calculate_Checksum(&MachineStatus[0],28);
	checksum2 = (uint8_t)((Check_sum16 >> 8)& 0xff);
	checksum1 = (uint8_t)((Check_sum16 >> 0)& 0xff);
	
	MachineStatus[index++]= checksum1;	    // 28
	MachineStatus[index++]= checksum2;	    // 29
	
	/*for(i=0; i< LENGTH ; i++)
	checksum1 = checksum1+ MachineStatus[i];
	checksum2 = checksum1 + checksum1;
	MachineStatus[index++]=checksum1 ;
	MachineStatus[index++]=checksum2 ;*/
	
	R_UART1_Send(&MachineStatus[0],LENGTH);
	
}

//***************** Recieve Data ******************//
void DataRecieved()
{
	static uint8_t uiData_Recieve_Count;
	uint8_t Rx_checksum1=0,Rx_checksum2=0;
	uint16_t Rx_Check_sum16 =0x00;
	uint8_t index =0x00;
	
	Rx_Check_sum16 =  Calculate_Checksum(&UartRxBuf[0],28);
	Rx_checksum2 = (uint8_t)((Rx_Check_sum16 >> 8)& 0xff);
	Rx_checksum1 = (uint8_t)((Rx_Check_sum16 >> 0)& 0xff);
	if((Rx_checksum2 == UartRxBuf[29])&&(Rx_checksum1 == UartRxBuf[28]))
	{
		 uiSW_VF_MD = UartRxBuf[2]; //for differentiating between VF and IFB motor bottom board software//VF-0, MD-1
	         uiMotorState_Run_Stop 	= UartRxBuf[4] ;
		 uiMotorState_Washstate = UartRxBuf[5];
		 uiMotorState_Duty_MB 	= UartRxBuf[6];				 
		 uiSupplyVoltage 	= UartRxBuf[8] ;
		 uiSupplyVoltage 	= (uiSupplyVoltage << 8) + UartRxBuf[7];
		 uiRPMValue 		= UartRxBuf[10] ;
		 uiRPMValue 		= (uiRPMValue << 8) + UartRxBuf[9];	 
		 uiIPM_Temperature 	= UartRxBuf[11] ;
		 uiUnbalance_Value 	= UartRxBuf[13] ;
		 uiUnbalance_Value 	= (uiRPMValue << 8) + UartRxBuf[12];
		 uiUnbalance_Level 	= UartRxBuf[14];
		 uiMotorError 		= UartRxBuf[15];
		 bSpin_Status = bSpin_Finished = UartRxBuf[16];
		 bUnbalance_Detected   = UartRxBuf[17];
		 uiBot_Prod_Version 	= UartRxBuf[18];
		 uiBot_test_Version 	= UartRxBuf[19];
		// uiSupplyVoltage = 230; // added 
		 if((UartRxBuf[20] == 0x01 )||(UartRxBuf[20] == 0x00)){
		 	bZCD_Display = bZcdDetection = UartRxBuf[20];}
		 else {
			 bZCD_Display = bZcdDetection = 0x00;
		 }
		 uiLS_RPMValue 		=  UartRxBuf[22] ;
		 uiLS_RPMValue 		=  (uiLS_RPMValue << 8) + UartRxBuf[21];	 
		 
		 bComm_MB = UartRxBuf[23];
		 uiBot_Ldf_Version = UartRxBuf[26];//byte num 26
		 Spin_Final_Profile_Index = UartRxBuf[27];
		 
		 if(bSpin_Finished == TRUE)
		 {
			 bSpin_Status = TRUE;
		 }
		 switch(uiMotorError)
		 {
			 case 1:
			 	bMotorError_E1 = TRUE;
				break;
			 case 2:
			 	bMotorError_E2 = TRUE;
				break;
			 case 3:
			 	bMotorError_E3 = TRUE;
				break;
			 /*case 4:
			 	bMotorError_Unbalnce = TRUE;
				bUnbalanceError = TRUE;
				break;*/	
			default:
				break;
	       }	
	}
	/*
	 UartRxBuf[0];
	 uiMotorState_Special = UartRxBuf[1] ;
	 uiMotorState_Tumble = UartRxBuf[2];
	 uiMotorState_Spin = UartRxBuf[3];
	 uiMotor_minutes = UartRxBuf[4];
	 uiUnbalance_Level = UartRxBuf[5];
	 uiSupplyVoltage = UartRxBuf[6] ;
	 uiSupplyVoltage = (uiSupplyVoltage << 8) + UartRxBuf[7];
	 uiRPMValue = UartRxBuf[8] ;
	 uiRPMValue = (uiRPMValue << 8) + UartRxBuf[9];
	 uiMotorError = UartRxBuf[10] ;
	 uiUnbalance_Value = UartRxBuf[11] ;
	 uiIPM_Temperature = UartRxBuf[12] ;
	bSpin_Status=bSpin_Finished = UartRxBuf[13] ;
	 bSpecialMotion_Finished = UartRxBuf[14];
	 bUnbalance_Detected = UartRxBuf[15];
	 bZCD_Display = bZcdDetection = UartRxBuf[16];
	 bComm_MB = UartRxBuf[17];
	 uiBot_Prod_Version = UartRxBuf[18];
	 uiBot_test_Version = UartRxBuf[19];
	 
	  
	 if(bSpin_Finished == TRUE)
	 {
		 bSpin_Status = TRUE;
	 }
	 
	 switch(uiMotorError)
	 {
		 case 1:
		 	bMotorError_E1 = TRUE;
			break;
		 case 2:
		 	bMotorError_E2 = TRUE;
			break;
		 case 3:
		 	bMotorError_E3 = TRUE;
			break;
		 case 4:
		 	bMotorError_Unbalnce = TRUE;
			bUnbalanceError = TRUE;
			break;	
		default:
			break;
	 }*/	  		 
}

static uint16_t Calculate_Checksum(uint8_t *Msg_ui8,uint8_t MsgLength_ui8)
{
	uint16_t Total_ui16 = RESET_VALUE;
	uint8_t Index_uiu8 = RESET_VALUE;
  
	for (Index_uiu8 = RESET_VALUE ; Index_uiu8 < MsgLength_ui8 ; Index_uiu8++)
	{
	    	Total_ui16 += (uint16_t)*(Msg_ui8++);                     
      }
	return (Total_ui16);	
}


void motor(uint8_t uiDuty_Set)
{
	uiSelected_Duty = uiDuty_Set;
}
