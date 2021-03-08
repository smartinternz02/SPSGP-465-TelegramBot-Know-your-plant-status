
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h> // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>
#include "DHT.h"

#define DHTPIN 4
#define motor 18
#define ldrPin 34
#define soilmoisture 35
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
// Replace with your network credentials
const char* ssid = "1058550129";
const char* password = "Stub921";

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "1214836823"

// Initialize Telegram BOT
#define BOTtoken "1388787884:AAEa1MJdu7k6aDfu9RR21YeR9zVZfMbPFeY"  // your Bot Token (Get from Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

//Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;
float h,t,l,s;

// Get sensor readings and return them as a String variable
String getReadings() {
   h = dht.readHumidity();
  // Read temperature as Celsius (the default)
   t = dht.readTemperature();
   l = analogRead(34);
   s = analogRead(35);
   Serial.println(s);
  String message = "Temperature: " + String(t) + " ºC \n";
  message += "Humidity: " + String (h) + " % \n";
  message += "Light Intensity: " + String (l) + " % \n";
  message += "Soil Moisture: " + String (s) + " % \n";
  return message;
}

//Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
// Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the /readings command to get current readings.\n";
      welcome += "Use /motoron and /motoroff \n";
      welcome += "to control Motor \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/readings") {
      String readings = getReadings();
      bot.sendMessage(chat_id, readings, "");
    }
    if (text == "/motoron") {
      String cmd = "Motor is switched on";
      digitalWrite(18, HIGH);
      bot.sendMessage(chat_id, cmd, "");
    }
    if (text == "/motoroff") {
      String cmd = "Motor is switched off";
      digitalWrite(18, LOW);
      bot.sendMessage(chat_id, cmd, "");
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(18, OUTPUT);
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
    if(s<100){
    bot.sendMessage(CHAT_ID, "Ur plant needs water!!", "");
    Serial.println("Ur plant needs water!!");
  }
}
