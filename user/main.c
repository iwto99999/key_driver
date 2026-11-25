#include "stm32f10x.h"
#include "keydrive.h"
#include "delay.h"
#include "Clock.h"
#include "OLED.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>



void APP_USART_Init(void);
//void MY_SI2C_Init(void);
//void My_ScreenDisplay_Init(void);
//int si2c_write_bytes(uint8_t addr, const uint8_t *pdata, uint16_t size);

int main(void)
{
//	APP_USART_Init();
//	MY_SI2C_Init();
//	My_ScreenDisplay_Init();

	OLED_Init();
	key_IOinit();
	key_Init();
	TIM2_Init_APP();
	
	// anpin

	while(1)
	{
		if(flag == 1)
		{
			flag =0;
			key_scan();
			key_event_process();
		}

	}
}

//void MY_SI2C_Init(void)
//{
//	si2c.SCL_GPIOx = GPIOB ;
//	si2c.SCL_GPIO_Pin = GPIO_Pin_0;
//	si2c.SDA_GPIOx = GPIOB;
//	si2c.SDA_GPIO_Pin = GPIO_Pin_1;
//	My_SI2C_Init(&si2c);
//}

//int si2c_write_bytes(uint8_t addr, const uint8_t *pdata, uint16_t size)
//{
//	return My_SI2C_SendBytes(&si2c, addr, pdata, size);
//}

//void My_ScreenDisplay_Init(void)
//{
//	OLED_InitTypeDef OLED_InitStruct;
//	OLED_InitStruct.i2c_write_cb = si2c_write_bytes;
//	OLED_Init(&oled,&OLED_InitStruct);
//}

void APP_USART_Init(void)
{
   // #1.初始化PA6 PA7
	
	// #1.1 使能AFIO  对USART1进行重映射
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
	
	// #1.2 初始化PA6--AF_PP PA7--IPU or IN_FLOATING;全双工模式  
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	// PA6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ;
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// PA7
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;// 输入上拉
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	
	 // #2.初始化USART1
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	 
	 USART_InitTypeDef USART_InitStruct = {0};
	 USART_InitStruct.USART_BaudRate = 115200;
	 USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	 USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	 USART_InitStruct.USART_Parity = USART_Parity_No;
	 USART_InitStruct.USART_StopBits = USART_StopBits_1;
	 USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	 
	 USART_Init(USART1, &USART_InitStruct);
	 
	 // #3.闭合总开关
	 USART_Cmd(USART1, ENABLE);
}
