#include "main.h"

void delay(volatile uint32_t time) {
    while(time--);
}

int main()
{
	// GPIO u ac
	RCC->APB2ENR |= (1 << 2); // Port A aktif

	GPIOA->CRL &= ~(0xF << 28); //
	GPIOA->CRL |= (0b0010 << 28); // Pin 7 = Output pull-up/pull-down, 2MHz

	while(1){
		GPIOA->BSRR |= (1<<7); // Led on
		delay(100000);
		GPIOA->BSRR |= (1 << 7+16); // Led off
		// GPIOA->BRR = (1<<7); bu da olur. Ayni sekilde led off yapar
		delay(100000);
	}

}
