#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHTesp.h"
#include "WiFi.h"

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

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HIGHT, &Wire, OLED_RESET);
DHTesp dhtSensor;

// Function prototypes
void print_line(String text, int column, int row, int textSize);
void print_time_now();
void update_time();
void update_time_with_check_alarm();
void ring_alarm();
void go_to_menu();
int wait_for_button_press();
void run_mode(int mode);
void set_time();
void set_alarm(int alarm_number);
void check_temp();
void set_timezone();

// Time global variables
int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;

unsigned long timeNow = 0;
unsigned long timeLast = 0;

// Alarm global variables
bool alarm_enabled = true;
int n_alarm = 2;
int alarm_hours[] = {0, 1};
int alarm_minutes[] = {1, 10};
bool alarm_triggered[] = {false, false};

// Buzzer notes
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

// Modes variables
int current_mode = 0;
String modes[] = {
  "1 - Set timezone",
  "2 - Set time",
  "3 - Set Alarm 1",
  "4 - Set Alarm 2",
  "5 - Disable Alarms"
};
int max_modes = sizeof(modes)/sizeof(modes[0]);

// Time server variables
#define NTP_SERVER     "pool.ntp.org"
float UTC_OFFSET = 0.0; // UTC offset in hours
#define UTC_OFFSET_DST 0 // UTC offset in hours for DST

void setup() {
  Serial.begin(9600);

  pinMode(BUZZER, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(PB_CANCEL, INPUT);
  pinMode(PB_OK, INPUT);
  pinMode(PB_UP, INPUT);
  pinMode(PB_DOWN, INPUT);

  dhtSensor.setup(DHT, DHTesp::DHT22);

  digitalWrite(LED_1, LOW);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)){
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.display();
  delay(2000);

  WiFi.begin("Wokwi-GUEST", "", 6);
  while(WiFi.status() != WL_CONNECTED){
    delay(250);
    display.clearDisplay();
    print_line("Connecting to WiFi...", 0, 0, 2);
    delay(500);
  }

  display.clearDisplay();
  print_line("Connected to Wifi", 0, 0, 2);
  delay(1000);

  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);

  display.clearDisplay();

  print_line("Welcome to MediBox", 10, 10, 2);
  delay(1000);
  display.clearDisplay();

  print_time_now();
  Serial.println("Max_modes: "+String(max_modes));
  
}

void loop() {
  update_time_with_check_alarm();

  if(digitalRead(PB_OK) == LOW){
    delay(200);
    go_to_menu();
  }

  check_temp();

}

void print_line(String text, int column, int row, int textSize){

  display.setTextSize(textSize);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(column,row);
  display.println(text);

  display.display();
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

void ring_alarm(){
  display.clearDisplay();
  print_line("Medicine Time", 0, 0, 2);

  // Turn on LED
  digitalWrite(LED_1, HIGH);

  // Play buzzer
  // Serial.println(String(digitalRead(PB_CANCEL)));

  bool alarm_cancelled = false;

  while(!alarm_cancelled && digitalRead(PB_CANCEL) == HIGH){
    for (int i= 0; i<n_notes; i++){
      if(digitalRead(PB_CANCEL) == LOW){
        delay(200);
        alarm_cancelled = true;
        break;
      }
      tone(BUZZER, notes[i]);
      delay(500);
      noTone(BUZZER);
      delay(2);
    }
  }

  digitalWrite(LED_1, LOW);
  display.clearDisplay();

}

void update_time_with_check_alarm(){

  update_time();
  print_time_now();

  if(alarm_enabled){
    for(int i = 0; i < n_alarm; i++){
      if(!alarm_triggered[i] && alarm_hours[i] == hours && alarm_minutes[i] == minutes){
        // Serial.println("Alarm triggered");
        ring_alarm();
        alarm_triggered[i] = true;
      }
    }
  }
}

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
      set_alarm(mode-1);
      break;
    case 4:
      alarm_enabled = false;
      display.clearDisplay();
      print_line("Disabled Alarms", 0, 0, 2);
      break;
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
      // If the user pressed cancel or ok, break the loop
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
      // If the user pressed cancel or ok, break the loop
      delay(200);
      break;
    }
  }
  
  display.clearDisplay();
  print_line("Time is set", 0, 0, 2);
  delay(1000);

  return;
}

void set_alarm(int alarm){
  int alarm_hour = alarm_hours[alarm];
  int alarm_minute = alarm_minutes[alarm];

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
      // If the user pressed cancel or ok, break the loop
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
      // If the user pressed cancel or ok, break the loop
      delay(200);
      break;
    }
  }
  
  display.clearDisplay();
  print_line("Alarm is set", 0, 0, 2);
  delay(1000);

  return;
}

void check_temp(){
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  if (data.temperature > 32){
    display.clearDisplay();
    print_line("TEMP HIGH", 0, 40, 1);
    delay(500);
  } else if (data.temperature < 24){
    display.clearDisplay();
    print_line("TEMP LOW", 0, 40, 1);
    delay(500);
  }

  if (data.humidity > 80){
    display.clearDisplay();
    print_line("HUMIDITY HIGH", 0, 50, 1);
    delay(500);
  } else if (data.humidity < 65){
    display.clearDisplay();
    print_line("HUMIDITY LOW", 0, 50, 1);
    delay(500);
  }
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
      // If the user pressed cancel or ok, break the loop
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