#include "find.h" 
//返回偏差值，若整幅图像白点太少返回数据50
#define HIGH 59
#define DUAN 3 	//路线最少白点数
#define BIAO 40	//路线最多白点数，用于判断标志线<判断右侧>返回值最后-100

//#define K 20	//前瞻分节
#define END 75	//终点白点数

//u8 biao_flag_l;
u8 biao_flag_n;
u8 lost_flag;
u8 end_flag=0;
u8 stop_flag_n = 0;
u8 stop_flag=0;
u8 K =20;			//前瞻分节
u8 all_flag = 0;
static u8 picture=0;
float find(unsigned char zip[],int m) 
{
	short han[60]= {0},left,right, r=0;
	int i, j, k=0, e=0, a=0, end = 0;
	static u32 all = 0;
	float sum = 0;
//	for(i=0;i<600;i++)
//	scanf("%d",&zip[i]);

	//对图像进行处理，读取特征值
	for(i=HIGH;i>=0;i--)
	{
		han[i] = 0;
		for(j=9;j>=0;j--)	
		{ 
			for(k=0;k<8;k++)
			{
				m=10*i+j;
				if(zip[m]-((zip[m]>>1)<<1))
				{
					if(j>4)
						right++;
					else
						left++;
					han[i]++;
					sum = ((j+1)*8-k) + sum-40;
					all++;
				}
				zip[m] = zip[m]>>1;
			}
		}
//		n=n+han[i];
		
		//判断该行是否断开
		if(han[i]<DUAN)
		{
			han[i]=-101;
			a++;
		}
			
			//判断标志线
		//	else if(han[i]>BIAO&&sum<0&&han[i]<END)
			else if(han[i]>BIAO&&sum>0&&han[i]<END)
				
			{han[i] = 0,r++;}

			//判断停止线
			else if(han[i]>END)
			{han[i] = 0,end++;
	//			printf("%d",han[i]);
			}
		
			//计算偏移量
		else
			han[i] = sum/han[i];
			
			sum = 0, right = 0, left = 0;
	}
	if(all>4000)
	{	
//		printf("%d		%d\n",all,stop_flag_n);
		all_flag = 1;
		TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	}
	//确认丢线
	if(a>53)
		lost_flag=1;
	//确认标线
	if(r>8)
		biao_flag_n=1;
	else
		biao_flag_n=0;
//	printf("stop?");
	//判断停止线和终点
	if(end>3&&end<8&&stop_flag_n==0)
	{	
//		printf("\n%d\n",end);
//		if(stop_flag_n==0)
//		{
////			stop_flag_n =1;
////				stop_flag = 1;
				TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
////			printf("stop\n");
//		}
//		else 
//		{
//			printf("%d\n",stop_flag_n);
//			printf("end");
//			stop_flag = 0;
//				end_flag = 1/*,printf("end\n")*/;

//		}
	}
	if(end > 20)
		TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);

	end = 0;
	//初始化数值
	k = 0, sum = 0, a=0;
	//纵向加权平均求总偏移量
	for(i = 0;i<HIGH;i++)
	{
		if(han[i]<40&&han[i]>-40)
		{
			sum=han[i]+sum;
			if(i<K)
			{
				a++;
				e = sum/a;
			}
			k++ ;
		}
//		 
	}
	sum = sum/k;
//	
	//判断是否前瞻主导
		sum = e;
//	printf("%d\n",picture);
	all = 0;
	if(picture<200)
		picture++,stop_flag_n = 0,stop_flag = 0;
//	printf("%d",stop_flag);
	return sum;
}
