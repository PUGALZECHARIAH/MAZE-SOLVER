
#include "Arduino.h"
#include "motors.h"

#include <EEPROM.h>

#define EEPROM_SIZE 64


void task1(void *parameter) {
  // init_interupts();

  off_sensors();   //off all sensors
  init_sensors();  //init all sensors

  while (true) {
    update_s();
    // Serial.print(left_v);
    // Serial.print(" ");
    // Serial.print(front_v);
    // Serial.print(" ");
    // Serial.println(right_v);
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void task2(void *parameter) {
  init_motors();

  // init_interupts();

  pinMode(34, INPUT);
  pinMode(35, INPUT);

  // while (true) {
  //   while (!digitalRead(34) && !digitalRead(35)){
  //     Serial.print("");
  //   }
  //     ;
  //   if (digitalRead(34)) {
  //     // Serial.println("right_algo");
  //     balance_r();
  //   }

  //   if (digitalRead(35)) {
  //     // Serial.println("left_algo");
  //     balance_l();
  //   }

 balance_r();

  if (digitalRead(34) && digitalRead(35)) {
    while (digitalRead(34) && digitalRead(35))
      ;
    if (digitalRead(35)) {
      EEPROM.write(0, 0);
      if (EEPROM.commit()) {
        Serial.println("EEPROM commit successful");
      } else {
        Serial.println("EEPROM commit failed");
      }

      Serial.println("Write as right");
    } else if (digitalRead(34)) {
      EEPROM.write(0, 1);
      if (EEPROM.commit()) {
        Serial.println("EEPROM commit successful");
      } else {
        Serial.println("EEPROM commit failed");
      }

      Serial.println("Write as left");
    }
  } else {
    if(EEPROM.read(0) == 0){
      Serial.println("right >>>");
      balance_r();
    }
    if(EEPROM.read(0) == 1){
      Serial.println("left >>>");
      balance_l();
    }
  }

  while(1)
  vTaskDelay(100000 / portTICK_PERIOD_MS);
}



void setup() {

  Serial.begin(115200);

  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("Failed to initialize EEPROM");
    return;
  }

  // Create task1 on Core 0
  xTaskCreatePinnedToCore(
    task1,     // Task function
    "Task 1",  // Name of task
    10000,     // Stack size (in words)
    NULL,      // Task input parameter
    1,         // Priority of the task
    NULL,      // Task handle
    1);        // Core where the task should run

  // Create task2 on Core 1
  xTaskCreatePinnedToCore(
    task2,     // Task function
    "Task 2",  // Name of task
    10000,     // Stack size (in words)
    NULL,      // Task input parameter
    1,         // Priority of the task
    NULL,      // Task handle
    1);        // Core where the task should run



  // while(1){
  //   update_s();
  //   Serial.print(left_v);
  //   Serial.print(" ");
  //   Serial.print(front_v);
  //   Serial.print(" ");
  //   Serial.println(right_v);
  // }
}

void loop() {
  // update_s();
  // Serial.println(left_v);
  // Serial.print(" ");
  // Serial.println(right_d);
  // delay(100);
}
