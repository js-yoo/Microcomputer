#include	<avr/io.h>
#include	<avr/interrupt.h>

volatile	unsigned long timer0;	// Overflow마다 1씩 증가될 변수

volatile	unsigned int number;	// 증가되어 7-세그먼트 LED에 디스플레이될 숫자
unsigned	char led[]={0x48, 0x7D,0xC4, 0x64, 0x71, 0x62, 0x43, 0x7C, 0x40, 0x70};

// 타이머/카운터0 인터럽트 서비스 루틴
ISR(TIMER0_OVF_vect)
{
	timer0++;	// Overflow마다 1씩 증가
	// Overflow count가 4의 배수일 때 10자리 또는 2의 배수일 때 1자리 디스플레이
	if(timer0%2==0){
		PORTC=(timer0%4==0) ? led[(number%100) / 10] : led[number%10];
		PORTD=(PORTD|0xC0)&~(1<<((timer0%4==0) ? PD7:PD6));
	}
}
int main(void)
{
	DDRC=0xFF;
	DDRD |= 1<<PD7 | 1<<PD6;

	TCCR0 = 1<<CS02 | 1<<CS01;
	TIMSK |=1<<TOIE0;

	timer0=0;
	sei();

	number=12;
	
	while(1);
	return 0;
}
