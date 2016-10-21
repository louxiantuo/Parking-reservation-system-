#ifndef __DS18B20_H
#define __DS18B20_H 
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 


//IO��������
#define DS18B20_IO_IN()  {GPIOG->CRH&=0XFFFF0FFF;GPIOG->CRH|=8<<12;}
#define DS18B20_IO_OUT() {GPIOG->CRH&=0XFFFF0FFF;GPIOG->CRH|=3<<12;}
////IO��������											   
#define	DS18B20_DQ_SET GPIO_SetBits(GPIOG,GPIO_Pin_11) //���ݶ˿�	
#define	DS18B20_DQ_CLR GPIO_ResetBits(GPIOG,GPIO_Pin_11) //���ݶ˿�	
#define	DS18B20_DQ_IN  GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_11)  //���ݶ˿�	
   	
u8 DS18B20_Init(void);//��ʼ��DS18B20
short DS18B20_Get_Temp(void);//��ȡ�¶�
void DS18B20_Start(void);//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);//����һ���ֽ�
u8 DS18B20_Read_Bit(void);//����һ��λ
u8 DS18B20_Check(void);//����Ƿ����DS18B20
void DS18B20_Rst(void);//��λDS18B20    
#endif
/*----------------------�·��� ������̳��www.doflye.net--------------------------*/














