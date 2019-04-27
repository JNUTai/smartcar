#ifndef __COMMON_H__
#define __COMMON_H__

#include "stm32f4xx.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define DEFAULT_USART   USART1

#define BITBAND_REG(Reg,Bit) (*((uint32_t*)((((uint32_t)&(Reg)) & 0xF0000000u) + 0x02000000u + (32u*(((uint32_t)&(Reg)) & 0x000FFFFFu)) + (4u*((uint32_t)(Bit))))))

#define POUTA(pin)      BITBAND_REG(GPIOA->ODR, (pin))
#define PINA(pin)       BITBAND_REG(GPIOA->IDR, (pin))
#define POUTB(pin)      BITBAND_REG(GPIOB->ODR, (pin))
#define PINB(pin)       BITBAND_REG(GPIOB->IDR, (pin))
#define POUTC(pin)      BITBAND_REG(GPIOC->ODR, (pin))
#define PINC(pin)       BITBAND_REG(GPIOC->IDR, (pin))
#define POUTD(pin)      BITBAND_REG(GPIOD->ODR, (pin))
#define PIND(pin)       BITBAND_REG(GPIOD->IDR, (pin))
#define POUTE(pin)      BITBAND_REG(GPIOE->ODR, (pin))
#define PINE(pin)       BITBAND_REG(GPIOE->IDR, (pin))

uint8_t USART_putchar(USART_TypeDef* USARTx, uint8_t ch);
uint8_t USART_getchar(USART_TypeDef* USARTx);

#endif
