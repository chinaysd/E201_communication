#include "bsp_uart.h"




void Uart_Init(void)
{
    unsigned int baud_set = 0;
    OTCON = 0XC0;
    SSCON0 = 0X50;
    baud_set = (unsigned int)(SYS_FRE_SET/16/BSP_BAURATE);
    SSCON1 = baud_set%256;
    SSCON2 = baud_set/256;
    IE1 = 0x01;      //开启SSI中断
    P2CON |= 0X01;	// 必须设置IO口，否则则为输入状态
    P20 = 1; 
    EA = 1;    
}

