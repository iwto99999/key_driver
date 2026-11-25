//
//  按键状态机
//

#include "stm32f10x.h"
#include "keydrive.h"
#include "Clock.h"
#include "usart.h"

void TIM2_Init_APP(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period  = 1000-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72-1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;//高级定时器有
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	TIM_Cmd(TIM2, ENABLE);
	
	TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStruct = {0};
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
	
}

volatile uint32_t g_mstick = 0;
volatile uint8_t flag = 0;	//执行key_scan 标志位
uint8_t cut = 0;
volatile uint32_t second_count = 0 ;
volatile uint8_t second = 0;
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		
		g_mstick ++;
		cut++;
		second_count ++;
		if(second_count >=1000)
		{
			second_count = 0;
			second++;
			if(second>=60)
			{
				second =0;
			}
		}
		
		if(cut == 30)
		{
			cut = 0;
			flag = 1;
		}		
	}
}

void key_IOinit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

// 按键结构体初始化
key_s key_ss[key_length];

void key_Init(void)
{
	for(uint8_t i=0; i<key_length; i++)
	{
		key_ss[i].State = idle;
		key_ss[i].key_event = 0;
		key_ss[i].last_time = 0;
		key_ss[i].key_count = 20;
	}
}

// 函数：读按键引脚的状态
// 参数：按键号数 (1、2、3)
// 返回值：SET or RESET
uint8_t key_readIO(uint8_t key_num)
{
	if(key_num == 0)
	{
		return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1);		
	}

	if(key_num == 1)
	{
		return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2);
	}
	
	if(key_num == 2)
	{
		return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
	}
	
	return Bit_SET ;
}





// 按键状态判断
// 参数：按键结构体数组号数  按键号数
void key_statejudge(key_s *key, uint8_t key_num)
    {
	
	switch (key->State)
	{
		case idle:
		{
			if(key_readIO(key_num) == Bit_RESET)
			{
				key->State = press_debouce;
				key->last_time = g_mstick ;
			}
			break;
		}
		
		case press_debouce:
		{
			if(key_readIO(key_num) == Bit_SET)
			{
				key->State = idle;
			}
			else if(g_mstick - key->last_time >= key->key_count)
			{
				key->State = pressed;
				key->key_event = 1;
			}
			break;
		}
		
		case pressed:
		{
			if(key_readIO(key_num) == Bit_SET)
			{
				key->State = release_debouse;
				key->last_time = g_mstick ;
			}
			break;
		}

		case release_debouse:
		{
			if(key_readIO(key_num) == Bit_RESET)
			{
				key->State = pressed;
			}
			else if(g_mstick - key->last_time  >= key->key_count)
			{
				key->State = idle;
			}
			break;
		}		
	}
}


// 扫描按键
void key_scan(void)
{
	for(uint8_t i=0; i<key_length; i++)
	{
		key_statejudge(&key_ss[i], i);
	}
}


void key_event_process(void)
{
	if(key_ss[0].key_event == 1)
	{
		key_ss[0].key_event = 0;
		USART_SendData(USART1, 'P');
		
	}
	
	if(key_ss[1].key_event == 1)
	{
		key_ss[1].key_event = 0;
		USART_SendData(USART1, 'Q');

	}
	
	if(key_ss[2].key_event == 1)
	{
		key_ss[2].key_event = 0;
		USART_SendData(USART1, 'R');
	}
}



