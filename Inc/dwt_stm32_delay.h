#ifndef DELAY_HAL_STM32_MICRO
#define DELAY_HAL_STM32_MICRO

#ifdef __cplusplus
extern 'C' {
#endif

#include "stm32f3xx_hal.h"
#include "main.h"

uint32_t DWT_Delay_Init(void);

/**
 * @brief This function provides a delay (in microseconds)
 * @param microseconds: delay in microseconds
 */
__STATIC_INLINE void DWT_Delay_us(volatile uint32_t microseconds)
{
	uint32_t clk_cycle_start = DWT->CYCCNT;
	/* Go to number of cycles for system */
	microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000U);
	/* Delay till end */
	while ((DWT->CYCCNT - clk_cycle_start) < microseconds);

}


#ifdef __cplusplus
}
#endif
#endif
