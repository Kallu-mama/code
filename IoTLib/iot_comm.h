#ifndef __IOT_COMM_H__
#define __IOT_COMM_H__
 
#define DATA_SIZE 			25


/* Frame Configuration */
#define HEADER 				0x63
#define CMD_LENGTH			0x14
#define LENGTH 				0x24
#define LENGTH1	 			0x1A
#define LENGTH2 			0x0A
#define LENGTH3 			0x2D
#define FAV_ACK_LENGTH			0x1D

#define AIDATA_LENGTH			600

enum AI_HEADER_LENGTH
{
	AI_PRS_HEADER_LENGTH = 5
};
typedef enum
{
	CNTRL_IDLE = 0,
	CNTRL_HEADER,
	CNTRL_LENGTH,
	CNTRL_DATA,
	CNTRL_CRC
}CNTRL_BYTE;	

void uartReceive(uint8_t dataReceive);
int checkSum1(void);
int checkSum2(void);
uint8_t get_UserOrSettingControl(void);
uint8_t get_UserControl(void);
uint8_t get_SettingControl(void);
uint8_t get_SelectCode(void);
uint8_t get_SelectIndex(void);
uint8_t get_SendAck(void);
uint8_t sendResponse(void);
uint8_t get_WiFiMode(void);
boolean get_RebootFlag(void);
void set_SendAck(uint8_t ui_SendAck);

/*********************** Function for UART received ***************************/
uint8_t get_SelectedProgram(void);
void set_SelectedProgram(uint8_t selectedProgram);
uint8_t get_RecvSettingQuery(void);
uint8_t get_RecvProgSettingQuery(void);
uint8_t get_RecvUserSettingQuery(void);
void set_RecvSettingQuery(uint8_t recvSettingQuery);
void set_RecvProgSettingQuery(uint8_t recvProgrmaSettingQuery);
void set_RecvUserSettingQuery(uint8_t recvUserSettingQuery);
/******************************************************************************/



#endif
