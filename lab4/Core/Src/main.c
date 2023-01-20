/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "cmsis_os.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
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
char RX_BUFF[BUFF_SIZE];
char TX_BUFF[MSG_BUFF];
char *newline = "\n\n";

int rx_id;
int paused = 0;
int frequency = 440;

float amplitude = 0.5;

uint8_t rx_data;
uint8_t newline_detected = 0;
uint8_t buttonPressed;
uint8_t normal_op = 0;

xTaskHandle xTaskHandle1, xTaskHandle2, xTaskHandle3, xTaskHandle4;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
	rx_id = 0;
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
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);

  memset(TX_BUFF, '\0', MSG_BUFF);
  snprintf(TX_BUFF, MSG_BUFF, "%s\n", WELCOME_MSG);
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)TX_BUFF, strlen(TX_BUFF));
  HAL_Delay(200);

  xTaskCreate(vTask1, (const char *)"TASK_1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(vTask2, (const char *)"TASK_2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  vTaskStartScheduler();
  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */



  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
/*
	  if(buttonPressed) {
		  buttonPressed = 0;
		  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)TX_BUFF, strlen(TX_BUFF));
	  }

	  HAL_UART_Receive_DMA(&huart2, &rx_data, 1);*/


	  //while (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE) == RESET);

	  /*if(HAL_UART_Receive_DMA(&huart2, (uint8_t*)&RX_BUFF[rx_id], 1) != HAL_OK) {  }

	  if(RX_BUFF[rx_id-1] == '\r' || RX_BUFF[rx_id-1] == '\n') {
		  HAL_UART_Transmit_DMA(&huart2, RX_BUFF, rx_id);
		  rx_id = 0;
		  continue;
	  }

	  rx_id++;*/

	  //HAL_UART_Transmit_DMA(&huart2, (uint8_t *)msg, strlen(msg));
	  //timer2_wait_millisec(500);
	  //heartbeat_blink(LED4_GREEN_ID);
/*
	  HAL_UART_Receive_DMA(&huart2, &rx_data, 1);
	  while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY) { }
	  if (rx_data == '\n') {
	      newline_detected = 1;
	  } else {
		  RX_BUFF[RX_BUFF_TAIL] = rx_data;
		  RX_BUFF_TAIL++;
	  }

	  if(newline_detected) {

		  RX_BUFF[RX_BUFF_TAIL] = '\0';
		  HAL_UART_Transmit_DMA(&huart2, RX_BUFF, sizeof(RX_BUFF));

		  newline_detected = 0;
		  RX_BUFF_TAIL = 0;

	  }*/

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

/* USER CODE BEGIN 4 */
void vTask1(void *pvParameters) {
	while(1) {
		heartbeat_blink(LED4_GREEN_ID);
	}
}

void vTask2(void *pvParameters) {
	while(1) {
		HAL_UART_Receive_DMA(&huart2, &rx_data, 1);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if(GPIO_Pin == USER_BUTTON_PIN) {
		HAL_NVIC_DisableIRQ(EXTI0_IRQn);
		buttonPressed = 0;
		HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	}
	else {
		__NOP();
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    // If not '\n' or '\n' echo back the received character
	if(rx_data == '\r' || rx_data == '\n') {
		if(normal_op) {
			if(RX_BUFF[0] == 'f' || RX_BUFF[0] == 'a') {
				for(int i = 1; i < strlen(RX_BUFF); i++) {
		            if (RX_BUFF[i] <= 0 && RX_BUFF[i] >= 9) {
		            	HAL_UART_Transmit_DMA(&huart2, (uint8_t *)ERR_MSG, strlen(ERR_MSG));
		            }
				}

				if(RX_BUFF[0] == 'f') {
					sscanf(RX_BUFF, "f%d", &frequency);
					HAL_UART_Transmit_DMA(&huart2, (uint8_t *)OK_MSG, strlen(OK_MSG));
				} else if(RX_BUFF[0] == 'a') {
					sscanf(RX_BUFF, "a%f", &amplitude);
					HAL_UART_Transmit_DMA(&huart2, (uint8_t *)OK_MSG, strlen(OK_MSG));
				}
			} else {
				HAL_UART_Transmit_DMA(&huart2, (uint8_t *)ERR_MSG, strlen(ERR_MSG));
			}
		} else {
			if(strcmp(RX_BUFF, PASSWORD) == 0) {
				HAL_UART_Transmit_DMA(&huart2, (uint8_t *)OK_MSG, strlen(OK_MSG));
				normal_op = 1;
			} else {
				HAL_UART_Transmit_DMA(&huart2, (uint8_t *)WELCOME_MSG, strlen(WELCOME_MSG));
			}
		}

		memset(RX_BUFF, '\0', BUFF_SIZE);
		rx_id = 0;
	} else {
		HAL_UART_Transmit_DMA(&huart2, &rx_data, 1);
		RX_BUFF[rx_id] = rx_data;
		rx_id++;
	}

}
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
