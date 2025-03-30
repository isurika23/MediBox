#include "time_manager.h"
#include "display.h"
#include "alarm_manager.h"

// Time global variables
int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;

unsigned long timeNow = 0;
unsigned long timeLast = 0;

void initTime() {
  configTime(UTC_OFFSET*3600, UTC_OFFSET_DST, NTP_SERVER);
}

void print_time_now(){
  display.clearDisplay();
  
  print_line(String(days), 0, 0, 2);
  print_line(":", 20, 0, 2);
  print_line(String(hours), 30, 0, 2);
  print_line(":", 50, 0, 2);
  print_line(String(minutes), 60, 0, 2);
  print_line(":", 80, 0, 2);
  print_line(String(seconds), 90, 0, 2);
}

void update_time(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    display.clearDisplay();
    print_line("Connection Err", 0, 0, 2);
    delay(1000);
    return;
  }

  char time_hour[3];
  char time_minute[3];
  char time_second[3];
  char time_day[3];

  strftime(time_hour, sizeof(time_hour), "%H", &timeinfo);
  strftime(time_minute, sizeof(time_minute), "%M", &timeinfo);
  strftime(time_second, sizeof(time_second), "%S", &timeinfo);
  strftime(time_day, sizeof(time_day), "%d", &timeinfo);

  hours = atoi(time_hour);
  minutes = atoi(time_minute);
  seconds = atoi(time_second);
  days = atoi(time_day);
}

void update_time_with_check_alarm(){
  update_time();
  print_time_now();

  if(alarm_enabled){
    for(int i = 0; i < n_alarm; i++){
      if(!alarm_triggered[i] && alarm_hours[i] == hours && alarm_minutes[i] == minutes){
        ring_alarm(i);
      }
    }
  }
}

void set_time(){
  int temp_hour = hours;
  int temp_minute = minutes;

  while(true){
    display.clearDisplay();
    print_line("Enter hour: "+String(temp_hour), 0, 0, 2);

    int pressed = wait_for_button_press();
    switch(pressed){
      case(PB_UP):
        delay(200);
        temp_hour = (temp_hour + 1) % 24;
        break;
      case(PB_DOWN):
        delay(200);
        temp_hour = (temp_hour - 1 + 24) % 24;
        break;
      case(PB_OK):
        delay(200);
        hours = temp_hour;
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
    print_line("Enter minute: "+String(temp_minute), 0, 0, 2);

    int pressed = wait_for_button_press();
    switch(pressed){
      case(PB_UP):
        delay(200);
        temp_minute = (temp_minute + 1) % 60;
        break;
      case(PB_DOWN):
        delay(200);
        temp_minute = (temp_minute - 1 + 60) % 60;
        break;
      case(PB_OK):
        delay(200);
        minutes = temp_minute;
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
  
  display.clearDisplay();
  print_line("Time is set", 0, 0, 2);
  delay(1000);

  return;
}

void set_timezone(){
  float temp_offset = UTC_OFFSET;

  while(true){
    display.clearDisplay();
    print_line("UTC + "+String(temp_offset, 1), 0, 0, 2);

    int pressed = wait_for_button_press();
    switch(pressed){
      case(PB_UP):
        delay(200);
        temp_offset += 0.5;
        if (temp_offset > 14.0){
          temp_offset = -12.0;
        }
        break;
      case(PB_DOWN):
        delay(200);
        temp_offset -= 0.5;
        if(temp_offset < -12.0){
          temp_offset = 14.0;
        }
        break;
      case(PB_OK):
        delay(200);
        UTC_OFFSET = temp_offset;
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
  
  display.clearDisplay();
  print_line("Timezone is set", 0, 0, 2);
  delay(1000);

  configTime(UTC_OFFSET*3600, UTC_OFFSET_DST, NTP_SERVER);
  return;
}