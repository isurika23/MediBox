#ifndef MENU_H
#define MENU_H

#include "config.h"

// Menu function prototypes
int wait_for_button_press();
void go_to_menu();
void run_mode(int mode);

// Modes variables
extern int current_mode;
extern String modes[];
extern int max_modes;

#endif // MENU_H