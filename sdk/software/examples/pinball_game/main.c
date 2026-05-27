#include <stdio.h> 
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "common_func.h"
#include "dvi.h"
#include "seg7.h"
#include "led.h"
#include "core_time.h"

#define CONFREG_INT_EN_ADDR     0xbf20f000u
#define CONFREG_INT_EDGE_ADDR   0xbf20f004u
#define CONFREG_INT_POL_ADDR    0xbf20f008u
#define CONFREG_INT_CLR_ADDR    0xbf20f00cu
#define CONFREG_TIMER_CMP_ADDR  0xbf20f104u
#define CONFREG_TIMER_EN_ADDR   0xbf20f108u
#define CONFREG_TIMER_CLR_ADDR  0xbf20f10cu

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

volatile int Plane_x = 400;
int Plane_y = 400;
int Plane_l = 100;
int Plane_w = 5;

// ball move direction
int Ball_dx = 2;
int Ball_dy = 2;

// ball move direction
int Plane_dx = 20;

int score = 0;

#define DEFAULT_DELAY_TIME 10
#define MIN_DELAY_TIME 1
#define MAX_DELAY_TIME 20

volatile int delay_time = DEFAULT_DELAY_TIME;

void Timer_IntrHandler(void);
void Button_IntrHandler(unsigned char button_state);

void showGameOver(void);

static inline void ClearButtonInterrupt(unsigned char button_mask)
{
	RegWrite(CONFREG_INT_CLR_ADDR, button_mask);
}

static inline void ClearTimerInterrupt(void)
{
	RegWrite(CONFREG_TIMER_CLR_ADDR, 0x1);
}

int chooseFlag = 1;

volatile int flag = 1;

void InterruptInit(void)
{
	// Enable button and timer Interrupt
	RegWrite(CONFREG_INT_EDGE_ADDR, 0x0f);//edge
	RegWrite(CONFREG_INT_POL_ADDR, 0x1f);//pol
	RegWrite(CONFREG_INT_CLR_ADDR, 0x1f);//clr
	RegWrite(CONFREG_INT_EN_ADDR, 0x1f);//en

	RegWrite(CONFREG_TIMER_CMP_ADDR, 25000000);//timercmp 500ms
	RegWrite(CONFREG_TIMER_EN_ADDR, 0x1);//timeren
}

void showGameOver(void);

void chooseTime(void)
{
	printf("Please Choosetime !!\n");
    setSegNum(1, delay_time / 10, 1, delay_time % 10);
    while (flag);
	if (delay_time < MIN_DELAY_TIME) {
		delay_time = DEFAULT_DELAY_TIME;
	}
	printf("Choosetime:%d\n",delay_time);
    chooseFlag = 0;
    setSegNum(0,0,0,0);
}

int main(int argc, char** argv)
{
	InterruptInit();

	chooseTime();

	while (1) {

        DVI_Draw_Rect(Plane_x,Plane_y,Plane_l,Plane_w);

        DVI_Draw_SQU(Ball_x,Ball_y,Ball_r);
        
        delay_ms(delay_time);

        // refresh place
        Ball_x += Ball_dx;
        Ball_y += Ball_dy;

        delay_ms(delay_time);

        if(Ball_y < 2)
        {
            Ball_y = 2;
            Ball_dy = -Ball_dy;
        }

        if(Ball_y > 600)
        {
            Ball_y = 600;
            Ball_dy = -Ball_dy;
        }

        if(Ball_x < 2)
        {
            Ball_x = 2;
            Ball_dx = -Ball_dx;
        }

        if(Ball_x > 800)
        {
            Ball_x = 800;
            Ball_dx = -Ball_dx;
        }

        if( Ball_y > 410 )
            break;

        if((Plane_y - Ball_y) < 2 && (Ball_x > (Plane_x - Plane_l)) && (Ball_x < (Plane_x + Plane_l))) 
        {
            Ball_dy = -Ball_dy;
            score++;
            setSegNum(0, score / 10, 1, score % 10); // 实时刷新分数到数码管
        }
        
    }

    showGameOver();

    return 0;

}

void HWI0_IntrHandler(void) { Button_IntrHandler(BUTTON1_MASK); }
void HWI1_IntrHandler(void) { Button_IntrHandler(BUTTON2_MASK); }
void HWI2_IntrHandler(void) { Button_IntrHandler(BUTTON3_MASK); }
void HWI3_IntrHandler(void) { Button_IntrHandler(BUTTON4_MASK); }
void HWI4_IntrHandler(void) { Timer_IntrHandler(); }

void Button_IntrHandler(unsigned char button_state)
{
	if (chooseFlag)
    {
        if (button_state & BUTTON1_MASK)
        {
            delay_time++;        
        
            if (delay_time > MAX_DELAY_TIME)
            {
                delay_time = MAX_DELAY_TIME;
            }
			printf("button1\n");
			ClearButtonInterrupt(BUTTON1_MASK);

        }else if (button_state & BUTTON4_MASK)
        {
            delay_time--;
            if (delay_time < MIN_DELAY_TIME)
            {
                delay_time = MIN_DELAY_TIME;
            }
			printf("button4\n");
			ClearButtonInterrupt(BUTTON4_MASK);
		}else if (button_state & BUTTON3_MASK)
        {
            if (delay_time < MIN_DELAY_TIME)
            {
                delay_time = DEFAULT_DELAY_TIME;
            }
            flag = 0;
			printf("button3\n");
			ClearButtonInterrupt(BUTTON3_MASK);
        }else{
			printf("button2\n");
			ClearButtonInterrupt(BUTTON2_MASK);
		}

        setSegNum(1,delay_time/10,1,delay_time%10);

    }
    
    if (!chooseFlag)
    {

        if (button_state & BUTTON1_MASK)
        {
            Plane_x += Plane_dx;
            if (Plane_x + Plane_l > 800)
            {
                Plane_x = 800 - Plane_l;
            }
			ClearButtonInterrupt(BUTTON1_MASK);
		}else if (button_state & BUTTON4_MASK)
        {
            Plane_x -= Plane_dx;
            if (Plane_x - Plane_l < 0)
            {
                Plane_x = Plane_l;
            }
			ClearButtonInterrupt(BUTTON4_MASK);
		}else if (button_state & BUTTON3_MASK){
			ClearButtonInterrupt(BUTTON3_MASK);
		}else if (button_state & BUTTON2_MASK){
			ClearButtonInterrupt(BUTTON2_MASK);
		}
    }
}

void showGameOver(void)
{
    printf("GameOver\n");
    printf("Your Score: %d\n", score); // 串口输出分数

    setSegNum(0, score / 10, 0, score % 10); // 数码管显示分数

    while (1)
    {
        setLedPin(0b1000000000000001);delay_ms(50);
        setLedPin(0b1100000000000011);delay_ms(50);
        setLedPin(0b1110000000000111);delay_ms(50);
        setLedPin(0b1111000000001111);delay_ms(50);
        setLedPin(0b1111100000011111);delay_ms(50);
        setLedPin(0b1111110000111111);delay_ms(50);
        setLedPin(0b1111111001111111);delay_ms(50);
        setLedPin(0b1111111111111111);delay_ms(50);
        setLedPin(0b1111111001111111);delay_ms(50);
        setLedPin(0b1111110000111111);delay_ms(50);
        setLedPin(0b1111100000011111);delay_ms(50);
        setLedPin(0b1111000000001111);delay_ms(50);
        setLedPin(0b1110000000000111);delay_ms(50);
        setLedPin(0b1100000000000011);delay_ms(50);
        setLedPin(0b1000000000000001);delay_ms(50);
    }
}
void Timer_IntrHandler(void)
{
     ClearTimerInterrupt();

}
// ...existing code...