#ifndef _KEYDRIVE_H_
#define _KEYDRIVE_H_

#include "stm32f10x.h"

#define key_length 3
                                       
// 状态枚举
typedef enum
{
	idle = 0,
	press_debouce,
	pressed,
	release_debouse
}KEY_STATE;

// 按键结构体
typedef struct
{
	KEY_STATE State;
	uint8_t key_event;
	uint32_t last_time;
	uint8_t key_count;
	
}key_s;


extern key_s key_ss[key_length];
extern volatile uint8_t flag;
extern volatile uint8_t second;

// tim和key初始化
void TIM2_Init_APP(void);
void key_IOinit(void);


void key_Init(void);	//	key结构体初始化
uint8_t key_readIO(uint8_t key_num);// 读按键引脚的状态
void key_statejudge(key_s *key, uint8_t key_num);
void key_scan(void);
void key_event_process(void);

#endif

