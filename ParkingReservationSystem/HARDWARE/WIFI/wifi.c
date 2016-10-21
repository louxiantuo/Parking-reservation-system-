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
	//����wifiӦ��ģʽΪ1
	wifi_send_cmd("AT+CWMODE=1", "OK", 20);
	
	wifi_send_cmd(wifi_connect,"OK",2000);
	
	//����Ƿ����ɹ�
	while (wifi_send_cmd("AT+CWJAP?","+CWJAP",400) != 0)		//û�м���AP
	{
		//����wifi
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
	
	//����͸��
	wifi_send_cmd("AT+CIPMODE=1","OK",100);
	
	
	//��������
	wifi_send_cmd((u8 *)"AT+CIPSEND",(u8 *)">",50);

}

//cmd:���������ַ���
//ack:�ڴ���Ӧ���������Ϊ�գ����ʾ����ҪӦ��
//waittime:�ȴ�ʱ�䣬(��λ��10ms)
//����ֵ��0, ���ͳɹ����õ����ڴ��Ľ����
//        1������ʧ��
u8 wifi_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 

	USART2_RX_STA=0;
	u2_printf("%s\r\n",cmd);	//��������

		sprintf((char*)USART2_TX_BUF, "%s\r\n", cmd);

	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)	//���յ��ڴ���Ӧ����
			{
				if(wifi_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//�õ���Ч������
				}
					USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}

//str:�ڴ���Ӧ����
//����ֵ:0, û�еõ��ڴ��ľ��ν��
//    �������ڴ����ν����λ��(str��λ��)
u8* wifi_check_cmd(u8 *str)
{
	
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
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



