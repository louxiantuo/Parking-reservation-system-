#include "wifi.h"
#include "beep.h"

//char HttpBuf[88];

void wifi_init(void)
{
	u8 wifi_connect[128];
	u2_printf("+++");
	delay_ms(50);
	
	//sprintf((char*)wifi_connect,"AT+CWJAP=\"%s\",\"%s\"", SSID,PASSWORD);
	sprintf((char*)wifi_connect,"AT+CWJAP=\"Celitea_102\",\"85208520\"");
	printf("%s",wifi_connect);
	if(!wifi_send_cmd("AT","OK",20))
		printf("\r\nERROR\r\n");;
	//设置wifi应用模式为1
	wifi_send_cmd("AT+CWMODE=1", "OK", 20);
	
	wifi_send_cmd(wifi_connect,"OK",2000);
	
	//检查是否加入成功
	while (wifi_send_cmd("AT+CWJAP?","+CWJAP",400) != 0)		//没有加入AP
	{
		//加入wifi
		if(wifi_send_cmd(wifi_connect,"OK",2000) == 0)
		{
		  printf("wifi init ok\r\n");
			break;
		}
		else
			printf("wifi init error");
	}
	BEEP = 1;
	delay_ms(500);
	BEEP = 0;
	printf("wifi_init\r\n");
	//	u8 HttpBuf[128] ;
	
	wifi_send_cmd("AT+CIPSTART=\"TCP\",\"192.168.1.137\",10000","OK",500);
	
	//启动透传
	wifi_send_cmd("AT+CIPMODE=1","OK",100);
	
	
	//发送数据
	wifi_send_cmd((u8 *)"AT+CIPSEND",(u8 *)">",50);

}

//cmd:发送命令字符串
//ack:期待的应答结果，如果为空，则表示不需要应答
//waittime:等待时间，(单位：10ms)
//返回值：0, 发送成功（得到了期待的结果）
//        1，发送失败
u8 wifi_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 

	USART2_RX_STA=0;
	u2_printf("%s\r\n",cmd);	//发送命令

		sprintf((char*)USART2_TX_BUF, "%s\r\n", cmd);

	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)	//接收到期待的应答结果
			{
				if(wifi_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//得到有效的数据
				}
					USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}

//str:期待有应答结果
//返回值:0, 没有得到期待的就任结果
//    其他，期待就任结果的位置(str的位置)
u8* wifi_check_cmd(u8 *str)
{
	
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
		printf("%s\r\n",strx);
	} 
	return (u8*)strx;
}



u8 UpLoad(u8 *send_cmd)
{

	wifi_send_cmd(send_cmd,"",50);
	return 0;
}



