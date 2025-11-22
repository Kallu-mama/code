
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#if ENABLE_IOT

#include "r_cg_serial.h"
#include "iot_config.h"
#include "iot_user_config.h"
#include "iot_comm.h"
#include "iot_functions.h"
#include "string.h"

//Used for Serial communication
static uint8_t recvByteCount = 0;
static uint8_t recvLength = 0;
static uint8_t recvBuf[30];
uint8_t *g_ucUARTBuff = recvBuf;
extern uint8_t AI_Recieve_Buf[AI_CLOTH_BYTE_HEADER_LENGTH];
//State machine variable
uint8_t recvState;

static uint8_t sendAck;
static uint8_t userOrSettingCntrl;

/*make extern*/
static boolean bRebootFlag;
static uint8_t selectCode;
static uint8_t selectIndex;

//unsigned char control=0x00;

uint8_t WiFiMode;
extern uint8_t WiFiSoftware[3];
extern uint8_t BkMultiFuncSetting;

static uint8_t settingQuery;
static uint8_t progSettingQuery;
static uint8_t userSettingQuery;

uint8_t userControl,settingControl;//in functions revised
uint8_t programSel;
uint16_t uiMacAddress;
extern uint8_t data[DATA_SIZE];//iot funct



/**********************************************************************
* Function:  void uartReceive(uint8_t dataReceive)
* Parameters: uint8_t dataReceive - data received from serial interrupt
* Return: None
* Description: Handle data received state machine for IOT protocol
***********************************************************************/
void uartReceive(uint8_t dataReceive) 
{	static uint8_t index_ui8 = 0;
	static boolean prs_header_b;
	//Received data to buffer
	*g_ucUARTBuff = dataReceive;	//for state machine logic
	
	
	switch(dataReceive)
	{
		case 0x96:/*PRS_HEADER*/
				AI_Recieve_Buf[index_ui8++] = dataReceive;
				prs_header_b = TRUE;
				break;
			
		default:
			if(prs_header_b)
			{
				AI_Recieve_Buf[index_ui8++] = dataReceive;
				if(index_ui8 > AI_CLOTH_BYTE_HEADER_LENGTH)
				{
					prs_header_b = 0;index_ui8 = 0;
				}
			}
			else
			{	prs_header_b = 0;
				index_ui8 = 0;
			}
			break;
	}

	switch(recvState)
	{
		case CNTRL_IDLE:
			//Check for header byte
			if(*g_ucUARTBuff == HEADER)
			{
				g_ucUARTBuff++;
				recvState = CNTRL_HEADER;
				memset(data,'\0',DATA_SIZE);
			}
			else
			{
				*g_ucUARTBuff = 0x00;
			}
			break;
		//if header received, then check for commnad 	
		case CNTRL_HEADER:
			//Latch received length
			recvLength = *g_ucUARTBuff;
			//Fixing length to be less than 20 data bytes for command
			if(recvLength <= CMD_LENGTH)
			{          	
				g_ucUARTBuff++;
				recvState = CNTRL_LENGTH;
				recvByteCount = 0;        		//Reset byte_count value before data bytes received
			}
			else
			{
				*g_ucUARTBuff = 0x00;
			}
			break;
			
		case CNTRL_LENGTH:
			if(recvByteCount <= recvLength)
			{
				data[recvByteCount]=*g_ucUARTBuff;
				recvByteCount++;
				g_ucUARTBuff++;

				if(recvByteCount == recvLength)
					recvState = CNTRL_DATA;
				else
					break;
			}
			else //added for 1second wifi comm update
			{
				break;
			}
				
		case CNTRL_DATA:
			recvState = CNTRL_IDLE;
			if(checkSum1())
			{
				if(checkSum2())
				{
					g_ucUARTBuff = recvBuf; //allocate 30 byte array length
					recvByteCount = 0 ;
					recvLength = 0;
					//Latch byte
					userOrSettingCntrl = data[0];
					selectIndex = data[4];
					//Check for user control
					if(userOrSettingCntrl == USER_CONTROL)
					{
						//Compair appliance type
						if(data[2] == get_IOTApplianceType())
						{
							//Update byte for
							userControl = data[3];
							WiFiMode = data[4];
							WiFiSoftware[0] = data[5];
							WiFiSoftware[1] = data[6];
							WiFiSoftware[2] = data[7];

							userSettingQuery = SET;

							//Received user control command
							if(userControl == CMD_START)
							{
								set_IOTState(STATE_IOT_START);	//Assign state STATE_IOT_START
							}
							if(userControl == CMD_PAUSE)
							{
								set_IOTState(STATE_IOT_PAUSE);  //Assign state STATE_IOT_PAUSE
							}
							if(userControl == CMD_SLEEP)
							{
								set_IOTState(STATE_IOT_STANDBY); //Assign state STATE_IOT_STANDBY
							}
						}
						else
						{
							//Appliance code does not match
							set_SendAck(3);
							memset(recvBuf,0,30);
							memset(data,'\0',DATA_SIZE);
							break;
						}
					}
					else if(userOrSettingCntrl == SETTING_CONTROL)
					{
						//Assign 
						settingControl = data[2];
						if(settingControl == MACID_SETTING)
						{
							uiMacAddress = data[3];
							uiMacAddress = (uiMacAddress << 8)+data[4];
						}
						else
						{
							programSel = selectCode = data[3];
							selectIndex = data[4];
							//Set flag for communication
							settingQuery = SET;
						}
					}
					else if(userOrSettingCntrl == PROGRAM_CONTROL)
					{
						programSel = data[3];
						set_IOTprogram(programSel);
						progSettingQuery = SET;
					}
					//Assign Ack state
					set_SendAck(1);
					break;
				   }
				   else
				   {
					   //Assign buffer
					   g_ucUARTBuff = recvBuf;
					   //Reset parameter
					   recvByteCount = 0;
					   recvLength = 0;
					   set_SendAck(3);
					   memset(recvBuf,0,30);
					   memset(data,'\0',DATA_SIZE);
					   break;
				   }
			}
			else
			{
				//Assign buffer
				g_ucUARTBuff = recvBuf;
				//Reset received buffer
				memset(recvBuf,0,30);
				memset(data,'\0',DATA_SIZE);    
				//Assign state
				recvState = CNTRL_IDLE;
				recvByteCount = 0;
				recvLength = 0;
				set_SendAck(2);
				break;
			}
	}
}

/**********************************************************************
* Function:  int checkSum1(void)
* Parameters: None
* Return: Checksum 1
* Description: Calculate checksum with received data
***********************************************************************/
int checkSum1(void)
{
    uint8_t checksum1 = 0;
    uint8_t i;
    for(i=0; i < recvLength-2; i++)
    {
        checksum1 = checksum1 + data[i];
    }
    checksum1 = checksum1 + HEADER + recvLength;

    if(checksum1==data[recvLength-2])
    {
        return 1;

    }
    else
    {
    	//Do nothing
    }
    return 0;
}

/**********************************************************************
* Function:  int checkSum2(void)
* Parameters: None
* Return: Checksum 1
* Description: Calculate checksum with received data including checksum2
***********************************************************************/
int checkSum2(void)
{
    uint8_t checksum2 = 0;
    uint8_t i;
    
    for(i=0;i<recvLength-1;i++)
    {
        checksum2 = checksum2 + data[i];
    }
    checksum2 = checksum2 + HEADER + recvLength;

    if(checksum2==data[recvLength-1])
        return 1;
    else
        return 0;
}

/**********************************************************************
* Function:  uint8_t get_UserOrSettingControl(void)
* Parameters: None
* Return: User or setting control command
* Description: Function return received user or setting control command
***********************************************************************/
uint8_t get_UserOrSettingControl(void)
{
	return userOrSettingCntrl;
}

/**********************************************************************
* Function:  uint8_t get_UserControl(void)
* Parameters: None
* Return: User control command
* Description: Function return received user control command
***********************************************************************/
uint8_t get_UserControl(void)
{
	return userControl;
}

/**********************************************************************
* Function:  uint8_t get_SettingControl(void)
* Parameters: None
* Return: Setting control command
* Description: Function return received setting control command
***********************************************************************/
uint8_t get_SettingControl(void)
{
	return settingControl;
}

/**********************************************************************
* Function:  uint8_t get_SelectCode(void)
* Parameters: None
* Return: Select code
* Description: Function return received selected code. Applicable for
* 			  setting control command
***********************************************************************/
uint8_t get_SelectCode(void)
{
	return selectCode;
}

/**********************************************************************
* Function:  uint8_t get_SelectIndex(void)
* Parameters: None
* Return: Select Index
* Description: Function return received select index. Applicable for
* 			  setting control command
***********************************************************************/
uint8_t get_SelectIndex(void)
{
	return selectIndex;
}

/**********************************************************************
* Function:  uint8_t get_SelectedProgram(void)
* Parameters: None
* Return: Select Program
* Description: Function return received select Program.
***********************************************************************/
uint8_t get_SelectedProgram(void)
{
	return programSel;
}

/**********************************************************************
* Function:  void set_SelectedProgram(uint8_t selectedProgram)
* Parameters: uint8_t selectedProgram
* Return: None
* Description: Set selected program.
***********************************************************************/
void set_SelectedProgram(uint8_t selectedProgram)
{
	programSel = selectedProgram;
}


/**********************************************************************
* Function:  uint8_t get_RecvSettingQuery(void)
* Parameters: None
* Return: feedback for setting query received
* Description: This is function use status feedback for receiving
* 				setting query.
***********************************************************************/
uint8_t get_RecvSettingQuery(void)
{
	return settingQuery;
}

/**********************************************************************
* Function:  uint8_t get_RecvProgSettingQuery(void)
* Parameters: None
* Return: Feedback for program setting query received
* Description: This is function use status feedback for receiving
* 				program setting query.
***********************************************************************/
uint8_t get_RecvProgSettingQuery(void)
{
	return progSettingQuery;
}

/**********************************************************************
* Function:  uint8_t get_RecvUserSettingQuery(void)
* Parameters: None
* Return: Feedback for user setting query received
* Description: This is function use status feedback for receiving
* 				user setting query.
***********************************************************************/
uint8_t get_RecvUserSettingQuery(void)
{
	return userSettingQuery;
}

/**********************************************************************
* Function:  void set_RecvSettingQuery(uint8_t recvSettingQuery)
* Parameters: uint8_t recvSettingQuery - Set/reset for setting query
* Return: None
* Description: This is function set or reset status feedback if received
* 				valid setting query.
***********************************************************************/
void set_RecvSettingQuery(uint8_t recvSettingQuery)
{
	settingQuery = recvSettingQuery;
}

/**********************************************************************
* Function:  void set_RecvProgSettingQuery(uint8_t recvProgrmaSettingQuery)
* Parameters: uint8_t recvProgrmaSettingQuery - Set/reset for
* 				program setting query
* Return: None
* Description: This is function set or reset status feedback if received
* 				valid program setting query.
***********************************************************************/
void set_RecvProgSettingQuery(uint8_t recvProgrmaSettingQuery)
{
	progSettingQuery = recvProgrmaSettingQuery;
}

/**********************************************************************
* Function:  void set_RecvUserSettingQuery(uint8_t recvUserSettingQuery)
* Parameters: uint8_t recvUserSettingQuery - Set/reset for user setting
* 				query
* Return: None
* Description: This is function set or reset status feedback if received
* 				valid user setting query.
***********************************************************************/
void set_RecvUserSettingQuery(uint8_t recvUserSettingQuery)
{
	userSettingQuery = recvUserSettingQuery;
}



/**********************************************************************
* Function:  void set_SendAck(uint8_t setSendAck)
* Parameters: uint8_t setSendAck - Set status as IOT command received
* 				Acknowledgment
* Return: None
* Description: This function set value as valid command received
***********************************************************************/
void set_SendAck(uint8_t setSendAck)
{
	sendAck = setSendAck;
}

/**********************************************************************
* Function:  uint8_t get_SendAck(void)
* Parameters: None
* Return: ack values as command received
* Description: This function get value as valid command received
***********************************************************************/
uint8_t get_SendAck(void)
{
	return sendAck;
}


/**********************************************************************
* Function: uint8_t get_WiFiMode(void)
* Parameters: None
* Return: wifi Mode
* Description: This function return wifi mode
***********************************************************************/
uint8_t get_WiFiMode(void)
{
	return WiFiMode;
}


/**********************************************************************
* Function: boolean get_RebootFlag(void)
* Parameters: None
* Return: Reboot flag
* Description: return reboot flag if valid reboot command received
***********************************************************************/
boolean get_RebootFlag(void)
{
	return bRebootFlag;
}


/**********************************************************************
* Function: uint8_t sendResponse(void)
* Parameters: None
* Return: 0
* Description: Send response as command received
***********************************************************************/
uint8_t sendResponse(void)
{
	uint16_t index;
	
	
	if(userOrSettingCntrl == USER_CONTROL)
	{
		/*
		if(userControl == CMD_READ_DATA1)//read cycle1
		{
			//iot_SendReadData1();
		}
		else if(userControl == CMD_READ_DATA2)//read cycle2
		{
			//iot_SendReadData2();
		}
		*/
#if ENABLE_IOT
		if(HEADER==0x63 /*&& recvLength==0x0A*/ && data[3]==0x0D)//added since the machine was not going in remote mode --inconsistent.
		{
			clr_IOTMultiFuncSetting(AP_MODE_SETTING);//switching of wifi module to direct mode again and again while adding the wifi to remote mode
			BkMultiFuncSetting &= ~AP_MODE_SETTING;
		}
#endif		
		
		if(userControl == CMD_READ_VERSION)
		{
			iot_SendVersion();
		}
	        else if(userControl == CMD_REBOOT)
		{
			iot_RebootSetting();
			bRebootFlag = TRUE;
		}
		else/* if(userControl == CMD_READ_STATUS)*/
		{
			iot_SendCmd();
		}
	}
	else if(userOrSettingCntrl == SETTING_CONTROL)
	{
		if(settingControl == READ_DATA)	//read data <= AST Control
		{
			ast_ReadData();
		}
		/*
		else if((settingControl >= PROGRAM_SETTING) && (settingControl <= TIME_SAVER))
		{
			//ast_OptionSetting();
			iot_SendCmd();
		}
		*/
		else
		{
			iot_SendCmd();
		}
	}
	else
	{
		iot_SendCmd();
	}
	return 0;
}

#endif

