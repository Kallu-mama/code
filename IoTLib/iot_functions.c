#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

#if ENABLE_IOT
#include "r_cg_serial.h"
#include "iot_config.h"

#include "iot_comm.h"
#include "iot_functions.h"
#include "iot_config.h"
#include "iot_user_config.h"
#include "Version.h"
/* Variable declaration for IoT code only */
static char countforsend = 0;
static uint8_t MachineStatus_iot[40];

//extern const char setTemperatureFeedback[][TEMP_MAX_OPTION];

/* Variable declaration for IoT code: making extern for others */
uint8_t data[DATA_SIZE];
uint8_t WiFiSoftware[3];

/******************* IOT Appliance type ******************/
static uint8_t iotApplianceType;
/*********************************************************/

/**************** IOT Feedback Variable ******************/
static uint8_t iotProgramSelection;
static uint8_t iotFuncSetting;
static uint8_t iotSpeedSetting;
static uint8_t iotExtraRinse;
static uint8_t iotTempSetting;
static uint8_t iotOptionEnable[OPTION_ENABLE_MAX];	//Option Enable 2 and 1
static uint16_t iotDelayInMin;
static uint8_t iotSoakInMin;
static uint8_t iotChildLock;
static uint8_t iotWashProgram;
static uint16_t iotBalanceTime;
static uint16_t	iotMotorSpeed;
static uint8_t iotWaterTemperature;
static uint16_t iotWaterLevel;
static uint8_t iotWaterLiter;
static uint8_t iotUnbalanceValue;
static uint8_t iotAlarmIndication[ALARM_MAX];	//Alarm Indication
static uint8_t iotMachineState;
static uint8_t iotLoadFlag;
static uint16_t iotProgramTime;
static uint8_t iotMachinePrevState;
static uint8_t iotDoorStatus;
static uint8_t iotError;
static uint8_t iotOutput[OUTPUT_MAX];
static uint8_t iotTestStep;
static uint8_t iotIpmtemp;
/*********************************************************/

/*************** HIL Segment Indication ******************/
static uint8_t programLED[PROGRAM_LED_SEG_MAX];
static uint8_t optionLED[OPTION_LED_SEG_MAX];
static uint8_t statusLED[STATUS_LED_SEG_MAX];
static uint8_t knobLED[KNOB_LED_SEG_MAX];

static uint8_t ledSeg[LED_SEG_MAX];
static uint8_t resvSeg;
/*********************************************************/

/*************** HIL Segment Indication ******************/
static uint8_t astSupplyVoltage;
static uint8_t astSupplyCurrent;
static uint8_t astDutyCycleInMin;
static uint8_t astDutyCycleInSec;
static uint8_t astStepTime;
static uint8_t astStepMinutesCompleted;
static uint8_t astStepSeconds;
static uint8_t astPresentStep;
static uint8_t astWeight;
static uint8_t astHumidity;
extern uint8_t uidisp_select_option;
extern boolean bCold_Soak_Selected;
extern volatile uint16_t  g_uart2_tx_count;
/*********************************************************/

/*************Define Structure UART communication*********/
UART_COMM *uartComm;
/*********************************************************/

/* External variables for IoT code */
//uint8_t uDetergentKey_bkp;

/*======================== IoT Code Starts From Here ===========================*/
/**********************************************************************
* Function: uint8_t get_IOTState(void)
* Parameters: None
* Return: machine state for IOT communication
* Description: Get machine state assign for IOT communication
***********************************************************************/
uint8_t get_IOTState(void)
{
	return iotMachineState;
}

/**********************************************************************
* Function: void set_IOTState(uint8_t stateIOT)
* Parameters: Assign machine state
* Return: machine state for IOT communication
* Description: Set machine state for IOT communication
***********************************************************************/
void set_IOTState(uint8_t stateIOT)
{
	iotMachineState = stateIOT;
}

/**********************************************************************
* Function: void set_IOTError(uint8_t ui_errorIoT)
* Parameters: Assign error
* Return: None
* Description: Set error for IOT communication
***********************************************************************/
void set_IOTError(uint8_t ui_errorIoT)
{
	iotError = ui_errorIoT;
}

/**********************************************************************
* Function: uint8_t get_IOTError(void)
* Parameters: None
* Return: Return
* Description: Get error set for IOT communication
***********************************************************************/
/*uint8_t get_IOTError(void)
{
	return iotError;
}*/

/**********************************************************************
* Function: int8_t set_IOTAlarm(uint8_t alarmIndex, uint8_t alarmStatus)
* Parameters: uint8_t alarmIndex => Alarm Index range 1 to 3
* 			  uint8_t alarmStatus => Set Alarm
* Return: Return alarmIndex is valid range else -1
* Description: Set alarm with Alarm index 1 to 3
***********************************************************************/
int8_t set_IOTAlarm(uint8_t alarmIndex, uint8_t alarmStatus)
{
	/*Check for max output index*/
	if(alarmIndex < ALARM_MAX)
	{
		iotAlarmIndication[alarmIndex] |= alarmStatus;
	}
	else
	{
		return -1;
	}
	return alarmIndex;
}

/**********************************************************************
* Function: int8_t get_IOTAlarm(uint8_t alarmIndex)
* Parameters: uint8_t alarmIndex => Alarm Index range 1 to 3
* Return: Return value reference with alarm Index
* Description: Return alarm value with Alarm index 1 to 3
***********************************************************************/
/*int8_t get_IOTAlarm(uint8_t alarmIndex)
{
	//Check for max output index//
	if(alarmIndex >= ALARM_MAX)
	{
		return -1;
	}
	return iotAlarmIndication[alarmIndex];
}*/


/**********************************************************************
* Function: int8_t clr_IOTAlarm(uint8_t alarmIndex, uint8_t alarmStatus)
* Parameters: uint8_t alarmIndex => Alarm Index range 1 to 3
* 			  uint8_t alarmStatus => Clear Alarm
* Return: Return alarmIndex is valid range else -1
* Description: Reset alarm with Alarm index 1 to 3
***********************************************************************/
int8_t clr_IOTAlarm(uint8_t alarmIndex, uint8_t alarmStatus)
{
	/*Check for max output index*/
	if(alarmIndex < ALARM_MAX)
	{
		iotAlarmIndication[alarmIndex] &=~ alarmStatus;
	}
	else
	{
		return -1;
	}
	return alarmIndex;
}


/**********************************************************************
* Function: int8_t set_IOTOptionEnable(uint8_t optionIndex, uint8_t optionStatus)
* Parameters: uint8_t optionIndex => Option Index range 1 to 3
* 			  uint8_t optionStatus => Set Option
* Return: Return optionIndex is valid range else -1
* Description: Set option if valid value received from setting control command
* 				reference with optionIndex
***********************************************************************/
int8_t set_IOTOptionEnable(uint8_t optionIndex, uint8_t optionStatus)
{
	/*Check for max output index*/
	if(optionIndex < OPTION_ENABLE_MAX)
	{
		iotOptionEnable[optionIndex] |= optionStatus;
	}
	else
	{
		return -1;
	}
	return optionIndex;
}

/**********************************************************************
* Function: int8_t clr_IOTOptionEnable(uint8_t optionIndex, uint8_t optionStatus)
* Parameters: uint8_t optionIndex => Option Index range 1 to 3
* 			  uint8_t optionStatus => Clear Option
* Return: Return optionIndex is valid range else -1
* Description: Reset option if valid value received from setting control command
* 				reference with optionIndex
***********************************************************************/
int8_t clr_IOTOptionEnable(uint8_t optionIndex, uint8_t optionStatus)
{
	/*Check for max output index*/
	if(optionIndex < OPTION_ENABLE_MAX)
	{
		iotOptionEnable[optionIndex] &=~ optionStatus;
	}
	else
	{
		return -1;
	}
	return optionIndex;
}


/**********************************************************************
* Function: int8_t get_IOTOptionEnable(uint8_t optionIndex)
* Parameters: uint8_t optionIndex => Alarm Index range 1 to 3
* Return: Return value reference with optionIndex
* Description: Return options value with option index 1 to 3
***********************************************************************/
/*int8_t get_IOTOptionEnable(uint8_t optionIndex)
{
	//Check for max output index//
	if(optionIndex >= OPTION_ENABLE_MAX)
	{
		return -1;
	}
	return iotOptionEnable[optionIndex];
}*/



/**********************************************************************
* Function: void set_IOTTemperature(uint8_t programSelection, uint8_t temperatueSelection)
* Parameters: uint8_t programSelection => Index refer to program
*			  uint8_t temperatueSelection => Temperature range selection
* Return: None
* Description: Give temperature feedback value with program and temperature
* 				for temperature slider
***********************************************************************/
/*void set_IOTTemperature(uint8_t programSelection, uint8_t temperatueSelection)
{
	iotTempSetting = setTemperatureFeedback[programSelection][temperatueSelection];
}*/

void set_IOTTemperature(uint8_t temperatueSelection)
{
	iotTempSetting = temperatueSelection;
}
/**********************************************************************
* Function: void reset_IOTTemperature(void)
* Parameters: None
* Return: None
* Description: Reset IOT temperature feedback
***********************************************************************/
void reset_IOTTemperature(void)
{
	iotTempSetting = 0;
}

void reset_IOTProgramOption(void)
{
	iotTempSetting = 0;
	iotExtraRinse = 0;
	iotSoakInMin   = 0;
	iotOptionEnable[OPTION_ENABLE_1] = 0x00;
	iotOptionEnable[OPTION_ENABLE_2] = 0x00;
	iotOptionEnable[OPTION_ENABLE_3] = 0x00;
	/*if(((uidisp_select_option & WarmSoak_Yes) == WarmSoak_Yes)||(bCold_Soak_Selected == TRUE))
	{
		//iotSoakInMin = 1;
		if(bCold_Soak_Selected == TRUE)
			set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_SOAK);
		else
			set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_WARM_SOAK);
	}*/
	
}
/**********************************************************************
* Function: void set_IOTApplianceType(uint8_t applianceType)
* Parameters: applianceType => Pass appliance type
* Return: None
* Description: Set IOT appliance type
***********************************************************************/
void set_IOTApplianceType(uint8_t applianceType)
{
	iotApplianceType = applianceType;
}


/**********************************************************************
* Function: uint8_t get_IOTApplianceType(void)
* Parameters: None
* Return: appliance type
* Description: Get IOT appliance type
***********************************************************************/
uint8_t get_IOTApplianceType(void)
{
	return iotApplianceType;
}


/**********************************************************************
* Function: void set_IOTProgramSelection(uint8_t programSelection)
* Parameters: programSelection => Program selection
* Return: None
* Description: Set program for IOT feedback communication
***********************************************************************/
void set_IOTProgramSelection(uint8_t programSelection)
{
	iotProgramSelection = programSelection;
}


/**********************************************************************
* Function: uint8_t get_IOTProgramSelection(void)
* Parameters: None
* Return: return program selection
* Description: Get program set for IOT feedback communication
***********************************************************************/
/*uint8_t get_IOTProgramSelection(void)
{
	return iotProgramSelection;
}*/


/**********************************************************************
* Function: void set_IOTMultiFuncSetting(uint8_t multiFuncSetting)
* Parameters: Bitwise multiFunction settings
* Return: None
* Description: Set multi function parameter
* 			   Bit wise access function
***********************************************************************/
void set_IOTMultiFuncSetting(uint8_t multiFuncSetting)
{
	iotFuncSetting |= multiFuncSetting;
}

/**********************************************************************
* Function: void clr_IOTMultiFuncSetting(uint8_t multiFuncSetting)
* Parameters: Bitwise multiFunction settings
* Return: None
* Description: Clear multi function parameter
* 			   Bit wise access function
***********************************************************************/
void clr_IOTMultiFuncSetting(uint8_t multiFuncSetting)
{
	iotFuncSetting &= ~multiFuncSetting;
}

/**********************************************************************
* Function: uint8_t get_IOTMultiFuncSetting(void)
* Parameters: None
* Return: Bitwise function parameter
* Description: Get multi function parameter
* 			   Bit wise access function
***********************************************************************/
uint8_t get_IOTMultiFuncSetting(void)
{
	return iotFuncSetting;
}

/**********************************************************************
* Function: void set_IOTSpeedSetting(uint8_t speedSetting)
* Parameters: speedSetting => Spin speed setting for feedback
* Return: None
* Description: Set spin speed setting for IOT communication
***********************************************************************/
void set_IOTSpeedSetting(uint8_t speedSetting)
{
	iotSpeedSetting = speedSetting;
}


/**********************************************************************
* Function: uint8_t get_IOTSpeedSetting(void)
* Parameters: None
* Return: Get spin speed
* Description: Get spin speed set for IOT communication
***********************************************************************/
/*uint8_t get_IOTSpeedSetting(void)
{
	return iotSpeedSetting;
}*/

/**********************************************************************
* Function: void set_IOTExtraRinse(uint8_t extraRinse)
* Parameters: extraRinse => Extra rinse setting for feedback
* Return: None
* Description: Set extra rinse setting for IOT communication
***********************************************************************/
void set_IOTExtraRinse(uint8_t extraRinse)
{
	iotExtraRinse = extraRinse;
}

/**********************************************************************
* Function: uint8_t get_IOTExtraRinse(void)
* Parameters: None
* Return: Get Extra rinse set value
* Description: Get extra rinse value set for IOT communication
***********************************************************************/
/*uint8_t get_IOTExtraRinse(void)
{
	return iotExtraRinse;
}*/

/**********************************************************************
* Function: void set_IOTStartUpDelayInMin(uint16_t delayInMin)
* Parameters: delayInMin => Set delay in minute for machine start up delay
* 							for feedback
* Return: None
* Description: Set start up delay in minute for IOT communication
***********************************************************************/
void set_IOTStartUpDelayInMin(uint16_t delayInMin)
{
	iotDelayInMin = delayInMin;
}


/**********************************************************************
* Function: uint16_t get_IOTStartUpDelayInMin(void)
* Parameters: None
* Return: Get start up delay value
* Description: Get start up delay in minute which set for
* 				IOT communication
***********************************************************************/
/*uint16_t get_IOTStartUpDelayInMin(void)
{
	return iotDelayInMin;
}*/

/**********************************************************************
* Function: void set_IOTSoakInMin(uint8_t soakInMin)
* Parameters: soakInMin => Set soak in minute for feedback
* Return: None
* Description: Set Soak value in minute for IOT communication
***********************************************************************/
void set_IOTSoakInMin(uint8_t soakInMin)
{
	iotSoakInMin = soakInMin;
}

/**********************************************************************
* Function: uint8_t get_IOTSoakInMin(void)
* Parameters: None
* Return: Get soak value in minute
* Description: Get soak value in minute set for IOT communication
***********************************************************************/
/*uint8_t get_IOTSoakInMin(void)
{
	return iotSoakInMin;
}*/

/**********************************************************************
* Function: void set_IOTChildLock(uint8_t childLock)
* Parameters: childLock => Set/Reset child lock
* Return: None
* Description: Set/Reset child lock for IOT communication
***********************************************************************/
void set_IOTChildLock(uint8_t childLock)
{
	iotChildLock = childLock;
}

/**********************************************************************
* Function: void set_IOTWashProgram(uint8_t washProgram)
* Parameters: washProgram => Set wash program
* Return: None
* Description: Set wash program for IOT communication
***********************************************************************/
/*void set_IOTWashProgram(uint8_t washProgram)
{
	iotWashProgram = washProgram;
}*/


/**********************************************************************
* Function: uint8_t get_IOTWashProgram(void)
* Parameters: None
* Return: Get wash program
* Description: Get wash program set for IOT communication
***********************************************************************/
/*uint8_t get_IOTWashProgram(void)
{
	return iotWashProgram;
}*/


/**********************************************************************
* Function: void set_IOTWaterTemperature(uint8_t waterTemperature)
* Parameters: waterTemperature => Set water temperature
* Return: None
* Description: Set water temperature for IOT communication
***********************************************************************/
void set_IOTWaterTemperature(uint8_t waterTemperature)
{
	iotWaterTemperature = waterTemperature;
}


/**********************************************************************
* Function: uint8_t get_IOTWaterTemperature(void)
* Parameters: None
* Return: Get water temperature
* Description: Get water temperature set for IOT communication
***********************************************************************/
/*uint8_t get_IOTWaterTemperature(void)
{
	return iotWaterTemperature;
}*/


/**********************************************************************
* Function: void set_IOTWaterLiter(uint8_t waterLiter)
* Parameters: waterLiter => Set water liter
* Return: None
* Description: Set water liter for IOT communication
***********************************************************************/
/*void set_IOTWaterLiter(uint8_t waterLiter)
{
	iotWaterLiter = waterLiter;
}*/

/**********************************************************************
* Function: uint8_t get_IOTWaterLiter(void)
* Parameters: None
* Return: Get water liter value
* Description: Get water liter value set for IOT communication
***********************************************************************/
/*uint8_t get_IOTWaterLiter(void)
{
	return iotWaterLiter;
}*/

/**********************************************************************
* Function: void set_IOTUnbalanceValue(uint8_t unbalanceValue)
* Parameters: unbalanceValue => Set unbalanced value
* Return: None
* Description: Set unbalanced value for IOT communication
***********************************************************************/
void set_IOTUnbalanceValue(uint8_t unbalanceValue)
{
	iotUnbalanceValue = unbalanceValue;
}

/**********************************************************************
* Function: uint8_t get_IOTUnbalanceValue(void)
* Parameters: None
* Return: Get unbalanced value
* Description: Get unbalanced value set for IOT communication
***********************************************************************/
/*uint8_t get_IOTUnbalanceValue(void)
{
	return iotUnbalanceValue;
}*/

/**********************************************************************
* Function: void set_IOTLoadFlag(uint8_t loadFlag)
* Parameters: loadFlag => Set load flag
* Return: None
* Description: Set load flag value for IOT communication
***********************************************************************/
void set_IOTLoadFlag(uint8_t loadFlag)
{
	iotLoadFlag = loadFlag;
}

/**********************************************************************
* Function: uint8_t get_IOTLoadFlag(void)
* Parameters: None
* Return: Get load flag value
* Description: Get load flag value set for IOT communication
***********************************************************************/
/*uint8_t get_IOTLoadFlag(void)
{
	return iotLoadFlag;
}*/


/**********************************************************************
* Function: void set_IOTDoorStatus(uint8_t doorStatus)
* Parameters: doorStatus => Door status
* Return: None
* Description: Set door status value for IOT communication
***********************************************************************/
void set_IOTDoorStatus(uint8_t doorStatus)
{
	iotDoorStatus = doorStatus;
}


/**********************************************************************
* Function: uint8_t get_IOTDoorStatus(void)
* Parameters: None
* Return: Get door status value
* Description: Get door status value for IOT communication
***********************************************************************/
/*uint8_t get_IOTDoorStatus(void)
{
	return iotDoorStatus;
}*/


/**********************************************************************
* Function: void set_IOTProgramTime(uint16_t programTime)
* Parameters: programTime => Program time in Minute
* Return: None
* Description: Set program time in minute for IOT communication
***********************************************************************/
void set_IOTProgramTime(uint16_t programTime)
{
	iotProgramTime = programTime;
}


/**********************************************************************
* Function: uint16_t get_IOTProgramTime(void)
* Parameters: None
* Return: Get program time in minute
* Description: Get program time in minute for IOT communication
***********************************************************************/
/*uint16_t get_IOTProgramTime(void)
{
	return iotProgramTime;
}*/

/**********************************************************************
* Function: void set_IOTBalanceTime(uint16_t balanceTime)
* Parameters: balanceTime => Balance time in Minute
* Return: None
* Description: Set balance time in minute for IOT communication
***********************************************************************/
void set_IOTBalanceTime(uint16_t balanceTime)
{
	iotBalanceTime = balanceTime;
}

/**********************************************************************
* Function: uint16_t get_IOTBalanceTime(void)
* Parameters: None
* Return: Get balance time in minute
* Description: Get balance time in minute for IOT communication
***********************************************************************/
/*uint16_t get_IOTBalanceTime(void)
{
	return iotBalanceTime;
}*/

/**********************************************************************
* Function: void set_IOTMotorSpeed(uint16_t motorSpeed)
* Parameters: motorSpeed => Motor speed
* Return: None
* Description: Set motor speed for IOT communication
***********************************************************************/
void set_IOTMotorSpeed(uint16_t motorSpeed)
{
	iotMotorSpeed = motorSpeed;
}

/**********************************************************************
* Function: uint16_t get_IOTMotorSpeed(void)
* Parameters: None
* Return: Get motor speed value
* Description: Get motor speed for IOT communication
***********************************************************************/
/*uint16_t get_IOTMotorSpeed(void)
{
	return iotMotorSpeed;
}*/

/**********************************************************************
* Function: void set_IOTWaterLevel(uint16_t waterLevel)
* Parameters: waterLevel => Set water level
* Return: None
* Description: Set water level for IOT communication
***********************************************************************/
void set_IOTWaterLevel(uint16_t waterLevel)
{
	iotWaterLevel = waterLevel;
}

/**********************************************************************
* Function: uint16_t get_IOTWaterLevel(void)
* Parameters: None
* Return: Get water level value
* Description: Get water level for IOT communication
***********************************************************************/
/*uint16_t get_IOTWaterLevel(void)
{
	return iotWaterLevel;
}*/


/**********************************************************************
* Function: int8_t set_IOTOutput(uint8_t outputIndex, uint8_t outputFlagStatus)
* Parameters: outputIndex => Set iot output index 1 to 3
* 			  outputFlagStatus => Set flag with index 1 to 3
* Return: return -1 if value in not in range 1 to 3
* Description: Set output value for diagnostic test
***********************************************************************/
/*int8_t set_IOTOutput(uint8_t outputIndex, uint8_t outputFlagStatus)
{
	//Check for max output index
	if(outputIndex < OUTPUT_MAX)
	{
		iotOutput[outputIndex] |= outputFlagStatus;
	}
	else
	{
		return -1;
	}
	return outputIndex;
}*/


/**********************************************************************
* Function: int8_t clr_IOTOutput(uint8_t outputIndex, uint8_t outputFlagStatus)
* Parameters: outputIndex => Set iot output index 1 to 3
* 			  outputFlagStatus => Set flag with index 1 to 3
* Return: return -1 if value in not in range 1 to 3
* Description: Clear output value for diagnostic test
***********************************************************************/
/*int8_t clr_IOTOutput(uint8_t outputIndex, uint8_t outputFlagStatus)
{
	//Check for max output index
	if(outputIndex < OUTPUT_MAX)
	{
		iotOutput[outputIndex] &=~ outputFlagStatus;
	}
	else
	{
		return -1;
	}
	return outputIndex;
}*/


/**********************************************************************
* Function: int8_t get_IOTOutput(uint8_t outputIndex)
* Parameters: None
* Return: Get output values for diagnostics
* Description: Get output value for IOT communication
***********************************************************************/
/*int8_t get_IOTOutput(uint8_t outputIndex)
{
	//Check for max output index//
	if(outputIndex >= OUTPUT_MAX)
	{
		return -1;
	}
	return iotOutput[outputIndex];
}*/


/**********************************************************************
* Function: void set_IOTTestStep(uint8_t testStep)
* Parameters: testStep => set test step for test mode
* Return: None
* Description: Set IOT step test for IOT communication
***********************************************************************/
void set_IOTTestStep(uint8_t testStep)
{
	iotTestStep = testStep;
}


/**********************************************************************
* Function: uint8_t get_IOTTestStep(void)
* Parameters: None
* Return: Test step
* Description: Get test step for test mode
***********************************************************************/
/*uint8_t get_IOTTestStep(void)
{
	return iotTestStep;
}*/



/*******************************AST Code***************************************/

/**********************************************************************
* Function: void set_ASTSupplyVoltageNCurrent(uint8_t supplyVoltage,uint8_t supplyCurrent)
* Parameters: supplyVoltage => Set AST supply voltage
* 			  supplyCurrent => Set AST supply current
* Return: None
* Description: Set voltage and current value for AST communication
***********************************************************************/
/*void set_ASTSupplyVoltageNCurrent(uint8_t supplyVoltage,uint8_t supplyCurrent)
{
	astSupplyVoltage = supplyVoltage;
	astSupplyCurrent = supplyCurrent;

}*/

/**********************************************************************
* Function: uint8_t get_ASTSupplyVoltage(void)
* Parameters: None
* Return: Get supply voltage value
* Description: Get supply voltage for AST communication
***********************************************************************/
/*uint8_t get_ASTSupplyVoltage(void)
{
	return astSupplyVoltage;
}*/

/**********************************************************************
* Function: uint8_t get_ASTSupplyCurrent(void)
* Parameters: None
* Return: Get supply current value
* Description: Get supply current for AST communication
***********************************************************************/
/*uint8_t get_ASTSupplyCurrent(void)
{
	return astSupplyCurrent;
}*/


/**********************************************************************
* Function: void set_ASTDutyCycleTime(uint8_t dutyCycleInMin,uint8_t dutyCycleInSec)
* Parameters: dutyCycleInMin => Set duty cycle in minute for AST
* 			  dutyCycleInSec => Set duty cycle in second for AST
* Return: None
* Description: Set voltage and current value for AST communication
***********************************************************************/
/*void set_ASTDutyCycleTime(uint8_t dutyCycleInMin,uint8_t dutyCycleInSec)
{
	astDutyCycleInMin = dutyCycleInMin;
	astDutyCycleInSec = dutyCycleInSec;
}*/


/**********************************************************************
* Function: uint8_t get_ASTDutyCycleInMin(void)
* Parameters: None
* Return: Get duty cycle in minute
* Description: Get duty cycle in minute
***********************************************************************/
/*uint8_t get_ASTDutyCycleInMin(void)
{
	return astDutyCycleInMin;
}*/

/**********************************************************************
* Function: uint8_t get_ASTDutyCycleInSec(void)
* Parameters: None
* Return: Get duty cycle in second
* Description: Get duty cycle in second
***********************************************************************/
/*uint8_t get_ASTDutyCycleInSec(void)
{
	return astDutyCycleInSec;
}*/


/**********************************************************************
* Function: void set_ASTStepTime(uint8_t stepTime)
* Parameters: stepTime => assign step time
* Return: None
* Description: Set step time value for AST communication
***********************************************************************/
void set_ASTStepTime(uint8_t stepTime)
{
	astStepTime = stepTime;
}


/**********************************************************************
* Function: uint8_t get_ASTStepTime(void)
* Parameters: None
* Return: Get step time
* Description: Get step time in AST communication
***********************************************************************/
/*uint8_t get_ASTStepTime(void)
{
	return astStepTime;
}*/


/**********************************************************************
* Function: void set_ASTStepMinutesCompleted(uint8_t stepMinutesCompleted)
* Parameters: stepMinutesCompleted => Step time completed in minute
* Return: None
* Description: Set step minute time completed value for AST communication
***********************************************************************/
/*void set_ASTStepMinutesCompleted(uint8_t stepMinutesCompleted)
{
	astStepMinutesCompleted = stepMinutesCompleted;
}*/


/**********************************************************************
* Function: uint8_t get_ASTStepMinutesCompleted(void)
* Parameters: None
* Return: Get step minute completed
* Description: Get step minute completed in AST communication
***********************************************************************/
/*uint8_t get_ASTStepMinutesCompleted(void)
{
	return astStepMinutesCompleted;
}*/


/**********************************************************************
* Function: void set_ASTStepSeconds(uint8_t stepSeconds)
* Parameters: stepSeconds => Step seconds
* Return: None
* Description: Set step second in AST communication
***********************************************************************/
/*void set_ASTStepSeconds(uint8_t stepSeconds)
{
	astStepSeconds = stepSeconds;
}*/

/**********************************************************************
* Function: uint8_t get_ASTStepSeconds(void)
* Parameters: None
* Return: Get step seconds
* Description: Get step seconds used AST communication
***********************************************************************/
/*uint8_t get_ASTStepSeconds(void)
{
	return astStepSeconds;
}*/

/**********************************************************************
* Function: void set_ASTPresentStep(uint8_t presentStep)
* Parameters: presentStep => Set present step
* Return: None
* Description: Set preset step in AST communication
***********************************************************************/
void set_ASTPresentStep(uint8_t presentStep)
{
	astPresentStep = presentStep;
}

/**********************************************************************
* Function: uint8_t get_ASTPresentStep(void)
* Parameters: None
* Return: Get present step
* Description: Get present step used AST communication
***********************************************************************/
/*uint8_t get_ASTPresentStep(void)
{
	return astPresentStep;
}*/


/**********************************************************************
* Function: void set_ASTWeightNHumidity(uint8_t weight, uint8_t humidity)
* Parameters: weight => Set weight value
* 			  humidity => Set humidity value
* Return: None
* Description: Set weight and humidity value for AST communication
***********************************************************************/
/*void set_ASTWeightNHumidity(uint8_t weight, uint8_t humidity)
{
	astWeight = weight;
	astHumidity = humidity;
}*/

/**********************************************************************
* Function: uint8_t get_ASTWeight(void)
* Parameters: None
* Return: Get weight value
* Description: Get weight value for AST communication
***********************************************************************/
/*uint8_t get_ASTWeight(void)
{
	return astWeight;
}*/

/**********************************************************************
* Function: uint8_t get_ASTHumidity(void)
* Parameters: None
* Return: Get humidity value
* Description: Get humidity value for AST communication
***********************************************************************/
/*uint8_t get_ASTHumidity(void)
{
	return astHumidity;
}*/

/**********************************************************************
* Function: void set_IOT_IPMtemp(uint8_t IpmTemp)
* Parameters: IpmTemp
* Return: None
* Description: Set run time IPM temp IOT feedback communication
***********************************************************************/
void set_IOT_IPMtemp(uint8_t IpmTemp)
{
	iotIpmtemp = IpmTemp;
}

/**********************************************************************
* Function: void iot_SendCmd(void)
* Parameters: None
* Return: None
* Description: Send IOT feedback communication for user control command
***********************************************************************/
void iot_SendCmd(void)
{
	int16_t index = 0,ibyte_count=0,i=0;
	uint8_t checksum1 = 0,checksum2 = 0;
	
	MachineStatus_iot[index++]= HEADER;     	//0
	MachineStatus_iot[index++]= LENGTH; 		//1

	if(data[0] == USER_CONTROL)
	{

		MachineStatus_iot[index++]= 0x81; 		//2
	}
	else
	{
		MachineStatus_iot[index++]= 0x83;		//2
	}

	/*Reserved*/
	MachineStatus_iot[index++]= iotIpmtemp;//0x00;    	 	//3
	/* Appliance Type */
	MachineStatus_iot[index++]= iotApplianceType;  	//4

	if(data[0] == USER_CONTROL)
	{
		MachineStatus_iot[index++]= get_UserControl();	//5
	}
	else
	{
		MachineStatus_iot[index++]= 0x00;		//5
	}

	//Program Selection
	MachineStatus_iot[index++] = iotProgramSelection;	//6
	MachineStatus_iot[index++] = iotFuncSetting;	//7
	MachineStatus_iot[index++] = iotSpeedSetting;	//8
	MachineStatus_iot[index++] = iotExtraRinse;		//9
	MachineStatus_iot[index++] = iotTempSetting;	//10
	MachineStatus_iot[index++] = iotOptionEnable[OPTION_ENABLE_2];	//11
	MachineStatus_iot[index++] = (uint8_t)(iotDelayInMin / 60);		//12
	MachineStatus_iot[index++] = (uint8_t)(iotDelayInMin % 60);  	//13
	MachineStatus_iot[index++] = iotSoakInMin;		//14
	MachineStatus_iot[index++] = iotChildLock;		//15
	MachineStatus_iot[index++] = iotWashProgram;	//16
	MachineStatus_iot[index++] = (uint8_t)(iotBalanceTime / 60);	//17
	MachineStatus_iot[index++] = (uint8_t)(iotBalanceTime % 60);	//18
	MachineStatus_iot[index++] = (uint8_t)(iotMotorSpeed >> 8);		//19
	MachineStatus_iot[index++] = (uint8_t)iotMotorSpeed;	//20
	MachineStatus_iot[index++] = iotWaterTemperature;		//21
	MachineStatus_iot[index++] = (uint8_t)(iotWaterLevel >> 8);		//22
	MachineStatus_iot[index++] = (uint8_t)iotWaterLevel;	//23
	MachineStatus_iot[index++] = iotAlarmIndication[ALARM_3];  //iotWaterLiter; 		//24
	MachineStatus_iot[index++] = iotUnbalanceValue;		//25
	MachineStatus_iot[index++] = iotAlarmIndication[ALARM_1];	//26
	MachineStatus_iot[index++] = iotAlarmIndication[ALARM_2];	//27
	MachineStatus_iot[index++] = iotOptionEnable[OPTION_ENABLE_3];	//28
	MachineStatus_iot[index++] = iotOptionEnable[OPTION_ENABLE_1];	//29
	MachineStatus_iot[index++] = iotMachineState;			//30
	MachineStatus_iot[index++] = iotLoadFlag;				//31
	MachineStatus_iot[index++] = (uint8_t)(iotProgramTime / 60);	//32//prog timeHigh
	MachineStatus_iot[index++] = (uint8_t)(iotProgramTime % 60);	//33 prog timeLow
	MachineStatus_iot[index++] = iotMachinePrevState;			//34
	MachineStatus_iot[index++] = iotDoorStatus;				//35

	for(i=0; i< LENGTH ; i++)
	{
		checksum1 = checksum1+ MachineStatus_iot[i];
	}
	checksum2 = checksum1 + checksum1;
	MachineStatus_iot[index++]=checksum1 ;
	MachineStatus_iot[index++]=checksum2 ;
 	if(g_uart2_tx_count == 0)
	{
		uartSend(&MachineStatus_iot[0],LENGTH+2);
	}
	//uartSend(&MachineStatus_iot[0],LENGTH+2);
}

/**********************************************************************
* Function: void ast_ReadData(void)
* Parameters: None
* Return: None
* Description: Send AST feedback  for setting control command
***********************************************************************/
void ast_ReadData(void)
{

	int16_t  index = 0,i=0;
	uint8_t checksum1=0,checksum2=0;

	MachineStatus_iot[index++]= HEADER;			//0
	//Check for setting control command
	if(data[0] == SETTING_CONTROL)
	{
		MachineStatus_iot[index++]= LENGTH3;	//1
		MachineStatus_iot[index++]= 0x83; 		//2
	}
	else
	{
		MachineStatus_iot[index++]= 0x0A;		//4
		MachineStatus_iot[index++]= 0x81;		//5
	}

	MachineStatus_iot[index++]= 0x00; 					//3
	MachineStatus_iot[index++]= iotApplianceType;		//4
	if(data[0] == SETTING_CONTROL)
	{
		MachineStatus_iot[index++]= get_SettingControl(); 	//5
	}
	else
	{
		MachineStatus_iot[index++]= 0x00;				//5
	}
	MachineStatus_iot[index++]= iotProgramSelection; 		//6
	MachineStatus_iot[index++]= iotFuncSetting;				//7
	MachineStatus_iot[index++]= astSupplyVoltage / 2; 		//8
	MachineStatus_iot[index++]= astDutyCycleInMin;			//9
	MachineStatus_iot[index++]= astDutyCycleInSec;			//10
	MachineStatus_iot[index++]= iotAlarmIndication[ALARM_3];//iotWaterLiter;				//11
	MachineStatus_iot[index++]= astStepTime;				//12
	MachineStatus_iot[index++]= iotSoakInMin;				//13
	MachineStatus_iot[index++]= (uint8_t)(iotBalanceTime / 60);		//14
	MachineStatus_iot[index++]= (uint8_t)(iotBalanceTime % 60);		//15
	MachineStatus_iot[index++]= (uint8_t)(iotMotorSpeed >> 8); 		//16
	MachineStatus_iot[index++]= (uint8_t)iotMotorSpeed;			//17
	MachineStatus_iot[index++]= iotWaterTemperature;			//18
	MachineStatus_iot[index++]= (uint8_t)(iotWaterLevel >> 8);		//19
	MachineStatus_iot[index++]= (uint8_t)(iotWaterLevel);			//20
	MachineStatus_iot[index++] = (uint8_t)(iotProgramTime / 60);	//21
	MachineStatus_iot[index++] = (uint8_t)(iotProgramTime % 60);	//22
	MachineStatus_iot[index++]= iotMachinePrevState;			//23
	MachineStatus_iot[index++]= iotDoorStatus;					//24
	MachineStatus_iot[index++]= iotOptionEnable[OPTION_ENABLE_2]; 	//25
	MachineStatus_iot[index++]= iotUnbalanceValue;			//26
	MachineStatus_iot[index++]= iotAlarmIndication[ALARM_1];		//27
	MachineStatus_iot[index++]= iotAlarmIndication[ALARM_2];		//28
	MachineStatus_iot[index++]= iotError;					//29
	MachineStatus_iot[index++]= iotOptionEnable[OPTION_ENABLE_1];	//30
	MachineStatus_iot[index++]= iotMachineState;			//31
	MachineStatus_iot[index++]= iotLoadFlag;				//32
	MachineStatus_iot[index++]= iotChildLock;				//33
	MachineStatus_iot[index++]= iotOutput[OUTPUT_1];		//34
	MachineStatus_iot[index++]= iotOutput[OUTPUT_2];		//35
	MachineStatus_iot[index++]= iotTestStep;				//36
	MachineStatus_iot[index++]= programLED[PROGRAM_LED_SEG_1];	//37
	MachineStatus_iot[index++]= statusLED[STATUS_LED_SEG_1];	//38
	MachineStatus_iot[index++]= optionLED[OPTION_LED_SEG_1];	//39
	MachineStatus_iot[index++]= knobLED[KNOB_LED_SEG_1];;		//40
	MachineStatus_iot[index++]= programLED[PROGRAM_LED_SEG_2];	//41
	MachineStatus_iot[index++]= statusLED[STATUS_LED_SEG_2];	//42
	MachineStatus_iot[index++]= optionLED[OPTION_LED_SEG_2];	//43
	MachineStatus_iot[index++]= astPresentStep;					//44

	for(i=0; i< LENGTH1 ; i++){
		checksum1 = checksum1+ MachineStatus_iot[i];
	}

	checksum2 = checksum1 + checksum1;
	MachineStatus_iot[index++]=checksum1 ;
	MachineStatus_iot[index++]=checksum2 ;
	
	if(g_uart2_tx_count == 0)
	{
		uartSend(&MachineStatus_iot[0],LENGTH3+2);
	}

	//uartSend(&MachineStatus_iot[0],LENGTH3+2);

	countforsend++;

}

/**********************************************************************
* Function: void iot_SendVersion(void)
* Parameters: None
* Return: None
* Description: Send feedback for send version
***********************************************************************/
void iot_SendVersion(void)
{
	int16_t index = 0,ibyte_count=0;
	uint8_t checksum1=0,checksum2=0;

	MachineStatus_iot[index++]= HEADER;		//0
	MachineStatus_iot[index++]= LENGTH;

	//Check for user command
	if(data[0] == USER_CONTROL)
	{
		MachineStatus_iot[index++]= 0x81; //2
	}
	else
	{
		MachineStatus_iot[index++]= 0x83;
	}

	MachineStatus_iot[index++]= data[1]; 	//3
	MachineStatus_iot[index++]= iotApplianceType;	//4

	if(data[0] == USER_CONTROL)
	{
		MachineStatus_iot[index++]= get_UserControl();//5
	}
	else
	{
		MachineStatus_iot[index++]= 0x00;
	}	

	MachineStatus_iot[index++]= WiFiSoftware[0];//6
	MachineStatus_iot[index++]= WiFiSoftware[1];//7
	MachineStatus_iot[index++]= WiFiSoftware[2];//8
	MachineStatus_iot[index++]= CONTROLLER_VERSION1;//9
	MachineStatus_iot[index++]= CONTROLLER_VERSION2;//10
	MachineStatus_iot[index++]= CONTROLLER_VERSION3;//11
	MachineStatus_iot[index++]= CONTROLLER_VERSION4;//12
	MachineStatus_iot[index++]= CONTROLLER_VERSION5;//13
	MachineStatus_iot[index++]= CONTROLLER_VERSION6;//14
	for(ibyte_count=0; ibyte_count< 24 ; ibyte_count++)
	{
		MachineStatus_iot[index++]= 0;
	}
	for(ibyte_count=0; ibyte_count< LENGTH; ibyte_count++)
	{
		checksum1 = checksum1+ MachineStatus_iot[ibyte_count];
	}
	checksum2 = checksum1 + checksum1;
	MachineStatus_iot[index++]=checksum1 ;
	MachineStatus_iot[index++]=checksum2 ;	
	
	if(g_uart2_tx_count == 0)
	{
		uartSend(&MachineStatus_iot[0],LENGTH+2);
	}

	//uartSend(&MachineStatus_iot[0],LENGTH+2);
}


/**********************************************************************
* Function: void iot_RebootSetting(void)
* Parameters: None
* Return: None
* Description: Rebooting command feedback
***********************************************************************/
void iot_RebootSetting(void)
{
	int16_t index = 0,i=0;
    uint8_t checksum1=0,checksum2=0;

    MachineStatus_iot[index++]= HEADER;     //0
    MachineStatus_iot[index++]= LENGTH2; // 1
    MachineStatus_iot[index++]= 0x81;

    MachineStatus_iot[index++]= get_UserControl();

    //All are reserved
    MachineStatus_iot[index++]= 0x00;
    MachineStatus_iot[index++]= 0x00;
    MachineStatus_iot[index++]= 0x00;
    MachineStatus_iot[index++]= 0x00;
    MachineStatus_iot[index++]= 0x00;
    MachineStatus_iot[index++]= 0x00;

    for(i=0; i< LENGTH1 ; i++)
    {
        checksum1 = checksum1+ MachineStatus_iot[i];
    }
    checksum2 = checksum1 + checksum1;
    MachineStatus_iot[index++]=checksum1 ;
    MachineStatus_iot[index++]=checksum2 ;
    if(g_uart2_tx_count == 0)
	{
		uartSend(&MachineStatus_iot[0],LENGTH2+2);
	}

    //uartSend(&MachineStatus_iot[0],LENGTH2+2);
}


/**********************************************************************
* Function: void ast_OptionSetting(void)
* Parameters: None
* Return: None
* Description: Feedback for response if option setting command received
***********************************************************************/
void ast_OptionSetting(void)
{
	int8_t index = 0,i = 0;
	uint8_t checksum1 = 0,checksum2 = 0;

	MachineStatus_iot[index++]= HEADER;			//0
	MachineStatus_iot[index++]= LENGTH2; 			//1
	//Check for HIL control
	if(get_UserOrSettingControl() == SETTING_CONTROL)
	{

		MachineStatus_iot[index++]= 0x83; 			//2
		MachineStatus_iot[index++]= get_SettingControl();	//3
	}
	else
	{
		MachineStatus_iot[index++]= 0x81;			//2
		MachineStatus_iot[index++]= 0x00;			//3
	}

    MachineStatus_iot[index++]= programLED[PROGRAM_LED_SEG_1];		//4
    MachineStatus_iot[index++]= statusLED[STATUS_LED_SEG_1];		//5
    MachineStatus_iot[index++]= optionLED[OPTION_LED_SEG_1];		//6
    MachineStatus_iot[index++]= iotOptionEnable[OPTION_ENABLE_1];	//7
    MachineStatus_iot[index++]= iotOptionEnable[OPTION_ENABLE_2];	//8
    MachineStatus_iot[index++]= iotOptionEnable[OPTION_ENABLE_3];	//9

    //Check Sum calculation
	for(i = 0; i < LENGTH2; i++) {
		checksum1 = checksum1+ MachineStatus_iot[i];
	}
	checksum2 = checksum1 + checksum1;

	MachineStatus_iot[index++]= checksum1 ;			//10
	MachineStatus_iot[index++]= checksum2 ;			//11
	
	if(g_uart2_tx_count == 0)
	{
		uartSend(&MachineStatus_iot[0],LENGTH2+2);
	}

	//uartSend(&MachineStatus_iot[0],LENGTH2+2);
}

/**********************************************************************
* Function: void iot_SendReadData1(void)
* Parameters: None
* Return: None
* Description: Feedback command for user command CMD_READ_DATA1
***********************************************************************/
/*void iot_SendReadData1(void)
{
	int index = 0,ibyte_count=0;

	uint8_t checksum1=0,checksum2=0;

	MachineStatus_iot[index++]= HEADER;				//0
	if(data[0] == USER_CONTROL)
	{
		MachineStatus_iot[index++]= LENGTH1; 		//1
		MachineStatus_iot[index++]= 0x81; 			//2
	}
	else
	{
		MachineStatus_iot[index++]= 0x0A;			//1
		MachineStatus_iot[index++]= 0x83;			//2
	}

	//Reserved
	MachineStatus_iot[index++]= 0x00; 				//3
	MachineStatus_iot[index++]= iotApplianceType;			//4

	if(data[0] == USER_CONTROL)
	{
		MachineStatus_iot[index++]= get_UserControl();	//5
	}
	else
	{
		MachineStatus_iot[index++]= 0x00;
	}

	MachineStatus_iot[index++]= iotProgramSelection; 	//6
	MachineStatus_iot[index++]= iotFuncSetting;		//7
	MachineStatus_iot[index++]= (uint8_t)(astSupplyVoltage / 2);	//8
	MachineStatus_iot[index++]= astDutyCycleInMin;			//9
	MachineStatus_iot[index++]= astDutyCycleInSec;		//10
	MachineStatus_iot[index++]= iotWaterLiter;			//11
	MachineStatus_iot[index++]= astStepTime;			//12
	MachineStatus_iot[index++]= iotSoakInMin;			//13
	MachineStatus_iot[index++]= (uint8_t)(iotBalanceTime / 60);	//14
	MachineStatus_iot[index++]= (uint8_t)(iotBalanceTime % 60);	//15
	MachineStatus_iot[index++]= (uint8_t)(iotMotorSpeed >> 8);	//16
	MachineStatus_iot[index++]= (uint8_t)(iotMotorSpeed);		//17
	MachineStatus_iot[index++]= iotWaterTemperature;			//18
	MachineStatus_iot[index++]= (uint8_t)(iotWaterLevel >> 8);	//19
	MachineStatus_iot[index++]= (uint8_t)iotWaterLevel;			//20
	MachineStatus_iot[index++]= (uint8_t)(iotProgramTime / 60);	//21
	MachineStatus_iot[index++]= (uint8_t)(iotProgramTime % 60);	//22
	MachineStatus_iot[index++]= iotMachinePrevState;		//23
	MachineStatus_iot[index++]= iotDoorStatus;				//24
	MachineStatus_iot[index++]= (uint8_t)(astSupplyCurrent/50);		//25	//Reference with Wifi document

	for(ibyte_count=0; ibyte_count< LENGTH1 ; ibyte_count++)
	{
		checksum1 = checksum1+ MachineStatus_iot[ibyte_count];
	}
	checksum2 = checksum1 + checksum1;
	MachineStatus_iot[index++]=checksum1 ;				//26
	MachineStatus_iot[index++]=checksum2 ;				//27

	uartSend(&MachineStatus_iot,LENGTH1+2);
}
*/
/**********************************************************************
* Function: void iot_SendReadData2(void)
* Parameters: None
* Return: None
* Description: Feedback command for user command CMD_READ_DATA1
***********************************************************************/
/*void iot_SendReadData2(void)
{
	int16_t index = 0,ibyte_count=0;
	uint8_t checksum1=0,checksum2=0;
	//Send data
	MachineStatus_iot[index++]= HEADER;				//0
	if(data[0] == USER_CONTROL)
	{
		MachineStatus_iot[index++]= LENGTH1; 		//1
		MachineStatus_iot[index++]= 0x81; 			//2
	}
	else
	{
		MachineStatus_iot[index++]= 0x0A;			//1
		MachineStatus_iot[index++]= 0x83;			//2
	}

	//Reserved//
	MachineStatus_iot[index++]= 0x00; 				//3
	MachineStatus_iot[index++]= iotApplianceType;			//4

	if(data[0] == USER_CONTROL)
	{
		MachineStatus_iot[index++]= get_UserControl();	//5
	}
	else
	{
		MachineStatus_iot[index++]= 0x00;
	}

	MachineStatus_iot[index++]= iotOptionEnable[OPTION_ENABLE_2];		//6
	MachineStatus_iot[index++]= iotUnbalanceValue;			//7
	MachineStatus_iot[index++]= iotAlarmIndication[ALARM_1];				//8
	MachineStatus_iot[index++]= iotAlarmIndication[ALARM_2];				//9
	MachineStatus_iot[index++]= iotError;				//10
	MachineStatus_iot[index++]= iotOptionEnable[OPTION_ENABLE_1];		//11
	MachineStatus_iot[index++]= iotMachineState;				//12
	MachineStatus_iot[index++]= iotLoadFlag;			//13
	MachineStatus_iot[index++]= iotChildLock;			//14
	MachineStatus_iot[index++]= iotOutput[OUTPUT_1];			//15
	MachineStatus_iot[index++]= iotOutput[OUTPUT_2];			//16
	MachineStatus_iot[index++]= programLED[PROGRAM_LED_SEG_1];			//17	prog led
	MachineStatus_iot[index++]= statusLED[STATUS_LED_SEG_1];			//18	status led
    MachineStatus_iot[index++]= optionLED[OPTION_LED_SEG_1];		//19
	MachineStatus_iot[index++]= knobLED[KNOB_LED_SEG_1];			//20	knob led
	MachineStatus_iot[index++]= iotOptionEnable[OPTION_ENABLE_3];			//21	Option Enable3

	MachineStatus_iot[index++]= astWeight;				//22
	MachineStatus_iot[index++]= astHumidity;			//23
	MachineStatus_iot[index++]= iotOutput[OUTPUT_3];			//24
	MachineStatus_iot[index++]= iotAlarmIndication[ALARM_3];	//25

	for(ibyte_count=0; ibyte_count< LENGTH1 ; ibyte_count++) {
		checksum1 = checksum1+ MachineStatus_iot[ibyte_count];
	}
	checksum2 = checksum1 + checksum1;
	MachineStatus_iot[index++]=checksum1 ;			//26
	MachineStatus_iot[index++]=checksum2 ;			//27

	uartSend(&MachineStatus_iot,LENGTH1+2);
}
*/
/* IoT related commands formatting and assigning byte code starts here */

/*======================= End Of IoT Code ================================*/

#endif 
