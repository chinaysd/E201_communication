#include "bsp_led.h"


void Led_Init(void)
{
   P5CON |= 0X02;
   P5PH  &= ~0X02;    
}

