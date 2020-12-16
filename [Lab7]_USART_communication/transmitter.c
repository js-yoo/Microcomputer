// jsyoo
#include <avr/io.h>
#include <stdio.h>
#include "lcd.h"
#include "usart.h"
#include <util/delay.h>

int main(void)
{
   char lcd_string[2][MAX_LCD_STRING];

    LCD_init(); // LCD 초기화

   USART_init(USART1, 12); // USART1로 통신 선로 지정(보오레이트 : 38400(UBRR=12), 8MHz)

   sprintf(lcd_string[0], "USART(TRANSMIT)"); 
   
   LCD_str_write(0, 0, lcd_string[0]);
 //LCD에 커서위치 지정을 위해 row=0, col=0, 첫 번째 행의 모든 문자열을 함수를 통해 전달함

unsigned char data = 0;


   while(1){
    
   data = data + 1; // 데이터 1씩 증가
    sprintf(lcd_string[1], "Data=%-5d", (int)data); // LCD 아랫줄 첫번째부터 data를 형식에 맞게 메모리 상에 저장
    LCD_str_write(1, 0, lcd_string[1]); // LCD에 출력
   USART1_send(data); // 데이터 송신
   _delay_ms(1000); // 원활한 통신 및 출력을 위해 1초 딜레이 지정

   }

 return 0;
}
