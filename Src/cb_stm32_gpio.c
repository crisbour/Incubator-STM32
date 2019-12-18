/*
 * cb_stm32_gpio.c
 *
 *  Created on: 18 Dec 2019
 *      Author: bourc
 */
#include "cb_stm32_gpio.h"
#include "assert.h"

/*Private array*/
static uint16_t GPIO_UsedPins[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};

void GPIO_INT_EnableClock(GPIO_TypeDef* GPIOx);
void GPIO_INT_Init(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin, GPIO_Mode_t GPIO_Mode, GPIO_OType_t GPIO_OType, GPIO_PuPd_t GPIO_PuPd, GPIO_Speed_t GPIO_Speed);

void GPIO_Init(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin, GPIO_Mode_t GPIO_Mode, GPIO_OType_t GPIO_OType, GPIO_PuPd_t GPIO_PuPd, GPIO_Speed_t GPIO_Speed) {

	/* Enable clock for GPIO */
	GPIO_INT_EnableClock(GPIOx);

	/* Do initialization */
	GPIO_INT_Init(GPIOx, GPIO_Pin, GPIO_Mode, GPIO_OType, GPIO_PuPd, GPIO_Speed);
}

uint16_t GPIO_GetPortSource(GPIO_TypeDef* GPIOx) {
	/* Get port source number */
	/* Offset from GPIOA                       Difference between 2 GPIO addresses */
	return ((uint32_t)GPIOx - (GPIOA_BASE)) / ((GPIOB_BASE) - (GPIOA_BASE));
}
void GPIO_INT_EnableClock(GPIO_TypeDef* GPIOx) {
	/* Set bit according to the 1 << portsourcenumber */
#if defined(STM32F0xx) || defined(STM32F303x8)
	RCC->AHBENR |= (1 << (GPIO_GetPortSource(GPIOx) + 17));
#else
	RCC->AHB1ENR |= (1 << GPIO_GetPortSource(GPIOx));
#endif
}

void GPIO_INT_Init(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin, GPIO_Mode_t GPIO_Mode, GPIO_OType_t GPIO_OType, GPIO_PuPd_t GPIO_PuPd, GPIO_Speed_t GPIO_Speed) {
	uint8_t ptr = GPIO_GetPortSource(GPIOx);

	assert(GPIO_Pin<16);	//Check if the GPIO_Pin value is valid: 0-15

#if defined(STM32F0xx) || defined(STM32F303x8)
	/* STM32F0xx series does not have FAST speed mode available */
	if (GPIO_Speed == GPIO_Speed_Fast) {
		/* Set speed to high mode */
		GPIO_Speed = GPIO_Speed_High;
	}
#endif


	/* Pin is used */
	GPIO_UsedPins[ptr] |= 1 << GPIO_Pin;

	/* Set GPIO PUPD register */
	GPIOx->PUPDR = (GPIOx->PUPDR & ~(0x03 << (2 * GPIO_Pin))) | ((uint32_t)(GPIO_PuPd << (2 * GPIO_Pin)));

	/* Set GPIO MODE register */
	GPIOx->MODER = (GPIOx->MODER & ~((uint32_t)(0x03 << (2 * GPIO_Pin)))) | ((uint32_t)(GPIO_Mode << (2 * GPIO_Pin)));

	/* Set only if output or alternate functions */
	if (GPIO_Mode == GPIO_Mode_OUT || GPIO_Mode == GPIO_Mode_AF) {
		/* Set GPIO OTYPE register */
		GPIOx->OTYPER = (GPIOx->OTYPER & ~(uint16_t)(0x01 << GPIO_Pin)) | ((uint16_t)(GPIO_OType << GPIO_Pin));

		/* Set GPIO OSPEED register */
		GPIOx->OSPEEDR = (GPIOx->OSPEEDR & ~((uint32_t)(0x03 << (2 * GPIO_Pin)))) | ((uint32_t)(GPIO_Speed << (2 * GPIO_Pin)));
	}
}
