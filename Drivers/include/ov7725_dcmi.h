#ifndef __DRV_OV7725_H__
#define __DRV_OV7725_H__

#include "common.h"
#include "ov7725_reg.h"
#include "delay.h"
#include "sccb.h"

#define OV7725_DATAROW (60)
#define OV7725_DATACOL (10)
#define OV7725_DATASIZE ((OV7725_DATACOL) * (OV7725_DATAROW))

typedef void (*OV7725_CALLBACKTYPE)(void);

extern uint8_t ov7725_data[2][OV7725_DATASIZE];
extern bool ov7725_img_valid;

uint8_t OV7725_Init(void);
void OV7725_Cmd(FunctionalState NewState);
uint8_t OV7725_GetImg(uint8_t *data);
uint8_t OV7725_WriteReg(uint8_t reg_addr, uint8_t reg_val);
uint8_t OV7725_ReadReg(uint8_t reg_addr, uint8_t length, uint8_t *pBuf);
void OV7725_Callback_Install(OV7725_CALLBACKTYPE cbfunction);
void OV7725_Callback_Uninstall(void);

#endif
