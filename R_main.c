/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
/* USER CODE END PD */
/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */
/* Private variables ---------------------------------------------------------*/
float temperatura_motor;
float energia;
CAN_HandleTypeDef hcan;
UART_HandleTypeDef huart2;
/* USER CODE BEGIN PV */
/* USER CODE END PV */
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_CAN_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
CAN_TxHeaderTypeDef TxHeader;

uint8_t dummydata[8] = {0};
uint32_t TxMailBox;

//getitng


int datacheck = 0;

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef RxHeader;
	    uint8_t RxData[8];

	    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);

	    if (RxHeader.StdId == 0x124)
	    {
	    	temperatura_motor = RxData[0];
	    	energia = RxData[1];
	        // Aqui estão os dados que a placa B mandou como resposta
	        // Ex: RxData[0] = temperatura, RxData[1] = umidade, etc.
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
 MX_USART2_UART_Init();
 MX_CAN_Init();
 /* USER CODE BEGIN 2 */
 HAL_CAN_Start(&hcan);
 // Activate the notification

 HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO1_MSG_PENDING);

 TxHeader.DLC = 0; //data length
 TxHeader.ExtId = 0x00;
 TxHeader.IDE = CAN_ID_STD;
 TxHeader.RTR = CAN_RTR_REMOTE;
 TxHeader.StdId = 0x102; //Id

 HAL_CAN_AddTxMessage(&hcan, &TxHeader, dummydata, &TxMailBox);

 datacheck = 1;

 /* USER CODE END 2 */
 /* Infinite loop */
 /* USER CODE BEGIN WHILE */
 while (1)
 {
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
 * @brief CAN Initialization Function
 * @param None
 * @retval None
 */
static void MX_CAN_Init(void)
{
 /* USER CODE BEGIN CAN_Init 0 */
 /* USER CODE END CAN_Init 0 */
 /* USER CODE BEGIN CAN_Init 1 */
 /* USER CODE END CAN_Init 1 */
 hcan.Instance = CAN1;
 hcan.Init.Prescaler = 18;
 hcan.Init.Mode = CAN_MODE_NORMAL;
 hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
 hcan.Init.TimeSeg1 = CAN_BS1_1TQ;
 hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
 hcan.Init.TimeTriggeredMode = DISABLE;
 hcan.Init.AutoBusOff = DISABLE;
 hcan.Init.AutoWakeUp = DISABLE;
 hcan.Init.AutoRetransmission = DISABLE;
 hcan.Init.ReceiveFifoLocked = DISABLE;
 hcan.Init.TransmitFifoPriority = DISABLE;
 if (HAL_CAN_Init(&hcan) != HAL_OK)
 {
   Error_Handler();
 }
 /* USER CODE BEGIN CAN_Init 2 */
 CAN_FilterTypeDef canfilterconfig;

 canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;
 canfilterconfig.FilterBank = 10; //which filter bank to use from the assigned ones
 canfilterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
 canfilterconfig.FilterIdHigh = 0;
 canfilterconfig.FilterIdLow = 0;
 canfilterconfig.FilterMaskIdHigh = 0;
 canfilterconfig.FilterMaskIdLow = 0x0000;
 canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
 canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
 canfilterconfig.SlaveStartFilterBank = 0; //doesnt matter in single can controllers

 HAL_CAN_ConfigFilter(&hcan, &canfilterconfig);
 /* USER CODE END CAN_Init 2 */
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
 /* USER CODE BEGIN MX_GPIO_Init_1 */
 /* USER CODE END MX_GPIO_Init_1 */
 /* GPIO Ports Clock Enable */
 __HAL_RCC_GPIOC_CLK_ENABLE();
 __HAL_RCC_GPIOD_CLK_ENABLE();
 __HAL_RCC_GPIOA_CLK_ENABLE();
 __HAL_RCC_GPIOB_CLK_ENABLE();
 /*Configure GPIO pin Output Level */
 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
 /*Configure GPIO pin Output Level */
 HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
 /*Configure GPIO pin : PC13 */
 GPIO_InitStruct.Pin = GPIO_PIN_13;
 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
 HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
 /*Configure GPIO pin : LD2_Pin */
 GPIO_InitStruct.Pin = LD2_Pin;
 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
 HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);
 /* USER CODE BEGIN MX_GPIO_Init_2 */
 /* USER CODE END MX_GPIO_Init_2 */
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
