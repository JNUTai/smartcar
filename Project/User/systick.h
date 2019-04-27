/******************************************************/
/* @file        SYSTICK.H                             */
/* @author      Stewart DHY                           */
/* @version     Ver 1.0                               */
/* @date        2016.1.9                              */
/* @note        系统滴答时钟配置头文件                */
/******************************************************/

#ifndef __LIB_SYSTICK_H__
#define __LIB_SYSTICK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

//延时函数通过中断/轮询实现 1:中断 0:轮询
#define DELAY_USING_INTERRUPT   0

//SYSTICK异常回调函数类型声明
typedef void (*SYSTICK_CALLBACKTYPE)(void);

//SYSTICK方法

/**************************************************************/
/* @name        SYSTICK_Init                                  */
/* @brief       SysTick初始化，获取系统当前内核频率           */
/* @code        SYSTICK_Init()                                */
/* @retval      None                                          */
/**************************************************************/
void SYSTICK_Init(void);

/**************************************************************/
/* @name        SYSTICK_Cmd                                   */
/* @brief       SysTick时钟使能                               */
/* @code        SYSTICK_Cmd(ENABLE)                           */
/* @param[in]   status 状态                                   */
/*               @arg ENABLE  启用                            */
/*               @arg DISABLE 停用                            */
/* @retval      None                                          */
/**************************************************************/
void SYSTICK_Cmd(bool status);

/**************************************************************/
/* @name        SYSTICK_Delay_us                              */
/* @brief       利用SysTick时钟延时                           */
/* @code        SYSTICK_Delay_us(1)                           */
/* @attention   nus的取值最好不要超过10000                    */
/* @param[in]   nus 延时时长，单位us                          */
/* @retval      None                                          */
/**************************************************************/
void SYSTICK_Delay_us(uint16_t nus);

/**************************************************************/
/* @name        SYSTICK_Delay_ms                              */
/* @brief       利用SysTick时钟延时                           */
/* @code        SYSTICK_Delay_ms(1)                           */
/* @param[in]   nms 延时时长，单位ms                          */
/* @retval      None                                          */
/**************************************************************/
void SYSTICK_Delay_ms(uint32_t nms);

/**************************************************************/
/* @name        SYSTICK_GetValue                              */
/* @brief       获取当前SysTick时钟计数值                     */
/* @code        SYSTICK_GetValue()                            */
/* @retval      SYSTICK当前计数值                             */
/**************************************************************/
uint32_t SYSTICK_GetValue(void);

/**************************************************************/
/* @name        SYSTICK_Reset                                 */
/* @brief       复位当前SysTick时钟计数值                     */
/* @code        SYSTICK_Reset()                               */
/* @retval      None                                          */
/**************************************************************/
void SYSTICK_Reset(void);

//函数别名
void Delay_us(uint32_t nus);
void Delay_ms(uint32_t nms);

#if (!DELAY_USING_INTERRUPT)    //以下函数只有在使用轮询方式延时时才可以使用

typedef enum
{
    SYSTICK_ONESHOOT,
    SYSTICK_PERIODIC
} SYSTICK_TYPE;

/**************************************************************/
/* @name        SYSTICK_INT_Config                            */
/* @brief       SysTick时钟中断使能                           */
/* @code        SYSTICK_INT_Config(ENABLE)                    */
/* @param[in]   status 状态                                   */
/*               @arg ENABLE  启用                            */
/*               @arg DISABLE 停用                            */
/* @retval      None                                          */
/**************************************************************/
void SYSTICK_INT_Config(bool status);

/**************************************************************/
/* @name        SYSTICK_Callback_Install                      */
/* @brief       SYSTICK中断回调函数注册                       */
/* @code        void SysTick_INT_ISR(void)
                {
                    //ISR
                }
                SYSTICK_Callback_Install(SysTick_INT_ISR);
   @endcode                                                   */
/* @param[in]   cbfunction 回调函数指针入口                   */
/* @retval      None                                          */
/**************************************************************/
void SYSTICK_Callback_Install(SYSTICK_CALLBACKTYPE cbfunction);

/**************************************************************/
/* @name        SYSTICK_Callback_Uninstall                    */
/* @brief       SYSTICK中断回调函数注销                       */
/* @code        SYSTICK_Callback_Uninstall()                  */
/* @retval      None                                          */
/**************************************************************/
void SYSTICK_Callback_Uninstall(void);

/**************************************************************/
/* @name        SYSTICK_SetReloadValue                        */
/* @brief       SysTick时钟复位值设置                         */
/* @code        SYSTICK_SetReloadValue(0x0000FFFF)            */
/* @param[in]   value 时钟复位值                              */
/* @param[in]   reset 清除当前定时器值                        */
/*               @arg true  清除                              */
/*               @arg false 保留                              */
/* @attention   value取值不可大于16777215(0x00FFFFFF)         */
/* @retval      None                                          */
/**************************************************************/
void SYSTICK_SetReloadValue(uint32_t value, bool reset);

/**************************************************************/
/* @name        SYSTICK_Countdown                             */
/* @brief       SysTick时钟开始倒计时，采用中断，非阻塞式     */
/* @code        SYSTICK_Countdown(1000, ENABLE)               */
/* @param[in]   nus 倒计时时长，单位微秒(us)                  */
/* @param[in]   interrupt 使能定时中断                        */
/*               @arg ENABLE  使能                            */
/*               @arg DISABLE 禁能                            */
/* @param[in]   config 倒计时方式                             */
/*               @arg SYSTICK_ONESHOOT 倒计时一次             */
/*               @arg SYSTICK_PERIODIC 倒计时周期触发中断     */
/* @retval      None                                          */
/**************************************************************/
void SYSTICK_Countdown(uint32_t nus, bool interrupt, SYSTICK_TYPE config);

#endif

#ifdef __cplusplus
}
#endif

#endif
