#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "../variant/seeed_xiao_esp32s3.h"
#include "../lib/pic/setup.h"
#include "eink.h"


GxEPD2_BW<GxEPD2_420_GDEY042T81, GxEPD2_420_GDEY042T81::HEIGHT> 
display(GxEPD2_420_GDEY042T81(/*CS=D1*/ EPD_CS, /*DC=D3*/EPD_DC, /*RST=D0*/ EPD_RST, /*BUSY=D2*/ EPD_BUSY));  // 4.2黑白   GDEY042T81   300*400, SSD1608 (IL3820)

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;
void eink_setup(){
  SPI.end();                
  SPI.begin();  // use VSPI

  pinMode(20, OUTPUT);
  digitalWrite(20, HIGH);

  display.init(115200, true, 2, false);
  display.setRotation(0);  // 0  is horz
  u8g2Fonts.begin(display);
  u8g2Fonts.setFontDirection(0);
  u8g2Fonts.setForegroundColor(GxEPD_BLACK);  
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);  


    // Display the startup image
  display.fillScreen(GxEPD_WHITE);  // Clear the screen to white
  display.firstPage();
  do{
  display.drawBitmap(0, 0, gImage_01, 400, 300, GxEPD_BLACK);  // Draw the image at (0,0)
  }while( display.nextPage()) ;// Perform a full refresh to show the image)
  delay(5000);  // Wait 3 seconds to let the user view the image

  
  u8g2Fonts.setFontMode(1);
  u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312);
  drawChart();
}


// 计算贝塞尔曲线上的点的函数
int calculateBezierPoint(int p0, int p1, int p2, int p3, float t) {
  float u = 1 - t;
  float tt = t * t;
  float uu = u * u;
  float uuu = uu * u;
  float ttt = tt * t;

  int point = uuu * p0;
  point += 3 * uu * t * p1;
  point += 3 * u * tt * p2;
  point += ttt * p3;

  return round(point);
}


void drawChart() {
  display.fillScreen(GxEPD_WHITE);  // 清空屏幕
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);
  int temperatures[] = { 5, 15, 20, 25, 10, 35 };
  int numDays = sizeof(temperatures) / sizeof(temperatures[0]);

  // 确定坐标轴范围
  int minTemp = 0;
  int maxTemp = 40;
  int minY = 20;                     // 留出顶部空间显示标题
  int maxY = display.height() - 10;  // 留出底部空间显示横坐标刻度
  u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312);
  u8g2Fonts.setCursor(10, 30);
  u8g2Fonts.print("温度图表");
  // 绘制纵坐标刻度和显示数值
  for (int i = 0; i <= 8; i++) {
    int y = map(i * 5, minTemp, maxTemp, maxY, minY);
    display.drawFastHLine(0, y, 5, GxEPD_BLACK);
    display.setCursor(5, y - 6);
    //display.print(i * 5);
  }

  // 绘制横坐标刻度
  for (int i = 0; i < numDays; i++) {
    int x = map(i, 0, numDays - 1, 0, display.width() - 1);
    display.drawFastVLine(x, maxY, 5, GxEPD_BLACK);
    if (x < display.width() - 10) {  // 只在屏幕内部显示横坐标数值
      display.setCursor(x - 6, maxY + 15);
      //display.print(i + 1);
    }
  }

  /// 绘制折线和模拟贝塞尔曲线的过渡段
  for (int i = 0; i < numDays - 1; i++) {
    int x1 = map(i, 0, numDays - 1, 0, display.width() - 1);
    int x2 = map(i + 1, 0, numDays - 1, 0, display.width() - 1);
    int y1 = map(temperatures[i], minTemp, maxTemp, maxY, minY);
    int y2 = map(temperatures[i + 1], minTemp, maxTemp, maxY, minY);

    // 添加贝塞尔曲线的控制点
    int cx1 = x1 + (x2 - x1) / 3;
    int cy1 = y1 + 5;
    int cx2 = x1 + 2 * (x2 - x1) / 3;
    int cy2 = y2 + 5;



    // 绘制模拟贝塞尔曲线的过渡段
    int prevX, prevY;
    for (int j = 0; j <= 10; j++) {
      float t = j / 10.0;
      int bx = calculateBezierPoint(x1, cx1, cx2, x2, t);
      int by = calculateBezierPoint(y1, cy1, cy2, y2, t);
      display.drawPixel(bx, by, GxEPD_BLACK);
      if (j > 0) {
        display.drawLine(prevX, prevY, bx, by, GxEPD_BLACK);
      }
      display.drawFastVLine(bx, by, 128 - by, GxEPD_BLACK);//填充竖线

      display.fillCircle(x1, y1, 3, GxEPD_BLACK);//转折点画圆
      display.fillCircle(x1, y1, 3, GxEPD_WHITE);//转折点白色圆模拟虚线

      display.setCursor(x1 - 12, y1 -7);//绘制温度数值
      display.print(temperatures[i]);

      prevX = bx;
      prevY = by;
    }
   
  }


  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);
  display.setCursor(10, 15);
  display.println("Temperature Chart");

  display.nextPage();  // 显示内容
}


// 第二个屏幕：横纵坐标对应关系
void drawCoordinatesExplanation() {
  display.fillScreen(GxEPD_WHITE);  // 清空屏幕
  u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312);
  u8g2Fonts.setCursor(10, 30);
  u8g2Fonts.print("横坐标：天数（1-6）");
  u8g2Fonts.setCursor(10, 50);
  u8g2Fonts.print("纵坐标：温度（°C）");

  int temperatures[] = { 5, 15, 20, 25, 10, 35 };
  for (int i = 0; i < 6; i++) {
    u8g2Fonts.setCursor(10, 70 + i * 20);
    u8g2Fonts.print("第");
    u8g2Fonts.print(i + 1);
    u8g2Fonts.print("天：");
    u8g2Fonts.print(temperatures[i]);
    u8g2Fonts.print("°C");
  }

  display.nextPage();  // 刷新显示
}

// 第三个屏幕：1:3分屏的横线
void drawSplitScreen() {
  display.fillScreen(GxEPD_WHITE);  // 清空屏幕
  int splitY = display.height() / 4;  // 1:3分屏，上1/4，下3/4

  // 绘制分隔横线
  display.drawFastHLine(0, splitY, display.width(), GxEPD_BLACK);

  // 上部分文本
  u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312);
  u8g2Fonts.setCursor(10, splitY / 2);
  u8g2Fonts.print("上部分：一些中文文本");

  // 下部分文本
  u8g2Fonts.setCursor(10, splitY + 20);
  u8g2Fonts.print("下部分：更多的中文文本");
  u8g2Fonts.setCursor(10, splitY + 40);
  u8g2Fonts.print("可以在这里添加更多内容");

  display.nextPage();  // 刷新显示
}