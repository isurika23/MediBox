#include "sensors.h"
#include "display.h"

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