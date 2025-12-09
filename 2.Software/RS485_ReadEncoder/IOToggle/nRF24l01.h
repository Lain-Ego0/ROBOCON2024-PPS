////@file    nRF24l01.h

#define MOSI_1 GPIOB->ODR|=(1<<5);
#define MOSI_0 GPIOB->ODR&=~(1<<5);

#define CE_1 GPIOB->ODR|=(1<<7);
#define CE_0 GPIOB->ODR&=~(1<<7);

#define CSN_1 GPIOB->ODR|=(1<<6);
#define CSN_0 GPIOB->ODR&=~(1<<6);

#define SCK_1 GPIOB->ODR|=(1<<5);
#define SCK_0 GPIOB->ODR&=~(1<<5);



///*********************
//2401初始化
///*********************
void RF2401_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  	/*开时钟PWM的与GPIO的*/

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 	
  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	         //MISO
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      //上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	         //IRQ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      //上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	CSN_1;


//  	GPIOA->ODR&=~(1<<9);
//  	GPIOB->ODR|=(1<<12);
	
//	
//	DDR_SPI|=((1<<MOSI)|(1<<SCK)|(1<<CSN)|(1<<CE));	//设置MOSI、SCK、CSN输出，其余输入
//	DDR_SPI&=~((1<<MISO)|(1<<IRQ));	
//	PORT_SPI|=(1<<CSN);
}

//uchar SPI_RW(uchar byte)
//{
//	uchar bit_ctr;
//   	for(bit_ctr=0;bit_ctr<8;bit_ctr++)   // output 8-bit    // 输出 8 位
//   	{
//   		MOSI = (byte & 0x80);         // output 'byte', MSB to MOSI		   // 从MOSI脚输出,从高位到低位.当BYTE 最高位为1时逻辑运算为1,输出1.运算为0时输出0
//   		byte = (byte << 1);           // shift next bit into MSB..	    // 左移1位.将低位向高位转移.
//   		SCK = 1;                      // Set SCK high..	   // 将SCK脚拉高,
//   		byte |= MISO;       		  //byte=byte|MISO  capture current MISO bit	 	// 从MISO 中读出状态位存入BYTE中.
//   		SCK = 0;            		  // ..then set SCK low again		// 将SCK脚拉低.发送一位完成.
//   	}
//    return(byte);           		  // return read byte	  	// 返回状态位
//}

