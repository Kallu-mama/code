#ifndef TUMBLEOPEARTION_H_
#define TUMBLEOPEARTION_H_

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"

boolean Tumbling(uint8_t,uint8_t,uint16_t,uint16_t,uint16_t,uint8_t);
boolean TumblingSpecial(uint8_t,uint8_t,uint16_t,uint16_t,uint16_t,uint8_t);
boolean TumblingSpecialHeat(uint8_t,uint8_t,uint16_t,uint16_t,uint8_t,uint8_t);
boolean anticrease(uint8_t,uint8_t);
boolean cooldown(uint8_t,uint8_t,uint16_t,uint16_t,uint16_t);
boolean SteamHeat(uint8_t ,uint8_t,uint16_t ,uint16_t ,uint8_t ,uint8_t);
void Cooling(uint8_t);

#endif
