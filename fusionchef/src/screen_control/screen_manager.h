#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

class ScreenManager {
public:
    static ScreenManager& getInstance();
    void nextScreen();
    void previousScreen();
    void switchScreen(int screen);
    void init();

private:
    ScreenManager() : currentScreen(0) {}
    ScreenManager(const ScreenManager&) = delete;
    ScreenManager& operator=(const ScreenManager&) = delete;
    
    int currentScreen;
};

#endif