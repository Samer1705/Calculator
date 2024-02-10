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

float64 result;								/* The Result of The Equation */
uint8 g_array[16] = {0}, g_arrayIndex = 0;	/* Array for All Characters Entered */
boolean g_newCalcFlag = FALSE;				/* Flag That Sets After Every Calculation */

/*******************************************************************************
 *                                Function Definitions                         *
 *******************************************************************************/


void displayArray(float64 arr[], uint8 size)
{
	uint8 i;
	LCD_displayStringRowColumn(2, 0, "                ");
	LCD_moveCursor(2, 0);
	for(i = 0; i < size; i++)
	{
		LCD_intgerToString(arr[i]);
		LCD_displayCharacter(',');
	}
	_delay_ms(1500);
}

/* Concatenates Characters To Become a Full Number */
float64 numConcat(uint8 array[], uint8 size)
{
	float64 num = 0;
	uint8 i;
	for (i = 0; i < size; i++)
	{
		num *= 10;
		num += array[i];
	}
	return num;
}

float64 operation(float64 num1, float64 num2, uint8 sign)
{
	switch(sign)
	{
	case '+':
		return num1 + num2;
		break;
	case '-':
		return num1 - num2;
		break;
	case '*':
		return num1 * num2;
		break;
	case '%':
		return num1 / num2;
	default:
		break;
	}
	return 0;
}

/* Function That Clears The Array & Screen */
void clearAll()
{
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
	float64 numsAndSigns[16] = {0}; 			/* Array for All Numbers & Signs in Equation */
	uint8 i, numsAndSignsIndex = 0, num[16] = {0}, numIndex = 0;
	for(i = 0; i < g_arrayIndex; i++)
	{
		if(g_array[i] >= 0 && g_array[i] <= 9)
		{
			num[numIndex++] = g_array[i];
			if(i == g_arrayIndex - 1)
				numsAndSigns[numsAndSignsIndex++] = numConcat(num, numIndex);
		}
		else
		{
			numsAndSigns[numsAndSignsIndex++] = numConcat(num, numIndex);
			numsAndSigns[numsAndSignsIndex++] = g_array[i];
			numIndex = 0;
		}
	}
	displayArray(numsAndSigns, numsAndSignsIndex);
	for(i = 0; i < numsAndSignsIndex; i++)
	{
		if(numsAndSigns[i] == '*' || numsAndSigns[i] == '%')
		{
			numsAndSigns[i-1] = operation(numsAndSigns[i-1], numsAndSigns[i+1], numsAndSigns[i]);
			uint8 j;
			for(j = i; j < numsAndSignsIndex; j++)
			{
				numsAndSigns[j] = numsAndSigns[j+2];
			}
			numsAndSignsIndex-=2;
			i = 0;
		}
	}
	displayArray(numsAndSigns, numsAndSignsIndex);
	for(i = 0; i < numsAndSignsIndex; i++)
	{
		if(numsAndSigns[i] == '+' || numsAndSigns[i] == '-')
		{
			numsAndSigns[i-1] = operation(numsAndSigns[i-1], numsAndSigns[i+1], numsAndSigns[i]);
			uint8 j;
			for(j = i; j < numsAndSignsIndex; j++)
			{
				numsAndSigns[j] = numsAndSigns[j+2];
			}
			numsAndSignsIndex-=2;
			i = 0;
		}
	}
	displayArray(numsAndSigns, numsAndSignsIndex);
	result = numsAndSigns[0];
	LCD_displayStringRowColumn(3, 0, "=");
	LCD_intgerToString(result);
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
