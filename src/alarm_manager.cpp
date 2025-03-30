#include "alarm_manager.h"
#include "display.h"
#include "menu.h"

// Alarm global variables
bool alarm_enabled = true;
int n_alarm = 2;
int alarm_hours[] = {0, 1};
int alarm_minutes[] = {1, 10};
bool alarm_triggered[] = {false, false};

void ring_alarm(int alarm){
  display.clearDisplay();
  print_line("Medicine Time", 0, 0, 2);

  // Turn on LED
  digitalWrite(LED_1, HIGH);

  bool alarm_break = false;

  while(!alarm_break && digitalRead(PB_OK) == HIGH && digitalRead(PB_CANCEL) == HIGH){
    for (int i= 0; i<n_notes; i++){
      if(digitalRead(PB_OK) == LOW){  // ok => stop alarm
        delay(200);
        alarm_break = true;
        alarm_triggered[alarm] = true;
        break;
      }

      if(digitalRead(PB_CANCEL) == LOW){  // cancel => snooze for 5 minutes
        delay(200);
        alarm_break = true;
        alarm_minutes[alarm] = alarm_minutes[alarm] + 5;
        if(alarm_minutes[alarm] >= 60){
          alarm_hours[alarm] = (alarm_hours[alarm] + 1) % 24;
          alarm_minutes[alarm] = (alarm_minutes[alarm] + 5) % 60;
        }
        alarm_triggered[alarm] = false;
        display.clearDisplay();
        print_line("Snoozed for 5 min", 0, 0, 2);
        delay(1000);
        break;
      }
      digitalWrite(LED_1, HIGH);
      tone(BUZZER, notes[i]);
      delay(500);
      digitalWrite(LED_1, LOW);
      noTone(BUZZER);
      delay(2);
    }
  }

  digitalWrite(LED_1, LOW);
  display.clearDisplay();
}

void set_alarm(int alarm){
  int alarm_hour = (alarm_hours[alarm] != -1) ? alarm_hours[alarm] : 0;
  int alarm_minute = (alarm_minutes[alarm] != -1) ? alarm_minutes[alarm] : 0;

  while(true){
    display.clearDisplay();
    print_line("Enter hour: "+String(alarm_hour), 0, 0, 2);

    int pressed = wait_for_button_press();
    switch(pressed){
      case(PB_UP):
        delay(200);
        alarm_hour = (alarm_hour + 1) % 24;
        break;
      case(PB_DOWN):
        delay(200);
        alarm_hour = (alarm_hour - 1 + 24) % 24;
        break;
      case(PB_OK):
        delay(200);
        alarm_hours[alarm] = alarm_hour;
        break;
      case(PB_CANCEL):
        delay(200);
        return;
    }
    if(pressed == PB_CANCEL || pressed == PB_OK){
      delay(200);
      break;
    }
  }

  while(true){
    display.clearDisplay();
    print_line("Enter minute: "+String(alarm_minute), 0, 0, 2);

    int pressed = wait_for_button_press();
    switch(pressed){
      case(PB_UP):
        delay(200);
        alarm_minute = (alarm_minute + 1) % 60;
        break;
      case(PB_DOWN):
        delay(200);
        alarm_minute = (alarm_minute - 1 + 60) % 60;
        break;
      case(PB_OK):
        delay(200);
        alarm_minutes[alarm] = alarm_minute;
        break;
      case(PB_CANCEL):
        delay(200);
        break;
    }
    if(pressed == PB_CANCEL || pressed == PB_OK){
      delay(200);
      break;
    }
  }

  alarm_enabled = true;
  
  alarm_triggered[alarm] = false;
  display.clearDisplay();
  print_line("Alarm is set", 0, 0, 2);
  delay(1000);

  return;
}

void view_active_alarms(){
  while(true){
    int active_alarms = 0;
    display.clearDisplay();
    
    for (int i = 0; i < n_alarm; i++){
      if(!alarm_enabled){
        break;
      }
      if(!alarm_triggered[i] && alarm_hours[i] != -1 && alarm_minutes[i] != -1){
        active_alarms++;
        print_line("Alarm " + String(i + 1) + ": " + String(alarm_hours[i]) + ":" + String(alarm_minutes[i]), 0, i * 10, 1);
      }
    }
    if(active_alarms == 0){
      display.clearDisplay();
      print_line("No active alarms", 0, 0, 2);
      delay(2000);
    }

    int pressed = wait_for_button_press();
    if (pressed == PB_CANCEL || pressed == PB_OK){
      delay(200);
      break;
    }
  }
  
  return;
}

void delete_alarm(){
  int alarm_to_delete = 0;
  while(true){
    display.clearDisplay();

    print_line("Delete Alarm: "+String(alarm_to_delete+1)+": "+alarm_hours[alarm_to_delete]+":"+alarm_minutes[alarm_to_delete], 0, 0, 1);

    int pressed = wait_for_button_press();
    
    switch(pressed){
      case(PB_DOWN):
        delay(200);
        alarm_to_delete = (alarm_to_delete + 1) % n_alarm;
        break;
      case(PB_UP):
        delay(200);
        alarm_to_delete = (alarm_to_delete - 1 + n_alarm) % n_alarm;
        break;
      case(PB_OK):
        delay(200);
        alarm_hours[alarm_to_delete] = -1;
        alarm_minutes[alarm_to_delete] = -1;
        display.clearDisplay();
        print_line("Deleted Alarm "+String(alarm_to_delete+1), 0, 0, 2);
        delay(1000);
        break;
      case(PB_CANCEL):
        delay(200);
        break;
    }

    if(pressed == PB_CANCEL){
      delay(200);
      break;
    }
  }
}