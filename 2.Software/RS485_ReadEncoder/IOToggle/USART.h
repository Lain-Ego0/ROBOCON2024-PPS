//@file    USART.h

#define   SET_RE    GPIOA->ODR|=(1<<8);
#define   RESET_RE  GPIOA->ODR&=~(1<<8);
unsigned char ReceiveData=0X0;
unsigned char ReceiveData_All[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
u8 SendData_All[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

u8 USART1_i=0;

u32 Encoder_Data = 0;


void USART1_Config(int botelv)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);

	USART_InitStructure.USART_BaudRate = botelv;

	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_Parity = USART_Parity_No;

	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);         //按上面参数配置USART1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);    //使能接收中断
	USART_Cmd(USART1, ENABLE);						//使能USART1


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         //USART1 TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);		       //A端口 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         //USART1 RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //复用开漏输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);		            //A端口 

	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	  
	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
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


void USART1_IRQHandler(void)
{
	  
	unsigned short int CRC_7;


  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
				
		ReceiveData = USART_ReceiveData(USART1);
		
		for(USART1_i=0;USART1_i<8;USART1_i++){ReceiveData_All[USART1_i]=ReceiveData_All[USART1_i+1];}
		ReceiveData_All[8]=ReceiveData;

		CRC_7=Crc_Count(ReceiveData_All,7); //计算出校验码
			
		if(CRC_7==(ReceiveData_All[6]+(ReceiveData_All[7]<<8)))//匹配CRC校验码是否正确
		{				
				if(ReceiveData_All[0]==0x01)//判断编码器ID是否正确
				{
					//得到编码器位置数据
					Encoder_Data = ReceiveData_All[3] << 24 + ReceiveData_All[4] << 16 + ReceiveData_All[5] << 8 + ReceiveData_All[6];
				}
		}
			

			
		USART_ClearFlag(USART1,USART_FLAG_RXNE);
  }
	
	if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
	{ 
		ReceiveData = USART_ReceiveData(USART1); 
		USART_ClearFlag(USART1, USART_FLAG_ORE);
	}

	if(USART_GetFlagStatus(USART1, USART_FLAG_NE) != RESET)USART_ClearFlag(USART1, USART_FLAG_NE);
	if(USART_GetFlagStatus(USART1, USART_FLAG_FE) != RESET)USART_ClearFlag(USART1, USART_FLAG_FE);
	if(USART_GetFlagStatus(USART1, USART_FLAG_PE) != RESET)USART_ClearFlag(USART1, USART_FLAG_PE);

}
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); 	
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
  //关闭JATA，使能PA15、PB3、PB4
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

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


//#endif 





