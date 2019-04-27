#include "ov7725.h"

#define OV7725_CTL_PORT     GPIOC
#define OV7725_SDA          1//3
#define OV7725_SCL          0//2

uint8_t ov7725_data[2][OV7725_DATASIZE];
bool ov7725_img_valid = false;
static uint8_t ov7725_valid_buff = 1;
static OV7725_CALLBACKTYPE OV7725_CALLBACKPTR = NULL;

static const SCCB_DEV ctl = {OV7725_CTL_PORT, OV7725_SDA, OV7725_SCL};

static const OV7725_REG_TYPE OV7725_INIT_REG[] =
{

  {OV7725_COM4         , 0x81},
  {OV7725_CLKRC        , 0x01},
  {OV7725_COM2         , 0x03},
  {OV7725_COM3         , 0xD0},
  {OV7725_COM7         , 0x40}, 
  {OV7725_HSTART       , 0x3F},
  {OV7725_HSIZE        , 0x50},
  {OV7725_VSTRT        , 0x03},
  {OV7725_VSIZE        , 0x78},
  {OV7725_HREF         , 0x00},
  {OV7725_SCAL0        , 0x0A},
  {OV7725_AWB_Ctrl0    , 0xE0},
  {OV7725_DSPAuto      , 0xff},
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
  {OV7725_BRIGHT       , 0x00},
  {OV7725_CNST         , 15},
  {OV7725_SIGN         , 0x06},
  {OV7725_UVADJ0       , 0x11},
  {OV7725_UVADJ1       , 0x02},
  
//  {OV7725_COM4         , 0x41},
//  {OV7725_CLKRC        , 0x00},
//  {OV7725_COM2         , 0x03},
//  {OV7725_COM3         , 0xD0},
//  {OV7725_COM7         , 0x40},
//  {OV7725_COM10        , 0x00},
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

//  {OV7725_HOutSize     , 0x14},

//  {OV7725_VOutSize     , 0x1E},
//  
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
//  {OV7725_BRIGHT       , 0x00},
//  {OV7725_CNST         , 0xFF},
//  {OV7725_SIGN         , 0x06},
//  {OV7725_UVADJ0       , 0x11},
//  {OV7725_UVADJ1       , 0x02}
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
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
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

static void OV7725_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
  TIM_ICInitTypeDef TIM_ICInitStruct;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIO时钟
  
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0     // 8位并行数据口
                           | GPIO_Pin_1
                           | GPIO_Pin_2
                           | GPIO_Pin_3
                           | GPIO_Pin_4
                           | GPIO_Pin_5
                           | GPIO_Pin_6
                           | GPIO_Pin_7;
  GPIO_Init(GPIOD, &GPIO_InitStruct);
  
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_10;
  GPIO_Init(GPIOE, &GPIO_InitStruct);
  
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOE, &GPIO_InitStruct);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_Period = 9;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
  
  TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStruct.TIM_ICFilter = 1;
  TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;//捕获下降沿
  TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInit(TIM1, &TIM_ICInitStruct);
  
  TIM_DMACmd(TIM1, TIM_DMA_CC1, ENABLE);
  TIM_Cmd(TIM1, ENABLE);
}
/************************************************
 * 函数名：VSYNC_GPIO_Config
 * 描述  ：OV7725 VSYNC中断相关配置
 * 输入  ：无
 * 输出  ：无
 * 注意  ：无
 ************************************************/
static void VSYNC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*初始化时钟，注意中断要开AFIO*/	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	  /*PA0---VSYNC*/
	/*初始化引脚*/
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*配置中断*/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling ; 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_GenerateSWInterrupt(EXTI_Line0);	/*中断挂到 EXTI_Line0  线*/		
	/*配置优先级*/
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetFlagStatus(EXTI_Line0) == SET)
  {
    EXTI_ClearFlag(EXTI_Line0);
    if(PINA(0)) // 一场结束
    {
      if(DMA_GetCurrDataCounter(DMA2_Stream1) != 0)
      {
        ov7725_img_valid = false;
        DMA_Cmd(DMA2_Stream1, DISABLE);
        while(DMA_GetCmdStatus(DMA2_Stream1) == ENABLE);
        DMA_SetCurrDataCounter(DMA2_Stream1, OV7725_DATASIZE * sizeof(ov7725_data[0][0]));
        DMA_MemoryTargetConfig(DMA2_Stream1, (uint32_t)&(ov7725_data[1 - ov7725_valid_buff][0]), DMA_Memory_0);
      }
    }
    else  // 一场开始
    {
      DMA_Cmd(DMA2_Stream1, ENABLE);
    }
  }
}
//void OV7725_Cmd(FunctionalState NewState)
//{
//  EXTI_InitTypeDef EXTI_InitStruct;
//  
//  if(NewState)
//  {
//    EXTI_ClearFlag(EXTI_Line8);
//    EXTI_InitStruct.EXTI_Line = EXTI_Line8;
//    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
//    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
//    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
//    EXTI_Init(&EXTI_InitStruct);
//  }
//  else
//  {
//    EXTI_InitStruct.EXTI_Line = EXTI_Line8;
//    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
//    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
//    EXTI_InitStruct.EXTI_LineCmd = DISABLE;
//    EXTI_Init(&EXTI_InitStruct);
//  }
//}

static void OV7725_DMA_Init(void)
{
  DMA_InitTypeDef  DMA_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  
  DMA_DeInit(DMA2_Stream1);
  DMA_InitStructure.DMA_Channel = DMA_Channel_6;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(GPIOD->IDR);
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&(ov7725_data[1 - ov7725_valid_buff][0]);
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = OV7725_DATASIZE * sizeof(ov7725_data[0][0]);
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  DMA_Init(DMA2_Stream1, &DMA_InitStructure);
  
  DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1);
  DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE);
  
  NVIC_EnableIRQ(DMA2_Stream1_IRQn);
}



uint8_t OV7725_Init(void)
{
  if(OV7725_SCCB_Init())
    return 1;
  
  OV7725_DMA_Init();
  OV7725_GPIO_Init();
  VSYNC_GPIO_Config();
  return 0;
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

void DMA2_Stream1_IRQHandler(void)
{
  DMA_Cmd(DMA2_Stream1, DISABLE);
  while(DMA_GetCmdStatus(DMA2_Stream1) != DISABLE);
  
  DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1);
  
  DMA_SetCurrDataCounter(DMA2_Stream1, OV7725_DATASIZE * sizeof(ov7725_data[0][0]));
  DMA_MemoryTargetConfig(DMA2_Stream1, (uint32_t)&(ov7725_data[ov7725_valid_buff][0]), DMA_Memory_0);
  
  ov7725_valid_buff = 1 - ov7725_valid_buff;
  ov7725_img_valid = true;
  
  if(OV7725_CALLBACKPTR)
    OV7725_CALLBACKPTR();
}
