//Archivo .h
#ifndef DEVICE
#define DEVICE 

//para que tenga las constantes y funciones de arduino
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_SH110X.h>





class Device{
    //Por convencion de C++, primero la parte publica y luego la privada
    public:
        Device(int w, int h, int reset, int I2C_SDA, int I2C_SCL,int pinDHT, const uint8_t model, int pinPOT, int ledAzul, int ledVerde);
        void begin(); //Inicia el atributo sensor y el atributo display
        void escribirPantalla(String texto); //puntero a un array de letras

        float readTemp();
        float readHum();
        float readPot();
        float mapPotToVolt(int raw);
        //funciones de actualizacion de datos
        void actualizarHumedad();
        void actualizarTemperatura();
        
        //funciones del encoder
        void tick();
        void chequearEncoder();
        void manejarRotacion(String sentido);

        //funciones del menu
        void showDisplay();
        void mostrarPantallaDHT();
        void mostrarPantallaPote();
        void mostrarPantallaLeds();

        
        void prenderLed(String color);
        void apagarLed(String color);

    private:
        DHT _sensor;
        Adafruit_SH1106G _display;

        // Pines del encoder
        int lastStateCLK;

        //variables del sensor
        float humedad = 0.0;
        float temp = 0.0;
        float tempPot = 0.0;
        
    public:
        int PIN_POT;
        int LED_AZUL;
        int LED_VERDE;
        int PIN_SDA; 
        int PIN_SCL;
        String currentDisplay = "";

}; //no olvidarse del ;


#endif