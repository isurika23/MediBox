#include "display.h"

void initDisplay() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)){
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.display();
  delay(2000);
}

void initWiFi() {
  WiFi.begin("Wokwi-GUEST", "", 6);
  while(WiFi.status() != WL_CONNECTED){
    delay(250);
    display.clearDisplay();
    print_line("Connecting to WiFi...", 0, 0, 2);
    delay(500);
  }

  display.clearDisplay();
  print_line("Connected to Wifi", 0, 0, 2);
  delay(1000);
}

void print_line(String text, int column, int row, int textSize){
  display.setTextSize(textSize);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(column,row);
  display.println(text);

  display.display();
}