#include "sys.h"
#include "usart.h"
#include "IO.h"
#include "delay.h"
#include <string.h>
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
#define Max_BUFF_Len 18

unsigned char Uart2_Buffer[Max_BUFF_Len];

unsigned int Uart2_Rx=0;
void uart_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
	
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

#include "OLED_I2C.h"
u16 speed=2300,stFlag=0;
extern uint8_t uart_flag;                     //���ձ�־
extern vu8 RX_lanya[12];
void USART1_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;

#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res=USART_ReceiveData(USART1);	//��ȡ���յ�������
		delay_init();
		switch(Res)
		{
				case 'G':
				{
					CAR_Speed(speed,speed-40);
					car_go();	
					stFlag=1;
				}
			
				break;
				case 'H':
				{
					
					CAR_Speed(2700,speed);
					car_go();	
					stFlag=2;
						
				}
				break;
				case 'J':
				{

					CAR_Speed(speed,2700);
					car_go();	
					stFlag=3;
				
				}
				break;
				case 'K':
				{
					CAR_Speed(speed,speed);
					car_back();					
					stFlag=4;			
				}
		
				break;
				case 'B':
				{

					CAR_Speed(2100,2080);
					delay_ms(100);
					OLED_CLS();
					while(1){
						//astronaut();
						CAR_Tracing();
						if(USART_ReceiveData(USART1)=='I') 
						{ 
							car_stop();
							break;
						}
					}
					
				}	
				break;
			case 'A':
			{
				int left_ps=0,right_ps=0 ,dis=33;
				TIM_SetCompare1(TIM1,75);
				CAR_Speed(2400,2400);
			  OLED_CLS();
				while(1){
				SR04_start();
				delay_ms(100);
				if(Read_distance()>dis){
					car_go();
				}else
				{
					
					car_stop();
					TIM_SetCompare1(TIM1,43);
					delay_ms(350);
					SR04_start();
					delay_ms(100);
					right_ps=Read_distance();
					delay_ms(100);
					TIM_SetCompare1(TIM1,98);
					delay_ms(35��0);
					SR04_start();
					delay_ms(100);
					left_ps=Read_distance();
					TIM_SetCompare1(TIM1,75);
					if(right_ps<left_ps&& left_ps>dis)
					{
						car_left();
						delay_ms(610);
						car_stop();
					}
					else if(right_ps>left_ps&& right_ps>dis)
					{
						car_right();
						delay_ms(610);
						car_stop();
						
					}else
					{
						
						car_back();
						delay_ms(610);
						car_stop();
						if(right_ps<left_ps)
						{
						
						car_left();
						delay_ms(520);
						car_stop();
						}else
						{
						car_right();
						delay_ms(520);
						car_stop();
						
						}
					
					}
					
				}
				if(USART_ReceiveData(USART1)=='I') {
					TIM_SetCompare1(TIM1,75);
					car_stop();
					break;}
				}
			}
			break;
			
			case 'I':
			{
				car_stop();
				stFlag=0;
			}
				break;
			case 'D':
			{
				if(speed>0){
				speed-=100;
				printf("speed UP");	
				switch(stFlag){
				case 1: CAR_Speed(speed,speed+80); break;
				case 2: CAR_Speed(2700,speed); break;	
				case 3: CAR_Speed(speed,2700); break;
				case 4: CAR_Speed(speed,speed+80);break;
				}
				}
				if(speed==0)
				{
					printf("Speed MAX");
				
				}
				

			}
				break;
			case 'E':
			{
				if(speed<2500){
				speed+=100;
				printf("speed DOWN");	
				switch(stFlag){
				case 1: CAR_Speed(speed,speed+80); break;
				case 2: CAR_Speed(2700,speed); break;	
				case 3: CAR_Speed(speed,2700); break;
				case 4: CAR_Speed(speed,speed+80);break;
				}
				}
				if(speed==2500)
				{
			
					printf("Speed MIN");
				}			
			}
				break;	
			case 'C':{
					u16 dis=0;
				CAR_Speed(2400,2400);
					while(1)
					{
						SR04_start();
						delay_ms(100);
						dis=Read_distance();
						if(dis<25&&dis>5)
						{
							car_stop();
						
						}
						else if(dis<5)
						{
						car_back();
						}else
						{
						car_go();
						}
						
				if(USART_ReceiveData(USART1)=='I') {
					car_stop();
					break;}
				
				}
				
				}
				break;
			case 'F':
						astronaut();
						OLED_CLS();
				break;
		}
		
	
     } 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 
	
#endif



