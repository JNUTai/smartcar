#include "main.h"

#define servMotorCenture  1522 //舵机中心位置
#define servMotorLeft     servMotorCenture+130  //舵机左极限166
#define servMotorRight    servMotorCenture-140 //舵机右极限69


//uint32_t Task_Delay[NumOfTask];
extern u8 lost_flag;
extern u8 end_flag;
extern u8 stop_flag_active;
extern u8 K;
uint8_t pic[OV7725_DATASIZE];
bool busy = false;
float LastErr=0,dErr=0;
float pPram = 10;          //比例系数4  
float dPram = 110;            //微分系7
float ctl;
uint32_t speed=1700;


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
  uint32_t duopwm = servMotorCenture;//5~25对应0~180度
  uint32_t last_duo=servMotorCenture;
	uint32_t i=0,j=0;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
  Debug_USART_Config();
  if(OV7725_Init())
    printf("Camera init failed\r\n");
  else
  {
    OV7725_Callback_Install(ov7725_isr);
    printf("Done!\r\n");
  }
	
  TIM3_PWM_Init(5000-1,84-1);  //电机定时器200Hz
  TIM4_PWM_Init(servMotorCenture);  //舵机定时器50Hz
	TIM5_PWM_Init();
  TIM_SetCompare1(TIM3,1500);
  Delay_ms(100);

	for(i=0;i<150;i++)
	if(busy)
		busy =false;
  while(1)
  {
    if(busy)
    {
/*******************这里是串口发送图像********************
      putchar(0x01);
      putchar(0xFE);
      for(i = 0; i < OV7725_DATASIZE; i++)
        putchar(pic[i]);
      putchar(0xFE);
      putchar(0x01);
********************************************************/
      
      ctl=find(pic,OV7725_DATASIZE);
     
/***************这里是舵机PD控制*******************/     
      dErr = (float)ctl;
      last_duo=duopwm;
      duopwm = servMotorCenture - (int)(dErr*pPram + (dErr-LastErr)*dPram)  ;  //差分表示微分项
			speed = 2000 - abs(servMotorCenture - duopwm)*2;        //速度与舵机转角相关
//			speed = 2000 - abs((int)ctl*15);
			if(speed<1650) speed = 1650;
			if(duopwm > servMotorLeft)  duopwm = servMotorLeft;
      if(duopwm < servMotorRight)  duopwm = servMotorRight;  
      LastErr = dErr;
/************************************************/
      if(lost_flag)
      {
        duopwm=last_duo;
        lost_flag=0;
      }
      //stop
      if(stop_flag_active)
      {
			//	printf("stop\n");
				TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
				TIM_Cmd(TIM3, DISABLE);
				USART_ITConfig(DEBUG_USART,USART_IT_RXNE,DISABLE);
//				TIM_SetCompare1(TIM3, 1200);
						stop_flag_active = 0;
			}
			
			if(end_flag)
      {
		//		printf("end\n");
				TIM_Cmd(TIM3, DISABLE);
      }
		//	printf("%d\n",speed);			
		//		TIM_SetCompare1(TIM3,1700);
      
			TIM_SetCompare1(TIM3,speed);
      TIM_SetCompare1(TIM4,duopwm);
 //     printf("%d\n",duopwm);
      busy = false;
    }
  }
}
