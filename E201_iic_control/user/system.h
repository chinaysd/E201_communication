#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "sc93f833x_c.h"

#include "bsp_led.h"
#include "TimeOut.h"
#include "bsp_timer.h"
#include "bsp_uart.h"
#include "bsp_i2c_s.h"
#include "bsp_i2c.h"

#define Get_I2C_INT()				P17


#define I2C_READ_ADDR			0X5B
#define I2C_WRITE_ADDR			0X5A


typedef enum{
	STOP_MODE = 0,
	HEAT_MODE,
	COOL_MODE
}Heat_Cool_Sta_t;

typedef struct{
	unsigned char LockDetFlag[2],LockFlag;			/*<´¦Àí°´¼üËø¼ü>*/
	Heat_Cool_Sta_t HC_Sta;
	unsigned char TP_GetKey,Chg_GetKey;			
	unsigned char Tp_LedSta,Tp_LedStaBak;			/*<¼ÇÂ¼LEDµÆ×´Ì¬>*/
	unsigned char Tp_CurKey,Chg_CurKey;	
	TIMEOUT_PARA LockLong_DetTimer[2];
	unsigned int SentData;
	unsigned char FootLedSta;
    unsigned char MASSAGESta;
    unsigned char WarmSta;
	unsigned char Pushrod_Status;
	unsigned char UpDataFlag;
	unsigned char ShutDownFlag;
	unsigned char OnLineCnt,OnLineFlag;
    unsigned char MassAutoClsFlag,WarmAutoClsFlag;
	unsigned char CoolAutoClsFlag,HeatAutoClsFlag,SystemAutoClsFlag,
TpLedDisCntFlag;
	unsigned int CoolAutoCnt,HeatAutoCnt,SystemAutoCnt,TpLedDisCnt;
    unsigned int MassAutoCnt,WarmAutoCnt;
	
}App_Para_t;

void System_Init(void);
void System_Handle(void);







#endif
