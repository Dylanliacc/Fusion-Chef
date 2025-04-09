#ifndef GESTURE_HANDLER_H
#define GESTURE_HANDLER_H

#include "Gesture.h"

// 手势识别相关定义
#define GES_REACTION_TIME    500
#define GES_ENTRY_TIME      800
#define GES_QUIT_TIME       1000

class GestureHandler {
public:
    GestureHandler();
    bool init();
    void handleGestures();

private:
    paj7620 gesture;
    void handleDirectionalGesture(paj7620_gesture_t direction);
};

#endif