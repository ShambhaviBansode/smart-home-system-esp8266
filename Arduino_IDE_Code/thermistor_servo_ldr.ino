#include <Servo.h>
#include <math.h>

// --- Thermistor Config ---
#define THERMISTOR_PIN A0          // Analog pin (shared with LDR)
#define SERIES_RESISTOR 10000
#define NOMINAL_RESISTANCE 10000
#define NOMINAL_TEMPERATURE 25
#define B_COEFFICIENT 3950
#define TEMP_THRESHOLD 30.0        // Fan ON temp

#define SERVO_PIN D2               // Fan (servo) control
#define SERVO_STOP 90              // Stop for continuous servo
#define SERVO_ON   180             // Fan ON rotation

// --- LDR Config ---
#define LDR_PIN A0                 // Same analog pin
#define LED_PIN D4                // LED output pin
#define LIGHT_THRESHOLD 700        // LDR threshold for darkness

Servo myServo;
bool fanIsOn = false;

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);      // LED off initially

  myServo.attach(SERVO_PIN);
  myServo.write(SERVO_STOP);       // Stop fan initially

  Serial.println("✅ System Ready: Thermistor + Servo + LDR + LED");
}

void loop() {
  // --- Thermistor Section ---
  int analogValue = analogRead(THERMISTOR_PIN);  // First read: thermistor

  if (analogValue > 0 && analogValue < 1023) {
    float voltage = analogValue / 1023.0;
    float resistance = SERIES_RESISTOR * (1.0 / voltage - 1.0);

    float steinhart = resistance / NOMINAL_RESISTANCE;
    steinhart = log(steinhart);
    steinhart /= B_COEFFICIENT;
    steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15);
    steinhart = 1.0 / steinhart;
    float temperatureC = steinhart - 373.15;

    Serial.print("🌡 Temperature: ");
    Serial.print(temperatureC, 2);
    Serial.print(" °C → ");

    if (temperatureC > TEMP_THRESHOLD) {
      Serial.println("🌀 Fan ON");
      if (!fanIsOn) {
        myServo.write(SERVO_ON);   // Turn fan ON (clockwise)
        fanIsOn = true;
      }
    } else {
      Serial.println("❄️ Fan OFF");
      if (fanIsOn) {
        myServo.write(SERVO_STOP); // Stop fan (neutral 90°)
        fanIsOn = false;
      }
    }
  } else {
    Serial.println("⚠️ Invalid thermistor reading");
  }

  delay(500);  // Small delay between sensor sections

  // --- LDR Section ---
  int lightLevel = analogRead(LDR_PIN);  // Second read: LDR
  Serial.print("💡 Light Level: ");
  Serial.println(lightLevel);

  if (lightLevel < LIGHT_THRESHOLD) {
    digitalWrite(LED_PIN, HIGH); // LED ON in darkness
    Serial.println("🌑 It's dark — LED ON");
  } else {
    digitalWrite(LED_PIN, LOW);  // LED OFF in bright light
    Serial.println("☀️ It's bright — LED OFF");
  }

  delay(1000); // Main loop delay
}
