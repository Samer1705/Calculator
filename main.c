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

float64 g_numsAndSigns[16] = {0}, result;
uint8 g_chars[16] = {0}, g_count = 1;

void clearAll()
{
	g_numsAndSigns[16] = 0;
	g_chars[16] = 0;
	LCD_clearScreen();
	LCD_displayString("CALCULATOR:");
	LCD_moveCursor(1, 0);
}

int main(void)
{
	uint8 key;
	LCD_init();
	LCD_displayString("CALCULATOR:");
	LCD_moveCursor(1, 0);
	while(1)
	{
		key = KEYPAD_getPressedKey();
		switch(key)
		{
		case 13:
			clearAll();
			break;
		case '=':
			break;
		default:
			if(key >= 0 && key <= 9) LCD_intgerToString(key);
			else LCD_displayCharacter(key);
			break;
		}
		_delay_ms(200);
	}
}
