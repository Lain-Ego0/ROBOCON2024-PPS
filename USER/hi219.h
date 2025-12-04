#ifndef __HI219M_H__
#define __HI219M_H__

#include "main.h"

#define HI219M_RX_BUF_LEN 128

typedef struct Hi219m_TypeDef
{
	float Yaw;
	float Pitch;
	float Roll;
	
	float Yaw_Gyo;
	float Pitch_Gyo;
	float Roll_Gyo;
	
	float Last_Yaw;
	float Last_Pitch;
	float Last_Roll;
	
	float Continuous_Yaw;
	float Continuous_Pitch;
	float Continuous_Roll;
	
	int32_t Round;
	int32_t Rounda;
	int32_t Roundb;
}Hi219m_TypeDef;

extern uint8_t P_uintH219MDataBuffer[HI219M_RX_BUF_LEN];//H219数据缓存数组 
extern uint16_t P_uintH219BuffLength;
extern Hi219m_TypeDef P_stHi219m;

void Hi219m_ParaInit(void);
void fnHi219m_DataDecode(Hi219m_TypeDef* st_data);
void Hi226_DataGet(void);
#endif



