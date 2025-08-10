
Date: 09-08-2025
Day: 27

# Timers    
## 1. Timer Nedir?
Timer, mikrodenetleyicide belirli bir süreyi saymak veya ölçmek için kullanılan bir çevresel birimdir. Genelde şu amaçlarla kullanılırı:
- Belirli periyotlarla interrupt oluşturmak,
- PWM (Pulse Width Modulation) sinyali üretmek,
- Input capture/output compare uygulamaları,
- Olayları saymak (counter)

## 2. Temel Bileşenler
- **Prescaler:** Sistemin clock frekansını yavaşlatır. Timer'ın sayma hızını belirler.
- **ARR(Auto Reload Register):** Timer buraya kadar sayar, sonra sıfırlanır.
- **CNT(Counter):** Timer'ın sayacına giren clock'u böler.
- **Interrupt:** ARR'ye ulaştığında bir kesme üretebilir.

## 3.Temel Timer Çalışma Modları
| Mod            | Açıklama                                                     |
| -------------- | ------------------------------------------------------------ |
| Up Counting    | 0’dan ARR’ye kadar sayar, sonra sıfırlanır                   |
| Down Counting  | ARR’den 0’a kadar sayar                                      |
| PWM Mode       | PWM sinyali üretmek için kullanılır                          |
| One-Pulse Mode | Tek bir pulse üretir                                         |
| Encoder Mode   | Quadrature encoder’dan veri okur (motorlarda sık kullanılır) |

## 4. Timer Yapısı
| Register Adı                  | Açıklama                                          |
| ----------------------------- | ------------------------------------------------- |
| `PSC` (Prescaler)             | Timer clock’ı yavaşlatır.                         |
| `ARR` (Auto Reload Register)  | Sayaç bu değere ulaştığında taşar (overflow).     |
| `CNT` (Counter)               | Sayım yapılan register (okuma-yazma yapılabilir). |
| `CR1` (Control Register 1)    | Timer başlatma/durdurma, sayma yönü vb.           |
| `DIER` (DMA/Interrupt Enable) | Hangi olaylarda interrupt oluşturulacak?          |
| `SR` (Status Register)        | Hangi olaylar oluştu, flag’ler burada tutulur.    |
| `EGR` (Event Generation Reg)  | Yazılım yoluyla olay tetikleme.                   |

## 5. İlk Timer Kodu: Temel Bir Timer Delay Uygulaması
**Hedef:** TIM2'yi kullanarak belirli aralıklarla bir LED'i yakıp söndüreceğiz.

Kullanılacak registerlar:
| Register                   | Açıklama                               |
| -------------------------- | -------------------------------------- |
| `RCC_APB1ENR`              | TIM2 clock enable                      |
| `TIM2_PSC`                 | Prescaler değeri                       |
| `TIM2_ARR`                 | Auto-reload değeri (overflow süresi)   |
| `TIM2_CR1`                 | Timer kontrolü (enable, direction vs.) |
| `TIM2_SR`                  | Status register (update flag)          |
| `GPIOx_MODER`, `GPIOx_ODR` | GPIO kontrolü                          |


    #include "main.h"

    int main()
    {
        // Clock enable (Tim2 - GPIOA)
        RCC->APB1ENR |= (1 << 0); // TIM2
        RCC->APB2ENR |= (1 << 2); // GPIOA

        // GPIOA Output
        GPIOA->CRL &= ~(0xF << 28) ; // PA7
        GPIOA->CRL |= (0b0010 << 28);

        // Timer
        TIM2->PSC = 8000  - 1; // 8 MHz / 8000 = 1 kHz (1 ms)
        TIM2->ARR = 1000  - 1; // 1ms * 1000 = 1 saniye
        TIM2->CNT = 0; // Counter = 0
        TIM2->CR1 |= TIM_CR1_CEN; // (1 << 0) => Timer başlat

        while(1){
            // Zaman doldumu kontrol et
            if(TIM2->SR & (1 << 0)){ // TIM_SR_UIF
                TIM2->SR &= ~(0xF << 0);//TIM_SR_UIF; // Flag temizle

                // Led toggel
                GPIOA->ODR ^= (1 << 7);
            }
        }
    }


## 6. PSC(Prescaler) ve ARR(Auto Reload Register) Neye Göre Belirlenir?
**Genel Mantık:**
Timer şöyle çalışır:
1. Timer'a bir clock frekansı gelir (mesela 36 MHz).
2. Bu clock, PSC(Prescaler) ile yavaşlatılır.
3. PSC'den geçen clock sinyali, sayacı (CNT) birer birer arttırır.
4. Sayaç ARR değerine ulaştığında taşar (overflow olur) ve bir eevent meydana gelir (örneğin LED toggle).

**Nasıl Hesaplanır:**
Sen bir *T* süresi elde etmek istiyorsun. Timer clock frekansı *f_clk* olsun.

T = ((PSC + 1) * (ARR + 1)) / f_clk


## 7. Notlar

Polling ile led yakma işlemleri. Bizim kullandığımız sistemde, TIM2 8 MHz'dir. ioc. dosyasından kontrol edildiğinde bu şekilde APB'nin clock degeri 8 MHz olarak gözükmektedir.

## 8. İkinci Timer Kodu: 2 snde bir LED Toggle

    #include "main.h"

    int main()
    {
        // Clock enable (Tim2 - GPIOA)
        RCC->APB1ENR |= (1 << 0); // TIM2
        RCC->APB2ENR |= (1 << 2); // GPIOA

        // GPIOA Output
        GPIOA->CRL &= ~(0xF << 28) ; // PA7
        GPIOA->CRL |= (0b0010 << 28);

        // Timer
        TIM2->PSC = 8000  - 1; // 8 MHz / 8000 = 1 kHz (1 ms)
        TIM2->ARR = 2000  - 1; // 1ms * 1000 = 1 saniye
        TIM2->CNT = 0; // Counter = 0
        TIM2->CR1 |= TIM_CR1_CEN; // (1 << 0) => Timer başlat

        while(1){
            // Zaman doldumu kontrol et
            if(TIM2->SR & (1 << 0)){ // TIM_SR_UIF
                TIM2->SR &= ~(0xF << 0);//TIM_SR_UIF; // Flag temizle

                // Led toggel
                GPIOA->ODR ^= (1 << 7);
            }
        }
    }

# Update Event ile Timer

## 1. Temel Kavramlar

- **DIER (DMA/Interrupt Enable Register):** Timer'ın hangi interrupt/DMA kaynaklarının NVIC'e sinyal göndereceğini kontrol eder. Update interrupt için *UIE* (bit=0) kullanılır. *UIE=1* ise update(taşma/UEV) gerçekleştiğinde timer bir interrupt isteği oluşturur.

- **SR (Status Register):** Timer'ınevent flag'larini (UIF, CCxIF vb.) içerir. Bu bayraklar periferal tarafından set edilir; yazılım bunları temizleyebilir.

- **UIF (Update Interrupt Flag):** Update/Event (counter overflow veya yazılımsal update) gerçekleşince set olur. Eğer *DIER.UIE = 1* VE UIF = 1 ise timer NVIC'e bir interrupt isteği gönderiri. 

- **NVIC(Nested Vectored Interrupt Controller):** CPU tarafındaki interrupt kabul mekanizmasıdır. Periferalin DIER'inde UIE=1 yapmak tek başına yeterli değildir; NVIC'de de ilgili IRQ hattını enable etmelisin.

## 2. Update Event Nereden Gelir? (UIF hangi durumlarda set olur?)
- Sayac *CNT* değerinin *ARR* ile eşleşip overflow yapması -> UEV (update event) -> UIF set
- Yazılımsal olarak *EGR* register'daki *UG* bitini 1 yaparsan güncelleme olayı (update event) anında üretilir (PSC/ARR preload yüklenir). Bu da UIF set eder - dolayısıyla bazen *UG* kullanınca hemen interrupt ooluşabilir; bunu yönetmek gerekir.

## 3. UIF Nasıl Temizlenir?
- UIF'ı temizlemek için SR üzerinde uygun yazma yapılmalıdır. Tüm bayrakları temizlemek için **TIMx->SR = 0;** veya sadece UIF'i temizlemek için **TIMx->SR &= ~TIM_SR_UIF;** kullanılır.

## 4. Tipik Akış
Sıralama:

1. Timer yapılandırması: PSC, ARR, CR1 (ARPE gibi) yaz.
2. Preload güncellemesi gerekiyorsa TIMx->EGR = TIM_EGR_UG; kullan (dikkat: UG UIF set eder). 
3. Clear status: TIMx->SR = 0; (UIF varsa temizle). 
4. TIMx->DIER |= TIM_DIER_UIE; — Update interrupt enable. 
5. NVIC_SetPriority(...); NVIC_ClearPendingIRQ(...); NVIC_EnableIRQ(...); — NVIC tarafı. 
6. TIMx->CR1 |= TIM_CR1_CEN; — sayaç başlasın.


## Önemli NOT:
Bu sırayla başlamak çoğu "ilk an" istemsiz interrupt’ı engeller. (Alternatif: bazı kodlar DIER etkinleştirmesinden sonra NVIC’i aktif eder — ama mutlaka önce SR temiz olduğundan emin ol.

## 5. İlk Kod: Interrupt Timer

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

# OUTPUT Compare (OC)

## 1. Output Compare Nedir?
**Temel Fikir**

Timer sürekli sayıyor (CNT). CNT, **CCRx (Capture/Compare Register)** içindeki değere ulaştığında bir event oluşur.
Bu olay ile:
- Bir pin donanımsal olarak değiştirilebilir. (Toggle, Set, Reset)
- Bir interrupt tetiklenebilir.
- Veya ikisi birden yapılabilir.

Avantajı:
- Timer CPU'dan bağımsız çalışır.
- Çok hassas tepkiler verilebilir.
- Maint loop veya yazılım delay kullanmaya gerek kalmaz.

## 2. Temel Bileşenler

### 1. CCRx (Capture/Compare Register X)
Bu register'a CNT'nin ulaşmasını istediğin değeri yazarsın. CNT == CCRx olduğunda "Compare MAtc" olayı olur.
(CC1R, CC2R, CC3R, CC4R)

### 2. CCMRx (Capture/Compare Mode Register)
Kanalın modunu belirler:
- Output Compare (OCxM)
-Input Capture

OC modlarında ayrıca:
- Toggle (her eşleşmede pin tersler)
- Set
- Reset
- PWM mode

### 3. CCER (Capture/Compare Enable Register)
Kanalın çıkışını etkinleştirir. (OCxE bitleri)

### 4. TIMx_DIER (DMA/Interrupt Enable Register)
Compare Match kesmesini açmak için CCxIE bitini setlersin

### 5. TIMx_SR (Status Register)
- CCxIF (Copture/Compare INterrupt Flag) -> Compare Match olduğunda set olur.
- Yazılımda kesmede veya polling ile temizlenir.

## 3. Output Compare Modları
| Mod              | Açıklama                                   | Kullanım Örneği    |
| ---------------- | ------------------------------------------ | ------------------ |
| **Toggle**       | CCRx eşleşmesinde pin terslenir (0→1, 1→0) | Kare dalga üretimi |
| **Set**          | CCRx eşleşmesinde pin 1 yapılır            | Pulse başlatma     |
| **Reset**        | CCRx eşleşmesinde pin 0 yapılır            | Pulse bitirme      |
| **PWM Mode 1/2** | Belirli duty cycle’da kare dalga           | Motor, LED kontrol |


## Kod

    #include "main.h"


    void GPIOset(){

        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

        GPIOA->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7);

        GPIOA->CRL |= (0x02 << GPIO_CRL_MODE7_Pos); // Output 2 MHz
        GPIOA->CRL |= (0x00 << GPIO_CRL_CNF7_Pos);  // Push-pull
    }

    void TIMset(){

        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // TIM2 clock
        TIM2->PSC = 7199;                   // 10 kHz
        TIM2->ARR = 10000;                   // (Tam periyot için, ama burada gerekmez)
        TIM2->CCR1 = 5000;                   // Compare match değeri


        // OC1 Toggle modu
        TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
        TIM2->CCMR1 |= (0x3 << TIM_CCMR1_OC1M_Pos); // Toggle mode
        TIM2->CCER |= TIM_CCER_CC1E;               // CH1 enable

    }
    void TIM2_IRQHandler(void) {
        if (TIM2->SR & TIM_SR_CC1IF) {
            TIM2->SR &= ~TIM_SR_CC1IF; // Bayrak temizle
            GPIOA->ODR ^= (1 << 7);
        }
    }
    int main(){
        GPIOset();
        TIMset();
        // Compare interrupt açma
        TIM2->DIER |= TIM_DIER_CC1IE;
        NVIC_EnableIRQ(TIM2_IRQn);

        TIM2->CR1 |= TIM_CR1_CEN;

        while(1){

        }
    }

Timer'ın çalışma hızı: 8 MHz. Yani timer her 1/8.000.000 saniye de 1 artar. 1 tick = 1 / 8.000.000 = 0.125 mikro saniye (us)

**1. Adım: Prescaler (PSC) nedir?**
Prescaler, timer clock’unu böler.

Formül: Timer frekansı = 8 MHz / (PSC + 1)

Örnek: PSC = 7199

Timer frekansı = 8.000.000 / (7199 + 1) = 8.000.000 / 7200 = 1111 Hz

Yani timer sayacı saniyede 1111 kere artıyor.

1 timer tick = 1 / 1111 = 0.9 ms (milisaniye)


**2. Adım: ARR nedir?**
ARR, timer sayacının hangi değerde sıfırlanacağını (overflow) belirler.

Örneğin ARR = 10000

Timer 0’dan 9999’a sayar, sonra sıfırlanır.

Toplam süre = timer tick süresi × ARR

Süre = 0.9 ms × 10000 = 9000 ms = 9 saniye

**3. Adım: CCR (Compare Register) ne işe yarar?**
CCR, timer sayacının hangi değere ulaştığında “compare event” olacağını belirler.

Örnek CCR = 5000

Yani timer 5000’e geldiğinde event tetiklenir.

Süre = 5000 × 0.9 ms = 4500 ms = 4.5 saniye

**4. Adım: Toggle modunda sinyal frekansı nedir?**
Toggle modu pin değerini değiştirir (0→1 veya 1→0)

Toggle periyodun yarısında gerçekleşir (yani CCR’de)

Periyot = 2 × CCR süresi = 2 × 4.5 s = 9 saniye

Frekans = 1 / 9 saniye = 0.11 Hz

**Özet**

| Parametre       | Değer   | Açıklama                    |
| --------------- | ------- | --------------------------- |
| Timer clock     | 8 MHz   | Timer’ın temel çalışma hızı |
| PSC             | 7199    | 8 MHz / 7200 = 1111 Hz      |
| ARR             | 10000   | Sayaç 10000 adımda overflow |
| CCR             | 5000    | Compare event 5000. adımda  |
| Sinyal frekansı | 0.11 Hz | Toggle periyodu 9 saniye    |


# PWM