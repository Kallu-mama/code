#include "TemperatureCalculation.h"
#include "r_cg_userdefine.h"


void TemperatureCalculation(int iResultADC,uint8_t *buffer)
{
	float rtb_ADC_Calc;
	rtb_ADC_Calc = 0.0196F * iResultADC;
 	if (rtb_ADC_Calc >= 1.05F)
  	{
    		if (rtb_ADC_Calc >= 1.95F)
		{
     		rtb_ADC_Calc = (-22.94F * rtb_ADC_Calc) + 89.87F;
    		} 
		else 
		{
     			 rtb_ADC_Calc = (((rtb_ADC_Calc * rtb_ADC_Calc) * 17.04F) - (88.742F *
        		rtb_ADC_Calc)) + 153.81F;
    		}
  	} 
	else
	{
    		rtb_ADC_Calc = (-73.38F * rtb_ADC_Calc) + 156.5F;
  	}
 	*buffer = (uint8_t)rtb_ADC_Calc;
}