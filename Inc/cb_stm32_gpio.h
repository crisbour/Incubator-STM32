#ifndef CB_GPIO_H
#define CB_GPIO_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f3xx_hal.h"

/**
 * @defgroup GPIO_Typedefs
 * @brief    GPIO Typedefs used for GPIO library for initialisation purposes
 * @{
 */

/**
 * @brief GPIO Mode enumeration
 */
typedef enum {
	GPIO_Mode_IN = 0x00,  /*!< GPIO Pin as General Purpose Input */
	GPIO_Mode_OUT = 0x01, /*!< GPIO Pin as General Purpose Output */
	GPIO_Mode_AF = 0x02,  /*!< GPIO Pin as Alternate Function */
	GPIO_Mode_AN = 0x03,  /*!< GPIO Pin as Analog input/output */
} GPIO_Mode_t;

/**
 * @brief GPIO Output type enumeration
 */
typedef enum {
	GPIO_OType_PP = 0x00, /*!< GPIO Output Type Push-Pull */
	GPIO_OType_OD = 0x01  /*!< GPIO Output Type Open-Drain */
} GPIO_OType_t;

/**
 * @brief  GPIO Speed enumeration
 */
typedef enum {
	GPIO_Speed_Low = 0x00,    /*!< GPIO Speed Low */
	GPIO_Speed_Medium = 0x01, /*!< GPIO Speed Medium */
	GPIO_Speed_Fast = 0x02,   /*!< GPIO Speed Fast, not available on STM32F0xx devices or STM32F303 */
	GPIO_Speed_High = 0x03    /*!< GPIO Speed High */
} GPIO_Speed_t;

/**
 * @brief GPIO pull resistors enumeration
 */
typedef enum {
	GPIO_PuPd_NOPULL = 0x00, /*!< No pull resistor */
	GPIO_PuPd_UP = 0x01,     /*!< Pull up resistor enabled */
	GPIO_PuPd_DOWN = 0x02    /*!< Pull down resistor enabled */
} GPIO_PuPd_t;

/**
 * @} GPIO_Typedefs
 */

/**
 * @brief  Initializes GPIO pins(s)
 * @note   This function also enables clock for GPIO port
 * @param  GPIOx: Pointer to GPIOx port you will use for initialization
 * @param  GPIO_Pin: GPIO pin(s) you will use for initialization
 * @param  GPIO_Mode: Select GPIO mode. This parameter can be a value of @ref GPIO_Mode_t enumeration
 * @param  GPIO_OType: Select GPIO Output type. This parameter can be a value of @ref GPIO_OType_t enumeration
 * @param  GPIO_PuPd: Select GPIO pull resistor. This parameter can be a value of @ref GPIO_PuPd_t enumeration
 * @param  GPIO_Speed: Select GPIO speed. This parameter can be a value of @ref GPIO_Speed_t enumeration
 * @retval None
 */
void GPIO_Init(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin, GPIO_Mode_t GPIO_Mode, GPIO_OType_t GPIO_OType, GPIO_PuPd_t GPIO_PuPd, GPIO_Speed_t GPIO_Speed);

/**
 * @brief  Sets pin as input
 * @note   Pins HAVE to be initialized first using @ref GPIO_Init() or @ref GPIO_InitAlternate() function
 * @note   This is just an option for fast input mode
 * @param  GPIOx: GPIOx PORT where you want to set pin as input
 * @param  GPIO_Pin: Select GPIO pin(s). You can select more pins with | (OR) operator to set them as input
 * @retval None
 */
#define GPIO_SetPinAsInput(GPIOx, GPIO_Pin)		( (GPIOx)->MODER = ((GPIOx)->MODER & ~(0x03 << (2 * GPIO_Pin))) | (GPIO_Mode_IN) << (2 * GPIO_Pin) )

/**
 * @brief  Sets pin as output
 * @note   Pins HAVE to be initialized first using @ref GPIO_Init() or @ref GPIO_InitAlternate() function
 * @note   This is just an option for fast output mode
 * @param  GPIOx: GPIOx PORT where you want to set pin as output
 * @param  GPIO_Pin: Select GPIO pin(s). You can select more pins with | (OR) operator to set them as output
 * @retval None
 */
#define GPIO_SetPinAsOutput(GPIOx, GPIO_Pin)	( (GPIOx)->MODER = ((GPIOx)->MODER & ~(0x03 << (2 * GPIO_Pin))) | (GPIO_Mode_OUT) << (2 * GPIO_Pin) )

/**
 * @brief  Sets pin low
 * @note   Defined as macro to get maximum speed using register access
 * @param  GPIOx: GPIOx PORT where you want to set pin low
 * @param  GPIO_Pin: Select GPIO pin(s). You can select more pins with | (OR) operator to set them low
 * @retval None
 */
#define GPIO_SetPinLow(GPIOx, GPIO_Pin)			((GPIOx)->BSRR = (uint32_t)(((uint16_t)(0x1<<GPIO_Pin)) << 16))

/**
 * @brief  Sets pin high
 * @note   Defined as macro to get maximum speed using register access
 * @param  GPIOx: GPIOx PORT where you want to set pin high
 * @param  GPIO_Pin: Select GPIO pin(s). You can select more pins with | (OR) operator to set them high
 * @retval None
 */
#define GPIO_SetPinHigh(GPIOx, GPIO_Pin)			( (GPIOx)->BSRR = ((uint16_t)(1<<GPIO_Pin)))

/**
 * @brief  Gets input data bit
 * @note   Defined as macro to get maximum speed using register access
 * @param  GPIOx: GPIOx PORT where you want to read input bit value
 * @param  GPIO_Pin: GPIO pin where you want to read value
 * @retval 1 in case pin is high, or 0 if low
 */
#define GPIO_GetInputPinValue(GPIOx, GPIO_Pin)	(((GPIOx)->IDR & (1<<GPIO_Pin)) == 0 ? 0 : 1)

#ifdef __cplusplus
}
#endif
#endif
