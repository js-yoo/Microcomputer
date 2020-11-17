#include <avr/io.h>

#define S0 	PB5 // PORTB 5를 S0으로 정의
#define S1 	PB6 // PORTB 6를 S1으로 정의
#define S2 	PB7 // PORTB 5를 S0으로 정의

#define D0 	PB0 // PORTB 0를 D0으로 정의
#define D1 	PB1 // PORTB 1를 D1으로 정의
#define D2 	PB2 // PORTB 2를 D2으로 정의
#define D3 	PB3 // PORTB 3를 D3으로 정의
#define D4 	PB4 // PORTB 4를 D4으로 정의

int main(void) // main program
{
	DDRB = 1<<D0 | 1<<D1 | 1<<D2 | 1<<D3 | 1<<D4; // DDRB의 D0~D4를 1로 설정
	// 무한 루프
	while(1){
		if( !(PINB & (1<<S0)) ) // S0의 입력이 LOW일 때 => Switch ON
			PORTB |= 1<<D0 | 1<<D1; // PORTB의 D0, D1의 LED ON
		else // S0의 입력이 High일 때 => Switch OFF
			PORTB &=~(1<<D0 | 1<<D1); // PORTB의 D0, D1의 LED OFF

		if( !(PINB & (1<<S1)) ) // S1의 입력이 LOW일 때 => Switch ON
			PORTB |= 1<<D2; // PORTB의 D2의 LED ON
		else // S1의 입력이 High일 때 => Switch OFF
			PORTB &=~(1<<D2); // PORTB의 D2의 LED OFF

		if( !(PINB & (1<<S2)) ) // S2의 입력이 LOW일 때 => Switch ON
			PORTB |= 1<<D3 | 1<<D4; // PORTB의 D3, D4의 LED ON
		else // S2의 입력이 High일 때 => Switch OFF
			PORTB &=~(1<<D3 | 1<<D4); // PORTB의 D3, D4의 LED OFF
	}
	return 0;
}
