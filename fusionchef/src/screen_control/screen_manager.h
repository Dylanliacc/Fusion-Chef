#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <Arduino.h>
#include <vector>
#include "../eink/eink.h"

enum class PageType {
    WEATHER,
    CALENDAR,
    WIFI_INFO,
    LONG_TEXT,
    CUSTOM
};

struct Page {
    PageType type;
    String name;
    void (*drawFunction)();
};

class ScreenManager {
public:
    static ScreenManager& getInstance();
    void nextScreen();
    void previousScreen();
    void nextSubPage();
    void previousSubPage();
    void switchScreen(int screen);
    void init();
    
    // 新增页面管理方法
    void registerPage(PageType type, const String& name, void (*drawFunction)());
    void removePage(PageType type);
    int getPageCount() const { return pages.size(); }
    
    // 原有方法
    int getCurrentScreen() const { return currentScreen; }
    int getCurrentSubPage() const { return currentSubPage; }
    int getTotalSubPages() const { return totalSubPages; }
    void setLongText(const String& text, const String& title, int parentPage = -1);
    const String& getCurrentText() const { return currentText; }
    const String& getCurrentTitle() const { return currentTitle; }
    int getParentPage() const { return parentPage; }

private:
    ScreenManager() : currentScreen(0), currentSubPage(0), totalSubPages(1), parentPage(-1) {}
    ScreenManager(const ScreenManager&) = delete;
    ScreenManager& operator=(const ScreenManager&) = delete;
    
    void updateTotalSubPages();
    void calculatePages();
    
    std::vector<Page> pages;
    int currentScreen;
    int currentSubPage;
    int totalSubPages;
    String currentText;
    String currentTitle;
    int parentPage;
};

#endif