#include "gesture_handler.h"
#include "../screen_control/screen_manager.h"
#include <Arduino.h>

GestureHandler::GestureHandler() {}

bool GestureHandler::init() {
    return gesture.init();
}

void GestureHandler::handleGestures() {
    paj7620_gesture_t result;
    if (gesture.getResult(result)) {
        switch (result) {
            case UP:
            case DOWN:
            case LEFT:
            case RIGHT:
                handleDirectionalGesture(result);
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
    delay(100);
}

void GestureHandler::handleDirectionalGesture(paj7620_gesture_t direction) {
    delay(GES_ENTRY_TIME);
    paj7620_gesture_t result;
    gesture.getResult(result);

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
                ScreenManager::getInstance().previousSubPage();
                break;
            case DOWN:
                Serial.println("Down");
                ScreenManager::getInstance().nextSubPage();
                break;
            case LEFT:
                Serial.println("Left");
                ScreenManager::getInstance().previousScreen();
                break;
            case RIGHT:
                Serial.println("Right");
                ScreenManager::getInstance().nextScreen();
                break;
            default:
                break;
        }
    }
}