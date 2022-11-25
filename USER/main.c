
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "OLED_I2C.h"
//#include "KUNKUN.h"
//#include "astronaut.h"
#include "IO.h"



extern  u8 TIM5CH1_CAPTURE_STA; //输入捕获状态 
extern  u16 TIM5CH1_CAPTURE_VAL;//输入捕获值
int distance;
extern u16 speed;
void kunkunstart(void);
void astronaut(void);
unsigned char i;
int main(void)
{

	



	delay_init(); //延时函数初始化 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设 置 NVIC 中 断 分 组 2
	uart_init(9600); //串口初始化波特率为 115200

	GPIO_Drive_Init();  
	SR04_GPIO_Init();
	TIM4_PWM_Init(7199,0); //72/7200
	GPIO_IFTracing_Init();  //红外初始化
	Encoder_Init_TIM3(0xffff,0); //编码器
	TIM1_STEngine_init(999,1439);//舵机
	TIM2_Cap_init(0XFFFF,72-1); //以 1Mhz 的频率计数
	TIM_SetCompare1(TIM1,75);
	I2C_Configuration();
	OLED_Init();
		OLED_CLS();
	astronaut();

	OLED_CLS();
	
	while(1)
{

	kunkunstart();
	//astronaut();
//OLED_Fill(0xFF);//全屏点亮
//		delay_ms(2000);
//		OLED_Fill(0x00);//全屏灭
//		delay_ms(2000);
//		for(i=0;i<5;i++)
//		{
//			OLED_ShowCN(22+i*16,0,i);//测试显示中文
//		}
		//delay_ms(2000);;
		//OLED_ShowStr(0,2,"HelTec Automation",1);//测试6*8字符
		//OLED_ShowStr(0,4,"Hello Tech",2);				//测试8*16字符
//			delay_ms(2000);;
//			OLED_CLS();//清屏
//		OLED_OFF();//测试OLED休眠
//		delay_ms(2000);;
//		OLED_ON();//测试OLED休眠后唤醒
		//OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//测试BMP位图显示
		//delay_ms(2000);;

//		SR04_start();

//		printf("%d",Read_distance());
		

//		
//		delay_ms(1500); //每隔200ms读取一次编码器计数，即速度。
		               //可以使用定时中断实现更精准的速度计算，用户可自定义
    //printf("%d %d %d %d",Read_Encoder_TIM3(),speed,left_ps,right_ps);
		
			
		
		// 125 100 75 50 25
		//TIM_SetCompare1(TIM1,75);
//	delay_ms(20000);
//			TIM_SetCompare1(TIM1,50);
//	delay_ms(20000);
//		TIM_SetCompare1(TIM1,75);
//	delay_ms(20000);
//		TIM_SetCompare1(TIM1,100);
//	delay_ms(20000);
//		TIM_SetCompare1(TIM1,125);
//	delay_ms(20000);
	}
}

//void kunkunstart(void)
//{
//int i=0;
//	for(;i<39;i++){
//	delay_ms(190);
//	switch(i)
//		{
//			case 1: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP1);break;
//			case 2: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP2);break;
//			case 3: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP3);break;
//			case 4: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP4);break;
//			case 5: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP5);break;
//			case 6: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP6);break;
//			case 7: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP7);break;
//			case 8: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP8);break;
//			case 9: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP9);break;
//			case 10: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP10);break;
//			case 11: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP11);break;
//			case 12: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP12);break;
//			case 13: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP13);break;
//			case 14: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP14);break;
//			case 15: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP15);break;
//			case 16: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP16);break;
//			case 17: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP17);break;
//			case 18: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP18);break;
//			case 19: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP19);break;
//			case 20: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP20);break;
//			case 21: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP21);break;
//			case 22: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP22);break;
//			case 23: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP23);break;
//			case 24: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP24);break;
//			case 25: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP25);break;
//			case 26: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP26);break;
//			case 27: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP27);break;
//			case 28: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP28);break;
//			case 29: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP29);break;
//			case 30: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP30);break;
//			case 31: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP31);break;
//			case 32: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP32);break;
//			case 33: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP33);break;
//			case 34: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP34);break;
//			case 35: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP35);break;
//			case 36: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP36);break;
//			case 37: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP37);break;
//			case 38: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP38);break;
//		}
//	}

//}
//void astronaut(void)
//{


//	j++;
//	if(j>10)
//	{
//		j=1;
//	}
//	delay_ms(150);
//	switch(j)
//		{
//			case 1: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP40);break;
//			case 2: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP41);break;
//			case 3: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP42);break;
//			case 4: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP43);break;
//			case 5: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP44);break;
//			case 6: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP45);break;
//			case 7: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP46);break;
//			case 8: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP47);break;
//			case 9: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP48);break;
//			case 10: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP49);break;
//			
//		}
//}

