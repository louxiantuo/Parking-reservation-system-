/***************** (C) 1209 Lab **************************
 * ÎÄ¼şÃû  : UltrasonicWave.c
 * ÃèÊö    £º³¬Éù²¨²â¾àÄ£¿é£¬UltrasonicWave_Configuration£¨£©º¯Êı
             ³õÊ¼»¯³¬ÉùÄ£¿é£¬UltrasonicWave_StartMeasure£¨£©º¯Êı
			 Æô¶¯²â¾à£¬²¢½«²âµÃµÄÊı¾İÍ¨¹ı´®¿Ú1´òÓ¡³öÀ´         
 * ÊµÑéÆ½Ì¨£ºMini STM32¿ª·¢°å  STM32F103RBT6
 * Ó²¼şÁ¬½Ó£º------------------
 *          | PC8  - TRIG      |
 *          | PC7  - ECHO      |
 *           ------------------
 * ¿â°æ±¾  £ºST3.5.0
 *
 * ×÷Õß    £ºLee 
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

extern int hcsr1;//0±íÊ¾Ã»³µ£¬1±íÊ¾ÓĞ³µ
extern int hcsr2;
extern int hcsr3;

int mark[3]={0},num[3]={0};

#define	TRIG_PORT      GPIOC		//TRIG       
#define	ECHO_PORT      GPIOC		//ECHO 
#define	TRIG_PIN       GPIO_Pin_8   //TRIG       
#define	ECHO_PIN       GPIO_Pin_7	//ECHO   

   


/*
 * º¯ÊıÃû£ºUltrasonicWave_Configuration
 * ÃèÊö  £º³¬Éù²¨Ä£¿éµÄ³õÊ¼»¯
 * ÊäÈë  £ºÎŞ
 * Êä³ö  £ºÎŞ	
 */
void UltrasonicWave_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//¹Ø±Õjtag
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
   
  GPIO_InitStructure.GPIO_Pin = TRIG_PIN;					 //PC8½ÓTRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //ÉèÎªÍÆÍìÊä³öÄ£Ê½
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(TRIG_PORT, &GPIO_InitStructure);	                 //³õÊ¼»¯ÍâÉèGPIO 

	
  GPIO_InitStructure.GPIO_Pin = ECHO_PIN;				     //PC7½ÓECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 //ÉèÎªÊäÈë
  GPIO_Init(ECHO_PORT,&GPIO_InitStructure);				 //³õÊ¼»¯GPIOC
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;					 //PC11½ÓTRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//ÉèÎªÍÆÍìÊä³öÄ£Ê½
	GPIO_Init(GPIOC, &GPIO_InitStructure);	                 //³õÊ¼»¯ÍâÉèGPIO 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12	;				     //PC12½ÓECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 //ÉèÎªÊäÈë
  GPIO_Init(GPIOC,&GPIO_InitStructure);						 //³õÊ¼»¯GPIOC
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;					 //PC0½ÓTRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//ÉèÎªÍÆÍìÊä³öÄ£Ê½
	GPIO_Init(GPIOC, &GPIO_InitStructure);	                 //³õÊ¼»¯ÍâÉèGPIO 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1	;				     //PC1½ÓECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 //ÉèÎªÊäÈë
  GPIO_Init(GPIOC,&GPIO_InitStructure);						 //³õÊ¼»¯GPIOC
	
	 //GPIOC.7	  ÖĞ¶ÏÏßÒÔ¼°ÖĞ¶Ï³õÊ¼»¯ÅäÖÃ
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource7);

		EXTI_InitStructure.EXTI_Line=EXTI_Line7;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//¸ù¾İEXTI_InitStructÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèEXTI¼Ä´æÆ÷
		
						 //GPIOC.12	  ÖĞ¶ÏÏßÒÔ¼°ÖĞ¶Ï³õÊ¼»¯ÅäÖÃ
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource12);
		EXTI_InitStructure.EXTI_Line=EXTI_Line12;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//¸ù¾İEXTI_InitStructÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèEXTI¼Ä´æÆ÷
			
						 //GPIOC.10	  ÖĞ¶ÏÏßÒÔ¼°ÖĞ¶Ï³õÊ¼»¯ÅäÖÃ
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource1);
		EXTI_InitStructure.EXTI_Line=EXTI_Line1;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);		//¸ù¾İEXTI_InitStructÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèEXTI¼Ä´æÆ÷
				
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//Ê¹ÄÜ°´¼üËùÔÚµÄÍâ²¿ÖĞ¶ÏÍ¨µÀ
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//ÇÀÕ¼ÓÅÏÈ¼¶2 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//×ÓÓÅÏÈ¼¶2 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//Ê¹ÄÜÍâ²¿ÖĞ¶ÏÍ¨µÀ
  	NVIC_Init(&NVIC_InitStructure);  	  //¸ù¾İNVIC_InitStructÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷
		
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//Ê¹ÄÜ°´¼üËùÔÚµÄÍâ²¿ÖĞ¶ÏÍ¨µÀ
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//ÇÀÕ¼ÓÅÏÈ¼¶2 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//×ÓÓÅÏÈ¼¶2 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//Ê¹ÄÜÍâ²¿ÖĞ¶ÏÍ¨µÀ
		NVIC_Init(&NVIC_InitStructure);  	  //¸ù¾İNVIC_InitStructÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//Ê¹ÄÜ°´¼üËùÔÚµÄÍâ²¿ÖĞ¶ÏÍ¨µÀ
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//ÇÀÕ¼ÓÅÏÈ¼¶2 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//×ÓÓÅÏÈ¼¶2 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//Ê¹ÄÜÍâ²¿ÖĞ¶ÏÍ¨µ
		NVIC_Init(&NVIC_InitStructure);  	  //¸ù¾İNVIC_InitStructÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷
		
		
			
}


void EXTI9_5_IRQHandler(void)
{
	float UltrasonicWave_Distance;      //¼ÆËã³öµÄ¾àÀë 
	delay_us(10);		                      //ÑÓÊ±10us
     if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
			TIM_SetCounter(TIM2,0);
			TIM_Cmd(TIM2, ENABLE);                                             //¿ªÆôÊ±ÖÓ
		
			while(GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN));	                 //µÈ´ıµÍµçÆ½

			TIM_Cmd(TIM2, DISABLE);			                                 //¶¨Ê±Æ÷2Ê§ÄÜ
			UltrasonicWave_Distance=TIM_GetCounter(TIM2)*2*5*34/200.0;									 //¼ÆËã¾àÀë&&UltrasonicWave_Distance<150
		
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
	EXTI_ClearITPendingBit(EXTI_Line7);  //Çå³ıEXTI7ÏßÂ·¹ÒÆğÎ»
}
}

void EXTI15_10_IRQHandler(void)
{
	float UltrasonicWave_Distance;      //¼ÆËã³öµÄ¾àÀë 
	delay_us(10);		//ÑÓÊ±10us
  if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{
			TIM_SetCounter(TIM2,0);
			TIM_Cmd(TIM2, ENABLE); 		//¿ªÆô
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12));		//µÈ´ıµÍµçÆ½
			TIM_Cmd(TIM2, DISABLE);			                                 //¶¨Ê±Æ÷3Ê§ÄÜ
			UltrasonicWave_Distance=TIM_GetCounter(TIM2)*2*5*34/200.0;									 //¼ÆËã¾àÀë&&UltrasonicWave_Distance<150

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
	
		
	
		EXTI_ClearITPendingBit(EXTI_Line12);  //Çå³ıEXTI12ÏßÂ·¹ÒÆğÎ»
	}
}


void EXTI1_IRQHandler(void)
{

	float UltrasonicWave_Distance;	//¼ÆËã³öµÄ¾àÀë 
	
	delay_us(10);		//ÑÓÊ±10us
     if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
			TIM_SetCounter(TIM2,0);
			TIM_Cmd(TIM2, ENABLE); 		//¿ªÆô
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1));		//µÈ´ıµÍµçÆ½
			TIM_Cmd(TIM2, DISABLE);					//¶¨Ê±Æ÷3Ê§ÄÜ
			UltrasonicWave_Distance=TIM_GetCounter(TIM2)*2*5*34/200.0;									 //¼ÆËã¾àÀë&&UltrasonicWave_Distance<150
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
	
	
	EXTI_ClearITPendingBit(EXTI_Line1);  //Çå³ıEXTI12ÏßÂ·¹ÒÆğÎ»
	}

}

/*
 * º¯ÊıÃû£ºUltrasonicWave_StartMeasure
 * ÃèÊö  £º¿ªÊ¼²â¾à£¬·¢ËÍÒ»¸ö>10usµÄÂö³å£¬È»ºó²âÁ¿·µ»ØµÄ¸ßµçÆ½Ê±¼ä
 * ÊäÈë  £ºÎŞ
 * Êä³ö  £ºÎŞ	
 */
void UltrasonicWave_StartMeasure(void)
{
	
	
		GPIO_SetBits(TRIG_PORT,TRIG_PIN); 	//ËÍ>10USµÄ¸ßµçÆ½£TRIG_PORT,TRIG_PINÕâÁ½¸öÔÚdefineÖĞÓĞ?
		delay_us(20);		                      //ÑÓÊ±20US
		GPIO_ResetBits(TRIG_PORT,TRIG_PIN);
		delay_ms(200);
		GPIO_SetBits(GPIOC,GPIO_Pin_11); 		  //ËÍ>10USµÄ¸ßµçÆ½£TGPIOC,GPIO_Pin_11ÕâÁ½¸öÔÚdefineÖĞÓĞ?
		delay_us(20);		                     //ÑÓÊ±20US
		GPIO_ResetBits(GPIOC,GPIO_Pin_11);
		delay_ms(200);                                     
		GPIO_SetBits(GPIOC,GPIO_Pin_0); 		  //ËÍ>10USµÄ¸ßµçÆ½£TGPIOC,GPIO_Pin_1ÕâÁ½¸öÔÚdefineÖĞÓĞ?
		delay_us(20);		                     //ÑÓÊ±20US
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
