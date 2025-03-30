#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include "config.h"

// Forward declaration for button press function 
// (to avoid circular dependency)
int wait_for_button_press();

// Time function prototypes
void print_time_now();
void update_time();
void update_time_with_check_alarm();
void initTime();
void set_time();
void set_timezone();

// Time global variables
extern int days;
extern int hours;
extern int minutes;
extern int seconds;

extern unsigned long timeNow;
extern unsigned long timeLast;

#endif // TIME_MANAGER_H