#ifndef WATERDRAIN_H_
#define WATERDRAIN_H_

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"

boolean heating(uint8_t,uint8_t,uint8_t,uint16_t,uint16_t,uint16_t);
boolean Heating_NoExtraTime(uint8_t,uint8_t,uint8_t,uint16_t,uint16_t,uint16_t);

void NormalHeat(uint8_t);
void TumbleHeat(uint8_t );
void ExtraHeat(uint8_t);

#endif
