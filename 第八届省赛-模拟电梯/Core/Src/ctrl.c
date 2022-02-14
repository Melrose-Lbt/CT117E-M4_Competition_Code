#include "ctrl.h"

Machine Info;
uint8_t floor_flag[4] = {0, 0, 0, 0};

uint8_t Adder(uint8_t *num){
	uint8_t sum=0;
	for(int i = 0; i<4;i++){
		sum = sum + num[i];
	}
	return sum;
}

void State_Init(void){
	Info.current_state = STAY;
	Info.Last_state = STAY;
	Info.current_floor = 1;
	Info.next_floor = 1;
	Info.press_start = 0;
	Info.press_permit = 1;
	Info.press_timeout = 0;
}
int Illegal(void){
	/* illegal */
	return 0;
}

void Opened_to_Next(){
	Info.press_permit = 0;
	if(Info.current_floor == 1 || Info.current_floor == 4){
		
		if(Info.current_floor == 1){
			if(Info.Last_state == UP){
				Illegal();
			}
			else if(Info.Last_state == DOWN){
									if(Adder(floor_flag)==0){
										Info.current_state = STAY;
										Info.press_permit = 1;
									}
									else{
										Info.current_state = WAITING;
									}
			}
		}
		else if(Info.current_floor == 4){
			if(Info.Last_state == DOWN){
				Illegal();
			}
			else if(Info.Last_state == UP){
									if(Adder(floor_flag)==0){
										Info.current_state = STAY;
										Info.press_permit = 1;
									}
									else{
										Info.current_state = WAITING;
									}
			}
		}
	}
	
	
	else{
		if(Adder(floor_flag)==0){
			Info.current_state = STAY;
			Info.press_permit = 1;
		}
		else{
			Info.current_state = WAITING;
		}
	}
}


void Waiting_to_closing(){
	Info.current_state = CLOSING;
}

void Stay_to_next(){
	if(Info.press_permit == 1){
		if(Info.press_start == 1){
			/* empty */
		}
		else if(Info.press_start == 0){
			Info.current_state = STAY;
		}
	}
	else{
		Info.current_state = STAY;
	}
}

void Closing_to_Closed(){
	Info.current_state = CLOSED;
}

void Closed_to_next(){
	if(Info.current_floor == 1 || Info.current_floor == 4){
		if(Info.current_floor == 1){
			Info.current_state = UP;
			Info.Last_state = Info.current_state;
		}
		else if(Info.current_floor == 4){
			Info.current_state = DOWN;
			Info.Last_state = Info.current_state;
		}
	}
	else{
		if(Info.Last_state == UP){
			/* empty */
		}
		else if(Info.Last_state == DOWN){
			/* empty */
		}
	}
}

void Up_to_Arrived(){
	Info.current_state = ARRIVE;
}

void Down_to_Arrived(){
	Info.current_state = ARRIVE;
}

void Arrived_to_Opening(){
	Info.current_state = OPENING;
}

void Opening_to_Opened(){
	Info.current_state = OPENED;
}

void Time_Handler(){
	static uint16_t wait_time = 0;
	static uint16_t door_time = 0;
	static uint16_t move_time = 0;
	static uint8_t init_flag = 1;
	static uint16_t press_interval = 0;
	if(init_flag == 1){
		Stay_to_next();
	}
	else{
		if(Info.current_state == STAY){
			Stay_to_next();
		}
		
		/* rethink */
		
		
		
		
		
		if(Info.current_state == CLOSING){
			door_time++;
			if(door_time == 4000){
				Closing_to_Closed();
				door_time = 0;
			}
		}
		
		if(Info.current_state == CLOSED){
			Closed_to_next();
		}
		
		if(Info.current_state == UP){
			move_time++;
			if(move_time == 6000){
				Up_to_Arrived();
				move_time = 0;
			}
		}
		else if(Info.current_state == DOWN){
			move_time++;
			if(move_time == 6000){
				Down_to_Arrived();
				move_time = 0;
			}
		}
		
		
		if(Info.current_state == ARRIVE){
			Arrived_to_Opening();
		}
		
		if(Info.current_state == OPENING){
			door_time++;
			if(door_time == 4000){
				Opening_to_Opened();
				door_time = 0;
			}
		}
		
		if(Info.current_state == OPENED){
			Opened_to_Next();
		}
		
	}
	
}
