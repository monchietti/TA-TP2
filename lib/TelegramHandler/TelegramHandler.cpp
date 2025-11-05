#include "TelegramHandler.h"
TelegramHandler::TelegramHandler(Device device, String botToken, const int chat_id) :
    bot(botToken, secured_client),
    device(device)
{

}


void TelegramHandler::begin(){
    secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
    device.begin();
    ThingSpeak.begin(clienteThingSpeak);// Inicia ThingSpeak con el cliente

}


void TelegramHandler::actualizar(){
    device.showDisplay();
    listenForMessages();
    
}

void TelegramHandler::listenForMessages(){
    if (millis() - previous > SCAN_TIME ){
    //dame la cantidad de mensajes nuevo que llegaron desde el ultimo SCAN_TIME
    int n = bot.getUpdates(bot.last_message_received + 1);
    Serial.println("mensajes: " + String(n));
    //mientras que haya mensajes nuevos, los manejo
    while(n){
      handleMessages(n);
      n = bot.getUpdates(bot.last_message_received + 1);
    }

    previous = millis();
  }
}

void TelegramHandler::handleMessages(int n){
    for (size_t i = 0; i < n; i++){
    //dame el id de los mensajes nuevos
    String chat_id = bot.messages[i].chat_id; //informativo
    //dame todos los mensajes
    String text = bot.messages[i].text; //contenido de cada mensaje
    Serial.println("Mensaje recibido: " + text);
    String type = bot.messages[i].type;

    if (type == "callback_query") {
        // En algunas versiones el callback llega dentro de text
        text = bot.messages[i].text;
        bot.answerCallbackQuery(bot.messages[i].query_id, "✔️ Comando recibido");
    }
    if (text == "/start") {
      // Construir el teclado inline. (texto, callback_data: lo que se recibe al presionar)
    String keyboardJson = 
      "["
        "[{\"text\":\"LED Azul ON\",\"callback_data\":\"/ledAzulOn\"},"
         "{\"text\":\"LED Azul OFF\",\"callback_data\":\"/ledAzulOff\"}],"
        "[{\"text\":\"LED Verde ON\",\"callback_data\":\"/ledVerdeOn\"},"
         "{\"text\":\"LED Verde OFF\",\"callback_data\":\"/ledVerdeOff\"}],"
        "[{\"text\":\"Leer DHT22\",\"callback_data\":\"/dht22\"},"
         "{\"text\":\"Leer Potenciómetro\",\"callback_data\":\"/pote\"}],"
        "[{\"text\":\"Enviar a ThingSpeak\",\"callback_data\":\"/platiot\"}],"
        "[{\"text\":\"Display\",\"callback_data\":\"/displaymenu\"}]"
      "]";

    bot.sendMessageWithInlineKeyboard(
      chat_id,
      "👋 ¡Bienvenido al Bot del dispositivo!\nSelecciona una opción:",
      "",
      keyboardJson);
    }
    else if (text == "/displaymenu") {
    String displayKeyboard = 
      "["
        "[{\"text\":\"Mostrar LEDs\",\"callback_data\":\"/displayled\"}],"
        "[{\"text\":\"Mostrar Potenciómetro\",\"callback_data\":\"/displaypote\"}],"
        "[{\"text\":\"Mostrar DHT22\",\"callback_data\":\"/displaydht22\"}]"
      "]";
    
    bot.sendMessageWithInlineKeyboard(
      chat_id,
      "📺 Opciones del Display:",
      "",
      displayKeyboard);
    }

    else if(text == "/ledAzulOn"){
        device.prenderLed("AZUL");
        bot.sendMessage(chat_id, "\n LED AZUL ENCENDIDO");
    }

    else if(text == "/ledAzulOff"){
        device.apagarLed("AZUL");
        bot.sendMessage(chat_id, "\n LED AZUL APAGADO");
    }

    else if(text == "/ledVerdeOn"){
        device.prenderLed("VERDE");
        bot.sendMessage(chat_id, "\n LED VERDE ENCENDIDO");
    }

    else if(text == "/ledVerdeOff"){
        device.apagarLed("VERDE");
        bot.sendMessage(chat_id, "\n LED VERDE APAGADO");
    }

    else if(text == "/dht22"){
        float temp = device.readTemp();
        float hum = device.readHum();
        String mensaje = "Temperatura: " + String(temp) + " °C \nHumedad: " + String(hum) + " %";
        bot.sendMessage(chat_id, mensaje);
    }

    else if(text == "/pote"){
        float poteValue = device.readPot();
        String mensaje = "Valor del potenciómetro: " + String(device.mapPotToVolt(poteValue)) + " V";
        bot.sendMessage(chat_id, mensaje);
    }


    else if(text == "/platiot" ){
        float temp = device.readTemp();
        float hum = device.readHum();
        ThingSpeak.setField(1, temp);
        ThingSpeak.setField(2, hum);
        ThingSpeak.writeFields(channelID, writeAPIKey); // Reemplaza con tu número de canal
        bot.sendMessage(chat_id, "\n Enviando datos a ThingSpeak \n Temperatura: " + String(temp) + " °C \nHumedad: " + String(hum) + " %");

        
    }

    else if(text.startsWith("/display")){
        String comando = text.substring(8); // Extrae el comando después de "/display"
        if(text =="/display"){
            bot.sendMessage(chat_id, "\n comandos display: \n /displayled \n /displaypote \n /displaydht22");
            return;
        }
        
        if(comando == "led"){
            device.currentDisplay = "LEDS";
            bot.sendMessage(chat_id, "\n Estado de los LEDs mostrado en el display.");
        } 
        else if (comando == "pote"){
            device.currentDisplay = "POTE";
            bot.sendMessage(chat_id, "\n Valor del potenciómetro mostrado en el display.");
        }
        else if (comando == "dht22"){
            device.currentDisplay = "DHT22";
            bot.sendMessage(chat_id, "\n Valores del DHT22 mostrados en el display.");
        }
        else {
            device.currentDisplay = "";
            bot.sendMessage(chat_id, "\n Comando de display no reconocido. Use: led, pote, dht22");
        }
    }

    
    else {
      bot.sendMessage(chat_id, "Comando no reconocido");
    }
  }
}
