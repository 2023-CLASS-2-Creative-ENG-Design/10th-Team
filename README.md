<picture>
   <img width="750" alt="logo" src="https://github.com/2023-CLASS-2-Creative-ENG-Design/10th-Team/assets/95516455/9f07127e-9b34-47e1-aadd-482eba870cdb">
</picture>

안전은 모두에게 보장되어야 할 기본적인 권리입니다. 그러나 우리가 잠시나마 자리를 비울 때마다, 그 안전은 늘 위협 받을 수 있습니다. 이러한 문제를 해결하고자, '찾아라 내 도도독' 프로젝트를 시작하게 되었습니다. 이 프로젝트는 아두이노 기반으로 도둑을 탐지하는 시스템을 개발하는 것을 목표로 하고 있습니다.

## What is 찾아라 내 도도독?

'찾아라 내 도도독'은 특정 공간에서 비정상적인 움직임이나 이상 행동을 감지하는 아두이노 기반의 보안 시스템입니다. 초음파 센서를 통해 창고, 복도, 창문, 입구 등의 공간을 모니터링하여 안전을 유지합니다.

## Overview

이 프로젝트의 핵심 구성 요소는 아두이노 보드와 초음파 센서입니다. 이 시스템은 WiFi를 통해 인터넷에 연결되며, 이상 행동이 감지되면 사용자에게 Telegram 어플리케이션을 통해 실시간으로 알림을 보내줍니다. 이를 통해 사용자는 언제 어디서든 자신의 공간에 이상한 움직임이 감지되면 알 수 있습니다.

## Features & Capabilities

'찾아라 내 도도독'은 다음과 같은 기능을 수행할 수 있습니다: 
- 여러 위치를 동시에 모니터링합니다.
- 최대 2m 거리에서 움직임을 감지합니다.
- 이상 징후가 감지되면 실시간으로 Telegram 계정에 알림을 전송합니다.

## Known issues and Limitations

- 사용자에게 알림을 보내려면 인터넷에 연결되어야 합니다.
- 감지된 움직임의 거리는 최대 2m로 제한됩니다.
- 사용자가 Telegram의 봇 토큰과 Chat ID를 직접 입력해야 한다는 번거러움이 있습니다.

## How it works

NewPing 라이브러리를 사용하여 초음파 센서를 처리합니다. 센서는 핑을 보내고, 에코(즉, 객체로 인한 핑의 반사)를 감지하면 알림을 트리거합니다. 알림은 Telegram의 API를 사용하여 미리 정의된 Telegram 계정에 메시지를 보내는 것입니다.

## Tech Stacks

### Hardware

- 아두이노: 메인 컨트롤러로 사용되며, 각 센서의 상태를 모니터링하고, 감지 정보를 서버로 전송합니다.
- 초음파 센서: 특정 위치에서 이상한 움직임을 감지하는데 사용됩니다.

### Software

- C/C++: 아두이노 프로그래밍에 사용되는 주요 언어입니다.
- WiFi: 아두이노가 인터넷에 연결하기 위해 사용되는 라이브러리입니다.
- HTTPClient: HTTP 요청을 보내는 데 사용되는 라이브러리입니다.
- WiFiClientSecure: HTTPS 연결을 위해 필요한 라이브러리입니다.
- NewPing: 초음파 센서를 제어하는 데 사용되는 라이브러리입니다.

### Communication

- 텔레그램 API: 사용자에게 알람 메시지를 전송하는 데 사용되는 API입니다.

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
  
  int httpCode = http.GET(); // HTTP 요청 수행
  ...
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
