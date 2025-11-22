#include "Buzzer.h"


extern uint16_t BuzzerFreq,mybuzzerdelay;
extern void R_TAU0_Channel3_Start();
extern void R_TAU0_Channel3_Stop();
extern boolean buzzer_on;
extern boolean bBuzzerStart;
extern uint8_t buzzerarray,SatrtUpDelay;
//extern uint8_t buzzercount,uiToneFreqSate,uiSelectedTone;
extern uint8_t buzzercount;
extern volatile uint8_t uiToneFreqSate;
extern volatile uint8_t uiSelectedTone;
extern uint8_t uiMachineState;
extern uint8_t uiMachineState;

//Buzzer StartTone[3]={{225,FREQUENCY1240},{225,FREQUENCY1570},{260,FREQUENCY2050}};
//Buzzer PauseTone ={1500,FREQUENCY2050};
//Buzzer StartUpTone = {300,FREQUENCY2050};
//Buzzer EndTone[3] = {{1800,FREQUENCY2050},{1800,FREQUENCY2050},{1800,FREQUENCY2050}};
//Buzzer ErrorTone[5] = {{250,FREQUENCY2050},{250,FREQUENCY2050},
//			{250,FREQUENCY2050},{250,FREQUENCY2050},{250,FREQUENCY2050}};
//Buzzer KeyPressTone =  {300,FREQUENCY2050}; 
//Buzzer InApplicable[2] = {{200,FREQUENCY2050},{200,FREQUENCY2050}};

//values changed to match WDR tone
Buzzer StartTone[3]={{198,FREQUENCY1240},{200,FREQUENCY1570},{700,FREQUENCY2050}};//{{225,FREQUENCY1240},{225,FREQUENCY1570},{260,FREQUENCY2050}};
Buzzer PauseTone = {1398,FREQUENCY3050};//{1500,FREQUENCY2050};
Buzzer StartUpTone = {700,FREQUENCY2050};//{300,FREQUENCY2050};
Buzzer EndTone[3] = {{1398,FREQUENCY3050},{1398,FREQUENCY3050},{1398,FREQUENCY3050}};//{{1800,FREQUENCY2050},{1800,FREQUENCY2050},{1800,FREQUENCY2050}};
Buzzer ErrorTone[5] = {{199,FREQUENCY1570},{199,FREQUENCY1570},{199,FREQUENCY1570},{199,FREQUENCY1570},{199,FREQUENCY1570}};//{{250,FREQUENCY2050},{250,FREQUENCY2050},{250,FREQUENCY2050},{250,FREQUENCY2050},{250,FREQUENCY2050}};
Buzzer KeyPressTone =  {700,FREQUENCY2050};//{300,FREQUENCY2050}; 
Buzzer InApplicable[2] = {{200,FREQUENCY2050},{300,FREQUENCY2050}};//{{200,FREQUENCY2050},{200,FREQUENCY2050}};
Buzzer KeyPressTone2 =  {700,FREQUENCY2050};//{300,FREQUENCY2050};	//ADDED FOR REMOVING BUZZER DELAY - AK
Buzzer EncoderTone =  {190,FREQUENCY4050};

uint8_t prev_tone;
static boolean result;


void ToneGeneration()
{
	if(SatrtUpDelay == RESET_VALUE)
	{
		if((mybuzzerdelay > 0)&&(prev_tone != uiSelectedTone))
		{
			R_TAU0_Channel3_Stop();
			prev_tone = uiSelectedTone;
			P6.3 = RESET_VALUE;	
			//bBuzzerStart = 0;
			mybuzzerdelay = uiToneFreqSate = 0;	
			buzzercount = 0;
			buzzerarray = 0;
		}
		switch(uiSelectedTone)
		{
			
			case STARTUPTONE:
					ToneFrequencyGenerator(&StartUpTone);//start up
					break;
			case STARTPRESSTONE:
					
					ToneFrequencyGenerator(&StartTone[buzzerarray]); // start
					break;
			case PAUSETONE:
					ToneFrequencyGenerator(&PauseTone);//pause
					break;
			case ENDTONE:
					ToneFrequencyGenerator(&EndTone[buzzerarray]);//end
					break;
			case ERRORTONE:
					ToneFrequencyGenerator(&ErrorTone[buzzerarray]);//error
					break;
			case KEYPRESSTONE: 
					ToneFrequencyGenerator(&KeyPressTone);// program /key press
					break;
			case INAPPLICABLE:
					ToneFrequencyGenerator(&InApplicable[buzzerarray]);//inapplicable
					break;
			case NOTONE:
					P6.3 = RESET_VALUE;
					break;		
			case KEYPRESSTONE2: 		//ADDED FOR REMOVING BUZZER DELAY - AK
					ToneFrequencyGenerator(&KeyPressTone2);// program key press
					break;
			case ENCODERTONE:
					ToneFrequencyGenerator(&EncoderTone);// Buzzer rotation
					break;
			default:
				break;
		}
		prev_tone = uiSelectedTone;
	}
	else
	{
		P6.3 = RESET_VALUE;
		mybuzzerdelay = 0;
	}
}

char  ToneFrequencyGenerator(Buzzer *SelectedTone)
{
	
	switch(uiToneFreqSate)
	{
//		case TONEINTIALIZE:
//					bBuzzerStart = TRUE;
//					if(SelectedTone->ToneFrequency == FREQUENCY1240)
//					{
//						TDR03 = 0x08CC;	
//						R_TAU0_Channel3_Start();
//						uiToneFreqSate = TONECHARGE;
//					}
//					else if(SelectedTone->ToneFrequency == FREQUENCY1570)
//					{
//						TDR03 = 0x0711;
//						R_TAU0_Channel3_Start();
//						uiToneFreqSate = TONECHARGE;
//					}
//					else if(SelectedTone->ToneFrequency == FREQUENCY2050)
//					{
//						TDR03 = 0x05CC;
//						R_TAU0_Channel3_Start();
//						uiToneFreqSate = TONECHARGE;
//					}
//					else
//					{
//						R_TAU0_Channel3_Stop();
//					}
//					
//					break;
//		case TONECHARGE:
//					// on time
//					
//				 	if(mybuzzerdelay <= (SelectedTone->WaveWidth/2))
//				    	{
//						P6.3 = 1;
//						
//				    	}
//					else
//					{
//		
//						uiToneFreqSate = TONEDISCHARGE;						
//					}
//					//result = STEP_INPROGRESS;
//					break;
		case TONEINTIALIZE:
		
					
					bBuzzerStart = TRUE;
					
					if(((SelectedTone->ToneFrequency) == (FREQUENCY1240)))
					{
						TDR03 = 0x22C0;//0x08CC;	
						R_TAU0_Channel3_Start();
						uiToneFreqSate = TONECHARGE1;
					}
					else if(((SelectedTone->ToneFrequency) == (FREQUENCY1570)))
					{
						TDR03 = 0x1840;//0x0711;
						R_TAU0_Channel3_Start();
						uiToneFreqSate = TONECHARGE1;
					}
					else if(((SelectedTone->ToneFrequency) == (FREQUENCY3050)))
					{
						TDR03 = 0x1720;//0x05CC;
						R_TAU0_Channel3_Start();
						uiToneFreqSate = TONECHARGE3;
					}
					else if(((SelectedTone->ToneFrequency) == (FREQUENCY2050)))
					{
						TDR03 = 0x1720;//0x05CC;
						R_TAU0_Channel3_Start();
						/*if((uiSelectedTone==PAUSETONE)||(uiSelectedTone==ENDTONE)){
							uiToneFreqSate = TONECHARGE3;
						}
						else{*/
						uiToneFreqSate = TONECHARGE2;
						//}
					}
					else if(((SelectedTone->ToneFrequency) == (FREQUENCY4050)))
					{
						TDR03 = 0x1652;//0x1720;//0x05CC;
						R_TAU0_Channel3_Start();
						uiToneFreqSate = TONECHARGE4;
					}
					
					else
					{
						R_TAU0_Channel3_Stop();
					}
					
					break;
		case TONECHARGE1:
					// on time
					
				 	if(mybuzzerdelay <= (SelectedTone->WaveWidth/2))
				    	{
						P6.3 = 1;
						
				    	}
					else
					{
		
						uiToneFreqSate = TONEDISCHARGE;						
					}
					//result = STEP_INPROGRESS;
					break;
		case TONECHARGE2:
					// on time
					
				 	if(mybuzzerdelay <= (SelectedTone->WaveWidth/7))
				    	{
						P6.3 = 1;
						
				    	}
					else
					{
		
						uiToneFreqSate = TONEDISCHARGE;						
					}
					//result = STEP_INPROGRESS;
					break;
		case TONECHARGE3:
					// on time
					
				 	if(mybuzzerdelay <= ((SelectedTone->WaveWidth)*0.71)/*/1.4*/)
				    	{
						P6.3 = 1;
						
				    	}
					else
					{
		
						uiToneFreqSate = TONEDISCHARGE;						
					}
					//result = STEP_INPROGRESS;
					break;
		case TONECHARGE4:
					if(mybuzzerdelay <= (SelectedTone->WaveWidth/4))
					{
						P6.3 = 1;
					}
					else
					{
		
						uiToneFreqSate = TONEDISCHARGE;						
					}
					break;
		case TONEDISCHARGE:
					// off time
				    	if(mybuzzerdelay <= SelectedTone-> WaveWidth)
				    	{
					    P6.3 = 0;
				    	}
					else
					{
						 mybuzzerdelay = 0;	
						 buzzercount ++;						
						 buzzerarray++;
						 if((uiSelectedTone == STARTUPTONE)||(uiSelectedTone == PAUSETONE)||(uiSelectedTone == KEYPRESSTONE)||(uiSelectedTone == KEYPRESSTONE2)||(uiSelectedTone == ENCODERTONE))
						 {
						 	if( buzzercount < 1)
						 	uiToneFreqSate = TONEINTIALIZE;
							else
						 	uiToneFreqSate =6;
						 }
						 else if(uiSelectedTone == INAPPLICABLE)
						 {
							if( buzzercount < 2)
						 	uiToneFreqSate = TONEINTIALIZE;
							else
						 	uiToneFreqSate =6;
						 }
						 else if((uiSelectedTone == STARTPRESSTONE)|| (uiSelectedTone == ENDTONE))
						 {
							if( buzzercount < 3)
						 	uiToneFreqSate = TONEINTIALIZE;
							else
						 	uiToneFreqSate =6;
						 }
						 else
						 {
							if( buzzercount < 5)
						 	uiToneFreqSate = TONEINTIALIZE;
							else
						 	uiToneFreqSate =6;
						 }
						
					}
					//result = STEP_INPROGRESS;
					break;
		
		case 6:
					buzzercount = 0;
					buzzerarray = 0;
					mybuzzerdelay = 0;
					SelectedTone = 0 ;
					R_TAU0_Channel3_Stop();
					bBuzzerStart = FALSE; 
					uiToneFreqSate = 0;
					uiSelectedTone = NOTONE;
					//result = STEP_FINISHED;
					break;
					
		default :										
					break;

	}
	return 0;
}

