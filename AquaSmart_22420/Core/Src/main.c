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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fsm.h"
#include "fsm_sensor.h"
#include "LoRa_comm.h"
#include "ring_buf.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define SENSOR1_TIME 	 400
#define SENSOR2_TIME 	 500
#define LEDS_TIME	 	 300
#define SEND_DATA_TIME 	 10000

/*ADC channels*/
#define ADC_Channel1 1
#define ADC_Channel2 2
#define ADC_Channel3 3
#define ADC_Channel4 4

/*PH sensor params*/

#define ID_Device 1

#define ID_ph_sensor 1

#define range_ph_acido 750 	 	// Only for test, real value is 2.
#define range_ph_basico 800		// Only for test, real value is 5.
#define range_ph_max 900		// Only for test, real value is 14.

#define ph_measure_period 600	// Test time to measure.
#define ph_sleep_period 5000	// Test time to sleep
#define ph_setup_period 1000		// Test time to setting up
#define ph_average 4			// number of measurements to make for 1 measure.

/*Turbidity sensor params*/
#define ID_turbidity_sensor 2

#define range_turb_min 750  	 	// Only for test.
#define range_turb_basico 800		// Only for test.
#define range_turb_max 900		// Only for test.

#define turb_measure_period 700	// Test time to measure.
#define turb_sleep_period 5500	// Test time to sleep
#define turb_setup_period 1000	// Test time to setting up
#define turb_average 4			// number of measurements to make for 1 measure.


#define NUMBER_OF_SENSORS 2

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

SPI_HandleTypeDef hspi1;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 512 * 4
};
/* Definitions for myTaskSensor1 */
osThreadId_t myTaskSensor1Handle;
const osThreadAttr_t myTaskSensor1_attributes = {
  .name = "myTaskSensor1",
  .priority = (osPriority_t) osPriorityLow4,
  .stack_size = 512 * 4
};
/* Definitions for myTaskLoRa */
osThreadId_t myTaskLoRaHandle;
const osThreadAttr_t myTaskLoRa_attributes = {
  .name = "myTaskLoRa",
  .priority = (osPriority_t) osPriorityLow2,
  .stack_size = 512 * 4
};
/* Definitions for myTaskSensor2 */
osThreadId_t myTaskSensor2Handle;
const osThreadAttr_t myTaskSensor2_attributes = {
  .name = "myTaskSensor2",
  .priority = (osPriority_t) osPriorityLow4,
  .stack_size = 512 * 4
};
/* Definitions for myTaskLEDs */
osThreadId_t myTaskLEDsHandle;
const osThreadAttr_t myTaskLEDs_attributes = {
  .name = "myTaskLEDs",
  .priority = (osPriority_t) osPriorityLow3,
  .stack_size = 512 * 4
};
/* Definitions for myQueueSensor1 */
osMessageQueueId_t myQueueSensor1Handle;
const osMessageQueueAttr_t myQueueSensor1_attributes = {
  .name = "myQueueSensor1"
};
/* Definitions for myQueueSensor2 */
osMessageQueueId_t myQueueSensor2Handle;
const osMessageQueueAttr_t myQueueSensor2_attributes = {
  .name = "myQueueSensor2"
};
/* Definitions for myQueueDataSaved */
osMessageQueueId_t myQueueDataSavedHandle;
const osMessageQueueAttr_t myQueueDataSaved_attributes = {
  .name = "myQueueDataSaved"
};
/* USER CODE BEGIN PV */

t_bool sensor1_ON, sensor2_ON;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_ADC1_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
void StartDefaultTask(void *argument);
void StartTaskSensor1(void *argument);
void StartTaskLoRa(void *argument);
void StartTaskSensor2(void *argument);
void StartTaskLEDs(void *argument);

/* USER CODE BEGIN PFP */
void Lora_inicio(int init);
void Lora_recibe(void);
void Lora_envia(sensor_buf_t mensaje);
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
  fsm_sensor_t fsm_s1;
  fsm_sensor_t fsm_s2;

  //fsm_t* fsm_LoRa;

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
  MX_ADC1_Init();
  MX_USB_OTG_FS_PCD_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of myQueueSensor1 */
  myQueueSensor1Handle = osMessageQueueNew (1, sizeof(sensor_t), &myQueueSensor1_attributes);

  /* creation of myQueueSensor2 */
  myQueueSensor2Handle = osMessageQueueNew (1, sizeof(sensor_t), &myQueueSensor2_attributes);

  /* creation of myQueueDataSaved */
  myQueueDataSavedHandle = osMessageQueueNew (2, sizeof(t_bool), &myQueueDataSaved_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of myTaskSensor1 */
  myTaskSensor1Handle = osThreadNew(StartTaskSensor1, &fsm_s1, &myTaskSensor1_attributes);

  /* creation of myTaskLoRa */
  myTaskLoRaHandle = osThreadNew(StartTaskLoRa, NULL, &myTaskLoRa_attributes);

  /* creation of myTaskSensor2 */
  myTaskSensor2Handle = osThreadNew(StartTaskSensor2, &fsm_s2, &myTaskSensor2_attributes);

  /* creation of myTaskLEDs */
  myTaskLEDsHandle = osThreadNew(StartTaskLEDs, NULL, &myTaskLEDs_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();
 
  /* We should never get here as control is now taken by the scheduler */
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

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_10B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin 
                          |Sensor1_Supply_Pin|Audio_RST_Pin|Sensor2_Supply_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : CS_I2C_SPI_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PDM_OUT_Pin */
  GPIO_InitStruct.Pin = PDM_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : NSS_Pin */
  GPIO_InitStruct.Pin = NSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(NSS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DIO0_Pin MODE_Pin */
  GPIO_InitStruct.Pin = DIO0_Pin|MODE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : RESET_Pin */
  GPIO_InitStruct.Pin = RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RESET_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CLK_IN_Pin */
  GPIO_InitStruct.Pin = CLK_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(CLK_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin 
                           Sensor1_Supply_Pin Audio_RST_Pin Sensor2_Supply_Pin */
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin 
                          |Sensor1_Supply_Pin|Audio_RST_Pin|Sensor2_Supply_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : I2S3_MCK_Pin I2S3_SCK_Pin I2S3_SD_Pin */
  GPIO_InitStruct.Pin = I2S3_MCK_Pin|I2S3_SCK_Pin|I2S3_SD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Audio_SCL_Pin Audio_SDA_Pin */
  GPIO_InitStruct.Pin = Audio_SCL_Pin|Audio_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */



/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */ 
}

/* USER CODE BEGIN Header_StartTaskSensor1 */
/**
* @brief Function implementing the myTaskSensor1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskSensor1 */
void StartTaskSensor1(void *argument)
{
  /* USER CODE BEGIN StartTaskSensor1 */
	uint32_t tDelay = 0;
	sensor_t sensor1;
	t_bool sensor1_measured = FALSE;
	ADC_ChannelConfTypeDef sConfig = {0};

    fsm_sensor_t* fsm_s1 = (fsm_sensor_t*)argument;

    sensor_initialization(&sensor1, ID_Device, ID_ph_sensor, Sensor1_Supply_Pin, ADC_Channel1, range_ph_acido, range_ph_basico, range_ph_max, ph_setup_period, ph_sleep_period, ph_measure_period, ph_average);
    fsm_sensor_init(fsm_s1, &sensor1);

    tDelay = osKernelGetTickCount();
/* Infinite loop */
  /* Infinite loop */
  for(;;)
  {

	/*Select ADC Channel 1*/

	if(fsm_s1->fsm.current_state <= 4 && sensor2_ON == FALSE)
	{
		sensor1_ON = TRUE;
		sConfig.Channel = ADC_CHANNEL_1;
		sConfig.Rank = 1;
		sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

		if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
		fsm_fire(&(fsm_s1->fsm));
		osMessageQueuePut (myQueueSensor1Handle, fsm_s1->param, 0, 0);
		osMessageQueuePut(myQueueDataSavedHandle, (t_bool*) &sensor1_measured, 0, 0);
	}
	else sensor1_ON = FALSE;

	if(fsm_s1->fsm.current_state > 4)
	{
		sensor1_measured = TRUE;
		fsm_fire(&(fsm_s1->fsm));
		osMessageQueuePut (myQueueSensor1Handle, fsm_s1->param, 0, 0);
		osMessageQueuePut(myQueueDataSavedHandle, (t_bool*) &sensor1_measured, 0, 0);
	}
	//	HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
    tDelay += pdMS_TO_TICKS(SENSOR1_TIME);
    osDelayUntil(tDelay);
  }
  /* USER CODE END StartTaskSensor1 */
}

/* USER CODE BEGIN Header_StartTaskLoRa */
/**
* @brief Function implementing the myTaskLoRa thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskLoRa */
void StartTaskLoRa(void *argument)
{
  /* USER CODE BEGIN StartTaskLoRa */
  uint32_t tDelay = 0;
  tDelay = osKernelGetTickCount();
  uint8_t master;
  t_bool sensor1_state;
  t_bool sensor2_state;

  /* master = 0 for slave.
   * master = 1 for master.
   * master 1 for all devices, 0 for GW*/
  master = 1;

  LoRa_initialization(master);

  /* Infinite loop */
  for(;;)
  {

	osMessageQueueGet(myQueueDataSavedHandle, &sensor1_state, 0, 0);
	osMessageQueueGet(myQueueDataSavedHandle, &sensor2_state, 0, 0);

	if (master == 1)
	{
		if(sensor1_state && sensor2_state)
		{
			for(uint8_t i = 0; i<NUMBER_OF_SENSORS; i++)
			{
				send_data();
			}
		}
	}
	else
	{
		receive_data();
	}
	tDelay += pdMS_TO_TICKS(SEND_DATA_TIME);
    osDelayUntil(tDelay);
  }
  /* USER CODE END StartTaskLoRa */
}

/* USER CODE BEGIN Header_StartTaskSensor2 */
/**
* @brief Function implementing the myTaskSensor2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskSensor2 */
void StartTaskSensor2(void *argument)
{
  /* USER CODE BEGIN StartTaskSensor2 */
	uint32_t tDelay = 0;
	sensor_t sensor2;
	t_bool sensor2_measured = FALSE;
	ADC_ChannelConfTypeDef sConfig = {0};

	fsm_sensor_t* fsm_s2 = (fsm_sensor_t*)argument;

	sensor_initialization(&sensor2, ID_Device, ID_turbidity_sensor, Sensor2_Supply_Pin, ADC_Channel2, range_turb_min, range_turb_basico, range_turb_max, turb_setup_period, turb_sleep_period, turb_measure_period, turb_average);
	fsm_sensor_init(fsm_s2, &sensor2);

	tDelay = osKernelGetTickCount();
	/* Infinite loop */
	/* Infinite loop */
	for(;;)
	{
		/*Select ADC Channel 2*/
		if(fsm_s2->fsm.current_state <= 4 && sensor1_ON == FALSE)
		{
			sensor2_ON = TRUE;
			sConfig.Channel = ADC_CHANNEL_2;
			sConfig.Rank = 1;
			sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

			if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
			{
				Error_Handler();
			}
			fsm_fire(&(fsm_s2->fsm));
			osMessageQueuePut (myQueueSensor2Handle, fsm_s2->param, 0, 0);
			osMessageQueuePut(myQueueDataSavedHandle, (t_bool*) &sensor2_measured, 0, 0);
		}
		else sensor2_ON = FALSE;

		if(fsm_s2->fsm.current_state > 4)
		{
			sensor2_measured = TRUE;
			fsm_fire(&(fsm_s2->fsm));

			osMessageQueuePut (myQueueSensor2Handle, fsm_s2->param, 0, 0);
			osMessageQueuePut(myQueueDataSavedHandle, (t_bool*) &sensor2_measured, 0, 0);
		}

		tDelay += pdMS_TO_TICKS(SENSOR2_TIME);
		osDelayUntil(tDelay);
	}
  /* USER CODE END StartTaskSensor2 */
}

/* USER CODE BEGIN Header_StartTaskLEDs */
/**
* @brief Function implementing the myTaskLEDs thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskLEDs */
void StartTaskLEDs(void *argument)
{
  /* USER CODE BEGIN StartTaskLEDs */
	uint32_t tDelay = 0;
	sensor_t sensor1;
	sensor_t sensor2;

	tDelay = osKernelGetTickCount();

  /* Infinite loop */
  for(;;)
  {
		osMessageQueueGet (myQueueSensor1Handle, &sensor1, NULL, 0);
		osMessageQueueGet (myQueueSensor2Handle, &sensor2, NULL, 0);


		if(sensor1.measuring == TRUE || sensor2.measuring == TRUE) /*LED BLUE => MEASURING*/
		{
			HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, SET);
			HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, RESET);
		}
		else
		{
			if(sensor1.sleeping == TRUE && sensor2.sleeping == TRUE)
			{
				HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, RESET);
				if(sensor1.error == TRUE || sensor2.error == TRUE) /*RED LED => SLEEPING SOMETHING WRONG*/
				{
					HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, SET);
					HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, RESET);
				}
				else	/*GREEN LED => SLEEPING ALL OK*/
				{
					HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, RESET);
					HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, SET);
				}

			}
			else /*STATE UNREACHABLE*/
			{
				HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, RESET);
				HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, RESET);
			}
		}

		if(sensor1.alarm == TRUE || sensor2.alarm == TRUE) HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, SET);
		else HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, RESET);

		tDelay += pdMS_TO_TICKS(SENSOR2_TIME);
	    osDelayUntil(tDelay);
  }
  /* USER CODE END StartTaskLEDs */
}

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
