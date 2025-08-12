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