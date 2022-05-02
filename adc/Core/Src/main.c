#include <stdint.h>
#include <stdio.h>
#include "sineWave.h"
#include "Audio_Drivers.h"
#include "main.h"
#include <string.h>
#include "stm32f407xx.h"
#include "stm32f4xx_hal.h"


ADC_HandleTypeDef hadc1;

UART_HandleTypeDef huart2;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART2_UART_Init(void);

// Initial variables.
float frequency = 880;


int main(void)
{
	// Initial components setup.
	setup();
	//flashGreen(); // Optional for testing
	uint16_t raw;
	char msg[10];
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_ADC1_Init();
	MX_USART2_UART_Init();

	// Main while loop for playing audio.
	while(1) {


		// ADC - Output comes out as "raw" variable which is a 16 bit variable

	    // Test: Set GPIO pin high
	    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
	    // Get ADC value
	    HAL_ADC_Start(&hadc1);
	    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	    raw = HAL_ADC_GetValue(&hadc1); // Serial port display
	    // Test: Set GPIO pin low
	    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
	    // Convert to string and print
	    sprintf(msg, "%hu\r\n", raw);
	    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);


	    // JAMES CODE - quantise, still needs work
/*

	    // Convert the Range of inputs from an input range to a set of frequencies
	    int RangeOne = (4096 - 0);
	    int RangeTwo = (1760 - 110);   //frequency range from A4 to A6
	    frequency = (((raw - 0) * RangeTwo) / RangeOne) + 0;

//	    // Quantise the Frequency
//	    frequency = quantise(frequency);
*/
	    // send frequency to the sineWave

		flashGreen();
		loopAudio(frequency);


	}
}



// Setup for reading inputs
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig = {0};

  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
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
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
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

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
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

static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

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
  printf("Wrong Parameter Values") */
}
#endif /* USE_FULL_ASSERT */


////Quantise the Frequency between 2 octaves
//float quantise(float frequency){
//	switch(frequency){
//	case 1760 ... 1661.23:
//			frequency = 1760;
//			break;
//	case 1661.22 ... 1567.99:
//			frequency = 1661.22;
//			break;
//	case 1567.98 ... 1479.99:
//			frequency = 1567.98;
//			break;
//	case 1479.98 ... 1396.92:
//			frequency = 1479.98;
//			break;
//	case 1396.91 ... 1318.52:
//			frequency = 1396.91;
//			break;
//	case 1318.51 ... 1244.52:
//			frequency = 1318.51;
//			break;
//	case 1244.51 ... 1174.67:
//			frequency = 1244.51;
//			break;
//	case 1174.66 ... 1108.74:
//			frequency = 1174.66;
//			break;
//	case 1108.73 ... 1046.6:
//			frequency = 1108.73;
//			break;
//	case 1046.5 ... 987.78:
//			frequency = 1046.5;
//			break;
//	case 987.77 ... 932.34:
//			frequency = 987.77;
//			break;
//	case 932.33 ... 880.01:
//			frequency = 932.33;
//			break;
//	case 880 ... 830.62:
//			frequency = 880;
//			break;
//	case 830.61 ... 784:
//			frequency = 830.61;
//			break;
//	case 783.99 ... 740:
//			frequency = 783.99;
//			break;
//	case 739.99 ... 698.47:
//			frequency = 739.99;
//			break;
//	case 698.46 ... 659.26:
//			frequency = 698.46;
//			break;
//	case 659.25 ... 622.26:
//			frequency = 659.25;
//			break;
//	case 622.25 ... 587.34:
//			frequency = 622.25;
//			break;
//	case 587.33 ... 554.38:
//			frequency = 587.33;
//			break;
//	case 554.37 ... 523.26:
//			frequency = 554.37;
//			break;
//	case 523.25 ... 493.89:
//			frequency = 523.25;
//			break;
//	case 493.88 ... 466.17:
//			frequency = 493.88;
//			break;
//	case 466.16 ... 440.01:
//			frequency = 466.16;
//			break;
//	case 440 ... 0:
//			frequency = 440;
//			break;
//	default:
//		frequency = 440;
//		break;
//	}
//	return frequency;
//}
