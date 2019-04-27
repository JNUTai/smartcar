#include "ov7725.h"

#define OV7725_CTL_PORT     GPIOA
#define OV7725_SDA          0
#define OV7725_SCL          1

uint8_t ov7725_data[2][OV7725_DATASIZE];
bool ov7725_img_valid = false;
static uint8_t ov7725_valid_buff = 1;
static OV7725_CALLBACKTYPE OV7725_CALLBACKPTR = NULL;

static const SCCB_DEV ctl = {OV7725_CTL_PORT, OV7725_SDA, OV7725_SCL};

static const OV7725_REG_TYPE OV7725_INIT_REG[] =
{
  {OV7725_COM4         , 0x41},
  {OV7725_CLKRC        , 0x00},
  {OV7725_COM2         , 0x03},
  {OV7725_COM3         , 0xD0},
  {OV7725_COM7         , 0x40},
  {OV7725_COM10        , 0x00},
  {OV7725_HSTART       , 0x3F},
  {OV7725_HSIZE        , 0x50},
  {OV7725_VSTRT        , 0x03},
  {OV7725_VSIZE        , 0x78},
  {OV7725_HREF         , 0x00},
  {OV7725_SCAL0        , 0x0A},
  {OV7725_AWB_Ctrl0    , 0xE0},
  {OV7725_DSPAuto      , 0xFF},
  {OV7725_DSP_Ctrl2    , 0x0C},
  {OV7725_DSP_Ctrl3    , 0x00},
  {OV7725_DSP_Ctrl4    , 0x00},

  {OV7725_HOutSize     , 0x14},

  {OV7725_VOutSize     , 0x1E},
  
  {OV7725_EXHCH        , 0x00},
  {OV7725_GAM1         , 0x0c},
  {OV7725_GAM2         , 0x16},
  {OV7725_GAM3         , 0x2a},
  {OV7725_GAM4         , 0x4e},
  {OV7725_GAM5         , 0x61},
  {OV7725_GAM6         , 0x6f},
  {OV7725_GAM7         , 0x7b},
  {OV7725_GAM8         , 0x86},
  {OV7725_GAM9         , 0x8e},
  {OV7725_GAM10        , 0x97},
  {OV7725_GAM11        , 0xa4},
  {OV7725_GAM12        , 0xaf},
  {OV7725_GAM13        , 0xc5},
  {OV7725_GAM14        , 0xd7},
  {OV7725_GAM15        , 0xe8},
  {OV7725_SLOP         , 0x20},
  {OV7725_LC_RADI      , 0x00},
  {OV7725_LC_COEF      , 0x13},
  {OV7725_LC_XC        , 0x08},
  {OV7725_LC_COEFB     , 0x14},
  {OV7725_LC_COEFR     , 0x17},
  {OV7725_LC_CTR       , 0x05},
  {OV7725_BDBase       , 0x99},
  {OV7725_BDMStep      , 0x03},
  {OV7725_SDE          , 0x04},
  {OV7725_BRIGHT       , 0x20},
  {OV7725_CNST         , 0xFF},
  {OV7725_SIGN         , 0x06},
  {OV7725_UVADJ0       , 0x11},
  {OV7725_UVADJ1       , 0x02}
//  
//  {OV7725_COM4         , 0x41},
//  {OV7725_CLKRC        , 0x01},
//  {OV7725_COM2         , 0x03},
//  {OV7725_COM3         , 0xC0},
//  {OV7725_COM7         , 0x40},
//  //{OV7725_COM7         , 0x46}, // 彩色 
//  {OV7725_COM10        , 0x08},
//  {OV7725_HSTART       , 0x3F},
//  {OV7725_HSIZE        , 0x50},
//  {OV7725_VSTRT        , 0x03},
//  {OV7725_VSIZE        , 0x78},
//  {OV7725_HREF         , 0x00},
//  {OV7725_SCAL0        , 0x0A},
//  {OV7725_AWB_Ctrl0    , 0xE0},
//  {OV7725_DSPAuto      , 0xFF},
//  {OV7725_DSP_Ctrl2    , 0x0C},
//  {OV7725_DSP_Ctrl3    , 0x00},
//  {OV7725_DSP_Ctrl4    , 0x00},
//  {OV7725_HOutSize     , 0x28},
//  {OV7725_VOutSize     , 0x3C},
//  {OV7725_EXHCH        , 0x00},
//  {OV7725_GAM1         , 0x0c},
//  {OV7725_GAM2         , 0x16},
//  {OV7725_GAM3         , 0x2a},
//  {OV7725_GAM4         , 0x4e},
//  {OV7725_GAM5         , 0x61},
//  {OV7725_GAM6         , 0x6f},
//  {OV7725_GAM7         , 0x7b},
//  {OV7725_GAM8         , 0x86},
//  {OV7725_GAM9         , 0x8e},
//  {OV7725_GAM10        , 0x97},
//  {OV7725_GAM11        , 0xa4},
//  {OV7725_GAM12        , 0xaf},
//  {OV7725_GAM13        , 0xc5},
//  {OV7725_GAM14        , 0xd7},
//  {OV7725_GAM15        , 0xe8},
//  {OV7725_SLOP         , 0x20},
//  {OV7725_LC_RADI      , 0x00},
//  {OV7725_LC_COEF      , 0x13},
//  {OV7725_LC_XC        , 0x08},
//  {OV7725_LC_COEFB     , 0x14},
//  {OV7725_LC_COEFR     , 0x17},
//  {OV7725_LC_CTR       , 0x05},
//  {OV7725_BDBase       , 0x99},
//  {OV7725_BDMStep      , 0x03},
//  {OV7725_SDE          , 0x04},
//  {OV7725_BRIGHT       , 0x20},
//  {OV7725_CNST         , 0x80},
//  {OV7725_SIGN         , 0x06},
//  {OV7725_UVADJ0       , 0x11},
//  {OV7725_UVADJ1       , 0x02},
//  {OV7725_COM8         , 0x00},
//  {OV7725_AECH         , 0x00},
//  {OV7725_AEC          , 0x50},
};

static uint8_t OV7725_SCCB_Init(void)
{
  uint8_t tmp, i;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  SCCB_Init((SCCB_DEV*)&ctl);
  
  SCCB_WriteByte((SCCB_DEV*)&ctl, OV7725_DEVICE_ADDR, OV7725_COM7, 0x80); //Reset all registers
  Delay_ms(50);
  
  if(SCCB_ReadByte((SCCB_DEV*)&ctl, OV7725_DEVICE_ADDR, OV7725_PID, 1, &tmp))
    return 1;   // Communication failure
  
  if(tmp == OV7725_DEVICE_PID)
  {
    tmp = sizeof(OV7725_INIT_REG) / sizeof(OV7725_INIT_REG[0]); // Get sizes of array
    for(i = 0; i < tmp; i++)    // Write register
    {
      if(SCCB_WriteByte((SCCB_DEV*)&ctl, OV7725_DEVICE_ADDR, OV7725_INIT_REG[i].ADDR, OV7725_INIT_REG[i].VAL))
          return 1;
      Delay_ms(2);
    }
  }
  else
    return 1;   // The device is not OV7725
  
  return 0;
}

static void OV7725_DCMI_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  DCMI_InitTypeDef DCMI_InitStructure;
//  DCMI_CROPInitTypeDef DCMI_CROPInitStructure;
  
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);//DCMI
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | 
                         RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE, ENABLE);//使能DCMI的GPIO时钟
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_DCMI);//DCMI_HSYNC
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_DCMI);//DCMI_PIXCLK
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_DCMI);//DCMI_D5
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_DCMI);//DCMI_VSYNC
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_DCMI);//DCMI_D6
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_DCMI);//DCMI_D7
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_DCMI);//DCMI_D0
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_DCMI);//DCMI_D1
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_DCMI);//DCMI_D2
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_DCMI);//DCMI_D3
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_DCMI);//DCMI_D4
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;
  DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;
  DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Rising;
  DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_High;
  DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_Low;
  DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;
  DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b;
  DCMI_Init(&DCMI_InitStructure);
  
//  DCMI_CROPInitStructure.DCMI_VerticalStartLine = 0;
//  DCMI_CROPInitStructure.DCMI_HorizontalOffsetCount = 0;
//  DCMI_CROPInitStructure.DCMI_VerticalLineCount = OV7725_DATAROW;
//  DCMI_CROPInitStructure.DCMI_CaptureCount = OV7725_DATACOL;
//  DCMI_CROPConfig(&DCMI_CROPInitStructure);
//  DCMI_CROPCmd(ENABLE);
  
  DCMI_ClearITPendingBit(DCMI_IT_FRAME);
  DCMI_ITConfig(DCMI_IT_FRAME, ENABLE);
  NVIC_EnableIRQ(DCMI_IRQn);
  DCMI_Cmd(ENABLE);
}

static void OV7725_DMA_Init(void)
{
  DMA_InitTypeDef  DMA_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  
  DMA_DeInit(DMA2_Stream1);
  DMA_InitStructure.DMA_Channel = DMA_Channel_1;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(DCMI->DR);
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&(ov7725_data[1 - ov7725_valid_buff][0]);
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = OV7725_DATASIZE / 4;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  DMA_Init(DMA2_Stream1, &DMA_InitStructure);
}

uint8_t OV7725_Init(void)
{
  if(OV7725_SCCB_Init())
    return 1;
  
  OV7725_DCMI_Init();
  OV7725_DMA_Init();
  
  return 0;
}

void OV7725_Cmd(FunctionalState NewState)
{
  DCMI_CaptureCmd(NewState);
}

uint8_t OV7725_GetImg(uint8_t *data)
{
  uint32_t i;
  uint8_t *ptr;
  
  if(ov7725_img_valid)
  {
    ptr = (uint8_t*)&(ov7725_data[ov7725_valid_buff][0]);
    for(i = 0; i < OV7725_DATASIZE * sizeof(ov7725_data[0][0]); i++)
      *data++ = *ptr++;
  }
  else
  {
    return 1;
  }
  
  return 0;
}

void OV7725_Callback_Install(OV7725_CALLBACKTYPE cbfunction)
{
  if(cbfunction)
    OV7725_CALLBACKPTR = cbfunction;
}

void OV7725_Callback_Uninstall(void)
{
  OV7725_CALLBACKPTR = NULL;
}

uint8_t OV7725_WriteReg(uint8_t reg_addr, uint8_t reg_val)
{
  return SCCB_WriteByte((SCCB_DEV*)&ctl, OV7725_DEVICE_ADDR, reg_addr, reg_val);
}

uint8_t OV7725_ReadReg(uint8_t reg_addr, uint8_t length, uint8_t *pBuf)
{
  return SCCB_ReadByte((SCCB_DEV*)&ctl, OV7725_DEVICE_ADDR, reg_addr, length, pBuf);
}

void DCMI_IRQHandler(void)
{
  DMA_Cmd(DMA2_Stream1, DISABLE);
  while(DMA_GetCmdStatus(DMA2_Stream1) != DISABLE);
  
  DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1);
  DCMI_ClearITPendingBit(DCMI_IT_FRAME);
  
  DMA_SetCurrDataCounter(DMA2_Stream1, OV7725_DATASIZE / 4);
  DMA_MemoryTargetConfig(DMA2_Stream1, (uint32_t)&(ov7725_data[ov7725_valid_buff][0]), DMA_Memory_0);
  DMA_Cmd(DMA2_Stream1, ENABLE);
  
  ov7725_valid_buff = 1 - ov7725_valid_buff;
  ov7725_img_valid = true;
  
  if(OV7725_CALLBACKPTR)
    OV7725_CALLBACKPTR();
}
