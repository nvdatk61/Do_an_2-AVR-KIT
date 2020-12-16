
#define F_CPU 8000000UL
#define FRE 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "hunget_adc.h"
#include "hunget_lcd.h"
#include "hunget_uart.h"
#include "thu_vien_rieng.h"

unsigned char push_button = 0;

int main(void)
{
	//Count
	
	//INIT();
	//PORT_new();
	
	//Button
	
	//INIT();
	//PB_2_LED();

	
	//LCD
		
	//ADC_2_LCD_new();
	
	//UART
	
	//char first_line[] = "Nguyen Vu Dat, 20160953, dtvt04-k61";
	//char second_line[] = "Vien Dien Tu-Vien Thong, Truong DHBKHN";
	//LCD_Init();		/* initialize lcd*/
	//LCD_String(first_line);/* write string on 1st line of lcd*/
    //LCD_String_xy(1,0, second_line);
	//_delay_ms(1000);
	//
	//UART_init(9600);
	//
	//while(1)
	//{
		//UART_SendString(first_line);
		//UART_SendString("\n"); //newline
		//UART_SendString(second_line);
		//UART_SendString("\n"); //newline
		//LCD_Command(0x18);/* shift entire display left */
			///* 0x1c la dic phai
			 //0x18 la dic trai*/
		//_delay_ms(1000);
	//}
	
	// Dong co
	uint8_t Do = 0;
	char Do_char[6];
	LCD_Init();
	LCD_String_xy(0,0, "Dieu khien servo");
	LCD_String_xy(1,1, "Goc quay:");
	DDRD |= (1<<PD5);	/* Make OC1A pin as output */ 
	TCNT1 = 0;		/* Set timer1 count zero */
	ICR1 = 2499;		/* Set TOP count for timer1 in ICR1 register */

	/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/64 */
	TCCR1A = (1<<WGM11)|(1<<COM1A1);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS11);
	OCR1A = 65;
	while(1)
	{
		push_button = PB_CHECK();
		LED7_OUT(push_button);
		switch(push_button)
		{
			case 1: OCR1A = 65;	/* Set servo shaft at -90° position */
					LCD_String_xy(1,11, " 0   ");
					_delay_ms(100);
					Do = 0;
					break;	
			case 2: OCR1A = 320;	/* Set servo shaft at -90° position */
					LCD_String_xy(1,11, "180  ");
					_delay_ms(100);
					Do = 180;
					break;	
			case 3: if(OCR1A >= 65 && OCR1A <= 310)
					{
						//count += 10; 
						OCR1A += 14; 
						Do += 10;
						sprintf(Do_char, "%d	",Do);
						//LCD_String_xy(1,11, Do_char);
						_delay_ms(200);
						break;
					}
					else
					{
						_delay_ms(200);
						break;
					}		
					break;				
			case 4: if(OCR1A >= 75 && OCR1A <= 320)
					{
						//count -= 10; 
						OCR1A -= 14;
						Do -= 10;
						sprintf(Do_char, "%d	",Do);
						//LCD_String_xy(1,11, Do_char );
						_delay_ms(200);
						break;
					}
					else
					{
						_delay_ms(200);
						break;
					}
					break;			
			//default:break;	
			//case 1: count = 65;  break;
			//case 2: count = 300; break;
			//default: break;
		}
		//OCR1A = count;
		//_delay_ms(2000);
	}
	return 0;
}