System reset, power reset  ve backup domain reset olarak tanımlanan üç tür reset vardır.

# Dökümandan
## 1. System Reset
System reset, clock controller CSR register'daki reset flaglari ve backup domain'daki registerler hariç tüm register'ları resetleme değerine ayarlar.

Aşağıdaki olaylardan biri gerçekleştiğinde System Reset gerçekleşir:
- NRST pininde low seviye (external reset)
- Window watchdog end of count koşulu (WWDG reset)
- Independent(bağımsız) Watchdog end of count koşuku (IWDG reset)
- Software reset (SW reset)
- Low-power management reset

Reset source, Control/Status register'daki reset flag'lari kontrol edilerek belirlenebilir (RCC_CSR)

### Software Reset
Cortex-M3 Application Interrupt ve Reset Control register'daki SYSRESETREQ biti, cihazlarda bir software resetlemesini zorlamak için set edilmelidir.

### Low-Power Management Reset
Low-power management reset'i oluşturmanın iki yolu vardır:
1) Standby moda girerken oluşturulan reset:
Bu tür resetleme, User Options Bytes'daki nRST_STDBY biti resetlenerek etkinleştirilir. Bu durumda bir Standby mode'u giriş dizisi başarıyla yürütüldüğünde, cihaz Standy moduna girmek yerine resetlenir.

2) Stop moduna girerken reset:
Bu tür resetleme, User Options Bytes'daki nRST_STOP biti resetlenerek etkinleştirilir. Bu durumda STOP moduna giriş dizisi başarıyla yürütüldüğünde, cihaz STOp moduna girmek yerine resetlenir.

## 2. Power Reset
Aşağıdaki olaylardan biri gerçekleştiriğinde bir Power Reset oluşur:
- Power-on/power-down reset (POR/PDR reset)
- Standby modundan çıkarken

Power reset, Backup Domain hariç tüm register'ları resetleme değerine ayarlar.

Bu sources NRST pinine etki eder ve delay phase'ı boyunca her zaman low tutulut. Reset servis rutini memory map de 0x0000_0004 adresine sabitlenmiştir.

Cihaza sağlanan System Reset sinyali NRST pininden çıkar, Pulse generator, her reset source'u (harici veya dahili resetleme) için minimumum 20 us'lik bir reset pulse süresini garenti eder. Harici bir resetleme durumunda, NRST pini low değerdeyken resetleme pulse üretilir.

## 3. Backup Domain Reset
Backup Domain, yalnızca backup domain'i etkileyen iki özel reset'e sahiptir. 

Aşağıdaki olaylardan biri gerçekleştiğinde bir Backup Domain Reset oluşur:
- Software reset, backup domaiin control register'da (RCC_BDCR) BDRST bitinin ayarlanmasıyla tetiklenir.
- Her iki besleme de daha önce kapatılmışsa, VDD veya VBAT gücü açılır.

## 4. Boot Configuration
STM32F10xx'de BOOT[0:1] pinleri aracılığıyla aşağıdaki tabloda gösterildiği gibi 3 farklı boot modes seçilebilir.

Boot Mode Selection Pins    |

BOOT1   | Boot0 |Boot Mode              | Aliasing                                      |
--------|-------|-----------------------|-----------------------------------------------|
X       | 0     | Main Flash Memory     | Main flash memory boot alanı olarak seçildi.  |
0       | 1     | System Memory         | System memory, boot alanı olarak seçildi.     |
1       | 1     | Embedded SRAM         | Embedded SRAM, boot alanı olarak seçildi.     |

Boot pinlerindeki değerler, resetlemeden sonra SYSCLK'nin 4. rising edge de kilitlenir. Gerekli boot modunu seçmek için reset'den sonra BOOT1 ve BOOT0 pinlerini ayarlamak kullanıcıya bağlıdır.

BOOT pinleri de Standby modundan çıkarken re-sampled'nir. Sonuç olarak Standby modunda gerekli Boot modu Configuration da tutulmalıdır. Bu başlatma gecikmesi geçtikten sonra, CPU 0x0000_0004 adresinden başlayarak memory'den kod yürütmeye başlar.

Bellek başlangıç adresleri:
- Kod alanı (Flash): 0x0000_0000 - CPU, sistem ilk açıldığında reset vektörünü buradan alır.
- Veri alanı (SRAM): 0x2000_0000 - RAM, burada başlar.

### BOOT Modları
#### 1. Flash'tan Boo
- Fflash hem 0x0000_0000 (aliased) hem de 0x0800_0000 adreslerinden erişilebilir.
- Varsayılan olarak buradan boot yapılır.

#### 2. System Memory'den Boot
- System memory 0x0000_0000 adresine yansıtılır.
- Gerçek adresi:
    - Connectivity line cihazlarda 0x1FFF_B000
    - Diğer cihazlarda 0x1FFF_F000
- İçindeki bootloader ise USB/UART gibi arayüzlerden programlama yapılır.

#### 3. SRAM'den Boot
- SRAM yalnızca 0x2000_0000 adresinden erişilir.
- **Önemli:** Bu durumda vektör tablosu RAM'e taşınmalıdır. (NVIC VTOR kullanılarak)

### XL-Density Cihazlar - Bank Seçimi
- Flash 2 bank'a bölünmüştür.
- Varsayılan: Bank1 (0x0800_0000)
- Bank2'den boot için, BFB2 bitini temizleyerek yapılandırılır.
    - Bank2 adresi: 0x0808_0000
    - Vektör tablosu buraya yeniden yerleştirilmedilir.

### Embedded Bootloader Özellikleri
- **Yer:** System memory içinde, ST tarafından önceden yüklenmiştir.
- **Amaç:** Seri arayüzlerden flash'ı yeniden programlamak.

Arayüzler:
- LOW/Medium/High density cihazlar = USART1
- XL-Density = USART1 veya USART2 (remapped)
- Connectivity Line cihazlar = USART1, USART2 (remapped), USB OTG FS (DFU modu)


**NOT!**
Bootloader USART üzerinden çalışırken HSI (8MHz iç osilatör) kullanılır. CAN ve USB OTG FS çalışması için ise harici osilatör (HSE) gereklidir.

# Özet
## 1. System Reset
System reset, yazılım ya da donanım kaynaklı çeşitli olaylarla MCU'nun **ana sistem bileşenlerinin sıfırlandığı** durumdur.

### Neler Resetlenir?
- CPU
- RAM içeri (SRAM)
- Çoğu periferin register'ları (GPIO, TIM, vb.)
- Clock konfigürasyonu (RCC)
- Stack pointer, PC (Program Counter) sıfırlanır.

### Ne Tetikler?
- NRST pin'ine dışarıdan füşük seviye uygulanması
- Watchdog zaman aşımları (IWDG veya WWDG)
- Yazılım reseti (SBC->AIRCR üzerinden)
- Hata durumlarında (ör: HardFault sonrası)

## 2. Power-On Reset (POR)
MCU'ya ilk defa enerji verildiğinde oluşan reset türüdür. Tam bir başlangıç sağlar.

### Neler Resetlenir?
- System reset'te etkilenen her şey
- Ayrıca power domain içi bloklar
- Backup domain dışı her şey temizlenir

### Özellik:
- Genellikle sadece sistemin ilk açılışında oluşur
- Çok güvenilir bir başlangıç sağlar

## 3. Backup Domain Reset
Backup Domain içindeki özel blokları (özellikle RTC ve backup register'ları) resetleyen işlemdir.

### Neye etki ederi?
- RTC clock ve konfigürasyonları
- Backup register'lar (BKP)
- Bölgedeki veriler, normal sistem resetinden etkilenmez - sadece bu reset türü ile temizlenir.

### Ne Tetikler?
- BKP yazma korumasının kaldırılıp yeniden etkinleştirilmesi
- RCC ayarları içinde BDRST biti kullanılarak yazılımda reset

## 4. Boot Configuration (BOOT0 ve BOOT1)
MCU ilk açıldığında **hangi bellekten başlayacağını** belirler. Yani reset sonrası PC (Program Counter) nereye gönderilecek?

### Bellek Seçenekleri
| BOOT1 | BOOT0 | Başlangıç Belleği             |
| ----- | ----- | ----------------------------- |
| x     | 0     | Main Flash Memory (Normal)    |
| 0     | 1     | System Memory (ST Bootloader) |
| 1     | 1     | Embedded SRAM (RAM boot)      |

- "System Memory" içinde ST'ninkendi bootloader'ı bulunur. USB veya UART üzerinden firmware yüklemek için kullanılır.
- "RAM boot" genelde test/debug için kullanılır.

### Nerede konfigüre edilir?
- Fiziksel pinler üzerinden (BOOT0)
- Kart tasarımında pull-up/pull-down ile sabitlenebilir
- Yazılımdan değiştirilmez-donanımsaldır

### Genel Akış Şeması (Reset & Boot)
STM32F103'te bir reset oluştuğunda yazılım ve donanım tarafında aşağıdaki sıralı süreç işler:

[Reset Olur]

↓

[Reset Türü Belirlenir]
- Power-on Reset
- System Reset
- Backup Domain Reset

↓

[Clock ve Memory yeniden yapılandırılır]
- RCC register’ları varsayılana döner
- Flash/Bus/SRAM erişim zamanlamaları hazırlanır

↓

[BOOT0/BOOT1 pinleri kontrol edilir]
- → Başlangıç belleği seçilir:
    - Main Flash
    - System Memory (Bootloader)
    - SRAM

↓

[Vector Table adresine zıplanır]
- → İlk adres: Stack Pointer
- → İkinci adres: Reset_Handler

↓

Reset_Handler()
- → SystemInit() fonksiyonunu çağırır
- → Saat ve temel ayarlar yapılır
- → Ardından main() fonksiyonuna geçilir

#### Açıklamalar:

- **Vector Table:** Genelde 0x0800_0000 adresinde yer alır ve ilk açılışta burada tanımlı reset vektörü kullanılır.  
- **Reset_Handler:** Yazılımcının kodundan önce çalışan, sistem başlatma işlemlerini yapan fonksiyondur.  
- **SystemInit():** Saat konfigürasyonu, cache ayarları gibi sistem düzeyinde yapılandırmaları içerir.  
- **main():** Kullanıcının uygulama kodu burada başlar.

---

#### Hedef Bilgi:
> Resetten sonra ne olacağını ve kodun nasıl başlatıldığını bilmek, bir mikrodenetleyici sisteminde *en temel yazılım kontrol akışını* anlamaktır.