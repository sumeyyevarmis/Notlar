# 1. Output Compare Mod Nedir?
Output Compare, timer içindeki CNT sayacı belirli bir değere (CCR) ulaştığında;
- Timer  kanalı (TIMx_Chy) üzerinde bir olay üretmesidir.
Bu olay;
- Pin toggle olabilir
- Pin High yapılabilir
- Pin low yapılabilir
- Sadece bir iç tetik (interrupt/DMA/başka perifere trigger) olabilir

Burdaki ana fikir:
- **Zaman geldiğinde bir olayı otomatik tetiklemek.**

# 2. Nerede Kullanılır
Kullanım alanı geniştir, ama ortak nota şudur:
- **Kesin zamanlamaya ihtiyaç duyulan her durumda.**

## A. Periyodik Zamanlayıcı (Interrupt için)
Os kullanmadan, jittersiz timer kesmesi üretmek.
- CCR += sabit değer -> tam stabil periyot
- Uygulaması -> LED blink, zaman tabanlı, delay üretimi.

## B. Donanım Tetikleme
Timer, eşleşme anında diğer modülleri tetikler:
- ADC trigger
- DAC trigger
- DMA trigger
- Capture birimi tetikleme
- Event system (örn -> timer senkronizasyonu)
Bu, CPU'ya yük bindirmeden donanımın kendi kendine çalışmas demektir.

 ## C. Dakik "bit-banging"
 Protokol zamanlaması gerektiğinde.

 Örneğin:
 - WS2812 LED zamanlaması
 - Özel PWM-like darbeler
 - Hassas step motor darbeleri

## D. Pin Dalga Üretimi
Toggle modunda:
- CCR eşleşince pin terslenir
  - Kare dalga üretirsin


# 3. Neden Var?
- A. Yazılımdaki gecikmleri bitirir (jitter yok). CPU meşgul olsa bile.
  - Timer donanımı nanosecond-level kararlı çalışır
  - Zig-Zag yok, kayma yok
- B. Yazılımdan bağımsızdır.
  - Timer çalışır -> olay zamanı geldiğinde kendisi tetikler.
  - CPU'ya ihtiyaç yok.
- C. Kesin zamanlama gerektiren şeyleri CPU'dan alır. Mesela;
  - Interruppt latencies
  - Context switch gecikmeleri
  - Pipeline gecikmeleri
- D. PWM'nin temeli Output Compare'dir.
  - PWM = Output Compare + AutoReload dönmesi
  - CCR eşleşmesi ile duty ayarlanır.
- E. Multi-Channel zaman kontrolü, tek bir timer ile.
  - Bir timer 4 kanal -> her biri farklı CCR
  - 4 farklı olayı 1 timer ile planlarsın

# 4. Tek Cümle Özet
**Belirlediğin zamanda donanım tarafondan tetiklenen olay sistemi**
