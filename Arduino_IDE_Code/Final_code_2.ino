#include <Servo.h>
#include <math.h>

// --- Thermistor Setup ---
#define THERMISTOR_PIN A0
#define SERIES_RESISTOR 10000
#define NOMINAL_RESISTANCE 10000
#define NOMINAL_TEMPERATURE 25
#define B_COEFFICIENT 3950
#define TEMP_THRESHOLD 28.0

// --- Fan Servo (Temperature-based) ---
#define FAN_SERVO_PIN D2
#define FAN_STOP_ANGLE 90
#define FAN_ON_ANGLE   180
Servo fanServo;
bool fanIsOn = false;

// --- LDR + LED Setup ---
#define LDR_PIN A0             // Shared with thermistor
#define LDR_LED_PIN D4
#define LIGHT_THRESHOLD 10   // Lower = darker

// --- Touch + Servo Setup ---
#define TOUCH_PIN D5
#define TOUCH_SERVO_PIN D3
Servo touchServo;
bool touchState = false;
bool lastTouchReading = HIGH;

// --- IR Sensor + LED + Buzzer ---
#define IR_SENSOR_PIN D6
#define IR_LED_PIN    D7
#define BUZZER_PIN    D8   // Active buzzer pin

void setup() {
  Serial.begin(115200);

  // Fan Servo setup
  fanServo.attach(FAN_SERVO_PIN);
  fanServo.write(FAN_STOP_ANGLE);

  // Touch Servo setup
  touchServo.attach(TOUCH_SERVO_PIN);
  touchServo.write(0);

  // Outputs and inputs
  pinMode(LDR_LED_PIN, OUTPUT);
  pinMode(IR_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(TOUCH_PIN, INPUT);

  digitalWrite(LDR_LED_PIN, LOW);
  digitalWrite(IR_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println(" System Ready: Thermistor + LDR + Touch + IR + Buzzer + 2 Servos");
}

void loop() {
  // --- Thermistor Fan Control ---
  int analogValue = analogRead(THERMISTOR_PIN);
  if (analogValue > 0 && analogValue < 1023) {
    float voltage = analogValue / 1023.0;
    float resistance = SERIES_RESISTOR * (1.0 / voltage - 1.0);

    float steinhart = resistance / NOMINAL_RESISTANCE;
    steinhart = log(steinhart);
    steinhart /= B_COEFFICIENT;
    steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15);
    steinhart = 1.0 / steinhart;
    float temperatureC = steinhart - 229.37;

    Serial.print("🌡 Temp: ");
    Serial.print(temperatureC, 2);
    Serial.print(" °C → ");

    if (temperatureC > TEMP_THRESHOLD) {
      if (!fanIsOn) {
        fanServo.write(FAN_ON_ANGLE);
        fanIsOn = true;
      }
      Serial.println("Fan ON");
    } else {
      if (fanIsOn) {
        fanServo.write(FAN_STOP_ANGLE);
        fanIsOn = false;
      }
      Serial.println(" Fan OFF");
    }
  } else {
    Serial.println("Invalid thermistor reading");
  }

  delay(300);

  // --- LDR + LED ---
  int lightLevel = analogRead(LDR_PIN);
  Serial.print("Light Level: ");
  Serial.println(lightLevel);

  if (lightLevel < LIGHT_THRESHOLD) {
    digitalWrite(LDR_LED_PIN, HIGH);
    Serial.println("Dark — LED ON");
  } else {
    digitalWrite(LDR_LED_PIN, LOW);
    Serial.println(" Bright — LED OFF");
  }

  // --- IR Motion + Buzzer + LED ---
  int irState = digitalRead(IR_SENSOR_PIN);
  if (irState == LOW) {
    digitalWrite(IR_LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("IR: Motion Detected!");
  } else {
    digitalWrite(IR_LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("IR: No motion");
  }

  // --- Touch Sensor Control ---
  bool currentTouch = digitalRead(TOUCH_PIN);
  if (lastTouchReading == HIGH && currentTouch == LOW) {
    touchState = !touchState;

    if (touchState) {
      touchServo.write(90);
      Serial.println(" Touch: Servo → 90°");
    } else {
      touchServo.write(0);
      Serial.println(" Touch: Servo → 0°");
    }

    delay(300);  // debounce
  }
  lastTouchReading = currentTouch;

  delay(300);
}
