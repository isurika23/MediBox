#include "config.h"
#include "display.h"
#include "time_manager.h"
#include "alarm_manager.h"
#include "menu.h"
#include "sensors.h"

void setup() {
  Serial.begin(9600);

  initPins();
  initDisplay();
  initWiFi();
  initTime();
  
  display.clearDisplay();
  print_line("Welcome to MediBox", 10, 10, 2);
  delay(1000);
  display.clearDisplay();

  print_time_now();
  Serial.println("Max_modes: " + String(max_modes));
}

void loop() {
  update_time_with_check_alarm();

  if(digitalRead(PB_OK) == LOW){
    delay(200);
    go_to_menu();
  }

  check_temp();
}

void initPins() {
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(PB_CANCEL, INPUT);
  pinMode(PB_OK, INPUT);
  pinMode(PB_UP, INPUT);
  pinMode(PB_DOWN, INPUT);

  dhtSensor.setup(DHT, DHTesp::DHT22);
  digitalWrite(LED_1, LOW);
}