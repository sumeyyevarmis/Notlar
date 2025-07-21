# GPIO Functional Description

General-purpıse (genel amaçlı) I/O portlarının her birinde iki adet 32 bit configuration register (GPIOx_CRL, GPIOx_CRH), iki adet 32 bit data register (GPIOx_IDR, GPIOx_ODR), bir adet 32 bit set/reset register (GPIOx_BSRR), bir adet 16 bit reset register (GPIOx_BRR) ve 32 bit locking register (GPIOx_LCKR) bulunur.

Dataseheeet de listelenen her bir I/O portunun belirli donanım özelliklerine bağlı olarak General Purpose IO (GPIO) portlarıın her bir port biti, yazılım tarafından çeşitli modlarda ayrı ayrı konfigüre edilebilir;
- Input floating
- Input pull-up
- Input pull-down
- Analog
- Output open-drain
- Output push-pull
- Alternate fuction push-pull
- Alternate fuction open-drain

GPIOx_BRR ve GPIOx_BSRR register'larının amacı, GPIO register'ndan herhangi birine otomatik read/modify erişimlerine izin vermektir. Bu şekilde read ve modify erişimi arasında IRQ oluşması riski yoktur.

## 1. General-Purpose I/O (GPIO)
Resetlenme sırasında ve hemen sonrasında, alternatif fonksiyonlar etkin değildir ve I/o portları *Input Floating* modunda konfigüre edilir.

Output olarak konfigüre edildiğinde, Output Data Register (GPIOx_ODR)'a yazılan değer I/o pininden çıkar. Output driver'ı Push-pull modunda veya Open-Drain modunda kullanmak mümkündür.

Input Data Register (GPIOx_IDR) her APB2 clock döngüsünde I/O pininde bulunan verileri yakalar.

Tüm GPIO pinleri, Input olarak konfigüre edildiğinde etkinleştirilebilen veya etkinleştirilemeyen internal bir pull-up ve weak pull-sown özelliğine sahiptir.

## 2. Atomic Bir Set or Reset
GPIOx_ODR bit seviyesinde programlarken yazılımın interrupt'ları devre dışı bırakılmasına gerek yoktur; Tek bir atomik APB2 yazma erişiminde yalnızca bir veya birkaç biti değiştirmek mümkündür. Bu değiştirilecek bitleri seçmek için Bit Set/Reset register'nın (GPIOx_BSRR veya sadece reset GPIOx_BRR) '1' olarak programlanmasıyla elde edilir. Seçilmeyen bitler değiştirilmeyecektir.

## 3. GPIO Registers

Register    | Kullanımı                     |
------------|-------------------------------|
GPIOx_CRL   | Pin 0-7 için konfigürasyon    |
GPIOx_CRH   | Pin 8-15 için konfigürasyon   |
GPIOx_IDR   | Input veri okuma              |
GPIOx_ODR   | Output veri yazma             |
GPIOx_BSRR  | Bit Set/Reset                 |
GPIOx_BRR   | Bit Reset                     |
GPIOx_LCKR  | Pin kilitleme                 |                                

## Code

#include "main.h"
// #include "stm32f1xx_hal.h"

void delay(volatile uint32_t time) {
    while(time--);
}

int main(void) {
    // 1. GPIOC saatini aç
    RCC->APB2ENR |= (1 << 4); // IOPCEN → Bit 4

    // 2. PC13'ü Output Push-Pull yap (2 MHz)
    // PC13 = pin13 → CRH register'ında 4 bitlik alan: [23:20]
    GPIOC->CRH &= ~(0xF << 20);     // Önce temizle
    GPIOC->CRH |=  (0x2 << 20);     // MODE13=10, CNF13=00

    while (1) {
        // 3. PC13 LOW yap (LED ON)
        GPIOC->BRR = (1 << 13);     // LED aktif low

        delay(500000);

        // 4. PC13 HIGH yap (LED OFF)
        GPIOC->BSRR = (1 << 13);    // LED söndür

        delay(500000);
    }
}

# Detay

## 1. GPIOC için Saat(Clock) Açma

### 1. Hedef Register *RCC_APB2ENR*
Tam adı; APB2 Peripheral Clock Enable Register. Yani; APB2 bus'ndaki çevresel birimlerin saat sinyallerini açmak veya kapatmak için kullanılır.

### 2. Kaynağı Bul : Reference Manual
STM32F103 için RM0008'de bu register şu şekilde gösterilir.

*Bit 4 IOPCEN: IO port C clock enable*
*Set and cleared by software.*
*0: IO port C clock disabled*
*1: IO port C clock enabled*


IOPCEN, GPIOC için saat sinyalini aktif hale getirir.
---