#include "Encoder.h"
#include "main.h"

u8 uio=0;
#define buli 175.929//Ö±¾¶

unsigned short int CRC_6;
float Yaw=0;
Positioning_system_TypeDef Positioning;
float AD_Value[12];

u32  Encoder_X1=0;
u32  Encoder_Y1=0;
float Y_distance_mm11=0;
float X_distance_mm11=0;
int X_laps1=0;
int Y_laps1=0;
void RCC_Configuration(void){SystemInit();}	

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{while (1){}}
#endif


void laps_and_distance_algorithm(void)
{
	if(Encoder_X>210000000){Encoder_X1=-(Encoder_X-2147483646);	
	Positioning.X_laps=Encoder_X1/4096;
	Positioning.X_laps_decimal=Encoder_X1/4096.0001f;
	X_distance_mm11=Positioning.X_laps_decimal*buli;
	Positioning.X_distance_mm=(int)X_distance_mm11;
	Positioning.X_distance_cm=Positioning.X_distance_mm/10;}
	if(Encoder_X>0&&Encoder_X<210000000){Encoder_X=Encoder_X;
	X_laps1=Encoder_X/4096;
	Positioning.X_laps=-X_laps1;
	Positioning.X_laps_decimal=Encoder_X/4096.0001f;
	X_distance_mm11=Positioning.X_laps_decimal*buli;
	Positioning.X_distance_mm=-(int)X_distance_mm11;
	Positioning.X_distance_cm=Positioning.X_distance_mm/10;}
		
	if(Encoder_Y>210000000){Encoder_Y1=-(Encoder_Y-2147483646);	
	Y_laps1=Encoder_Y1/4096;
	Positioning.Y_laps=-Y_laps1;
	Positioning.Y_laps_decimal=Encoder_Y1/4096.0001f;
	Y_distance_mm11=Positioning.Y_laps_decimal*buli;
	Positioning.Y_distance_mm=-(int)Y_distance_mm11;
	Positioning.Y_distance_cm=Positioning.Y_distance_mm/10;}
	if(Encoder_Y>0&&Encoder_Y<210000000){Encoder_Y=Encoder_Y;	
	Positioning.Y_laps=Encoder_Y/4096;
	Positioning.Y_laps_decimal=Encoder_Y/4096.0001f;
	Y_distance_mm11=Positioning.Y_laps_decimal*buli;
	Positioning.Y_distance_mm=(int)Y_distance_mm11;
	Positioning.Y_distance_cm=Positioning.Y_distance_mm/10;}
		
}

void reset(void)
{
	SendData_All[0]=0x01;
	SendData_All[1]=0x06;
	SendData_All[2]=0x00;
	SendData_All[3]=0x08;
	SendData_All[4]=0x00;
	SendData_All[5]=0x01;
	CRC_6=Crc_Count(SendData_All,6); 
	SendData_All[6]=CRC_6;
	SendData_All[7]=CRC_6>>8; 
	USART_Transmit(USART3,SendData_All,8);
	uio=Encoder_Data;
	Delay(100);
	
}

void Yreset(void)
{
	YSendData_All[0]=0x02;
	YSendData_All[1]=0x06;
	YSendData_All[2]=0x00;
	YSendData_All[3]=0x08;
	YSendData_All[4]=0x00;
	YSendData_All[5]=0x01;
	CRC_6=Crc_Count(YSendData_All,6); 
	YSendData_All[6]=CRC_6;
	YSendData_All[7]=CRC_6>>8; 
	USART_Transmit(USART3,YSendData_All,8);
	uio=Encoder_Data;
	Delay(100);
	
}
void disreset(void)
{
	SendData_All[0]=0x01;
	SendData_All[1]=0x03;
	SendData_All[2]=0x00;
	SendData_All[3]=0x00;
	SendData_All[4]=0x00;
	SendData_All[5]=0x02;
	CRC_6=Crc_Count(SendData_All,6); 
	SendData_All[6]=CRC_6;
	SendData_All[7]=CRC_6>>8; 
	USART_Transmit(USART3,SendData_All,8);
	uio=Encoder_Data;
	Delay(100);
	
}

void disresetY(void)
{
	YSendData_All[0]=0x02;
	YSendData_All[1]=0x03;
	YSendData_All[2]=0x00;
	YSendData_All[3]=0x00;
	YSendData_All[4]=0x00;
	YSendData_All[5]=0x02;
	CRC_6=Crc_Count(YSendData_All,6); 
	YSendData_All[6]=CRC_6;
	YSendData_All[7]=CRC_6>>8; 
	USART_Transmit(USART3,YSendData_All,8);
	uio=Encoder_Data;
	laps_and_distance_algorithm();
	Delay(100);
	
}
