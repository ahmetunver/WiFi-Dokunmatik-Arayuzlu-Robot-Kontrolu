<div align="center">

# 🤖 WiFi Dokunmatik Arayüzlü Robot Kontrolü

**TR** | [EN ↓](#-wifi-touch-interface-robot-control--english)

[![Arduino](https://img.shields.io/badge/Arduino-C%2B%2B-00979D?style=flat-square&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![ESP8266](https://img.shields.io/badge/ESP8266-WiFi-FF6900?style=flat-square&logo=espressif&logoColor=white)](https://www.espressif.com/)
[![HTML](https://img.shields.io/badge/Arayüz-HTML%2FCSs-E34F26?style=flat-square&logo=html5&logoColor=white)](https://developer.mozilla.org/en-US/docs/Web/HTML)
[![Python](https://img.shields.io/badge/Python-Yardımcı-3776AB?style=flat-square&logo=python&logoColor=white)](https://www.python.org/)
[![License](https://img.shields.io/badge/Lisans-MIT-green?style=flat-square)](LICENSE)

*WiFi modülü aracılığıyla herhangi bir tarayıcıdan dokunmatik olarak kontrol edilebilen robot sistemi.*

</div>

---

## 🇹🇷 Türkçe

### 📌 Proje Hakkında

Bu proje, **ESP8266 WiFi modülü** üzerinde çalışan bir web sunucusu aracılığıyla robotu **tarayıcı tabanlı dokunmatik bir arayüzle** uzaktan kontrol etmeyi sağlar. Robot, aynı WiFi ağına bağlı herhangi bir akıllı telefon, tablet veya bilgisayarın tarayıcısından yönetilebilir — ek bir uygulama kurulumuna gerek yoktur.

Sistem üç katmandan oluşur:

```
📱 Dokunmatik Web Arayüzü (HTML)
        ↕  HTTP istekleri (WiFi)
🔧 ESP8266 Web Sunucusu (C++ / Arduino)
        ↕  GPIO sinyal kontrolü
⚙️  Motor Sürücü + Robot Motoru
```

### ✨ Özellikler

- 📶 **Kablosuz Kontrol** — ESP8266 üzerinde kurulu web sunucusu sayesinde tam WiFi kontrolü
- 🖱️ **Dokunmatik Arayüz** — Telefon / tablet ekranından parmakla kontrol
- 🌐 **Tarayıcı Tabanlı** — Ek uygulama gerektirmez; herhangi bir tarayıcı yeterli
- ➡️ **Yön Kontrolü** — İleri, Geri, Sola Dön, Sağa Dön, Dur komutları
- ⚡ **Düşük Gecikme** — HTTP tabanlı anlık komut iletimi
- 🔋 **Düşük Güç Tüketimi** — ESP8266'nın verimli WiFi mimarisi
- 🛠️ **Kolay Kurulum** — Arduino IDE ile programlanabilir, ek donanım gerektirmez

### 🔧 Donanım Bileşenleri

| Bileşen | Açıklama |
|---|---|
| **ESP8266 (NodeMCU / ESP-01)** | WiFi modülü + web sunucusu + mikrodenetleyici |
| **DC Motor (×2)** | Robot tekerleklerini süren motorlar |
| **Motor Sürücü (L298N / L293D)** | Motorların yön ve hız kontrolü |
| **Şasi & Tekerlekler** | Robotun mekanik gövdesi |
| **Güç Kaynağı** | Li-Po pil veya AA pil paketi |
| **Bağlantı Kabloları** | Jumper kablolar |

### 💻 Yazılım Bileşenleri

| Bileşen | Dil | Açıklama |
|---|---|---|
| `Arduino Kodu` | C++ | ESP8266 üzerinde çalışan web sunucusu + motor kontrol mantığı |
| `Web Arayüzü` | HTML / CSS / JS | Tarayıcıda açılan dokunmatik kontrol paneli |
| `Yardımcı Script` | Python | Seri port iletişimi / test / debug aracı |

### 📁 Proje Yapısı

```
WiFi-Dokunmatik-Arayuzlu-Robot-Kontrolu/
└── WiFi-Dokunmatik-Arayuzlu-Robot-Kontrolu/
    ├── *.ino / *.cpp       # Arduino / ESP8266 ana kodu (C++)
    │                         Motor kontrolü + HTTP web sunucusu
    ├── *.html              # Dokunmatik web kontrol arayüzü
    │                         İleri / Geri / Sol / Sağ / Dur butonları
    └── *.py                # Yardımcı Python script(leri)
                              Seri port iletişimi / test araçları
```

### 🌐 Çalışma Mantığı

```
1. ESP8266 WiFi ağına bağlanır ve bir IP adresi alır
2. ESP8266 üzerinde HTTP web sunucusu başlatılır
3. Kullanıcı tarayıcıdan ESP8266'nın IP adresini açar
4. Dokunmatik butonlara basıldığında HTTP isteği gönderilir
   Örnek: http://192.168.x.x/ileri
5. ESP8266 isteği alır → Motor sürücüye GPIO sinyali gönderir
6. Robot hareket eder 🚗
```

### 🚀 Kurulum & Yükleme

#### 1. Arduino IDE Hazırlığı

```
Arduino IDE → Dosya → Tercihler →
Ek Kart URL'si ekle:
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```

```
Araçlar → Kart → Kart Yöneticisi → "esp8266" ara → Yükle
```

#### 2. Gerekli Kütüphaneler

```
Araçlar → Kütüphane Yöneticisi:
✅ ESP8266WiFi      (ESP8266 çekirdek paketinde mevcut)
✅ ESP8266WebServer (ESP8266 çekirdek paketinde mevcut)
```

#### 3. WiFi Bilgilerini Girin

`*.ino` dosyasında şu satırları kendi ağ bilgilerinizle güncelleyin:

```cpp
const char* ssid     = "WIFI_ADINIZ";
const char* password = "WIFI_SIFRENIZ";
```

#### 4. Kodu Yükleyin

```
Araçlar → Kart → NodeMCU 1.0 (veya kullandığınız ESP8266 modeli)
Araçlar → Port → (ESP8266'nın bağlı olduğu COM portu)
→ Yükle (Upload)
```

#### 5. IP Adresini Bulun

```
Araçlar → Seri Monitör → 115200 baud
Serial çıktısında ESP8266'nın aldığı IP adresini görürsünüz:
"IP adresi: 192.168.x.x"
```

#### 6. Robotu Kontrol Edin

Telefon veya bilgisayarınızın tarayıcısını açın ve şu adresi girin:

```
http://192.168.x.x
```

Dokunmatik kontrol paneli açılacaktır. 

### 🔌 Bağlantı Şeması

```
ESP8266 (NodeMCU)         L298N Motor Sürücü
─────────────────         ──────────────────
D1 (GPIO5)   ──────────►  IN1  (Motor A İleri)
D2 (GPIO4)   ──────────►  IN2  (Motor A Geri)
D3 (GPIO0)   ──────────►  IN3  (Motor B İleri)
D4 (GPIO2)   ──────────►  IN4  (Motor B Geri)
GND          ──────────►  GND
3.3V / 5V    ──────────►  VCC (Lojik)
```

> ⚠️ **Not:** Pin numaraları kullandığınız ESP8266 modeline göre değişebilir. Kodun içindeki `#define` veya `const int` satırlarından pin atamanızı doğrulayın.

### 🛠️ Gereksinimler

- Arduino IDE 1.8+ veya Arduino IDE 2.x
- ESP8266 Arduino Çekirdek Paketi
- Python 3.x (yardımcı script için, isteğe bağlı)
- Ortak bir WiFi ağı (robot ve kontrol cihazı aynı ağda olmalı)

---

## 🇬🇧 WiFi Touch Interface Robot Control — English

<div align="center">

*A browser-based touchscreen robot controller running on an ESP8266 WiFi module.*

</div>

### 📌 About the Project

This project enables **remote control of a robot** through a **browser-based touchscreen interface** via a web server running on an **ESP8266 WiFi module**. The robot can be operated from any smartphone, tablet or computer browser connected to the same WiFi network — no additional app installation required.

The system has three layers:

```
📱 Touch Web Interface (HTML)
        ↕  HTTP requests (WiFi)
🔧 ESP8266 Web Server (C++ / Arduino)
        ↕  GPIO signal control
⚙️  Motor Driver + Robot Motors
```

### ✨ Features

- 📶 **Wireless Control** — Full WiFi control via a web server hosted on the ESP8266
- 🖱️ **Touch Interface** — Control with your finger from phone or tablet screen
- 🌐 **Browser-Based** — No extra app needed; any browser works
- ➡️ **Directional Control** — Forward, Backward, Left, Right, Stop commands
- ⚡ **Low Latency** — Instant HTTP-based command delivery
- 🔋 **Low Power** — Leverages ESP8266's efficient WiFi architecture
- 🛠️ **Easy Setup** — Programmable via Arduino IDE, no extra hardware required

### 🔧 Hardware Components

| Component | Description |
|---|---|
| **ESP8266 (NodeMCU / ESP-01)** | WiFi module + web server + microcontroller |
| **DC Motor (×2)** | Motors driving the robot wheels |
| **Motor Driver (L298N / L293D)** | Direction and speed control for motors |
| **Chassis & Wheels** | Robot's mechanical body |
| **Power Supply** | Li-Po battery or AA battery pack |
| **Jumper Wires** | Connection cables |

### 💻 Software Components

| Component | Language | Description |
|---|---|---|
| `Arduino Code` | C++ | Web server + motor control logic running on ESP8266 |
| `Web Interface` | HTML / CSS / JS | Touchscreen control panel opened in the browser |
| `Helper Script` | Python | Serial communication / testing / debugging tool |

### 📁 Project Structure

```
WiFi-Dokunmatik-Arayuzlu-Robot-Kontrolu/
└── WiFi-Dokunmatik-Arayuzlu-Robot-Kontrolu/
    ├── *.ino / *.cpp       # Arduino / ESP8266 main code (C++)
    │                         Motor control + HTTP web server
    ├── *.html              # Touchscreen web control interface
    │                         Forward / Backward / Left / Right / Stop buttons
    └── *.py                # Helper Python script(s)
                              Serial communication / testing tools
```

### 🌐 How It Works

```
1. ESP8266 connects to the WiFi network and obtains an IP address
2. An HTTP web server starts on the ESP8266
3. User opens the ESP8266's IP address in a browser
4. Tapping a button sends an HTTP request
   Example: http://192.168.x.x/forward
5. ESP8266 receives the request → sends GPIO signal to motor driver
6. Robot moves 🚗
```

### 🚀 Setup & Upload

#### 1. Prepare Arduino IDE

```
Arduino IDE → File → Preferences →
Add board URL:
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```

```
Tools → Board → Board Manager → search "esp8266" → Install
```

#### 2. Required Libraries

```
Tools → Library Manager:
✅ ESP8266WiFi      (included in ESP8266 core package)
✅ ESP8266WebServer (included in ESP8266 core package)
```

#### 3. Enter Your WiFi Credentials

Update these lines in the `*.ino` file:

```cpp
const char* ssid     = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
```

#### 4. Upload the Code

```
Tools → Board → NodeMCU 1.0 (or your ESP8266 model)
Tools → Port → (the COM port your ESP8266 is connected to)
→ Upload
```

#### 5. Find the IP Address

```
Tools → Serial Monitor → 115200 baud
The serial output will show the ESP8266's assigned IP:
"IP address: 192.168.x.x"
```

#### 6. Control the Robot

Open a browser on your phone or computer and go to:

```
http://192.168.x.x
```

The touchscreen control panel will load. 

### 🔌 Wiring Diagram

```
ESP8266 (NodeMCU)         L298N Motor Driver
─────────────────         ──────────────────
D1 (GPIO5)   ──────────►  IN1  (Motor A Forward)
D2 (GPIO4)   ──────────►  IN2  (Motor A Backward)
D3 (GPIO0)   ──────────►  IN3  (Motor B Forward)
D4 (GPIO2)   ──────────►  IN4  (Motor B Backward)
GND          ──────────►  GND
3.3V / 5V    ──────────►  VCC (Logic)
```

> ⚠️ **Note:** Pin numbers may vary depending on your ESP8266 model. Verify pin assignments from the `#define` or `const int` lines in the code.

### 🛠️ Requirements

- Arduino IDE 1.8+ or Arduino IDE 2.x
- ESP8266 Arduino Core Package
- Python 3.x (for the helper script, optional)
- A shared WiFi network (robot and control device must be on the same network)

---

<div align="center">

© 2025 Ahmet Ünver

Made with  WiFi +  ESP8266 +  Arduino


</div>
