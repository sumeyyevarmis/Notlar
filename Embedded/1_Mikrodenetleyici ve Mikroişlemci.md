# Mikroişlemci ve Mikrodenetleyici Nedir?

## Mikroişlemci Nedir?
Yapısında bir CPU, ön bellek ve I/O portları olan devrelere *mikroişlemci* denir. Mikroişlemciler bulundukları elektronik yapıların beyinleridir.

Mikroişlemcinin beyni CPU'dur. Veri işleme ve veri akışı bu birim sayesinde gerçekleşir. Bu veri işlemleri genellikle CPU'nun içerisnde bulunan ALU(Artitmetik Mantık Birim)'de gerçekleşir. Bu birimde sayısal ve lojik işlemler yapılır. CPU'ların içerisinde 8-16-32-64 bitllik *register*'lar bulunur. Register'lar, bilginin geçici süre depolanmasını sağlar. CPU'lar mikroişlemcinin hafızasındaki programları bulma, çağırma ve onları çalıştırma görevi görür.

Mikroişlemciye gönderilen verleri ilk olarak hafızaya gelir ve burada depolanır. CPU'ların doğrudan eriştiği birim *bellek*'tir. Bellekte iki tane birincil hafıza vardır : **RAM** ve **ROM**. RAM(Random Accecc Memory), geçici hafıza birimidir, mikroişlemci kapandığında buradaki veriler silinir. ROM(Ready Only Memory), kalıcı hafız birimidir, sistem kapansa dahil buradaki veriler silinmez.

CPU'daki verilerin akışını, bellek ve I/O birimlerinin bağlantılarını sağlayan 3 çeşit **BUS**(veri yolu) vardır. Bunlar: Adress BUS, Data BUS ve Control BUS.
**Adress BUS:** Verinin okunacağı ya da verinin yazılacağı bölgeyi belirten adres bilgisi bu bus üzerinde sağlanır. Tek yönlüdür.
**Data BUS:** CPU'dan bellek ve I/O portlarına ya da bu birimlerden CPU'ya doğru çift yönlü bir veri hattıdır.
**Control BUS:** Mikroişlemcideki birimler arası iletişimi düzenleyen sinyalleri ileten, kontrol eden veri hattıdır. Her mikroişlemcide farklı sayıda hatta sahip Control BUS bulunabilir.

CPU içerisindeki hafıza birimlerine *register* denir. Işlemcinin çekirdeğinde olduklarından dolayı verilere ulaşmak daha hızlı gerçekleşir.

## Mikrodenetleyici Nedir?
Mikrodenetleyici (MCU, uC) MOSFET ve tümleşik devre(IC) üzerine kurulmuş, dışarıdan gelen bir veriyi hafızasına alan, derleyen ve sonucada çıktı veren bir bilgisayardır. Mikrodenetleyicinin yapısında:
- CPU,
- RAM,
- ROM,
- I/O Portları,
- Seri ve paralel portlar,
- Kristal Osilatör,
- Sayıcılar,
- Bazılarında A/D ve D/A Converter bulunur.

## Mikrodenetleyici ve Mikroişlemci Arasındaki Farklar Nelerdir?
Mikrodenetleyicilerin yapısında, mikroişlemciler de bulunmaktadır. Mikroişlemcilerin yapısında; CPU, ön bellek ve I/O portlarının olmasına karşın mikrodenetleyicide, seri paralel portlar, sayıcılar ve converter de bulunmaktadır. Mikrodenetleyiciler, gerçek zamanlı uygulamalarda daha başarılı çalışmaktadır. Mikrodenetleyiciler, bir işi çok küçük boyutlarda ve daha az enerji harcayarak yapabilmektedirler. Bunun yanısıra mikroişlemciler aynı anda daha çoklu işlem yapabilirken mikrodenetleyiciler  aynı anda tek bir iş yapabilirler.

## Mikrodenetleyici Türleri
- PIC Mikrodenetleyicisi,
- ARM Mikrodenetleyicisi (Acorn RISC Machine),
- 8051 Mikrodenetleyicisi,
- AVR Mikrodenetleyicisi,
- MSP Mikrodenetleyicisi.

## Mikrodenetleyiici Ne İşe Yarar?
Mikrodenetleyici, programmlandığı yazılımı hafızasına kaydedip, işleyebileceği şekilde derler ve bir çıkış sainyali gönderir. Kısacası, mikrodenetleyiciler elektronik devrelerde beyin görevi görür, elektronik sistemleri kontrol etmemizi sağlar.

## Mikrodenetleyici Çeşitleri
Mimarilerine göre 2 gruba ayrılır:

### 1. Hafıza Organizasyonun Açısından
#### a. Von Neumann Mimarisi
Program komutları ve veriler aynı bellekten alınarak tek bir yol üzerinden işlemciye gönderilir. Önce komut, daha sonra da veri işlenir. Geçmişte bu mimari tercih edilmekteydi. Kullanılan işlemci: 80x86, 68HC11 vb.

#### b. Harvard Mimarisi
Genellikle bu mimari tercih edilmektedir. Verilere ve komutlara farklı yollardan ulaşılır, bu sayede çalışması daha hızlıdır. Kullanılan işlemciler: 8051, PIC.

### 2. Komut İşleme Tekniği Açısından
#### a. RISC
RISC, küçükten özel komutları daha hızlı ve yüksek optimizasyonla yürütmektir.

#### b. CISC
CISC, daha çok basit derleyicilerle iyi çalışan iyi kişisel bilgisayarlar yapar.