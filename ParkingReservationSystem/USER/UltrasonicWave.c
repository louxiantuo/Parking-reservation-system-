/***************** (C) 1209 Lab **************************
 * 文件名  : UltrasonicWave.c
 * 描述    ：超声波测距模块，UltrasonicWave_Configuration（）函数
             初始化超声模块，UltrasonicWave_StartMeasure（）函数
			 启动测距，并将测得的数据通过串口1打印出来         
 * 实验平台：Mini STM32开发板  STM32F103RBT6
 * 硬件连接：------------------
 *          | PC8  - TRIG      |
 *          | PC7  - ECHO      |
 *           ------------------
 * 库版本  ：ST3.5.0
 *
 * 作者    ：Lee 
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

extern int hcsr1;//0表示没车，1表示有车
extern int hcsr2;
extern int hcsr3;

int mark[3]={0},num[3]={0};

#define	TRIG_PORT      GPIOC		//TRIG       
#define	ECHO_PORT      GPIOC		//ECHO 
#define	TRIG_PIN       GPIO_Pin_8   //TRIG       
#define	ECHO_PIN       GPIO_Pin_7	//ECHO   

   


/*
 * 函数名：UltrasonicWave_Configuration
 * 描述  ：超声波模块的初始化
 * 输入  ：无
 * 输出  ：无	
 */
void UltrasonicWave_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//关闭jtag
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
   
  GPIO_InitStructure.GPIO_Pin = TRIG_PIN;					 //PC8接TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(TRIG_PORT, &GPIO_InitStructure);	                 //初始化外设GPIO 

	
  GPIO_InitStructure.GPIO_Pin = ECHO_PIN;				     //PC7接ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 //设为输入
  GPIO_Init(ECHO_PORT,&GPIO_InitStructure);				 //初始化GPIOC
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;					 //PC11接TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//设为推挽输出模式
	GPIO_Init(GPIOC, &GPIO_InitStructure);	                 //初始化外设GPIO 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12	;				     //PC12接ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 //设为输入
  GPIO_Init(GPIOC,&GPIO_InitStructure);						 //初始化GPIOC
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;					 //PC0接TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//设为推挽输出模式
	GPIO_Init(GPIOC, &GPIO_InitStructure);	                 //初始化外设GPIO 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1	;				     //PC1接ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 //设为输入
  GPIO_Init(GPIOC,&GPIO_InitStructure);						 //初始化GPIOC
	
	 //GPIOC.7	  中断线以及中断初始化配置
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource7);

		EXTI_InitStructure.EXTI_Line=EXTI_Line7;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		
						 //GPIOC.12	  中断线以及中断初始化配置
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource12);
		EXTI_InitStructure.EXTI_Line=EXTI_Line12;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
			
						 //GPIOC.10	  中断线以及中断初始化配置
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource1);
		EXTI_InitStructure.EXTI_Line=EXTI_Line1;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
				
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能按键所在的外部中断通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//子优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
		
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//子优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
		NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//使能按键所在的外部中断通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//子优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通�
		NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
		
		
			
}


void EXTI9_5_IRQHandler(void)
{
	float UltrasonicWave_Distance;      //计算出的距离 
	delay_us(10);		                      //延时10us
     if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
			TIM_SetCounter(TIM2,0);
			TIM_Cmd(TIM2, ENABLE);                                             //开启时钟
		
			while(GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN));	                 //等待低电平

			TIM_Cmd(TIM2, DISABLE);			                                 //定时器2失能
			UltrasonicWave_Distance=TIM_GetCounter(TIM2)*2*5*34/200.0;									 //计算距离&&UltrasonicWave_Distance<150
		
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
	EXTI_ClearITPendingBit(EXTI_Line7);  //清除EXTI7线路挂起位
}
}

void EXTI15_10_IRQHandler(void)
{
	float UltrasonicWave_Distance;      //计算出的距离 
	delay_us(10);		//延时10us
  if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{
			TIM_SetCounter(TIM2,0);
			TIM_Cmd(TIM2, ENABLE); 		//开启
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12));		//等待低电平
			TIM_Cmd(TIM2, DISABLE);			                                 //定时器3失能
			UltrasonicWave_Distance=TIM_GetCounter(TIM2)*2*5*34/200.0;									 //计算距离&&UltrasonicWave_Distance<150

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
	
		
	
		EXTI_ClearITPendingBit(EXTI_Line12);  //清除EXTI12线路挂起位
	}
}


void EXTI1_IRQHandler(void)
{

	float UltrasonicWave_Distance;	//计算出的距离 
	
	delay_us(10);		//延时10us
     if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
			TIM_SetCounter(TIM2,0);
			TIM_Cmd(TIM2, ENABLE); 		//开启
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1));		//等待低电平
			TIM_Cmd(TIM2, DISABLE);					//定时器3失能
			UltrasonicWave_Distance=TIM_GetCounter(TIM2)*2*5*34/200.0;									 //计算距离&&UltrasonicWave_Distance<150
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
	
	
	EXTI_ClearITPendingBit(EXTI_Line1);  //清除EXTI12线路挂起位
	}

}

/*
 * 函数名：UltrasonicWave_StartMeasure
 * 描述  ：开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
 * 输入  ：无
 * 输出  ：无	
 */
void UltrasonicWave_StartMeasure(void)
{
	
	
		GPIO_SetBits(TRIG_PORT,TRIG_PIN); 	//送>10US的高电平RIG_PORT,TRIG_PIN这两个在define中有?
		delay_us(20);		                      //延时20US
		GPIO_ResetBits(TRIG_PORT,TRIG_PIN);
		delay_ms(200);
		GPIO_SetBits(GPIOC,GPIO_Pin_11); 		  //送>10US的高电平GPIOC,GPIO_Pin_11这两个在define中有?
		delay_us(20);		                     //延时20US
		GPIO_ResetBits(GPIOC,GPIO_Pin_11);
		delay_ms(200);                                     
		GPIO_SetBits(GPIOC,GPIO_Pin_0); 		  //送>10US的高电平GPIOC,GPIO_Pin_1这两个在define中有?
		delay_us(20);		                     //延时20US
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
