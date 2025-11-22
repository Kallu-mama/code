#ifndef __IOT_CONTROL_H__
#define __IOT_CONTROL_H__


/* Time in seconds */
#define ONE_SECONDS_COUNT		50
#define TWO_SECONDS_COUNT 		100
#define FIVE_SECONDS_COUNT 		250
#define THREE_SECONDS_COUNT 		150

void check_IOTRebootFlag();
void MachineDiagnosticTest();
void IOT_CommHandler(void);
void UserDataSetting(void);
void OptionDataSetting(void);
void Set_Delay(uint16_t Delay);
void Set_Spin(uint16_t Spin);
void DataReading();
void Set_Temperature(uint8_t Temp);

void setIOT_temp(uint8_t);
void setIOT_spinSel(uint8_t);

void updateSelectedProgram(void);
void updateIotParam(void);
void detergent_order();
void diagnostic_mode(void);
void wifi_ap_mode(void);
#endif

