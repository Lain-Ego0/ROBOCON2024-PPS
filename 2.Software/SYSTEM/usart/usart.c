#include "sys.h"
#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

void uart1_init(void){
	//GPIO端口设置
	//USART 初始化设置
	USART_InitTypeDef USART_InitStructure;	
	GPIO_InitTypeDef		GPIO_Initstructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	//中断优先级分组


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//使能USART3时钟
	//配置GPIO
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB,&GPIO_Initstructure);

	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIO_Initstructure);

	USART_InitStructure.USART_BaudRate = 115200;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); 
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
	USART_Cmd(USART1, ENABLE);   	
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//开启串口接受中断
	USART_ClearFlag(USART1, USART_FLAG_TC);	

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//中断道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器

}

void USART1_IRQHandler(void){                	//串口1中断服务程序
    // 检查串口接收数据寄存器是否非空
    if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
		USART_ClearITPendingBit(USART1,USART_IT_IDLE);
		USART1->DR;
		USART1->SR;		
		DMA_Cmd(DMA1_Channel5,DISABLE);
		P_uintH219BuffLength = HI219M_RX_BUF_LEN - (DMA1_Channel5->CNDTR);
		DMA_ClearFlag(DMA_ISR_GIF5 | DMA_ISR_TCIF5 | DMA_ISR_HTIF5 | DMA_ISR_TEIF5);
		while(P_uintH219BuffLength > 128);
		DMA_SetCurrDataCounter(DMA1_Channel5,HI219M_RX_BUF_LEN);
		DMA_Cmd(DMA1_Channel5,ENABLE);
		Hi226_DataGet();	
	}
} 

#if EN_USART3_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void uart2_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 启用 GPIOA 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // 启用 USART2 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    // 配置 USART2 的 TX 引脚（GPIOA.2）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 设置为复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置 USART2 的 RX 引脚（GPIOA.3）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 设置为浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置 NVIC 用于 USART2 中断
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 抢占优先级 2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        // 子优先级 3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 使能 IRQ 通道
    NVIC_Init(&NVIC_InitStructure);                           // 初始化 NVIC 寄存器

    // USART2 配置
    USART_InitStructure.USART_BaudRate = 115200;                    // 设置波特率为 115200
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;     // 数据位长度为 8 位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;          // 1 个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;             // 无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控制
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // 启用发送和接收模式

    USART_Init(USART2, &USART_InitStructure);                      // 初始化 USART2
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);                 // 使能 USART2 接收中断
    USART_Cmd(USART2, ENABLE);                                     // 使能 USART2
}

uint8_t UART3_ReadWriteByte(uint8_t TxData)
{
    uint8_t TxWait = 0;
    uint8_t RxWait = 0;
    // 等待发送缓存为空
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
    {
        TxWait++;
        if (TxWait > 250) // 等待时间过长则放弃本次读写
            return 0;
    }

    USART_SendData(USART2, TxData); // 串口3发送一个字节

    // 等待接收缓存非空
    while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
    {
        RxWait++;
        if (RxWait > 250) // 等待时间过长则放弃本次读写
			return 0;
    }

    return USART_ReceiveData(USART2); // 返回接收到的字节
}


#endif	



