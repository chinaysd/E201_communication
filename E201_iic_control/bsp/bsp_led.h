#ifndef _BSP_LED_H_
#define _BSP_LED_H_
#include "sc93f833x_c.h"


#define LED1_PORT    P5
#define LED1_PIN     P51
#define LED1_SET(X)  ((X)?(LED1_PIN=X):(LED1_PIN=X))


void Led_Init(void);




#endif