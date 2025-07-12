# CLOCKS

System clock'u (SYSCLK) sürme için üç farklı saat kaynağı kullanılır:
- HSI oscillator clock
- HSE oscillator clock
- PLL clock

Ayrıca cihazlar aşağıdaki secondary clock kaynaklarına da sahiptir:
- LSI RC: 40 kHz düşük hızlı dakili (internal) RC bağımsız watchdog ve isteğe bağlı olarak Stop/Standby modunda Auto-wakeup için kullanılan RTC'yi çalıştırır.
- LSE Crystal: 32.768 kHz low speed external (harici) kristal, isteğe bağlı olarak real-time clock'u (RTCCLK) çalıştırır.

## HSE Clock (High-Speed External)
Dışarıdan bağladığı bir kristal (örnek: 8 MHz) veya clock sinyali (kare dalga). HSI'ya göre daha doğru ve kararlıdır.

Tür             | Açıklama                                              | Ayar          |
----------------|-------------------------------------------------------|---------------|
Crystal Mode    | MCU'ya 2 bacaklı krisal bağlarsın.                    | HSEBYP = 0    |
Bypass  Mode    | Clock sinyalini kare dalga olarak doğrudan verirsin.  | HSEBYP = 1    |


## HSI Clock ( High-Speed Internal)
Mikrodenetleyicinin içinde yerleşik bir saat kaynağıdır. Harici devre gerekmez, her zaman hazırdır. Frekansı sabittir => 8 MHz

Avantajları                 | Dezavantajları                                        |
----------------------------|-------------------------------------------------------|
Harici kristal gerekmez.    | HSE'ye göre daha az kararlıdır.                       |
Devre sade olur.            | Sıcaklıkta frekans sapması olabilir.                  |
Daha az yer kaplar.         | Kesin zamanlama gerektiren uygulamalarda önerilmez.   |

### HSE Nerede Kullanılır
- Boot sırasında varsayılan clock kaynağıdır. (MCU ilk açıldığında)
- Eğer HSE çalışmazsa yedek olarak kullanılabilir.
- İsteğe bağlı olarak sistem saat kaynağı olarak da seçilebilir.

## PLL (Phase-Locked Loop)
- PLL bir çarpan devresidir.
- Düşük frekanslı bir clock sinyalini alır. (Örneğin= 8 MHz)
- Bunu belli bir çarpanla çoğaltır. (Örneğin= x9)
- Sonuçta daha **yüksek bir clock** elde edilir.

### PLL Ne İşe Yarar
- HSE veya HSI gibi kaynaklar 8 MHz civarındadir.
- Ama sistemin hızlı çalışması için bu clock'u arttırmak istersiniz.
- PLL ile bu frekansı çarpıp, örneğin 72 MHz yapabilirsin

### PLL Giriş Kaynakları (PLLSRC)
Kaynak  | Açıklama                          |
--------|-----------------------------------|
HSI/2   | Dahili 8 MHz'in yarısı -> 4 MHz   |
HSE     | Harici kristal (4-16 MHz)         |

## LSE Clock (Low-Speed External)
- STM32'nin RTC(Gerçek zamanlı saati) gibi düşük hızda, hassas zamanlayıcılar için kullandığı harici bir saat kaynağıdır.
- Genelde 32.768 kHz kristal osilatör bağlanarak kullanılır.

### LSE Ne İçin Kullanılır

Kullanım Alanı      | Açıklama                                      |
--------------------|-----------------------------------------------|
RTC                 | Gerçek zaman saati (saniye, dakika, tarih)    |
Backup Domain       | Güç kesilse bile zamanı korur.                |
Standby/Low Power   | Uyanma zamanlayıcısı olarak.                  |

### LSE Saat Kaynağının Özellikleri

Özellik         |   Değer                           |
----------------|-----------------------------------|
Frekans         | 32.768 kHz                        |
Giriş tipi      | Kristal                           |
Voltaj bölgesi  | Backup domain (bataryalı bölge)   |
Kararlılık      | Yüksek (Zaman tutmak için ideal)  |

## LSI Clock (Low-Speed Internal)
- STM32'nin içinde bulunan dahili düşük frekanslı saat kaynağıdır.
- Harici kristal içermez.
- Frekans yaklaşık 40 kHz (+/- 10 kHz)

**NOT!:**
Kararlılığı düşüktür ama basit zamanlayıcı işleri için yeterlidir.

### LSI Ne İçin Kullanılır

Kullanım Alanı              |   Açıklama                        |              
----------------------------|-----------------------------------|
RTC                         | Hassasiyet önemli değilse         |
IWDG(Independent Watchdog)  | LSI kullanılır.                   |
Low-power modları           | Uyandırıcı zamanlayıcı olarak.    |

**NOT!:**
LSI kullanımı çok az enerji tüketir.

### LSI Özellikleri

Özellik         | Değer                     |
----------------|---------------------------|
Frekans         | ~40 kHz                   |
Kaynak          | Dahili RC osilatör        |
Donanım Gerek?  | Hayır                     |
Kararlılık      | Düşük                     |
Tipik Kullanım  | Watchdog, basit zamanlama |


# Ana Sistem Clock'ları (System Core için kullanılır)

Kaynak  | Açıklama                  | Kullanıldığı yer                      |
--------|---------------------------|---------------------------------------|
HSI     | Dahili 8 MHz              | Sistem açılışında, temel işlerde      |
HSE     | Harici 4-16 MHz kristal   | Kararlı System Clock, USB, UART       |
PLL     | Clock çoğaltıcı (x2-x16)  | HSI veya HSE'den alır -> Hızlandırır. |

## Amaçlar
- Mikrodenetleyicinin "ana saatini" belirlemek (SYSCLK).
- Kodun çalıştığı hız, çevre birimlerinin saatleri buraya bağlı.

# Zaman Tabanlı Clock'lar (RTC ve watchdog gibi özel işler için)

Kaynak  |   Açıklama                    | Kullanıldığı yer          |
--------|-------------------------------|---------------------------|
LSI     | Dahili ~40 kHz RC osilatör    | Watchdog, RTC(basit)      |
LSR     | Harici 32.768 kHz kristal     | Gerçek zaman saati (RTC)  |

## Amaçlar
- Zaman tutmak (RTC), sistem kapalıyken bile saat çalışsın.
- Düşük güçte çalışabilecek, sabit saat kaynakları sağlar.

# Ana Karşılaştırma - Güven / Hız / Amaç Açısından

Clock   | Frekans       | Nereden Gelir     | Kullanım alanı        | Hız       | Kararlılık    | Harici Devre Gerekli? |
--------|---------------|-------------------|-----------------------|-----------|---------------------------------------|
HSI     | 8 MHZ         | Dahili            | SYSCLK, temel işler   | Orta      | Orta          | Hayır                 |
HSE     | 8 MHz         | Harici kristal    | SYSCLK, USB, UART     | Orta      | Yüksek        | Evet                  |
PLL     | x2-x16        | HSI/2 veya HSE    | SYSVLK hız artırma    | Yüksek    | Aynı kaynakla | Hayır                 |
LSI     | ~40 kHz       | Dahili            | RTC, watchdog         | Düşük     | Düşük         | Hayır                 |
LSE     | 32.768 kHz    | Harici kristal    | RTC, zaman takibi     | Düşük     | Yüksek        | Evet                  |


# ÖZET

- **HSI:** Dahili 8 MHz. Sistemi boşlatmak için yeterli. Harici devre gerekmez.
- **HSE:** Harici kristal (4-16 MHz). Daha kararlı clock sağlar. USB, UART için önerilir.
- **PLL:** Saat çoğaltıcıdır. HSE veya HSE/2'yi x2-x16 çarpar. 72 MHz' e kadar çıkabilir.
- **LSE:** Harici 32.768 kHz kristal. Hassas zaman için (RTC) en uygundur.
- **LSI:** Dahili ~40 kHz. Watchdog ve düşük hassasiyetli RTC için kullanılır.


- RTC istiyorsa             -> LSE veya LSI
- Watchdog istiyorsan       -> LSI
- Hızlı sistem istiyorsan   -> HSE + PLL
- Basit sistem istiyorsan   -> HSI