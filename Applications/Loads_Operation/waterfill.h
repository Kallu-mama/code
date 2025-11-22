#ifndef WATERFILL_H_
#define WATERFILL_H_

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "AI_Comm_Global.h"

boolean AIWaterFreq_DataCollection();
boolean WashFilling(uint8_t,uint8_t ,uint8_t,uint16_t,uint16_t );
void Wash_WithinOneMinute(uint8_t,uint16_t);
void LevelReached(uint8_t);
boolean SteamFill(uint8_t ,uint16_t );
void SteamFilling();

#endif
