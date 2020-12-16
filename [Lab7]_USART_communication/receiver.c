// jsyoo
#include <avr/io.h>
#include <stdio.h>
#include "lcd.h"
#include "usart.h"
#include <util/delay.h>

int main(void)
{ char lcd_string[2][MAX_LCD_STRING];

 LCD_init(); // LCD 초기화

 USART_init(USART1, 12); // USART1로 통신 선로 지정(보오레이트 : 38400(UBRR=12), 8MHz)

 sprintf(lcd_string[0], "USART(RECEIVE)"); 

  LCD_str_write(0, 0, lcd_string[0]);

unsigned char data=0;

while(1){

   data=USART1_receive(); // 송신된 데이터를 수신
   sprintf(lcd_string[1], "Data=%-5d", (int)data); // 수신한 데이터를 형식에 맞춰 저장
   LCD_str_write(1, 0, lcd_string[1]); // LCD에 출력

   }
 return 0;
}
