#include <Arduino.h>
#include "./eink/eink.h"
#include "Gesture.h"

// 手势识别相关定义
#define GES_REACTION_TIME    500             // 短的反应时间
#define GES_ENTRY_TIME        800             // 进入时间，确保手势被快速识别
#define GES_QUIT_TIME         1000            // 退出时间，手势识别后的延迟

paj7620 Gesture;

int currentScreen = 0;  // 当前屏幕索引

// 声明 switchScreen 函数
void switchScreen(int screen);
void handleDirectionalGesture(paj7620_gesture_t direction);

void setup() {
    Serial.begin(9600);
    while(!Serial) {
        delay(100);
    }
    Serial.println("\nPAJ7620U2 TEST DEMO: Recognize 9 gestures.");
    
    if (Gesture.init()) {
        Serial.println("PAJ7620U2 initialization failed");
    } else {
        Serial.println("PAJ7620U2 initialization success");
    }
    Serial.println("Please input your gestures:\n");

    eink_setup();  // 初始化墨水屏
}

void loop() {
    paj7620_gesture_t result;
    if (Gesture.getResult(result)) {
        switch (result) {
        case UP:
        case DOWN:
        case LEFT:
        case RIGHT:
            handleDirectionalGesture(result);  // 处理方向性手势
            break;

        case PUSH:
            Serial.println("Forward");
            delay(GES_QUIT_TIME);
            break;

        case POLL:
            Serial.println("Backward");
            delay(GES_QUIT_TIME);
            break;

        case CLOCKWISE:
            Serial.println("Clockwise");
            break;

        case ANTI_CLOCKWISE:
            Serial.println("Anti-clockwise");
            break;

        case WAVE:
            Serial.println("Wave");
            break;

        default:
            break;
        }
    }
    delay(100);  // 确保手势被平滑地轮询
}

// 处理方向性手势：UP、DOWN、LEFT、RIGHT
void handleDirectionalGesture(paj7620_gesture_t direction) {
    delay(GES_ENTRY_TIME);
    paj7620_gesture_t result;
    Gesture.getResult(result);

    if (result == PUSH) {
        Serial.println("Forward");
        delay(GES_QUIT_TIME);
    } else if (result == POLL) {
        Serial.println("Backward");
        delay(GES_QUIT_TIME);
    } else {
        switch (direction) {
        case UP:
            Serial.println("Up");
            break;
        case DOWN:
            Serial.println("Down");
            break;
        case LEFT:
            Serial.println("Left");
            break;
        case RIGHT:
            Serial.println("Right");
            break;
        default:
            break;
        }
    }

    // 根据手势切换屏幕
    switch (direction) {
        case RIGHT:
            currentScreen = (currentScreen + 1) % 3;  // 向右切换到下一个屏幕
            switchScreen(currentScreen);  // 更新显示内容
            break;
        case LEFT:
            currentScreen = (currentScreen - 1 + 3) % 3;  // 向左切换到上一个屏幕
            switchScreen(currentScreen);  // 更新显示内容
            break;
        default:
            break;
    }
}

// 根据当前屏幕索引更新墨水屏显示内容
void switchScreen(int screen) {
    switch (screen) {
    case 0:
        drawChart();
        break;
    case 1:
        drawCoordinatesExplanation();
        break;
    case 2:
        drawSplitScreen();
        break;
    default:
        break;
    }
}
