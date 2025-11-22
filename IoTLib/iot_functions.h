
#ifndef __IOT_FUNC_H
#define __IOT_FUNC_H
typedef enum
{
	OPTION_ENABLE_1	= 0,
	OPTION_ENABLE_2,
	OPTION_ENABLE_3,
	OPTION_ENABLE_MAX,
}OPTION_ENABLE;

typedef enum
{
	ALARM_1 = 0,
	ALARM_2,
	ALARM_3,
	ALARM_MAX,
}ALARM;

typedef enum
{
	LED_SEG_1 = 0,
	LED_SEG_2,
	LED_SEG_3,
	LED_SEG_4,
	LED_SEG_5,
	LED_SEG_MAX
}LED_SEGMENT;

typedef enum
{
	PROGRAM_LED_SEG_1 = 0,
	PROGRAM_LED_SEG_2,
	PROGRAM_LED_SEG_MAX
}PROGRAM_LED_SEGMENT;

typedef enum
{
	OPTION_LED_SEG_1 = 0,
	OPTION_LED_SEG_2,
	OPTION_LED_SEG_MAX
}OPTION_LED_SEGMENT;

typedef enum
{
	STATUS_LED_SEG_1 = 0,
	STATUS_LED_SEG_2,
	STATUS_LED_SEG_MAX
}STATUS_LED_SEGMENT;


typedef enum
{
	KNOB_LED_SEG_1 = 0,
	KNOB_LED_SEG_2,
	KNOB_LED_SEG_MAX
}KNOB_LED_SEGMENT;

typedef enum
{
	OUTPUT_1 = 0,
	OUTPUT_2,
	OUTPUT_3,
	OUTPUT_MAX,
}OUTPUT;


typedef struct
{
	uint16_t (*uartReceive)(uint8_t * const, uint16_t);
	uint16_t (*uartTransmit)(uint8_t * const, uint16_t);
}UART_COMM;

//extern UART_COMM *uartComm;

/* IoT functions declaration */
uint8_t get_IOTState(void);
void set_IOTState(uint8_t stateIOT);

void set_IOTError(uint8_t ui_errorIoT);
//uint8_t get_IOTError(void);

int8_t set_IOTAlarm(uint8_t alarmIndex, uint8_t alarmStatus);
int8_t clr_IOTAlarm(uint8_t alarmIndex, uint8_t alarmStatus);
//int8_t get_IOTAlarm(uint8_t alarmIndex);

void set_IOTrinse(unsigned char uc_Rinse);

int8_t set_IOTOptionEnable(uint8_t optionIndex, uint8_t optionStatus);
int8_t clr_IOTOptionEnable(uint8_t optionIndex, uint8_t optionStatus);
int8_t get_IOTOptionEnable(uint8_t optionIndex);

void reset_IOTTemperature(void);
//void set_IOTTemperature(uint8_t programSelection, uint8_t temperatueSelection);
void set_IOTTemperature(uint8_t temperatueSelection);
void reset_IOTparam();
void reset_IOTProgramOption();
/*Appliance type declaration  */
void set_IOTApplianceType(uint8_t applianceType);
uint8_t get_IOTApplianceType(void);

/*Function Deceleration for IOT lib*/
void set_IOTProgramSelection(uint8_t programSelection);
uint8_t get_IOTProgramSelection(void);

void set_IOT_IPMtemp(uint8_t IpmTemp); //for IPM temp log

void set_IOTMultiFuncSetting(uint8_t multiFuncSetting);
void clr_IOTMultiFuncSetting(uint8_t multiFuncSetting);
uint8_t get_IOTMultiFuncSetting(void);

void set_IOTSpeedSetting(uint8_t speedSetting);
uint8_t get_IOTSpeedSetting(void);

void set_IOTExtraRinse(uint8_t extraRinse);
uint8_t get_IOTExtraRinse(void);

void set_IOTStartUpDelayInMin(uint16_t delayInMin);
uint16_t get_IOTStartUpDelayInMin(void);

void set_IOTSoakInMin(uint8_t soakInMin);
uint8_t get_IOTSoakInMin(void);

void set_IOTChildLock(uint8_t childLock);

void set_IOTWashProgram(uint8_t washProgram);
uint8_t get_IOTWashProgram(void);

void set_IOTWaterTemperature(uint8_t waterTemperature);
uint8_t get_IOTWaterTemperature(void);

void set_IOTWaterLiter(uint8_t waterLiter);
uint8_t get_IOTWaterLiter(void);

void set_IOTUnbalanceValue(uint8_t unbalanceValue);
uint8_t get_IOTUnbalanceValue(void);

void set_IOTLoadFlag(uint8_t loadFlag);
uint8_t get_IOTLoadFlag(void);

void set_IOTDoorStatus(uint8_t doorStatus);
uint8_t get_IOTDoorStatus(void);

void set_IOTProgramTime(uint16_t programTime);
uint16_t get_IOTProgramTime(void);

void set_IOTBalanceTime(uint16_t balanceTime);
uint16_t get_IOTBalanceTime(void);

void set_IOTMotorSpeed(uint16_t motorSpeed);
uint16_t get_IOTMotorSpeed(void);

void set_IOTWaterLevel(uint16_t waterLevel);
uint16_t get_IOTWaterLevel(void);

//int8_t set_IOTOutput(uint8_t outputIndex, uint8_t outputFlagStatus);
//int8_t clr_IOTOutput(uint8_t outputIndex, uint8_t outputFlagStatus);
//int8_t get_IOTOutput(uint8_t outputIndex);

/*HIL function*/
void set_HILSupplyVoltageNCurrent(uint8_t supplyVoltage,uint8_t supplyCurrent);
uint8_t get_HILSupplyVoltage(void);
uint8_t get_HILSupplyCurrent(void);
void set_ASTDutyCycleTime(uint8_t dutyCycleInMin,uint8_t dutyCycleInSec);
uint8_t get_ASTDutyCycleInMin(void);
uint8_t get_ASTDutyCycleInSec(void);
void set_ASTStepTime(uint8_t stepTime);
uint8_t get_ASTStepTime(void);
void set_ASTStepMinutesCompleted(uint8_t stepMinutesCompleted);
uint8_t get_ASTStepMinutesCompleted(void);
void set_ASTStepSeconds(uint8_t stepSeconds);
uint8_t get_ASTStepSeconds(void);
void set_ASTTestStep(uint8_t testStep);
uint8_t get_ASTTestStep(void);
void set_ASTPresentStep(uint8_t presentStep);
uint8_t get_ASTPresentStep(void);
void set_ASTWeightNHumidity(uint8_t weight, uint8_t humidity);
uint8_t get_ASTWeight(void);
uint8_t get_ASTHumidity(void);


void set_IOTTestStep(uint8_t testStep);
uint8_t get_IOTTestStep(void);




void set_SelectedProgram(unsigned char selectedProgram);


/* IoT acknowledgment functions declaration */
void iot_SendCmd(void);
//void iot_SendReadData1(void);
//void iot_SendReadData2(void);
void iot_SendVersion(void);
void iot_RebootSetting(void);
void iot_AppReboot(void);

void ast_ReadData(void);
void ast_OptionSetting(void);
void set_IOTprogram(uint8_t code);
MD_STATUS uartSend(uint8_t * const tx_buf, uint16_t tx_num);

#endif
