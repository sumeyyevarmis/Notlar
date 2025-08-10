#include "main.h"

void GPIOset(){
	RCC->APB2ENR |= (1 << 2); // GPIOA

	// GPIOA Output
	GPIOA->CRL &= ~(0xF << 28) ; // PA7
	GPIOA->CRL |= (0b0010 << 28);

	GPIOA->CRL &= ~(0xF << 24) ; // PA6
	GPIOA->CRL |= (0b0010 << 24);


}

void TIMset(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	TIM2->PSC = 8000 - 1;
	TIM2->ARR = 1000 - 1;
	TIM2->CNT = 0; // Counter = 0

	TIM2->CR1 &= ~TIM_CR1_CEN; // (1 << 0) => Timer reset

	TIM2->EGR = TIM_EGR_UG; // update event

	TIM2->SR = 0; // Clear flag

	TIM2->DIER |= TIM_DIER_UIE; // Enable update interrupt
}

void TIM3set(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	TIM3->PSC = 8000 - 1;
	TIM3->ARR = 1000 - 1;
	TIM3->CNT = 0; // Counter = 0

	TIM3->CR1 &= ~TIM_CR1_CEN; // (1 << 0) => Timer reset

	TIM3->EGR = TIM_EGR_UG; // update event

	TIM3->SR = 0; // Clear flag

	TIM3->DIER |= TIM_DIER_UIE; // Enable update interrupt
}


void TIM2_IRQHandler(void){
	// Zaman doldumu kontrol et
	if(TIM2->SR & TIM_SR_UIF){ //
		TIM2->SR = ~TIM_SR_UIF;//  Flag temizle
		// Led toggel
		GPIOA->ODR ^= (1 << 7);
	}
}

void TIM3_IRQHandler(void){
	// Zaman doldumu kontrol et
	if(TIM3->SR & TIM_SR_UIF){ //
		TIM3->SR = ~TIM_SR_UIF;//  Flag temizle
		// Led toggel
		GPIOA->ODR ^= (1 << 6);
	}
}

int main(){
	GPIOset();
	TIMset();
	TIM3set();

	NVIC_SetPriority(TIM2_IRQn, 1);
	NVIC_ClearPendingIRQ(TIM2_IRQn);
	NVIC_EnableIRQ(TIM2_IRQn);

	NVIC_SetPriority(TIM3_IRQn, 2);
	NVIC_ClearPendingIRQ(TIM3_IRQn);
	NVIC_EnableIRQ(TIM3_IRQn);

	TIM2->CR1 |= TIM_CR1_CEN; // Timer start
	TIM3->CR1 |= TIM_CR1_CEN; // Timer start

	while(1){

	}
}
