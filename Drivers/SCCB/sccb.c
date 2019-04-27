#include "sccb.h"

#define SDA_H               (*sda_o = 1)
#define SDA_L               (*sda_o = 0)
#define SCL_H               (*scl = 1)
#define SCL_L               (*scl = 0)
#define SDA_IN()            (*sda_i)

static uint32_t *sda_o, *sda_i, *scl;

static void SCCB_Delay(void)
{
    uint32_t time = SCCB_DELAY_TIME;
    while(time)
        time--;
}

static uint8_t SCCB_Start(void)
{
    SDA_H;
    SCL_H;
    SCCB_Delay();
    
    if(!SDA_IN())
        return 1;
    SDA_L;
    SCCB_Delay();
    SCL_L;
    
    if(SDA_IN())
    {
        SCL_H;
        return 2;
    }
    
    return 0;
}

static void SCCB_Stop(void)
{
    SCL_L;
    __nop();
    __nop();
    SDA_L;
    SCCB_Delay();
    SCL_H;
    SCCB_Delay();
    SDA_H;
    SCCB_Delay();
}

static void SCCB_Ack(void)
{
    SCL_L;
    SCCB_Delay();
    SDA_L;
    SCCB_Delay();
    SCL_H;
    SCCB_Delay();
    SCL_L;
    SCCB_Delay();
}

static void SCCB_NoAck(void)
{
    SCL_L;
    SCCB_Delay();
    SDA_H;
    SCCB_Delay();
    SCL_H;
    SCCB_Delay();
    SCL_L;
    SCCB_Delay();
}

static uint8_t SCCB_WaitAck(void)
{
    SCL_L;
    __nop();
    __nop();
    SDA_H;
    SCCB_Delay();
    SCL_H;
    SCCB_Delay();
    if(SDA_IN())
    {
        SCL_L;
        return 1;
    }
    SCL_L;
    
    return 0;
}

static void SCCB_Write8Bit(uint8_t data)
{
    uint8_t i;
    for(i = 0; i < 8; i++)
    {
        if(data & 0x80)
            SDA_H;
        else
            SDA_L;
        
        data <<= 1;
        SCCB_Delay();
        SCL_H;
        SCCB_Delay();
        SCL_L;
        __nop();
        __nop();
        __nop();
    }
}

static uint8_t SCCB_Read8Bit(void)
{
    uint8_t i;
    uint8_t recv = 0;
    SCL_L;
    SDA_H;
    for(i = 0; i < 8; i++)
    {
        recv <<= 1;
        SCL_L;
        SCCB_Delay();
        SCL_H;
        SCCB_Delay();
        if(SDA_IN())
            recv |= 0x01;
    }
    __nop();
    __nop();
    SCL_L;
    
    return recv;
}

static uint8_t SCCB_WriteByte_Once(uint8_t dev_addr, uint8_t reg_addr, uint8_t reg_val)
{
    if(SCCB_Start())
        return 1;
    
    SCCB_Write8Bit(dev_addr & 0xFE);
    if(SCCB_WaitAck())
    {
        SCCB_Stop();
        return 2;
    }
    SCCB_Write8Bit(reg_addr);
    if(SCCB_WaitAck())
    {
        SCCB_Stop();
        return 3;
    }
    SCCB_Write8Bit(reg_val);
    if(SCCB_WaitAck())
    {
        SCCB_Stop();
        return 3;
    }
    SCCB_Stop();
    
    return 0;
}

static uint8_t SCCB_ReadByte_Once(uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint8_t *pBuf)
{
    if(SCCB_Start())
        return 1;
    
    SCCB_Write8Bit(dev_addr & 0xFE);
    if(SCCB_WaitAck())
    {
        SCCB_Stop();
        return 2;
    }
    
    SCCB_Write8Bit(reg_addr);
    if(SCCB_WaitAck())
    {
        SCCB_Stop();
        return 2;
    }
    SCCB_Stop();
    
    if(SCCB_Start())
        return 3;
    
    SCCB_Write8Bit(dev_addr | 0x01);
    if(SCCB_WaitAck())
    {
        SCCB_Stop();
        return 4;
    }
    
    while(length)
    {
        *pBuf = SCCB_Read8Bit();
        if(length == 1)
            SCCB_NoAck();
        else
            SCCB_Ack();
        pBuf++;
        length--;
    }
    
    SCCB_Stop();
    
    return 0;
}

uint8_t SCCB_WriteByte(SCCB_DEV* dev, uint8_t dev_addr, uint8_t reg_addr, uint8_t reg_val)
{
    uint8_t i = 0;
    
    sda_o = &BITBAND_REG(dev->PORT->ODR, dev->SDA);
    sda_i = &BITBAND_REG(dev->PORT->IDR, dev->SDA);
    scl = &BITBAND_REG(dev->PORT->ODR, dev->SCL);
    
    while(SCCB_WriteByte_Once(dev_addr, reg_addr, reg_val))
    {
        if(++i == 20)
            return 1;
    }
    
    return 0;
}

uint8_t SCCB_ReadByte(SCCB_DEV* dev, uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint8_t *pBuf)
{
    uint8_t i = 0;
    
    sda_o = &BITBAND_REG(dev->PORT->ODR, dev->SDA);
    sda_i = &BITBAND_REG(dev->PORT->IDR, dev->SDA);
    scl = &BITBAND_REG(dev->PORT->ODR, dev->SCL);
    
    while(SCCB_ReadByte_Once(dev_addr, reg_addr, length, pBuf))
    {
        if(++i == 20)
            return 1;
    }
    
    return 0;
}

void SCCB_Init(SCCB_DEV* dev)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStructure.GPIO_Pin = (1 << (uint16_t)(dev->SDA));
    GPIO_Init(dev->PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin = (1 << (uint16_t)(dev->SCL));
    GPIO_Init(dev->PORT, &GPIO_InitStructure);
    GPIO_SetBits(dev->PORT, ((1 << (uint16_t)(dev->SDA)) | (1 << (uint16_t)(dev->SCL))));
}
