#ifndef TELEGRAMHANDLER
#define TELEGRAMHANDLER



#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include "Device.h"
#include <ThingSpeak.h>

class TelegramHandler{
    public:
        TelegramHandler(Device device, String botToken, const int chat_id);

    private:
        UniversalTelegramBot bot;
        int _chat_id;
        
        WiFiClientSecure secured_client;
        unsigned long previous;
        const unsigned long SCAN_TIME = 500;
        Device device;
        //Cliente para ThingSpeak
        WiFiClient clienteThingSpeak;
        const char* writeAPIKey = "8Q69REQE7ZYGILJ6";
        const int channelID = 3145077;
    public:
        void listenForMessages();
        void handleMessages(int n);
        void begin();
        void actualizar();
    
};

#endif