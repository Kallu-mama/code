#ifndef PROGRAMSTATE_H_
#define PROGRAMSTATE_H_

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "AI_Comm_Global.h"

void CurrentState(void);
void ErrorHandling(void);

void PressureSwitchFailure();
void StopAllEvents();
void Emergency_DoorOpen();
void HeaterFailure();

void MachineStartUp();
void MachineStartState();
void TestModePause();
void ErrorStatButtonChange();
void PauseState();
void ComponentSwitchOff();
void ComponentSwitchOff1();
void SpinProfileReset();
void ProgramEnd();
void DoorError_Check();
boolean DoorCondition_Check();
#endif
