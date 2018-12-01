#include "bsp_timer.h"


void Timer_Init(void)
{
	TMCON = 0x00;		
	TMOD = 0x11;    
	TL0 = (65536 - 1000)%256;    
	TH0 = (65536 - 1000)/256;
	TR0 = 0;
	ET0 = 1;//定时器0允许
	TR0 = 1;//打开定时器0
}