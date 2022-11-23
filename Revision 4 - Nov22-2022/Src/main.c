/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "lcd.h"
#include <stdio.h>
#include "bmp180.h"
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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
extern void SysClockConfig(void);
int temperature, pressure;
char data_print[30];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  BMP180_init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  Lcd_PortType ports[] = { GPIOC, GPIOB, GPIOA, GPIOA };
  Lcd_PinType pins[] = {GPIO_PIN_7, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_6};
  Lcd_HandleTypeDef LCD = Lcd_create(ports, pins, GPIOB, GPIO_PIN_5, GPIOB, GPIO_PIN_4, LCD_4_BIT_MODE);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  int key_pressed(int row, int col)
  {
	  if(       (row == 1) && (col == 1) )
	  {
		  return 1; // 1 is clicked
	  }else if( (row == 1) && (col == 2) )
	  {
		  return 2; // 2 is clicked
	  }else if( (row == 1) && (col == 3) )
	  {
		  return 3; // 3 is clicked
	  }else if( (row == 2) && (col == 1) )
	  {
		  return 4; // 4 is clicked
	  }else if( (row == 2) && (col == 2) )
	  {
		  return 5; // 5 is clicked
	  }else if( (row == 2) && (col == 3) )
	  {
		  return 6; // 6 is clicked
	  }else if( (row == 3) && (col == 1) )
	  {
		  return 7; // 7 is clicked
	  }else if( (row == 3) && (col == 2) )
	  {
		  return 8; // 8 is clicked
	  }else if( (row == 3) && (col == 3) )
	  {
		  return 9; // 9 is clicked
	  }else if( (row == 4) && (col == 1) )
	  {
		  return (-10); // * is clicked
	  }else if( (row == 4) && (col == 2) )
	  {
		  return 0; // 0 is clicked
	  }else if( (row == 4) && (col == 3) )
	  {
		  return (-20); // # is clicked
	  }else
	  {
		  return 100; // If another key or no key was pressed
	  }
  }

  int get_col()
  {
  	  if( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET )
  	  {
  		  return 2;
  	  }else if( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_RESET )
  	  {
  		  return 1;
  	  }else if( HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_RESET )
  	  {
  		  return 3;
  	  }else
  	  {
  		  return 1000;
  	  }
  }

  int get_row()
  {
  	  if( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_RESET )
  	  {
  		  return 1;
  	  }else if( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == GPIO_PIN_RESET )
  	  {
  		  return 4;
  	  }else if( HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_RESET )
  	  {
  		  return 3;
  	  }else if( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_RESET )
  	  {
  		  return 2;
  	  }else
  	  {
  		  return 1000;
  	  }
  }

  // For the cursor function, first parameter is the row, second parameter is column
  for(int i = 0; i < 5; ++i)
  {
	  Lcd_cursor(&LCD, 0,3);
	  Lcd_string(&LCD, "Welcome to ");
	  Lcd_cursor(&LCD, 1,2);
	  Lcd_string(&LCD, "TravelAssist");
	  HAL_Delay(1000);
	  Lcd_clear(&LCD);
	  HAL_Delay(1000);
  }

  int time[] = {0, 0, 0};
  int col = 0;
  int row = 0;

  while (1)
  {
	  update_BMP180(OSS1);

	  // Get temperature and Pressure values from BMP180 sensor
	  double current_temp = get_temperature(); // In Celsius
	  Lcd_cursor(&LCD, 0,0);
	  Lcd_string(&LCD, "Enter * for C or");
	  Lcd_cursor(&LCD, 1,0);
	  Lcd_string(&LCD, "# for F: ");
	  HAL_Delay(500);
	  int key = key_pressed( get_row(), get_col() );

	  // If they choose Fahrenheit:
	  if( key == (-20) )
	  {
		  current_temp = (current_temp * 1.8) + 32;
	  }

	  temperature = (int) current_temp;
	  temperature -= 2;

	  int pressurepascal = get_pressure();
	  pressure = pressurepascal/1000;

	  Lcd_cursor(&LCD, 0,0);
	  Lcd_string(&LCD, "Temperature:");
	  Lcd_cursor(&LCD, 0,13);
	  Lcd_int(&LCD, temperature);
	  Lcd_cursor(&LCD, 0,15);

	  if( key == (-20) )
	  {
		  Lcd_string(&LCD, "F");
	  }else
	  {
		  Lcd_string(&LCD, "C");
	  }

	  Lcd_cursor(&LCD, 1,0);
	  Lcd_string(&LCD, "Pressure:");
	  Lcd_cursor(&LCD, 0,11);
	  Lcd_int(&LCD, pressure);
	  Lcd_cursor(&LCD, 0,13);
	  Lcd_string(&LCD, "kPa");

	  HAL_Delay(4000);
	  Lcd_clear(&LCD);
	  HAL_Delay(1000);



	  // Get the time that the user enters
	  Lcd_cursor(&LCD, 0,0);
	  Lcd_string(&LCD, "Enter time: ");
	  Lcd_cursor(&LCD, 1, 0);
	  Lcd_string(&LCD, "* for : in time");

	  int index = 0;
	  for( int i = 0; i < 5; ++i)
	  {
		  while(1)
		  {
			  if( index > 4)
			  {
				  break;
			  }

			  col = get_col();
			  row = get_row();

		  	  time[i] = key_pressed(row, col);
		  	  Lcd_cursor(&LCD, 1, index);

		  	  if( time[i] == 100 )
		  	  {
		  		  // Do nothing...
		  	  }else if( (index == 2) && (time[i] == -10))
		  	  {
				Lcd_string(&LCD, ":");
				++index;
				break;
		  	  }else
		  	  {
		  		Lcd_int(&LCD, time[i]);
		  		++index;
				break;
			  }
		  }
	  }


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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin PA6 PA7 */
  GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PC7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB4 PB5 PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
