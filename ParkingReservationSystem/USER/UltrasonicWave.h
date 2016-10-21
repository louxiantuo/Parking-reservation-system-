#ifndef __UltrasonicWave_H
#define	__UltrasonicWave_H



void UltrasonicWave_Configuration(void);               //对超声波模块初始化
void UltrasonicWave_StartMeasure(void);                //开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
void relay_init(void);
void relay_close(int switch_close);
void relay_open(int switch_open);
#endif /* __UltrasonicWave_H */

