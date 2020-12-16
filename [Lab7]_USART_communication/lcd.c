// jsyoo
#include	<avr/io.h>
#include	"lcd.h"

#define	RS	PD5	// LCD 문자디스플레이에 연결된 포트D 의 핀번호 
#define	RW	PD6    // RW 제어 신호를 PD6로 받기
#define	E	PD7    // Enable 신호를 PD7로 받기
// E 신호를 일정기간 유지시키는 함수
void	gen_E_strobe(void) 
{	volatile	int	i;

	PORTD |= 1<<E;		// E 신호를 High로
	for(i=0; i<10; i++);		// E 스트로브 신호를 일정기간 High로 유지
	PORTD &= ~(1<<E);		// E 신호를 Low로
}	
// 다른 명령이 동작 중인지 확인하고 기다리는 함수
void	wait_BusyFlag(void) 
{	volatile int 		i;
	unsigned char		bf;
// 0번지 읽고 Busy Flag 읽기 위해 입력으로 지정
	DDRC = 0x0;			// 포트C를 입력핀으로 설정
	PORTD = (PORTD & ~(1<<RS)) | 1<<RW; // RS <- Low, RW <- High
	do{
		PORTD |= 1<<E;		// E 신호를 High로
		for(i=0; i<10; i++);	// E 스트로브 신호를 일정기간 High로 유지
		bf = PINC & 1<<PC7;	// busy flag 읽어 냄
		PORTD &= ~(1<<E);	// E 신호를 Low로
	}while( bf );			// bf 값이 0이 아니면 busy, 0 일 때까지 반복
}
// 명령어를 수행하는 함수
void	LCD_command(unsigned char data)
{	wait_BusyFlag();		// busy flag가 0될 때까지 대기, 1이면 다음 동작으로 넘어가기
	DDRC = 0xFF;			// command 날리기 위해 출력모드로 지정
	PORTC = data;			// 데이터 버스 상에 올리기
	PORTD &= ~(1<<RS | 1<<RW);	// RS <- 0, RW <-0 = 0번지 write
	gen_E_strobe();		// E 스트로브 신호 만들기
}
// 데이터를 쓰기 위한 함수
void	LCD_data_write(unsigned char data)
{	wait_BusyFlag();
	DDRC = 0xFF; // 출력모드로 지정
	PORTC = data; // 데이터 버스 상에 올리기
	PORTD = (PORTD | 1<<RS) & ~(1<<RW); // RS <- 1, RW <-0 = 1번지 write
	gen_E_strobe();
}
// LCD를 쓰기 전 초기화하는 함수
void	LCD_init(void)
{
	DDRD |= 1<<RS | 1<<RW | 1<<E;	// RS(5), RW(6), E(7) 핀을 출력핀으로 설정
	
	PORTD &= ~(1<<RS | 1<<E | 1<<RW); 	// 초기에 RS, E, RW <- 0
// 인터페이스/디스플레이 설정 : DL(1-8bit 인터페이스), N(1-두줄표시), F(1-문자5x10도트)
	LCD_command(0x3C); 
// 커서 초기 위치 : 커서 위치를 1행 1열로 움직임
LCD_command(0x02);
// 화면 클리어 : 스페이스에 해당되는 ASCII 문자 0x20인가
//              커서 위치를 1행 1열에 움직임
	LCD_command(0x01);
// 문자 입력 모드 : I/D(1), SH(0) – 커서를 오른쪽을 이동하면서 문자 입력
	LCD_command(0x06); 
// 디스플레이 ON/OFF 제어 : D(1-디스플레이ON), C(1-커서ON), B(1-커서깜박임ON)
	LCD_command(0x0F);
}
// (row, col)을 받아 해당 위치로 커서 세팅
void	set_cursor(unsigned int row, unsigned int col)
{
	LCD_command(0x80 + (row % 2) * 0x40 + (col % 0x40));
}
// 함수 정의 : row, col 위치에서 문자열 str 을 LCD에 출력시킨다.
void	LCD_str_write(unsigned int row, unsigned int col, char *str)
{
	int	i;
	
	set_cursor(row, col);
// 문자열(space 포함)이 끝날 때까지 반복
	for(i=0; (i+col < MAX_LCD_STRING) && (str[i] != '\0'); i++)
		LCD_data_write(str[i]);
}
