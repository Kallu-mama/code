#ifndef __AI_COMM_H__
#define __AI_COMM_H__


#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "string.h"
#include "r_cg_serial.h"
#include "Application_Timer_GLobal.h"


/*-------------------------------Macros Declaration----------------------------------------*/
//#define AI_PRS_HEADER_LENGTH		PRS_DATA_LENGTH+3 /*Header + reserved + resp_req + reserved*/

/*-----------------------------------------------------------------------------------------*/


enum AI_CLOTH_TYPES
{
	AI_CLOTH_EMPTY,
	AI_CLOTH_COTTON_100,
	AI_CLOTH_SYNTH_100,
	AI_CLOTH_COTTON_50_SYNTH_50,
	AI_CLOTH_COTTON_25_SYNTH_75,
	AI_CLOTH_COTTON_75_SYNTH_25,
	AI_CLOTH_TYPE_TOTAL_NO
};
enum AI_APPLICABLE_OPTIONS
{
	AI_TEMP_LOC,
	AI_RPM_LOC,
	NO_OF_OPTIONS
};
enum AI_States{
	AI_IDLE,		//AI_Idle
	AI_DATA_COMPUTATION,	//AI_Data_Colleaction
	AI_TRANSMIT,		//AI_Transmition
	AI_RECIEVE,		//AI_Reception
	AI_EXIT			//AI_Exit_Entry
};
enum RREQ_RESP{
	REQ = 0x01,
	RESP 	
};
enum AI_HEADER{
	PRS_HEADER	= 0x96,	//Pressure sensor header
	NO_OF_HEADER	= 1
};

enum AI_DATA_LENGTH
{
	PRS_DATA_LENGTH = 255
};
typedef struct
{
	enum AI_HEADER AI_Header;
	uint8_t AI_DataLength_ui8; //unused data byte.
	enum RREQ_RESP Req_Resp;
	uint8_t Selected_Program_ui8;
	uint8_t Machine_Type_ui8;
}AI_COMM_PACKET;

typedef struct 
{
	uint8_t AI_En_b	:1;
	uint8_t default_option_ui8[NO_OF_OPTIONS];
	uint8_t min_option_ui8[NO_OF_OPTIONS];
	uint8_t max_option_ui8[NO_OF_OPTIONS];
	
}AI_OPTIONS_APPLICANT;

/*-------------------------------Boolean variable Declaration------------------------------*/
/*-----------------------------------------------------------------------------------------*/

/*-------------------------------Global Variable Declaration-------------------------------*/

/*-----------------------------------------------------------------------------------------*/


/*---------------------Smartplus Programs Not Consider---------------------*/
AI_OPTIONS_APPLICANT AI_Option_Object[NO_OF_PROGRAMS] = {
				{	/*POSITION0*/	
					0,
					{0,0},
					{0,0},
					{0,0}
				},
				{	/*MIXEDSOIL = 1*/
					0,
					{0,4},
					{0,0},
					{4,4}
				},
				{	/*COTTON = 2*/
					1,
					{Wash40,Rpm1000},
					{WashCold,Rpm000},
					{P_40,Rpm1000}
				},
				{	/*SYNTHETIC = 3*/   
					1,
					{Wash40,Rpm1000},
					{WashCold,Rpm000},
					{P_40,Rpm1000}//{Wash40,Rpm1000}
				},
				{	/*BABYWEAR = 4*/
					0,
					{0,4},
					{0,0},
					{4,4}
				},
				{	/*STEAMFRESH = 5*/
					0,
					{0,4},
					{0,0},
					{4,4}
				},
				
				{	/*ADDITIVESRINSE = 6*/
					0,
					{0,4},
					{0,0},
					{4,4}
				},
				{	/*CRADLEWASH = 7*/
					0,
					{0,4},
					{0,0},
					{4,4}
				},
				{	/*CURTAINS = 8*/ 
					0,
					{0,4},
					{0,0},
					{4,4}
				},
				{	/*EXPRESS = 9*/
					0,
					{0,4},
					{0,0},
					{4,4}
				},

				{	/*SMARTPLUS = 10*/
					0,
					{0,4},
					{0,0},
					{4,4}
				},
};


#endif
