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
