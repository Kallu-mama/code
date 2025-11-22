#ifndef __IOT_USER_CONFIG_H__
#define __IOT_USER_CONFIG_H__

#include "Version.h"

/* Settings for IoT related functionality */
#define DIAGNOSTIC_MODE_SWITCH (OPTION_KEY + SELECT_KEY)

/* Versions settings */
#define SFVERSION 			0x10

#define CONTROLLER_VERSION1 		0x04 //0x02 //0x01
#define CONTROLLER_VERSION2 		0x00
#define CONTROLLER_VERSION3 		IB_PRODUCTION_VER_IOT //Top board //0x08

#define CONTROLLER_VERSION4 		0x01
#define CONTROLLER_VERSION5 		0x00
#define CONTROLLER_VERSION6 		PB_PRODUCTION_VER_IOT	//Power board //0x0A
/* Temperature Setting */
typedef enum
{
	IOT_TEMP_COLD = 0,
	IOT_TEMP_30,
	IOT_TEMP_40,
	IOT_TEMP_60,
	IOT_TEMP_95,
	//IOT_TEMP_20,
	IOT_TEMP_40EC,
	IOT_TEMP_60EC,
	TEMP_MAX_OPTION
}TEMP_OPTIONS;



#endif

