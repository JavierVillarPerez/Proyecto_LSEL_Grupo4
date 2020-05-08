/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "fsm.h"
#include "fsm_sensor.h"

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
/* USER CODE BEGIN Variables */
t_bool sensor1_ON, sensor2_ON;

/* USER CODE END Variables */
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
  .priority = (osPriority_t) osPriorityLow2,
  .stack_size = 512 * 4
};
/* Definitions for myTaskLoRa */
osThreadId_t myTaskLoRaHandle;
const osThreadAttr_t myTaskLoRa_attributes = {
  .name = "myTaskLoRa",
  .priority = (osPriority_t) osPriorityBelowNormal,
  .stack_size = 512 * 4
};
/* Definitions for myTaskSensor2 */
osThreadId_t myTaskSensor2Handle;
const osThreadAttr_t myTaskSensor2_attributes = {
  .name = "myTaskSensor2",
  .priority = (osPriority_t) osPriorityLow2,
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

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTaskSensor1(void *argument);
void StartTaskLoRa(void *argument);
void StartTaskSensor2(void *argument);
void StartTaskLEDs(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Pre/Post sleep processing prototypes */
void PreSleepProcessing(uint32_t *ulExpectedIdleTime);
void PostSleepProcessing(uint32_t *ulExpectedIdleTime);

/* USER CODE BEGIN PREPOSTSLEEP */
__weak void PreSleepProcessing(uint32_t *ulExpectedIdleTime)
{
/* place for user code */ 
}

__weak void PostSleepProcessing(uint32_t *ulExpectedIdleTime)
{
/* place for user code */
}
/* USER CODE END PREPOSTSLEEP */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	  fsm_sensor_t fsm_s1;
	  fsm_sensor_t fsm_s2;
  /* USER CODE END Init */

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
  myQueueSensor2Handle = osMessageQueueNew (2, sizeof(sensor_t), &myQueueSensor2_attributes);

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
//  myTaskSensor2Handle = osThreadNew(StartTaskSensor2, &fsm_s2, &myTaskSensor2_attributes);

  /* creation of myTaskLEDs */
  myTaskLEDsHandle = osThreadNew(StartTaskLEDs, NULL, &myTaskLEDs_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
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

	    fsm_sensor_t* fsm_s1 = (fsm_sensor_t*)argument;

	    sensor2_ON = FALSE;

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
			adc_config_ch1();
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
	  sensor_buf_t data2send;
	  tDelay = osKernelGetTickCount();
	  uint8_t master;
	  t_bool sensor1_state = FALSE;
	  t_bool sensor2_state = FALSE;

	  /*master 1 for all devices, 0 for GW*/
	  master = 1;

	  Lora_inicio(master);  //0 es esclavo, 1 es maestro

	  /* Infinite loop */
	  for(;;)
	  {
		  osMessageQueueGet(myQueueDataSavedHandle, &sensor1_state, 0, 0);
	//	  osMessageQueueGet(myQueueDataSavedHandle, &sensor2_state, 0, 0);

		if (master == 1)
		{
//			if(sensor1_state && sensor2_state)
			if(sensor1_state)
			{
				for(uint8_t i = 0; i<NUMBER_OF_SENSORS; i++)
				{
					Lora_envia();
				}
			}
		}
		else
		{
			Lora_recibe();
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
//void StartTaskSensor2(void *argument)
//{
//	uint32_t tDelay = 0;
//	sensor_t sensor2;
//	t_bool sensor2_measured = FALSE;
//
//    fsm_sensor_t* fsm_s2 = (fsm_sensor_t*)argument;
//
//	sensor_initialization(&sensor2, ID_Device, ID_turbidity_sensor, Sensor2_Supply_Pin, ADC_Channel2, range_turb_min, range_turb_basico, range_turb_max, turb_setup_period, turb_sleep_period, turb_measure_period, turb_average);
//    fsm_sensor_init(fsm_s2, &sensor2);
//
//    tDelay = osKernelGetTickCount();
///* Infinite loop */
//  /* Infinite loop */
//  for(;;)
//  {
//
//	/*Select ADC Channel 1*/
//
//	if(fsm_s2->fsm.current_state <= 4 && sensor1_ON == FALSE)
//	{
//		sensor2_ON = TRUE;
//		adc_config_ch2();
//		fsm_fire(&(fsm_s2->fsm));
//		osMessageQueuePut (myQueueSensor1Handle, fsm_s2->param, 0, 0);
//		osMessageQueuePut(myQueueDataSavedHandle, (t_bool*) &sensor2_measured, 0, 0);
//	}
//	else sensor2_ON = FALSE;
//
//	if(fsm_s2->fsm.current_state > 4)
//	{
//		sensor2_measured = TRUE;
//		fsm_fire(&(fsm_s2->fsm));
//		osMessageQueuePut (myQueueSensor1Handle, fsm_s2->param, 0, 0);
//		osMessageQueuePut(myQueueDataSavedHandle, (t_bool*) &sensor2_measured, 0, 0);
//	}
//	//	HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
//    tDelay += pdMS_TO_TICKS(SENSOR2_TIME);
//    osDelayUntil(tDelay);
//  }
//}

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


//			if(sensor1.measuring == TRUE || sensor2.measuring == TRUE) /*LED BLUE => MEASURING*/
			if(sensor1.measuring == TRUE) /*LED BLUE => MEASURING*/
			{
				HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, SET);
				HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, RESET);
			}
			else
			{
				if(sensor1.sleeping == TRUE)
//				if(sensor1.sleeping == TRUE && sensor2.sleeping == TRUE)
				{
					HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, RESET);
//					if(sensor1.error == TRUE || sensor2.error == TRUE) /*RED LED => SLEEPING SOMETHING WRONG*/
					if(sensor1.error == TRUE) /*RED LED => SLEEPING SOMETHING WRONG*/
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

//			if(sensor1.alarm == TRUE || sensor2.alarm == TRUE) HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, SET);
			if(sensor1.alarm == TRUE) HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, SET);
			else HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, RESET);

			tDelay += pdMS_TO_TICKS(SENSOR2_TIME);
		    osDelayUntil(tDelay);
	  }
	  /* USER CODE END StartTaskLEDs */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
