#include "find.h" 
//返回偏差值，若整幅图像白点太少返回数据50
#define HIGH 59
#define DUAN 5 	//路线最少白点数
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
u8 picture=0;
float find(unsigned char zip[],int m) 
{
	short han[60]= {0},left,right, r=0;
	int i, j, k=0, e=0, a=0, end = 0;
	
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
			else if(han[i]>BIAO&&sum<0&&han[i]<END)
			//else if(han[i]>BIAO&&sum>0&&han[i]<END)
				
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
	//确认丢线
	if(a>20)
		lost_flag=1;
	//确认标线
	if(r>8)
		biao_flag_n=1;
	else
		biao_flag_n=0;
//	printf("stop?");
	//判断停止线和终点
	if(end>3)
	{	
//		printf("\n%d\n",end);
		if(stop_flag_n==0)
		{stop_flag_n =1,stop_flag = 1/*,printf("stop\n")*/;}
		else 
		{
//			printf("%d\n",stop_flag_n);
//			printf("end");
			stop_flag = 0;
			if(end > 20)
				end_flag = 1;
			else 
				end_flag = 0;
		}
	}	
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
	}
	sum = sum/k;
	
	//判断是否前瞻主导
//	if(a<15)
//		e = sum;
//	if(e-sum>5||sum-e>5)
		sum = e;
	picture++;
	if(picture<100)
		stop_flag_n = 0,stop_flag = 0;
	return sum;
}

/*#include "find.h" 
//返回偏差值，若整幅图像白点太少返回数据50
#define HIGH 59
#define DUAN 5 //路线最少白点数
#define BIAO 40//路线最多白点数，用于判断标志线<判断右侧>返回值最后-100
#define YOU 20
#define LOST 200//丢线时的白点数
#define END 3400//终点白点数

u8 biao_flag_l;
u8 biao_flag_n;
u8 lost_flag;
u8 end_flag=0;
u8 K=20;
float find(unsigned char zip[],int m) 
{
	short han[60]= {0},left,right, r=0;
	int i, j, k=0,n=0, e=0, a=0;
	float sum = 0;
//	for(i=0;i<600;i++)
//	scanf("%d",&zip[i]);
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
//					sum = ((j+1)*8-k) + sum;
					sum = ((j+1)*8-k) + sum-40;
				}
				zip[m] = zip[m]>>1;
			}
		}
		n=n+han[i];
		if(han[i]<DUAN)
		{
			han[i]=101;
//			if(i>=59)
//				han[i] = 40;
//				han[i] = 0;
//			else
//				han[i] = han[i+1];//+(han[i+1]-han[i+2]);
      a++;
		}
//else if(han[i]>BIAO&&i>40)
		 else if(han[i]>BIAO&&sum>0)
		 {han[i] = 0,r++;}
			//				han[i] = sum/han[i],han[i]=-han[i];
//    else if(han[i]>70)
//     e++;
		 else if(left>YOU&&sum<0)
        han[i]= sum/han[i]-15;
     else
			han[i] = sum/han[i];
		sum = 0, right = 0, left = 0;
	}
  if(a>20)
    lost_flag=1;
	if(r>8)
    biao_flag_n=1;
	else
		biao_flag_n=0;
	if(biao_flag_n - biao_flag_l>0)
  {
    end_flag++;
//    printf("%d\n",end_flag);
  }
    biao_flag_l = biao_flag_n;
	//	if(n<LOST)
//	{
//		sum = 0;
//		return sum;
//	}
//	if(n>END)
//	{
//    end_flag = 1;
//		sum = 0;
//		return sum;
//	}
	k = 0, sum = 0, a=0;
	for(i = 0;i<HIGH;i++)
	{
		if(han[i]<40)
		{
			sum=han[i]+sum;
			if(i<K)
			{
				a++;
				e = sum/a;
			}
//sum = han[i]*(int)(i/10)+sum,k=(int)(i/10)+k;
			k++;
		}
	}
	sum = sum/k;
	if(a<15)
		e = sum;
	if(e-sum>5||sum-e>5)
		sum = e;
	return sum;
}
*/