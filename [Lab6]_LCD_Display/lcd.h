// jsyoo
#ifndef	__LCD_H__ // if not defined, #define 문장으로 정의하지 않았다면 아래에 등장하는
                  // #endif 문장 이전의 내용들을 include 영역에 포함시키라는 뜻
#define	__LCD_H_
// 디스플레이할 칸 수를 지정 (0x40 : 64칸)
#define	MAX_LCD_STRING	0x40
// 다른 소스 파일에 선언된 함수 사용
extern void	gen_E_strobe(void);
extern void	wait_BusyFlag(void);
extern void	LCD_command(unsigned char data);
extern void	LCD_data_write(unsigned char data);
extern void	LCD_init(void);
extern void	set_cursor(unsigned int row, unsigned int col);
extern void	LCD_str_write(unsigned int row, unsigned int col, char *str);
#endif
