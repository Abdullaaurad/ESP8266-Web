#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// WiFi credentials
const char *ssid = "Wifi adress"; // Replace with your network SSID
const char *password = "Wifi Password"; // Replace with your network password

// Ultrasonic Sensor Pins
const int trigPin = D1; // GPIO5
const int echoPin = D2; // GPIO4

// LDR Pin
const int ldrPin = A0; // Analog pin A0 for LDR

// Create a web server object
ESP8266WebServer server(80);

// Setup Wi-Fi connection
void setupWiFi()
{
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected to WiFi!");
    Serial.println("IP Address: " + WiFi.localIP().toString());
}

// Setup function
void setup()
{
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(ldrPin, INPUT);

    setupWiFi();

    // Define HTTP server route
    server.on("/", HTTP_GET, handleRoot);
    server.on("/data", HTTP_GET, handleData); // Route for fetching data

    // Start the server
    server.begin();
    Serial.println("HTTP server started");
}

// Function to measure distance from ultrasonic sensor
long measureDistance()
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    long distance = (duration * 0.0344) / 2; // Convert time to distance (cm)

    return distance;
}

// Function to calculate LUX value from LDR reading
float calculateLux(int ldrValue)
{
    // Calibration formula for converting LDR value to LUX
    // This may vary depending on your LDR and setup
    float voltage = (ldrValue / 1023.0) * 5.0; // Convert LDR value to voltage (assuming 5V reference)
    float lux = 500.0 * (5.0 / voltage);       // Example conversion, adjust according to your sensor's characteristics
    return lux;
}

// Function to handle root URL for web server
void handleRoot()
{
    String page = "<html><head><style>";

    // CSS for styling
    page += "body {font-family: Arial, sans-serif; background-color: #f4f4f9; margin: 0; padding: 20px;}";
    page += "h1 {color: #333; text-align: center;}";

    // Layout for the state boxes
    page += ".container { display: flex; justify-content: center; gap: 20px; margin-top: 50px; }";
    page += ".box { background-color: #ffffff; padding: 30px; border-radius: 10px; box-shadow: 0px 4px 6px rgba(0, 0, 0, 0.1); width: 250px; text-align: center;}";
    page += ".box h2 { font-size: 24px; color: #333;}";
    page += ".box p { font-size: 18px; color: #555; font-weight: bold; margin: 20px 0;}";

    // Button Styling
    page += "#refresh {font-size: 16px; padding: 10px 20px; background-color: #4CAF50; color: white; border: none; border-radius: 5px; cursor: pointer;}";
    page += "#refresh:hover {background-color: #45a049;}";

    page += "</style></head><body>";

    page += "<h1>ESP8266 Sensor Data</h1>";

    // HTML structure with state boxes
    page += "<div class='container'>";
    page += "<div class='box' id='distanceBox'>";
    page += "<h2>Distance</h2>";
    page += "<p id='distance'>Loading...</p>";
    page += "</div>";

    page += "<div class='box' id='luxBox'>";
    page += "<h2>LUX Value</h2>";
    page += "<p id='luxValue'>Loading...</p>";
    page += "</div>";
    page += "</div>";

    page += "<div style='text-align: center; margin-top: 30px;'>";
    page += "<button id='refresh' onclick='fetchData()'>Refresh</button>";
    page += "</div>";

    page += "<script>";

    // JavaScript for auto-refreshing the data
    page += "function fetchData() {";
    page += "  fetch('/data').then(response => response.text()).then(data => {";
    page += "    let values = data.split(',');";
    page += "    document.getElementById('distance').innerText = values[0] + ' cm';";
    page += "    document.getElementById('luxValue').innerText = values[1] + ' LUX';";
    page += "  });";
    page += "}";

    page += "setInterval(fetchData, 1000);"; // Refresh data every 1 second
    page += "fetchData();";                  // Initial fetch on page load

    page += "</script>";

    page += "</body></html>";

    server.send(200, "text/html", page); // Send the page with embedded JavaScript
}

// Function to send the sensor data as a response
void handleData()
{
    long distance = measureDistance();  // Get distance from ultrasonic sensor
    int ldrValue = analogRead(ldrPin);  // Get LDR value
    float lux = calculateLux(ldrValue); // Convert LDR value to LUX

    String data = String(distance) + "," + String(lux); // Concatenate the data (distance and LUX)
    server.send(200, "text/plain", data);               // Send data as plain text
}

// Main loop
void loop()
{
    server.handleClient(); // Handle incoming client requests
}
