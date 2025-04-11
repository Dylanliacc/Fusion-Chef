#include "eink.h"
#include "../weather/weather_service.h"
#include "../network/wifi_manager.h"
#include <U8g2_for_Adafruit_GFX.h>
#include <time.h>

extern GxEPD2_BW<GxEPD2_420_GDEY042T81, GxEPD2_420_GDEY042T81::HEIGHT> display;
extern U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

void drawWeatherPage() {
    display.fillScreen(GxEPD_WHITE);
    
    // Header
    u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312);
    u8g2Fonts.setCursor(10, 30);
    u8g2Fonts.print("天气信息");
    
    // Draw divider
    display.drawFastHLine(0, 40, display.width(), GxEPD_BLACK);
    
    // Get latest weather data
    WeatherData weather = WeatherService::getInstance().getLatestWeatherData();
    
    // Weather info section
    u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312);
    u8g2Fonts.setCursor(20, 80);
    u8g2Fonts.print(weather.city);
    
    u8g2Fonts.setCursor(20, 110);
    u8g2Fonts.print("温度: ");
    u8g2Fonts.print(weather.temperature, 1);
    u8g2Fonts.print("°C");
    
    u8g2Fonts.setCursor(20, 140);
    u8g2Fonts.print("湿度: ");
    u8g2Fonts.print(weather.humidity, 0);
    u8g2Fonts.print("%");
    
    u8g2Fonts.setCursor(20, 170);
    u8g2Fonts.print("天气: ");
    u8g2Fonts.print(weather.description);
    
    // Update time
    u8g2Fonts.setCursor(20, 200);
    u8g2Fonts.print("更新时间: ");
    u8g2Fonts.print(weather.updateTime);
    
    display.nextPage();
}

void drawCalendarPage() {
    display.fillScreen(GxEPD_WHITE);
    
    // Header
    u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312);
    u8g2Fonts.setCursor(10, 30);
    u8g2Fonts.print("日历");
    
    // Draw divider
    display.drawFastHLine(0, 40, display.width(), GxEPD_BLACK);
    
    // Get current time
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    
    // Display date and time
    char dateStr[32];
    sprintf(dateStr, "%d年%d月%d日", 
            timeinfo.tm_year + 1900, 
            timeinfo.tm_mon + 1, 
            timeinfo.tm_mday);
    
    u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312);
    u8g2Fonts.setCursor(20, 80);
    u8g2Fonts.print(dateStr);
    
    char timeStr[32];
    sprintf(timeStr, "%02d:%02d:%02d", 
            timeinfo.tm_hour,
            timeinfo.tm_min,
            timeinfo.tm_sec);
    
    u8g2Fonts.setCursor(20, 110);
    u8g2Fonts.print(timeStr);
    
    // Week day
    const char* weekdays[] = {"周日", "周一", "周二", "周三", "周四", "周五", "周六"};
    u8g2Fonts.setCursor(20, 140);
    u8g2Fonts.print(weekdays[timeinfo.tm_wday]);
    
    display.nextPage();
}

void drawWifiInfoPage() {
    display.fillScreen(GxEPD_WHITE);
    
    // Header
    u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312);
    u8g2Fonts.setCursor(10, 30);
    u8g2Fonts.print("网络状态");
    
    // Draw divider
    display.drawFastHLine(0, 40, display.width(), GxEPD_BLACK);
    
    u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312);
    
    // Connection status
    u8g2Fonts.setCursor(20, 80);
    if (WifiManager::getInstance().isConnected()) {
        u8g2Fonts.print("已连接");
        
        // SSID
        u8g2Fonts.setCursor(20, 110);
        u8g2Fonts.print("网络: ");
        u8g2Fonts.print(WiFi.SSID());
        
        // IP Address
        u8g2Fonts.setCursor(20, 140);
        u8g2Fonts.print("IP: ");
        u8g2Fonts.print(WifiManager::getInstance().getLocalIP().toString());
        
        // Signal strength
        u8g2Fonts.setCursor(20, 170);
        u8g2Fonts.print("信号强度: ");
        u8g2Fonts.print(WiFi.RSSI());
        u8g2Fonts.print(" dBm");
    } else {
        u8g2Fonts.print("未连接");
    }
    
    display.nextPage();
}

void drawLongTextPage(const String& content, const String& title, int parentPage, int subPage, int totalPages) {
    display.fillScreen(GxEPD_WHITE);
    
    // Header
    u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312);
    u8g2Fonts.setCursor(10, 30);
    u8g2Fonts.print(title);
    
    // Draw divider
    display.drawFastHLine(0, 40, display.width(), GxEPD_BLACK);
    
    // Display current page content
    u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312);
    display.setTextWrap(true);
    
    // Word wrap and display text
    int y = 70;
    int lineStart = 0;
    int maxWidth = display.width() - 40; // Leave margins
    String currentLine;
    
    for (int i = 0; i < content.length(); i++) {
        currentLine = content.substring(lineStart, i + 1);
        int16_t x1, y1;
        uint16_t w, h;
        display.getTextBounds(currentLine.c_str(), 0, 0, &x1, &y1, &w, &h);
        
        if (w > maxWidth || content[i] == '\n') {
            // Draw the line
            u8g2Fonts.setCursor(20, y);
            u8g2Fonts.print(content.substring(lineStart, i));
            lineStart = i;
            y += LINE_HEIGHT;
        }
    }
    // Print remaining text
    if (lineStart < content.length()) {
        u8g2Fonts.setCursor(20, y);
        u8g2Fonts.print(content.substring(lineStart));
    }
    
    // Draw page indicator
    String pageIndicator = String(subPage + 1) + "/" + String(totalPages);
    u8g2Fonts.setCursor(display.width() - 40, display.height() - 20);
    u8g2Fonts.print(pageIndicator);
    
    // Draw parent page number if provided
    if (parentPage >= 0) {
        String parentInfo = String(parentPage);
        u8g2Fonts.setCursor(20, display.height() - 20);
        u8g2Fonts.print(parentInfo);
    }
    
    display.nextPage();
}