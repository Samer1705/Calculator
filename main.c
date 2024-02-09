/*
 ============================================================================
 Name        : main.c
 Author      : Samer Sameh Lawindy
 ============================================================================
 */
#include "std_types.h"
#include "lcd.h"
#include "keypad.h"
#include "avr/delay.h"

uint8 g_chars[16] = {0}, count = 1;


int main(void)
{
	uint8 key;
	LCD_init();
	LCD_displayString("CALCULATOR:");
	LCD_moveCursor(1, 1);
	while(1)
	{
		key = KEYPAD_getPressedKey();
		if(key >= 0 && key <= 9) LCD_intgerToString(key);
		else LCD_displayCharacter(key);
		_delay_ms(200);
	}
}
