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

/*******************************************************************************
 *                                Global Variables                             *
 *******************************************************************************/

uint64 g_numsAndSigns[16] = {0}; 			/* Array for All Numbers & Signs in Equation */
float64 result;								/* The Result of The Equation */
uint8 g_array[16] = {0}, g_arrayIndex = 0;	/* Array for All Characters Entered */
boolean g_newCalcFlag = FALSE;				/* Flag That Sets After Every Calculation */

/*******************************************************************************
 *                                Function Definitions                         *
 *******************************************************************************/

/* Concatenates Characters To Become a Full Number */
float64 numConcat(int array[], int size)
{
	int num = 0;
	for (int i = 0; i < size; i++)
	{
		num *= 10;
		num += array[i];
	}
	return num;
}

/* Function That Clears The Array & Screen */
void clearAll()
{
	g_numsAndSigns[16] = 0;
	g_array[16] = 0;
	g_arrayIndex = 0;
	g_newCalcFlag = FALSE;
	LCD_clearScreen();
	LCD_displayString("CALCULATOR:");
	LCD_moveCursor(1, 0);
}

/* Displays Character on LCD Screen & Pushes It in The Array of Characters */
void input(uint8 key)
{
	if(g_arrayIndex < 16)
	{
		if(key >= 0 && key <= 9) LCD_intgerToString(key);
		else LCD_displayCharacter(key);
		g_array[g_arrayIndex++] = key;
	}
}

/*  */
void calculate()
{
	uint8 i, numsAndSignsIndex = 0, num[16] = {0}, numIndex = 0;
	for(i = 0; i <= g_arrayIndex; i++)
	{
		if(g_array[i] == '+' || g_array[i] == '-' || g_array[i] == '*' || g_array[i] == '%')
		{
			g_numsAndSigns[numsAndSignsIndex] = g_array[i];
			if(numIndex != 0)
				g_numsAndSigns[numsAndSignsIndex - 1] = numConcat(num, numIndex);
			numsAndSignsIndex++;
		}
		else
		{
			num[numIndex++] = g_array[i];
		}
	}
	if(numsAndSignsIndex == 1)
		g_numsAndSigns[numsAndSignsIndex] = numConcat(num, numIndex);
	LCD_displayStringRowColumn(3, 0, "=");
	LCD_intgerToString(g_numsAndSigns[0]);
	g_newCalcFlag = TRUE;
}

/*******************************************************************************
 *                                Main Function                                *
 *******************************************************************************/

int main(void)
{
	uint8 key;
	LCD_init();
	LCD_displayString("CALCULATOR:");
	LCD_moveCursor(1, 0);
	while(1)
	{
		key = KEYPAD_getPressedKey();	/* Reads Pressed Key */
		if(g_newCalcFlag) clearAll();	/* If Its New Calculation Clears The Previous */
		/* Checks The Pressed Key */
		switch(key)
		{
		/* If Its "Enter", Clear Everything */
		case 13:
			clearAll();
			break;
		/* If Its "=", Calculate The Written Equation */
		case '=':
			calculate();
			break;
		/* If Its a Number or a Sign, Call Input Function */
		default:
			input(key);
			break;
		}
		_delay_ms(200);					/* Delay Between Key Presses */
	}
}
