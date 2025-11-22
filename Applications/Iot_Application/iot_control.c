#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

#if ENABLE_IOT
#include "ProgramState.h"
//#include "motorcontrol.h"
#include "TestMode.h"
//#include "SpinProfiles.h"
#include "Selection.h"

#include "ProgramSelector.h"
#include "keyevent.h"
#include "STLED316S.h"
#include "iot_config.h"
#include "r_cg_serial.h"
#include "iot_control.h"
#include "iot_user_config.h"
#include "backup.h"
/* Local variables */
static uint8_t uDetergentCount=0;
static uint8_t uDiagnosticCount;

static int uDiagnostic_Led_Time;
static uint8_t uApSetCount;
static int WaterLevel;

uint8_t uRebootCount;
/* Global variables */
boolean bDiagnostic_Set_Flag;
char cMachineDiagnosticState;
uint8_t MotorSpeed;
boolean bDiagnosticMode;
int uDiagnostic_Time_Value;
uint8_t Program_LED1;
uint8_t Option_LED;
uint8_t Option_LED1;
uint8_t Status_LED1;
uint8_t Status_LED2;


uint8_t uDetergentKey;
uint8_t WiFi_Comn_Test = 17;
uint8_t WiFi_FCT_Comn_Test = 8;//6;

/* External variables */
extern uint16_t uidelaymin;
extern uint8_t uiProgram_Position,uiSoak_Press_count;
extern boolean bPhysical_DoorLock_Status,bTimeSaver_Selected,bCirculation_Pump;
extern uint8_t uiSmartProgram_Selected;
extern int cSwitch_Pressed;
extern uint16_t uikeypress_20mscount;
uint16_t Select;
//extern uint8_t uiSelectedTone;
extern volatile uint8_t uiSelectedTone;
extern uint8_t spin_first_press, uiRpmSel, uiPSER_Option_Avbl,uiDoorPulseCount,uidisp_select_option,uilast_selected_option;
extern uint8_t cDoor_Lock_sec,uiMachineState,uiSelected_Position,cError_Occured,cNextStep_Start_Delay;//bDoor_Lock_sec
extern boolean bDoorLockStatus,bDoorLock,bTEST_COMPLETED;
extern boolean bMainWashValve,bPrewashValve;
extern boolean bStart_Traic_Fire,bDisplay_Flag,Switch_Reset,uiover_heat,bDelay_Start;
extern boolean bTestTempRead,bStartButton,bChildLockButton;
extern uint8_t TempModeTemperature;
extern uint8_t cdigits_data[7],cdigits_data_array[6];//bStartButton;
extern uint16_t iWaterLevel,iMotor_Spinning_Seconds;
extern uint8_t bTEST_Position, uiUnbalance_Value;
extern uint8_t cTest_Forward,cFlash_Count;
extern uint8_t cStep_Seconds,uiStep_Minutes_Completed;
extern uint8_t prev_prgposition;
extern uint8_t ckey_digit_data[6];
extern char StepTime;
extern boolean bDrainPump,bHeater,bMotor_Start,bStop_Minutes_Count,bMotorTriacShort,bSpecialEnduranceSelected;
extern boolean bTestSpinValueRead,bWaterLevel_Read_Required,bTachoOpen,bNtcFailed,bSpin_Start;
extern boolean buzzerTone6, bProgram_Start, bProgramStart;
extern boolean bTestMode_Selection,bFCTMode_Selection,bHotRinse_Selected,bCold_Soak_Selected;
extern boolean TubClean_Activated,bTestHeat,bKeyDetected,bDoorPulse,bDelay_Finished,bWakeup_F;
extern boolean bExtraRinse_Flag;
extern uint8_t uiTemperature,uiBefore_Snooze,uisnooze_mode_pos;
extern uint16_t uiRPMValue,fSetSpeed;
extern unsigned char data[DATA_SIZE];
extern uint8_t LoadFlag,uiIDLE_COUNT,uiDISP_MCount;
uint8_t UserControl,SettingControl;
uint8_t UserOrSettingCntrl;
extern uint16_t uidelaymin, uikeypress_20mscount;
extern uint16_t BalanceTime;
extern uint16_t ProgramTime,ProgressTime;
extern uint8_t uiopt_check, temp_first_press, uiTempSel, uiExtraRinse_Press_count;
uint8_t Program_LED,Status_LED,BkMultiFuncSetting,testVar;
extern uint8_t WiFiMode;
extern uint8_t WiFiMode_bkp,uikey_debounce_delay,uiRMP_Default;
extern uint8_t uiPresent_Step,uiBeforeState,uiRinseholdState,uiError_Occured,uiIPM_Temperature;
extern volatile uint32_t g_tau0_ch3_width;
/* Function declaration of externally used functions */
extern void MotorStop();
extern void ResProg_led();
extern void Clear_Option_INPause();
extern void Power_OnOff(void);
extern unsigned char ucWrite_DataBuffer[MemoryLength];
/* change require */
static uint8_t Option_enable1;

/* Code for IoT functionality on switch press starts here */
void check_IOTRebootFlag()
{
	if(get_RebootFlag() == TRUE)
	{
		uRebootCount++;
		uiPSER_Option_Avbl = 0x00;
		if(uRebootCount > ONE_SECONDS_COUNT)
		{
			iot_AppReboot();
		}
	}	
	if((uiMachineState == SNOOZEMODE)&&(uiBefore_Snooze == IDLE_START))	
	{
		UserOrSettingCntrl = get_UserOrSettingControl();
		if((UserOrSettingCntrl == SETTING_CONTROL))
			bWakeup_F = TRUE;					
	}
}	
void UserDataSetting(void)
{
	//Check for Query
	if(get_RecvUserSettingQuery() == SET)
	{
		//Assign parameter
		UserControl = get_UserControl();
		SettingControl = get_SettingControl();
		Select = get_SelectCode();
		UserOrSettingCntrl = get_UserOrSettingControl();
		if(UserOrSettingCntrl == USER_CONTROL)
		{
			if((UserControl == CMD_MACHINE_SWITCH_ON)||(UserControl == CMD_MACHINE_SWITCH_OFF))
			{
				cSwitch_Pressed = POWER_KEY;
				Power_OnOff();
			}					
			else if((UserControl == CMD_IOT_FUNCTION_FLAG)&&((BkMultiFuncSetting & AP_MODE_SETTING)  == 0x02))//new added
			{
				clr_IOTMultiFuncSetting(AP_MODE_SETTING);/*resetting AP mode bit*/
			}
			else if((UserControl == CMD_READ_STATUS) && (get_WiFiMode() == 0x02) &&((BkMultiFuncSetting & AP_MODE_SETTING)  == 0x02) && ((get_IOTMultiFuncSetting() & AP_MODE_SETTING) == RESET_VALUE))//new added
			{
				//Clear bit
				BkMultiFuncSetting &= ~AP_MODE_SETTING;
			}
			else if((UserControl == CMD_IOT_FUNCTION_FLAG)&&(get_WiFiMode() == 0x00))//new added
			{
				clr_IOTMultiFuncSetting(DETERGENT_ORDER);//resetting detergent order bit//
				BkMultiFuncSetting &= ~DETERGENT_ORDER;
			}	
		}
		if((bChildLockButton == FALSE))
		{
			if(UserOrSettingCntrl == USER_CONTROL)
			{
				if((UserControl == CMD_START) && (uiMachineState != START_PRESSED) &&((uiBeforeState != PROGRAM_END)||(uiMachineState != SNOOZEMODE))&& (uiMachineState != PROGRAM_END) && (uiMachineState != TESTMODE) && (Switch_Reset == 0x01))
				{
					cSwitch_Pressed = START_KEY;					
					ButtonPressed();
				}
				else if((UserControl == CMD_PAUSE) && (uiMachineState != PAUSE_PRESSED) && ((uiBeforeState != PROGRAM_END)||(uiMachineState != SNOOZEMODE)) && (uiMachineState != PROGRAM_END) && (uiMachineState != TESTMODE) && (Switch_Reset == 0x01))
				{
					cSwitch_Pressed = START_KEY;
					ButtonPressed();
				}
				else if((UserControl == CMD_PROG_CHANGE)&&(uiMachineState != TESTMODE)&&(Switch_Reset == 0x01)&&(uiError_Occured == RESET_VALUE))
				{
					uiMachineState = PROGRAM_RESET;
					set_IOTState(0x00);  //Added by surya for resetting state
					set_SelectedProgram(0x00);
					uiSelected_Position = 0; //// this add for reset the uC commnad  add by manish
					bDoorPulse = FALSE; //This is added for clear the bEmegency_Lever_pull flag for door :added by jayesh
					bDelay_Finished = FALSE; //This is added for to select delay for selected program after program cancel: add by jayesh
					if(bSpecialEnduranceSelected == TRUE)
					{
						bSpecialEnduranceSelected = FALSE;
					}
				}												
			}

		}


		set_RecvUserSettingQuery(CLEAR);
	}
}


/* Diagnostic Mode Teting Code Starts Here */
void OptionDataSetting()
{
	if(get_RecvSettingQuery() == SET)
	{
		UserControl = get_UserControl();
		SettingControl = get_SettingControl();
		Select = get_SelectCode();
		UserOrSettingCntrl = get_UserOrSettingControl();

		if((UserOrSettingCntrl == SETTING_CONTROL) && ((bStartButton == 1)||(uiRinseholdState == 2)) && (SettingControl == CHILDLOCK_SETTING) && (uiMachineState != POWEROFF)||((bChildLockButton == 1)&&(bDoorLockStatus == FALSE)))
		{	//ChildLock button
			if((Select == 0x01) && (bChildLockButton == 0)) {
				cSwitch_Pressed = SPIN_TEMP_KEY;
			} else if((Select == 0x00) &&(bChildLockButton == 1)) {
				cSwitch_Pressed = SPIN_TEMP_KEY;
			}
			uikeypress_20mscount = 200;//100;	//changed for 7ms key changes.
			ButtonPressed();
		}
		else if ((UserOrSettingCntrl==SETTING_CONTROL)&&(uiMachineState == 0x03 || uiMachineState == 0x00 || uiMachineState == 0x01) && ((SettingControl == RINSEHOLD_SETTING) || (SettingControl == SPEED_SETTING)))
		{	//RinseHold Setting
			if ((Select == 1) && (uiRpmSel != Rinsehold))
			{
				uiRpmSel = Rpm1200;//Rpm1400; //Previous Position//Max RPM for Sub-30K 6.5Kg
			}
			else if ((Select == 0) && (uiRpmSel == Rinsehold)) {
				uiRpmSel = 0x00; //Previous position to Rpm00 to Set Rpm00
			}
			spin_first_press  = 2; //To avoid again to display the previos value
			uiSelectedTone = KEYPRESSTONE;
			SpinSelection();
		}
		if(bChildLockButton == FALSE)
		{
			if((bProgramStart == FALSE || uiMachineState == PAUSE_PRESSED)&&(UserOrSettingCntrl == SETTING_CONTROL)&&(uiMachineState != SNOOZEMODE))
			{
				//TimeSaver Options
				if((SettingControl ==TIME_SAVER)&&((Select == 0x01)||(Select == 0x00)) && (uiPSER_Option_Avbl & TimeSaver_Yes))
				{
					uiopt_check = TimeSaver_Yes;
					OptionSelection();
					uiopt_check = TimeSaver_Yes;
					uiSelectedTone = KEYPRESSTONE;
					SelectOption();
				}
				//PreWash Options
				else if((SettingControl == PREWASH_SETTING) && ((Select == 0x01)||(Select == 0x00)) && (uiPSER_Option_Avbl & Prewash_Yes))
				{
					uiopt_check = Prewash_Yes;
					OptionSelection();
					uiopt_check = Prewash_Yes;
					uiSelectedTone = KEYPRESSTONE;
					SelectOption();
				}

				//Extra Rinse				
				else if((SettingControl == EXTRARINSE_SETTING)&& (uiPSER_Option_Avbl & ExtraRinse_Yes))
				{
					if(Select == 0x00)
					{
						uiExtraRinse_Press_count = 3;
					}
					else if(Select == 0x01)
					{
						uiExtraRinse_Press_count = 0;
					}
					else if(Select == 0x02)
					{
						uiExtraRinse_Press_count = 1;
					}
					else
					{
						uiExtraRinse_Press_count = 2;
					}
					uiopt_check = ExtraRinse_Yes;
					OptionSelection();
					uiopt_check = ExtraRinse_Yes;
					uiSelectedTone = KEYPRESSTONE;
					bExtraRinse_Flag = TRUE;	//ADDED FOR INCREASING COUNT AS PER APP SELECTION - AK; 
					SelectOption();
				}
				// Steam				
				else if((SettingControl == STEAM_SETTING) && ((Select == 0x01)||(Select == 0x00)) && (uiPSER_Option_Avbl & Steam_Yes))
				{
					uiopt_check = Steam_Yes;
					OptionSelection();
					uiopt_check = Steam_Yes;
					uiSelectedTone = KEYPRESSTONE;

					SelectOption();
				}
				// HotRinse				
				else if((SettingControl == HOT_RINSE) && ((Select == 0x01)||(Select == 0x00)))
				{						
					if(Select == 0x01)
					{
						bHotRinse_Selected = TRUE;
						set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_HOT_RINSE);
						clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_TIMESAVER);
						ckey_digit_data[0] = ckey_digit_data[0] &  (~TimeSaver_Yes); // turn off navigation of Steam 
						uidisp_select_option = uidisp_select_option & (~TimeSaver_Yes);
						bTimeSaver_Selected = FALSE;
					}
					else
					{
						bHotRinse_Selected = FALSE;
						clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_HOT_RINSE);
					}					
					uiSelectedTone = KEYPRESSTONE;					
					ProgramTimming();
					
				}
				// Soak
				else if((SettingControl == SOAK_SETTING) && ((Select == 0x01)||(Select == 0x00)))
				{						
					if(Select == 0x01)
					{
						bCold_Soak_Selected = TRUE;						
						ckey_digit_data[0] = ckey_digit_data[0] &  (~TimeSaver_Yes); // turn off navigation of Steam 
						uidisp_select_option = uidisp_select_option & (~TimeSaver_Yes);
						bTimeSaver_Selected = FALSE;
						
						set_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_SOAK);
						//set_IOTSoakInMin(0x01);
						clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_TIMESAVER);
						//OptionSelection();
						//SelectOption();
					}
					else
					{
						bCold_Soak_Selected = FALSE;
						clr_IOTOptionEnable(OPTION_ENABLE_1, OPTION_1_SOAK);
						//set_IOTSoakInMin(0x00);
						//OptionSelection();
						//SelectOption();
					}
					ProgramTimming();
					uiSelectedTone = KEYPRESSTONE;					
				}
				// Warm Soak
				else if ((SettingControl == WARM_SOAK_SETTING)&&(Select == 0x01 || Select == 0x00 )&& (uiPSER_Option_Avbl & WarmSoak_Yes))
				{
					uiopt_check = WarmSoak_Yes;
					OptionSelection();
					uiopt_check = WarmSoak_Yes;
					uiSelectedTone = KEYPRESSTONE;
					SelectOption();
				}
				// Delay Start
				else if((SettingControl == DELAY_SETTING) && (uiPSER_Option_Avbl & DelayStart_Yes))
				{
					switch(get_SelectIndex()) {
					case 0:
						Select = 0;
						break;
					case 1:
						Select = 30;
						break;
					case 2:
						Select = 60;
						break;
					case 3:
						Select = 90;
						break;
					case 4:
						Select = 120;
						break;
					case 5:
						Select = 150;
						break;
					case 6:
						Select = 180;
						break;
					case 7:
						Select = 210;
						break;
					case 8:
						Select = 240;
						break;
					case 9:
						Select = 270;
						break;
					case 10:
						Select = 300;	//5 HRS
						break;
					case 11:
						Select = 360; //6 HRS
						break;
					case 12:
						Select = 420; //7
						break;
					case 13:
						Select = 480;//8
						break;
					case 14:
						Select = 540;//9
						break;
					case 15:
						Select = 600;//10
						break;
					case 16:
						Select = 660;	//11
						break;
					case 17:
						Select = 720;	//12
						break;
					case 18:
						Select = 780;	//13
						break;
					case 19:
						Select = 840;	//14
						break;
					case 20:
						Select = 900;	//15
						break;
					case 21:
						Select = 960;	//16
						break;
					case 22://
						Select = 1020;	//17
						break;
					case 23:
						Select = 1080; //18
						break;
					case 24:
						Select = 1140; //19
						break;
                      // NEO 3 & HALF SEGMENT DISPLAY HENCE MAX DELAY START OF 19: 00 HRS.
//					case 25:
//						Select = 1200;
//						break;
//					case 26:
//						Select = 1320;
//						break;
//					case 27:
//						Select = 1440;
//						break;
					default:
						Select = 1441; //Invalid
						break;
					}
					//uiSelectedTone = KEYPRESSTONE;
					Set_Delay(Select);
				}
					//Below program selection is used only in case of program setting through HIL
				else if ((uiMachineState == 0x03 || uiMachineState == 0x00) && (SettingControl==PROGRAM_SETTING)) {
					set_IOTprogram(get_SelectedProgram());
				}
				else if ((uiMachineState == 0x03 || uiMachineState == 0x00 || bDelay_Start == TRUE) && (SettingControl==TEMPERATURE_SETTING)) {
					Set_Temperature((uint8_t)Select);
				}
				else if((uiMachineState == 0x03 || uiMachineState == 0x00 || uiMachineState == 0x01)&& (SettingControl==SPEED_SETTING)) {
					//spin selection
					switch(get_SelectIndex()) {
					case 0:
						Select = 0;
						break;
					case 1:
						Select = 400;
						break;
					case 2:
						Select = 600;
						break;
					case 4:
						Select = 800;
						break;
					case 6:
						Select = 1000;
						break;
					case 7:
						Select = 1200;
						break;
//					case 8:
//						Select = 1400;
//						break;
					default:
						Select = 1; //Invalid
						break;
					}
					uiSelectedTone = KEYPRESSTONE;
					Set_Spin(Select);
				}
				
			}
			
		}
		set_RecvSettingQuery(CLEAR);
	}
}

void set_IOTprogram(unsigned char code)
{
	if(uiMachineState==IDLE_START||uiMachineState==PROGRAM_END||uiMachineState == SNOOZEMODE)
	{
		
		TubClean_Activated = FALSE;//shu
		if((uiMachineState == SNOOZEMODE)&&(uiBefore_Snooze == IDLE_START))
		{
			uisnooze_mode_pos = 0;
		}
		switch(code) {
		case MIXEDSOIL:
			uiSelected_Position = uiProgram_Position = MIXEDSOIL;			
			break;
		case COTTON:
			uiSelected_Position = uiProgram_Position = COTTON;
			break;
		case SYNTHETIC:
			uiSelected_Position = uiProgram_Position = SYNTHETIC;	//SYNTHETIC		
			break;
		case BABYWEAR:
			uiSelected_Position = uiProgram_Position = BABYWEAR;			
			break;	
		case STEAMFRESH:
			uiSelected_Position = uiProgram_Position = STEAMFRESH;
			break;	
			
		case ADDITIVESRINSE:
			uiSelected_Position = uiProgram_Position = ADDITIVESRINSE;
			break;
		case CRADLEWASH:
			uiSelected_Position = uiProgram_Position = CRADLEWASH;			
			break;
		case CURTAINS:
			uiSelected_Position = uiProgram_Position = CURTAINS;	
			break;
//		case WETCLEAN:
//			uiSelected_Position = uiProgram_Position = WETCLEAN; //WETCLEAN
//			break;
		case EXPRESS:
			uiSelected_Position = uiProgram_Position = EXPRESS;			
			break;
			
		case IOT_STEAMWASH:
			uiSelected_Position = uiProgram_Position = SMARTPLUS;
			uiSmartProgram_Selected = STEAMWASH;
			set_IOTProgramSelection(IOT_STEAMWASH);
			break;
		case IOT_INNERWEAR:
			uiSelected_Position = uiProgram_Position = SMARTPLUS;			
			uiSmartProgram_Selected = INNERWEAR;
			set_IOTProgramSelection(IOT_INNERWEAR);
			break;
//		case IOT_CURTAINS:
//			uiSelected_Position = uiProgram_Position = SMARTPLUS;		
//			uiSmartProgram_Selected = CURTAINS;
//			set_IOTProgramSelection(IOT_CURTAINS);
//			break;
		case IOT_DARKWASH:
			uiSelected_Position = uiProgram_Position = SMARTPLUS;			
			uiSmartProgram_Selected = DARKWASH;
			set_IOTProgramSelection(IOT_DARKWASH);
			break;
		case IOT_TUBCLEAN:
			TubClean_Activated = TRUE;
			uiSelected_Position = uiProgram_Position = SMARTPLUS;//Otional with additive rinse			
			uiSmartProgram_Selected = TUBCLEAN;			
			set_IOTProgramSelection(IOT_TUBCLEAN);			
			break;
		case IOT_SPORTWEAR:
			uiSelected_Position = uiProgram_Position = SMARTPLUS;			
			uiSmartProgram_Selected = SPORTWEAR;
			set_IOTProgramSelection(IOT_SPORTWEAR);
			break;
		case IOT_UNIFORM:
			uiSelected_Position = uiProgram_Position = SMARTPLUS;			
			uiSmartProgram_Selected = UNIFORM;
			set_IOTProgramSelection(IOT_UNIFORM);
			break;
		case IOT_JEANS:
			uiSelected_Position = uiProgram_Position = SMARTPLUS;			
			uiSmartProgram_Selected = JEANS;
			set_IOTProgramSelection(IOT_JEANS);
			break;	
		case IOT_SHIRTS:
			uiSelected_Position = uiProgram_Position = SMARTPLUS;			
			uiSmartProgram_Selected = SHIRTS;
			set_IOTProgramSelection(IOT_SHIRTS);
			break;
		case IOT_LINEN:
			uiSelected_Position = uiProgram_Position = SMARTPLUS;			
			uiSmartProgram_Selected = LINENWASH;
			set_IOTProgramSelection(IOT_LINEN);
			break;
		case IOT_WOOLLENS:
			uiSelected_Position = uiProgram_Position = SMARTPLUS;			
			uiSmartProgram_Selected = WOOLLENS;
			set_IOTProgramSelection(IOT_WOOLLENS);
			break;
		case IOT_ANTIALLERGEN:
			uiSelected_Position = uiProgram_Position = SMARTPLUS;			
			uiSmartProgram_Selected = ANTIALLERGEN;
			set_IOTProgramSelection(IOT_ANTIALLERGEN);
			break;
		case IOT_EXPRESS_30:
			uiSelected_Position = uiProgram_Position = SMARTPLUS;			
			uiSmartProgram_Selected = EXPRESS_30;
			set_IOTProgramSelection(IOT_EXPRESS_30);
			break;
		default:
			break;
		}
		uiSelectedTone = KEYPRESSTONE;
		ProgramTime = 0;
		ProgressTime = 0;
		reset_IOTProgramOption();
		Reset_ProgParamter();
		if(uiSmartProgram_Selected == TUBCLEAN)
		{
			set_IOTTemperature(SET_TEMP_95);
		}
		/*Set programming selection*/
		if(uiSelected_Position != SMARTPLUS)
		{
			set_IOTProgramSelection(uiSelected_Position);
		}
	}
}

void Set_Delay(uint16_t Delay) {
	uint8_t valid = 1;

	uidelaymin = Delay;

	if (Delay == 0) {
		uidelaymin = 1440 + 120; //To Deselct the Delay option
		valid = 1;
	}
	else if( Delay > 0 && Delay <= 300)
	{
		if ((Delay % 30) == 0) {
			uidelaymin = Delay - 30;
			valid = 1;
		}
	}
	else if((Delay > 300)&&(Delay <= 1140))//720
	{
		if ((Delay % 60) == 0) {
			uidelaymin = Delay - 60;
			valid = 1;
		}
	}
	else if((Delay > 720)&&(Delay <= 1440))
	{
		if ((Delay % 120) == 0) {
			uidelaymin = Delay - 120;
			valid = 1;
		}
	}

	if (valid = 1) {
		uiopt_check = DelayStart_Yes;
		OptionSelection();
		uiopt_check = DelayStart_Yes;
		uiSelectedTone = KEYPRESSTONE;
		SelectOption();
	}
}

void Set_Temperature (uint8_t Temp)
{
	if((bStartButton == RESET_VALUE) && (bProgram_Start == FALSE)) {
		set_IOTOptionEnable(OPTION_ENABLE_2,OPTION_2_TEMPERATURE);
		switch(Temp) {
		case 2:	//WashCold
				uiTempSel = 0; //Previous Select value			
			break;
		case 3:	//Wash30			
				uiTempSel = 1; //Previous Select value							
			break;
		case 4:	//Wash40						
				uiTempSel = 2; //Previous Select value							
			break;
		case 5:	//Wash60							
				uiTempSel = 3; //Previous Select value							
			break;
		case 6:	//Wash95			
				uiTempSel = 4; //Previous Select value							
			break;
		/*case 7:	//Wash20			
				uiTempSel = 1; //Previous Select value							
			break;*/
		case 8: //Wash 40EC
				uiTempSel = 5; //Previous Select value
			break;
		case 9: //Wash 60EC
				uiTempSel = 6; //Previous Select value
			break;
		default:
			break;
		}
		if( uidisp_select_option & Prewash_Yes)
		{
			uiTempSel = uiTempSel + APP_PREWASH_ON_OFF;
		}
		temp_first_press  = 2; //To avoid again to display the previos value
		uiSelectedTone = KEYPRESSTONE;
		TemperatureSelection();
	}
	else{
		clr_IOTOptionEnable(OPTION_ENABLE_2,OPTION_2_TEMPERATURE);
	}
}

void Set_Spin (uint16_t Spin)
{
	if(((bStartButton == RESET_VALUE) && (bProgram_Start == FALSE)) || uiMachineState == PAUSE_PRESSED)
	{
		set_IOTOptionEnable(OPTION_ENABLE_2,OPTION_2_SPIN_SPEED);
		switch(Spin) {
		case 0:		//RPM0
			uiRpmSel = 0x00; //Previous Position
			break;
		case 400:	//RPM400
			uiRpmSel = 0x10; //Previous Position
			break;
		case 600:	//RPM600
			uiRpmSel = 0x20; //Previous Position			
			break;
		case 800:	//RPM800
			uiRpmSel = 0x30; //Previous Position			
			break;
		case 1000:	//RPM1000		
			uiRpmSel = 0x40; //Previous Position							
			break;
		case 1200:	//RPM1200			
			uiRpmSel = 0x50; //Previous Position							
			break;
//		case 1400:	//RPM1400			
//			uiRpmSel = 0x60; //Previous Position							
//			break;
		default:
			break;
		}
		spin_first_press  = 2;//To avoid again to display the previos value
		SpinSelection();
	}
	else {
		clr_IOTOptionEnable(OPTION_ENABLE_2,OPTION_2_SPIN_SPEED);
	}
}

//This will handler iot communication
//This function is called from Main.c
void IOT_CommHandler(void)
{
	if(get_SendAck() == 1)
	{
		if(MachineErrors.EBits.WifiComnError == 1)
		{
			MachineErrors.EBits.WifiComnError = RESET_VALUE;
		}
		WiFi_FCT_Comn_Test = 8;//6;
		WiFi_Comn_Test = 17;
		
		//Update selected program
		updateSelectedProgram();
		//This function update parameter at runtime
	//	updateIotParam(); //commented and taken down for 1second wifi comm update

		//Set user parameter and
		OptionDataSetting();
		UserDataSetting();
		
		updateIotParam(); //for 1second wifi comm update

		//Send response
		sendResponse();
		//Reset ack
		set_SendAck(0);
		
		//AIComm_FrameTx();
	}
}

//This is function update program selection
void updateSelectedProgram(void)
{
	if(uiSelected_Position == SMARTPLUS)
	{	
		switch(uiSmartProgram_Selected)
		{
			
			case INNERWEAR : 
					   set_IOTProgramSelection(IOT_INNERWEAR);
					   break;
//			case CURTAINS	  :
//					   set_IOTProgramSelection(IOT_CURTAINS);
//					   break;
			case DARKWASH	  :
					   set_IOTProgramSelection(IOT_DARKWASH);
					   break;
			case TUBCLEAN	  :
					   set_IOTProgramSelection(IOT_TUBCLEAN);
					   break;		   
			case SPORTWEAR  :
					   set_IOTProgramSelection(IOT_SPORTWEAR);
					   break;
			case UNIFORM	  :
					   set_IOTProgramSelection(IOT_UNIFORM);
				           break;
			case JEANS	  :
					   set_IOTProgramSelection(IOT_JEANS);
				           break;
			case SHIRTS	  :
					   set_IOTProgramSelection(IOT_SHIRTS);
				           break;
			case STEAMWASH	  :
					   set_IOTProgramSelection(IOT_STEAMWASH);
				           break;
			case LINENWASH    :
					   set_IOTProgramSelection(IOT_LINEN);
				           break;
			case WOOLLENS     :
					   set_IOTProgramSelection(IOT_WOOLLENS);
				           break;
			case ANTIALLERGEN :
					   set_IOTProgramSelection(IOT_ANTIALLERGEN);
				           break;
			case EXPRESS_30   :
					   set_IOTProgramSelection(IOT_EXPRESS_30);
				           break;
			default	          :
					   break;
		}
	}
	else
	{
		//TubClean_Activated = RESET_VALUE;//shu
		set_IOTProgramSelection(uiSelected_Position);
	}
}




//This is custom function which update delay update parameter
void updateIotParam(void)
{
	//Update motor speed, level, balance time
	set_IOTMotorSpeed(uiRPMValue);
	set_IOTWaterLevel(iWaterLevel);
	set_IOTBalanceTime(BalanceTime);
	set_IOTProgramTime(ProgramTime);
	set_IOT_IPMtemp(uiIPM_Temperature);

	if((bDoorLockStatus == TRUE)&&(bPrewashValve == TRUE))
	{
		set_IOTLoadFlag(LOAD_PREWASH); 
	}
	else if((bDoorLockStatus == TRUE)&&(bMainWashValve == TRUE))
	{
		set_IOTLoadFlag(LOAD_MAINWASH); 
	}
	else if((bDoorLockStatus == TRUE)&&(bDrainPump == TRUE))
	{
		set_IOTLoadFlag(LOAD_DRAIN); 
	}
	else if((bDoorLockStatus == TRUE)&&(bHeater == TRUE))
	{
		set_IOTLoadFlag(LOAD_HEATER); 
	}
	else
	{
		set_IOTLoadFlag(0);
	}

	//Door locking status
	if((bDoorLock == TRUE)&&(bDoorLockStatus == TRUE))
	{
		set_IOTDoorStatus(DOOR_LOCKED);/*Door Locked*/ 
	}
	else if((bDoorLock == TRUE)&&(bDoorLockStatus == FALSE))
	{
		set_IOTDoorStatus(DOOR_LOCKING);/*Door locking*/
	}
	else if((bDoorLock == FALSE)&&(bDoorLockStatus == TRUE))
	{
		set_IOTDoorStatus(DOOR_UNLOCKING);/*Door unlocking*/
	}
	else
	{
		set_IOTDoorStatus(DOOR_UNLOCKED);/*Door unlocked*/
	}

	/*Update No of Extra Rinse*/

	set_IOTExtraRinse(uiExtraRinse_Press_count);

	/*Update water temperature*/
	set_IOTWaterTemperature((uint8_t)uiTemperature);

	/*Unbalance value*/
	set_IOTUnbalanceValue(uiUnbalance_Value);

	set_ASTStepTime(uiStep_Minutes_Completed);

	set_IOTChildLock((uint8_t)bChildLockButton);

	set_IOTStartUpDelayInMin(uidelaymin);

	set_ASTPresentStep(uiPresent_Step);
	
	setIOT_temp(uiTempSel);   //
	
	setIOT_spinSel(uiRpmSel); //
	
}



void reset_IOTparam()
{
	set_SelectedProgram(0x00);
	

	set_IOTState(0x00);//Clear IotState during machine Power off.

	/*Set child lock*/
	set_IOTChildLock(0);

	set_IOTMotorSpeed((uint16_t)0);

	/*Update water temperature*/
	set_IOTWaterTemperature((uint8_t)0);
	/*Water level*/
	set_IOTWaterLevel(0);
	/*Reset unbalanced*/
	set_IOTUnbalanceValue(0);
	
	set_IOT_IPMtemp(0);

	set_IOTLoadFlag(RESET_VALUE);
	if((uiMachineState != POWEROFF) && (uiMachineState != RESTARTPOWER))
	{
		reset_IOTProgramOption();
	}
	set_IOTAlarm(ALARM_1, 0x00);
	set_IOTAlarm(ALARM_2, 0x00);
	set_IOTAlarm(ALARM_3, 0x00);
}

void setIOT_temp(uint8_t selected_temp)
{	
	set_IOTOptionEnable(OPTION_ENABLE_2,OPTION_2_TEMPERATURE);	
	switch(selected_temp)
	{
		case  WashCold:	
		case P_COLD:	//ADDED TO UPDATE THE TEMP IF PREWASH IS ALSO SELECTED. - AK
			set_IOTTemperature(SET_TEMP_COLD);	
			break;
			
		/*case  Wash20:		
			set_IOTTemperature(SET_TEMP_20);	
			break;*/	
			
		case  Wash30:
		case P_30:	//ADDED TO UPDATE THE TEMP IF PREWASH IS ALSO SELECTED. - AK
			set_IOTTemperature(SET_TEMP_30);		
			break;
			
		case  Wash40:
		case P_40:	//ADDED TO UPDATE THE TEMP IF PREWASH IS ALSO SELECTED. - AK
			set_IOTTemperature(SET_TEMP_40);		
			break;
			
		case  Wash60:
		case P_60:	//ADDED TO UPDATE THE TEMP IF PREWASH IS ALSO SELECTED. - AK
			set_IOTTemperature(SET_TEMP_60);		
			break;
			
		case Wash95:
		case P_95:	//ADDED TO UPDATE THE TEMP IF PREWASH IS ALSO SELECTED. - AK
			set_IOTTemperature(SET_TEMP_95);
			break;
			
		case Wash40E:				
			set_IOTTemperature(SET_TEMP_40E);
			break;
		
		case Wash60E:				
			set_IOTTemperature(SET_TEMP_60E);
			break;					
		default:						
			break;
	}					
}

void setIOT_spinSel(uint8_t selected_rpm)
{	
	if(selected_rpm == Rinsehold){
		clr_IOTOptionEnable(OPTION_ENABLE_2,OPTION_2_SPIN_SPEED);
		set_IOTOptionEnable(OPTION_ENABLE_2,OPTION_2_RINSE_HOLD);
		set_IOTSpeedSetting(NO_SPIN);
	}
	else
	{
		set_IOTOptionEnable(OPTION_ENABLE_2,OPTION_2_SPIN_SPEED);
		clr_IOTOptionEnable(OPTION_ENABLE_2,OPTION_2_RINSE_HOLD);
	}
	switch(selected_rpm)
	{								
		case  Rpm000:			
			set_IOTSpeedSetting(NO_SPIN);			
		break;
		case  Rpm400:			
			set_IOTSpeedSetting(SPIN_400);		
		break;
		case  Rpm600:			
			set_IOTSpeedSetting(SPIN_600);			
		break;
		case  Rpm800:			
			set_IOTSpeedSetting(SPIN_800);			
		break;
		case Rpm1000:			
			set_IOTSpeedSetting(SPIN_1000);
		break;
		case Rpm1200:			
			set_IOTSpeedSetting(SPIN_1200);
		break;		
//		case Rpm1400:			
//			set_IOTSpeedSetting(SPIN_1400);
//		break;		
		default :		
		break;
		
	}	
}

void iot_AppReboot(void)
{
	static const unsigned char illegalInstruction = 0xFF;
	void (*dummy) (void) = (void (*)(void))&illegalInstruction;
	DI();

	ucWrite_DataBuffer[131] = 1; //bFirmwareUpgrade
	MemoryWrite();
	MotorStop();
	bDrainPump = FALSE;
	bHeater = FALSE;
	bPrewashValve = FALSE;
	bMainWashValve = FALSE;
	bCirculation_Pump = FALSE;
	ComponentSwitchOff();
	dummy();
}

void initIOTCommuncation(void)
{	
	set_IOTApplianceType(APPLIANCES_WASHING_MACHINE);
}



//This function add for dynamic UART selection
MD_STATUS uartSend(uint8_t * const tx_buf, uint16_t tx_num)
{
	return R_UART2_Send(tx_buf,tx_num);

}




#endif
