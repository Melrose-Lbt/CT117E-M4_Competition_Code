#include "ctrl.h"

Machine Info;
uint8_t floor_flag[4] = {0, 0, 0, 0};
uint8_t next_flag = 0;
char buffer[30];
uint8_t next_floor_buffer[4] = {0, 0, 0, 0};

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
	Info.up_or_down = 1;
}
int Illegal(void){
	/* illegal */
	return 0;
}

void Opened_to_Next(){
	if(Info.current_floor == 1 || Info.current_floor == 4){
		
		if(Info.current_floor == 1){
									if(Adder(floor_flag)==0){
										Info.current_state = STAY;
										Info.press_permit = 1;
									}
									else{
										Info.current_state = WAITING;
									}
			
		}
		else if(Info.current_floor == 4){
									if(Adder(floor_flag)==0){
										Info.current_state = STAY;
										Info.press_permit = 1;
									}
									else{
										Info.current_state = WAITING;
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

void Key_stop_handler(){
	static uint16_t key_cnt = 0;
	key_cnt++;
	if(key_cnt > 1000){
		Info.press_permit = 0;
		key_cnt = 0;
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
		if(Info.next_floor>Info.current_floor){
			Info.current_state = UP;
			Info.Last_state = Info.current_state;
		}
		else if(Info.next_floor < Info.current_floor){
			Info.current_state = DOWN;
			Info.Last_state = Info.current_state;
		}
		
	}
}

void Up_to_Arrived(){
	Info.current_state = ARRIVE;
  Info.current_floor = Info.next_floor;

	floor_flag[Info.current_floor - 1] = 0;
	
	Renew_buffer(Info.current_floor - 1);
	Info.next_floor = Find_min();
}

void Down_to_Arrived(){
	Info.current_state = ARRIVE;
	Info.current_floor = Info.next_floor;

	floor_flag[Info.current_floor - 1] = 0;
	Renew_buffer(Info.current_floor - 1);
	Info.next_floor = Find_max();
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
	static uint16_t press_interval = 0;
		
	
	
	if(Info.press_start == 1){
		if(Info.press_permit == 1){
			Info.current_state = STAY;
		}
		else if(Info.press_permit == 0 && next_flag == 0){
			Info.current_state = CLOSING;
		}
	}
	else if(Info.press_start == 0){
		Info.current_state = STAY;
	}
	
	
	
	
	
	
	
		if(Info.current_state == CLOSING){
			door_time++;
			if(door_time == 4000){
				Closing_to_Closed();
				next_flag = 1;
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
		
		if(Info.current_state == WAITING){
			wait_time++;
			if(wait_time == 2000){
				Waiting_to_closing();
				wait_time = 0;
			}
		}
}









void Renew_buffer(uint8_t pos){
	next_floor_buffer[pos] = 0;
}

uint8_t Find_min(){
	uint8_t min = 0;
	for(int i =0;i<4;i++){
		if(next_floor_buffer[i]!=0){
			min = next_floor_buffer[i];
			break;
		}
	}
	
	
	for(int i = 0;i<4;i++){
		if(next_floor_buffer[i] == 0){
			continue;
		}
		else{
			if(next_floor_buffer[i]>=min){
				continue;
			}
			else{
				min = next_floor_buffer[i];
			}
		}
	}
	return min;
}

uint8_t Find_max(){
	uint8_t max = 0;
	for(int i =0;i<4;i++){
		if(next_floor_buffer[i]!=0){
			max = next_floor_buffer[i];
			break;
		}
	}
	for(int i = 0;i<4;i++){
		if(next_floor_buffer[i] == 0){
			continue;
		}
		else{
			if(next_floor_buffer[i]<=max){
				continue;
			}
			else{
				max = next_floor_buffer[i];
			}
		}
	}
	return max;
}


void Init_next_floor_buffer(){
	for(int i=0;i<4;i++){
		if(floor_flag[i]==1){
			next_floor_buffer[i] = i+1;
		}
	}
	if(Info.up_or_down == 1){
		Info.next_floor = Find_min();
	}
	else if(Info.up_or_down == 0){
		Info.next_floor = Find_max();
	}
}

void debug_Info(){
	sprintf(buffer, "  last state:%d  ",Info.Last_state);
		LCD_DisplayStringLine(Line1, (uint8_t *)buffer);
		
		sprintf(buffer, "  sum floor:%d  ",Adder(floor_flag));
		LCD_DisplayStringLine(Line2, (uint8_t *)buffer);
		
		sprintf(buffer, "  next floor:%d  ",Info.next_floor);
		LCD_DisplayStringLine(Line3, (uint8_t *)buffer);
		
		sprintf(buffer, "  floor:%d  ",Info.current_floor);
		LCD_DisplayStringLine(Line4, (uint8_t *)buffer);
		
		sprintf(buffer, "  press_permit:%d  ",Info.press_permit);
		LCD_DisplayStringLine(Line5, (uint8_t *)buffer);
		
		sprintf(buffer, "  state:%d  ",Info.current_state);
		LCD_DisplayStringLine(Line6, (uint8_t *)buffer);
	
}
void Show_Screen(){
	
	LCD_DisplayStringLine(Line2, "  Current Stage  ");
	sprintf(buffer, "      %d      ",Info.current_floor);
	LCD_DisplayStringLine(Line4, (uint8_t *)buffer);
}


void main_logic(){
	if(Info.press_permit == 1){
		
			if(Key_Status[0]==Key_pressed && floor_flag[0]==0 && Info.current_floor != 1){
				floor_flag[0] = 1;
			}
			if(Key_Status[1]==Key_pressed && floor_flag[1]==0 && Info.current_floor != 2){
				floor_flag[1] = 1;
			}
			if(Key_Status[2]==Key_pressed && floor_flag[2]==0 && Info.current_floor != 3){
				floor_flag[2] = 1;
			}
			if(Key_Status[3]==Key_pressed && floor_flag[3]==0 && Info.current_floor != 4){
				floor_flag[3] = 1;
			}
			Init_next_floor_buffer();
			
		}
		
		if(Info.current_state == STAY){
			if(Info.current_floor == 1){
				Info.up_or_down = 1;
			}
			else if(Info.current_floor == 4){
				Info.up_or_down = 0;
			}
			else if(Info.current_floor == 2 || Info.current_floor == 3){
				uint8_t max = Find_max();
				if(max > Info.current_floor){
					Info.up_or_down = 1;
				}
				else if(max < Info.current_floor){
					Info.up_or_down =0;
				}
			}
		}
		if(Info.current_state == OPENING){
			LEDx_Off(7);
			LEDx_Off(6);
			LEDx_Off(5);
			LEDx_Off(4);
			indexes = 7;
		}
		
		if(Info.current_state == UP){
			LEDx_UPflow();
		}
		if(Info.current_state == DOWN){
			LEDx_Downflow();
		}
		
		for(int i = 0; i<4;i++){
			if(floor_flag[i] == 1){
				LEDx_On(i);
			}
			else if(floor_flag[i] == 0){
				LEDx_Off(i);
			}
		}
		
		debug_Info();
		
}
	
