#include <TFT_eSPI.h>
#include <Wire.h>
#include <WiFi.h>

TFT_eSPI tft = TFT_eSPI();

const char* ssid = "Enter SSID";
const char* password = "Enter Password";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 0;

bool screenBlack = false;

void drawPage1(struct tm* timeinfo) {
  if (screenBlack) {
    tft.fillScreen(TFT_BLACK);
    return;
  }

  tft.fillScreen(TFT_BLACK);

  char dateStr[10];
  strftime(dateStr, sizeof(dateStr), "%a %d", timeinfo);
  tft.setTextColor(tft.color565(234, 53, 107), TFT_BLACK);
  tft.drawCentreString(dateStr, 115, 30, 4);

  char timeStr[6];
  strftime(timeStr, sizeof(timeStr), "%H:%M", timeinfo);
  tft.setTextColor(tft.color565(255, 255, 255), TFT_BLACK);
  tft.drawCentreString(timeStr, 115, 80, 7);

  tft.setTextColor(tft.color565(116, 251, 241), TFT_BLACK);
  tft.drawCentreString("4°C", 80, 150, 4);

  tft.setTextColor(tft.color565(117, 250, 99), TFT_BLACK);
  tft.drawCentreString("2025", 160, 150, 4);

  tft.fillTriangle(110, 220, 130, 220, 120, 230, tft.color565(255, 255, 255));
}

void setup() {
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    drawPage1(&timeinfo);
  }
}

void loop() {
  static time_t lastUpdate = 0;
  time_t now = time(nullptr);

  if (now != lastUpdate) {
    lastUpdate = now;
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      drawPage1(&timeinfo);
    }
  }
}
