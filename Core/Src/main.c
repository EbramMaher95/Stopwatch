/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

///
/// defining the matrix keypad pins and port
/// it is on port B
Keypad_Matrix key = { .Row_Port = GPIOB, .Row_Start_Pin = 12, .Col_Port = GPIOB,
		.Col_Start_Pin = 6, .Col = 4, .Row = 4,

};

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV privant variance*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

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

	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	///
	/// defining the LCD pins and ports
	Alcd_t lcd = { .Data_GPIO = GPIOA, .Data_GPIO_Start_Pin = 0, .RS_GPIO =
	GPIOA, .RS_GPIO_Pin = GPIO_PIN_4, .EN_GPIO = GPIOA, .EN_GPIO_Pin =
	GPIO_PIN_5, };

	char str_H[2], str_M[2], str_S[2];

	//variables for hours, minutes and seconds
	uint8_t HH, MM, SS;

	//start flag variable
	uint8_t start_flag = 0;

	//initializing the LCD
	Alcd_Init(&lcd, 2, 16);

	//initializing the Keypad
	Keypad_Matrix_Init(&key);

	Alcd_Clear(&lcd);

	//displaying the instructions for 5 seconds
	Alcd_PutAt(&lcd, 0, 0, "Start: 1 Stop: 2");
	Alcd_PutAt(&lcd, 1, 0, "Reset: 3");
	HAL_Delay(5000);

	//clear the instructions
	Alcd_Clear(&lcd);

	//displaying the time

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		Keypad_Matrix_Refresh(&key);

		//case start: 1 is selected and time is zero
		//this means if 1 is selected but the time is not zero, nothing happens
		if (Keypad_Matrix_Read_Key(&key, 1) && (HH = 0) && (MM = 0)
				&& (SS = 0)) {

			//set the start flag to 1
			start_flag = 1;

			while (start_flag == 1) {
				//increasing the time
				HAL_Delay(1000); //a second elapsed
				SS++;
				if (SS == 60) {
					SS = 0;
					MM++;
					if (MM >= 60) {
						MM = 0;
						HH++;
					}

					//convert the hours, minutes and seconds into string
					itoa(HH, str_H, 10);
					itoa(MM, str_M, 10);
					itoa(SS, str_S, 10);

					//displaying the time
					Alcd_Clear(&lcd);
					Alcd_PutAt(&lcd, 0, 0, "Running");
					Alcd_PutAt(&lcd, 1, 0, str_H);
					Alcd_PutAt(&lcd, 1, 2, ":");
					Alcd_PutAt(&lcd, 1, 3, str_M);
					Alcd_PutAt(&lcd, 1, 5, ":");
					Alcd_PutAt(&lcd, 1, 6, str_H);

					//check if any button is pressed
					if (Keypad_Matrix_Read_Key(&key, 2)
							| Keypad_Matrix_Read_Key(&key, 3)) {

						start_flag = 0;
					}

				}

			}
		}

		//stop is pressed (2) and stop watch is running
		//this means that if stop is pressed and the flag is not 1, nothing happens
		if (Keypad_Matrix_Read_Key(&key, 2) && (start_flag = 1)) {

			//set the start flag to zero
			start_flag = 0;

			//no incrementing happens to the time

			//convert the hours, minutes and seconds into string
			itoa(HH, str_H, 10);
			itoa(MM, str_M, 10);
			itoa(SS, str_S, 10);

			//displaying the time
			Alcd_Clear(&lcd);
			Alcd_PutAt(&lcd, 0, 0, "Stopped");
			Alcd_PutAt(&lcd, 1, 0, str_H);
			Alcd_PutAt(&lcd, 1, 2, ":");
			Alcd_PutAt(&lcd, 1, 3, str_M);
			Alcd_PutAt(&lcd, 1, 5, ":");
			Alcd_PutAt(&lcd, 1, 6, str_H);
		}

		//if the reset button (3) is pressed
		//the start flag is not taken into consideration
		if (Keypad_Matrix_Read_Key(&key, 3)) {

			//set the start flag to 0
			start_flag = 0;

			//reseting the time
			HH = 0, MM = 0, SS = 0;

			//convert the hours, minutes and seconds into string
			itoa(HH, str_H, 10);
			itoa(MM, str_M, 10);
			itoa(SS, str_S, 10);

			//displaying the time
			Alcd_Clear(&lcd);
			Alcd_PutAt(&lcd, 0, 0, "Reset");
			Alcd_PutAt(&lcd, 1, 0, str_H);
			Alcd_PutAt(&lcd, 1, 2, ":");
			Alcd_PutAt(&lcd, 1, 3, str_M);
			Alcd_PutAt(&lcd, 1, 5, ":");
			Alcd_PutAt(&lcd, 1, 6, str_H);

		}

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */

}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
