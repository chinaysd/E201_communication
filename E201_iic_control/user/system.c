#include "system.h"

unsigned char Uart0SendFlag,Uart0ReceiveFlag;

unsigned char Rev_Buf;

TIMEOUT_PARA TimeOut_Para[2];

void System_Init(void)
{
   TimeOutDet_Init();
   Led_Init(); 
   LED1_SET(1);    
   Timer_Init();
   Uart_Init(); 
}

void Send_Byte(unsigned char DATA)
{
    SSDAT = DATA;
    while(!(Uart0SendFlag));
    Uart0SendFlag = 0;
}

unsigned char Rev_Byte(void)
{
   return SSDAT;    
}


void System_Handle(void)
{
  static unsigned int Cnt; 
  if(0x77 == Rev_Byte())
  {
    LED1_SET(0);   
  }
  
  if(TimeOutDet_Check(&TimeOut_Para[0]))
  {
     TimeOut_Record(&TimeOut_Para[0],100);
     ++ Cnt;
     if(Cnt & 0x01)
     {
        //
        Send_Byte(0X55);     
     }
     else 
     {
        //;            
     }
  }
  
}

/**************************************************
*函数名称：void timer0/1() interrupt 1/3
*函数功能：定时器中断产生方波
*入口参数：void
*出口参数：void
**************************************************/
void timer0() interrupt 1
{
    TL0 = (65536 - 1000)%256;    
	TH0 = (65536 - 1000)/256;
    TimeOutDet_DecHandle();
}

void timer1() interrupt 3
{
    TL1 = (65536 - 24000)%256;
	TH1 = (65536 - 24000)/256;
	
}

void Timer2Int(void) interrupt 5
{		
	TF2 = 0;   //溢出清零
    
}

/*****************************************************
*函数名称：void UartInt(void) interrupt 4
*函数功能：Uart0中断函数
*入口参数：void
*出口参数：void
*****************************************************/
void Bsp_Uart1IqrHandle() interrupt 7
{
   if(SSCON0&0x02)
   {
        SSCON0&=0xFD;
        Uart0SendFlag = 1;
   }
   	if((SSCON0&0x01)){
		SSCON0&=0xFE;
        Uart0ReceiveFlag = 1;
    }
}
