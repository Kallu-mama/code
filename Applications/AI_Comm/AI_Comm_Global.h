#ifndef __AI_COMM_GLOBAL_H__
#define __AI_COMM_GLOBAL_H__


#define AI_CLOTH_BYTE_HEADER_LENGTH	7
#define AI_WATER_LEVEL			42000
#define AI_TRANSMIT_BUF_SIZE		700//445

enum AI_EXEC_State	// AI_execution state.
{	AI_Idle,
	AI_Runing,
	AI_Done,
	AI_Fail
};
enum HEADER_Type{
	PRS_HEADER_LOC = 0,
	NO_OF_AI_HEADER
	
};
void setAIComm_State_ui8(uint8_t state);
void setAIWaterFreq_ReadEnable(uint8_t);
void AI_ProgramTimeUpdate(void);
uint8_t getAIComm_ExecState_ui8(void);
uint8_t getAIComm_Program_ui8(void);
void setAIComm_Program_ui8(uint8_t);
void AI_Execution();
void AI_Comm_Entry(uint8_t);
void AI_ProgramTimeUpdate(void);
void AI_Comm_Do();
boolean AI_Comm_Run();
boolean AI_Enable_Status();
#endif
