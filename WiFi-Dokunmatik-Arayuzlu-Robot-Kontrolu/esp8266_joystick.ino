#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "ESP-JOYSTICK";
const char* password = "12345678";

ESP8266WebServer server(80);

const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html lang='tr'>
<head>
  <meta charset='UTF-8'>
  <title>Joystick Kontrol</title>
  <meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no'>
  <style>
    body {
      background-color: #1e1e2e;
      margin: 0;
      padding: 0;
      overflow: hidden;
      font-family: Arial, sans-serif;
      color: white;
      touch-action: none; /* Kritik: Tarayıcının varsayılan dokunmatik eylemlerini engeller */
    }

    .container {
      position: absolute;
      width: 100%;
      height: 100%;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      gap: 20px;
    }

    #status {
      position: absolute;
      top: 20px;
      left: 50%;
      transform: translateX(-50%);
      background-color: rgba(76, 175, 80, 0.8);
      padding: 10px 20px;
      border-radius: 20px;
      font-size: 18px;
      z-index: 100;
      opacity: 1;
      transition: opacity 0.3s;
    }

    #joystick-container {
      position: relative;
      width: 240px;
      height: 240px;
      background-color: rgba(255, 255, 255, 0.1);
      border: 3px solid #64b5f6;
      border-radius: 50%;
      box-shadow: 0 0 15px rgba(100, 181, 246, 0.5), 
                  inset 0 0 30px rgba(100, 181, 246, 0.3);
    }

    #joystick-base {
      position: absolute;
      width: 80px;
      height: 80px;
      background-color: rgba(255, 255, 255, 0.2);
      border-radius: 50%;
      top: 50%;
      left: 50%;
      transform: translate(-50%, -50%);
      border: 2px solid rgba(255, 255, 255, 0.5);
    }

    #car {
      position: absolute;
      width: 50px;
      height: 80px;
      top: 50%;
      left: 50%;
      transform: translate(-50%, -50%) rotate(0deg);
      transform-origin: center center;
      transition: transform 0.05s ease-out;
    }

    #car svg {
      width: 100%;
      height: 100%;
    }

    #debug-info {
      position: absolute;
      bottom: 20px;
      left: 50%;
      transform: translateX(-50%);
      background-color: rgba(0, 0, 0, 0.7);
      padding: 10px 20px;
      border-radius: 10px;
      font-size: 14px;
      z-index: 100;
      text-align: center;
      width: 80%;
    }

    .direction-label {
      position: absolute;
      color: rgba(255, 255, 255, 0.7);
      font-weight: bold;
    }

    #label-up { top: -25px; left: 50%; transform: translateX(-50%); }
    #label-down { bottom: -25px; left: 50%; transform: translateX(-50%); }
    #label-left { left: -25px; top: 50%; transform: translateY(-50%); }
    #label-right { right: -25px; top: 50%; transform: translateY(-50%); }
    
    /* Hız kontrolü stilleri */
    .speed-control {
      display: flex;
      flex-direction: column;
      align-items: center;
      width: 80%;
      max-width: 300px;
      background-color: rgba(255, 255, 255, 0.1);
      border-radius: 10px;
      padding: 15px;
      margin-top: 20px;
    }

    .speed-control h3 {
      margin-top: 0;
      margin-bottom: 10px;
      color: #90caf9;
      font-size: 18px;
    }

    .slider-container {
      width: 100%;
      display: flex;
      align-items: center;
      gap: 15px;
    }

    .slider-container input {
      flex-grow: 1;
    }

    input[type="range"] {
      -webkit-appearance: none;
      width: 100%;
      height: 8px;
      background: #455a64;
      border-radius: 4px;
      outline: none;
    }

    input[type="range"]::-webkit-slider-thumb {
      -webkit-appearance: none;
      width: 20px;
      height: 20px;
      background: #64b5f6;
      border-radius: 50%;
      cursor: pointer;
      transition: background 0.2s;
    }

    input[type="range"]::-webkit-slider-thumb:hover {
      background: #90caf9;
    }

    .speed-value {
      font-size: 16px;
      font-weight: bold;
      min-width: 40px;
      text-align: center;
    }

    .speed-presets {
      display: flex;
      justify-content: space-between;
      width: 100%;
      margin-top: 10px;
    }

    .preset-btn {
      background-color: #455a64;
      color: white;
      border: none;
      border-radius: 4px;
      padding: 5px 10px;
      font-size: 14px;
      cursor: pointer;
      transition: all 0.2s;
    }

    .preset-btn:hover {
      background-color: #64b5f6;
    }

    .preset-btn.active {
      background-color: #64b5f6;
      box-shadow: 0 0 5px rgba(100, 181, 246, 0.8);
    }
  </style>
</head>
<body>
  <div class="container">
    <div id="status">Bağlandı!</div>
    <div id="joystick-container">
      <div class="direction-label" id="label-up">İLERİ</div>
      <div class="direction-label" id="label-down">GERİ</div>
      <div class="direction-label" id="label-left">SOL</div>
      <div class="direction-label" id="label-right">SAĞ</div>
      <div id="joystick-base"></div>
      <div id="car">
        <svg viewBox="0 0 100 160" xmlns="http://www.w3.org/2000/svg">
          <!-- Araba gövdesi -->
          <rect x="20" y="30" width="60" height="100" rx="15" fill="#f44336" />
          <!-- Ön cam -->
          <path d="M30 40 L70 40 L65 20 L35 20 Z" fill="#90caf9" />
          <!-- Farlar -->
          <rect x="25" y="125" width="15" height="5" rx="2" fill="#ffeb3b" />
          <rect x="60" y="125" width="15" height="5" rx="2" fill="#ffeb3b" />
          <!-- Tekerler -->
          <rect x="10" y="60" width="10" height="25" rx="3" fill="#212121" />
          <rect x="80" y="60" width="10" height="25" rx="3" fill="#212121" />
          <rect x="10" y="100" width="10" height="25" rx="3" fill="#212121" />
          <rect x="80" y="100" width="10" height="25" rx="3" fill="#212121" />
        </svg>
      </div>
    </div>
    
    <div class="speed-control">
      <h3>Motor Hızı</h3>
      <div class="slider-container">
        <input type="range" id="speed-slider" min="100" max="250" value="150" step="5">
        <span id="speed-value" class="speed-value">150</span>
      </div>
      <div class="speed-presets">
        <button class="preset-btn active" data-speed="150">Otomatik</button>
        <button class="preset-btn" data-speed="100">Yavaş</button>
        <button class="preset-btn" data-speed="175">Orta</button>
        <button class="preset-btn" data-speed="250">Hızlı</button>
      </div>
    </div>
    
    <div id="debug-info">Dokunmatik: Beklemede</div>
  </div>

  <script>
    // Temel değişkenleri tanımla
    const car = document.getElementById('car');
    const joystickContainer = document.getElementById('joystick-container');
    const statusElement = document.getElementById('status');
    const debugInfo = document.getElementById('debug-info');
    const speedSlider = document.getElementById('speed-slider');
    const speedValue = document.getElementById('speed-value');
    const presetButtons = document.querySelectorAll('.preset-btn');
    
    // Tarayıcıda varsayılan davranışları engelleyelim
    document.addEventListener('touchmove', function(e) {
      e.preventDefault();
    }, { passive: false });
    
    // Joystick konteynerini ve merkez noktasını hesapla
    let joystickRect = joystickContainer.getBoundingClientRect();
    let containerCenter = {
      x: joystickRect.width / 2,
      y: joystickRect.height / 2
    };
    
    let radius = joystickRect.width / 2;
    let currentDirection = null;
    let isDragging = false;
    let currentSpeed = 150; // Varsayılan hız

    // Sayfa yüklendikten sonra başlangıç pozisyonunu ayarla
    window.addEventListener('load', () => {
      updateContainerPosition();
      statusElement.textContent = "Hazır! Joystick'i kullanın";
    });

    // Pencere boyutu değiştiğinde merkezleri güncelle
    window.addEventListener('resize', updateContainerPosition);

    function updateContainerPosition() {
      joystickRect = joystickContainer.getBoundingClientRect();
      containerCenter = {
        x: joystickRect.width / 2,
        y: joystickRect.height / 2
      };
      
      // Arabayı merkeze yerleştir
      car.style.left = `${containerCenter.x}px`;
      car.style.top = `${containerCenter.y}px`;
      radius = joystickRect.width / 2;
      
      debugInfo.textContent = `Merkez: ${Math.round(containerCenter.x)}, ${Math.round(containerCenter.y)}`;
    }

    function updateCar(eventX, eventY) {
      // Joystick konteyneri içindeki göreceli konumu hesapla
      const rect = joystickContainer.getBoundingClientRect();
      const x = eventX - rect.left;
      const y = eventY - rect.top;
      
      // Merkeze göre farkı hesapla
      const dx = x - containerCenter.x;
      const dy = y - containerCenter.y;
      
      // Uzaklığı hesapla
      const distance = Math.sqrt(dx * dx + dy * dy);
      
      // Maksimum mesafe kontrolü
      const maxDistance = radius - 25;
      const actualDistance = Math.min(distance, maxDistance);
      
      // Açıyı hesapla
      const angleRad = Math.atan2(dy, dx);
      const angleDeg = angleRad * 180 / Math.PI;
      
      // Joystick sınırları içinde kalmak için pozisyonu sınırla
      const newX = containerCenter.x + actualDistance * Math.cos(angleRad);
      const newY = containerCenter.y + actualDistance * Math.sin(angleRad);
      
      // Arabayı güncelle
      car.style.left = `${newX}px`;
      car.style.top = `${newY}px`;
      car.style.transform = `translate(-50%, -50%) rotate(${angleDeg + 90}deg)`;
      
      // Debug bilgisini güncelle
      debugInfo.textContent = `dx: ${Math.round(dx)}, dy: ${Math.round(dy)}, mesafe: ${Math.round(distance)}, hız: ${currentSpeed}`;
      
      // Yönü belirle ve komutu gönder
      const dir = getDirection(dx, dy);
      if (dir && dir !== currentDirection) {
        currentDirection = dir;
        sendCommand(dir);
        statusElement.textContent = `Komut: ${dir}, Hız: ${currentSpeed}`;
      }
      
      return { dx, dy, dir };
    }

    function getDirection(dx, dy) {
      // Eğer joystick merkeze çok yakınsa dur komutu gönder
      const distance = Math.sqrt(dx * dx + dy * dy);
      if (distance < 30) {
        return "stop";
      }
      
      // Açıyı hesapla
      const angle = Math.atan2(dy, dx) * 180 / Math.PI;
      
      // Açıya göre yön tayini
      if (angle > -22.5 && angle <= 22.5) {
        return "right";
      } else if (angle > 22.5 && angle <= 67.5) {
        return "down-right";
      } else if (angle > 67.5 && angle <= 112.5) {
        return "down";
      } else if (angle > 112.5 && angle <= 157.5) {
        return "down-left";
      } else if ((angle > 157.5 && angle <= 180) || (angle >= -180 && angle <= -157.5)) {
        return "left";
      } else if (angle > -157.5 && angle <= -112.5) {
        return "up-left";
      } else if (angle > -112.5 && angle <= -67.5) {
        return "up";
      } else if (angle > -67.5 && angle <= -22.5) {
        return "up-right";
      }
      
      return "stop";
    }

    function sendCommand(dir) {
      fetch(`/move?dir=${dir}&speed=${currentSpeed}`)
        .then(res => {
          console.log(`Komut gönderildi: ${dir}, Hız: ${currentSpeed}`);
          statusElement.textContent = `Komut: ${dir}, Hız: ${currentSpeed}`;
        })
        .catch(err => {
          console.error("Hata:", err);
          statusElement.textContent = "Bağlantı hatası!";
          statusElement.style.backgroundColor = "rgba(244, 67, 54, 0.8)";
        });
    }

    // Fare olayları
    joystickContainer.addEventListener('mousedown', (e) => {
      isDragging = true;
      updateCar(e.clientX, e.clientY);
    });

    document.addEventListener('mouseup', () => {
      if (!isDragging) return;
      isDragging = false;
      
      // Bırakıldığında arabayı merkeze geri getir
      car.style.left = `${containerCenter.x}px`;
      car.style.top = `${containerCenter.y}px`;
      car.style.transform = 'translate(-50%, -50%) rotate(0deg)';
      
      // Durma komutu gönder
      sendCommand("stop");
      currentDirection = "stop";
      debugInfo.textContent = "Dokunmatik: Bırakıldı";
    });

    document.addEventListener('mousemove', (e) => {
      if (!isDragging) return;
      updateCar(e.clientX, e.clientY);
    });

    // Dokunmatik olaylar - Yeniden Yazıldı
    joystickContainer.addEventListener('touchstart', (e) => {
      e.preventDefault();
      isDragging = true;
      const touch = e.touches[0];
      updateCar(touch.clientX, touch.clientY);
      debugInfo.textContent = `Dokunmatik başladı: ${Math.round(touch.clientX)}, ${Math.round(touch.clientY)}`;
    }, { passive: false });

    document.addEventListener('touchmove', (e) => {
      if (!isDragging) return;
      e.preventDefault();
      const touch = e.touches[0];
      updateCar(touch.clientX, touch.clientY);
      debugInfo.textContent = `Dokunmatik hareket: ${Math.round(touch.clientX)}, ${Math.round(touch.clientY)}`;
    }, { passive: false });

    document.addEventListener('touchend', (e) => {
      if (!isDragging) return;
      e.preventDefault();
      isDragging = false;
      
      // Bırakıldığında arabayı merkeze geri getir
      car.style.left = `${containerCenter.x}px`;
      car.style.top = `${containerCenter.y}px`;
      car.style.transform = 'translate(-50%, -50%) rotate(0deg)';
      
      // Durma komutu gönder
      sendCommand("stop");
      currentDirection = "stop";
      debugInfo.textContent = "Dokunmatik: Bırakıldı";
    }, { passive: false });

    document.addEventListener('touchcancel', (e) => {
      if (!isDragging) return;
      e.preventDefault();
      isDragging = false;
      
      // İptal durumunda da merkeze getir
      car.style.left = `${containerCenter.x}px`;
      car.style.top = `${containerCenter.y}px`;
      car.style.transform = 'translate(-50%, -50%) rotate(0deg)';
      
      // Durma komutu gönder
      sendCommand("stop");
      currentDirection = "stop";
      debugInfo.textContent = "Dokunmatik: İptal edildi";
    }, { passive: false });
    
    // Hız kontrolü işlemleri
    speedSlider.addEventListener('input', function() {
      currentSpeed = parseInt(this.value);
      speedValue.textContent = currentSpeed;
      
      // Preset butonlarını güncelle
      presetButtons.forEach(btn => {
        if (parseInt(btn.dataset.speed) === currentSpeed) {
          btn.classList.add('active');
        } else {
          btn.classList.remove('active');
        }
      });
      
      // Eğer aktif bir yön varsa, hızı hemen güncelle
      if (currentDirection && currentDirection !== "stop") {
        sendCommand(currentDirection);
      }
    });
    
    // Hız preset butonları
    presetButtons.forEach(btn => {
      btn.addEventListener('click', function() {
        const speed = parseInt(this.dataset.speed);
        currentSpeed = speed;
        speedSlider.value = speed;
        speedValue.textContent = speed;
        
        // Aktif butonu güncelle
        presetButtons.forEach(b => b.classList.remove('active'));
        this.classList.add('active');
        
        // Eğer aktif bir yön varsa, hızı hemen güncelle
        if (currentDirection && currentDirection !== "stop") {
          sendCommand(currentDirection);
        }
      });
    });
  </script>
</body>
</html>)rawliteral";

void handleMove() {
  String dir = server.arg("dir");
  int speed = 150; // Varsayılan hız
  
  // Eğer hız parametresi gönderildiyse, onu al
  if (server.hasArg("speed")) {
    speed = server.arg("speed").toInt();
    
    // Hız sınırlarını kontrol et
    if (speed < 100) speed = 100;
    if (speed > 250) speed = 250;
  }
  
  // Hem yön hem de hız bilgisini Arduino'ya gönder
  if (dir == "up") Serial.println("U:" + String(speed));
  else if (dir == "down") Serial.println("D:" + String(speed));
  else if (dir == "left") Serial.println("L:" + String(speed));
  else if (dir == "right") Serial.println("R:" + String(speed));
  else if (dir == "up-left") Serial.println("UL:" + String(speed));
  else if (dir == "up-right") Serial.println("UR:" + String(speed));
  else if (dir == "down-left") Serial.println("DL:" + String(speed));
  else if (dir == "down-right") Serial.println("DR:" + String(speed));
  else if (dir == "stop") Serial.println("S:0");
  
  server.send(200, "text/plain", "OK");
}

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void setup() {
  Serial.begin(9600);
  
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP adresi: ");
  Serial.println(myIP);
  
  server.on("/", handleRoot);
  server.on("/move", handleMove);
  
  // CORS için gerekli header'ları ekle
  server.onNotFound([]() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(404, "text/plain", "Not Found");
  });
  
  server.begin();
  Serial.println("ESP8266 hazir. Baglan: ESP-JOYSTICK");
}

void loop() {
  server.handleClient();
}