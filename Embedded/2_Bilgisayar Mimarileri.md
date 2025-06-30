# Von Neumann ve Harvard Mimarisi
Von Neumann ve Harvard mimarileri, bilgisayar mimarisi alanındaki iki temel modeldir. Bir bilgiisayar sistemindeki bellek ve işlem birimlerinin organizasyonunu açıklar.

## Von Neumann Mimarisi
Program verilerinin ve komut verilerinin aynı bellekte depolandığı, bilgisayar mimarisidir.

### Avantajları
- **Basitlik:** Tüm veri ve komutların tek bir bellek alanında depolanması, yolların çakışması nedeniyle karmaşık yönlendrme oluşturmaya gerek kalmaz.
- **Maliyet Düşüklüğü:** Diğer mimari tasarımlara kıyasla daha az sayıda bileşene ihtiyaç duyulmaktadır.
- **Esneklik:** Bir program, devreler gibi bazı temel fiziksel unsurlarda bir değişiiklik yaşanmadan her zaman değiştirilebilir veya düzeltilebilir.

### Dezavantajları
- **Bottlenck Issues:** Paylaşılan veri yolu, veri ve kontrol komutlarının aynı anda alınamaması ve dolayısıyla yavaşlama nedeniyle sorunlar oluşabilir.
- **Bellek Bozulması:** Veriler ve komutlar aynı bellekte bulunduğundan, birinin diğerini silerek sistem hatalarına yol açma olasılığı vardır.

## Harvard Mimarisi
Komut ve veri için ayrı depolama ve ayrı veri yolları bulunan bilgisayar mimarisidir. Temel olarak Von Neumann mimarisindeki **darboğaz(bottlenck issues)** sorununu aşmak için geliştirilmiştir.

### Özellikleri
- Komut ve veri için ayrı bellek alanları,
- Sabit komut uzunluğu,
- Paralel komut ve veri erişimi,
- Daha verimli bellek kullanımı,
- Gömülü sistemler için uygundur,
- Sınırlı esneklik.

### Avantajları
- **Daha Hızlı İşlem:** Ver ve komutlar için iki veri yolunun bulunması, yalnızca bir veri yolunun kullanıldığı durumlarda ortaya çıkabiilecek çekişme sorununu ortadan kaldırır.
- **Gelişmiş Güvenlik:** Bu şekilde veriler, komutlarla aynı yerde saklanmadığından, bellek bozulması olasılığı azdır.
- **Kaynakların Verimli Kullanımı:** Veriler ve farklı büyüklükteki komutlar için farklı belleklerin kullanılmasını sağlayarak veri yollarının ve diğer kaynakların en iyi şekilde kullanımını sağlar.

### Dezavantajları
- **Karmaşıklık:** Bu türün uygulanması ve tasarımı daha karmaşıktır, bu nedenle diğer donanım olanaklarına ihtiyaç duyar.
- **Daha Yüksek Maliyet:** İki adet bellek seti ve iki ayrı veri yolu gerektirdiğinden, uygulama maliyetleri Von Neumann mimarisine kıyasla daha yüksektir.
- **Daha Az Esneklik Rakipleri:** Farklı bellek bölgeleri nedeniyle sistemi değiştirmek veya hatta geliştirmek bile biraz zor olabilir.

## Von Neumann vs Harvard Mimarisi

|Von Neuman|Harvard|
|----------|-------|
| - Depolanmış program bilgisayarkavramına dayanan eski bir bilgisayar mimarisidir. | - Harard Mark1 röle tabanlı modele dayanan modern bilgisayar mimarisidir.
| - Komutlar ve veriler için aynı fiziksel bellek adresi kullanılır. | - Komutlar ve veriler için ayrı fiziksel bellek adresi kullanılır.
| - Veri ve komut aktarımı için ortak bir veri yolu vardır. | - Veri ve komut aktarımı için ayrı veri yolları kullanılır.
| - Tek bir komutun yürütülmesi için iki saat döngüsü gerekir. | - Bir komu tek bir çevrimde yürütülür.
| - Maliyeti Daha ucuz. | - Von Neumann mimarisine göre daha maliyetli.
| - CPU aynı anda hem komutlara erişip hemde okuyup/yazamaz. | - CPU aynı anda hem komutlara erişebilir hem de okuma/yazma yapabilir.
| - Kişisel bilgisayarlarda ve küçük bilgisayarlarda kullanılır. | - Mikrodenetleyicilerde ve sinyal işlemede kullanılır.

# RISC ve CISC Mimarileri
Mikroişlemciler bellek yönetimi ve komut işleme tekniği açısından farklı mimarileri barındırabilirler. Bellek yönetimi açısından Von Neumann ve Harvard mimarileri kullanılırker, komut işleme tekniği açısından RISC ve CISC mimarileri kullanılır.

## CISC İşlemci Mimarisi Nedir?
CISC (Complex Instruction Set Computer), karmaşık assembly komutları ve dolayısıyla komplex devrelerden oluşan bir mikroişlemci mimarisidir. Bu nedenle kod çevrimi aşamaları daha yavaşken, kodların derlenmesi daha kısadır.

CISC mimarisiyle tasarlanmış mikroişlemci, çok sayıda komut içerdiğinden dolayı karmaşık devrelere, donanım yüküne ve tümdevre boyutuna daha çok ihtiyaç duyar. Bu da maliyeti büyük orada arttırır. Ddiğer yandan CISC mimarisinde her eylem için bir komut tanımı yapıldığından kod çevrimi aşamaları uzun sürer.

CISC'de donanım, tasarımdan hızlıdır düşüncesi hakimdir.

### Özellikleri
- Kod çevrimi daha kısa sürede geçekleşir.
- Kod çevrimi aşamaları uzundur.
- İşlemci için tasarlanmış assembly komutları karmaşıktır.
- Eylemler için fazlaca komut tanımlanmıştır. Bu yüzden karmaşık komutlar, karmaşık kod çözücüler ve daha çok donanıma gereksinim duyulur.
- Komutlar, karmaşık adresleme modlarıyla bellekte arayüz oluşturur.
- Talimatların yürütülmesi genellikle 1'den fazla clock alır.
- Çeşitli komutları çalıştırmak için mikro-kod kullanılır.

### Örnekler
Intel x86 serisi, PDP-11, IBM 370/168, Microvax 2, Motorola 68000.

## RISC İşlemci Mimarisi Nedir?
RISC (Reduced Instruction Set Computer), basti komutlar kullanan ve komutları daha hızlı işleyen bir işlemci mimarisidir. RISC mimarisinde karmaşık komutlar kullanılmadığında dolayı, komutlar hızlı işetilir. Basit komutlar ve basit kod çözücüler nedeniyle karmaşık devre elemanlarına olan gereksinim daha azdır, böylece maliyetten de kazanç sağlanır.

RISC mimariisinde komutlar tekdüze olduğundan kod çavrimi daha basittir. Ek olarak RISC mimarisinde, CISC mimarisinin tek bir komutla yapabileceği işlemin daha fazla kodla yapılmasına ihtiyaç duyacaktır. Buda daha uzun programlama kodu meydana getirir.

### Özellikleri
- Derleyiciye sayıcıya fazla karmaşık kodlar iletilir.
- Talimatlar bir saat döngüsünde yürütülür.
- Aynı işev için fazla kod yazılması gerekir. 
- MikKod çevrimi aşamaları mikro kod çevrimi aşamalarından geçmediğinde hızlıdır.
- Daha sade ve anlaşolor kod kullanılır.
- Komut sayısı az olması nedeniyle daha az donanıma ihtiyaç duyulacağından daha basit kod çevrimi yapılır.

### Örnekler
Alpha, ARC, ARM, SUN SPARC, MIPS R2000, Intel i860, Motorola 8800, IBM R5/6000

## CISC vs RISC
|CISC|RISC|
|----|----|
| - Karmaşık komutlar kullanılır. | -Basit komutlar kullanılır.
| - Donanıma yazılımdan hızlıdır prensibine bağlı kalınarak donanıma odaklanılır.| - Yazılıma odaklanılır.
| - Mikro kod çevrimi aşamları nedeniyle kod çevrimi uzun sürer. | - Mikro kod çevrim aşamaları olmadığında kod çevrimi hızlıdır.
| - Kod sadedir. | - Kod büyüklüğü artabilmektedir.
| - Doanım masrafı çoktur. | - Donanım masrafı azdır.

# STM32F103C6
72 MHz frekansında çalışan yüksek performanslı ARM Cortex M3 32-bit RISC çekirdeğini, yüksek hızlı gömülü bellekleri (32 KB'a kadar Flash bellek ve 6 KB'a kadar SDRAM) ve iki APB veri yoluna bağlı kapsamlı bir gilişmiş I/O ve çevre birimleri yelpazesini bünyesinde barındırır.

İki adet 12-bitlik ADC, üç adet genel amaçlı 16-bitlik timer ve bir adet PWM timer'ın yanı sıra standar ve gelişmiş iletişim arayüzleri sunuyor
 iki adede kadar I2C ve SPI, üç adet USART, bir adet USB ve bir adet CAN.