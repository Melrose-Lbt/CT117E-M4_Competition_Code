#ifndef __LED_H
#define __LED_H

/* personal include */
#include "main.h"

/* function */
void LED_Init(void);
void LEDx_On(uint8_t num);
void LEDx_Off(uint8_t num);
void LEDx_Tuggle(uint8_t num, uint16_t time);
void LED_All_Off(void);


#endif
