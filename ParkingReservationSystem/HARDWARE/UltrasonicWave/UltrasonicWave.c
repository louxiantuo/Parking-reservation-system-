/******************** (C) 1209 Lab **************************
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
#include "timer.h"
#include "delay.h"
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
	{/*
		num[0]++;
		if(UltrasonicWave_Distance>50)mark[0]++;
		while(num[0]>=10)
		{
			num[0]=0;
			if(mark[0]>7)printf("1车位无车\t");
			else printf("1车位有车\t");
			mark[0]=0;
	}*/
	
	printf("distance1:%f cm\t",UltrasonicWave_Distance);
	
		
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
			TIM_SetCounter(TIM3,0);
			TIM_Cmd(TIM3, ENABLE); 		//开启
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12));		//等待低电平
			TIM_Cmd(TIM3, DISABLE);			                                 //定时器3失能
			UltrasonicWave_Distance=TIM_GetCounter(TIM3)*2*5*34/200.0;									 //计算距离&&UltrasonicWave_Distance<150
		if(UltrasonicWave_Distance>0)
		{
		/*	num[1]++;
			if(UltrasonicWave_Distance>50)mark[1]++;
			while(num[1]>=10)
			{
				num[1]=0;
				if(mark[1]>7)printf("2车位无车\n");
				else printf("2车位有车\n");
				mark[1]=0;
			}*/
			
			printf("distance2:%f cm\t",UltrasonicWave_Distance);
		}
		EXTI_ClearITPendingBit(EXTI_Line12);  //清除EXTI12线路挂起位
	}
}


void EXTI1_IRQHandler(void)
{

	float UltrasonicWave_Distance;	//计算出的距离 
	
	delay_us(10);		//延时10us
     if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
			TIM_SetCounter(TIM4,0);
			TIM_Cmd(TIM4, ENABLE); 		//开启
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1));		//等待低电平
			TIM_Cmd(TIM4, DISABLE);					//定时器3失能
			UltrasonicWave_Distance=TIM_GetCounter(TIM4)*2*5*34/200.0;									 //计算距离&&UltrasonicWave_Distance<150
	if(UltrasonicWave_Distance>0)
	{
		printf("distance3:%f cm\n",UltrasonicWave_Distance);
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

/******************* (C) 1209 Lab *****END OF FILE************/
