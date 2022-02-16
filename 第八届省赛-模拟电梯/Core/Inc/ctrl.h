#ifndef __CTRL_H
#define __CTRL_H

/* includes */
#include "main.h"
#include "led.h"
#include "key.h"
#include "lcd.h"

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
	
	uint8_t up_or_down;
	
}Machine;

extern Machine Info;
extern uint8_t floor_flag[4];
extern uint8_t next_flag;

void State_Init(void);
void Opened_to_Next();
void Waiting_to_closing();
void Closing_to_Closed();
void Closed_to_next();
void Up_to_Arrived();
void Down_to_Arrived();
void Arrived_to_Opening();
void Opening_to_Opened();
uint8_t Adder(uint8_t *num);
extern uint8_t next_floor_buffer[4];
void Renew_buffer(uint8_t pos);
uint8_t Find_max();
uint8_t Find_min();

void main_logic();
void Time_Handler();
void Key_stop_handler();


#endif
