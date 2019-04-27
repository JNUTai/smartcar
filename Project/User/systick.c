/******************************************************/
/* @file        SYSTICK.C                             */
/* @author      Stewart DHY                           */
/* @version     Ver 1.0                               */
/* @date        2016.1.9                              */
/* @note        系统滴答时钟配置                      */
/******************************************************/

#include "systick.h"

#if (DELAY_USING_INTERRUPT) //中断方式产生延时
static uint32_t delay_time = 0;

void SYSTICK_Init(void)
{
    SystemCoreClockUpdate();
    SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_TICKINT_Msk);
    SysTick->VAL = 0;
}

void SYSTICK_Delay_us(uint16_t nus)
{
    delay_time = 1UL;
    SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_TICKINT_Msk);
    SysTick->LOAD = ((SystemCoreClock / 1000000UL) * nus - 1UL) & SysTick_LOAD_RELOAD_Msk;
    SysTick->VAL = 0;
    SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk);
    while((delay_time > 0) && (SysTick->CTRL & SysTick_CTRL_ENABLE_Msk));
    SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_TICKINT_Msk);
}

void SYSTICK_Delay_ms(uint32_t nms)
{
    delay_time = nms;
    SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_TICKINT_Msk);
    SysTick->LOAD = ((SystemCoreClock / 1000UL) - 1UL) & SysTick_LOAD_RELOAD_Msk;
    SysTick->VAL = 0;
    SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk);
    while((delay_time > 0) && (SysTick->CTRL & SysTick_CTRL_ENABLE_Msk));
    SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_TICKINT_Msk);
}

void SysTick_Handler(void)
{
    if(delay_time > 0) delay_time--;
}
#else   //轮询方式产生延时
static SYSTICK_CALLBACKTYPE SYSTICK_CALLBACKPTR = NULL;
static bool oneshoot = false;

void SYSTICK_Init(void)
{
    SystemCoreClockUpdate();
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->VAL = 0;
}

void SYSTICK_INT_Config(bool status)
{
    (status)? (SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk):(SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk);
}

void SYSTICK_Callback_Install(SYSTICK_CALLBACKTYPE cbfunction)
{
    if(cbfunction != NULL)
        SYSTICK_CALLBACKPTR = cbfunction;
}

void SYSTICK_Callback_Uninstall(void)
{
    SYSTICK_CALLBACKPTR = NULL;
}

void SysTick_Handler(void)
{
    if(oneshoot) SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
    if(SYSTICK_CALLBACKPTR)
        SYSTICK_CALLBACKPTR();
}

void SYSTICK_Delay_us(uint16_t nus)
{
    uint32_t last_ctrl, last_load, tmp;
    last_ctrl = SysTick->CTRL;
    last_load = SysTick->LOAD;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->LOAD = ((SystemCoreClock / 1000000UL) * nus - 1UL) & 0x00FFFFFFUL;
    SysTick->VAL = 0;
    SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_ENABLE_Msk);
    do
    {
        tmp = SysTick->CTRL;
    } while((!(tmp & SysTick_CTRL_COUNTFLAG_Msk)) && (tmp & SysTick_CTRL_ENABLE_Msk));
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
    
    SysTick->VAL = 0;
    SysTick->LOAD = last_load;
    SysTick->CTRL = last_ctrl;
}

void SYSTICK_Delay_ms(uint32_t nms)
{
    uint32_t last_ctrl, last_load, tmp;
    last_ctrl = SysTick->CTRL;
    last_load = SysTick->LOAD;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->LOAD = ((SystemCoreClock / 1000UL) - 1UL) & 0x00FFFFFFUL;
    SysTick->VAL = 0;
    SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_ENABLE_Msk);
    while(nms > 0)
    {
        tmp = SysTick->CTRL;
        if(!(tmp & SysTick_CTRL_ENABLE_Msk))
            break;
        if(tmp & SysTick_CTRL_COUNTFLAG_Msk)
            nms--;
    }
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
    
    SysTick->VAL = 0;
    SysTick->LOAD = last_load;
    SysTick->CTRL = last_ctrl;
}

void SYSTICK_SetReloadValue(uint32_t value, bool reset)
{
    assert_param(value < 0x01000000UL);
    
    SysTick->LOAD = value;
    if(reset)
        SysTick->VAL = 0;
}

void SYSTICK_Countdown(uint32_t nus, bool interrupt, SYSTICK_TYPE config)
{
    if(config == SYSTICK_ONESHOOT)
        oneshoot = true;
    else
        oneshoot = false;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->LOAD = ((SystemCoreClock / 1000000UL) * nus - 1UL) & 0x00FFFFFFUL;
    SysTick->VAL = 0;
    SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_ENABLE_Msk|((interrupt)?(SysTick_CTRL_TICKINT_Msk):(0)));
}

#endif

void SYSTICK_Cmd(bool status)
{
    (status)? (SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk):(SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk);
}

uint32_t SYSTICK_GetValue(void)
{
    return SysTick->VAL;
}

void SYSTICK_Reset(void)
{
    SysTick->VAL = 0;
}

//函数别名
void Delay_us(uint32_t nus) __attribute__((weak, alias("SYSTICK_Delay_us")));
void Delay_ms(uint32_t nms) __attribute__((weak, alias("SYSTICK_Delay_ms")));
