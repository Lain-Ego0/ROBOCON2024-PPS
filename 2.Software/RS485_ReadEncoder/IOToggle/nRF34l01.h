//@file    nRF34l01.h
uchar SPI_RW(uchar byte)
{
	uchar bit_ctr;
   	for(bit_ctr=0;bit_ctr<8;bit_ctr++)   // output 8-bit    // 输出 8 位
   	{
   		MOSI = (byte & 0x80);         // output 'byte', MSB to MOSI		   // 从MOSI脚输出,从高位到低位.当BYTE 最高位为1时逻辑运算为1,输出1.运算为0时输出0
   		byte = (byte << 1);           // shift next bit into MSB..	    // 左移1位.将低位向高位转移.
   		SCK = 1;                      // Set SCK high..	   // 将SCK脚拉高,
   		byte |= MISO;       		  //byte=byte|MISO  capture current MISO bit	 	// 从MISO 中读出状态位存入BYTE中.
   		SCK = 0;            		  // ..then set SCK low again		// 将SCK脚拉低.发送一位完成.
   	}
    return(byte);           		  // return read byte	  	// 返回状态位
}