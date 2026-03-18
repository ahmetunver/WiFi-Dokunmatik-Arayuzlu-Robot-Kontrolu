#include <Arduino.h>
#define ENA 5
#define ENB 6
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

String gelenVeri;
int motorSpeed = 150; // Varsayılan hız

void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  dur();
  Serial.println("Arduino hazir.");
}

void loop() {
  if (Serial.available()) {
    gelenVeri = Serial.readStringUntil('\n');
    gelenVeri.trim(); // boşlukları temizle
    
    Serial.print("Gelen: ");
    Serial.println(gelenVeri);
    
    // Yön ve hız bilgisini ayır (Ör: "U:200")
    int ayracIndex = gelenVeri.indexOf(':');
    
    if (ayracIndex > 0) {
      String yon = gelenVeri.substring(0, ayracIndex);
      motorSpeed = gelenVeri.substring(ayracIndex + 1).toInt();
      
      Serial.print("Yon: ");
      Serial.println(yon);
      Serial.print("Hiz: ");
      Serial.println(motorSpeed);
      
      // Hız sınırlarını kontrol et
      if (motorSpeed < 0) motorSpeed = 0;
      if (motorSpeed > 255) motorSpeed = 255;
      
     
      if (yon == "U") ileri();
      else if (yon == "D") geri();
      else if (yon == "L") sagaDon();     
      else if (yon == "R") solaDon();      
      else if (yon == "UL") ileriSag();    
      else if (yon == "UR") ileriSol();    
      else if (yon == "DL") geriSag();     
      else if (yon == "DR") geriSol();     
      else if (yon == "S") dur();
    } else {
      // Eski format için uyumluluk (hız ayarlama özelliği olmadan)
      if (gelenVeri == "U") ileri();
      else if (gelenVeri == "D") geri();
      else if (gelenVeri == "L") sagaDon();      // DEĞİŞTİRİLDİ
      else if (gelenVeri == "R") solaDon();      // DEĞİŞTİRİLDİ
      else if (gelenVeri == "UL") ileriSag();    // DEĞİŞTİRİLDİ
      else if (gelenVeri == "UR") ileriSol();    // DEĞİŞTİRİLDİ
      else if (gelenVeri == "DL") geriSag();     // DEĞİŞTİRİLDİ
      else if (gelenVeri == "DR") geriSol();     // DEĞİŞTİRİLDİ
      else if (gelenVeri == "S") dur();
    }
  }
}

void ileri() {
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void geri() {
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void solaDon() {
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void sagaDon() {
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void ileriSol() {
  analogWrite(ENA, motorSpeed * 0.5); // Sol motor yavaş
  analogWrite(ENB, motorSpeed);       // Sağ motor normal
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void ileriSag() {
  analogWrite(ENA, motorSpeed);       // Sol motor normal
  analogWrite(ENB, motorSpeed * 0.5); // Sağ motor yavaş
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void geriSol() {
  analogWrite(ENA, motorSpeed * 0.5); // Sol motor yavaş
  analogWrite(ENB, motorSpeed);       // Sağ motor normal
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void geriSag() {
  analogWrite(ENA, motorSpeed);       // Sol motor normal
  analogWrite(ENB, motorSpeed * 0.5); // Sağ motor yavaş
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void dur() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}