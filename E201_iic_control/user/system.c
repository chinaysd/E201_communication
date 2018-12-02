#include "system.h"

#define APP_SET_BACKLIGHT(X)		( App_Para.Tp_LedSta &= (unsigned char)(X))
#define APP_CLR_BACKLIGHT(X)		( App_Para.Tp_LedSta |= (unsigned char)(~X))


unsigned char Uart0SendFlag,Uart0ReceiveFlag;

unsigned char Rev_Buf;

TIMEOUT_PARA TimeOut_Para[2];

static xdata App_Para_t App_Para;
static xdata IIC_PARA_t I2c_Para[IIC_NUM];


void System_Init(void)
{
   int i;
   TimeOutDet_Init();
   Led_Init(); 
   LED1_SET(1);
   for(i = 0;i < IIC_NUM;i ++){
		I2c_Para[i].ID = i;
		if(i == 0){
			I2c_Para[i].read_addr = I2C_READ_ADDR;
			I2c_Para[i].write_addr = I2C_WRITE_ADDR;
		}
		IIC_Init(&I2c_Para[i]);
	}
	P1PH |= 0X80; // i2c int 
   APP_CLR_BACKLIGHT(0x40);	//0x40    0xbf
   IIC_Write_Data(&I2c_Para[0], I2c_Para[0].write_addr, App_Para.Tp_LedSta);
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
  #if 0
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
        IIC_Write_Data(&I2c_Para[0], I2c_Para[0].write_addr, App_Para.Tp_LedSta);           
     }
  }
  #endif
  	if(!Get_I2C_INT()){
		IIC_Read_Data(&I2c_Para[0], I2c_Para[0].read_addr, &App_Para.TP_GetKey);
	
		switch(App_Para.TP_GetKey){
			case 0x01:{
				if(App_Para.Tp_CurKey != 0x01){
					App_Para.Tp_CurKey = 0x01;
					TimeOut_Record(&App_Para.LockLong_DetTimer[1], 100);
					App_Para.LockDetFlag[1] = 1;
				}
				break;
			}

			case 0x02:{
				if(App_Para.Tp_CurKey != 0x02){
					App_Para.Tp_CurKey = 0x02;
					//App_Key4Handle();//制冷/制热
					//App_Key1Handle(); //制冷/制热
				}
				break;
			}

			case 0x04:{
				if(App_Para.Tp_CurKey != 0x04){
					App_Para.Tp_CurKey = 0x04;
					//App_Key5Handle();   //脚灯
				}
				break;
			}

			case 0x08:{
				if(App_Para.Tp_CurKey != 0x08){
					App_Para.Tp_CurKey = 0x08;
					//App_Key1Handle(); //制冷/制热
					//App_Key4Handle();//制冷/制热
				}
				break;
			}
			
			case 0x10:{
				if(App_Para.Tp_CurKey != 0x10){
					App_Para.Tp_CurKey = 0x10;
					//App_Key3Handle();
				}
				break;
			}

			case 0x20:{
				if(App_Para.Tp_CurKey != 0x20){
					App_Para.Tp_CurKey = 0x20;
					//App_Key2Handle();
				}
				break;
			}
			default:{
				if(App_Para.Tp_CurKey){
					App_Para.Tp_CurKey = 0;
					//LED1_PIN = OFF; 
					//LED2_PIN = OFF;
					//APP_CLR_BACKLIGHT(TP_KEY5_LED);
					//APP_CLR_BACKLIGHT(TP_KEY3_LED);
					IIC_Write_Data(&I2c_Para[0], I2c_Para[0].write_addr, App_Para.Tp_LedSta);
					App_Para.LockDetFlag[1] = 0;					
					//if(App_Para.Pushrod_Status != PUSHROD_STOP){
					//	App_Para.UpDataFlag = True;
					//	App_Para.Pushrod_Status = PUSHROD_STOP;
					//}
				}
				break;
			}
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

void I2c_Irq() interrupt 7{
	/*<主机发送数据>*/
	if( (SSCON0&0x07) == 0x02){
		unsigned char get_data = SSDAT;
		if(get_data != I2C_ADDR){
			//App_LedHandle(get_data);
		}
	}
	/*<主机接收数据>*/
	else if( (SSCON0&0x07) == 0x03 ){
		if(I2C_IntGet() == 0){
			I2C_IntSet(1);
		}
	}
	/*<必须手动清掉此位>*/	
	SSCON0 &= ~0X40;
}





