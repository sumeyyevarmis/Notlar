## CubeMX RCC Ayarları

## RCC Nedir?

RCC, STM32'nin tüm çevresel birimlerine ve işlemci çekirdeklerine saat (Clock) sinyali değıtan bloktur. CubeMX'de yanış yapılandırılmış bir RCC, timerların yanlış frekans üretmesi, UART bound rate hatası veya ACI/AAHB/APB bus'larına bağlı periferallerin düzgün çalışmaması gibi sonuçlara yol açar.


## STM32H755'in Saat Kaynakları

Önce H755'teki fiziksel saat kaynakları;

![Clock-Source](doc/6_1-STM32-Clock-Source.png)


## CubeMX RCC Sekmesindeki Ayarlar

CubeMX'te Pinout&Configuration -> System Core -> RCC altında şu seçimler yapılır;

- **High Speed Clock (HSE):** Nucleo-H755ZI-Q kartında kristal bulunur ancak varsayılan konfigurasyonda kullanılmaz. ST-Link, MCO çıkışından OSC_IN pinine 8 MHz kare dakga sinyali verir. Bu yüzden; **HSE = Bypass Clock Source**. Crystal/Ceremic Resonator seçilirse OSC_OUT da sürülmeye çalışılır, karşısında aktif kristal olmadığından saat başlamaz veya kararsız kalır.

- **Low Speed Clock (LSE):** RTC veya IWDG kullanılacaksa 32.768 kHz kristal için **Crystal/Ceremic Resonator seçilir**. Kullanılmayacaksa **Disable** bırakılır.


## Clock Configuration Sekmesi

PLL hesabı şu formülle yapılır;
````
f_VCO = HSE * (N/M)
f_SYSCLK = f_VCO / P
````

Nucleo-H755 için standart 480 Mhz konfigürasyonu;

| Parametre | Değer     | Açıklama                  |
|-----------|-----------|---------------------------|
| HSE       | 8 MHz     | ST-Link MCO sinyal        |
| /M        | 4         | Giriş bölücü -> 2 MHz     |
| *N        | 240       | VCO çarpanı -> 480 MHz    |
| /P        | 1         | SYSCLK bölücü -> 480 MHz  |

- VCO frekansı 192 MHz - 836 MHz arasında olmalıdır. 2 * 240 = 480 MHz, limit içindedir.

- Bus bölücüleri;

| Bus           | Frekans   | Bölücü            |
|---------------|-----------|-------------------|
| SYSCLK        | 480 MHz   | /1                |
| HCLK/AXI      | 240 MHz   | /2 (D1CPRE)       |
| APB1/2/3      | 120 MHz   | /2 (PPRE)         |
| CM4 (HCLK3)   | 240 MHz   | ayrı ayarlanır    |


- Adım adım yapılacaklar;
    1. PLL Source Mux -> HSE seç
    2. PLL1: M = 4, N = 20, P = 1
    3. System Clock Mux = PLLCLK
    4. D1CPRE = /1 -> SYSCLK = 480 MHz
    5. HPRE = /2 -> HCLK = 240 MHz
    6. D2PPRE1, D2PPRE2, D3PPRE = /2 -> APB'ler 120 MHz
    7. CubeMX hataları kırmızıyla gösterir - tüm kutular temizlenene kadar düzenle.

 ## Timer Clock (TIMxCLK) - Önemli
 APB prescaler 1'den farklıysa (bölünüyorsa), timer saati otomatik olarak *2 çarpılır;

 **APB1 = 120 MHz, rescaler = /2 -> TIMxCLK = 240 MHz**

 PSC ve ARR hesabı yapılırken CubeMX'in Clock Configuration sekmesinde timer'ın yanında yazan frekans esas almak gerekir.

 ## Neye Göre Karar Verilir? 
 - Kullanılacak periferallerin maksimum saat gereksinimi
 - Güç tüketimi (düşük güç -> düşük saat)
 - USB kullanımı -> 48 MHz tam sayı katı gerekir, PLL2 / PLL3 devreye girer.
 - Ethernet gibi özel periferal clock gereksinimleri, Stansart kullanım (timer, UART, SPI, vb.) için 480 / 240 / 120 MHz konfigürasyonu en sağlıklı başlangıçtır.

## Neden SMPS Olmak Zorunda ?
- STM32H755'in içinde iki farklı güç regülatörü vardır; **LDO** (linear Dropout Supply) ve **SMPS** (Switching Mode Power Supply). Bunlar işlemci çekirdeğine giden dahili voltaj üretir.

- LDO basit bir devredir. Giriş voltajını ısı olarak harcayarak düşürür. Herhangi bir harici bir bileşen gerektirmez ama verimlilik düşüktür.

- SMPS ise anahtarlamalı bir devredir. Enerjiyi bir bobinde depolayıp serbest bırakarak çok daha verimli çalışır. Ama harici bobin ve kondansatör gerektirir.

- STM32H755'in içinde dahili bir SMP devresi bulunur. Bu devrenin çalışması için gereken harici pasif bileşenler (bobin ve kondansatör) Nucleo kartı üzerinde zaten lehimlidir. ST, bu kartı varsayılan olarak SMPS konfigürasyonuyla tasarlamıştır. Bu yüzden yazılımda **PWR_DIRECT_SMPS_SUPPLY** seçilmelidir.

## Parameter Settings

### 1. PWR Supply Configuration

| Seçenek                      | Anlamı                              |    
|------------------------------|-------------------------------------|
| PWR_LDO_SUPPLY               | Sadece LDO                          |
| PWR_DIRECT_SMPS_SUPPLY       | Sadece SMPS                         |
| PWR_SMPS_1V8_SUPPLISE_LDO    | SMPS 1.8 V üretir, LDO'yu besler    |
| PWR_SMPS_2V5_SUPPLIES_LDO    | SMPS 2.5 V üretir, LDO'yu besler    |
| PWR_SMPS_1V8_SUPPLUES_EXT    | SMPS harici devreyi besler          |
| PWR_EXTERNAL_SOURCE_SUPPLY   | Harici güç kaynağı                  |

 - Nucleo kartında default SMPS olduğu için: **PWR_DIRECT_SMPS_SUPPLY** seçilir.

### 2. Voltage Scale (Core Voltage)
İşlemci çekirdeğinin çalışma voltajını belirler. Daha yüksek voltaj -> daha yüksek frekans mümkün, ama daha fazla güç tüketimi

| Scale      | Çekirdek Voltajı  | Max. SYSCLK    | SMPS'de kullanılabilir mi?    |
|------------|-------------------|----------------|-------------------------------|
| Scale 0    | 1.35 V            | 480 MHz        | Hayır                         |
| Scale 1    | 1.25 V            | 400 MHz        | Evet                          |
| Scale 2    | 1.15 V            | 300 MHz        | Evet                          |
| Scale 3    | 1.05 V            | 200 MHz        | Evet                          |

- Nucleo + SMPS + 240 MHz için Scale 2 makul ve güvenilir bir seçimdir.

### LSE Drive Capability
LSE (32,768 kHz düşük hız kristali) kullanılıyorsa, kristalin sürüş gücünü belirler. Dört kademesi vardır; **LOW / MEDIUM LOW / MEDIUM HIGH / HIGH**

Kristal ne kadar büyük kapasitans değerine sahipse o kadar yüksek sürüş gücü gerektirir. Çoğu standart 32,768 kHz kristal için **LOW** veya **Medium LOW** yeterlidir. Yanlış seçilirse, kristal salınımı başlatmaz.

Eğer LSE kullanılmıyorsan bu parametrenin önemi yoktur.

### 4. HSE / LSE CSS (Clock Security System)
CSS (Clock Security System) saat kaynağını izleyen bir donanım güvenlik mekanizmasıdır. HSE veya LSE arızalanırsa (osilatör durur, sinyal gelmez), CSS bunu algılar ve otomatik olarak yedek saat kaynağına geçer - HSE arızalanırsa HSI'ya döner. Aynı zamanda interrupt üretir. Böylece yazılımda arızandan haberdar olur.

**Enable / Disable**

Kritik bir uygulamada (medikal, otomotiv, endüstriyel) **Enable** edilmesi önerilir.
