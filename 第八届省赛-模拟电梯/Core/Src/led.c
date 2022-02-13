#include "led.h"

/* data structure */
uint16_t LED_Pin[8] = {LED1_Pin, LED2_Pin, LED3_Pin, LED4_Pin, LED5_Pin, LED6_Pin, LED7_Pin, LED8_Pin};
GPIO_TypeDef *LED_Port[8] = {LED1_GPIO_Port, LED2_GPIO_Port, LED3_GPIO_Port, LED4_GPIO_Port, LED5_GPIO_Port, LED6_GPIO_Port, LED7_GPIO_Port, LED8_GPIO_Port};

/* blockers between led and lcd */
void Lock_on(void){
	HAL_GPIO_WritePin(LED_EN_GPIO_Port, LED_EN_Pin, GPIO_PIN_SET);
}

void Lock_off(void){
	HAL_GPIO_WritePin(LED_EN_GPIO_Port, LED_EN_Pin, GPIO_PIN_RESET);
}

void LED_All_Off(void){
	Lock_on();
	GPIOC->ODR = 0xFF00;
	Lock_off();
}



/* higher api */
void LEDx_On(uint8_t num){
	
	Lock_on();
	HAL_GPIO_WritePin(LED_Port[num], LED_Pin[num], GPIO_PIN_RESET);
	Lock_off();
}

void LEDx_Off(uint8_t num){
	Lock_on();
	HAL_GPIO_WritePin(LED_Port[num], LED_Pin[num], GPIO_PIN_SET);
	Lock_off();
}


void LEDx_Tuggle(uint8_t num, uint16_t time){
	
	Lock_on();
	HAL_GPIO_TogglePin(LED_Port[num], LED_Pin[num]);
	Lock_off();
	HAL_Delay(time);
	
}




