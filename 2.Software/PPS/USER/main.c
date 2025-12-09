#include "main.h"

int sb=0;
int isb=0;

int main(void)
{                                     
	if (SysTick_Config(72000))  while (1);
	/*尝试配置SysTick定时器，如果配置失败（SysTick_Config返回非0值），
	则进入一个空的无限循环。这实际上是一种简单的错误处理机制。进入无限
	循环的行为意味着程序停止执行后续的代码，这通常用于在开发阶段捕获错
	误，因为它可以让程序停在错误发生的地方，便于开发者调试。*/
	Delay(500);
	Hi219m_ParaInit();
	uart1_init();
	USART3_Init();
	uart2_init();
	Configure_DMA();
	//delay_init();                                                                                                                                                                       
    while(1)
	{
		isb++;
		//发送读取编码器指令
		if(isb<20){reset();Yreset();}//重置X和Y编码器
		if(isb>=20)
		{
			//读取编码器数据
			disreset();
			disresetY();
			prepare_data();//处理数据
			send_data();//发送数据
			laps_and_distance_algorithm();
			if(Encoder_Y>2147483646){sb++;}	
		 }
	 }

}








