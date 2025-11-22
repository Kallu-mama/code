#include "STLED316S.h"
#include "r_cg_userdefine.h"
#include "iot_config.h"

extern boolean bDisplay_Flag,chip_select;
extern uint8_t cdigits_data[7];
extern uint8_t BkMultiFuncSetting;
static uint8_t data_address;

//----------------------------------------------------------------------------------------------------------------------
uint8_t key_values[3];

void Delay_ms(uint16_t t)
{
	uint16_t i;
	uint16_t j;
	for(j=t; j>0;j--)
	for(i=3590;i>0;i--); // for 1ms
}
//----------------------------------------------------------------------------------------------------------------------
//Delay in us
void Delay_us(uint16_t t)
{
	uint16_t i;
	uint16_t j;
	for(j=t; j>0;j--)
	{
		for(i=1;i>0;i--) // for 1us		
		{
			NOP(); NOP(); NOP();
			NOP(); NOP(); NOP();
		}
	}
}

void STLED316_Write(unsigned char * Buffer, unsigned char BytesToWrite)
{
	unsigned char BitNum, ByteNum;
	
	chip_select = FALSE;
	STB1_LOW;	//STLED316_STB_PORT &= ~(1 << STLED316_STB_PIN);
	
	NOP();NOP(); NOP();		//_delay_us(1);
	for (ByteNum=0; ByteNum < BytesToWrite; ByteNum++)
	{
		for (BitNum = 0; BitNum < 8; BitNum++ )
		{
				CLK1_LOW;  // 
				
				if ( Buffer[ByteNum] & (1 << BitNum) ){
					DATA1_OUT_HIGH; //
				}
				else {
					DATA1_OUT_LOW;  // 
				}
				CLK1_HIGH; // 
			
			NOP(); NOP();
		}
	}
	NOP();NOP(); NOP(); // _delay_us(1);
	STB1_HIGH; // 
}
//----------------------------------------------------------------------------------------------------------------------


//FOR DISPLAY ONLY
//----------------------------------------------------------------------------------------------------------------------
/* void STLED316_Write1(unsigned char * Buffer, unsigned char BytesToWrite)
{
	unsigned char BitNum, ByteNum;
	STB_LOW;	//STLED316_STB_PORT &= ~(1 << STLED316_STB_PIN);
	NOP();NOP(); NOP();		//_delay_us(1);

	for (ByteNum=0; ByteNum < BytesToWrite; ByteNum++)
	{
		for (BitNum = 0; BitNum < 8; BitNum++ )
		{
			CLK_LOW;  // STLED316_CLK_PORT &= ~(1 << STLED316_CLK_PIN);
			
			if ( Buffer[ByteNum] & (1 << BitNum) ){
				DATA_OUT_HIGH; //STLED316_DIO_PORT |= (1 << STLED316_DIO_PIN); 
			}
			else {
				DATA_OUT_LOW;  // STLED316_DIO_PORT &= ~(1 << STLED316_DIO_PIN);
			}
			
			CLK_HIGH; // STLED316_CLK_PORT |= (1 << STLED316_CLK_PIN);
			NOP(); NOP();
		}
	}
	NOP();NOP(); NOP(); // _delay_us(1);
	STB_HIGH; // STLED316_STB_PORT |= (1 << STLED316_STB_PIN);
}*/
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//read function
void STLED316_Read(unsigned char Command, unsigned char * Buffer, char BytesToRead )
{
	unsigned char BitNum, ByteNum;
	//Send command
	chip_select = FALSE;
	STB1_LOW;

	NOP();NOP(); NOP();
	
		
		
			for (BitNum = 0; BitNum < 8; BitNum++ )
			{
				CLK1_LOW;    //STLED316_CLK_PORT &= ~(1 << STLED316_CLK_PIN);	
				if ( Command & (1 << BitNum) )
				{
					DATA1_OUT_HIGH; //STLED316_DIO_PORT |= (1 << STLED316_DIO_PIN);
				} 
				else 
				{
					DATA1_OUT_LOW;  //STLED316_DIO_PORT &= ~(1 << STLED316_DIO_PIN);
				}
			
				CLK1_HIGH;       //STLED316_CLK_PORT |= (1 << STLED316_CLK_PIN);
				NOP();NOP();
			}
		
	NOP();NOP(); NOP(); // _delay_us(1);
	
		DIO_IN_SET(); //STLED316_DIO_DD &= ~(1 << STLED316_DIO_PIN);
	
	//Read data
	NOP();NOP();
	for (ByteNum=0; ByteNum < BytesToRead; ByteNum++)
	{
		Buffer[ByteNum] = 0;
		key_values[ByteNum] = 0;
			for (BitNum = 0; BitNum < 8; BitNum++ )
			{
				CLK1_LOW;      //STLED316_CLK_PORT &= ~(1 << STLED316_CLK_PIN);
				 // change the data pin afterwords
				if((DATA1 == 1)&&((BitNum!=0)))
				{
					Buffer[ByteNum] |= (1 << BitNum);        // if (STLED316_DIO_PINS & (1 << STLED316_DIO_PIN)) Buffer[ByteNum] |= (1 << BitNum);
					key_values[ByteNum] |= (1 << BitNum);
				}
				NOP();  //_delay_us(1);
				CLK1_HIGH;     //STLED316_CLK_PORT |= (1 << STLED316_CLK_PIN);
				
				NOP();  //_delay_us(1);
			}

	}
	NOP(); NOP(); NOP(); //_delay_us(1);
	
	
		STB1_HIGH; // STLED316_STB_PORT |= (1 << STLED316_STB_PIN);
		DIO_OUT_SET();  //STLED316_DIO_DD |= (1 << STLED316_DIO_PIN);  //  set the pin output
	
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//not used function
uint8_t STLED316_ReadByte(unsigned char Command  )
{
	unsigned char BitNum, ByteNum , data;
	//Send command
	if(chip_select == TRUE)
	{
		STB_LOW;	//STLED316_STB_PORT &= ~(1 << STLED316_STB_PIN);
	}
	else
	{
		STB1_LOW;	//STLED316_STB_PORT &= ~(1 << STLED316_STB_PIN);
	}
	NOP(); NOP();
	for (BitNum = 0; BitNum < 8; BitNum++ )
	{
		if(chip_select == TRUE)
		{
			CLK_LOW;    //STLED316_CLK_PORT &= ~(1 << STLED316_CLK_PIN);
		
			if ( Command & (1 << BitNum) )
			{
				DATA_OUT_HIGH; //STLED316_DIO_PORT |= (1 << STLED316_DIO_PIN);
			} 
			else 
			{
				DATA_OUT_LOW;  //STLED316_DIO_PORT &= ~(1 << STLED316_DIO_PIN);
			}
			CLK_HIGH;      	//STLED316_CLK_PORT |= (1 << STLED316_CLK_PIN);
			NOP();
		}
		else
		{
			CLK1_LOW;    //STLED316_CLK_PORT &= ~(1 << STLED316_CLK_PIN);
		
			if ( Command & (1 << BitNum) )
			{
				DATA1_OUT_HIGH; //STLED316_DIO_PORT |= (1 << STLED316_DIO_PIN);
			} 
			else 
			{
				DATA1_OUT_LOW;  //STLED316_DIO_PORT &= ~(1 << STLED316_DIO_PIN);
			}
			CLK1_HIGH;      	//STLED316_CLK_PORT |= (1 << STLED316_CLK_PIN);
			NOP();
		}
	}
	//Read data
	if(chip_select == TRUE)
	{
		DIO_IN_SET(); 	//STLED316_DIO_DD &= ~(1 << STLED316_DIO_PIN);
	}
	
	
	for (BitNum = 0; BitNum < 8; BitNum++ )
	{
		if(chip_select == TRUE)
		{
			CLK_LOW;      	//STLED316_CLK_PORT &= ~(1 << STLED316_CLK_PIN);
			NOP();	//_delay_us(1);		
			if(DATA == 1) 
			{
				data |= (1 << BitNum);        // if (STLED316_DIO_PINS & (1 << STLED316_DIO_PIN)) Buffer[ByteNum] |= (1 << BitNum);
			}
			
			CLK_HIGH;     	//STLED316_CLK_PORT |= (1 << STLED316_CLK_PIN);
			NOP();  	//_delay_us(1);
		}
		else
		{
			CLK1_LOW;      	//STLED316_CLK_PORT &= ~(1 << STLED316_CLK_PIN);
			NOP();	//_delay_us(1);		
			if(DATA1 == 1) 
			{
				data |= (1 << BitNum);        // if (STLED316_DIO_PINS & (1 << STLED316_DIO_PIN)) Buffer[ByteNum] |= (1 << BitNum);
			}
			
			CLK1_HIGH;     	//STLED316_CLK_PORT |= (1 << STLED316_CLK_PIN);
			NOP();  
		}
	}
	
	NOP();NOP(); //_delay_us(1);
	if(chip_select == TRUE)
	{
		STB_HIGH; // STLED316_STB_PORT |= (1 << STLED316_STB_PIN);
		DIO_OUT_SET();  //STLED316_DIO_DD |= (1 << STLED316_DIO_PIN);  //  set the pin output
	}
	else
	{
		STB1_HIGH; // STLED316_STB_PORT |= (1 << STLED316_STB_PIN);
		  // DIO1_OUT_SET();  //STLED316_DIO_DD |= (1 << STLED316_DIO_PIN);  //  set the pin output
	}
	
	
	return data;
	
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
// STLED316_DisplayOn
// This function turns display on, you MUST call it before STLED316 can display anything.

void STLED316_DisplayOn()
{
	unsigned char Buffer = 0x8A;//0x8F;
	STLED316_Write(&Buffer, 1);
#if ENABLE_IOT
	clr_IOTMultiFuncSetting(MACHINE_ON_OFF);
	BkMultiFuncSetting &= ~MACHINE_ON_OFF;
#endif
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
// STLED316_DisplayOff
// Turns display off.
void STLED316_DisplayOff()
{
	unsigned char Buffer = 0x82; //0x87;
	STLED316_Write(&Buffer, 1);
#if ENABLE_IOT	
		set_IOTMultiFuncSetting(MACHINE_ON_OFF);
		BkMultiFuncSetting |= MACHINE_ON_OFF;
#endif
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
// STLED316_SetLEDStates
// This function turns on/off LEDs depending on LEDs variable bits, bit 0 = 0 turns off D1, bit 0 = 1 turns on D1,
// bit 1 = 0 turns off D2, bit 1 = 1 turns on D2 and so on.
void STLED316_SetLEDStates(unsigned char LEDs)
{
	unsigned char Buffer[2];
	Buffer[0] = 0x8; 
	Buffer[1] = LEDs;
	STLED316_Write(Buffer, 2);
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
// STLED316_SetBrghtCtrlModeAndDigitCnt
// Set brightness control mode
// Mode:
//   - STLED316_BRGHT_MODE_CONST - constant mode, all segments and LEDs will have equal brightness
//     passed in ConstBrightness  parameter (values 0..7)
//   - STLED316_BRGHT_MODE_VAR - brightness can be set individually
// DigitCount: 1..6 - number of enabled displays
void STLED316_SetBrghtCtrlModeAndDigitCnt(unsigned char Mode, unsigned char ConstBrightness, unsigned char DigitCount)
{	
    	unsigned char Buffer[2] ; 
   	Buffer[0] = 0x10;
    	Buffer[1] = (Mode | ((DigitCount > 0) & (DigitCount < 7) ? (DigitCount - 1) : 5) | ((ConstBrightness & 0x7) << 5));
  	STLED316_Write(Buffer, 2);
}
//----------------------------------------------------------------------------------------------------------------------
//new function for re initializations of grids and segments
void STLED316_setDigits_seg_cnt()
{
	unsigned char Buffer = 0x03;//0x8F;
	STLED316_Write(&Buffer, 1);
}

// STLED316_SetLEDBrightness
// LEDBrightness - 8-element array, every element is brightness of single LED, you should pass
// values in range 0..7. Do not expect that 0 turns off LED, it will be just less bright.

void STLED316_SetLEDBrightness(unsigned char * LEDBrightness)
{
	unsigned char Buffer[5], i;
	Buffer[0]=0x18;
	for (i=0; i<4; i++) Buffer[i+1] = (LEDBrightness[2*i] & 0x7) | ((LEDBrightness[2*i+1] & 0x7) << 4);
	STLED316_Write(Buffer, 5);
}

//----------------------------------------------------------------------------------------------------------------------
// STLED316_7Seg_Write
// This functions sets what LED display segments will be on.
// - DisplayNumber - number of display, values 0..5.
// - Segments - which segments will be on, combination of STLED316_xxx_SEG constants.
//   For example STLED316_RT_SEG | STLED316_RB_SEG gives '1' (both right segments on).

void STLED316_7Seg_Write(unsigned char DisplayNumber, unsigned char Segments)
{			
	unsigned char Buffer[2] ;
	Buffer[0] = DisplayNumber; 
	Buffer[1] = Segments;
	STLED316_Write(Buffer, 2);
}

//----------------------------------------------------------------------------------------------------------------------
// STLED316_7Seg_SetBrghtInd
// Set brightness of two of 7-segment displays: 1st and 2nd, 3rd and 4th, 5th and 6th
// - DisplayNum - number of display: for DisplayNum=0 or DisplayNum=1 brightness of both displays 0 and 1 will be set,
//   for DisplayNum=2 or DisplayNum=3 brightness of both displays 2 and 3 will be set and so on.
// - Brightness1, Brightness2 - brightness values (0..7)
//
// To use this function you need to call STLED316_SetBrghtCtrlModeAndDigitCnt(STLED316_BRGHT_MODE_VAR... before!!!

void STLED316_7Seg_SetBrghtInd(unsigned char DisplayNum, unsigned char Brightness1, unsigned char Brightness2)
{
	unsigned char Buffer[2];
	Buffer[0]=0x10 | (((DisplayNum >> 1) & 0x3) + 1);
	Buffer[1]=(Brightness1 & 0x7) | ((Brightness2 & 0x7) <<4);
	STLED316_Write(Buffer, 2);
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
// STLED316_ReadLEDsAndButtons
// Reads LED states (into LEDs) and buttons states (buttons S1..S8 into Bunnons1 variable, buttons S9..S16 into Bunnons2 variable)
void STLED316_ReadLEDsAndButtons(unsigned char * LEDs, unsigned char * Buttons1, unsigned char * Buttons2)
{
	unsigned char Buffer[3];
	STLED316_Read(0x48, Buffer, 3);
	*LEDs = Buffer[0];
	*Buttons1 = Buffer[1];
	*Buttons2 = Buffer[2];
	
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
// STLED316_SetLEDBrightness
// LEDBrightness - 8-element array, every element is brightness of single LED, you should pass
// values in range 0..7. Do not expect that 0 turns off LED, it will be just less bright.
void STLED316_ReadLEDBrightness(unsigned char * LEDBrightness)
{
	int i;
	STLED316_Read(0x58, LEDBrightness, 4);

	for (i=3; i>=0; i--) 
	{
		LEDBrightness[2*i+1] = (LEDBrightness[i] >> 4) & 0x7;
		LEDBrightness[2*i] = LEDBrightness[i] & 0x7;	
	}
}
//----------------------------------------------------------------------------------------------------------------------
// this function must be update after every 1min / 500msec 
void SevenSegPrint(uint16_t num)
{
   
   /*   
   This function breaks apart a given integer into separete digits
   and writes them to the display array i.e. digits[]   
   */
   uint8_t i=0;
   uint8_t j;
   uint8_t digits[4];
   uint16_t hours;
   uint16_t mins;
   
   if(num>9999) return;
   
   /*
   hours = num / 60;
   mins = num % 60 ;
   num = hours * 100;
   num = num + mins;
   
   */
   
   
   {
	    for(j=0;j<4;j++)
	   {
	      cdigits_data[j] = cdigits_data[j] & 0x80; // reset to zero for display digits without affecting the other led stastus
	   }
	   
	   while(num)
	   {
	      digits[i]=num%10;
	      cdigits_data[3-i] = cdigits_data[3-i] | fill_data_array(digits[i]); 
	      i++;
	      num=num/10;
	   }
	   
	   for(i=i;i<4;i++)
	   {
		cdigits_data[3-i] = cdigits_data[3-i] | fill_data_array(0)  ; 
		 
	   }
	   for(i=0;i<4;i++) digits[i]=0;
	   
	
   
   }
}
#if ENABLE_IOT
void MacAddressDisplay(uint16_t num)
{
   
   /*   
   This function breaks apart a given integer into separete digits
   and writes them to the display array i.e. digits[]   
   */
   uint8_t i=0;
   uint8_t j;
   uint8_t digits[4];
   
   if(num>65535) return;
   

   {
	    for(j=0;j<4;j++)
	   {
	      cdigits_data[j] = cdigits_data[j] & 0x80; // reset to zero for display digits without affecting the other led stastus
	   }
	   
	   while(num)
	   {
	      digits[i]=num%0x10;
	      cdigits_data[3-i] = cdigits_data[3-i] | fill_data_array(digits[i]); 
	      //cdigits_data[0] = cdigits_data[0] & 0x00;
	      i++;
	      num=num/0x10;
	   }
	   
	   for(i=i;i<4;i++)
	   {
		cdigits_data[3-i] = cdigits_data[3-i] | fill_data_array(0); 
		 
	   }
	   for(i=0;i<4;i++) digits[i]=0;
   }
  
}
#endif
void DisplayTime(uint16_t num)
{
   /*   
   This function breaks apart a given integer into separete digits
   and writes them to the display array i.e. digits[]   
   */
   
   uint8_t i=0;
   uint8_t j;
   uint8_t digits[4];
   uint16_t hours;
   uint16_t mins;
   
   if(num>9999) return;
   
   hours = num / 60;
   mins = num % 60 ;
   num = hours * 100;
   num = num + mins;
   
  
	    for(j=0;j<4;j++)
	   {
	      cdigits_data[j] = cdigits_data[j] & 0x80; // reset to zero for display digits without affecting the other led stastus
	   }
	   
	   while(num)
	   {
	      digits[i]=num%10;
	      cdigits_data[3-i] = cdigits_data[3-i] | fill_data_array(digits[i]);
	      i++;
	      num=num/10;
	   }
	   
	   for(i=i;i<4;i++)
	   {
		cdigits_data[3-i] = cdigits_data[3-i] | fill_data_array(0)  ; 
		if(cdigits_data[0]==0x3f)
		{
			cdigits_data[0] = 0x00;
		}
		 
	   }
	   for(i=0;i<4;i++) digits[i]=0;
	   
	
   
   
}

void DIO_OUT_SET(void)
{
     PM7 &= (~ _10_PMn4_MODE_INPUT);
     PM7 |= _00_PMn4_MODE_OUTPUT;
     PM7 |= 0x20;
}
 //----------------------------------------------------------------------------------------------------------------------
 void DIO_IN_SET(void)
{
     PM7 &= (~ _00_PMn4_MODE_OUTPUT);
     PM7 |= _10_PMn4_MODE_INPUT; 
     PM7 |= 0x20;
}
//----------------------------------------------------------------------------------------------------------------------

uint8_t fill_data_array(char data_value)
{
	uint8_t ret;
	switch(data_value)
	{
		case 1:
		case '1':
			ret = 0x06;
			break;
		case 2:
		case '2':
			ret = 0x5B;
			break;
		case 3:
		case '3':
			ret = 0x4F;
			break;
		case 4:
		case '4':
			ret = 0x66;
			break;
		case 'S':
		case 5:
		case '5':
			ret = 0x6D;
			break;
		case 6:
		case '6':
			ret = 0x7D;
			break;
		case 7:
		case '7':
			ret = 0x07;
			break;
		case 8:
		case '8':
			ret = 0x7F;
			break;
		case 9:
		case 'g':
		case '9':
			ret = 0x6F;
			break;
		case 0:
		case '0':
		case 'O':
			ret = 0x3F;
			break;
		case 'A':
		case 0x0a:
			ret = 0x77;
			break;
		
		case 'C':
		case 0x0c:
			ret = 0x39;
			break;
		
		case 'E':
		case 0x0e:
			ret = 0x79;
			break;
		
		case 'F':
		case 0x0f:
			ret = 0x71;
			break;
		case 'H':
			ret = 0x76;
			break;
		
		case 'L':
			ret = 0x38;
			break;
			
		case 'P':
			ret = 0x73;
			break;
		
		case 'U':
			ret = 0x3E;
			break;
		
		case 'b':
		case 0x0b:
			ret = 0x7C;
			break;
		
		case 'c':
			ret = 0x58;
			break;
		case 'd':
		case 0x0d:
			ret = 0x5E;
			break;
		
		case 'i':
			ret = 0x04;
			break;
		
		case 'n':
			ret = 0x54;
			break;
		case 'o':
			ret = 0x5C;
			break;
		
		case 't':
			ret = 0x78;
			break;
		case 'l':
			ret = 0x06;
			break;
		case 'u':
			ret = 0x1C;
			break;
		
		case 'r':
			ret = 0x50;
			break;
		
		case '-':
			ret = 0x40;
			break;
		case '=':
			ret = 0x48;
			break;
		case '#':
			ret = 0x4E;
			break;
		case 'h' :
			ret = 0x49;
			break;
		case 'v' :
			ret = 0x36;
			break;
		case 'y' :
			ret = 0x6E;
			break;
		case 'z' :
			ret = 0x74;
			break;	
		case 'a' :
			ret = 0x01;
			break;
		case '|' :
			ret = 0x06;
			break;
		case '_' :
			ret = 0x08;
			break;
		case 'e' :
			ret = 0x30;
			break;
		case '+' :
			ret = 0x03;
			break;
		case 'x' :
			ret = 0x0C;
			break;
		case 'q' :
			ret = 0x18;
			break;
		case 'm' :
			ret = 0x21;
			break;
		case 'R':
			ret = 0x10;
			break;
		default : 
			break;
	}
	
	return ret; 
			
	
}



void update_display_data(uint8_t *digits)
{
   /*   
   This function breaks apart a given integer into separete digits
   and writes them to the display array i.e. digits[]   
   */
   
  uint8_t j =0;
  //uint8_t prev_data_arry[6];
  
  
  //if(strcmp(&prev_data_arry,&digits,4)!=0)
  {
	   for(j=0;j<4;j++)
	   {
	      cdigits_data[j] = cdigits_data[j] & 0x80; // reset to zero for display digits without affecting the other led stastus
	   }
	   for(j=0;j<4;j++)
	   {
	      cdigits_data[j] = cdigits_data[j] | fill_data_array(digits[j]); // fill_data_array(unsigned char data_value)
	   }
	   //strcpy(&prev_data_arry,&digits);
  }
   //for(j=i;j<4;j++) digits[j]=0;
   
}
char *mystrcpy(char *dest, const char *src)
{
   char *temp = dest;
   while(*dest++ = *src++);
   return temp;
}

//---------6964 initializarion function------------
//Initialize 6964 ptc 
// This ptc initialization is called in main.c only once
void Initialise_6964(void)
{
	Delay_ms(200);
	DIO_OUT_SET();
	
	//INITIALIZATION IS DONE AS PER DATASHEET
	 
	STB1_LOW;
	NOP(); //delay of 1us
	writeByteData(0x80);	//COMMAND 4 display off 
	NOP();	
	STB1_HIGH;
	NOP();
	
	
	STB1_LOW;
	NOP();
	writeByteData(0x40); // COMMAND 2 write auto inc add
	NOP();
	STB1_HIGH;
	NOP();
	
	
	STB1_LOW;
	NOP();
	writeByteData(0xC0); // COMMAND 3 set address
	NOP();
	for( data_address = 0x00; data_address <=0x0d ; data_address++)
	{
		writeByteData(0x00); // set data
		NOP();
	}
	
	STB1_HIGH;
	NOP();
	
	STB1_LOW;
	NOP();
	writeByteData(0x03); //COMMAND 1 : seg and grid matrix : 7 grids 10 seg
	NOP();
	STB1_HIGH;
	NOP();
	
	
	STB1_LOW;
	NOP();
	//writeByteData(0x8F);	//COMMAND 4 : disply on at max pulse width
	writeByteData(0x8A);
	NOP();
	STB1_HIGH;
	NOP();
	
}
/*
--------------------------------------------------------------------------------
	Function Name : writeByteData
	Arguments     : uint8_t
	Return Type   : void
	Description   : Write segment data byte
--------------------------------------------------------------------------------
*/
static void writeByteData(uint8_t data)
{
	uint8_t i;
	for(i = 0; i < 8 ;i ++)
	{
		//Min. time for each instruction = 1/32M = 31.25ns
		CLK1_LOW;
		if(data & (0x1 << i))		//31.25ns
		{
			DATA1_OUT_HIGH;		//31.25ns
		}
		else
		{
			DATA1_OUT_LOW;		//31.25ns
		}
		NOP();
		CLK1_HIGH;
		NOP();
	}
}
