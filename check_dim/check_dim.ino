#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

SoftwareSerial espSerial(6, 7); // RX, TX (changed from 2, 3 to 6, 7 to avoid conflict with IR_PIN_A1 and IR_PIN_A2)

Adafruit_SSD1306 display(128, 64, &Wire, -1); // OLED display object

void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);

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

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1); // Halt the program if display initialization fails
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setRotation(2);
}

void loop() {
  // Read light level from LDR sensor
  int lightLevel = analogRead(LDR_PIN);
  Serial.print("Light Level: ");
  Serial.println(lightLevel);

  int dim = 20;

  // Initialize count variables
  int countA = 0;
  int countB = 0;

  // Check if the light level is above the threshold
  if (lightLevel > LDR_THRESHOLD) {
    // Check IR sensors and count LEDs for City A
    if (digitalRead(IR_PIN_A1) == LOW) {
      analogWrite(LED_PIN_A1, 255);
      countA++;
    } else {
      analogWrite(LED_PIN_A1, dim);
    }
    if (digitalRead(IR_PIN_A2) == LOW) {
      analogWrite(LED_PIN_A2, 255);
      countA++;
    } else {
      analogWrite(LED_PIN_A2, dim);
    }

    // Check IR sensors and count LEDs for City B
    if (digitalRead(IR_PIN_B1) == LOW) {
      analogWrite(LED_PIN_B1, 255);
      countB++;
    } else {
      analogWrite(LED_PIN_B1, dim);
    }
    if (digitalRead(IR_PIN_B2) == LOW) {
      analogWrite(LED_PIN_B2, 255);
      countB++;
    } else {
      analogWrite(LED_PIN_B2, dim);
    }

    // Check for traffic condition in City A
    static unsigned long cityATime = 0;
    if (countA == 2) { // Both LEDs in City A are ON
      if (cityATime == 0) {
        cityATime = millis(); // Start timer
      }
      if (millis() - cityATime >= 5000) { // Display "TRAFFIC" after 5 seconds
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("TRAFFIC in City A!");
        display.display();
      }
    } else {
      cityATime = 0; // Reset timer
    }
    
    // Check for traffic condition in City B
    static unsigned long cityBTime = 0;
    if (countB == 2) { // Both LEDs in City B are ON
      if (cityBTime == 0) {
        cityBTime = millis(); // Start timer
      }
      if (millis() - cityBTime >= 5000) { // Display "TRAFFIC" after 5 seconds
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("TRAFFIC in City B!");
        display.display();
      }
    } else {
      cityBTime = 0; // Reset timer
    }


    static unsigned long cityABTime = 0;
    if (countA == 2 && countB==2) { // Both LEDs in City A are ON
      if (cityABTime == 0) {
        cityABTime = millis(); // Start timer
      }
      if (millis() - cityABTime >= 5000) { // Display "TRAFFIC" after 5 seconds
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("TRAFFIC in Both Cities!!");
        display.display();
      }
    } else {
      cityABTime = 0; // Reset timer
    }

    // Check for "no traffic" condition
    if (countA != 2 && countB != 2) {
      // Clear the OLED display if neither City A nor City B is in "TRAFFIC" state
      display.clearDisplay();
      display.display();
    }

  } else {
    // If the light level is below the threshold, turn off all LEDs and clear OLED display
    analogWrite(LED_PIN_A1, 0);
    analogWrite(LED_PIN_A2, 0);
    analogWrite(LED_PIN_B1, 0);
    analogWrite(LED_PIN_B2, 0);
    display.clearDisplay();
    display.display();
  }

  Serial.print("Count for City A: ");
  Serial.println(countA);
  Serial.print("Count for City B: ");
  Serial.println(countB);

  // Send data to the ESP32
  espSerial.print(lightLevel); // Send the count of lights glowing for City A
  espSerial.print(" ");    // Separate the fields
  espSerial.print(countA); // Send the count of lights glowing for City A
  espSerial.print(" ");    // Separate the fields
  espSerial.print(countB); // Send the count of lights glowing for City B
  espSerial.print(" ");

  // Debugging output
  Serial.print("Transmitted Data: ");
  Serial.print(countA);
  Serial.print(" ");
  Serial.print(countB);
  Serial.print(" ");
  Serial.println(lightLevel);

  delay(1000); // Add a delay for the next loop iteration
}
