
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "USART.h"


//#include "encoderdata.h"


unsigned short int CRC_6;
int main(void)
{
	GPIO_Configuration();
	USART1_Config(9600);	
	if (SysTick_Config(72000))  while (1);
	Delay(500);
	
	
	
	

	while (1)
	{
		//发送读取编码器指令
		SET_RE;
		SendData_All[0]=0x01;
		SendData_All[1]=0x03;
		SendData_All[2]=0x00;
		SendData_All[3]=0x00;
		SendData_All[4]=0x00;
		SendData_All[5]=0x02;
		CRC_6=Crc_Count(SendData_All,6); 
		SendData_All[6]=CRC_6;
		SendData_All[7]=CRC_6>>8;
		USART_Transmit(USART1,SendData_All,8);
		RESET_RE;
		
		Delay(50);


	
	}
}

//以下
void RCC_Configuration(void){SystemInit();}	
#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{while (1){}}
#endif
