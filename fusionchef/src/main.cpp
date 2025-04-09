#include <Arduino.h>
#include <WiFi.h>
#include "gesture_control/gesture_handler.h"
#include "screen_control/screen_manager.h"

GestureHandler gestureHandler;

void setup() {
    Serial.begin(9600);
    while(!Serial) {
        delay(100);
    }
    if (gestureHandler.init()) {
        Serial.println("PAJ7620U2 initialization failed");
    } else {
        Serial.println("PAJ7620U2 initialization success");
    }
    ScreenManager::getInstance().init();

    WiFi.mode(WIFI_AP_STA);
    WiFi.beginSmartConfig();

  //等待抓取到手机APP发送的UDP包
  Serial.println("Waiting for SmartConfig.");
  while (!WiFi.smartConfigDone()) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("SmartConfig received.");

  //等待与目标WIFI网络建立连接
  Serial.println("Waiting for WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
}
}
void loop() {
    gestureHandler.handleGestures();
}
