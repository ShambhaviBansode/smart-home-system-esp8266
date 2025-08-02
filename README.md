# smart-home-system-esp8266

A smart home automation system using **ESP8266**, designed to control and monitor appliances based on sensor inputs and mobile commands. The system supports real-time automation and remote access through the **Blynk IoT app**, making it ideal for energy-efficient smart homes.

---

## 📌 Features

-  Automatic fan control based on temperature (DHT11)
-  Light automation using ambient light levels (LDR)
-  Touch-based control for appliances (TTP223)
-  Motion detection using IR sensor with visual alert (LED)
-  Manual and app-based control of devices
-  Real-time remote monitoring and switching via Blynk

---

## 🛠️ Components Used

- **NodeMCU ESP8266** – WiFi-enabled microcontroller
- **DHT11 Sensor** – Temperature & humidity
- **LDR** – Light detection
- **TTP223** – Capacitive touch sensor
- **IR Obstacle Sensor** – Motion detection
- **Relay Module** – To switch AC appliances
- **Servo Motor** – Optional for door/window automation
- **LED, Resistors, Wires, Breadboard**

---

## 🧰 Technologies & Tools

- **Programming Language:** Embedded C (Arduino)
- **Platform:** Arduino IDE
- **IoT Platform:** Blynk (Blynk 2.0 with auth token)
- **Libraries Used:** DHT, BlynkSimpleEsp8266, Servo, etc.

---

## 🚀 Getting Started

1. Clone this repository or download the code.
2. Open `home_automation.ino` in Arduino IDE.
3. Install required libraries: `Blynk`, `DHT`, `Servo`, etc.
4. Update the WiFi credentials and Blynk Auth Token in the code.
5. Connect hardware as per circuit diagram.
6. Upload code to ESP8266 and launch Blynk app to control devices.

---

## 📁 Project Structure

home-automation-project/
├── code/
│ └── home_automation code in Arduino IDE
├── images/
│ └── Protoype images
│ └── Circuit Images
├── README.md

## 👤 Author

**Shambhavi Bansode**  
[LinkedIn](https://linkedin.com/in/shambhavi-bansode) 

