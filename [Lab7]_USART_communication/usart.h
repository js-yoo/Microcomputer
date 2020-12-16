// jsyoo
#ifndef	__USART_H__
#define	__USART_H__
#include	<stdio.h>

#define	USART0	((unsigned char)0)
#define	USART1	((unsigned char)1)

extern	void USART_init(unsigned char ch, unsigned int ubrr_baud);
extern	int USART1_send(char data);
extern	int USART1_receive();

#endif
