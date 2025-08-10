# A. Interrupt
## 1. Interrupt Nedir ?
Interrupt en genel anlamıyla işlemcimizde anlık olarak işlenmekte olan olayın her hangi bir anında kesilmesi ve başka bir iş yapılmasına denir. İşlem kesildikten sonra alt işlem bitirilene kadar üst işlem parçacığına devam edilir. Dikkat edilmesi gereken nokta; hangi olayların kesme hangi olaylaın kesme olmadığıdır. Eğer bir işlemin normal akışı içerisindeki olaylar, başka bir olayı engelliyorsa bu her zaman interrupt olmaz.

## 2. Interrupt'ın Kullanım Amacı
- Bir çok işlemi aynı anda gerçekleştirebilmek
- İşlem bekletmekten kaçınmak
- Senkronizasyon gerektiren uygulama ve sistemleri tasarlamak

## 3. Polling ve Interrupt Arasındaki Fark
### Polling
Mikrodenetleyicinin veyya işlemcinin belirli bir kaynağı (örneğin bir buton, sensör, UART veri hattı vs.) sürekli olarak kontrol etmesidir.
#### Özellikleri
- Ana döngü içinde sürekli kontrol yapılır
- Sistem başka bir iş yapamadan hep kontrol eder
- Gecikmelere yol açabilir, verimsizdir.

#### Örnek
while(1){
  if(GPIOA->IDR & (1 << 0)) // PA0 pini high mı?
  {
    // butona baslmış, işlem yap
  }
}

### Interrupt
Bir olay gerçekleştiğinde (örneğin birpin high olduğunda, timer dolduğunda, UART veri geldiğinde) işlemciye bir *'kesme sinyali'* gönderilmesiyle işlemcinin o anda yaptığı işi bırakıp ilgili olaya tepki vermesidir.
#### Özellikleri
- Olay gerçekleşene kadar sistem serbesttir, başka işler yapabilir
- Gerçek zamanlı tepkiler sağlar
- Daha verimlidir
#### Örnek
void EXTI0_IRQHandler(void) // PA0 pini için kesme fonksiyonu
{
  if(EXTI->PR & (1 << 0)){
    EXTI->PR |= (1 << 0); // kesme bayrağını temizle
    // Butona basılmış, işlem yap
  }
}

## 4. ISR (Interrupt Service Routine) Nedir ?
Interrupt gerçekleştiğinde işlemcinin hemen çalıştırdığı özel fonksiyondur.
### ISR Nedir
- KEsme gerçekleştiğinde çağrılan fonksiyondur
- İşlemci, o anda yaptığı işi durdurur ve ilgili ISR fonksiyonuna geçer
- ISR içinde kesmeye neden olan olaya yanıt verir
- ISR tamamlandıktan sonra işlemci kaldığı yere döner ve normal işine devam eder.

 ### Örnek

void EXTI0_IRQHandler(void) // PA0 pini için kesme fonksiyonu
{
  if(EXTI->PR & (1 << 0)){
    EXTI->PR |= (1 << 0); // kesme bayrağını temizle
    // Butona basılmış, işlem yap
  }
}

### ISR Hakkında Önemli Noktalar
- ISR çok hızlı tamamlanmalıdır, uzun işlemler burada yapılmaz
- ISR içinde genellikle kesme bayrağı temizlenir
- ISR çalışırken başka ISR kesilebilir ya da engellenebilir (öncelik ayarlarıyla)
- ISR'ler gerçek zamanlı sistemlerin bel kemiğidir.

# B. NVIC: Nested Vectored Interrupt Controller

## 1. NVIC Nedir ?
Kesmeleri(interrupt) kontrol eden ve yöneten birimdir. Cortex-M3 çekirdeği ile birlikte gelir.

### Görevi:
- Kesmeleri aktifleştirmek veya devre dışı bırakmak
- Öncelik belirlemek
- İşlemciye kesme gerektiğinde hangi ISR çalışacak, onu yönetmek
- Aynı anda birden fazla kesme olursa sıralamak
- Daha öncelikli kesmeye anında geçiş sağlamak

## 2. NVIC Ne Yapar ?
Görev              |     Açıklama                                                |
-------------------|-------------------------------------------------------------|
Enable/Disable     | Hangi interrupt aktif, hangisi pasif belirler               |
Priority(öncelik)  | Her kesmeye bir öncelik verir. Küçük sayı = yüksek öncelik  |
Pendig Takibi      | Bekleyen kesmeyi "pendig" olarak işaretler                  |
Active Takibi      | O anda çalışan kesmeyi "active" olarak işaretler            |
Nested Interrupt   | İç içe kesmekere izin verir (önceliğe göre)                 |

## 3. NVIC = Öncelik (Priority) Nasıl Ayarlanır?
STM32F103'te sadece *preemption priority* kullanılır, *sub-priority* yoktur.

### C Fonksiyonu ile
NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority);

### Örnek
NVIC_SetPriority(EXTI0_IRQn, 0);
NVIC_SetPriority(USART1_IRQn, 2);

Burada; EXTI* kesmesi en yüksek öncelikli, USART1 daha düşük önceliğe sahiptir.

## 4. Hangi Interrupt Aktif/Pasif
### Aktifleştirme
NVIC_EnableIRQ(EXTI0_IRQn); // EXTI0 kesmesi aktif hale gelir

### Pasifleştirme
NVIC_DisableIRQ(EXTI0_IRQn); // EXTI0 kesmesini devre dışı bırakır

### Örnek = EXTI0 (PA0) Buton Kesmesi
// 1. NVIC'te EXTI0 kesmesini aktif et
NVIC_EnableIRQ(EXTI0_IRQn);

// 2. Önceliği belirle
NVIC_SetPriority(EXTI0_IRQn, 1);

// 3. ISR fonksiyonunu tanımla
void EXTI0_IRQHandler(void) // PA0 pini için kesme fonksiyonu
{
  if(EXTI->PR & (1 << 0)){
    EXTI->PR |= (1 << 0); // kesme bayrağını temizle
    // Butona basılmış, işlem yap
  }
}

## 5. Önemli Notlar
- NVIC ile her interrupt için ayrı ayrı kontrol ve öncelik tanımlanabilir.
- NVıC olmadan kesme sistemi çalışmaz
- NVIC kesmeleri maskeler, sıralar ve yönetir.
- STM32'de CubeMX kullanıyorsan bu ayarı arka planda yapar ama manuel yazarken kontrol sende.

## 6. User Manual'den
### Özellikler
- 16 programlanabilir öncelik seviyesi (4 bit kesme önceliği kullanılır)
- Low-Latency exception ve interrupt jandling (Düşük gecikmeli istisna ve kesme işleme)
- Power management control
- Implementation of System Control register (Sistem kontrol register'larının uygulanması)

NVIC ve processor (işlemci) core interface'i yakında bağlantılıdır, bu da low latency interrupt işlemine ve late arriving interrupt'ın verimli bir şekilde işlenmesine olanak tanır.

Core exception'ları da dahil olmak üzere interrupt'lar NVIC tarafından yönetilir.

## 7. NVIC Registerları
Register  | Açıklama
----------|-----------
ISER      | Interrupt Set Enable Register (Kesmeyi aktif yapar)
ICER      | Interrupt Clear Enable Register (Kesmeyi pasif yapar)
ISPR      | Interrupt Set Pending Register (Kesmeyi beklemeye alır)
ICPR      | Interrupt Clear Pending Register (Kesmeyi beklemeden çıkartır)
IPR       | Interrupt Priority Register (Öncelik ayarlanır)

## 8. NVIC Nerelerde Kullanılır
Uygulama                      | Açıklama
------------------------------|---------------------------------
Buton kesmeleri (EXTI)        | GPIO pininden gelen sinyal
USART veri alımı              | Veri geldiğinde kesme üretilir
Timer kesmeleri               | Süre dolunca interrupt 
ADC dönüşümü tamamlandığında  | Kesme ile haber verilir
DMA transfer tamamlandığında  | NVIC üzerinden kesme gelir.
    
# C. EXTI: External Interrupt Controller
## 1. EXTI Nedir?
EXTI (External Interrupt), STM32'nin dış dünyadan gelen sinyallere tepki vermesini saağlar. Yani; bir GPIO pini üzerinden gelen dış olay -> EXTI hattı -> Kesme (interrupt)

## 2. EXTI Hatları
STM32F103'te 16 adet EXTI hattı vardır.

ExtI Hattı  | Açıklama
------------|---------
EXTI0       | GPIOx pin 0 için
EXTI1       | GPIOx pin 1
....        | .....
EXTI15      | GPIOx pin 15

Her EXTI hattı aynı numaradaki GPIO pinlerine bağlanabilir. (PA0, PB0, PC0.. = hepsi EXTI0 hattına bağlanabilir ama aynı anda sadece biri seçilir.)

### GPIO-EXTI Hattı Bağlantı Örneği
// PA0 pinin EXTI0 hattına bağla
AFIO->EXTICR[0] &= ~(0xF << 0);
// EXTICR[0] (EXTI0 için), 0x0 = Port A

// EXTI0'ı rising edge ile tetikle
EXTI->RTSR |= (1 << 0); // Rising edge enable
EXTI->FTSR &= ~(1 << 0); // Falling edge disable

// EXTI0'ı etkinleştir
EXTI->IMR |= (1 << 0); // Interrupt Mask Register, EXTI0 aktif

// NVIC'te aktif et
NVIC_EnableIRQ(EXTI0_IRQn);

## 3. User Manual'den
External interrupt/event controller, bağlantı hattı cihazlarında 20 adede kadar kenar dedektöründen veya event/interrupt istekleri oluşturmak için diğer cihazlardan 19 adet kenar dedektöründen oluşur. Her input line, türü (riising or falling or both) seçmek için bağımsız olarak konfgürre edilebilir. Her line bağımsız olarak da maskelenebilir. Pending register, interrupt requests'nin sstatus satırlarını korur.

### 1. Main Features
- Her interrupt/event line'nı bağımsız tetikleme ve maske
- Her interrupt line için özel durum biti (dedicated status)
- 20 adede kadar software event/interrupt talebinin oluşturulması
- APB2 clock periyodundan daha düşük darbe genişliğine sahip external sinyalin algılanması.

### 2. Wakeup Event Management
STM32F10ccc, çekirdeği (WFE) uyandırmak için external veya internal event'leri idare edebilir.

Wakeup event şu şekilde oluşturulabilir:
- Peripheral control register'da bir kesmenin eetkinleştirilmesi ancak NVIC'de etkinleştirilmemesi ve Cortex-M3 System Control Register'da SEVONPEND bitinin etkinleştirilmesi
- Event modunda, external veya internal bir EXTI hattının yapılandırılması.

Connectivity line cihazlarda, Ethernet wakeup event'leri de WFE wakeup özelliğine sahiptir.

### 3. Functional Description
Interrupt oluşturmak için, interrupt line'ı configure ve enable yapılmalıdır. Bu, iki trigger register'nın istenen edge detection programlanması ve interrupt mask register'daki ilgili bite "1" yazılarak interrupt talebibnin etkinleştirilmesiyle yapılır. External interrupt line'da seçilen edge oluştuğunda, bir interrupt request oluşur. Interrupt line'a karşılık gelen "1" yazılarak resetlenir.

Event oluşturmak için, event line configure ve enable yapılmalıdır. Bu, iki trigger register'nın istenen edge detection ile programlanması ve event mask register'daki ilgili bite "1" yazılarak event talebinin etkinleştirilmesiyle yapılır. Event line da seçilen edge oluştuğunda, bir event pulse üüretilir. Event line'a karşılık gelen pending bit ayarlanmaz.

Bir interrupt/event request, software interrupt/event register'na "1" yazılarak yasılım tarafından da oluşturulur.

#### Hardware Interrupt Seletion
20 line interrupt source olarak yapılandırmak için aşağıdaki prosedürü kullanın;
- 20 Interrupt Line'ın mask bitlerini yapılandırma (EXTI_IMR)
- Interruprt Line'larının Trigger selection bitlerini yapılandırın (EXTI_RTSR ve EXTI_FTSR)
- External Interrupt Controller (EXTI) ile işleşen NVIC IRq kanalını kontrol eden enable ve mask bitlerini yapılandırın, böylece 20 line'dan birbirlerine gelen interrupt doğru şekilde anaylanabilir.

#### Hardware Event Selection
20 Line event source olarak yapılandırmak için aşağıdaki prosedürü kullanın;
- 20 Line'nın mask bitlerini yapılandırın (EXTI_EMR)
- Event Line'larının Trigger Selection bitlerini yapılandırın (EXTI_RTSR ve EXTI_FTSR)

#### Software Interrupt/Event Selection
20 line, software interrupt/event Line'ları olarak yapılandırılabilir. Aşağıda bir software interrupt'ı oluşturmak için prosedür vardır:
- 20 Interrupt/event Line'nın mask bitlerini yapılandırın. (EXTI_EMR ve EXTI_IMR)
- Software interrupt register'nın (EXTI_SWIER) gerekli bitini ayarlayın.

### 4. Interrupt ve Event
#### Inteerrupt
CPU'yu keser ve ilgili ISR fonksiyonuna atlar.

#### Event
CPU'yu kesmez. Donanıma (örneğin Timer, DMA,RTC) olay bildirimi gönderir.

# D. AFIO: Alternative Function I/O
Mikrodenetleyicilerde, GPIO pinlerinin özel ievler kazanmasını sağlayan birimdir.

## 1. AFIO Nedir?
STM32F1 serisinde GPIO pinleri *alternnatif fonksiyonlara* yönlendirmek için kullanılır.

**Örneğin**
- PA0 pinini USART1_TX yapmak
- PA8 pinini MCO (saat çıkışı) yapmak
- PB6 pinini I2C1_SCL yapmak
- PA0 pinini EXTO0 kesmesine bağlamak

**Not**
Kısacası; GPIO pininin ne görevde çalışacağını AFIO üzerinden belirlersiniz.

## 2. AFIO Ne Yapar
Görev                                 |     Açıklama
--------------------------------------|---------------------------------------------
GPIO pinini alternatife yönlendiriri  | Örn-> USART1_TX
EXTI hatllarına port seçimi yapar     | EXTI0 için PA0 mı PB0 mı?
Debug pinlerini serbest bırakır       | SWD, JTAG pinlerini GPIO olarak kullanmak için

## 3. AFIO Register'ları
Register          |     Açıklama
------------------|---------------------------------------------------------------
AFIO->MAPR        | Pin yönlendirme ve JTAG yapılandırma
AFIO->EXTICR[x]   | EXTI hatlarının hangi GPIO portuna bağlı olacağını belirler
AFIO->EVCR        | Event çıkılı için konfigürasyon  (genelde kullanılmaz)

### En Önemlisi-> AFIO->EXTICR[x]  
Bu register'lar EXTI0-EXTI15 hatlarının hangi GPIO portından tatikleneceğini belirler

EXTI Hattı    | Register        | Bit Konumu
--------------|-----------------|-------------
EXTI0         | EXTICR[0]       | Bit 3:0
EXTI1         | EXTICR[0]       | Bit 7:4
EXTI2         | EXTICR[0]       | Bit 11:8
....          | ...             | ....
EXTI15        | EXTICR[3]       | Bit 15:12

### Port Kodları
GPIO Port | Değer
----------|------
PA        | 0x0
PB        | 0x1
PC        | 0x2
PD        | 0x3
PE        | 0x4

### Örnek EXTI0 için PB0 Kullanmak

AFIO->EXTICR[0] &= ~(OxF << 0); // Önce sıfırla
AFIO->EXTICR[0] |= (0x1 << 0); // 0x1 = PortB

### AFIO->MAPR 
Bazı pinlerin varsayılan alternate fuction'ı saabittir, bazıları yönetilebilir.

#### Remap Kullanımı
RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // AFIO clock enable
RCC->MAPR |= AFIO_MAPR_USART3_REMAP_FULLREMAP;

### Örnek: PB3 pinini EXTI3 olarak tanımlama
RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; //AFIO saatini aç

AFIO->EXTICR[0] &= ~(OxF << 12); // EXTI3 için temizle
AFIO->EXTICR[0] |= (0x1 << 12); // 0x1 = PortB

EXTI->IMR |= (1 << 3); // Interrupt Mask aktif
EXTI->RTSR |= (1 << 3); // Rising edge
NVIC_EnableIRQ(EXTI3_IRQn); // NVIC'te aktif et

### AFIO Neden Önemlidir?
- Aynı numaralı GPIO pinleri farklı portlarda olabilir. (PA0, PB0, PC0,...)
- EXTI0 hattına sadece bir porttan bir pin bağlanabilir.
- AFIO->EXTICR[] ile hangi portun seçileceğini sen belirlersin
- Remap olmazsa bazı preferkler çalışmaz ya da çakışır.
- JTAG'ı devre dışı bırakmadan bazı pinler GPIO yapılmaz.
