#include "menu.h"
#include "display.h"
#include "time_manager.h"
#include "alarm_manager.h"
#include "sensors.h"

// Modes variables
int current_mode = 0;
String modes[] = {
  "1 - Set timezone",
  "2 - Set time",
  "3 - Set Alarm 1",
  "4 - Set Alarm 2",
  "5 - Disable Alarms",
  "6 - View Active Alarms",
  "7 - Delete Alarm"
};
int max_modes = sizeof(modes)/sizeof(modes[0]);

int wait_for_button_press(){
  while(true){
    if(digitalRead(PB_UP) == LOW){
      delay(200);
      return PB_UP;
    } else if (digitalRead(PB_DOWN) == LOW){
      delay(200);
      return PB_DOWN;
    } else if (digitalRead(PB_OK) == LOW){
      delay(200);
      return PB_OK;
    } else if (digitalRead(PB_CANCEL) == LOW){
      delay(200);
      return PB_CANCEL;
    }

    update_time();
  }
}

void go_to_menu(){
  while(digitalRead(PB_CANCEL) == HIGH){
    display.clearDisplay();
    print_line(modes[current_mode], 0, 0, 2);

    int pressed = wait_for_button_press();

    switch(pressed){
      case(PB_UP):
        delay(200);
        display.clearDisplay();
        current_mode = (current_mode + 1) % max_modes;
        break;
      case(PB_DOWN):
        delay(200);
        current_mode = (current_mode - 1 + max_modes) % max_modes;
        break;
      case(PB_OK):
        delay(200);
        run_mode(current_mode);
        break;
      case(PB_CANCEL):
        delay(200);
        break;
    }
  }
}

void run_mode(int mode){
  switch(mode){
    case 0:
      set_timezone();
      break;
    case 1:
      set_time();
      break;
    case 2:
    case 3:
      set_alarm(mode-2);
      break;
    case 4:
      alarm_enabled = false;
      display.clearDisplay();
      print_line("Disabled Alarms", 0, 0, 2);
      break;
    case 5:
      view_active_alarms();
      break;
    case 6:
      delete_alarm();
  }
}