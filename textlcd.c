#include "textlcd.h"

/*------------ Define Function Prototype -------------*/
/* Initialize Text Lcd work in mode
 * 4 bit using RS,EN,D4,D5,D6,D7 Pin
 * Pin RW is connected to GND
 */
void textlcd_init(TextLcd_4Bit_HandleTypeDef * Lcd_HandleTypeDef,
		TextLcd_Dimension _TextLcd_Dimension,
		GPIO_TypeDef*				RS_GPIOx,
		GPIO_Pin_TypeDef			RS_PINx,
		GPIO_TypeDef*				EN_GPIOx,
		GPIO_Pin_TypeDef			EN_PINx,
		GPIO_TypeDef*				D4_GPIOx,
		GPIO_Pin_TypeDef			D4_PINx,
		GPIO_TypeDef*				D5_GPIOx,
		GPIO_Pin_TypeDef			D5_PINx,
		GPIO_TypeDef*				D6_GPIOx,
		GPIO_Pin_TypeDef			D6_PINx,
		GPIO_TypeDef*				D7_GPIOx,
		GPIO_Pin_TypeDef			D7_PINx)
		{
	/* Store Information in Pointer Lcd_HandleTypeDef */
	Lcd_HandleTypeDef->_TextLcd_Dimension=_TextLcd_Dimension;

	Lcd_HandleTypeDef->RS_GPIOx=RS_GPIOx;
	Lcd_HandleTypeDef->RS_PINx=RS_PINx;

	Lcd_HandleTypeDef->EN_GPIOx=EN_GPIOx;
	Lcd_HandleTypeDef->EN_PINx=EN_PINx;

	Lcd_HandleTypeDef->D4_GPIOx=D4_GPIOx;
	Lcd_HandleTypeDef->D4_PINx=D4_PINx;

	Lcd_HandleTypeDef->D5_GPIOx=D5_GPIOx;
	Lcd_HandleTypeDef->D5_PINx=D5_PINx;

	Lcd_HandleTypeDef->D6_GPIOx=D6_GPIOx;
	Lcd_HandleTypeDef->D6_PINx=D6_PINx;

	Lcd_HandleTypeDef->D7_GPIOx=D7_GPIOx;
	Lcd_HandleTypeDef->D7_PINx=D7_PINx;


	/* Initialize all pin as output */
	GPIO_Init(Lcd_HandleTypeDef->RS_GPIOx,Lcd_HandleTypeDef->RS_PINx,GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(Lcd_HandleTypeDef->EN_GPIOx,Lcd_HandleTypeDef->EN_PINx,GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(Lcd_HandleTypeDef->D4_GPIOx,Lcd_HandleTypeDef->D4_PINx,GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(Lcd_HandleTypeDef->D5_GPIOx,Lcd_HandleTypeDef->D5_PINx,GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(Lcd_HandleTypeDef->D6_GPIOx,Lcd_HandleTypeDef->D6_PINx,GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(Lcd_HandleTypeDef->D7_GPIOx,Lcd_HandleTypeDef->D7_PINx,GPIO_MODE_OUT_PP_HIGH_FAST);

	Delay_ms(40);

	PIN_LOW(Lcd_HandleTypeDef->D4_GPIOx,Lcd_HandleTypeDef->D4_PINx);
	PIN_HIGH(Lcd_HandleTypeDef->D5_GPIOx,Lcd_HandleTypeDef->D5_PINx);
	PIN_LOW(Lcd_HandleTypeDef->D6_GPIOx,Lcd_HandleTypeDef->D6_PINx);
	PIN_LOW(Lcd_HandleTypeDef->D7_GPIOx,Lcd_HandleTypeDef->D7_PINx);
	PIN_LOW(Lcd_HandleTypeDef->RS_GPIOx,Lcd_HandleTypeDef->RS_PINx);

	PIN_HIGH(Lcd_HandleTypeDef->EN_GPIOx,Lcd_HandleTypeDef->EN_PINx);
	PIN_LOW(Lcd_HandleTypeDef->EN_GPIOx,Lcd_HandleTypeDef->EN_PINx);

	textlcd_write(Lcd_HandleTypeDef,0,0x28);
	textlcd_write(Lcd_HandleTypeDef,0,0x0c);
	textlcd_write(Lcd_HandleTypeDef,0,0x06);
	textlcd_write(Lcd_HandleTypeDef,0,0x01);
		}

/* Write Character to Text Lcd
 * at specified position
 */
void textlcd_write(TextLcd_4Bit_HandleTypeDef * Lcd_HandleTypeDef,
		uint8_t type, uint8_t data)
{
	Delay_ms(2);
	if(type)
	{
		PIN_HIGH(Lcd_HandleTypeDef->RS_GPIOx,Lcd_HandleTypeDef->RS_PINx);
	}else
	{
		PIN_LOW(Lcd_HandleTypeDef->RS_GPIOx,Lcd_HandleTypeDef->RS_PINx);
	}

	//Send High Nibble
	if(data&0x80)
	{
		PIN_HIGH(Lcd_HandleTypeDef->D7_GPIOx,Lcd_HandleTypeDef->D7_PINx);
	}else
	{
		PIN_LOW(Lcd_HandleTypeDef->D7_GPIOx,Lcd_HandleTypeDef->D7_PINx);
	}

	if(data&0x40)
	{
		PIN_HIGH(Lcd_HandleTypeDef->D6_GPIOx,Lcd_HandleTypeDef->D6_PINx);
	}else
	{
		PIN_LOW(Lcd_HandleTypeDef->D6_GPIOx,Lcd_HandleTypeDef->D6_PINx);
	}

	if(data&0x20)
	{
		PIN_HIGH(Lcd_HandleTypeDef->D5_GPIOx,Lcd_HandleTypeDef->D5_PINx);
	}else
	{
		PIN_LOW(Lcd_HandleTypeDef->D5_GPIOx,Lcd_HandleTypeDef->D5_PINx);
	}

	if(data&0x10)
	{
		PIN_HIGH(Lcd_HandleTypeDef->D4_GPIOx,Lcd_HandleTypeDef->D4_PINx);
	}else
	{
		PIN_LOW(Lcd_HandleTypeDef->D4_GPIOx,Lcd_HandleTypeDef->D4_PINx);
	}
	PIN_HIGH(Lcd_HandleTypeDef->EN_GPIOx,Lcd_HandleTypeDef->EN_PINx);
	PIN_LOW(Lcd_HandleTypeDef->EN_GPIOx,Lcd_HandleTypeDef->EN_PINx);


	//Send Low Nibble
	if(data&0x08)
	{
		PIN_HIGH(Lcd_HandleTypeDef->D7_GPIOx,Lcd_HandleTypeDef->D7_PINx);
	}else
	{
		PIN_LOW(Lcd_HandleTypeDef->D7_GPIOx,Lcd_HandleTypeDef->D7_PINx);
	}

	if(data&0x04)
	{
		PIN_HIGH(Lcd_HandleTypeDef->D6_GPIOx,Lcd_HandleTypeDef->D6_PINx);
	}else
	{
		PIN_LOW(Lcd_HandleTypeDef->D6_GPIOx,Lcd_HandleTypeDef->D6_PINx);
	}

	if(data&0x02)
	{
		PIN_HIGH(Lcd_HandleTypeDef->D5_GPIOx,Lcd_HandleTypeDef->D5_PINx);
	}else
	{
		PIN_LOW(Lcd_HandleTypeDef->D5_GPIOx,Lcd_HandleTypeDef->D5_PINx);
	}

	if(data&0x01)
	{
		PIN_HIGH(Lcd_HandleTypeDef->D4_GPIOx,Lcd_HandleTypeDef->D4_PINx);
	}else
	{
		PIN_LOW(Lcd_HandleTypeDef->D4_GPIOx,Lcd_HandleTypeDef->D4_PINx);
	}
	PIN_HIGH(Lcd_HandleTypeDef->EN_GPIOx,Lcd_HandleTypeDef->EN_PINx);
	PIN_LOW(Lcd_HandleTypeDef->EN_GPIOx,Lcd_HandleTypeDef->EN_PINx);
}



/* Write a string to text lcd
 * at specified position
 */
void textlcd_puts(TextLcd_4Bit_HandleTypeDef * Lcd_HandleTypeDef,
		uint8_t row, uint8_t column, int8_t *string)
{
	uint8_t i=0;
	//Set Cursor Position
	switch (Lcd_HandleTypeDef->_TextLcd_Dimension)
	{
	case TextLcd_Dimension16xN: //For LCD16x2 or LCD16x4
		switch(row)
		{
		case 0: //Row 0
			textlcd_write(Lcd_HandleTypeDef,0,0x80+0x00+column);
			break;
		case 1: //Row 1
			textlcd_write(Lcd_HandleTypeDef,0,0x80+0x40+column);
			break;
		case 2: //Row 2
			textlcd_write(Lcd_HandleTypeDef,0,0x80+0x10+column);
			break;
		case 3: //Row 3
			textlcd_write(Lcd_HandleTypeDef,0,0x80+0x50+column);
			break;
		}
		break;
		case TextLcd_Dimension20xN: //For LCD20x2 or LCD20x4
			switch(row)
			{
			case 0: //Row 0
				textlcd_write(Lcd_HandleTypeDef,0,0x80+0x00+column);
				break;
			case 1: //Row 1
				textlcd_write(Lcd_HandleTypeDef,0,0x80+0x40+column);
				break;
			case 2: //Row 2
				textlcd_write(Lcd_HandleTypeDef,0,0x80+0x14+column);
				break;
			case 3: //Row 3
				textlcd_write(Lcd_HandleTypeDef,0,0x80+0x54+column);
				break;
			}
			break;
	}

	while(*(string+i))
	{
		textlcd_write(Lcd_HandleTypeDef,1,*(string+i));
		i++;
	}
}

/* Clear screen text lcd */
void textlcd_clear(TextLcd_4Bit_HandleTypeDef * Lcd_HandleTypeDef)
{
	textlcd_write(Lcd_HandleTypeDef,0,0x01);
}
