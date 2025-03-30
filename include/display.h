#ifndef DISPLAY_H
#define DISPLAY_H

#include "config.h"

// Display function prototypes
void print_line(String text, int column, int row, int textSize);
void initDisplay();
void initWiFi();

#endif // DISPLAY_H