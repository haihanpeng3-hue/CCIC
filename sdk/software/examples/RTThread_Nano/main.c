#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <rtthread.h>
#include <rthw.h>
#include "led.h"

/*
*************************************************************************
*                               变量
*************************************************************************
*/
unsigned long UART_BASE = 0xbf000000UL;
unsigned long CONFREG_UART_BASE = 0xbfafff10UL;
int simu_flag;

/* 定义线程控制块指针 */
static rt_thread_t led1_thread = RT_NULL;
static rt_thread_t led2_thread = RT_NULL;

/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void led1_thread_entry(void* parameter);
static void led2_thread_entry(void* parameter);

/*
*************************************************************************
*                             main 函数
*************************************************************************
*/
/**
* @brief  主函数
* @param  无
* @retval 无
*/
int main(void)
{
    rt_kprintf("Hi, this is RT-Thread!!\n");

    simu_flag = RegRead(0xbf20f500);

    led1_thread =                          /* 线程控制块指针 */
    rt_thread_create( "led1",              /* 线程名字 */
                    led1_thread_entry,   /* 线程入口函数 */
                    RT_NULL,             /* 线程入口函数参数 */
                    1024,                 /* 线程栈大小 */
                    3,                   /* 线程的优先级 */
                    20);                 /* 线程时间片 */

    /* 启动线程，开启调度 */
    if (led1_thread != RT_NULL)
        rt_thread_startup(led1_thread);
    else
        return -1;

    led2_thread =                          /* 线程控制块指针 */
    rt_thread_create( "led2",              /* 线程名字 */
                    led2_thread_entry,   /* 线程入口函数 */
                    RT_NULL,             /* 线程入口函数参数 */
                    1024,                 /* 线程栈大小 */
                    4,                   /* 线程的优先级 */
                    20);                 /* 线程时间片 */

    /* 启动线程，开启调度 */
    if (led2_thread != RT_NULL)
        rt_thread_startup(led2_thread);
    else
        return -1;

    return 0;
}

/*
*************************************************************************
*                             线程定义
*************************************************************************
*/

static void led1_thread_entry(void* parameter)
{
    while (1)
    {
        toggleLedPin(0);
        if(simu_flag){
            rt_thread_mdelay(1);
        }
        else{
            rt_thread_mdelay(1000);
        }
        
        toggleLedPin(1);
        if(simu_flag){
            rt_thread_mdelay(1);
        }
        else{
            rt_thread_mdelay(1000);
        }

    }
}

static void led2_thread_entry(void* parameter)
{
    while (1)
    {
        toggleLedPin(2);
        if(simu_flag){
            rt_thread_mdelay(1);
        }
        else{
            rt_thread_mdelay(1000);
        }

        toggleLedPin(3);
        if(simu_flag){
            rt_thread_mdelay(1);
        }
        else{
            rt_thread_mdelay(1000);
        }

    }
}
/****************************END OF FILE****************************/