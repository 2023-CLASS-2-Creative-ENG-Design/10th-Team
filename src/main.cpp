#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <../Secrets.h>

String server = "api.telegram.org";
String message = "Hello!";

void setup()
{
  Serial.begin(115200);

  WiFi.begin(getWifiSSID(), getWifiPassWord());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop()
{
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClientSecure httpsClient;
    HTTPClient http;

    String url = "/bot" + getToken() + "/sendMessage?chat_id=" + getChatID() + "&text=" + message;
    httpsClient.setInsecure(); // SSL 인증서 검증 비활성화
    http.begin(httpsClient, "https://" + server + url); // HTTPS 연결 설정

    Serial.print("[HTTPS] GET... ");
    int httpCode = http.GET(); // HTTP 요청 수행

    if (httpCode > 0) {
      String payload = http.getString(); // HTTP 응답 가져오기
      Serial.println("HTTP Status code: " + String(httpCode));
      Serial.println("HTTP Response: " + payload);
    }
    else {
      Serial.printf("[HTTPS] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end(); // HTTP 연결 종료
  }

  delay(10000);
}
