#ifndef EINK_H
#define EINK_H
#include<Arduino.h>
#include <Adafruit_GFX.h>
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "../variant/seeed_xiao_esp32s3.h"

// Display dimensions
#define DISPLAY_HEIGHT 300
#define DISPLAY_WIDTH 400
#define LINE_HEIGHT 20
#define LINES_PER_PAGE 12
#define MAX_CHARS_PER_PAGE 500  // Maximum characters that can fit on one page

void eink_setup();
void drawChart();
void drawCoordinatesExplanation();
void drawSplitScreen();
int calculateBezierPoint(int p0, int p1, int p2, int p3, float t);

void drawWeatherPage();
void drawCalendarPage();
void drawWifiInfoPage();
void drawLongTextPage(const String& content, const String& title, int parentPage, int subPage, int totalPages);

#endif