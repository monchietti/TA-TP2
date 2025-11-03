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
    if(text == "/start"){
      // al estar dentro de una cadena se deben usar los caracteres de escape para las comillas
      String json = "[[\"/ledAzulOn\", \"/ledAzulOff\", \"/ledVerdeOn\", \"/ledVerdeOff\"], [\"/dht22\", \"/pote\", \"/platiot\", \"/display\"]]";
      //manda como texto lo que dice el boton json por lo que se debe cambiar las opciones
      bot.sendMessageWithReplyKeyboard(
        chat_id, 
        "Chat iniciado! \n Comandos: \n /ledAzulOn: enciende LED AZUL \n /ledAzulOff: apagar LED AZUL "
        "\n /ledVerdeOn: enciende LED VERDE \n /ledVerdeOff: apagar LED VERDE" 
        "\n /dht22: informar valores de humedad y temperatura"
        "\n /pote: leer valor de voltaje del potenciometro"
        "\n /platiot enviar datos de humedad y temperatura a ..."
        "\n /display<conmando>: permite mostrar el estado de los componentes en el display"
        "\n \t led-pote-dht22", "", json);
      //mensaje de bienvenida + mostrar opciones de MENU:
      //el mensaje va para un chat_id concreto

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
