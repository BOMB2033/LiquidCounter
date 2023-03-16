/*
 * LiquidCounter.cpp
 *
 * Created: 15.03.2023 12:30:59
 * Author : Fedor Kasper
 * ќдин импульс п€ть милилитров
 * «амыкание на ноль
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "i2c.h"
#include "LCD_I2C.h"

unsigned int LiquidCounter = 0;

int ArraySize(char array[])
{
	int i = 0;
	while(array[i] != 0) i++;
	return i;
}

char *IntToChar(unsigned int i)
{
	if (i<=65100)
	{
		char *sim = "0000000";
		sim[6] = 'l';
		sim[5] = 'm';
		sim[4] = i%10 + 48;
		if (i > 9)
		{
			sim[3] = (i%100 - i%10)/10 + 48;
		}else{
			sim[3] = ' ';
		}
		if (i > 99)
		{
			sim[2] = (i%1000 - i%100)/100 + 48;
			}else{
			sim[2] = ' ';
		}
		if (i > 999)
		{
			sim[1] = (i%10000 - i%1000)/1000 + 48;
			}else{
			sim[1] = ' ';
		}
		if (i > 9999)
		{
			sim[0] = (i%100000 - i%10000)/10000 + 48;
			}else{
			sim[0] = ' ';
		}
		
		
		
		return sim;
	}else{
		LiquidCounter = 0;
		return "Overflow";
	}
	
}

ISR(INT0_vect)
{
	LiquidCounter+=50;
	lcd_cmd(0x01);
	char *str = IntToChar(LiquidCounter); 
	int c = ArraySize(str);
	lcd_cmd(0x80 + 16 - c);
	lcd_msg(str);
	lcd_cmd(0xC0);
	lcd_msg("FvK");
}

void interrupts_ini()
{
	EICRA = 2;/*
	–егистр EICRA Ч определ€ет тип обрабатываемого событи€

	  3	      2	      1	      0
	ICS11	ICS10	ICS01	ICS00
	Ѕиты регистра ICS01 и ICS00 отвечают за тип событи€ на INT0,  ICS11 и ICS10 за INT1.

	“ипы событий при которых происходит прерывание:

	00 Ц Ќизкий уровень на ножке INT0 или INT1
	01 Ц Ћюбое изменение на ножке INT0 или INT1
	10 Ц нисход€щий фронт на ножке INT0 или INT1
	11 Ц восход€щий фронт на ножке INT0 или INT1
	*/
	EIMSK = 1;/*
	–егистр EIMSK Ч разрешает прерывани€
	  1       0
	INT1	INT0
	*/
	DDRD &=~(1<<2);
	PORTD |= (1<<2);
	sei();
}

void Init()
{
	DDRB = 0;
	PORTB = 0;
	
	interrupts_ini();
	i2c_init();
	i2c_start();
	i2c_write(0x70);		
	lcd_init();	
	lcd_cmd(0x80);
	lcd_msg("Good morning");
	lcd_cmd(0xC0);
	lcd_msg("Fedor Kasper");
} 


int main(void)
{
    Init();
	
	
    while (1) 
    {
			
    }
}

