#ifndef __CTRL_H
#define __CTRL_H

/* includes */
#include "main.h"


/* state define */
typedef enum{
	STAY,
	ARRIVE,
	UP,
	DOWN,
	OPENING,
	CLOSING,
	OPENED,
	CLOSED,
	WAITING
}State;

typedef struct{
	State current_state;
	State Last_state;
	uint8_t current_floor;
	uint8_t next_floor;
	
	uint8_t press_start;  /* 1 start to press, 0 otherwise */
	uint8_t press_permit; /* 1 permit, 0 not permit */
	uint8_t press_timeout;
	
}Machine;




#endif
