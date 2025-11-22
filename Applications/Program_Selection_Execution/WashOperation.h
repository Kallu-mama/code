#ifndef _WASH_OPERATION_H
#define _WASH_OPERATION_H

#ifndef PROGRAMSELECTION_H_
#include "r_cg_macrodriver.h"
#include "Application_Timer_Global.h"
#include "AI_Comm_Global.h"
#include "r_cg_userdefine.h"


void Programs();
void washing(Operation *);

						
//4 category load sensing

/*----------------------------------------------MIXED Program ---------------------------------------------------------------------*/
//const unsigned char MIXED_TimeSaverTumbleTXTime_ui8[4][9] = {	/*	NONE	Cold	30	40	60 	P_COLD	P_30	P_40	P_60 */
//									{0,	3,	2,	0,	2,	3,	2,	0,	2}, /*0*/
//									{0,	3,      3,      0,      4,	3,      3,      0,      4}, /*1kg*/
//									{0,	3,      3,      0,      6,	3,      3,      0,      6}, /*2kg*/
//									{0,	3,      3,      0,      6,	3,      3,      0,      6}/*3kg*///only applicable when prog has switched from cotton with load category 3
//							};
const unsigned char MIXED_TimeSaverTumbleTXTime_ui8[4][5] = {	/*	NONE	Cold	30	40	60 */
									{0,	3,	2,	0,	2}, /*0*/
									{0,	3,      3,      0,      4}, /*1kg*/
									{0,	3,      3,      0,      6}, /*2kg*/
									{0,	3,      3,      0,      6}/*3kg*///only applicable when prog has switched from cotton with load category 3
							};
								
//const unsigned char MIXED_TumbleTXResetTime_ui8[4][9] = {	/*	NONE	Cold	30	40	60 	Cold	30	40	60 */
//									{0,	0,	1,	1,	0,	0,	1,	1,	0}, /*0*/
//									{0,	3,      1,      1,      6,	3,      1,      1,      6}, /*1*/
//									{0,	5,      11,     1,      11,	5,      11,     1,      11}, /*2*/
//									{0,	5,      11,     1,      11,	5,      11,     1,      11}/*3*/ //only applicable when prog has switched from cotton with load category 3
//								};
const unsigned char MIXED_TumbleTXResetTime_ui8[4][9] = {	/*	NONE	Cold	30	40	60  */
									{0,	0,	1,	1,	0}, /*0*/
									{0,	3,      1,      1,      6}, /*1*/
									{0,	5,      11,     1,      11}, /*2*/
									{0,	5,      11,     1,      11}/*3*/ //only applicable when prog has switched from cotton with load category 3
								};

/*----------------------------------------------COTTON 95,60,40,30,cold Program ---------------------------------------------------------------------*/
								
const unsigned char COTTON_TimeSaverTumbleTXTime_ui8[4][6]	= {	/*	NONE	Cold	30	40	60	95 */
										{0,	7,	10,	2,	0,	0}, /*0*/
										{0,	7,      10,     3,      5,	5}, /*1*/
										{0,	10,     10,     7,      7,	7}, /*2*/
										{0,	15,     10,     7,      7,	7}, /*3*/	
									};
//const unsigned char COTTON_TimeSaverTumbleTXTime_ui8[4][11]	= {	/*	NONE	Cold	30	40	60	95 	P_COLD	P_30	P_40	P_60	P_95 */
//										{0,	7,	10,	2,	0,	0,	7,	10,	2,	0,	0}, /*0*/
//										{0,	7,      10,     3,      5,	5,	7,      10,     3,      5,	5}, /*1*/
//										{0,	10,     10,     7,      7,	7,	10,     10,     7,      7,	7}, /*2*/
//										{0,	15,     10,     7,      7,	7,	15,     10,     7,      7,	7}, /*3*/	
//									};
								
const unsigned char COTTON_TumbleTXResetTime_ui8[4][6]	 =  {	/*	NONE	Cold	30	40	60	95 */
									{0,	11,	13,	3,	13,	8}, /*0*/
									{0,	11,     16,     4,      13,	8}, /*1*/
									{0,	11,    	19,     16,     15,	10}, /*2*/
									{0,	21,     29,     24,     26,	21}, /*3*/
								};
//const unsigned char COTTON_TumbleTXResetTime_ui8[4][11]	 =  {	/*	NONE	Cold	30	40	60	95 	P_COLD	P_30	P_40	P_60	P_95 */
//									{0,	11,	13,	3,	13,	8,	11,	13,	3,	13,	8}, /*0*/
//									{0,	11,     16,     4,      13,	8,	11,     16,     4,      13,	8}, /*1*/
//									{0,	11,    	19,     16,     15,	10,	11,    	19,     16,     15,	10}, /*2*/
//									{0,	21,     29,     24,     26,	21,	21,     29,     24,     26,	21}, /*3*/
//								};
/*----------------------------------------------SYNTHETIC 60,40,30,cold Program ---------------------------------------------------------------------*/
								
const unsigned char SYNTHETIC_TimeSaverTumbleTXTime_ui8[4][5] = {	/*	NONE	Cold	30	40	60 */
										{0,	2,	18,	20,	15}, /*NONE*/
										{0,	3,      22,     25,     20}, /*1kg*/
										{0,	5,     	27,     30,     28}, /*2kg*/
										{0,	5,     	27,     30,     28}, /*3kg*/
									};
//const unsigned char SYNTHETIC_TimeSaverTumbleTXTime_ui8[3][9] = {	/*	NONE	Cold	30	40	60 	P_COLD	P_30	P_40	P_60 */
//										{0,	2,	18,	20,	15,	2,	18,	20,	15},/*NONE*/
//										{0,	3,      22,     25,     20,	3,      22,     25,     20}, /*1kg*/
//										{0,	5,      27,     30,     28,	5,      27,     30,     28}, /*2kg*/
//									};

const unsigned char SYNTHETIC_TumbleTXResetTime_ui8[4][5] = {	/*	NONE	Cold	30	40	60 */
									{0,	 6,	24,	29,	33}, /*0*/	
									{0,	 6,     41,     39,     37}, /*1*/
									{0,	 6,     45,     41,     37}, /*2*/
									{0,	 6,     45,     41,     37}, /*3*/
								};
//const unsigned char SYNTHETIC_TumbleTXResetTime_ui8[3][9] = {	/*	NONE	Cold	30	40	60 	P_COLD	P_30	P_40	P_60 */
//									{0,	 6,	24,	29,	33,	 6,	24,	29,	33}, /*0*/	
//									{0,	 6,     41,     39,     37,	 6,     41,     39,     37}, /*1*/
//									{0,	 6,     45,     41,     37, 	 6,     45,     41,     37}, /*2*/
//								};

uint8_t WashOperation_TimeSaverTime(uint8_t uiDuration);
uint8_t WashOperations_HeatTime(uint8_t step_time);
boolean WashOperation_Last30secDrain(void);
#endif
