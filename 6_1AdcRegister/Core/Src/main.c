#include "main.h"

void ADC1_Init(void)
{
    // 1. RCC Saatleri Aç
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // GPIOA clock enable
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;   // ADC1 clock enable

    // 2. GPIO PA0 -> Analog Mode
    GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0); // MODE0=00 (input), CNF0=00 (analog)

    // 3. ADC Clock Prescaler (PCLK2 / 2) — 8 MHz / 2 = 4 MHz
    RCC->CFGR &= ~RCC_CFGR_ADCPRE;       // Temizle
    RCC->CFGR |= RCC_CFGR_ADCPRE_DIV2;   // /2 seç

    // 4. Sampling Time ayarı (Channel 0 -> 55.5 cycles)
    ADC1->SMPR2 &= ~ADC_SMPR2_SMP0;
    ADC1->SMPR2 |= ADC_SMPR2_SMP0_1 | ADC_SMPR2_SMP0_0; // 55.5 cycles

    // 5. Sıralama Ayarı (SQR3: 1. dönüşüm kanalı = 0)
    ADC1->SQR3 = 0; // Channel 0

    // 6. ADC etkinleştir
    ADC1->CR2 |= ADC_CR2_ADON; // ADC'yi aç
    for (volatile int i = 0; i < 1000; i++); // kısa gecikme

    // 7. Kalibrasyon
    ADC1->CR2 |= ADC_CR2_CAL; // kalibrasyon başlat
    while (ADC1->CR2 & ADC_CR2_CAL); // bitene kadar bekle
}

uint16_t ADC1_Read(void)
{
    ADC1->CR2 |= ADC_CR2_ADON; // 1. yazış: ADC başlatma (wake up)
    ADC1->CR2 |= ADC_CR2_ADON; // 2. yazış: Conversion başlat

    while (!(ADC1->SR & ADC_SR_EOC)); // EOC (End of Conversion) bekle
    return ADC1->DR; // 12-bit sonuç (0-4095)
}
uint16_t value;
int main(void)
{

    ADC1_Init();

    while (1)
    {
        value = ADC1_Read();
        // float volt = value * 3.3f / 4095.0f; // Voltaj hesabı
        for (volatile int i = 0; i < 50000; i++); // basit delay
    }
}
