#ifndef SERIALCOMM_H_
#define SERIALCOMM_H_

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_userdefine.h"

#define HEADER 0x80 // bcoz for 9kg "90" is used
#define LENGTH 30//20
#define DEST_NODE 	0x03
#define MESSAGE_TYPE 	0x00

void SendCommand(void);
static uint16_t Calculate_Checksum(uint8_t *Msg_ui8,uint8_t MsgLength_ui8);
void motor(uint8_t );
#endif
