/******************************************************/
/* @file        SYSTICK.H                             */
/* @author      Stewart DHY                           */
/* @version     Ver 1.0                               */
/* @date        2016.1.9                              */
/* @note        ϵͳ�δ�ʱ������ͷ�ļ�                */
/******************************************************/

#ifndef __LIB_SYSTICK_H__
#define __LIB_SYSTICK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

//��ʱ����ͨ���ж�/��ѯʵ�� 1:�ж� 0:��ѯ
#define DELAY_USING_INTERRUPT   0

//SYSTICK�쳣�ص�������������
typedef void (*SYSTICK_CALLBACKTYPE)(void);

//SYSTICK����

/**************************************************************/
/* @name        SYSTICK_Init                                  */
/* @brief       SysTick��ʼ������ȡϵͳ��ǰ�ں�Ƶ��           */
/* @code        SYSTICK_Init()                                */
/* @retval      None                                          */
/**************************************************************/
void SYSTICK_Init(void);

/**************************************************************/
/* @name        SYSTICK_Cmd                                   */
/* @brief       SysTickʱ��ʹ��                               */
/* @code        SYSTICK_Cmd(ENABLE)                           */
/* @param[in]   status ״̬                                   */
/*               @arg ENABLE  ����                            */
/*               @arg DISABLE ͣ��                            */
/* @retval      None                                          */
/**************************************************************/
void SYSTICK_Cmd(bool status);

/**************************************************************/
/* @name        SYSTICK_Delay_us                              */
/* @brief       ����SysTickʱ����ʱ                           */
/* @code        SYSTICK_Delay_us(1)                           */
/* @attention   nus��ȡֵ��ò�Ҫ����10000                    */
/* @param[in]   nus ��ʱʱ������λus                          */
/* @retval      None                                          */
/**************************************************************/
void SYSTICK_Delay_us(uint16_t nus);

/**************************************************************/
/* @name        SYSTICK_Delay_ms                              */
/* @brief       ����SysTickʱ����ʱ                           */
/* @code        SYSTICK_Delay_ms(1)                           */
/* @param[in]   nms ��ʱʱ������λms                          */
/* @retval      None                                          */
/**************************************************************/
void SYSTICK_Delay_ms(uint32_t nms);

/**************************************************************/
/* @name        SYSTICK_GetValue                              */
/* @brief       ��ȡ��ǰSysTickʱ�Ӽ���ֵ                     */
/* @code        SYSTICK_GetValue()                            */
/* @retval      SYSTICK��ǰ����ֵ                             */
/**************************************************************/
uint32_t SYSTICK_GetValue(void);

/**************************************************************/
/* @name        SYSTICK_Reset                                 */
/* @brief       ��λ��ǰSysTickʱ�Ӽ���ֵ                     */
/* @code        SYSTICK_Reset()                               */
/* @retval      None                                          */
/**************************************************************/
void SYSTICK_Reset(void);

//��������
void Delay_us(uint32_t nus);
void Delay_ms(uint32_t nms);

#if (!DELAY_USING_INTERRUPT)    //���º���ֻ����ʹ����ѯ��ʽ��ʱʱ�ſ���ʹ��

typedef enum
{
    SYSTICK_ONESHOOT,
    SYSTICK_PERIODIC
} SYSTICK_TYPE;

/**************************************************************/
/* @name        SYSTICK_INT_Config                            */
/* @brief       SysTickʱ���ж�ʹ��                           */
/* @code        SYSTICK_INT_Config(ENABLE)                    */
/* @param[in]   status ״̬                                   */
/*               @arg ENABLE  ����                            */
/*               @arg DISABLE ͣ��                            */
/* @retval      None                                          */
/**************************************************************/
void SYSTICK_INT_Config(bool status);

/**************************************************************/
/* @name        SYSTICK_Callback_Install                      */
/* @brief       SYSTICK�жϻص�����ע��                       */
/* @code        void SysTick_INT_ISR(void)
                {
                    //ISR
                }
                SYSTICK_Callback_Install(SysTick_INT_ISR);
   @endcode                                                   */
/* @param[in]   cbfunction �ص�����ָ�����                   */
/* @retval      None                                          */
/**************************************************************/
void SYSTICK_Callback_Install(SYSTICK_CALLBACKTYPE cbfunction);

/**************************************************************/
/* @name        SYSTICK_Callback_Uninstall                    */
/* @brief       SYSTICK�жϻص�����ע��                       */
/* @code        SYSTICK_Callback_Uninstall()                  */
/* @retval      None                                          */
/**************************************************************/
void SYSTICK_Callback_Uninstall(void);

/**************************************************************/
/* @name        SYSTICK_SetReloadValue                        */
/* @brief       SysTickʱ�Ӹ�λֵ����                         */
/* @code        SYSTICK_SetReloadValue(0x0000FFFF)            */
/* @param[in]   value ʱ�Ӹ�λֵ                              */
/* @param[in]   reset �����ǰ��ʱ��ֵ                        */
/*               @arg true  ���                              */
/*               @arg false ����                              */
/* @attention   valueȡֵ���ɴ���16777215(0x00FFFFFF)         */
/* @retval      None                                          */
/**************************************************************/
void SYSTICK_SetReloadValue(uint32_t value, bool reset);

/**************************************************************/
/* @name        SYSTICK_Countdown                             */
/* @brief       SysTickʱ�ӿ�ʼ����ʱ�������жϣ�������ʽ     */
/* @code        SYSTICK_Countdown(1000, ENABLE)               */
/* @param[in]   nus ����ʱʱ������λ΢��(us)                  */
/* @param[in]   interrupt ʹ�ܶ�ʱ�ж�                        */
/*               @arg ENABLE  ʹ��                            */
/*               @arg DISABLE ����                            */
/* @param[in]   config ����ʱ��ʽ                             */
/*               @arg SYSTICK_ONESHOOT ����ʱһ��             */
/*               @arg SYSTICK_PERIODIC ����ʱ���ڴ����ж�     */
/* @retval      None                                          */
/**************************************************************/
void SYSTICK_Countdown(uint32_t nus, bool interrupt, SYSTICK_TYPE config);

#endif

#ifdef __cplusplus
}
#endif

#endif
