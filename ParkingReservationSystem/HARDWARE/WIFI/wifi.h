#ifndef _WIFI_H
#define _WIFI_H
#include "usart2.h"
#include "string.h"
#include "sys.h"
#include "delay.h"


#define SSID 			"TP-LINK_5F23"//"ADESKTOP-OU3N6VF"//"celitea"//"XinDianShe"//"PandoraBox_AD891C"//
#define PASSWORD		"85208520"//"xindian10403"//""
#define SERVICE_IP 		"192.168.1.118"//"192.168.1.214"
#define SERVICE_PORT 	"80"

u8 wifi_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8* wifi_check_cmd(u8 *str);
u8 UpLoad(u8 *send_cmd);
void wifi_init(void);


#endif

