

#ifndef __LONG_UART_H__
#define  __LONG_UART_H__

void Long_Uart0_Init(void);
void Long_Uart0_PutChar(char c);
void Long_Uart0_PutString(char *s);
int32_t Long_Uart0_GetChar(char *c);

void Long_Uart2_Init(void);
void Long_Uart2_PutChar(char c);
void Long_Uart2_PutString(char *s);
int32_t Long_Uart2_GetChar(char *c);

#endif
