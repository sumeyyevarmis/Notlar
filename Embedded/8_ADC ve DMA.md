Date: 12-08-2025
Day: 30

# ADC

## ADC Nedir?
**ADC**, Analog-Dijital Dönüştürücü anlamına gelir. Gerçek dünyadaki analog sinyalleri (örneğin sıcaklık, ışık, voltaj gibi sürekli değişen fiziksel büyüklükler) sayısal (dijital) verilere çeviren bir bileşendir.

**Neden ADC'ye ihtiyaç vardır?**
- Mikrodenetleyiciler dijital cihazlardır. Yani işmcileri sadece 0 ve 1 şeklinde iki seviyeli sinyalleri anlayabilir.
- Ancak çevremizdeki birçok sinyal analogdur.
- Bu analog sinyal mikrodenetleyici tarafından işlenebilmesi için dijital forma çevrilmelidir.
- İşte bu dönüşümü yapan modüle ADC nedir.

## ADC Nasıl Çalışır?
1. Analog Giriş Sinyali: Örneğin 0-3.3V arasında değişen bir voltaj.
2. Örnekleme (sampling): ADC belli aralıklarla analog sinyalin değerini okur (örnekler).
3. Kuantizasyon (Quantization): Ölçülen analog değer belirli bir dijital aralığa yuvarlanır.
4. Dijital Kod Üretme: Örneklenen ve kuantize edilen sinyal ADC'nin bit sayısına göre (örneğin 12 bit) dijital kod olarak çıkış verir.

## Temel Kavramlar
- **Çözünürlük (Resolution):** ADC'nin kaç bit ile ölçüm yaptığıdır. Örneğin 12 bit ADC, 0-4095 arasında (2^12-1) değer üretebilir.
- **Referans Voltajı (Vref):** ADC'nin ölçüm yapacağı maksimum voltajıdır. Bu voltajın üzerindeki değerler, ADC maksimum değere eşitlenir.
- **Örnekleme Hızı (Sampling Rate):** ADC'nin birim zamanda kaç kere ölçüm yaptığı.
- **Sinyal Seviyesi:** Giriş sinyali 0 ile Vreg arasında olmalıdır.

## STM32'de ADC Mimarisi
STM32 mikrodenetleyicilerde ADC modülleri genelde 12 bit çözünürlüktedir (bazı modellerde 10 veya 16 bit olabilir). İşte temel yapı taşları:
### 1. ADC Kanalları
- STM32 ADC'leri genellikle çoklu kanala sahiptir (örneğin 16 kanala kadar).
- Her kanal bir GPIO pinine bağlıdır ve farklı analog girişlerden veri alabilir.

## 2. Referans Voltajı
- Genellikle MCU'nun besleme voltajı (3.3V) referans voltajı olarak kullanılır.
- Bazı modellerde dahili referans voltajı da mevcuttur.

## 3. ADC Çalışma Modları
- **Single Conversion Mode:** Tek seferlik dönüşüm yapar.
- **Continous Conversion Mode:** Sürekli dönüşüm yapar.
- **Scan Mode:** Birden fazla kanalın sırayla dönüştürülmesini sağlar.
- **DMA ile Çalışma:** ADC verisini DMA ile bellek transferi yapabilir, böylece işlemciyi az meşgul eder.

## 4. ADC Donanım Blokları
- **Sample and Hold:** Analog sinyali örnekler ve sabitler.
- **Successive Approximation Register (SAR):** Ardışık yaklaşık değer hesaplayarak dijital çıktığı oluşturur.
- **ADC Data Register:** Dijital sonuç burada tutulur.

## 5. Saat ve Prescaler Ayarları
- ADC, genellikle CPU saatinden farklı daha düşük frekansta çalışır.
- ADC için uygun saat ayarlanmalıdır ki dönüşüm sağlıklı olsun.

## 6. DMA Desteği
- ADC verileri DMA ile otomatik belleğe aktarılabilir, böylece işlemci boşta kalır.

# ADC Çalışma Süreci STM32'de Özet
1. ADC modülünü aktif et.
2. Giriş kanalı seçilir.
3. Referans voltaj ayarlanır (genellikle Vdd)
4. ADC dönüşümü başlatılır (software trigger, timer veya harici tetikleme olabilir)
5. ADC analog sinyali örnekler, dönüşümü tamamlar.
6. Dijital değer ADC data register'dan okunur.
7. Gerekirse DMA ile otomatik aktarım yapılabilir.

# Özet Tablo
| Kavram            | Açıklama                                   |
| ----------------- | ------------------------------------------ |
| ADC               | Analog sinyali dijitale çevirir            |
| Çözünürlük (bit)  | ADC'nin dijital kod uzunluğu (örn. 12 bit) |
| Vref              | Ölçüm yapılacak maksimum voltaj            |
| Kanal             | ADC'nin farklı giriş pinleri               |
| Sampling Rate     | Saniyedeki örnek sayısı                    |
| Single/Continuous | Tek seferlik veya sürekli ölçüm            |
| DMA               | Veri transferini otomatik yapar            |

# Kod
    #include "stm32f10x.h"

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

    int main(void)
    {
        uint16_t value;
        ADC1_Init();

        while (1)
        {
            value = ADC1_Read();
            // float volt = value * 3.3f / 4095.0f; // Voltaj hesabı
            for (volatile int i = 0; i < 50000; i++); // basit delay
        }
    }
