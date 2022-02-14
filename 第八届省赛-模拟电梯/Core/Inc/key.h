#ifndef __KEY_H
#define __KEY_H

#include "main.h"

/* Key status */
typedef enum{
	Key_pressed,
	Key_released,
	Key_stay,
	Key_longpressed
}Key_status;

extern Key_status Key_Status[4];

void Check(uint8_t num);
void press_once(uint8_t num);
void press_hold(uint8_t num, uint16_t time);
void Check_All(void);



#endif
