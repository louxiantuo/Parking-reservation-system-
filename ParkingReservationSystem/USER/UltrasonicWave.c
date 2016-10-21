/***************** (C) 1209 Lab **************************
 * �ļ���  : UltrasonicWave.c
 * ����    �����������ģ�飬UltrasonicWave_Configuration��������
             ��ʼ������ģ�飬UltrasonicWave_StartMeasure��������
			 ������࣬������õ�����ͨ������1��ӡ����         
 * ʵ��ƽ̨��Mini STM32������  STM32F103RBT6
 * Ӳ�����ӣ�------------------
 *          | PC8  - TRIG      |
 *          | PC7  - ECHO      |
 *           ------------------
 * ��汾  ��ST3.5.0
 *
 * ����    ��Lee 
*********************************************************************************/

#include "UltrasonicWave.h"
#include "usart.h"
#include "usart2.h"
#include "timer.h"
#include "delay.h"
#include "wifi.h"
#include "beep.h"
#include "sys.h"
#include "wifi.h"
#include "exti.h" 

extern int hcsr1;//0��ʾû����1��ʾ�г�
extern int hcsr2;
extern int hcsr3;

int mark[3]={0},num[3]={0};

#define	TRIG_PORT      GPIOC		//TRIG       
#define	ECHO_PORT      GPIOC		//ECHO 
#define	TRIG_PIN       GPIO_Pin_8   //TRIG       
#define	ECHO_PIN       GPIO_Pin_7	//ECHO   

   


/*
 * ��������UltrasonicWave_Configuration
 * ����  ��������ģ��ĳ�ʼ��
 * ����  ����
 * ���  ����	
 */
void UltrasonicWave_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//�ر�jtag
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
   
  GPIO_InitStructure.GPIO_Pin = TRIG_PIN;					 //PC8��TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(TRIG_PORT, &GPIO_InitStructure);	                 //��ʼ������GPIO 

	
  GPIO_InitStructure.GPIO_Pin = ECHO_PIN;				     //PC7��ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 //��Ϊ����
  GPIO_Init(ECHO_PORT,&GPIO_InitStructure);				 //��ʼ��GPIOC
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;					 //PC11��TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//��Ϊ�������ģʽ
	GPIO_Init(GPIOC, &GPIO_InitStructure);	                 //��ʼ������GPIO 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12	;				     //PC12��ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 //��Ϊ����
  GPIO_Init(GPIOC,&GPIO_InitStructure);						 //��ʼ��GPIOC
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;					 //PC0��TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//��Ϊ�������ģʽ
	GPIO_Init(GPIOC, &GPIO_InitStructure);	                 //��ʼ������GPIO 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1	;				     //PC1��ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 //��Ϊ����
  GPIO_Init(GPIOC,&GPIO_InitStructure);						 //��ʼ��GPIOC
	
	 //GPIOC.7	  �ж����Լ��жϳ�ʼ������
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource7);

		EXTI_InitStructure.EXTI_Line=EXTI_Line7;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
		
						 //GPIOC.12	  �ж����Լ��жϳ�ʼ������
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource12);
		EXTI_InitStructure.EXTI_Line=EXTI_Line12;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
			
						 //GPIOC.10	  �ж����Լ��жϳ�ʼ������
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource1);
		EXTI_InitStructure.EXTI_Line=EXTI_Line1;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
				
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//�����ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
		
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//�����ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
		NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//�����ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ�
		NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
		
		
			
}


void EXTI9_5_IRQHandler(void)
{
	float UltrasonicWave_Distance;      //������ľ��� 
	delay_us(10);		                      //��ʱ10us
     if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
			TIM_SetCounter(TIM2,0);
			TIM_Cmd(TIM2, ENABLE);                                             //����ʱ��
		
			while(GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN));	                 //�ȴ��͵�ƽ

			TIM_Cmd(TIM2, DISABLE);			                                 //��ʱ��2ʧ��
			UltrasonicWave_Distance=TIM_GetCounter(TIM2)*2*5*34/200.0;									 //�������&&UltrasonicWave_Distance<150
		
	if(UltrasonicWave_Distance>0)
	{
		num[0]++;
		if(UltrasonicWave_Distance<50)mark[0]++;
		while(num[0]>=10)
		{
			num[0]=0;
			if(mark[0]>7)
				hcsr1 = 1;
				//UpLoad(send_cmd,&buf);;
			else 
				hcsr1 = 0;
				//{u2_printf("status hcsr 1 1\n");sprintf((char*)hcsr_1,"status hcsr 1 0\n");}
			mark[0]=0;
		}
	
//	printf("distance1:%f cm\t",UltrasonicWave_Distance);
	
		
	}
	EXTI_ClearITPendingBit(EXTI_Line7);  //���EXTI7��·����λ
}
}

void EXTI15_10_IRQHandler(void)
{
	float UltrasonicWave_Distance;      //������ľ��� 
	delay_us(10);		//��ʱ10us
  if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{
			TIM_SetCounter(TIM2,0);
			TIM_Cmd(TIM2, ENABLE); 		//����
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12));		//�ȴ��͵�ƽ
			TIM_Cmd(TIM2, DISABLE);			                                 //��ʱ��3ʧ��
			UltrasonicWave_Distance=TIM_GetCounter(TIM2)*2*5*34/200.0;									 //�������&&UltrasonicWave_Distance<150

		num[1]++;
		if(UltrasonicWave_Distance<50)mark[1]++;
		while(num[1]>=10)
		{
			num[1]=0;
			if(mark[1]>7)
				hcsr2 = 1;
			else 
				hcsr2 = 0;
			//u2_printf("status hcsr 2 1\n");
			mark[1]=0;
		}
	
//	printf("distance2:%f cm\t",UltrasonicWave_Distance);
	
		
	
		EXTI_ClearITPendingBit(EXTI_Line12);  //���EXTI12��·����λ
	}
}


void EXTI1_IRQHandler(void)
{

	float UltrasonicWave_Distance;	//������ľ��� 
	
	delay_us(10);		//��ʱ10us
     if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
			TIM_SetCounter(TIM2,0);
			TIM_Cmd(TIM2, ENABLE); 		//����
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1));		//�ȴ��͵�ƽ
			TIM_Cmd(TIM2, DISABLE);					//��ʱ��3ʧ��
			UltrasonicWave_Distance=TIM_GetCounter(TIM2)*2*5*34/200.0;									 //�������&&UltrasonicWave_Distance<150
	if(UltrasonicWave_Distance>0)
	{
		num[2]++;
		if(UltrasonicWave_Distance<50)mark[2]++;
		while(num[2]>=10)
		{
			num[2]=0;
			if(mark[2]>7)
					hcsr3 = 1;
			//UpLoad(status_hcsr_1_0,&buf);
			else 
					hcsr3 = 0;
			//u2_printf("status hcsr 3 1\n");
			mark[2]=0;
		}
	
//	printf("distance3:%f cm\t",UltrasonicWave_Distance);
	
		
	}
	
	
	EXTI_ClearITPendingBit(EXTI_Line1);  //���EXTI12��·����λ
	}

}

/*
 * ��������UltrasonicWave_StartMeasure
 * ����  ����ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��
 * ����  ����
 * ���  ����	
 */
void UltrasonicWave_StartMeasure(void)
{
	
	
		GPIO_SetBits(TRIG_PORT,TRIG_PIN); 	//��>10US�ĸߵ�ƽ�TRIG_PORT,TRIG_PIN��������define����?
		delay_us(20);		                      //��ʱ20US
		GPIO_ResetBits(TRIG_PORT,TRIG_PIN);
		delay_ms(200);
		GPIO_SetBits(GPIOC,GPIO_Pin_11); 		  //��>10US�ĸߵ�ƽ�TGPIOC,GPIO_Pin_11��������define����?
		delay_us(20);		                     //��ʱ20US
		GPIO_ResetBits(GPIOC,GPIO_Pin_11);
		delay_ms(200);                                     
		GPIO_SetBits(GPIOC,GPIO_Pin_0); 		  //��>10US�ĸߵ�ƽ�TGPIOC,GPIO_Pin_1��������define����?
		delay_us(20);		                     //��ʱ20US
		GPIO_ResetBits(GPIOC,GPIO_Pin_0);
		delay_ms(200);
	
}

void relay_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO, ENABLE);
  GPIO_StructInit(&GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode	=	GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Pin	=	GPIO_Pin_0;
  GPIO_InitStruct.GPIO_Speed	=	GPIO_Speed_2MHz;
  GPIO_Init(GPIOE,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode	=	GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Pin	=	GPIO_Pin_1;
  GPIO_InitStruct.GPIO_Speed	=	GPIO_Speed_2MHz;
  GPIO_Init(GPIOE,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode	=	GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Pin	=	GPIO_Pin_2;
  GPIO_InitStruct.GPIO_Speed	=	GPIO_Speed_2MHz;
  GPIO_Init(GPIOE,&GPIO_InitStruct);
	  GPIO_InitStruct.GPIO_Mode	=	GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Pin	=	GPIO_Pin_3;
  GPIO_InitStruct.GPIO_Speed	=	GPIO_Speed_2MHz;
  GPIO_Init(GPIOE,&GPIO_InitStruct);
	GPIO_SetBits(GPIOE,GPIO_Pin_0);
	GPIO_SetBits(GPIOE,GPIO_Pin_1);
	GPIO_SetBits(GPIOE,GPIO_Pin_2);
	GPIO_SetBits(GPIOE,GPIO_Pin_3);

	
}

void relay_open(int switch_open)
{
		switch(switch_open)
		{
			case 0:GPIO_ResetBits(GPIOE,GPIO_Pin_0);break;
			case 1:GPIO_ResetBits(GPIOE,GPIO_Pin_1);break;
			case 2:GPIO_ResetBits(GPIOE,GPIO_Pin_2);break;
			case 3:GPIO_ResetBits(GPIOE,GPIO_Pin_3);break;

		}

}
void relay_close(int switch_close)
{
		switch(switch_close)
		{
			case 0:GPIO_SetBits(GPIOE,GPIO_Pin_0);break;
			case 1:GPIO_SetBits(GPIOE,GPIO_Pin_1);break;
			case 2:GPIO_SetBits(GPIOE,GPIO_Pin_2);break;
			case 3:GPIO_SetBits(GPIOE,GPIO_Pin_3);break;
			
		}
}


/******************* (C) 1209 Lab *****END OF FILE************/
