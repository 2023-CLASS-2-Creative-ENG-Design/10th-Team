#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <../Secrets.h>
#include <NewPing.h>

#define NUMBER_OF_SENSOR 3
#define MAX_DISTANCE 20
#define BUZZER_PIN 5
#define MAX_ALARM_COUNT 5
#define ALARM_RESET_INTERVAL 3000

struct SensorLocation {
  NewPing sensor;
  String location;
};

SensorLocation sensorLocations[] = {
  {NewPing(16, 17, MAX_DISTANCE), "정문"}, // (trigger_pin, echo_pin, maximum_distance)
  {NewPing(18, 19, MAX_DISTANCE), "창문"},
  {NewPing(26, 27, MAX_DISTANCE), "후문"}
};

int alarmCount = 0;
unsigned long lastAlarmTime = 0;

void sendTelegramNotification(String location);
void soundAlarm();

void setup() {
  Serial.begin(115200);

  WiFi.begin(getWifiSSID(), getWifiPassWord());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {
    unsigned long currentTime = millis();

    if (currentTime - lastAlarmTime >= ALARM_RESET_INTERVAL) {
      alarmCount = 0;
    }

    for (int i = 0; i < NUMBER_OF_SENSOR; i++) {
      delay(50); 

      unsigned int uS = sensorLocations[i].sensor.ping();

      if (uS != NO_ECHO) {
        sendTelegramNotification(sensorLocations[i].location);
        if (alarmCount < MAX_ALARM_COUNT) {
          soundAlarm();
          alarmCount++;
          lastAlarmTime = currentTime;
        }
      }
    }
  }
}

void sendTelegramNotification(String location) {
  WiFiClientSecure httpsClient;
  HTTPClient http;

  String message = location + "에서 수상한 움직임이 감지되었습니다!";
  String url = "/bot" + getToken() + "/sendMessage?chat_id=" + getChatID() + "&text=" + message;
  
  httpsClient.setInsecure(); // SSL 인증서 검증 비활성화
  http.begin(httpsClient, "https://api.telegram.org" + url); // HTTPS 연결 설정

  Serial.print("[HTTPS] GET... ");
  int httpCode = http.GET(); // HTTP 요청 수행

  if (httpCode > 0) {
    String payload = http.getString(); // HTTP 응답 가져오기
    Serial.println("HTTP Status code: " + String(httpCode));
    Serial.println("HTTP Response: " + payload);
  }
  else {
    Serial.printf("[HTTPS] GET... failed, error: %s", http.errorToString(httpCode).c_str());
  }

  http.end(); // HTTP 연결 종료
}

void soundAlarm() {
  for (int i = 0; i < 3; i++) { // 3번 반복
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);                   // 1초 대기
    digitalWrite(BUZZER_PIN, LOW); 
    delay(1000);                   
  }
}
