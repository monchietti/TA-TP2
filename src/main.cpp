#include <Arduino.h>
#include "Device.h"
#include "TelegramHandler.h"
// ---- Pines ----
#define PIN_DHT       33      // DATA del DHT22
#define DHTTYPE       DHT22
#define I2C_SDA       21
#define I2C_SCL       22
#define PIN_POT 32
#define LED_VERDE 23
#define LED_AZUL 2

//Constantes
String token = "8444080654:AAEshP2UtNAy5kTm1pbJoeyQ3LyLWTh5Ddw";
String ssid = "Wokwi-GUEST";
String password = "";

// ---- Objetos ----
Device device(127, 64, -1,  I2C_SDA, I2C_SCL, PIN_DHT, DHT22, PIN_POT, LED_AZUL, LED_VERDE);
TelegramHandler telegramHandler(device, token, 123456789);


void setup() {
  Serial.begin(115200);
  delay(100);
  // iniciar WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi conectado!");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  telegramHandler.begin();
}

void loop() {
  telegramHandler.actualizar();
}
