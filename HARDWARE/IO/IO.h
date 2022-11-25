
#ifndef __BEEP_H
#define __BEEP_H
#include "sys.h"
#include "usart.h"
#include "delay.h"

#include "stm32f10x_tim.h"

#define LED PCout(13)

#define in5 PBout(5)
#define in6 PAout(12)

#define in8 PBout(0)
#define in9 PBout(1)

#define HW_PIN10_Left PCin(14)
#define HW_PIN11_Right PCin(15)

#define SR04 PAout(2)

void astronaut(void); //太空人
void kunkunstart(void);
void Encoder_Init_TIM3(u16 arr,u16 psc);   //读取编码器的数值
int Read_Encoder_TIM3(void);


void GPIO_Drive_Init(void); //IO初始化
void TIM4_PWM_Init(u16 arr, u16 psc); 

void GPIO_IFTracing_Init(void);   //红外初始化
void CAR_LRSpeed(u16 Lspeed,u16 Rspeed);
void CAR_Speed(u16 speed1,u16 speed2);
void CAR_Tracing(void);


void TIM1_STEngine_init(u16 arr, u16 psc);//舵机
void SR04_GPIO_Init(void);
void SR04_start(void);
void TIM2_Cap_init(u16 arr, u16 psc);
int Read_distance(void);

void car_go(void);
void car_back(void);
void car_stop(void);
void car_left(void);
void car_right(void);
#endif
