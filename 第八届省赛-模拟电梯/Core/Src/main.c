/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "ctrl.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t next_floor_buffer[4] = {0, 0, 0, 0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
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
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	char buffer[30];
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
	LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
	LED_All_Off();
	State_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
		
		for(int i = 0; i<4;i++){
			if(floor_flag[i] == 1){
				LEDx_On(i);
			}
			else if(floor_flag[i] == 0){
				LEDx_Off(i);
			}
		}
		
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
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV6;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
