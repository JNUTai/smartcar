#include "common.h"

#ifdef DEFAULT_USART
#pragma import(__use_no_semihosting)

#ifdef __CC_ARM // MDK Support
#ifdef __cplusplus
extern "C" {
#endif

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

void _sys_exit(int x)
{
    x = x;
}

__weak int fputc(int ch, FILE *f)
{
    while(USART_GetFlagStatus(DEFAULT_USART, USART_FLAG_TC) == RESET);
    USART_SendData(DEFAULT_USART, ch);
    
    return ch;
}

__weak int fgetc(FILE *f)
{
    while(USART_GetFlagStatus(DEFAULT_USART, USART_FLAG_RXNE) == RESET);
    
    return USART_ReceiveData(DEFAULT_USART);
}

#ifdef __cplusplus
}
#endif
#endif
#endif

uint8_t USART_putchar(USART_TypeDef* USARTx, uint8_t ch)
{
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
    USART_SendData(USARTx, ch);
    
    return ch;
}

uint8_t USART_getchar(USART_TypeDef* USARTx)
{
    while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
    
    return USART_ReceiveData(USARTx);
}
