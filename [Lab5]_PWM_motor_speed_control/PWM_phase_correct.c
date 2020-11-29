#include <avr/io.h>
#include <avr/interrupt.h>

#define	F_CPU	16000000UL				// CPU Clock 16MHz
#define	PRESCALE		256L
#define	PULSE_PER_OVERFLOW	510L
#define	MS_OVERFLOW_CYCLE	((double)(PULSE_PER_OVERFLOW * PRESCALE) \
				/(double)((double)F_CPU/1000.0))

#define	OC0	PB4
#define	UP	PB7
#define	DOWN	PB6

#define	NUM_REQ		2
#define	REQ_UP		0
#define	REQ_DOWN	1
#define	DEBOUNCE_CYCLE	50				// Delay 값

volatile	unsigned	long	timer0;	// Overflow마다 1씩 증가될 변수
volatile	unsigned	int		number;
unsigned char led[] = {0x48, 0x7D, 0xC4, 0x64, 0x71, 0x62, 0x43, 0x7C, 0x40, 0x70};

volatile	unsigned	long	req[NUM_REQ] = {0, 0};
double	ms_ov_cycle;
// Timer/Counter0 Interrupt service routine
SIGNAL(SIG_OVERFLOW0) // 타이머/카운터0 인터럽트 서비스 루틴
{	int	i;
	timer0++;							// Overflow마다 1씩 증가
	
	// 오버플로 횟수가 짝수일 때 10자리, 홀수일 때 1자리 디스플레이
	PORTC = (timer0 % 2 == 0) ? led[(number % 100) / 10] : led[number%10];
	PORTD = (PORTD |0xC0) & ~(1<<((timer0 % 2 == 0) ? PD7 : PD6));
	
	for(i=0; i<NUM_REQ; i++)
		if( req[i] > 0)					// REQ 요청이 있을 때만 
		req[i]--;						// 시간 지연 경과 응답
}

// ms_interval초 시간 지연을 위한 오버플로 횟수 계산 함수
unsigned long	ms_req_timer0(unsigned long ms_interval)
{	return ( ms_interval <= 0) ? 0 : \
			+ (unsigned long)(ms_interval / ms_ov_cycle); 
			// interval/cycle(=클럭 횟수) 반환
}

int	main(void)
{	
	DDRC = 0xFF;						// 출력 지정
	DDRD |= 1<<PD7 | 1<<PD6;			// 두 자리 7-segment LED를 켜기 위한 출력(자리수 선택)
	DDRB |= 1<<OC0;						// OC0=PB4(4) 출력
	DDRB &= ~(1<<UP | 1<<DOWN);			// UP, DOWN 스위치 위치를 입력 방향으로
	PORTB |= 1<<UP | 1<<DOWN;			// UP, DOWN 스위치 내부 풀업 저항

	TCCR0 = 1<<WGM00;              		// Phase Correct PWM mode
	TCCR0 |= 1<<CS02 | 1<<CS01;   		// 프리스케일러 CS02:00=(1,1,0) 256분주
	TCCR0 |= 1<<COM01;					// 상승 중 OCR0와 일치하면 Clear, 하강 중 일치하면 Set
	
	TIMSK |= 1<<TOIE0;					// TIMER/COUNTER0 INTERRUPT ENABLE
	
	timer0 = 0;
	sei();

	ms_ov_cycle	= MS_OVERFLOW_CYCLE;
	OCR0 = 0;
	number = OCR0*100/256;				// OCR0 값과 number값을 일치
	
	while(1)							// 무한 loop req[REQ_UP]==0 은 DEBOUNCE_CYCLE 시간경과 요청 완료 검사
{
	  if( (req[REQ_UP]==0) && !(PINB & (1<<UP)))
                 { //req[UP]응답, UP 스위치 눌림 검사
		 req[REQ_UP] = ms_req_timer0(DEBOUNCE_CYCLE); // delay 시간 재설정
		 OCR0 = (OCR0 == 255) ? 255 : OCR0 + 1; // 최댓값은 255, UP 스위치가 눌리면 OCR0 증가
         }
			
	  if( (req[REQ_DOWN]==0) && !(PINB & (1<<DOWN)))
                 { //req[DOWN]응답,DOWN 스위치 검사
		 req[REQ_DOWN] = ms_req_timer0(DEBOUNCE_CYCLE); // delay 시간 재설정
	          OCR0 = (OCR0 == 0) ? 0 : OCR0 - 1; // 최솟값은 0, DOWN 스위치가 눌리면 OCR0 감소
	  }     

	  number = OCR0 * 100/256;
	}
	return 0;
}
