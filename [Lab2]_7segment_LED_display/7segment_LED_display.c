#include <avr/io.h>

unsigned char digit[]={0x88, 0xBE,0xC4, 0xA4, 0xB2, 0xA1, 0x83, 0xBC, 0x80, 0xB0};
// 0~9 포트 출력값에 해당하는 16진수 값(공통 애노드 기준)
void display_7segled(unsigned char led[],unsigned int number) // sub routine
{	PORTC = led[number];	} // PORTC에 원하는 led에 해당하는 16진수값 할당 

int main(void)
{
	DDRC = 0xFF; // PORT C를 출력으로 사용
	display_7segled(digit, 4); // sub routine에 4를 지정하여 호출
		
	return 0;
}
