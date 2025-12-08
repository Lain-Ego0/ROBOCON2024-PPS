#include "Key.h"
#include "delay.h"
#include <stdio.h>
#include <stdint.h>

#define   SET_RE    GPIOA->ODR|=(1<<8);
#define   RESET_RE  GPIOA->ODR&=~(1<<8);
unsigned char ReceiveData=0X0;
unsigned char ReceiveData_All[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
u8 SendData_All[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
u8 YSendData_All[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t Encoder_XA[4] = {0x00, 0x00, 0x00, 0x00};
uint8_t Encoder_YA[4] = {0x00, 0x00, 0x00, 0x00};
u32 Encoder_X=0;
u32 Encoder_Y=0;
u8 laps=0;
u8 USART1_i=0;

u32 Encoder_Data = 0;
u32 Encoder_value = 0;

void USART3_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);

	USART_InitStructure.USART_BaudRate = 9600;

	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_Parity = USART_Parity_No;

	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &USART_InitStructure);         //按上面参数配置USART1
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);    //使能接收中断
	USART_Cmd(USART3, ENABLE);						//使能USART1


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         //USART1 TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);		       //A端口 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	         //USART1 RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //复用开漏输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);		            //A端口 

	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	  
	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	
}
/*************************校验码计算函数*************************/
unsigned int Crc_Count(unsigned char pbuf[],unsigned char num)
{
   int i,j;
   unsigned int wcrc=0xffff;
   for(i=0;i<num;i++)
   {
     wcrc^=(unsigned int)(pbuf[i]);
		 for (j=0;j<8;j++)
		 {
			if(wcrc&0x0001)
			{
				 wcrc>>=1;
				 wcrc^=0xa001;
			}
			else wcrc>>=1;		
		 }
   }   
   return wcrc;
}
/**********************************发送函数************************************************************/
void USART_Transmit(USART_TypeDef* USARTx,u8 *data,int num)	 //发送数组
{
	while(num--)
	{
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);//发送寄存器空
		USART_SendData(USARTx,*data++);//发送单个数据//USART1->DR=*data++;
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);//等待数据帧发送完毕
	}
}
void USART_Transmit1(USART_TypeDef* USARTx,u8 data)//发送一个字节
{ 
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);//发送寄存器空
	USART_SendData(USARTx,data);//发送单个数据//USART1->DR=*data++;
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);//等待数据帧发送完毕
}
uint32_t hexArrayToDecimal(uint8_t hexArray[], int size) {
    uint32_t result = 0;
    int i;
	// Assuming each element in the hex array represents a 2-digit hex number
    for (i = 0; i < size; ++i) 
	{
        result = (result << 8) | hexArray[i];
    }
    return result;
}
uint32_t YhexArrayToDecimal(uint8_t hexArray[], int size) 
{
    uint32_t result = 0;
    int i;
    // Assuming each element in the hex array represents a 2-digit hex number
    for (i = 0; i < size; ++i) { result = (result << 8) | hexArray[i]; }
    return result;
}
void USART3_IRQHandler(void)
{
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{	
		ReceiveData = USART_ReceiveData(USART3);
		for(USART1_i=0;USART1_i<8;USART1_i++){ReceiveData_All[USART1_i]=ReceiveData_All[USART1_i+1];}
		ReceiveData_All[8]=ReceiveData;
		if(ReceiveData_All[0]==0x01&&ReceiveData_All[1]==0x03&&ReceiveData_All[2]==0x04)//匹配CRC校验码是否正确
		{				
			if(ReceiveData_All[0]==0x01)//判断编码器ID是否正确
			{
				if(ReceiveData_All[1]==0x03)//判断编码器ID是否正确
				{
					if(ReceiveData_All[2]==0x04)//判断编码器ID是否正确
					{
						Encoder_XA[0]=ReceiveData_All[3];
						Encoder_XA[1]=ReceiveData_All[4];
						Encoder_XA[2]=ReceiveData_All[5];
						Encoder_XA[3]=ReceiveData_All[6];
						Encoder_X = hexArrayToDecimal(Encoder_XA, 4);
					}
				}
			}	
			USART_ClearFlag(USART3,USART_FLAG_RXNE);
		}	
		if(ReceiveData_All[0]==0x02&&ReceiveData_All[1]==0x03&&ReceiveData_All[2]==0x04)//匹配CRC校验码是否正确
		{				
			if(ReceiveData_All[0]==0x02)//判断编码器ID是否正确
			{
				Encoder_YA[0]=ReceiveData_All[3];
				Encoder_YA[1]=ReceiveData_All[4];
				Encoder_YA[2]=ReceiveData_All[5];
				Encoder_YA[3]=ReceiveData_All[6];
				Encoder_Y = YhexArrayToDecimal(Encoder_YA, 4);
			}	
			USART_ClearFlag(USART3,USART_FLAG_RXNE);
		}
		if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
		{ 
			ReceiveData = USART_ReceiveData(USART3); 
			USART_ClearFlag(USART3, USART_FLAG_ORE);
		}
		if(USART_GetFlagStatus(USART3, USART_FLAG_NE) != RESET)USART_ClearFlag(USART3, USART_FLAG_NE);
		if(USART_GetFlagStatus(USART3, USART_FLAG_FE) != RESET)USART_ClearFlag(USART3, USART_FLAG_FE);
		if(USART_GetFlagStatus(USART3, USART_FLAG_PE) != RESET)USART_ClearFlag(USART3, USART_FLAG_PE);
	}
}

static __IO uint32_t TimingDelay;

void Delayms(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}



