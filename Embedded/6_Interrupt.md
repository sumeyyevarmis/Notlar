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
    
