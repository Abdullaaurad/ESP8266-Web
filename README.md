# ESP8266 Ultrasonic and LUX Sensor Web Dashboard

This project uses an **ESP8266** microcontroller to read data from an **ultrasonic distance sensor** and a **Light Dependent Resistor (LDR)**, then displays the data on a web-based dashboard. The web page shows the **distance** (in cm) measured by the ultrasonic sensor and the **LUX** (light intensity) calculated from the LDR sensor, and allows the user to refresh the data.

## Table of Contents

- [Introduction](#introduction)
- [Components Required](#components-required)
- [Wiring Diagram](#wiring-diagram)
- [Code Overview](#code-overview)
  - [Wi-Fi Setup](#wi-fi-setup)
  - [Ultrasonic Sensor](#ultrasonic-sensor)
  - [LDR (Light Sensor)](#ldr-light-sensor)
  - [Web Interface](#web-interface)
- [How to Use](#how-to-use)
- [Future Modifications](#future-modifications)
- [Possible Use Cases](#possible-use-cases)
- [Troubleshooting](#troubleshooting)

## Introduction

The ESP8266 is a powerful and cost-effective microcontroller with built-in Wi-Fi capabilities. This project utilizes the ESP8266 to create a simple web server that serves a dashboard for viewing sensor data in real-time.

- **Ultrasonic Sensor**: Measures the distance from the sensor to an object.
- **LDR (Light Dependent Resistor)**: Measures the intensity of light in the environment.
- **Web Interface**: Displays the data on a responsive webpage, and allows users to refresh the data via a button or auto-refresh mechanism.

This project provides an interactive way to monitor the distance and light intensity, suitable for use in home automation, environmental monitoring, or DIY IoT projects.

## Components Required

- **ESP8266 Module (e.g., NodeMCU, Wemos D1 Mini)**
- **Ultrasonic Sensor (HC-SR04)**
  - **VCC** → 3.3V (or 5V based on your ESP model)
  - **GND** → GND
  - **TRIG** → D1 (GPIO5)
  - **ECHO** → D2 (GPIO4)
- **Light Dependent Resistor (LDR)**
  - **One leg** → A0 (Analog input pin)
  - **Other leg** → GND (through a 10kΩ resistor to ground or 3.3V, depending on your LDR circuit)
- **Breadboard and Connecting Wires**
- **Power Supply** (e.g., Micro USB for NodeMCU or a 5V adapter)

## Wiring Diagram

Here's a simple schematic to wire the ultrasonic sensor and LDR to your ESP8266:

```
ESP8266 (NodeMCU/Wemos)    HC-SR04 Ultrasonic Sensor    LDR
3.3V / 5V                  VCC                          -
GND                        GND                          -
D1 (GPIO5)                 TRIG                         -
D2 (GPIO4)                 ECHO                         -
A0                         -                            LDR
```

- Ensure the **TRIG** and **ECHO** pins are correctly connected to **GPIO5** and **GPIO4** respectively.
- The **LDR** should be connected to the **analog input pin (A0)**.

## Code Overview

The code is written for the ESP8266 and is divided into several sections.

### Wi-Fi Setup

The `setupWiFi()` function connects the ESP8266 to the specified Wi-Fi network:

```cpp
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}
Serial.println("Connected to WiFi!");
Serial.println("IP Address: " + WiFi.localIP().toString());
```

### Ultrasonic Sensor

The `measureDistance()` function triggers the ultrasonic sensor to measure the distance and returns the value in centimeters:

```cpp
long measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return (duration * 0.0344) / 2; // Convert to cm
}
```

### LDR (Light Sensor)

The `calculateLux()` function converts the analog value from the LDR into a LUX value:

```cpp
float calculateLux(int ldrValue) {
  float voltage = (ldrValue / 1023.0) * 5.0;
  return 500.0 * (5.0 / voltage); // Example conversion formula
}
```

### Web Interface

The ESP8266 serves a webpage at the root ("/") that shows the distance and LUX values. The data is refreshed every second using JavaScript, and the user can manually refresh the page by clicking the Refresh button.

```cpp
server.on("/", HTTP_GET, handleRoot);
server.on("/data", HTTP_GET, handleData);
```

The HTML, CSS, and JavaScript are embedded within the code to generate a simple and responsive dashboard.

## How to Use

1. Upload the code to your ESP8266 using the Arduino IDE.
2. Monitor the Serial Output to get the IP address of the ESP8266 once it's connected to Wi-Fi.
3. Open a web browser and go to `http://<ESP_IP_ADDRESS>` to access the sensor data.
4. The page will display:
   - Distance (cm) from the ultrasonic sensor.
   - LUX value calculated from the LDR sensor.
   - Manual Refresh: Click the Refresh button to manually refresh the data.
   - Auto Refresh: The data will also refresh every second automatically.

## Future Modifications

Here are some potential modifications and enhancements for the project:

- Data Logging: Store the data in a file or a database for historical analysis.
- Add More Sensors: Integrate more sensors like temperature, humidity, or gas sensors and display their values on the dashboard.
- Mobile App Integration: Build a mobile app (using Blynk, for example) to view the data in real-time.
- Data Visualization: Use JavaScript libraries like Chart.js to visualize the data (graphical representation) on the web page.
- Error Handling: Add error handling for sensor reading issues or Wi-Fi disconnection.
- Email Alerts: Send email notifications if certain sensor thresholds are exceeded (e.g., when the distance is too low or the light is too bright).

## Possible Use Cases

This project can be used in a variety of applications:

- Home Automation: Monitor the distance to a door/window or detect objects, and measure light levels in different rooms for smart lighting control.
- Environmental Monitoring: Use the LDR and ultrasonic sensor to measure light and environmental changes in greenhouses or plant monitoring systems.
- Security Systems: Integrate with motion sensors to track room occupancy or detect movement and light intensity in secure areas.
- Robotics: Use the ultrasonic sensor to avoid obstacles or assist in navigation and map-building for autonomous robots.
- IoT Projects: Serve as an entry-level project to explore the capabilities of the ESP8266 in an IoT environment.

## Troubleshooting

- Wi-Fi Connection Issues: Ensure the Wi-Fi credentials (SSID and password) are correctly entered. Check the Serial Monitor for connection status.
- Data Not Updating: Make sure the web page is set to auto-refresh every second or manually click the Refresh button.
- Incorrect Distance Readings: Verify the ultrasonic sensor wiring and placement. Ensure there's no interference or obstruction around the sensor.
- LUX Value: If the LUX value seems incorrect, adjust the `calculateLux()` function formula for your specific LDR sensor.

## Conclusion

This project demonstrates a basic IoT application where the ESP8266 is used to measure real-time sensor data and display it through a web interface. By connecting an ultrasonic sensor and an LDR to the ESP8266, you can create an interactive, real-time monitoring dashboard.

The project is highly customizable and can serve as a foundation for more advanced IoT and home automation projects.
