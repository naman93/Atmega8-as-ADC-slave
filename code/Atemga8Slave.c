#ifndef F_CPU
#define F_CPU 8000000UL // clock frequency
#endif

#include <avr/io.h>
#include <util/delay.h>

//initialize ADC
void adc_init()
{
	// AREF = AVcc (Voltage given at AVCC will be taken as reference)
	ADMUX = (1<<REFS0);
	
	// ADC Enable (ADEN set to 1) and prescalar set to 64 (ADPS2 and ADPS1 == 1 and ADPS0 = 0) 
	// 8000000/64 = 125000
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);
}

// read ADC value
void adc_read(uint8_t ch)
{
	// select the corresponding channel 2~5
	// ANDing with '7' will always keep the value
	// of 'ch' between 0 and 7
	ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch;     // clears the bottom 3 bits before setting the ADC pin to read from
	
	// start single conversion
	// write '1' to ADSC
	ADCSRA |= (1<<ADSC);
	
	// wait for conversion to complete
	// ADSC becomes '0' again
	// till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	
}

// calls the adc_read function with appropriate pin selected
void getAdcData(int pin)
{
	
	switch (pin) {
		
		case 0 :
		adc_read(0x02);
		break;
		
		case 1 :
		adc_read(0x03);
		break;
		
		case 2 : 
		adc_read(0x04);
		break;
		
		case 3 :
		adc_read(0x05);
		break;
	} 
	
}


// sends the data from the ADC data registers to master
void sendAdcData(void)
{
	int count = 0; //tracks the number of bits sent to the master
	int new_clk = 0; //used to track rising edge of master clock 
	int old_clk = 0; //stores old value of master clock
	
	while (count<10) // ADC data has 10 bits (ADCL - 7) and (ADCH - 2) , (ADLAR = 0) - data is right aligned
	{
		
		if((PINB & 0b00001000) == 0b00001000)
		{
			_delay_us(300);
			if((PINB & 0b00001000) == 0b00001000)
			{
				new_clk = 1;
			}
			
		}
		
		else
		{
			new_clk = 0;
		}
		
		if (new_clk == 1 && old_clk == 0)
		{
			
			if (count>=0 && count<=7)
			{
				if (((ADCL >> count) & 0x00000001) == 0b00000001)
				{
					PORTB = 0b00010000;
				}
				
				else
				{
					PORTB = 0b00000000;
				}
				
				
			}
			
			if (count>7)
			{
				if (((ADCH >> (count-8)) & 0x00000001) == 0b00000001)
				{
					PORTB = 0b00010000;
				}
				
				else
				{
					PORTB = 0b00000000;
				}
			}
			
			count = count+1;
			
		}
		
		old_clk = new_clk;		
	}
	
}


int main(void)
{
	DDRB = 0b00010000; // used to obtain input from and give response to master.
	//{PB1 (state),PB3 (master clock)} - inputs    PB4 (data) - output
	
	// initialize ADC
	adc_init();
	
	_delay_ms(50);
	
	// Variables used for positive edge-triggered (master clock) initialization
	int oldClk = 0;
	int newClk = 0;
	
	while(1)
	{
		
		if((PINB & 0b00001010) == 0b00001010)
		{
			newClk = 1;
		}
		
		else
		{
			newClk = 0;
		}
		
		if (newClk == 1 && oldClk == 0) //Positive edge detected
		{
			for(int i=0; i<4; i++)
			{
				getAdcData(i);
				sendAdcData();
			}
			
		}
		
		oldClk = newClk; // old value of clock is stored in oldClk
	}
}



