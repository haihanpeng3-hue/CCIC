#include <stdio.h> 
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "common_func.h"

void DMA_IntrHandler(void);
void Timer_IntrHandler(void);
void Button_IntrHandler(unsigned char button_state);

volatile int a = 0;

#define CPSIZE 12
unsigned int UART_BASE = 0xbf000000;
int src[CPSIZE]  = {1,2,3,4,5,6,7,8,9,10,11,12};
int dst[CPSIZE]  = {0,0,0,0,0,0,0,0,0,0,0,0};

int main(int argc, char** argv)
{	
	RegWrite(0xbf300004,&src);//dma saddr
	RegWrite(0xbf300008,&dst);//dma daddr
	RegWrite(0xbf30000c,CPSIZE);//dma length
    
	RegWrite(0xbf20f004,0x00);//edge
	RegWrite(0xbf20f008,0x20);//pol
	RegWrite(0xbf20f00c,0x20);//clr
	RegWrite(0xbf20f000,0x20);//en

	RegWrite(0xbf300000,0x1);//dma en

	while(1)
	{

	}

	return 0;
}

void HWI0_IntrHandler(void)
{	
	unsigned int int_state;
	int_state = RegRead(0xbf20f014);

	if((int_state & 0x10) == 0x10){
		Timer_IntrHandler();
	}
	else if((int_state & 0x20) == 0x20){
		DMA_IntrHandler();
	}
	else if(int_state & 0xf){
		Button_IntrHandler(int_state & 0xf);
	}
}

void DMA_IntrHandler(void)
{
	RegWrite(0xbf300000,0x2);
	RegWrite(0xbf20f00c, 0x20);
	
	printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", dst[0],dst[1],dst[2],dst[3],dst[4],dst[5],dst[6],dst[7],dst[8],dst[9],dst[10],dst[11]);
   
}

void Timer_IntrHandler(void)
{
	RegWrite(0xbf20f108,0);
	RegWrite(0xbf20f108,1);
	a = 5;
	printf("a=%d\n",a);
}

void Button_IntrHandler(unsigned char button_state)
{
	if((button_state & 0b1000) == 0b1000){
		a = 4;
		printf("a=%d\n",a);
		RegWrite(0xbf20f00c,0x8);//clr
	}
	else if((button_state & 0b0100) == 0b0100){
		a = 3;
		printf("a=%d\n",a);
		RegWrite(0xbf20f00c,0x4);//clr
	}
	else if((button_state & 0b0010) == 0b0010){
		a = 2;
		printf("a=%d\n",a);
		RegWrite(0xbf20f00c,0x2);//clr
	}
	else if((button_state & 0b0001) == 0b0001){
		a = 1;
		printf("a=%d\n",a);
		RegWrite(0xbf20f00c,0x1);//clr
	}
}