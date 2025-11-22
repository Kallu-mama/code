#ifndef BACKUP_H_
#define BACKUP_H_

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "pfdl_types.h"
#include "pfdl.h"
#include "AI_Comm_GLobal.h"

void FDL_status_chk(void);	//Function that used to check the status of FDL Handler
void FDL_Blank_Chk(void);	//Function that used to execute FDL blank check command
void FDL_Erase(void);		//Function that used to execute FDL erase command
void FDL_Write(void);		//Function that used to execute FDL write command
void FDL_Verify(void);		//Function that used to execute FDL verify command
void FDL_Read(void);

void FDL_status_chk_1(void);	//Function that used to check the status of FDL Handler
void FDL_Blank_Chk_1(void);	//Function that used to execute FDL blank check command
void FDL_Erase_1(void);		//Function that used to execute FDL erase command
void FDL_Write_1(void);		//Function that used to execute FDL write command
void FDL_Verify_1(void);		//Function that used to execute FDL verify command
void FDL_Read_1(void);

void MemoryMirror(void);
void LowVoltage(void);
void Position_Idle(void);
void PowerOnRead();
void MemoryWrite(void);
void Memory_Mirror_Write();
void MemoryRead(void);
void MemoryValuesRead();
void ResetMemoryValues();
void Display_Backup();
void Option_Backup();
#endif
