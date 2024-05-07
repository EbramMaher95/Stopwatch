/*
 * HD44780_LCD.h
 *
 *  Created on: Feb 3, 2024
 *      Author: ramys
 */

#ifndef HD44780_LCD_HD44780_LCD_H_
#define HD44780_LCD_HD44780_LCD_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "stm32f1xx.h"
#include "../../ARM_Drivers/Delay_Micros/dwt_stm32_delay.h"

#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

typedef struct ALCD
{
	uint8_t Lines;
	uint8_t Chars;

	//hardware interface variables
	GPIO_TypeDef *Data_GPIO;
	uint8_t Data_GPIO_Start_Pin;

	GPIO_TypeDef *EN_GPIO;
	uint16_t EN_GPIO_Pin;

	GPIO_TypeDef *RS_GPIO;
	uint16_t RS_GPIO_Pin;

	// hold the row offsets based on the line length
	uint8_t RowOffsets[4];
	uint8_t _displaycontrol;
} Alcd_t;

/* User Functions  */

void Alcd_Init(Alcd_t *lcd, uint8_t Lines, uint8_t Chars);
// deh el user by7ot feha hardware ya3ni el lcd wasla 3la eh
void Alcd_Put_n(Alcd_t *lcd, char *text, uint8_t len);
//
void Alcd_PutAt_n(Alcd_t *lcd, uint8_t Row, uint8_t Col, char *text, uint8_t len);
//where to start from
void Alcd_Clear(Alcd_t *lcd);
//
void Alcd_Display_Control(Alcd_t *lcd, uint8_t ON_OFF, uint8_t CUR_ON_OFF, uint8_t BLINK_ON_OFF);
//on_off 3lshan text ytl3 wla mytl3sh nfso hwa hwa ll cursor w ll blink
void Alcd_CursorAt(Alcd_t *lcd, uint8_t Row, uint8_t Col);
//location of the cursor
void Alcd_CreateChar(Alcd_t *lcd, uint8_t Location, uint8_t Map[]);
//
void Alcd_PutChar(Alcd_t *lcd, char chr);
//display one characher
int Str_Len(char *string);
// 3lshan a3rf tool el lenght w 3lshan msh a3tmd 3la function gwa el comipler
#define Alcd_Put(lcd, text) Alcd_PutAt_n(lcd, Row, Col, text, Str_Len(text))
//3lshan user y7ot 2 argumnet bdal 3 arg
#define Alcd_PutAt(lcd, Row, Col, text) Alcd_PutAt_n(lcd, Row, Col, text, Str_Len(text))

#endif /* HD44780_LCD_HD44780_LCD_H_ */
