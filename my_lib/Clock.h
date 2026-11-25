#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "stm32f10x.h"


// 当前模式
typedef enum
{
	MODE_NORMAL, // 正常界面
	MODE_SET		 // 时钟设置
}Clock_Mode_t;

// 要修改的项目
typedef enum
{
	SET_HOUR,
	SET_MINUTE	
}Clock_Set;


void Clock_display(void);

#endif

