#include "screen_manager.h"
#include "../eink/eink.h"

ScreenManager& ScreenManager::getInstance() {
    static ScreenManager instance;
    return instance;
}

void ScreenManager::init() {
    eink_setup();
    // 注册默认页面
    registerPage(PageType::WEATHER, "Weather", drawWeatherPage);
    registerPage(PageType::CALENDAR, "Calendar", drawCalendarPage);
    registerPage(PageType::WIFI_INFO, "WiFi", drawWifiInfoPage);
    switchScreen(currentScreen);
}

void ScreenManager::registerPage(PageType type, const String& name, void (*drawFunction)()) {
    // 检查是否已存在相同类型的页面
    for (const auto& page : pages) {
        if (page.type == type) return;
    }
    
    Page newPage = {type, name, drawFunction};
    pages.push_back(newPage);
}

void ScreenManager::removePage(PageType type) {
    for (auto it = pages.begin(); it != pages.end(); ++it) {
        if (it->type == type) {
            pages.erase(it);
            // 如果删除的是当前页面，切换到第一个页面
            if (currentScreen >= pages.size()) {
                currentScreen = 0;
                switchScreen(currentScreen);
            }
            return;
        }
    }
}

void ScreenManager::nextScreen() {
    if (pages.empty()) return;
    currentScreen = (currentScreen + 1) % pages.size();
    currentSubPage = 0;
    updateTotalSubPages();
    switchScreen(currentScreen);
}

void ScreenManager::previousScreen() {
    if (pages.empty()) return;
    currentScreen = (currentScreen - 1 + pages.size()) % pages.size();
    currentSubPage = 0;
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
            if (!currentText.isEmpty()) {
                calculatePages();
            } else {
                totalSubPages = 1;
            }
            break;
        default:
            totalSubPages = 1;
            break;
    }
}

void ScreenManager::switchScreen(int screen) {
    if (screen < 0 || screen >= pages.size()) return;
    
    if (pages[screen].type == PageType::LONG_TEXT && !currentText.isEmpty()) {
        int startPos = currentSubPage * MAX_CHARS_PER_PAGE;
        int endPos = std::min((int)(currentSubPage + 1) * MAX_CHARS_PER_PAGE, (int)currentText.length());
        String pageContent = currentText.substring(startPos, endPos);
        drawLongTextPage(pageContent, currentTitle, parentPage, currentSubPage, totalSubPages);
    } else {
        pages[screen].drawFunction();
    }
}

void ScreenManager::setLongText(const String& text, const String& title, int parentPageNum) {
    currentText = text;
    currentTitle = title;
    parentPage = parentPageNum;
    calculatePages();
}

void ScreenManager::calculatePages() {
    if (currentText.isEmpty()) {
        totalSubPages = 1;
        return;
    }
    totalSubPages = (currentText.length() + MAX_CHARS_PER_PAGE - 1) / MAX_CHARS_PER_PAGE;
    if (totalSubPages < 1) totalSubPages = 1;
}