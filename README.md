# University Automation

C++ backend, React + TypeScript frontend ve Electron masaüstü arayüzü kullanılarak geliştirilmiş bir üniversite otomasyon sistemidir.

Proje; nesne yönelimli analiz ve tasarım prensipleri, modülerlik, genişletilebilirlik, yeniden kullanılabilirlik ve sorumlulukların ayrılması göz önünde bulundurularak geliştirilmiştir.

---

## İçindekiler

- [Proje Hakkında](#proje-hakkında)
- [Özellikler](#özellikler)
- [Mimari ve Tasarım](#mimari-ve-tasarım)
- [Kullanılan Teknolojiler](#kullanılan-teknolojiler)
- [Kurulum ve Çalıştırma](#kurulum-ve-çalıştırma)
- [Demo Kullanıcıları](#demo-kullanıcıları)
- [Testler](#testler)
- [Proje Yapısı](#proje-yapısı)
- [Demo Senaryoları](#demo-senaryoları)
- [Notlar](#notlar)

---

## Proje Hakkında

Sistem temel olarak iki kullanıcı rolünü desteklemektedir:

- Öğrenci
- Öğretim Üyesi

Öğrenci tipleri:

- Lisans
- Yüksek Lisans

Ders tipleri:

- Lisans Dersi
- Yüksek Lisans Dersi

Öğrenci tipi, GPA, ders tipi ve seçilen notlandırma yöntemine bağlı akademik kurallar ayrı policy ve strategy sınıfları üzerinden uygulanmaktadır.

---

## Özellikler

### Öğrenci

Öğrenciler:

- Sisteme giriş yapabilir.
- Açık dersleri görüntüleyebilir.
- Kredi limitleri dahilinde derslere kayıt olabilir.
- Kayıtlı oldukları dersleri görüntüleyebilir.
- Sınav notlarını görüntüleyebilir.
- Dönem sonu başarı ve harf notlarını görüntüleyebilir.

### Öğretim Üyesi

Öğretim üyeleri:

- Kendilerine atanmış dersleri görüntüleyebilir.
- Derslere kayıtlı öğrencileri görüntüleyebilir.
- Derslerin sınav sayılarını belirleyebilir.
- Öğrenci tipine göre farklı notlandırma yöntemleri tanımlayabilir.
- Sınav notlarını kaydedebilir.
- Tüm sınav notları tamamlandıktan sonra dönem sonu başarı ve harf notlarını hesaplayabilir.

---

## Mimari ve Tasarım

Proje katmanlı bir yapıda geliştirilmiştir.

```text
React / Electron UI
        |
        v
REST API
        |
        v
Application Layer
        |
        v
Domain Model
        |
        v
Policies / Strategies
