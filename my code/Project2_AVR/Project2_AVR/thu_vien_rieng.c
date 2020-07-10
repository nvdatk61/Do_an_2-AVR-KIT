/*
 * thu_vien_rieng.c
 *
 * Created: 7/10/2020 9:16:59 PM
 *  Author: Dat
 */ 
#include"thu_vien_rieng.h"
#include"hunget_adc.h"
#include "hunget_lcd.h"
#include <avr\io.h>
#define FRE 8
#define LED7seg PORTC
#define LED1 PORTD
unsigned char push_button=0;
void INIT()
{
	DDRD=0xFF; // Port D lam output
	LED1=0xFF; // Logic =1 Leg don

	DDRC=0xFF; // Port C lam output
	LED7seg=0xFF; /// Logic =1 Leg7-seg

	DDRA = 0x00;
	PORTA = 0x00;
	ADC_PRES(128); // Init cho bo ADC
	//ADC_AVCC();
	ADC_2V56();
	ADC_IN(1);
}

void PORT()
{
	char array[]={0x88,0xEB,0x4C,0x49,0x2B,0x19,0x18,0xCB,0x08,0x09};
					/* write hex value for CA display from 0 to 9 */ 
    uint8_t led_shift = 0xFF;
    while(1)
    {
		for(int i=0;i<10;i++)
		{
			PORTD = led_shift;
			if(led_shift != 0 && i < 8)
				led_shift = led_shift << 1;
			else
				led_shift = 0xFF;
			
			PORTC = array[i]; /* write data on to the LED port */
			_delay_ms(1000); /* wait for 1 second */ 
		}	
    }

}
void LED7_OUT(int num)
{
//	LED7seg &= 0x08;
	switch(num)
	{
	case 0: LED7seg = 0x80; break;
	case 1: LED7seg = 0xE3; break;
	case 2: LED7seg = 0x44; break;
	case 3: LED7seg = 0x41; break;
	case 4: LED7seg = 0x23; break;
	case 5: LED7seg = 0x11; break;
	case 6: LED7seg = 0x10; break;
	case 7: LED7seg = 0xC3; break;
	case 8: LED7seg = 0x00; break;
	case 9: LED7seg = 0x01; break;
	}


}
void DELAY_MS(unsigned int mili_count)
{
	unsigned int i,j;
	mili_count = mili_count * FRE;
	for(i = 0; i < mili_count; i++)
	for(j = 0; j < 53; j++);
}
void PB_2_LED()
{
	LED7seg= 0x80;
	unsigned char push_button;
	for(;;)
	{
		push_button = PB_CHECK();
		if(!push_button) continue;
		LED7_OUT(push_button);
		switch (push_button)
		{
		case 1: PORTD = 0b11111100; break;
		case 2: PORTD = 0b11110011; break;
		case 3: PORTD = 0b11001111; break;
		case 4: PORTD = 0b00111111; break;
	//	default: PORTD = 0B00000000;
		}
	}

}
unsigned char PB_CHECK()
{	
		if((PINB & 0x0F) != 0x0F)
		{
		if(!(PINB & (1<<PB0)))	return 1;
		if(!(PINB & (1<<PB1)))	return 2;
		if(!(PINB & (1<<PB2)))	return 3;
		if(!(PINB & (1<<PB3)))	return 4;
		}

		return push_button;
}
void ADC_2_LCD()
{		
		DDRD |= (1<<PD5);   // PD5 output
		PORTD &= ~(1<<PD5); // PD5 = 0
		PORTC |= 0x0F;  // 4 bit thap output
		LCD4_INIT(0,0); // 
		LCD4_CUR_GOTO(1,0);
		LCD4_OUT_STR("Nguyen Vu Dat");
		LCD4_CUR_GOTO(2,0);
		LCD4_OUT_STR("Temp:    Do C");
		int V;
	for(;;)
	{
		ADC_STA_CONVERT();
		LCD4_CUR_GOTO(2,6);
		V=ADC*25;
		LCD4_OUT_DEC(V/100, 2);
		LCD4_OUT_STR(".");
		V=V-(V/100)*100;
		LCD4_OUT_DEC(V, 2);
	}
	DELAY_MS(500);
}
//moi
void LED7_OUT_new(uint8_t num)
{

	char array[]={0x88,0xEB,0x4C,0x49,0x2B,0x19,0x18,0xCB,0x08,0x09};
					/* write hex value for CA display from 0 to 9 */ 
   
    while(1)
    {
		for(int i=0;i<10;i++)
		{
			PORTC = array[num]; /* write data on to the LED port */
			DELAY_MS(1000); /* wait for 1 second */ 
		}
    }
}
void PORT_new(){
	char array[]={0x88,0xEB,0x4C,0x49,0x2B,0x19,0x18,0xCB,0x08,0x09};
					/* write hex value for CA display from 0 to 9 */ 
    uint8_t led_shift = 0xFF;
    while(1)
    {
		for(int i=0;i<10;i++)
		{
			PORTD = led_shift;
			if(led_shift != 0 && i < 8)
				led_shift = led_shift << 1;
			else
				led_shift = 0xFF;
			
			PORTC = array[i]; /* write data on to the LED port */
			_delay_ms(1000); /* wait for 1 second */ 
		}	
    }
}	
void LCD_Command(unsigned char cmnd)
{
	LCD_Data_Port= cmnd;
	LCD_Command_Port &= ~(1<<RS);	/* RS=0 command reg. */
	LCD_Command_Port &= ~(1<<RW);	/* RW=0 Write operation */
	LCD_Command_Port |= (1<<EN);	/* Enable pulse */
	_delay_us(1);
	LCD_Command_Port &= ~(1<<EN);
	_delay_ms(3);
}
void LCD_Char (unsigned char char_data)	/* LCD data write function */
{
	LCD_Data_Port= char_data;
	LCD_Command_Port |= (1<<RS);	/* RS=1 Data reg. */
	LCD_Command_Port &= ~(1<<RW);	/* RW=0 write operation */
	LCD_Command_Port |= (1<<EN);	/* Enable Pulse */
	_delay_us(1);
	LCD_Command_Port &= ~(1<<EN);
	_delay_ms(1);
}
void LCD_Init (void)			/* LCD Initialize function */
{
	LCD_Command_Dir = 0xFF;		/* Make LCD command port direction as o/p */
	LCD_Data_Dir = 0xFF;		/* Make LCD data port direction as o/p */
	_delay_ms(20);			/* LCD Power ON delay always >15ms */
	
	LCD_Command (0x38);		/* Initialization of 16X2 LCD in 8bit mode */
	LCD_Command (0x0C);		/* Display ON Cursor OFF */
	LCD_Command (0x06);		/* Auto Increment cursor */
	LCD_Command (0x01);		/* Clear display */
	LCD_Command (0x80);		/* Cursor at home position */
}
void LCD_String (char *str)		/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		LCD_Char (str[i]);
	}
}
void LCD_String_xy (char row, char pos, char *str)/* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	LCD_String(str);		/* Call LCD string function */
}
void LCD_Clear()
{
	LCD_Command (0x01);		/* clear display */
	LCD_Command (0x80);		/* cursor at home position */
}
void LCD_shift(){
	int shift,i;
	char first_line[] = "Nguyen Vu Dat, 20160953";
	char second_line[] = "Vien Dien tu-Vien Thong, Truong DHBKHN";
	LCD_Init();		/* Initialize LCD*/
	LCD_String(first_line);/* Write string on 1st line of LCD*/
    LCD_String_xy(1,0, second_line);
	shift = 30;	/* Number of time shifts count=30 */
	_delay_ms(1000);
	
	while(1)
	{
		for(i=0;i<shift;i++)
		{
			LCD_Command(0x1c);/* shift entire display right */
			_delay_ms(1000);
		}			
		shift=30;/* number of time shifts 30 */
			
		for(i=0;i<30;i++)
		{
			LCD_Command(0x18);/* shift entire display left */
			_delay_ms(1000);
		}
	}
}


void ADC_Init()
{
	DDRA=0x0;			/* Make ADC port as input */
	ADCSRA = 0x87;			/* Enable ADC, fr/128  */
	ADMUX = 0x40;			/* Vref: Avcc, ADC channel: 0 */
	
}

int ADC_Read(char channel)
{
	int Ain,AinLow;
	
	ADMUX=ADMUX|(channel & 0x0f);	/* Set input channel to read */

	ADCSRA |= (1<<ADSC);		/* Start conversion */
	while((ADCSRA&(1<<ADIF))==0);	/* Monitor end of conversion interrupt */
	
	_delay_us(10);
	AinLow = (int)ADCL;		/* Read lower byte*/
	Ain = (int)ADCH*256;		/* Read higher 2 bits and 
					Multiply with weight */
	Ain = Ain + AinLow;				
	return(Ain);			/* Return digital value*/
}

 
void ADC_2_LCD_new(){
	char String[5];
	int value;

	ADC_Init();
	LCD_Init();			/* Initialization of LCD */
	LCD_String("Test ADC & LCD");	/* Write string on 1st line of LCD */
	LCD_String_xy(1, 0, "ADC0:     /1023" );
	while(1)
	{
		LCD_Command(0xc4);	/* LCD16x2 cursor position */
		value=ADC_Read(0);	/* Read ADC channel 0 */
		itoa(value,String,10);	/* Integer to string conversion */ 
		LCD_String_xy(1, 6, String);						
		_delay_ms(500);
	}
}


void UART_init(long USART_BAUDRATE)
{
	UCSRB |= (1 << RXEN) | (1 << TXEN);/* Turn on transmission and reception */
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);/* Use 8-bit character sizes */
	UBRRL = BAUD_PRESCALE;		/* Load lower 8-bits of the baud rate value */
	UBRRH = (BAUD_PRESCALE >> 8);	/* Load upper 8-bits*/
}

unsigned char UART_RxChar()
{
	while ((UCSRA & (1 << RXC)) == 0);/* Wait till data is received */
	return(UDR);			/* Return the byte*/
}

void UART_TxChar(char ch)
{
	while (! (UCSRA & (1<<UDRE)));	/* Wait for empty transmit buffer*/
	UDR = ch ;
}

void UART_SendString(char *str)
{
	unsigned char j=0;
	
	while (str[j]!=0)		/* Send string till null */
	{
		UART_TxChar(str[j]);	
		j++;
	}
}
