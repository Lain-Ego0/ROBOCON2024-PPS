#ifndef  _KEY_H
#define  _KEY_H

#include "main.h"

void USART3_Init(void);

void Serial_Init(void);
void Fa_song_dai_ma_Init(void);
extern u32 Encoder_X;
extern u32 Encoder_Y;
extern u8 laps;


extern u32 Encoder_Data;
extern u32 Encoder_value;
extern unsigned char ReceiveData_All[];
extern unsigned char ReceiveData;
extern u8 SendData_All[];
extern u8 YSendData_All[];

void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);
void Delayms(__IO uint32_t nCount);
void USART_Transmit(USART_TypeDef* USARTx,u8 *data,int num);
void USART_Transmit1(USART_TypeDef* USARTx,u8 data);
unsigned int Crc_Count(unsigned char pbuf[],unsigned char num);
#endif

