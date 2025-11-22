#ifndef KEYEVENT_H_
#define KEYEVENT_H_

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#define CHILDLOCK_LONGPRESS()  ((bStartButton == TRUE)? 100 : 200);
#define TUBCLEAN_LONGPRESS()  ((bStartButton == TRUE)? 120 : 200);
#define EXTRARINSE_LONGPRESS()  ((bStartButton == TRUE)? 100 : 200);
#define SPIN_BUTTON_PRESS()  ((bStartButton == TRUE)? 99 : 190);
#define DEBUG_LONGPRESS()  ((bStartButton == TRUE)? 100 : 200);
#define WIFIRESET_LONGPRESS()  ((bStartButton == TRUE)? 100 : 200);

void SwitchPress();
void ProgramSelector();
void ButtonPressed();
void Key_Press_Tone();
#endif
