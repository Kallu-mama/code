#ifndef ERRORMANAGE_H_
#define ERRORMANAGE_H_

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"

#define SOAK_MAX_COUNT 0x03
#define APP_PREWASH_ON_OFF  0x07
void RapidWashSelection();
void TemperatureSelection();
void Check_LaundryAdd();
void OptionSelection();
void SpinSelection();
void SelectOption();
uint16_t AddDelay(uint16_t);
void Display_TEMP(uint8_t );
void Display_RPM(uint8_t );
void Display_KG(uint8_t );
void display_availoption(uint8_t );
void turnon_display_led(char ,char ,char ,char ,char ,char ,char);
void check_doorlockstatus();
void CheckDrain_PVMVSymbol();
void MachineState_LED(char ,char ,char ,char );
void TurnOnSelected_Option();
void KEYS_DISPLAY();
void LoadSensing_display();
#endif
