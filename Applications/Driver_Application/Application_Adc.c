#include "Application_Adc.h"
#include "r_cg_adc.h"

extern uint8_t uiNTCDigital,IB_ADC_Value;

void Application_Adc()
{
	 static uint8_t uiChannel_Selection;
	 
    	uiChannel_Selection++;
	switch(uiChannel_Selection)
	{
		case 1:	 uiNTCDigital = ADCRH; //Temperature i/p
			ADS = _01_AD_INPUT_CHANNEL_1; 
			break;
		case 2: IB_ADC_Value=ADCRH;  //Encoder i/p
			ADS = _00_AD_INPUT_CHANNEL_0; 
			break;
		default:break;
	}
	if(uiChannel_Selection > 1)
	uiChannel_Selection = RESET_VALUE;
}