/*
 * hunget_adc.h
 *
 * Created: 7/10/2020 9:19:15 PM
 *  Author: Dat
 */ 


#ifndef HUNGET_ADC_H_
#define HUNGET_ADC_H_
#define ADC_PRES(factor) \
{ \
	ADCSRA |= (1<<ADEN); \
	switch(factor) \
	{ \
		case 2: ADCSRA |= (1<<ADPS0); break; \
		case 4: ADCSRA |= (1<<ADPS1); break; \
		case 8: ADCSRA |= (1<<ADPS0)|(1<<ADPS1); break; \
		case 16: ADCSRA |= (1<<ADPS2); break; \
		case 32: ADCSRA |= (1<<ADPS0)|(1<<ADPS2); break; \
		case 64: ADCSRA |= (1<<ADPS1)|(1<<ADPS2); break; \
		case 128: ADCSRA |= (1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2); break; \
		/* Mac dinh la 32 */ \
		default: ADCSRA |= (1<<ADPS0)|(1<<ADPS2); \
	} \
}

/************************************************************************************/

// Chon dien ap tham chieu. Neu khong chon thi dien ap tham chieu
// mac dinh la dien ap tren chan AREF.

#define ADC_AVCC() ADMUX |= (1<<REFS0)
#define ADC_2V56() ADMUX |= (1<<REFS1)|(1<<REFS0)
#define ADC_AREF() ADMUX &= ~((1<<REFS1)|(1<<REFS0))

/*************************************************************************************/

// Chon dau vao single ended. Neu khong chon thi dau vao mac dinh la dau ADC0

#define ADC_IN(channel) \
{ \
	unsigned char adc_input = 0; \
	adc_input = ADMUX & 0xE0; \
	adc_input |= (channel); \
	ADMUX = adc_input; \
}

#define ADC_ALIGN_L() 		ADMUX |= (1<<ADLAR)
#define ADC_ALIGN_R() 		ADMUX &= ~(1<<ADLAR)
#define ADC_AUTO() 			ADCSRA |= (1<<ADATE)
#define ADC_STA_CONVERT() 	ADCSRA |= (1<<ADSC)




#endif /* HUNGET_ADC_H_ */