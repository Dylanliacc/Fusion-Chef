#include <Arduino.h>
#include "./eink/eink.h"

void setup()
{
  Serial.begin(115200);
  delay(10);
  randomSeed(analogRead(0)); // 初始化随机数种子
  eink_setup();
}

int currentScreen = 0;  // 当前屏幕索引
void loop()
{
  delay(1000); // 每10秒切换一次屏幕
  // 随机选择下一个屏幕（0, 1, 2）
  currentScreen = random(3);
  // 根据当前屏幕索引调用对应的绘制函数
  switch (currentScreen)
  {
  case 0:
    drawChart();
    break;
  case 1:
    drawCoordinatesExplanation();
    break;
  case 2:
    drawSplitScreen();
    break;
  }
}
