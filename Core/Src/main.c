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

	//string for the time displaying
	char str[16];

	//variables for hours, minutes, seconds and milliseconds
	uint8_t HH = 0, MM = 0, SS = 0, MS = 0;

	//start flag variable
	uint8_t start_flag = 0;

	//variables for string length and n is for looping
	uint8_t Length;

	//initializing the LCD
	Alcd_Init(&lcd, 2, 16);

	//initializing the Keypad
	Keypad_Matrix_Init(&key);

	//clear the display
	Alcd_Clear(&lcd);

	//display the instructions for operation
	Alcd_PutAt(&lcd, 0, 0, "Press & Hold");
	HAL_Delay(2000);
	Alcd_Clear(&lcd);

	Alcd_PutAt(&lcd, 0, 0, "Start: 4 Stop: 5");
	Alcd_PutAt(&lcd, 1, 0, "Reset: 6");
	HAL_Delay(2000);

	//clear the instructions
	Alcd_Clear(&lcd);

	//display the time
	Length = sprintf(str, "%02d:%02d:%02d", HH, MM, SS);
	Alcd_PutAt(&lcd, 0, 0, "waiting 4 input");
	Alcd_PutAt_n(&lcd, 1, 0, str, Length);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {

		//refresh the keypad
		Keypad_Matrix_Refresh(&key);

		//if 4 is pressed -> start
		if (Keypad_Matrix_Read_Key(&key, 4)) {

			//set the start flag to 1
			start_flag = 1;

			//if 5 is pressed -> stop
		} else if (Keypad_Matrix_Read_Key(&key, 5)) {

			//set the flag to zero
			start_flag = 0;

			//display the time
			Length = sprintf(str, "%02d:%02d:%02d:%02d", HH, MM, SS, MS);
			Alcd_Clear(&lcd);
			Alcd_PutAt(&lcd, 0, 0, "Stopped");
			Alcd_PutAt_n(&lcd, 1, 0, str, Length);

		}

		// 6 is pressed and stop watch is counting -> reset
		if (Keypad_Matrix_Read_Key(&key, 6)) {

			//set the flag to zero
			start_flag = 0;

			//resetting the time to zero
			MS = SS = MM = HH = 0;

			//displaying the time
			Length = sprintf(str, "%02d:%02d:%02d:%02d", HH, MM, SS, MS);
			Alcd_Clear(&lcd);
			Alcd_PutAt(&lcd, 0, 0, "Reset");
			Alcd_PutAt_n(&lcd, 1, 0, str, Length);
		}

		//in case that the start flag is 1 --> start counting
		if (start_flag == 1) {

			//delay for 100 ms -> this time is adjusted to compensate cycles delay
			HAL_Delay(100);

			//increment milliseconds by 100 -> it is 10 because the resolution is
			// 2 digits in the display only
			MS = MS + 10;

			//in case 1 second elapsed
			if (MS > 99) {

				//decrement 1 second from the milliseconds counter (1000 ms)
				MS = MS - 100;

				//increment seconds by 1
				SS++;

				//if 1 minute elapsed
				if (SS > 59) {

					//reset seconds counter
					SS = 0;

					//increment minutes by 1
					MM++;

					//if 1 hour elapsed
					if (MM > 59) {

						//reset minutes counter
						MM = 0;

						//increment hours by 1
						HH++;

						//if reached 100 -> display maximum capacity reached
						if (HH > 99) {

							//set the start flag to 0 to stop counting
							start_flag = 0;

							//display a message
							Alcd_Clear(&lcd);
							Alcd_PutAt(&lcd, 0, 0, "Stopped");
							Alcd_PutAt(&lcd, 1, 0, "Overflow");

						}

					}

				}

			}
			//displaying the time
			Alcd_Clear(&lcd);
			Length = sprintf(str, "%02d:%02d:%02d:%02d", HH, MM, SS, MS);
			Alcd_PutAt(&lcd, 0, 0, "Running");
			Alcd_PutAt_n(&lcd, 1, 0, str, Length);

		}

		/* USER CODE BEGIN 3 */
		/* USER CODE END WHILE */

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
