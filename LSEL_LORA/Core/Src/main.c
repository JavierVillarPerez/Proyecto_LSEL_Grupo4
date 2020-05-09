/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "spi.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include <string.h>
#include "stdio.h"
#include "SX1278.h"
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
SX1278_hw_t SX1278_hw;
SX1278_t SX1278;

int master=0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Lora_inicio(int init);
void Lora_recibe(void);
void Lora_envia(sensor_buf_t prueba);
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
	uint8_t buffer[] = "Hello world!\r\n";
	uint8_t len = 0;
	sensor_buf_t prueba;

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
  MX_SPI1_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
  len = strlen((char*) buffer);

  Lora_inicio(master);  //0 es esclavo, 1 es maestro

  prueba.ID = 1;
  prueba.measure = 100;
  prueba.alarm = 0;
  prueba.error = 0;
  prueba.threshold_L = 80;
  prueba.threshold_H = 120;

  HAL_Delay(1000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	if (master == 1) {
		Lora_envia(prueba);
		HAL_Delay(1000);
	} else {
		Lora_recibe();
	}
	  CDC_Transmit_FS(buffer, len);
	  HAL_Delay(1000);
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
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
int _write(int file, char *ptr, int len) {
	int i;
	for (i = 0; i < len; i++) {
		ITM_SendChar(*ptr++);
	}
	return len;
}

void Lora_inicio(int init){
	int ret;
	//initialize LoRa module
	SX1278_hw.dio0.port = DIO0_GPIO_Port;
	SX1278_hw.dio0.pin = DIO0_Pin;
	SX1278_hw.nss.port = NSS_GPIO_Port;
	SX1278_hw.nss.pin = NSS_Pin;
	SX1278_hw.reset.port = RESET_GPIO_Port;
	SX1278_hw.reset.pin = RESET_Pin;
	SX1278_hw.spi = &hspi1;
	SX1278.readBytes=0;
	SX1278.rxBuffer[0]=0;
	SX1278.hw = &SX1278_hw;

	printf("Configuring LoRa module\r\n");
	SX1278_begin(&SX1278, SX1278_433MHZ, SX1278_POWER_17DBM, SX1278_LORA_SF_8,
			SX1278_LORA_BW_20_8KHZ, 10);
	printf("Done configuring LoRaModule\r\n");
	while (ret!=1){
		if (init == 1) {
			ret = SX1278_LoRaEntryTx(&SX1278, 16, 2000);
		} else {
			ret = SX1278_LoRaEntryRx(&SX1278, 16, 2000); //tiene que valer 1
		}
		printf("ret: %d\n", ret);
	}
}
void Lora_recibe(void){
	int ret;
	char buffer[64];
	ret = SX1278_LoRaRxPacket(&SX1278);
	if (ret > 0) {
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		SX1278_read(&SX1278, (uint8_t *) buffer, ret);
		printf("Content (%d): %s\r\n", ret, buffer);
	}
}
void Lora_envia(sensor_buf_t prueba){
	int ret;
	char buffer[64];
	int message_length;
	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	message_length = sprintf(buffer, "AquaSmart %d %d %d %d %d %d", prueba.ID, prueba.measure, prueba.alarm, prueba.error, prueba.threshold_L, prueba.threshold_H);
	ret = SX1278_LoRaEntryTx(&SX1278, message_length, 2000);
	printf("Sending %s\r\n", buffer);
	ret = SX1278_LoRaTxPacket(&SX1278, (uint8_t *) buffer, message_length, 2000);
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
