#include "Device.h"


Device::Device(int w, int h, int reset, int i2c_sda, int i2c_scl,int pinDHT, const uint8_t model, int pinPOT, int ledAzul, int ledVerde) : 
_sensor(pinDHT, model), 
_display(w, h, &Wire, reset)
{
    PIN_POT = pinPOT;
    LED_AZUL = ledAzul;
    LED_VERDE = ledVerde;
    PIN_SDA = i2c_sda;
    PIN_SCL = i2c_scl;
}

void Device::begin()
{   
    Serial.println("INICIANDO...");
    // inicializacion de la pantalla
    Serial.println(PIN_SDA);
    Serial.println(PIN_SCL);
    Wire.begin(PIN_SDA, PIN_SCL);
    if (!_display.begin(0x3C, true))
    {
        Serial.println(F("ERROR: No se detecta OLED (0x3C)."));
        while (true)
            delay(10);
    }

    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setTextColor(SH110X_WHITE);
    // inicializacion del sensor
    _sensor.begin();

    // inicializacion pines
    pinMode(LED_VERDE, OUTPUT);
    pinMode(LED_AZUL, OUTPUT);
}

void Device::escribirPantalla(String texto){
    _display.clearDisplay();
    _display.setCursor(0, 0);
    _display.println(texto);
    _display.display();
}

float Device::readTemp()
{
    return _sensor.readTemperature();
}
float Device::readHum()
{
    return _sensor.readHumidity();
}

float Device::readPot()
{
    return analogRead(PIN_POT);
}
float Device::mapPotToVolt(int raw)
{
    return 0.0f + (3.3f - 0.0f) * (float)raw / 4095.0f;
}

void Device::prenderLed(String color)
{
    if(color == "AZUL")
        digitalWrite(LED_AZUL, HIGH);
    else if(color == "VERDE")
        digitalWrite(LED_VERDE, HIGH);
}
void Device::apagarLed(String color){
    if(color == "AZUL")
        digitalWrite(LED_AZUL, LOW);
    else if(color == "VERDE")
        digitalWrite(LED_VERDE, LOW);
}

void Device::showDisplay(){
    if (currentDisplay == "DHT22"){
        mostrarPantallaDHT();
    }
    else if (currentDisplay == "POTE"){
        mostrarPantallaPote();
    }
    else if (currentDisplay == "LEDS"){
        mostrarPantallaLeds();
    }
    else{
        escribirPantalla("\n \n \n \n TP2 - GRUPO 3");
    }
}

void Device::mostrarPantallaDHT(){
    String texto = "DHT22:\n"
                    "Temp: " + String(readTemp()) + " C\n" +
                    "Humedad: " + String(readHum()) + " %";
    escribirPantalla(texto);
}

void Device::mostrarPantallaPote(){
    String texto = "POTENCIOMETRO:\n"
                    "Voltaje: " + String(mapPotToVolt(readPot())) + " V";
    escribirPantalla(texto);

}

void Device::mostrarPantallaLeds(){
    String texto = "LED AZUL: " + String(digitalRead(LED_AZUL) ? "ENCENDIDO" : "APAGADO") + "\n" +
                    "LED VERDE: " + String(digitalRead(LED_VERDE) ? "ENCENDIDO" : "APAGADO");
    escribirPantalla(texto);
}


// ========== FUNCIONES DEL ENCODER ==========
//valida si hubo o no rotacion y en que direccion


// ========== FUNCIONES DEL MENÚ ==========

