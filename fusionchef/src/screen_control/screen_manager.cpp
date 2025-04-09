#include "screen_manager.h"
#include "../eink/eink.h"

ScreenManager& ScreenManager::getInstance() {
    static ScreenManager instance;
    return instance;
}

void ScreenManager::init() {
    eink_setup();
    switchScreen(currentScreen);
}

void ScreenManager::nextScreen() {
    currentScreen = (currentScreen + 1) % 3;
    switchScreen(currentScreen);
}

void ScreenManager::previousScreen() {
    currentScreen = (currentScreen - 1 + 3) % 3;
    switchScreen(currentScreen);
}

void ScreenManager::switchScreen(int screen) {
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