#ifndef WATERHEAT_H_
#define WATERHEAT_H_

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_userdefine.h"
#include "Application_Timer_Global.h"

boolean draining(uint8_t,uint8_t,uint8_t,uint16_t);
boolean Conditional_drain(uint16_t uiSet_DrainLevel);
void NormalDrain(uint8_t,uint8_t,uint8_t);
void ExtraDrain(uint8_t,uint8_t,uint8_t);
void Foam_Removal_prewash_gui8(void);
void Foam_reset(void);
#endif
