#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHTesp.h"
#include "WiFi.h"

// Pin definitions
#define SCREEN_WIDTH 128
#define SCREEN_HIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define BUZZER 5
#define LED_1 15
#define PB_CANCEL 34
#define PB_OK 32
#define PB_UP 33
#define PB_DOWN 35
#define DHT 12

// Time server definitions
#define NTP_SERVER     "pool.ntp.org"
#define UTC_OFFSET_DST 0 // UTC offset in hours for DST

// Buzzer notes
extern int C;
extern int D;
extern int E;
extern int F;
extern int G;
extern int A;
extern int B;
extern int C_H;
extern int notes[];
extern int n_notes;

// Global variables and objects
extern Adafruit_SSD1306 display;
extern DHTesp dhtSensor;
extern float UTC_OFFSET;

#endif // CONFIG_H