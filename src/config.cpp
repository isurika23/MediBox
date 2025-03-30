#include "config.h"

// Initialize global objects
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HIGHT, &Wire, OLED_RESET);
DHTesp dhtSensor;

// Initialize UTC offset
float UTC_OFFSET = 5.5; // UTC offset in hours

// Define buzzer notes
int C = 262;
int D = 294;
int E = 330;
int F = 349;
int G = 392;
int A = 440;
int B = 494;
int C_H = 523;
int notes[] = {C, D, E, F, G, A, B, C_H};
int n_notes = 8;