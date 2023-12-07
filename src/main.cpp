#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <../Secrets.h>
#include <NewPing.h>

#define MAX_DISTANCE 10

struct SensorLocation {
  NewPing sensor;
  String location;
};

// Define sensor and location pairs
SensorLocation sensorLocations[] = {
  {NewPing(12, 13, MAX_DISTANCE), "창고"}, // (trigger_pin, echo_pin, maximum_distance)
  {NewPing(16, 17, MAX_DISTANCE), "복도"},
  {NewPing(18, 19, MAX_DISTANCE), "창문"},
  {NewPing(26, 27, MAX_DISTANCE), "현관문"}
};

void sendTelegramNotification(String location);

void setup() {
  Serial.begin(115200);

  WiFi.begin(getWifiSSID(), getWifiPassWord());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {

    for (int i = 0; i < 4; i++) {
      delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.

      unsigned int uS = sensorLocations[i].sensor.ping(); // Send ping, get ping time in microseconds (uS).

      if (uS != NO_ECHO) { // if there's an echo
        sendTelegramNotification(sensorLocations[i].location);
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
