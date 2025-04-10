#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

// Total number of main screens
#define TOTAL_SCREENS 4 // Chart, Coordinates, Split, Weather, Calendar, WiFi, LongText

class ScreenManager {
public:
    static ScreenManager& getInstance();
    void nextScreen();
    void previousScreen();
    void nextSubPage();
    void previousSubPage();
    void switchScreen(int screen);
    void init();
    int getCurrentScreen() const { return currentScreen; }
    int getCurrentSubPage() const { return currentSubPage; }
    int getTotalSubPages() const { return totalSubPages; }

private:
    ScreenManager() : currentScreen(0), currentSubPage(0), totalSubPages(1) {}
    ScreenManager(const ScreenManager&) = delete;
    ScreenManager& operator=(const ScreenManager&) = delete;
    
    void updateTotalSubPages();
    
    int currentScreen;
    int currentSubPage;
    int totalSubPages;
};

#endif