#ifndef FOAMSENSING_H_
#define FOAMSENSING_H_

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"

void MotorFailCondition();
void PressureSensorError();
void WaterOverFlowCondition();
void WaterOverFlow();
void FoamCheck();
void MotorFail();
void Read_Errors(uint8_t );
void Store_errors(uint8_t );
void VoltageControl();

#endif