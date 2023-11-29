#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <UniversalTelegramBot.h>

String telegram_bot_token = getenv("TELEGRAM_BOT_TOKEN");
String telegram_chat_id = getenv("TELEGRAM_CHAT_ID");

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}
