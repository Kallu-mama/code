#ifndef PROGRAMSELECTION_H_
#define PROGRAMSELECTION_H_

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_userdefine.h"
#include "AI_Comm_Global.h"


void Programs();
boolean DelayStart();
void RapidTime(uint8_t,uint8_t );
void SpecialEnduranceProgram();
void Program_Finished();
boolean RinseHold();

#endif
