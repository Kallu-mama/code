#ifndef BUZZER_H_
#define BUZZER_H_

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_userdefine.h"

void ToneGeneration();
char  ToneFrequencyGenerator(Buzzer *);

#endif