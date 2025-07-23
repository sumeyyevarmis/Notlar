# Interrupt
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
