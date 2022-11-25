#include "IO.h"
#include "OLED_I2C.h"
#include "astronaut.h"
#include "KUNKUN.h"

void GPIO_Drive_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	//ʹ��IO�˿ڵ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	GPIO_Init(GPIOB,&GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_ResetBits(GPIOB,GPIO_Pin_9);//PB4����C��ƽ
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	GPIO_Init(GPIOB,&GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_ResetBits(GPIOA,GPIO_Pin_12);//PB4����͵�ƽ
	GPIO_Init(GPIOA,&GPIO_InitStructure);


	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	GPIO_Init(GPIOC,&GPIO_InitStructure);//��ʼ��

}

void TIM4_PWM_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //��ʹ�ܶ�ʱ�� 2 ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	
	//���ø�����Ϊ�����������,��� TIM3 CH1 �� PWM ���岨�� GPIOA.1|2
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
		//��ʼ�� TIM2
		TIM_TimeBaseStructure.TIM_Period = arr; //�������Զ���װ������ֵ
		TIM_TimeBaseStructure.TIM_Prescaler =psc; //����Ԥ��Ƶֵ
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���ģʽ
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //�۳�ʼ�� TIMx

		//��ʼ�� TIM2 Channel2 PWM ģʽ
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ�� PWM ģʽ 2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�
		
		TIM_OC1Init(TIM4, &TIM_OCInitStructure); //�ܳ�ʼ������ TIM2 OC2
		TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���
		
		
		TIM_OC2Init(TIM4, &TIM_OCInitStructure); //�ܳ�ʼ������ TIM2 OC4
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���
		
		
		TIM_Cmd(TIM4, ENABLE); //��ʹ�� TIM2 

}
void CAR_Speed(u16 speed1,u16 speed2)
{
	TIM_SetCompare1(TIM4,speed2);
	TIM_SetCompare2(TIM4,speed1);
	
}



void Encoder_Init_TIM3(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure; //����һ�����ų�ʼ���Ľṹ��  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//����һ����ʱ����ʼ���Ľṹ��
  TIM_ICInitTypeDef TIM_ICInitStructure; //����һ����ʱ��������ģʽ��ʼ���Ľṹ��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʹ��TIM4ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��CPIOBʱ��
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//PB6��PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//����GPIO_InitStructure�Ĳ�����ʼ��GPIOB0

	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc; // Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ѡ��ʱ�ӷ�Ƶ������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct�Ĳ�����ʼ����ʱ��TIM4
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3��CH1��CH2ͬʱ�������ķ�Ƶ
	TIM_ICStructInit(&TIM_ICInitStructure); //��TIM_ICInitStruct �е�ÿһ��������ȱʡֵ����
	TIM_ICInitStructure.TIM_ICFilter = 10;  //�����˲�������
	TIM_ICInit(TIM3, &TIM_ICInitStructure); //��TIM_ICInitStructure������ʼ����ʱ��TIM4������ģʽ
	
	TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��4
}

//��ȡ����������
int Read_Encoder_TIM3(void)
{
	int Encoder_TIM;
	Encoder_TIM=TIM3->CNT; //��ȡ����
	if(Encoder_TIM>0xefff)Encoder_TIM=Encoder_TIM-0xffff; //ת������ֵΪ�з����ֵ������0��ת��С��0��ת��
	                                                      //TIM4->CNT��ΧΪ0-0xffff����ֵΪ0��
	TIM3->CNT=0; //��ȡ����������
	return Encoder_TIM; //����ֵ
}




void SR04_GPIO_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);//��ʼ��
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
}

//��ʱ�� 2 ͨ�� 2 ���벶������
TIM_ICInitTypeDef TIM2_ICInitStructure;
void TIM2_Cap_init(u16 arr,u16 psc)
{
			GPIO_InitTypeDef GPIO_InitStructure;
			TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
			NVIC_InitTypeDef NVIC_InitStructure;
	
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //��ʹ�� TIM2 ʱ��
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //��ʹ�� GPIOA ʱ��
			
	//��ʼ�� GPIOA.0 ��
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //PA0 ���� 
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ���� 
			GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ�� GPIOA.0
			GPIO_ResetBits(GPIOA,GPIO_Pin_1); //PA0 ����
			
	
	//�ڳ�ʼ�� TIM5 ����
			TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ
			TIM_TimeBaseStructure.TIM_Prescaler =psc; //Ԥ��Ƶ�� 
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // TDTS = Tck_tim
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���ģʽ
			TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //��ʼ�� TIMx
			
	//�۳�ʼ�� TIM5 ���벶��ͨ�� 1
			TIM2_ICInitStructure.TIM_Channel = TIM_Channel_2; //ѡ������� IC1 ӳ�䵽 TI1 ��
			TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //�����ز���
			TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽 TI1 ��
			TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //���������Ƶ,����Ƶ
			TIM2_ICInitStructure.TIM_ICFilter = 0x00; //IC1F=0000 ���������˲��� ���˲�
			TIM_ICInit(TIM2, &TIM2_ICInitStructure); //��ʼ�� TIM5 ���벶��ͨ�� 1
			
			//�ݳ�ʼ�� NVIC �ж����ȼ�����
			NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //TIM3 �ж�
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ� 2 ��
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ� 0 ��
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ����ʹ��
			NVIC_Init(&NVIC_InitStructure); //��ʼ�� NVIC
			
			TIM_ITConfig( TIM2,TIM_IT_Update|TIM_IT_CC2,ENABLE);//����������жϲ����ж�
			TIM_Cmd(TIM2,ENABLE ); //��ʹ�ܶ�ʱ�� 5
}

u8 TIM5CH1_CAPTURE_STA=0; //���벶��״̬ 
u16 TIM5CH1_CAPTURE_VAL;//���벶��ֵ
//�ݶ�ʱ�� 5 �жϷ������
void TIM2_IRQHandler(void)
{ 
if((TIM5CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
{ 
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{	 
				if(TIM5CH1_CAPTURE_STA&0X40) //�Ѿ����񵽸ߵ�ƽ
					{
					if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
						{
								TIM5CH1_CAPTURE_STA|=0X80; //��ǳɹ�������һ��
								TIM5CH1_CAPTURE_VAL=0XFFFF;
						}
						else TIM5CH1_CAPTURE_STA++;

					}
	}

	if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET) //���� 1 ���������¼�
	{
			if(TIM5CH1_CAPTURE_STA&0X40) //����һ���½���
			{ 
				TIM5CH1_CAPTURE_STA|=0X80; //��ǳɹ�����һ��������
				TIM5CH1_CAPTURE_VAL=TIM_GetCapture2(TIM2);
				TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising); //����Ϊ�����ز���
			}else //��δ��ʼ,��һ�β���������
			{			
				TIM5CH1_CAPTURE_STA=0; //���
				TIM5CH1_CAPTURE_VAL=0;
				TIM_SetCounter(TIM2,0);
				TIM5CH1_CAPTURE_STA|=0X40; //��ǲ�����������
				TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling); //����Ϊ�½��ز���
			} 
		} 
 
	} 
TIM_ClearITPendingBit(TIM2, TIM_IT_CC2|TIM_IT_Update); //����жϱ�־λ

}

void SR04_start(void)
{
		delay_ms(200);
		SR04 = 0;
		delay_us(10);
		SR04 = 1;	

}
int Read_distance(void){
	
		int time,distance;
			if(TIM5CH1_CAPTURE_STA&0X80)//�ɹ�������һ��������
		{
				time=TIM5CH1_CAPTURE_STA&0X3F;
				time*=65536;//���ʱ���ܺ�
				time+=TIM5CH1_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
				distance=time*0.034/2;
				TIM5CH1_CAPTURE_STA=0; //������һ�β���
			return distance;
		}
	return 0;


}




void TIM1_STEngine_init(u16 arr, u16 psc){ //���
	
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_OCInitTypeDef TIM_OCInitStructure;

	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	

	//���ø�����Ϊ�����������,��� TIM1 CH1 �� PWM ���岨��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM1_CH1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //���ù������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ�� GPIO
	
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
			
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CH1 PWM2 ģʽ
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //OC1 �͵�ƽ��Ч
		TIM_OC1Init(TIM1, &TIM_OCInitStructure); //����ָ���Ĳ�����ʼ������ TIMx
		
		TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); //CH1 Ԥװ��ʹ��
		
		TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ�� TIMx �� ARR �ϵ�Ԥװ�ؼĴ���
		TIM_CtrlPWMOutputs(TIM1,ENABLE); //MOE �����ʹ��,�߼���ʱ�����뿪��
		TIM_Cmd(TIM1,ENABLE);
	
}


void GPIO_IFTracing_Init(void){   //����
	GPIO_InitTypeDef  GPIO_InitStructure;
	//ʹ��IO�˿ڵ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;  //����Ĭ����������
	GPIO_ResetBits(GPIOC,GPIO_Pin_14);
	GPIO_Init(GPIOC,&GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;   
	GPIO_ResetBits(GPIOC,GPIO_Pin_15);
	GPIO_Init(GPIOC,&GPIO_InitStructure);//��ʼ��

}




void CAR_Tracing(void){

	if(HW_PIN10_Left==0&&HW_PIN11_Right==1){
				car_right();
			
		}	
		if(HW_PIN10_Left==1&&HW_PIN11_Right==0){
				car_left();
		
		}
		if(HW_PIN10_Left==0&&HW_PIN11_Right==0){
				car_go();
		
		}
		if(HW_PIN10_Left==1&&HW_PIN11_Right==1){
				car_stop();	
		}

}

void car_go(void)
{
	in8=0;
	in9=1;
	in5=1;
	in6=0;	 
}
void car_back(void){
	in8=1;
	in9=0;
	in5=0;
	in6=1;
}
void car_stop(void)
{
	in8=0;
	in9=0;
	in5=0;
	in6=0;
}

void car_left(void)
{
	in8=1;
	in9=0;
	in5=1;
	in6=0;
	TIM_SetCompare1(TIM3,1500);
}
	

void car_right(void)
{
	in8=0;
	in9=1;
	in5=0;
	in6=1;
	TIM_SetCompare2(TIM3,1500);
	
}
void astronaut(void)
{
int j=0;

	for(;j<10;j++){
		delay_ms(180);
	switch(j)
		{
			case 1: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP40);break;
			case 2: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP41);break;
			case 3: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP42);break;
			case 4: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP43);break;
			case 5: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP44);break;
			case 6: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP45);break;
			case 7: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP46);break;
			case 8: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP47);break;
			case 9: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP48);break;
			case 10: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP49);break;
			
		}
	}
}

void kunkunstart(void)
{
int i=0;
	for(;i<39;i++){
	delay_ms(190);
	switch(i)
		{
			case 1: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP1);break;
			case 2: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP2);break;
			case 3: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP3);break;
			case 4: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP4);break;
			case 5: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP5);break;
			case 6: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP6);break;
			case 7: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP7);break;
			case 8: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP8);break;
			case 9: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP9);break;
			case 10: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP10);break;
			case 11: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP11);break;
			case 12: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP12);break;
			case 13: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP13);break;
			case 14: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP14);break;
			case 15: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP15);break;
			case 16: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP16);break;
			case 17: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP17);break;
			case 18: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP18);break;
			case 19: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP19);break;
			case 20: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP20);break;
			case 21: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP21);break;
			case 22: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP22);break;
			case 23: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP23);break;
			case 24: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP24);break;
			case 25: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP25);break;
			case 26: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP26);break;
			case 27: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP27);break;
			case 28: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP28);break;
			case 29: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP29);break;
			case 30: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP30);break;
			case 31: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP31);break;
			case 32: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP32);break;
			case 33: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP33);break;
			case 34: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP34);break;
			case 35: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP35);break;
			case 36: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP36);break;
			case 37: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP37);break;
			case 38: OLED_DrawBMP(0,0,64,8,(unsigned char*)BMP38);break;
		}
	}

}
