/*
 ============================================================================
 Name        : main.c
 Author      : Samer Sameh Lawindy
 ============================================================================
 */
#include "std_types.h"
#include "lcd.h"
#include "keypad.h"
#include "util/delay.h"

/*******************************************************************************
 *                                Global Variables                             *
 *******************************************************************************/

uint8 	g_array[16] = {0},		/* Array for All Characters Entered */
		g_arrayCount = 0;		/* Number of Characters Entered */
boolean g_newCalcFlag = FALSE;	/* Flag That Sets After Every Calculation */

/*******************************************************************************
 *                                Function Definitions                         *
 *******************************************************************************/

/* Function Used For Shifting Array (Still in Progress) */
void shiftArray(float64 *arr[], uint8 currentIndex, uint8 *size, uint8 shift)
{
	uint8 j;
	for(j = currentIndex; j < *size; j++)
	{
		(*arr)[j] = (*arr)[j + shift];
	}
	*size -= shift;
}

/* Function That Displays Array (Used For Testing) */
void displayArray(float64 arr[], uint8 size)
{
	uint8 i;
	LCD_displayStringRowColumn(2, 0, "                ");
	LCD_moveCursor(2, 0);
	for(i = 0; i < size; i++)
	{
		LCD_floatToString(arr[i]);
		LCD_displayCharacter(',');
	}
	_delay_ms(500);
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

/* Calculates The Operation Given & Returns The Result */
float64 operation(float64 num1, float64 num2, uint8 sign)
{
	switch(sign)
	{
	case '+':
		return num1 + num2;
	case '-':
		return num1 - num2;
	case '*':
		return num1 * num2;
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
	g_arrayCount = 0;
	g_newCalcFlag = FALSE;
	LCD_clearScreen();
	LCD_displayString("CALCULATOR:");
	LCD_moveCursor(1, 0);
}

/* Function that Displays Character on LCD Screen & Pushes It in The Array of Characters */
void input(uint8 key)
{
	if(g_arrayCount < 16)
	{
		if(key >= 0 && key <= 9) LCD_intgerToString(key);
		else LCD_displayCharacter(key);
		g_array[g_arrayCount++] = key;
	}
}

/* Function that Does All the Calculation Process */
void calculate()
{
	float64 numsAndSigns[16] = {0},	/* Array for All Numbers & Signs in Equation */
			result; 				/* The Result of The Equation */
	uint8 	numsAndSignsCount = 0,	/* Count for the Numbers & Signs */
			num[16] = {0},			/* Array for Numbers to be Concatenated  */
			numCount = 0,			/* Count for Numbers to be Concatenated */
			i;						/* Index Used in For Loops */

	for(i = 0; i < g_arrayCount; i++)
	{
		if(g_array[i] >= 0 && g_array[i] <= 9)
		{
			num[numCount++] = g_array[i];
			if(i == g_arrayCount - 1)
				numsAndSigns[numsAndSignsCount++] = numConcat(num, numCount);
		}
		else
		{
			numsAndSigns[numsAndSignsCount++] = numConcat(num, numCount);
			numsAndSigns[numsAndSignsCount++] = g_array[i];
			numCount = 0;
		}
	}
	/* displayArray(numsAndSigns, numsAndSignsCount); */
	for(i = 0; i < numsAndSignsCount; i++)
	{
		if(numsAndSigns[i] == '*' || numsAndSigns[i] == '%')
		{
			numsAndSigns[i-1] = operation(numsAndSigns[i-1], numsAndSigns[i+1], numsAndSigns[i]);
			/* shiftArray(&numsAndSigns, i, &numsAndSignsCount, 2); */
			uint8 j;
			for(j = i; j < numsAndSignsCount; j++)
			{
				numsAndSigns[j] = numsAndSigns[j + 2];
			}
			numsAndSignsCount -= 2;
			i = 0;
		}
	}
	/* displayArray(numsAndSigns, numsAndSignsCount); */
	for(i = 0; i < numsAndSignsCount; i++)
	{
		if(numsAndSigns[i] == '+' || numsAndSigns[i] == '-')
		{
			numsAndSigns[i-1] = operation(numsAndSigns[i-1], numsAndSigns[i+1], numsAndSigns[i]);
			/* shiftArray(&numsAndSigns, i, &numsAndSignsCount, 2); */
			uint8 j;
			for(j = i; j < numsAndSignsCount; j++)
			{
				numsAndSigns[j] = numsAndSigns[j + 2];
			}
			numsAndSignsCount -= 2;
			i = 0;
		}
	}
	/* Get the Final Result From The numsAndSigns Array and Display on LCD */
	result = numsAndSigns[0];
	LCD_displayStringRowColumn(3, 0, "=");
	LCD_floatToString(result);
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
