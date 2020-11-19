# jsyoo

#define F_CPU	16000000UL // CPU 주파수가 16MHz일 때 다음과 같이 정의

#include <avr/io.h>
#include <util/delay.h> // _delay_ms()함수를 사용하기 위해

#define PRESSED	1 // 스위치가 한번 눌린 상태를 1로 정의
#define	RELEASED	0 // 스위치 눌린 이후의 상태를 0으로 정의
#define DEBOUNCE_MS	1000 // 1000ms delay를 주기위해 1000으로 정의

unsigned char digit[]={0x88, 0xBE,0xC4, 0xA4, 0xB2, 0xA1, 0x83, 0xBC, 0x80, 0xB0}; 
// 0~9 포트 출력값에 해당하는 16진수 값(공통 애노드 기준)

void display_7segled(unsigned char led[],unsigned int number) // sub routine
{	PORTC = led[number];	} // PORTC에 원하는 led에 해당하는 16진수값 할당 

int main(void) // main program
{
	int number; // 메인 프로그램 실행동안 스위치가 눌린 횟수를 저장할 변수
	int before; // 이전 상태를 저장할 변수

	DDRC = 0xFF; // PORT C를 출력으로 사용
	DDRD = DDRD & ~(1<<PD0); // PD0를 스위치 사용을 위해 입력핀으로 설정 
	PORTD = PORTD | 1<<PD0; // PD0를 내부 풀업저항으로 연결

	number = 0; // number 변수의 초깃값 설정
	before = RELEASED; // before 변수의 초기값을 설정

	while(1){
		display_7segled(digit, number%10); 
		// number가 10이상일 때를 위해 %10을 사용해 나머지로 인덱싱
		if(before==RELEASED && !(PIND&1<<PD0)){ //전에 눌리지 않은 상태에서 처음으로 눌림
			number++; // 스위치가 눌렸으니 number를 1증가
			_delay_ms(DEBOUNCE_MS); // 1000ms만큼 딜레이
			before=PRESSED; // 눌린상태로 저장
		}else if(before==PRESSED&&(PIND&1<<PD0)){ // 전에 눌린 상태에서 처음으로 떨어짐
			_delay_ms(DEBOUNCE_MS); // 1000ms만큼 딜레이
			before=RELEASED; // 떨어진 상태로 저장
		}
	}
	return 0;
}
