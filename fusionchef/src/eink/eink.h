#ifndef EINK_H
#define EINK_H
#include<Arduino.h>
#include <Adafruit_GFX.h>
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "../variant/seeed_xiao_esp32s3.h"




void eink_setup();
void drawChart();
void drawCoordinatesExplanation();
void drawSplitScreen();
int calculateBezierPoint(int p0, int p1, int p2, int p3, float t);

#endif