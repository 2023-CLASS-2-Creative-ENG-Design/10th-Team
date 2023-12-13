<p align="center">
<img src="history/logo.png">
</p>

안전은 모두에게 보장되어야 할 기본적인 권리입니다. 우리가 잠시 자리를 비웠을 때, 그 안전은 위협받을 수 있습니다. 이러한 문제에 대응하기 위해, '찾아라 내 도도독' 프로젝트를 시작하게 되었습니다.  
이 프로젝트는 아두이노 기반의 도둑 탐지 시스템을 개발하는 것을 목표로 합니다.

## What is 찾아라 내 도도독?

'찾아라 내 도도독'은 아두이노를 기반으로 한 보안 시스템입니다. 이 시스템은 초음파 센서를 통해 창고, 복도, 창문, 입구 등의 공간을 모니터링하고, 이상한 움직임이나 행동을 감지합니다.

## Overview

이 프로젝트의 핵심 구성 요소는 ESP32 보드와 초음파 센서입니다. 이 시스템은 WiFi를 통해 인터넷에 연결되며, 이상 행동이 감지되면 사용자에게 텔레그램 앱을 통해 실시간으로 알림을 보냅니다. 또한, 피에조 부저 기능이 추가되어 움직임이 감지될 때 청각적 경고도 제공합니다.

## Features & Capabilities

'찾아라 내 도도독'은 다음과 같은 기능을 수행할 수 있습니다: 
- 여러 위치를 짧은 주기로 돌아가며 모니터링합니다.
- 최대 2m 거리에서 움직임을 감지합니다.
- 이상 징후가 감지되면 실시간으로 텔레그램 계정에 알림을 전송합니다.
- 피에조 부저를 사용하여 청각적 경고를 제공합니다.

## Known issues and Limitations

- 인터넷 연결이 필요합니다.
- 감지 가능한 최대 거리는 2m입니다.
- 사용자가 Telegram 봇 토큰과 Chat ID를 설정해야 합니다.

## How it works

NewPing 라이브러리를 사용하여 초음파 센서를 제어합니다. 센서는 핑을 보내고, 에코(즉, 객체로부터의 핑 반사)를 감지하면 알림을 트리거합니다. 알림은 Telegram API를 사용하여 정의된 Telegram 계정으로 메시지를 보냅니다.

## Tech Stacks

### Hardware

- ESP32: 메인 컨트롤러로 사용되며, 각 센서의 상태를 모니터링하고, 감지 정보를 전송합니다.
- 초음파 센서: 이상한 움직임을 감지하는 데 사용됩니다.
- 피에조 부저: 이상 행동 감지 시 청각적 경고를 제공합니다.

### Software

- C/C++: 아두이노 프로그래밍의 주 언어입니다.
- Visual Studio Code: 코드 편집 및 디버깅을 위한 주요 IDE입니다.
- PlatformIO: Visual Studio Code에서 사용하는 아두이노 개발 환경입니다.
- WiFi, HTTPClient, WiFiClientSecure: 인터넷 연결 및 HTTPS 통신을 위한 라이브러리들입니다.
- NewPing: 초음파 센서를 제어하는 데 사용되는 라이브러리입니다.

### Communication

- Telegram API: 사용자에게 텔레그램으로 알림 메시지를 전송하는 데 사용됩니다.

### Code Snippet

아래는 프로젝트의 주요 코드 부분입니다. 이 코드는 초음파 센서를 이용하여 특정 공간에서 이상한 움직임을 감지하며, 감지한 정보를 텔레그램을 통해 알림으로 전송합니다.

```C++
SensorLocation sensorLocations[] = {
  {NewPing(12, 13, MAX_DISTANCE), "창고"},
  ...
};

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {
    for (int i = 0; i < 4; i++) {
      unsigned int uS = sensorLocations[i].sensor.ping();
      if (uS != NO_ECHO) {
        sendTelegramNotification(sensorLocations[i].location);
        soundAlarm();
      }
    }
  }
}

void sendTelegramNotification(String location) {
  WiFiClientSecure httpsClient;
  HTTPClient http;

  String message = location + "에서 수상한 움직임이 감지되었습니다!";
  String url = "/bot" + getToken() + "/sendMessage?chat_id=" + getChatID() + "&text=" + message;
  
  int httpCode = http.GET(); // HTTP 요청 수행
  ...
}

void soundAlarm() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);                   // 1초 대기
    digitalWrite(BUZZER_PIN, LOW); 
    delay(1000);                   
  }
}
```

## Contributors
- [양석훈](https://github.com/Seokhun-Yang) *(Seokhun Yang)* <br>
- [이기준](https://github.com/rlwns1224) *(rlwns1224)* <br>
- [이상민](https://github.com/sladkt) *(LeeSangMin)* <br>
- [정윤서](https://github.com/cloudddddl) *(cloudddddl)* <br>

## Project plan and presentation materials

프로젝트 계획서와 프로젝트를 진행하면서 사용했던 발표 자료들을 저장한 디렉토리입니다.

- [History](./history)
