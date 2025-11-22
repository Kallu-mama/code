#ifndef WATERFILL_H_
#define WATERFILL_H_

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"

void Valveselection(uint8_t);
void Valveselection1(uint8_t);
void LowPressure(uint8_t,uint8_t,uint16_t);
void NoWater(uint8_t);
boolean reset_filling(uint8_t,uint16_t );
void RinseTumble(uint8_t);
void CommonOperation(uint8_t,uint8_t,uint16_t );
void ResetLowPressure(uint8_t);
void ResetFill_Valve_Selection(uint8_t );

#endif
