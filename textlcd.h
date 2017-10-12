#ifndef __TEXT_LCD_H
#define __TEXT_LCD_H

#include "stm8s.h"
#include "mystm8s.h"


/*----------- Define Private Struct and enum ------------*/

typedef enum
{
	TextLcd_Dimension16xN=1, /* For text lcd 16x2 or 16x4 */
	TextLcd_Dimension20xN=2  /* For text lcd 20x2 or 20x4 */

}TextLcd_Dimension;


typedef struct
{
	TextLcd_Dimension _TextLcd_Dimension;

	GPIO_TypeDef*				RS_GPIOx;
	GPIO_Pin_TypeDef			RS_PINx;

	GPIO_TypeDef*				EN_GPIOx;
	GPIO_Pin_TypeDef			EN_PINx;

	GPIO_TypeDef*				D4_GPIOx;
	GPIO_Pin_TypeDef			D4_PINx;

	GPIO_TypeDef*				D5_GPIOx;
	GPIO_Pin_TypeDef			D5_PINx;

	GPIO_TypeDef*				D6_GPIOx;
	GPIO_Pin_TypeDef			D6_PINx;

	GPIO_TypeDef*				D7_GPIOx;
	GPIO_Pin_TypeDef			D7_PINx;
}TextLcd_4Bit_HandleTypeDef;


/*------------ Define Private Macro -----------------*/

#define PIN_LOW(PORT,PIN) GPIO_WriteLow(PORT,PIN);
#define PIN_HIGH(PORT,PIN) GPIO_WriteHigh(PORT,PIN);




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
		GPIO_Pin_TypeDef			D7_PINx);



void textlcd_write(TextLcd_4Bit_HandleTypeDef * Lcd_HandleTypeDef,
		uint8_t type, uint8_t data);


void textlcd_puts(TextLcd_4Bit_HandleTypeDef * Lcd_HandleTypeDef,
		uint8_t row, uint8_t column, int8_t *string);


void textlcd_clear(TextLcd_4Bit_HandleTypeDef * Lcd_HandleTypeDef);

#endif

