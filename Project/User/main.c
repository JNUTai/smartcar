#include "main.h"

#define servMotorCenture  120 //�������λ��
#define servMotorLeft     servMotorCenture+44  //�������166
#define servMotorRight    servMotorCenture-44 //����Ҽ���69


//uint32_t Task_Delay[NumOfTask];
extern u8 lost_flag;
extern u8 end_flag;
extern u8 K;
uint8_t pic[OV7725_DATASIZE];
bool busy = false;
float LastErr=0,dErr=0;
float pPram = 0.8 ;          //����ϵ��0.7
float dPram = 3.2 ;            //΢��ϵ3.1
float ctl;
uint32_t speed=60;
void ov7725_isr(void)
{
  uint8_t *ptr = (uint8_t*)&pic[0];
  if(!busy)
  {
    OV7725_GetImg(ptr);
    busy = true;
  }
}

int main(void)
{
    int i;
  uint32_t duopwm = servMotorCenture;//5~25��Ӧ0~180��
  uint32_t last_duo;
 
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Debug_USART_Config();

  if(OV7725_Init())
    printf("Camera init failed\r\n");
  else
  {
    OV7725_Callback_Install(ov7725_isr);
    //OV7725_Cmd(ENABLE);
    printf("Done!\r\n");
  }
  
  TIM3_PWM_Init(100,84-1);  //�����ʱ��10kHz
  TIM4_PWM_Init(duopwm);  //�����ʱ��50Hz
	TIM_SetCompare1(TIM3,0);
//  TIM_Cmd(TIM3, DISABLE);
  while(1)
  {
    if(busy)
    {
/*******************�����Ǵ��ڷ���ͼ��********************
      putchar(0x01);
      putchar(0xFE);
      for(i = 0; i < OV7725_DATASIZE; i++)
        putchar(pic[i]);
      putchar(0xFE);
      putchar(0x01);
********************************************************/
      
      ctl=find(pic,OV7725_DATASIZE);

//      if(end_flag)
//			{
////				TIM_SetCompare1(TIM3,100);
//        end_flag=0;
////				while(1);
//			}
      
      
/***************�����Ƕ��PD����*******************/     
      dErr = (ctl);
      
      last_duo=duopwm;
      duopwm = servMotorCenture - (int)(dErr*pPram + (dErr-LastErr)*dPram);  //��ֱ�ʾ΢����
//      speed = 35 + abs(servMotorCenture - duopwm)*2;        //�ٶ�����ת�����

			if(duopwm >= servMotorLeft)  duopwm = servMotorLeft;
      if(duopwm <= servMotorRight)  duopwm = servMotorRight;  
      
      LastErr = dErr;
/************************************************/
      if(lost_flag)
      {
        duopwm=last_duo;
        lost_flag=0;
      }
      
//      TIM_SetCompare1(TIM3,speed);
      TIM_SetCompare1(TIM4,duopwm);
      
      busy = false;
    }
  }
}
