#include <stdint.h>
#define __IO volatile

typedef struct
{
	__IO uint32_t MODER; 
	__IO uint32_t OTYPER; 
	__IO uint32_t OSPEEDR; 
	__IO uint32_t PUPDR; 
	__IO uint32_t IDR; 
	__IO uint32_t ODR; 
	__IO uint32_t BSRR; 
	__IO uint32_t LCKR; 
	__IO uint32_t AFR[2]; 
}
GPIO_TypeDef;

#define RCC_BASE        0x40023800U
#define AHB1ENR_OFFSET  0x30U
#define GPIOA_BASE      0x40020000U

#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + AHB1ENR_OFFSET))
#define GPIOA           ((GPIO_TypeDef *)GPIOA_BASE)

#define PA5_PIN         5U
#define PA5_SHIFT       (PA5_PIN * 2U)

static void delay_cycles(volatile uint32_t cycles)
{
	while(cycles--)
	{
		__asm volatile("nop");
	}
}

int main(void)
{
	// Enable clock for GPIOA (bit 0)
    RCC_AHB1ENR |= (1U << 0U);

    // Configure PA5 as general-purpose output
    GPIOA->MODER &= ~(0x3U << PA5_SHIFT); 
    GPIOA->MODER |= (0x1U << PA5_SHIFT); 

    // Push-pull, low speed, no pull-up/pull-down
    GPIOA->OTYPER &= ~(1U << PA5_PIN);
    GPIOA->OSPEEDR &= ~(0x3U << PA5_SHIFT);
    GPIOA->PUPDR &= ~(0x3U << PA5_SHIFT);

    // Ensure LED is off initially
    GPIOA->BSRR = (1U << (PA5_PIN + 16U)); 

    while(1)
    {
    	GPIOA->BSRR = (1U << PA5_PIN); // Set PA5
    	delay_cycles(2000000U); // Delay in cycles (not fixed ms)

    	GPIOA->BSRR = (1U << (PA5_PIN + 16U)); // Reset PA5
    	delay_cycles(2000000U);
    }
}
