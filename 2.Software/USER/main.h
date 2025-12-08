#ifndef __MAIN_H__
#define __MAIN_H__
///////////////////////////////////
//
//存放各种头文件
///////////////////////////////////
extern float AD_Value[12];
#include "stm32f10x.h"
#include <stdio.h>
#include "stdlib.h"
#include "stdbool.h"
#include <string.h>
#include <stdarg.h>
#include "sys.h"
//FreeRTOS
#include "delay.h"

//BSP
#include "usart.h"
#include "it.h"
#include "timer.h"
#include "Adc.h"
#include "gpio.h"
#include "dma.h"

//模块
#include "Key.h"
#include "Encoder.h"
#include "packet.h"
#include "imu_data_decode.h"
#include "hi219.h"

#endif 

