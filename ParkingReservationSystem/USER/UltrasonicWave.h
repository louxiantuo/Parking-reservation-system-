#ifndef __UltrasonicWave_H
#define	__UltrasonicWave_H



void UltrasonicWave_Configuration(void);               //�Գ�����ģ���ʼ��
void UltrasonicWave_StartMeasure(void);                //��ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��
void relay_init(void);
void relay_close(int switch_close);
void relay_open(int switch_open);
#endif /* __UltrasonicWave_H */

