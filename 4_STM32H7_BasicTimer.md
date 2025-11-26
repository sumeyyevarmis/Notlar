# 1. Basic Timer Nedir?

STM32 içinde en sade ve en temel zamanlayıcı türüdür. Bu timer'lar diğer gelişmiş timer'lara göre daha basit, hafif ve sınırlı özelliklere sahiptir.

Basic timer şunları yapar:
- Belirli bir süre sayıp update event üretir. Bu genelde interrupt tetikler.
- Interrupt üretir; Timer süreyi doldurduğunda, update interrupt (UIE) üretir. Kodun içindeki callback çalışır.
- DAC için trigger olabilir; DAC için tetikleme kaynağı olabilir.

Basic timer'ın yapamadıkları:
- PWM oluşturamaz
- Output compare yok; bir GPIO pinine dalga üretmez.
- Input Capture yok; bir sinyalin frekansını veya süresini ölçemez.
- Encoder mode tetiklemez
- GPIO çıkışını kontrol edemz; tek başına pine bağlı değildir.

Basic timer kullanılabileceği yerler:
- Zaman tabanlı interrupt'lar;
  * Her 1ms'de bir bir task çalıştırma
  * Sistem zamanlayıcısı oluşturma
  * LED blink gibi periyodik işler
- DAC trigger üretme
  * DAC sampling hızını ayarlamak için
- Free-running sayacı
  * Sadece sayan bir sayaç olarak

Basic Timer tercih nedenleri:
- **Çok hafiftir;** Kesme üretme dışında fazla şey yapmadığından düşük güç tüketimi ve daha az konfigürasyon karmaşıklığı vardır.
- **Timer pinlerine ihtiyacı yok;** GPIO'a bağlı olmadığı için kart tasarımında yer kaplamaz.
- **CPU yükü düşük;** Basit yapı = rahat kontrol

Basic timer'ların içinde;
- Channel yok
- Output compare unit yok
- Capture unit yok

Bu nedenle, PWM, Output Compare, Input Capture işleri yoktur.

# 2. Prescaler Nedir?

Prescaler, timer'ın giriş clock frekansını bölerek yavaşlatan bir bölücüdür. Başka bir değişle; Timer'ın sayma hızını azaltmak için kullanılır.

## a. Timer'a Giden Clock Neden Bölünür
Çünkü STM32'de Timer clock'ları genelde yüksektir.

Örneğin: STM32H7 'de TIM6 clock'u 240 MHZ.
Timer her clock vuruşunda 1 sayar.
Yani prescaler = 0 olsaydı:
- Timer 240 milyon kez/saniye sayardı.
- 16-bir timer -> 65535'e ulaşıp taşması çok hızlı olurdu

Hesaplarsak:

T = (ARR + 1) / 240000000 = 65536 / 240000000 ≈ 0.00027 s

Yani;
- 0.00027 saniye ≈ 27 ms
- Timer saniyede ∽ 3700 kez taşardı (3700 Hz)
Bu, LED blink gibi işler için tamamen kullanışsız.

İşte bu nedenle Prescaler vardır. Yüksek frekansı daha düşük frekansa bölmek için.

Mesala: PSC = 23999 (yaklaşık 2400)
Ftimer = 240 Mhz / 24000 = 10 kHz

Artık timer saniyede 240 milyon değil, 10 bin kez sayar. Bu da insan ölçeğinde süreler üretmeyi mümkün kılar.

## b. Prescaler Ne İşe Yarar?
- Timer'ı insan ölçeğine getirir:
  * 1 saniyelik süre elde etmek için prescaler gereklidir.
- Timer çözünürlüğünü ayarlarsın:
  * PSC düşük -> daha yüksek çözünürlük
  * PSC yüksek -> daha düşük çözünürlük ama daha uzun süreler
- Timer periyot hesaplamasını kolaylaştırır.
  * Bu süreleri ARR'ye bindirmek yerine PSC'ye paylaştırırsın.
 
## c. Prescaler Olmasaydı Ne Olurdu
- Timer çok hızlı taşardı
- Uzun süreler imkansız olurdu
- Timer kullanımı kısıtlı olurdu
- Yüksek frekanslı interrupt yağmuru olur. Interrupt 3700 Hz gibi aşırı hızda olur -> CPU çöker.

## d. Tek Cümle ile Prescaler
**Prescaler, timer'ın giriş frekansını bölerek daha uzun ve kullanılabilir süreler üretmeyi sağlar. Prescaler olmazsa timer aşırı hızlı çalış ve pratik süreler oluşturulamaz.**

# 3. Auto Reload Register

Auto-Reload Register (ARR) timerın sayacağı üst sınırı belirleyen register'dır. Aslında timerın "periyodunu" belirleyem kilit nokta burasıdır.

## a. ARR Nedir?
Timer sayacı (CNT) 0'dan başlar ve ARR değerine kadar sayar. ARR'ye ulaştığında overflow olur ve;
- CNT tekrar 0'a döner
- Update event (UEV) oluşur
- Istersen interrupt tetiklenir.

Yani ARR; "Timerın bir döngüsünün kaç sayımda tamamlanacağını belirleyen" register'dır.

## b. ARR Ne İşe Yarar?

1) Timer frekansını/periyodunu ayarlarsın: Timer çıkış frekansını belirleyen iki parametre vardır:
  - PSC(prescaler) -> Zaman tabanlı bölücü
  - ARR(Auto-reload) -> Sayaç üst sınır.
2) Interrupt aralığını belirler: Örneğin 1ms'de bir interrupt istiyorsan ARR onu sağlayan değerdir.
3) PWM modunda duty/periyot hesaplarında periyodu belirler (TIM6 PWM değil ama mantık aynı)

## c. Neden ARR Kullanılır?
Çünkü timerın periyodunu değiştirebileceğin tek yer burasıdır. Prescaler ile clock'u bölersin -> ARR ile bu bölünmüş clock'un kaç sayıp yapacağını belirlersin. Bu sayede:
- Timer çok hızlıysa periyodu uzatmak için ARR'yi büyütürsün
- Timer çok yavaşsa daha kısa periyot için ARR'yi küçültürsün
- Dinamik olarak periyot değiştirebilirsin (ARR güncellenmesi ile)

## ARR Nden/Ne Zaman Değişir?
- PSC'nin çok büyümesi gerekiyorsa. PSC maksimum 65535 olabilir.
- Yüksek çözünürlük gerekiyorsa. Eğer ARR çok büyükse, PSC çok küçük olur. PSC küçükse timer çözünürlüğü artar.
- İsenen süre ARR'nin kapasitesinden taşarsa
- PWM, DAC trigger veya başka modlarda ARR = Periyodun kendisir. PWM'de ARR periyodu belirler, CCR duty belirler.

Bu yüzden ARR'yi değiştirmek zorunlu olur.

# 4. Auto-Reload Preload Nedir? (Enable/Disable)

## a. Auto-Reload Preload (APRE) Nedir?
ARR register'ın güncellenme davranışını kontrol eder.
- Timer sayıyor -> CNT (counter) 0'dan ARR'ye kadar sayar.
- Eğer ARR runtime'da değişirse, yani değer hemen mi yoksa bir sonraki timer döngüsünde geçerli olacak, işte ARPE bunu belirler.

## b. ARPE Disable (Pasif)
ARR değeri değiştiği anda yüklenir. Sonuç:
- Timer o anda sayıyor, yeni ARR ile bir "glitch" oluşabilir -> sürede ani değişiklik, kısa bir periyot veya beklenmedik interrupt tetiklenmesi.
- Kısa süreli glitch'ler gözle görülemeyebilir ama hassas sistemlerde sorun olabilir.

**Özet:
ARR değiştiğinde hemen uygulanır, timer döngüsü beklenmez.**

## c. ARPE Enable (Aktif)
ARR değeri değiştirildiğinde buffer register'a yazılır. Yani değer sadece update event (UEV) geldiğinde geçerli olur. Sonuç:
- Timer glitch riski yok.
- Periyor değişikliiği düzgün ve kontrollü bir şekilde uygulanır.
- Hassas zamanlama gerektiren uygulamalarda tercih edilir.

## d. Neye Göre Karar Veriyoruz
|                              Durum                                     |         ARPE         |
|------------------------------------------------------------------------|----------------------|
| Timer çalışırken ARR'yi değiştirmeyeceksen                             | Disable yeterli      |
| ARR runtime'da değişecek (dinamik periyor, PWM duty, motor kontrol vs. | Enable önerilir      |
| Hassas periyot ve interrupt zamanlaması önemli                         | Enable tercih edilir |
| Basit timer ile tek seferlik delay                                     | Disable da olur      |

## e. Örnek (PWM gibi senaryo)
Timer PWM modunda, ARR runtime erişebilir. Eğer Enable değilse -> PWM dalga boyu glitch yapabilir. Eğer Enable ise -> PWM dalga boyu düzgün, bir sonraki cycle'da değişir.
