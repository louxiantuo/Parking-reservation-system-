#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usart2.h"
#include "beep.h"
#include "wifi.h"
#include "timer.h"
#include "exti.h" 
#include "UltrasonicWave.h"
#include <string.h>
int number = 0;
int hcsr1;
int hcsr2;
int hcsr3;
unsigned char relay1 = 0;
unsigned char relay2 = 0;
unsigned char relay3 = 0;
unsigned char relay_main = 0;
//超声波状态
u8 status_hcsr_1_1[128];//1表示有车
u8 status_hcsr_2_1[128];
u8 status_hcsr_3_1[128];
u8 status_hcsr_1_0[128];
u8 status_hcsr_2_0[128];
u8 status_hcsr_3_0[128];

//继电器状态
u8 status_relay_1_1[128];//1表示已经被锁定
u8 status_relay_2_1[128];
u8 status_relay_3_1[128];
u8 status_relay_1_0[128];
u8 status_relay_2_0[128];
u8 status_relay_3_0[128];

//主继电器状态
u8 status_relay_main_1[128];//1表示打开状态
u8 status_relay_main_0[128];
void cmd_init()
{
	//命令初始化
	sprintf((char*)status_hcsr_1_1,"status hcsr 1 1\r\n");
	sprintf((char*)status_hcsr_2_1,"status hcsr 2 1\r\n");
	sprintf((char*)status_hcsr_3_1,"status hcsr 3 1\r\n");
	sprintf((char*)status_hcsr_1_0,"status hcsr 1 0\r\n");
	sprintf((char*)status_hcsr_2_0,"status hcsr 2 0\r\n");
	sprintf((char*)status_hcsr_3_0,"status hcsr 3 0\r\n");
	
	sprintf((char*)status_relay_1_1,"status relay 1 1\r\n");
	sprintf((char*)status_relay_2_1,"status relay 2 1\r\n");
	sprintf((char*)status_relay_3_1,"status relay 3 1\r\n");
	sprintf((char*)status_relay_1_0,"status relay 1 0\r\n");
	sprintf((char*)status_relay_2_0,"status relay 2 0\r\n");
	sprintf((char*)status_relay_3_0,"status relay 3 0\r\n");
	
	sprintf((char*)status_relay_main_1,"status relay main 1\r\n");
	sprintf((char*)status_relay_main_0,"status relay main 0\r\n");
}



//控制命令
u8 control_relay_1_1[128];
u8 control_relay_2_1[128];
u8 control_relay_3_1[128];
u8 control_relay_1_0[128];
u8 control_relay_2_0[128];
u8 control_relay_3_0[128];
u8 control_relay_main_1[128];
u8 control_relay_main_0[128];
u8 control_getinfo[128];//查询所有继电器和超声波传感器的状态
//u8* buf;
void control_init()
{
	sprintf((char*)control_relay_1_1,"control relay 1 1");
	sprintf((char*)control_relay_2_1,"control relay 2 1");
	sprintf((char*)control_relay_3_1,"control relay 3 1");
	sprintf((char*)control_relay_1_0,"control relay 1 0");
	sprintf((char*)control_relay_2_0,"control relay 2 0");
	sprintf((char*)control_relay_3_0,"control relay 3 0");
	
	sprintf((char*)control_relay_main_1,"control relay main 1");
	sprintf((char*)control_relay_main_0,"control relay main 0");
	
	sprintf((char*)control_getinfo,"control getinfo");
}

void send_all()
{
			if(hcsr1==1)
		{
			UpLoad(status_hcsr_1_1);
		}
		else
			UpLoad(status_hcsr_1_0);
			if(hcsr2==1)
		{
			UpLoad(status_hcsr_2_1);
		}
		else
			UpLoad(status_hcsr_2_0);
			if(hcsr3==1)
		{
			UpLoad(status_hcsr_3_1);
		}
		else
			UpLoad(status_hcsr_3_0);
		
		if(relay1 == 1)
		{
			UpLoad(status_relay_1_1);
		}
		else 
			UpLoad(status_relay_1_0);
		
		if(relay2 == 1)
		{
			UpLoad(status_relay_2_1);
		}
		else
			UpLoad(status_relay_2_0);
		
		if(relay3 == 1)
		{
			UpLoad(status_relay_3_1);
		}
		else
			UpLoad(status_relay_3_0);
		
		if(relay_main == 1)
		{
			UpLoad(status_relay_main_1);
		}
		else 
			UpLoad(status_relay_main_0);
}


//u8 humidity;
//u8 value;

void check_relay()
{
	

	if(strcmp(USART2_RX_BUF,control_relay_1_1) == 0)
	{ 
		relay1 = 1;
		//printf(".....................\n");
		relay_open(1);
		strcpy(USART2_RX_BUF,"\n");
	}
	if(strcmp(USART2_RX_BUF,control_relay_1_0) == 0)
	{
		relay1 = 0;
		relay_close(1);
		strcpy(USART2_RX_BUF,"\n");
	}
	if(strcmp(USART2_RX_BUF,control_relay_2_1) == 0)
	{
		relay2 = 1;
		relay_open(2);
		strcpy(USART2_RX_BUF,"\n");
	}
	if(strcmp(USART2_RX_BUF,control_relay_2_0) == 0)
	{
		relay2 = 0;
		relay_close(2);
		strcpy(USART2_RX_BUF,"\n");
	}
	if(strcmp(USART2_RX_BUF,control_relay_3_1) == 0)
	{
		relay3 = 1;
		relay_open(3);
		strcpy(USART2_RX_BUF,"\n");
	}
	if(strcmp(USART2_RX_BUF,control_relay_3_0) == 0)
	{
		relay3 = 0;
		relay_close(3);
		strcpy(USART2_RX_BUF,"\n");
	}
	
	if(strcmp(USART2_RX_BUF,control_relay_main_1) == 0)
	{
		relay_main = 1;
		relay_open(0);
		strcpy(USART2_RX_BUF,"\n");
		delay_ms(1000);
		delay_ms(1000);
		relay_close(0);
		relay_main = 0;
	}

}
u8 send_cmd[128];
u8 send_cmd_test[128];
int main(void)
{
  hcsr1 = 1;
	cmd_init();//发往PC端命令
	control_init();//来自PC端发送的命令
	uart_init(115200); 
	printf("\r\n车位预定系统硬件部分\r\n");
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	delay_init();	    	 //延时函数初始化	  
	//uart_init(115200);	 //串口初始化为115200
	uart2_init(115200);
	Timerx_Init(5000,7199); 
	UltrasonicWave_Configuration(); 
	BEEP_Init();
	wifi_init();//wifi模块初始化
	relay_init();
	sprintf((char*)send_cmd,"the first cmd");
	
	UpLoad("STM_32\r\n");
	
	while (1)
	{
	
		
		UltrasonicWave_StartMeasure();		//开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
		
		TIM3_IRQHandler();
		
		
		number++;
		if (number > 10)
		{
			send_all();
			number = 0;
		}		
		check_relay();
	}
}

