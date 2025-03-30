#ifndef ALARM_MANAGER_H
#define ALARM_MANAGER_H

#include "config.h"

// Forward declaration for button press function
// (to avoid circular dependency)
int wait_for_button_press();

// Alarm function prototypes
void ring_alarm(int alarm);
void set_alarm(int alarm_number);
void view_active_alarms();
void delete_alarm();

// Alarm global variables
extern bool alarm_enabled;
extern int n_alarm;
extern int alarm_hours[];
extern int alarm_minutes[];
extern bool alarm_triggered[];

#endif // ALARM_MANAGER_H