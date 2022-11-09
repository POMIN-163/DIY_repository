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
#include <stdbool.h>
#include <stdint.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define RCLK_H GPIOA->ODR |= (uint32_t)GPIO_PIN_2  // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 1)
#define SCLK_H GPIOA->ODR |= (uint32_t)GPIO_PIN_7  // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1)
#define DIO_H  GPIOA->ODR |= (uint32_t)GPIO_PIN_6  // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1)

#define RCLK_L GPIOA->ODR &= ~(uint32_t)GPIO_PIN_2  // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0)
#define SCLK_L GPIOA->ODR &= ~(uint32_t)GPIO_PIN_7  // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0)
#define DIO_L  GPIOA->ODR &= ~(uint32_t)GPIO_PIN_6  // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0)

#define LED_ALL_HIGH 0x3ffff

#pragma pack(1)

union _led_disp {
    struct io {
        bool io1  : 1;
        bool io2  : 1;
        bool io3  : 1;
        bool io4  : 1;
        bool io5  : 1;
        bool io6  : 1;
        bool io7  : 1;
        bool io8  : 1;
        bool io9  : 1;
        bool io10 : 1;
        bool io11 : 1;
        bool io12 : 1;
        bool io13 : 1;
        bool io14 : 1;
        bool io15 : 1;
        bool io16 : 1;
        bool io17 : 1;
        bool io18 : 1;
    } io;
    int32_t value;
} my_disp = {.value = LED_ALL_HIGH};

typedef union _led_disp led_disp;

#pragma pack()

/* 刷屏序列 */
uint32_t led_seq[1024];
uint32_t led_seq_num;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * @brief 输出到屏
 *
 * @param value
 **/
void display_out(uint32_t value) {
    for (size_t i = 0; i < 18; i++) {
        if (value & 0x20000) {
            DIO_H;
        } else {
            DIO_L;
        }
        value <<= 1;
        SCLK_L;
        SCLK_H;
    }
    RCLK_L;
    RCLK_H;
}

/**
 * @brief
 *
 * @param self
 **/
inline void display_add_buf(uint32_t value) { led_seq[led_seq_num++] = value; }

inline void display_clear_buf() { led_seq_num = 0; }

void display_reflash() {
    static uint8_t oddeven = 1;
    // oddeven ^= 1;
    if (oddeven) {
        for (int i = 0; i < led_seq_num; i++) {
            display_out(led_seq[i]);
        }
    } else {
        for (int i = led_seq_num - 1; i > -1; i--) {
            display_out(led_seq[i]);
        }
    }
}

void display_clear() {
    led_seq_num = 0;
    display_reflash();
}

/**
 * @brief 设置八位阳极
 *
 * @param value
 **/
inline void display_set_anode(uint8_t value) {
    my_disp.value &= 0xfff00;
    my_disp.value |= value;
}

/**
 * @brief 清空阴极
 *
 * @param value
 **/
inline void display_clear_cathode() { my_disp.value |= 0x3ff00; }

/* 火焰图标 */
void display_fire(bool en) {
    if (!en) return;
    // [A1 A2 A3 A4] [B1 B2]
    display_clear_cathode();
    display_set_anode(0x0f);
    my_disp.io.io18 = 0;
    display_add_buf(my_disp.value);

    display_clear_cathode();
    display_set_anode(0x03);
    my_disp.io.io16 = 0;
    display_add_buf(my_disp.value);
}

/* 淋浴图标 */
void display_flujo(bool en) {
    if (!en) return;
    // [A5 A6 A7 A8] [B3 B4]
    display_clear_cathode();
    display_set_anode(0xf0);
    my_disp.io.io18 = 0;
    display_add_buf(my_disp.value);

    display_clear_cathode();
    display_set_anode(0x0c);
    my_disp.io.io16 = 0;
    display_add_buf(my_disp.value);
}

/* 时钟图标 */
void display_clock(bool en) {
    if (!en) return;
    // [A9 A10 A11 A12] [B5 B6]
    display_clear_cathode();
    display_set_anode(0x0f);
    my_disp.io.io17 = 0;
    display_add_buf(my_disp.value);

    display_clear_cathode();
    display_set_anode(0x30);
    my_disp.io.io16 = 0;
    display_add_buf(my_disp.value);
}

/* 风扇图标 */
void display_fan(bool en) {
    if (!en) return;
    // [A13 A14 A15 A16 A17] [B7 B8]
    display_clear_cathode();
    display_set_anode(0xf0);
    my_disp.io.io17 = 0;
    display_add_buf(my_disp.value);

    display_clear_cathode();
    display_set_anode(0x01);
    my_disp.io.io14 = 0;
    display_add_buf(my_disp.value);

    display_clear_cathode();
    display_set_anode(0xc0);
    my_disp.io.io16 = 0;
    display_add_buf(my_disp.value);
}

/* 优先图标 */
void display_first(bool en) {
    if (!en) return;
    // [A18 A19]
    display_clear_cathode();
    display_set_anode(0x06);
    my_disp.io.io14 = 0;
    display_add_buf(my_disp.value);
}

/* 容量显示 50 100 150 200 250 300 350 400 */
void display_capacity(uint16_t capacity) {
    if (capacity > 400) return;
    capacity /= 50;
    uint8_t bx[] = {
        0, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff,
    };
    uint8_t ax1[] = {
        0, 0x08, 0x18, 0x38, 0xf8,
    };
    uint8_t ax2[] = {
        0,
        0x03,
        0x1f,
        0xff,
    };
    // [A20 ~ A35]
    display_clear_cathode();
    if (capacity > 0) {
        display_clear_cathode();
        display_set_anode(ax1[capacity > 4 ? 4 : capacity]);
        my_disp.io.io14 = 0;
        display_add_buf(my_disp.value);
    }
    if (capacity > 4) {
        display_clear_cathode();
        display_set_anode(ax2[capacity == 8 ? 3 : capacity - 4]);
        my_disp.io.io13 = 0;
        display_add_buf(my_disp.value);
    }
    if (capacity == 8) {
        display_clear_cathode();
        display_set_anode(0x07);
        my_disp.io.io11 = 0;
        display_add_buf(my_disp.value);
    }

    // [B9 ~ B16]
    display_clear_cathode();
    display_set_anode(bx[capacity]);
    my_disp.io.io15 = 0;
    display_add_buf(my_disp.value);

    // [B17 ~ B18]
    display_clear_cathode();
    display_set_anode(0xc0);
    my_disp.io.io12 = 0;
    display_add_buf(my_disp.value);
}

/* 温度显示 */
void display_temperature(uint16_t temperature) {
    if (temperature > 88) return;
    uint8_t smgz[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};
    uint8_t arr[2];
    arr[0] = temperature / 10;
    arr[1] = temperature % 10;

    display_clear_cathode();
    display_set_anode(smgz[arr[0]]);
    my_disp.io.io9 = 0;
    display_add_buf(my_disp.value);

    display_clear_cathode();
    display_set_anode(smgz[arr[1]]);
    my_disp.io.io10 = 0;
    display_add_buf(my_disp.value);

    display_clear_cathode();
    display_set_anode(0x38);
    my_disp.io.io11 = 0;
    display_add_buf(my_disp.value);
}

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

    display_fire(1);
    display_flujo(1);
    display_clock(1);
    display_fan(1);
    display_first(1);
    display_capacity(400);
    display_temperature(12);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        display_reflash();
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = 0;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_10;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
    RCC_OscInitStruct.PLL.PLLM = 2;
    RCC_OscInitStruct.PLL.PLLN = 15;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
        Error_Handler();
    }
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

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
