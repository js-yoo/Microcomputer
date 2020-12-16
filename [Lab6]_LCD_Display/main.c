// jsyoo
#include	<stdio.h>
#include	<avr/io.h>

#define	F_CPU	16000000UL
#include	<util/delay.h>
#include	"lcd.h"

// 전역변수 선언
long 	a = 0, b = 0;
double 	x = 0., y = 0.;

//////////////////////////////////////////////////////////////
int	main(void)
{	// 지역 변수 선언
	char lcd_string[2][MAX_LCD_STRING];

	LCD_init(); // LCD를 초기화하는 루틴(어떻게 사용할지를 사용 전에 미리 정하기)

	while(1){
		a = a + 1;
		b = b + 2;

		x += 0.1;
		y += 0.2;
// sprintf : 디스플레이할 글자를 메모리 공간 상에 변수를 잡아서 저장
// lcd_string[0], lcd_string[1] 각 행에 쓸 글자를 저장
		sprintf(lcd_string[0], "A=%-5ld B=%-5ld", a, b); // %-5ld : 5칸 decimal로
		LCD_str_write(0, 0, lcd_string[0]);
// %-5.1f : 5칸 float, 1자리 소수점
		sprintf(lcd_string[1], "X=%-5.1f Y=%-5.1f   ", x, y); 
		LCD_str_write(1, 0, lcd_string[1]);
		_delay_ms(1000); // 1초마다 디스플레이 하기 위해 딜레이
	}
	return 0;
}
