#ifndef __DRV_SCCB_H__
#define __DRV_SCCB_H__

#include "common.h"

#define SCCB_DELAY_TIME 2000

typedef struct
{
    GPIO_TypeDef*   PORT;
    uint8_t         SDA;
    uint8_t         SCL;
} SCCB_DEV;

void SCCB_Init(SCCB_DEV* dev);
uint8_t SCCB_WriteByte(SCCB_DEV* dev, uint8_t dev_addr, uint8_t reg_addr, uint8_t reg_val);
uint8_t SCCB_ReadByte(SCCB_DEV* dev, uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint8_t *pBuf);

#endif
