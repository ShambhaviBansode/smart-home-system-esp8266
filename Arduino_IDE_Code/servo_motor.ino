#include <Servo.h>
#include <math.h>

// Thermistor config
#define THERMISTOR_PIN A0
#define SERIES_RESISTOR 10000
#define NOMINAL_RESISTANCE 10000
#define NOMINAL_TEMPERATURE 25
#define B_COEFFICIENT 3950
#define TEMP_THRESHOLD 30.0  // Fan ON temp

#define SERVO_PIN D2

Servo myServo;
bool fanIsOn = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  myServo.attach(SERVO_PIN);
  myServo.write(0);  // Ensure fan is off
  Serial.println("🌡️ Thermistor + Servo Fan Control Started");
}

void loop() {
  int analogValue = analogRead(THERMISTOR_PIN);

  // Avoid invalid values
  if (analogValue == 0 || analogValue >= 1023) {
    Serial.println("⚠️ Invalid analog reading");
    delay(1000);
    return;
  }

  float voltage = analogValue / 1023.0;
  float resistance = SERIES_RESISTOR * (1.0 / voltage - 1.0);

  float steinhart = resistance / NOMINAL_RESISTANCE;
  steinhart = log(steinhart);
  steinhart /= B_COEFFICIENT;
  steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15);
  steinhart = 1.0 / steinhart;
  float temperatureC = steinhart - 173.15;

  Serial.print("Temperature: ");
  Serial.print(temperatureC, 2);
  Serial.print(" °C → ");

  if (temperatureC > TEMP_THRESHOLD) {
    Serial.println("🌀 Fan ON");
    if (!fanIsOn) {
      myServo.write(180);   // Turn fan ON (clockwise)
      fanIsOn = true;
    }
  } else {
    Serial.println("❄️ Fan OFF");
    if (fanIsOn) {
      myServo.write(0);     // Turn fan OFF (reset)
      fanIsOn = false;
    }
  }

  delay(1000);
}
