// Данный кусочек кода отвечает за внедрения "Шапки", в которой прописаны детали библиотеки
#include "qmobot.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "UniversalTelegramBot.h"

// Initialize Wifi connection to the router
char ssid[] = "NU"; // your network SSID (name)
char password[] = "1234512345"; // your network key

int datchik = 13;

// Initialize Telegram BOT
#define BOTtoken "911022197:AAEhKKnuTgaNCKJ_Zbeflj2G9yFFs--zcjI" // your Bot Token (Get from Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime; //last time messages' scan has been done
bool Start = false;

int ledStatus = 0;

void handleNewMessages(int numNewMessages) {
Serial.println("handleNewMessages");
Serial.println(String(numNewMessages));

for (int i=0; i<numNewMessages; i++) {
String chat_id = String(bot.messages[i].chat_id);
String text = bot.messages[i].text;

if (text == "/ledon") {
digitalWrite(BUILTIN_LED, HIGH); // turn the LED on (HIGH is the voltage level)
ledStatus = 1;
bot.sendMessage(chat_id, "Включен", "");
}

if (text == "/ledoff") {
ledStatus = 0;
digitalWrite(BUILTIN_LED, LOW); // turn the LED off (LOW is the voltage level)
bot.sendMessage(chat_id, "Выключен", "");
}

if (text == "/status") {
if(ledStatus){
bot.sendMessage(chat_id, "Включен", "");
} else {
bot.sendMessage(chat_id, "Выключен", "");
}
}

if (text == "/start") {
String welcome = "Привет, я Qmobot Smart Home.\n";
welcome += "/ledon : включить лампу\n";
welcome += "/ledoff : выключить лампу\n";
welcome += "/status : показать статус лампы\n";
bot.sendMessage(chat_id, welcome, "Markdown");
}

if (text == "/dozor"){
int pokazaniye;

while(true){
  pokazaniye = digitalRead(datchik);
  if ( pokazaniye == 1){
    bot.sendMessage(chat_id,"Hataga bireu kirdi", "");
    delay(100);
  }
  else{
    delay(1000);
  }

  Serial.print("rabotaet"); 
  }

}
}
}

void setup() {
Qchip.begin(true, true);

// Attempt to connect to Wifi network:
Serial.print("Connecting Wifi: ");
Serial.println(ssid);

// Set WiFi to station mode and disconnect from an AP if it was Previously
// connected
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
Serial.print(".");
delay(500);
}

Serial.println("");
Serial.println("WiFi connected");
Serial.print("IP address: ");
Serial.println(WiFi.localIP());

pinMode(datchik, INPUT);
pinMode(BUILTIN_LED, OUTPUT); // initialize digital BUILTIN_LED as an output.
delay(10);
digitalWrite(BUILTIN_LED, LOW); // initialize pin as off
}

void loop() {
if (millis() > Bot_lasttime + Bot_mtbs) {
int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

while(numNewMessages) {
Serial.println("got response");
handleNewMessages(numNewMessages);
numNewMessages = bot.getUpdates(bot.last_message_received + 1);
}

Bot_lasttime = millis();
}

}


