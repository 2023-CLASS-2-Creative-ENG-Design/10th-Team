#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <../Secrets.h>

// 센서 관련 구조체
struct Sensor
{
  int trigPin;
  int echoPin;
  int speakerPin;
  int distance;
  bool objectDetected;
  String location;
};

// 센서 개수와 센서 객체 배열
#define SENSOR_COUNT 4
Sensor sensors[SENSOR_COUNT] = {
    {2, 3, 11, 0, false, "출입문"},
    {4, 5, 12, 0, false, "창문"},
    {6, 7, 13, 0, false, "복도"},
    {8, 9, 14, 0, false, "창고"}};

#define ledPin 10
#define ALARM_COUNT 5
#define DETECT_DISTANCE 10
#define DELAY_TIME 500
#define ALARM_FREQUENCY 1000

bool objectDetected = false; // 물체 감지 여부
int alarmCount = 0;          // 알람이 울린 횟수

WiFiClient client;

// 헤더 파일에서 WiFi와 텔레그램 정보 읽기
String ssid = getWifiSSID();
String password = getWifiPassWord();
String botToken = getToken();
String chatID = getChatID();

// 감지 후 사용자에게 보낼 메시지
String message = "에서 비정상적인 움직임이 감지됨. 확인해주세요.";

void setup()
{
  for (int i = 0; i < SENSOR_COUNT; ++i)
  {
    pinMode(sensors[i].trigPin, OUTPUT);
    pinMode(sensors[i].echoPin, INPUT);
    pinMode(sensors[i].speakerPin, OUTPUT);
  }

  pinMode(ledPin, OUTPUT);

  // WiFi 연결
  WiFi.begin(ssid.c_str(), password.c_str());
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
  }
}

void loop()
{
  // 각 센서 읽기
  for (int i = 0; i < SENSOR_COUNT; ++i)
  {
    readSensor(sensors[i]);
  }

  // 객체가 감지되었는지 판단
  bool objectDetectedNow = false;
  String detectedLocation = "";

  for (int i = 0; i < SENSOR_COUNT; ++i)
  {
    if (sensors[i].objectDetected)
    {
      objectDetectedNow = true;
      detectedLocation = sensors[i].location;
      break;
    }
  }

  // 객체가 감지되었을 때 텔레그램 메시지 전송
  if (objectDetectedNow && !objectDetected)
  {
    objectDetected = true;

    String finalMessage = detectedLocation + message;

    // 텔레그램 메시지 전송
    if (WiFi.status() == WL_CONNECTED)
    {
      HTTPClient http;
      http.begin(client, "http://api.telegram.org/bot" + botToken + "/sendMessage?chat_id=" + chatID + "&text=" + urlEncode(finalMessage));
      http.GET();
      http.end();
    }
  }

  // 객체 감지 상태 리셋
  if (!objectDetectedNow)
  {
    objectDetected = false;
  }
}

String urlEncode(const String &str)
{
  String encodedStr = "";
  char c;

  for (int i = 0; i < str.length(); i++)
  {
    c = str.charAt(i);

    if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '*')
    {
      encodedStr += c;
    }
    else if (c == ' ')
    {
      encodedStr += '+';
    }
    else
    {
      encodedStr += '%';
      encodedStr += String((c >> 4) & 0xF, 16);
      encodedStr += String(c & 0xF, 16);
    }
  }

  return encodedStr;
}

// 센서 읽기 함수
void readSensor(Sensor &sensor)
{
  digitalWrite(sensor.trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor.trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor.trigPin, LOW);
  long duration = pulseIn(sensor.echoPin, HIGH);
  sensor.distance = (duration / 2) / 29.1;

  if (sensor.distance < DETECT_DISTANCE && !sensor.objectDetected)
  {
    sensor.objectDetected = true;
    for (int i = 0; i < ALARM_COUNT; i++)
    {
      tone(sensor.speakerPin, ALARM_FREQUENCY);
      delay(DELAY_TIME);
      noTone(sensor.speakerPin);
      delay(DELAY_TIME);
    }
  }

  if (sensor.distance >= DETECT_DISTANCE)
  {
    sensor.objectDetected = false;
  }
}