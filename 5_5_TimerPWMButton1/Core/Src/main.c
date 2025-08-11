#include "main.h"

void GPIO_PWM_Init(void) {
    // GPIOA Clock enable
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // PA6 = TIM3_CH1 (Alternate Function Push-Pull, 50 MHz)
    GPIOA->CRL &= ~(0xF << (6 * 4));   // PA6 temizle
    GPIOA->CRL |=  (0xB << (6 * 4));   // 0xB = AF Push-Pull, 50 MHz
}


void TIM3_PWM_Init(void) {
    // TIM3 Clock enable
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // Timer frekans ayarı
    TIM3->PSC = 8 - 1;     // Timer clock = 8 MHz / 8 = 1 MHz
    TIM3->ARR = 1000 - 1;   // Periyot = 1 kHz PWM (1 MHz / 1000)

    // PWM1 modu (OC1M = 110)
    TIM3->CCMR1 &= ~TIM_CCMR1_OC1M;
    TIM3->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE; // Preload enable

    // Kanal 1 enable
    TIM3->CCER |= TIM_CCER_CC1E;

    // Başlangıç duty cycle (%50)
    TIM3->CCR1 = 500;

    // Timer enable
    TIM3->CR1 |= TIM_CR1_CEN;
}

void GPIO_Input_Init(void){
	RCC->APB2ENR |= (1 << 0); // AFIO

	// 2. PA5 -> input pull-up
	GPIOA->CRL &= ~(0xF << 20); // PA5 konfigürasyonları temizlenir
	GPIOA->CRL |= (0b1000 << 20); // PA5 input, pull-up olarak tanımlanır.
	GPIOA->ODR |= (1 << 5); // Pull-up, butona basıınca 0 olacak

	// 4. EXTI ayarları (EXTI5 için PA5)
	AFIO->EXTICR[1] &= ~(0xF << 4); // EXTI5 için PA5 seçilir

	EXTI->IMR |= (1 << 5); // Interrupt mask aktif
	EXTI->FTSR |= (1 << 5); // Falling edge trigger aktif
}

void EXTI9_5_IRQHandler(void){
	// Interrupt flag temizle
	 if (EXTI->PR & (1 << 5)) {
		EXTI->PR |= (1 << 5); // Interrupt flag temizle

		TIM3->CCR1 += 100;

		if(TIM3->CCR1 >= 1000){
			TIM3->CCR1 = 0;
		}
	 }


}


int main(void) {

	GPIO_PWM_Init();
	TIM3_PWM_Init();
	GPIO_Input_Init();
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	while(1){

	}
}
