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
    currentScreen = (currentScreen + 1) % TOTAL_SCREENS;
    currentSubPage = 0; // Reset subpage when switching screens
    updateTotalSubPages();
    switchScreen(currentScreen);
}

void ScreenManager::previousScreen() {
    currentScreen = (currentScreen - 1 + TOTAL_SCREENS) % TOTAL_SCREENS;
    currentSubPage = 0; // Reset subpage when switching screens
    updateTotalSubPages();
    switchScreen(currentScreen);
}

void ScreenManager::nextSubPage() {
    if (currentSubPage < totalSubPages - 1) {
        currentSubPage++;
        switchScreen(currentScreen);
    }
}

void ScreenManager::previousSubPage() {
    if (currentSubPage > 0) {
        currentSubPage--;
        switchScreen(currentScreen);
    }
}

void ScreenManager::updateTotalSubPages() {
    switch (currentScreen) {
        case 3: // LongText page
            totalSubPages = 3; // Example: 3 subpages for long text
            break;
        default:
            totalSubPages = 1;
            break;
    }
}

void ScreenManager::switchScreen(int screen) {
    switch (screen) {
        case 0:
            drawWeatherPage();
            break;
        case 1:
            drawCalendarPage();
            break;
        case 2:
            drawWifiInfoPage();
            break;
        case 3:
            drawLongTextPage(currentSubPage);
            break;
        default:
            break;
    }
}