#include "it.h"
#include "string.h"

///////////////////////////////////////////////////////nrf24数据发送发送float///////////////////////////////////////////////

#define UART_BUFFER_SIZE 256
#define START_BYTE1 0x5A
#define START_BYTE2 0xA5
#define END_BYTE1 0xA6
#define END_BYTE2 0x6A

uint8_t uart_tx_buffer[UART_BUFFER_SIZE]; // 发送缓冲区

union FloatBytes {
    float floatValue;
    uint8_t bytes[sizeof(float)];
};

float remote_states[16]; // 待发送的20个浮点数数据

void prepare_data(void) {
//在这里准备待发送的数据，将要发送的浮点数数据赋值给remote_states数组
//	memset(remote_states, 0, sizeof(remote_states));  // 清除数组内容

	remote_states[0] = Positioning.X_laps;
	remote_states[1] = Positioning.X_distance_mm;
	remote_states[2] = Positioning.Y_laps;
	remote_states[3] = Positioning.Y_distance_mm;

	remote_states[4] = P_stHi219m.Yaw;
	remote_states[5] = P_stHi219m.Pitch;
	remote_states[6] = P_stHi219m.Roll;
	
	remote_states[7] = P_stHi219m.Yaw_Gyo;
	remote_states[8] = P_stHi219m.Pitch_Gyo;
	remote_states[9] = P_stHi219m.Roll_Gyo;
	
	remote_states[10] = P_stHi219m.Last_Yaw;
	remote_states[11] = P_stHi219m.Last_Pitch;
	remote_states[12] = P_stHi219m.Last_Roll;
	
	remote_states[13] = P_stHi219m.Continuous_Pitch;
	remote_states[14] = P_stHi219m.Continuous_Roll;
	remote_states[15] = P_stHi219m.Continuous_Yaw;
}

void send_data(void) {
    // 在这里将待发送的数据转换为字节流，并发送
    union FloatBytes floatData;
    int i=0,j;
    int bufferIndex = 0;

    // 发送起始字节
    uart_tx_buffer[bufferIndex++] = START_BYTE1;
    uart_tx_buffer[bufferIndex++] = START_BYTE2;

    // 将float类型的数据转换为字节流存储到发送缓冲区
    for (i = 0; i < 16; i++) 
	{
        floatData.floatValue = remote_states[i];
        // 将4个字节存储到发送缓冲区
        for (j = 0; j < sizeof(float); j++) 
		{
            uart_tx_buffer[bufferIndex++] = floatData.bytes[j];
        }
    }
    // 发送结束字节
    uart_tx_buffer[bufferIndex++] = END_BYTE1;
    uart_tx_buffer[bufferIndex++] = END_BYTE2;

    // 发送数据   uart_tx_buffer[i]
    for (i = 0; i < bufferIndex; i++) 
	{
		USART_SendData(USART2, uart_tx_buffer[i]);
		while (USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET){}
    }
}


