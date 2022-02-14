#include "key.h"

uint16_t Key_Pin[4] = {KEY1_Pin, KEY2_Pin, KEY3_Pin, KEY4_Pin};
GPIO_TypeDef *Key_Port[4] = {KEY1_GPIO_Port, KEY2_GPIO_Port, KEY3_GPIO_Port, KEY4_GPIO_Port};
Key_status Key_Status[4] = {0, 0, 0, 0};
uint16_t Key_filter[4] = {0, 0, 0, 0};
uint16_t Key_cnt[4] = {0, 0, 0, 0};
uint16_t Key_interval[4] = {0, 0, 0, 0};

void Check(uint8_t num){
	if(HAL_GPIO_ReadPin(Key_Port[num], Key_Pin[num])==GPIO_PIN_RESET){
		Key_filter[num] <<= 1;
		Key_filter[num] |= 0x0001;
		if(Key_filter[num]>0x03FF){
			if(Key_Status[num] == Key_released){
				Key_Status[num] = Key_pressed;
			}
			if(Key_Status[num] == Key_longpressed){
				Key_cnt[num]++;
				if(Key_cnt[num]>Key_interval[num]){
					Key_Status[num] = Key_pressed;
					Key_cnt[num] = 0;
				}
			}
		}
	}
	else{
		Key_filter[num] >>= 1;
		if(Key_filter[num] == 0){
			Key_Status[num] = Key_released;
			Key_cnt[num] = 0;
		}
	}
}

void Check_All(void){
	for(int i = 0;i < 4;i++){
		Check(i);
	}
}

void press_once(uint8_t num){
	Key_Status[num] = Key_stay;
}

void press_hold(uint8_t num, uint16_t time){
	Key_interval[num] = time;
	Key_Status[num] = Key_longpressed;
}
