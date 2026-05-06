#include <Arduino.h>

#define PHOTO_SENSOR_PIN 4
#define LED_PIN 16

uint16_t light_sensor_value[10];

void setup() {
  Serial.begin(115200);

  uint32_t startLog = millis();
  while (!Serial && (millis() - startLog < 3000)) {
    delay(10);
  }

  Serial.println("\n--- Система нічного освітлення запущена ---");

  pinMode(PHOTO_SENSOR_PIN, ANALOG);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  uint32_t average_light_value = 0;

  for (int i = 0; i < 10; i++) {
    light_sensor_value[i] = analogRead(PHOTO_SENSOR_PIN);
    average_light_value += light_sensor_value[i];
    delay(10);
  }
  
  average_light_value /= 10;
  
  Serial.printf("Середнє значення світла: %u\n", average_light_value);

  // Якщо значення МАЛЕ — значить ТЕМНО
  if (average_light_value < 1000) { 
    digitalWrite(LED_PIN, HIGH); 
    Serial.println("СТАН: ТЕМНО -> LED УВІМКНЕНО");
  } else {
    digitalWrite(LED_PIN, LOW); 
    Serial.println("СТАН: СВІТЛО -> LED ВИМКНЕНО");
  }

  Serial.println("-----------------------------------");
  delay(1000); 
}