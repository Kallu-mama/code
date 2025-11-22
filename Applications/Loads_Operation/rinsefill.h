#ifndef WATERFILL_H_
#define WATERFILL_H_

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"


void Rinse_WithinOneMinute(uint8_t,uint8_t,uint16_t);
boolean RinseFilling(uint8_t,uint8_t ,uint8_t,uint16_t,uint16_t);

void RinseLevelReached(uint8_t);
void RinseDesireLevelReached(uint8_t,uint8_t);


#endif
