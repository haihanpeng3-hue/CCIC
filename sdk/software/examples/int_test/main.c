#include <stdio.h> 
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "common_func.h"
#include "dvi.h"

#define CONFREG_INT_EN_ADDR     0xbf20f000u
#define CONFREG_INT_EDGE_ADDR   0xbf20f004u
#define CONFREG_INT_POL_ADDR    0xbf20f008u
#define CONFREG_INT_CLR_ADDR    0xbf20f00cu
#define CONFREG_TIMER_CMP_ADDR  0xbf20f104u
#define CONFREG_TIMER_EN_ADDR   0xbf20f108u
#define CONFREG_TIMER_CLR_ADDR  0xbf20f10cu
#define CONFREG_SIMU_FLAG_ADDR  0xbf20f500u

#define BUTTON1_MASK            0x1u
#define BUTTON2_MASK            0x2u
#define BUTTON3_MASK            0x4u
#define BUTTON4_MASK            0x8u

//BSP板级支持包所需全局变量
unsigned long UART_BASE = 0xbf000000;					//UART16550的虚地址
unsigned long CONFREG_TIMER_BASE = 0xbf20f100;			//CONFREG计数器的虚地址
unsigned long CONFREG_CLOCKS_PER_SEC = 50000000L;		//CONFREG时钟频率
unsigned long CORE_CLOCKS_PER_SEC = 33000000L;			//处理器核时钟频率


int Ball_x = 400;
int Ball_y = 100;
int Ball_r = 5;

int Plane_x = 400;
int Plane_y = 400;
int Plane_l = 100;
int Plane_w = 5;

int  simu_flag;
volatile unsigned char int_flag;

void Timer_IntrHandler(void);
void Button_IntrHandler(unsigned char button_state);

static inline void ClearButtonInterrupt(unsigned char button_mask)
{
	RegWrite(CONFREG_INT_CLR_ADDR, button_mask);
}

static inline void ClearTimerInterrupt(void)
{
	RegWrite(CONFREG_TIMER_CLR_ADDR, 0x1);
}

int main(int argc, char** argv)
{
	int_flag = 0;
	DVI_Draw_Rect(Plane_x,Plane_y,Plane_l,Plane_w);
    DVI_Draw_SQU(Ball_x,Ball_y,Ball_r);

	simu_flag = RegRead(CONFREG_SIMU_FLAG_ADDR);
	RegWrite(CONFREG_INT_EDGE_ADDR, 0x0f);
	RegWrite(CONFREG_INT_POL_ADDR, 0x1f);
	RegWrite(CONFREG_INT_CLR_ADDR, 0x1f);
	RegWrite(CONFREG_INT_EN_ADDR, 0x1f);
	if(simu_flag){
		RegWrite(CONFREG_TIMER_CMP_ADDR, 5000);//timercmp 0.1ms
	}
	else{
		RegWrite(CONFREG_TIMER_CMP_ADDR, 50000000);//timercmp 1s
	}
	RegWrite(CONFREG_TIMER_EN_ADDR, 0x1);//timeren

	while(int_flag == 0)
	{
		
	}

	return 0;
}

void HWI0_IntrHandler(void) { Button_IntrHandler(BUTTON1_MASK); }
void HWI1_IntrHandler(void) { Button_IntrHandler(BUTTON2_MASK); }
void HWI2_IntrHandler(void) { Button_IntrHandler(BUTTON3_MASK); }
void HWI3_IntrHandler(void) { Button_IntrHandler(BUTTON4_MASK); }
void HWI4_IntrHandler(void) { Timer_IntrHandler(); }

void Timer_IntrHandler(void)
{
	ClearTimerInterrupt();
	printf("timer int\n");
}

void Button_IntrHandler(unsigned char button_state)
{
	if((button_state & BUTTON4_MASK) == BUTTON4_MASK){
		printf("button4 int\n");
		int_flag = 1;
		ClearButtonInterrupt(BUTTON4_MASK);
	}
	else if((button_state & BUTTON3_MASK) == BUTTON3_MASK){
		printf("button3 int\n");
		ClearButtonInterrupt(BUTTON3_MASK);
	}
	else if((button_state & BUTTON2_MASK) == BUTTON2_MASK){
		printf("button2 int\n");
		ClearButtonInterrupt(BUTTON2_MASK);
	}
	else if((button_state & BUTTON1_MASK) == BUTTON1_MASK){
		printf("button1 int\n");
		ClearButtonInterrupt(BUTTON1_MASK);
	}
}
