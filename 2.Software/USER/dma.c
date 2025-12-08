

#include "stm32f10x.h"
#include "dma.h"

void Configure_DMA(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    
    // 使能DMA1时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    // 重置DMA通道到默认值
    DMA_DeInit(DMA1_Channel5);
    
    // 设置DMA通道配置参数
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)P_uintH219MDataBuffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = HI219M_RX_BUF_LEN;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);
    
    // 启用DMA传输完成中断
    DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);

    // 使能USART1的DMA接收请求
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    
    // 使能DMA通道
    DMA_Cmd(DMA1_Channel5, ENABLE);
}

//#include "dma.h"
//#define BUFFER_SIZE 100

//// 配置DMA1通道1
//void Configure_DMA(void)
//{
//    DMA_InitTypeDef DMA_InitStructure;
//    
//    // 使能DMA1时钟
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//    
//    // 配置DMA
//    DMA_DeInit(DMA1_Channel5);
//    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;  // 外设基地址：USART1数据寄存器地址
//    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)P_uintH219MDataBuffer; // 内存基地址：接收数据缓冲区的地址
//    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                 // 数据传输方向：外设到内存
//    DMA_InitStructure.DMA_BufferSize = HI219M_RX_BUF_LEN;                   // 数据传输长度：接收数据缓冲区大小
//    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  // 外设地址自增模式禁用
//    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;           // 内存地址自增模式使能
//    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设数据位宽：字节
//    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         // 内存数据位宽：字节
//    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                     // DMA传输模式：正常模式
//    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;               // DMA优先级：高
//    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                      // 内存到内存传输禁用
//    DMA_Init(DMA1_Channel5, &DMA_InitStructure);
//    
//    // 使能USART1的DMA接收
//    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
//    
//    // 使能DMA1通道1
//    DMA_Cmd(DMA1_Channel5, ENABLE);
//}
