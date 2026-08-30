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

```

### Nesne Yönelimli Tasarım

Temel domain sınıfları:

```text

Person

Student

UndergraduateStudent

GraduateStudent

Instructor

Course

UndergraduateCourse

GraduateCourse

Enrollment

Exam

ExamScore

```

Inheritance yalnızca gerçek IS-A ilişkilerinde kullanılmaktadır.

Örnek:

```text

UndergraduateStudent IS-A Student

GraduateStudent IS-A Student

UndergraduateCourse IS-A Course

GraduateCourse IS-A Course

```

Değişebilir davranışlar composition ile ayrıştırılmıştır.

```text

Student HAS-A CreditPolicy

Student HAS-A LetterGradePolicy

Course HAS-A PassingPolicy

Course HAS-A CourseGradingPolicy

CourseGradingPolicy HAS-A GradeCalculationStrategy

```

### Policy Tasarımı

Değişme ihtimali bulunan akademik kurallar policy sınıflarında tutulmaktadır.

#### CreditPolicy

```text

CreditPolicy

    |

    +-- UndergraduateCreditPolicy

    +-- GraduateCreditPolicy

    +-- UnlimitedCreditPolicy

```

Lisans öğrencileri:

```text

GPA > 3.0          -> 25 kredi

2.0 <= GPA <= 3.0 -> 20 kredi

GPA < 2.0          -> 15 kredi

```

Yüksek lisans öğrencileri:

```text

GPA > 3.5          -> 15 kredi

3.0 <= GPA <= 3.5 -> 10 kredi

GPA < 3.0          -> 6 kredi

```

#### LetterGradePolicy

```text

LetterGradePolicy

    |

    +-- UndergraduateLetterGradePolicy

    +-- GraduateLetterGradePolicy

```

Sayısal notun harf notuna dönüştürülmesi öğrenci tipine göre ayrı policy sınıfları tarafından gerçekleştirilir.

#### PassingPolicy

Mevcut geçme kuralları:

```text

Undergraduate Course -> minimum DD

Graduate Course      -> minimum CC

```

### Strategy Pattern

Dönem sonu başarı notunun hesaplanmasında Strategy Pattern kullanılmaktadır.

```text

GradeCalculationStrategy

        |

        +-- WeightedAverageStrategy

        +-- ThresholdStrategy

```

Aynı ders içerisinde farklı öğrenci tipleri için farklı grading strategy tanımlanabilir.

Bu sayede yeni bir notlandırma yöntemi eklenirken mevcut domain ve controller sınıflarının temel davranışlarının değiştirilmesi gerekmez.

### Bellek Yönetimi

Ownership bulunan nesnelerde `std::unique_ptr` kullanılmaktadır.

Örnekler:

```text

ApplicationState

    -> unique_ptr<Student>

    -> unique_ptr<Course>

    -> unique_ptr<Instructor>

    -> unique_ptr<Enrollment>

Student

    -> unique_ptr<CreditPolicy>

    -> unique_ptr<LetterGradePolicy>

Course

    -> unique_ptr<Exam>

    -> unique_ptr<CourseGradingPolicy>

CourseGradingPolicy

    -> unique_ptr<GradeCalculationStrategy>

```

Ownership bulunmayan ilişkilerde non-owning pointer kullanılmaktadır.

Örnek:

```text

Enrollment -> Student*

Enrollment -> Course*

Instructor -> Course*

ExamScore  -> Exam*

```

### Güvenlik ve Doğrulama

Projede aşağıdaki kontroller uygulanmaktadır:

- Geçersiz GPA değerlerinin engellenmesi

- Geçersiz sınav notlarının engellenmesi

- Geçersiz final score değerlerinin engellenmesi

- Pozitif olmayan ID kontrolleri

- Boş isim ve ders kodu kontrolleri

- Null pointer kontrolleri

- Duplicate enrollment kontrolü

- Kredi limiti kontrolü

- Geçersiz grading configuration kontrolleri

- Eksik sınav notlarıyla final hesaplanmasının engellenmesi

- Score değiştirildiğinde eski final sonucunun geçersiz hale getirilmesi

Const correctness, smart pointer kullanımı ve ownership ayrımı da tasarım boyunca dikkate alınmıştır.

---

## Kullanılan Teknolojiler

### Backend

- C++17

- CMake

- cpp-httplib

- nlohmann/json

- GoogleTest

### Frontend

- React

- TypeScript

- Vite

- React Router

### Masaüstü

- Electron

### Geliştirme Ortamı

- Windows

- Visual Studio 2026

- CMake

- Ninja

- Node.js

- npm

---

## Kurulum ve Çalıştırma

### Gereksinimler

Mevcut masaüstü yapılandırması için:

- Windows 10/11

- Visual Studio 2026

- Desktop Development with C++ workload

- CMake 3.16 veya üzeri

- Ninja

- Node.js

- npm

- İnternet bağlantısı

CMake ilk configure işleminde aşağıdaki bağımlılıkları otomatik olarak indirir:

- nlohmann/json

- cpp-httplib

- GoogleTest

### 1. Projeyi Alın

Git kullanılıyorsa:

```bash

git clone https://github.com/aylgny/UniversityAutomation.git

cd UniversityAutomation

```

ZIP üzerinden teslim alındıysa proje klasörünün çıkarılması yeterlidir.

### 2. Backend'i Derleyin

Electron mevcut yapılandırmada backend executable'ını aşağıdaki konumda beklemektedir:

```text

out/build/x64-debug/UniversityAutomation.exe

```

Bu nedenle `x64-debug` preset'i kullanılmalıdır.

```bash

cmake --preset x64-debug

```

Ardından:

```bash

cmake --build out/build/x64-debug

```

### 3. Frontend Bağımlılıklarını Kurun

```bash

cd frontend

npm install

```

### 4. Frontend Build Alın

```bash

npm run build

```

Bu işlem:

```text

frontend/dist/

```

klasörünü oluşturur.

Ardından proje ana dizinine dönün:

```bash

cd ..

```

### 5. Electron Bağımlılıklarını Kurun

Proje ana dizininde:

```bash

npm install

```

### 6. Uygulamayı Başlatın

```bash

npm run electron

```

Electron:

1\. C++ backend'i başlatır.

2\. React build'ini masaüstü penceresinde açar.

3\. Backend ile REST API üzerinden iletişim kurar.

4\. Uygulama kapatıldığında backend process'ini sonlandırır.

Backend varsayılan olarak:

```text

localhost:8080

```

üzerinde çalışmaktadır.

---

## Demo Kullanıcıları

Authentication mekanizması projenin ana kapsamı olmadığı için demo amacıyla basitleştirilmiştir.

### Lisans Öğrencisi

```text

Username: undergrad

Password: 1234

```

### Yüksek Lisans Öğrencisi

```text

Username: graduate

Password: 1234

```

### Öğretim Üyesi

```text

Username: instructor

Password: 1234

```

Production seviyesinde bir sistemde password hashing, session/token yönetimi ve detaylı authorization mekanizmalarının eklenmesi gerekir.

---

## Testler

GoogleTest kullanılarak otomatik testler oluşturulmuştur.

Mevcut test paketi:

```text
111 tests from 19 test suites
111 passed
```

Test kapsamı içerisinde:

- Lisans kredi politikaları

- Yüksek lisans kredi politikaları

- GPA sınır değerleri

- Unlimited credit policy

- Harf notu politikaları

- Harf notu sınır değerleri

- Weighted Average Strategy

- Threshold Strategy

- Ders kayıt işlemleri

- Duplicate enrollment kontrolü

- Kredi limiti ihlalleri

- GradingController akışları

- Person validation

- Student validation

- Course validation

- Exam validation

- ExamScore validation

- Enrollment validation

- PassingPolicy

- CourseGradingPolicy

- Farklı StudentType'lar için farklı grading policy kullanımı

- Eksik sınav notlarıyla final hesaplanmasının engellenmesi

- Score değiştiğinde eski final sonucunun temizlenmesi

- Grading policy referanslarının güvenli şekilde korunması

Testler:

```bash

ctest --test-dir out/build/x64-debug --output-on-failure

```

komutu ile çalıştırılabilir.

Alternatif olarak:

```bash

out/build/x64-debug/UniversityTests.exe

```

doğrudan çalıştırılabilir.

---

## Proje Yapısı

```text

UniversityAutomation/

|

|-- backend/

|   |

|   |-- include/

|   |   |-- api/

|   |   |-- application/

|   |   |-- domain/

|   |   |-- policies/

|   |   `-- strategies/

|   |

|   |-- src/

|   |   |-- api/

|   |   |-- application/

|   |   |-- domain/

|   |   |-- policies/

|   |   |-- strategies/

|   |   `-- main.cpp

|   |

|   `-- tests/

|

|-- frontend/

|   |-- src/

|   `-- package.json

|

|-- electron/

|   `-- main.js

|

|-- CMakeLists.txt

|-- CMakePresets.json

|-- package.json

`-- README.md

```

---



## Veri Sıfırlama

Uygulama verileri `data.json` dosyasında saklanmaktadır.

Electron üzerinden çalıştırıldığında dosya varsayılan olarak:

```text

out/build/x64-debug/data.json

```

konumunda oluşur.

Demo verilerini başlangıç durumuna döndürmek için:

1\. Uygulamayı kapatın.

2\. `data.json` dosyasını silin.

3\. Uygulamayı tekrar başlatın.

Uygulama dosyayı bulamadığında seed data yeniden oluşturulur.

---

## Notlar

Bu proje bir yazılım tasarım vaka çalışması kapsamında geliştirilmiştir.

Öğrenci, ders ve öğretim üyesi oluşturma/yönetme ekranları temel kapsam içerisinde değildir. İlgili nesneler başlangıç verileri üzerinden oluşturulmaktadır.

Authentication altyapısı da demonstrasyon amacıyla basitleştirilmiştir.

Projenin temel odağı:

- Nesne yönelimli tasarım

- Modülerlik

- Genişletilebilirlik

- Değişebilir iş kurallarının ayrıştırılması

- Test edilebilirlik

- Bellek yönetimi

- Const correctness

- Temiz ownership yönetimi

konularıdır.
