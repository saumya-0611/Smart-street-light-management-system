#include <SoftwareSerial.h>

// Define IR sensor and LED pins for City A and City B
const int IR_PIN_A1 = 2;
const int IR_PIN_A2 = 3;
const int IR_PIN_B1 = 4;
const int IR_PIN_B2 = 5;
const int LED_PIN_A1 = 9;
const int LED_PIN_A2 = 10;
const int LED_PIN_B1 = 11;
const int LED_PIN_B2 = 13;
const int LDR_PIN = A0;
const int LDR_THRESHOLD = 700;

SoftwareSerial espSerial(2, 3); // RX, TX

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);

  // Initialize pins
  pinMode(LDR_PIN, INPUT);
  pinMode(IR_PIN_A1, INPUT);
  pinMode(IR_PIN_A2, INPUT);
  pinMode(IR_PIN_B1, INPUT);
  pinMode(IR_PIN_B2, INPUT);
  pinMode(LED_PIN_A1, OUTPUT);
  pinMode(LED_PIN_A2, OUTPUT);
  pinMode(LED_PIN_B1, OUTPUT);
  pinMode(LED_PIN_B2, OUTPUT);
}

void loop() {
  // Read light level from LDR sensor
  int lightLevel = analogRead(LDR_PIN);
  Serial.print("Light Level: ");
  Serial.println(lightLevel);

  int countA = 0;
  int countB = 0;
  // Check if the light level is above the threshold
  if (lightLevel > LDR_THRESHOLD) {
      // Initialize count variables


    // Check IR sensors for City A
    if (digitalRead(IR_PIN_A1) == LOW) {
      analogWrite(LED_PIN_A1, 255);
      countA++;
      while (digitalRead(IR_PIN_A1) == LOW) {
       delay(100);
       }
       
      for (int dim = 255; dim >= 20; dim--) {
        analogWrite(LED_PIN_A1, dim);
        delay(60);
        }
    } else{
      analogWrite(LED_PIN_A1,20);
      }
    
    if (digitalRead(IR_PIN_A2) == LOW) {
      analogWrite(LED_PIN_A2, 255);
      countA++;
      while (digitalRead(IR_PIN_A2) == LOW) {
       delay(100);
       }
       
      for (int dim = 255; dim >= 20; dim--) {
        analogWrite(LED_PIN_A2, dim);
        delay(60);
        }
    } else{
      analogWrite(LED_PIN_A2,20);
      }

    // Check IR sensors for City B
    if (digitalRead(IR_PIN_B1) == LOW) {
      analogWrite(LED_PIN_B1, 255);
      countB++;
      while (digitalRead(IR_PIN_B1) == LOW) {
       delay(100);
       }
       
      for (int dim = 255; dim >= 20; dim--) {
        analogWrite(LED_PIN_B1, dim);
        delay(60);
        }
    } else{
      analogWrite(LED_PIN_B1,20);
      }

    
    if (digitalRead(IR_PIN_B2) == LOW) {
      analogWrite(LED_PIN_B2, 255);
      countB++;
      while (digitalRead(IR_PIN_B2) == LOW) {
       delay(100);
       }
       
      for (int dim = 255; dim >= 20; dim--) {
        analogWrite(LED_PIN_B2, dim);
        delay(60);
        }
    } else{
      analogWrite(LED_PIN_B2,20);
      }
  } else {
    // If the light level is below the threshold, turn off all LEDs
    analogWrite(LED_PIN_A1, 0);
    analogWrite(LED_PIN_A2, 0);
    analogWrite(LED_PIN_B1, 0);
    analogWrite(LED_PIN_B2, 0);
  }

  Serial.print("Count for City A: ");
  Serial.println(countA);
  Serial.print("Count for City B: ");
  Serial.println(countB);

  // Send data to the ESP32
  espSerial.print(countA); // Send the count of lights glowing for City A
  espSerial.print("\n");    // Separate the fields
  espSerial.print(lightLevel); // Send the LDR intensity for City A
  espSerial.print("\n");

  espSerial.print(countB); // Send the count of lights glowing for City B
  espSerial.print("\n");    // Separate the fields
  espSerial.print(lightLevel); // Send the LDR intensity for City B
  espSerial.print("\n");

  delay(1000); // Add a delay for the next loop iteration
}
