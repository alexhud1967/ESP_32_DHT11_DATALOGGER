#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"

const char* ssid = "Hudspeth"; // Change this to your WiFi SSID
const char* password = "5593490267"; // Change this to your WiFi password

#define DHTPIN 15
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

// Constants for data logging and graphing
const int samplesPerMinute = 2;
const int maxDataPoints = 24 * 60 * samplesPerMinute; // 24 hours * 60 minutes * 2 samples per minute

float temperatureData[maxDataPoints];
float humidityData[maxDataPoints];
int currentIndex = 0;

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHT11 Web Server with Data Logging"));

  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  server.on("/", handleRoot);
  server.begin();
  Serial.println("Server started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Log data at regular intervals
  static unsigned long lastDataTime = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - lastDataTime >= 30000) { // Log data every 30 seconds
    lastDataTime = currentMillis;

    // Read sensor data
    float temperature = dht.readTemperature(true);
    float humidity = dht.readHumidity();

    // Store data in circular buffer
    temperatureData[currentIndex] = temperature;
    humidityData[currentIndex] = humidity;
    currentIndex = (currentIndex + 1) % maxDataPoints; // Wrap around index

    // Print readings to Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" F");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  }

  // Handle client requests
  server.handleClient();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>DHT11 Temperature and Humidity</title>";
  html += "<meta http-equiv='refresh' content='60'>"; // Auto-refresh every 60 seconds
  html += "<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>"; // Include Chart.js library
  html += "</head><body>";
  html += "<h1>DHT11 Temperature and Humidity</h1>";

  // Display current time
  html += "<p id='currentTime'></p>";
  html += "<script>";
  html += "function updateTime() {";
  html += "  var now = new Date();";
  html += "  var hours = now.getHours();";
  html += "  var minutes = now.getMinutes();";
  html += "  var seconds = now.getSeconds();";
  html += "  var timeString = hours + ':' + (minutes < 10 ? '0' + minutes : minutes) + ':' + (seconds < 10 ? '0' + seconds : seconds);";
  html += "  document.getElementById('currentTime').textContent = 'Current Time: ' + timeString;";
  html += "}";
  html += "updateTime();";
  html += "setInterval(updateTime, 1000);"; // Update time every second
  html += "</script>";

  // Display latest temperature and humidity
  int latestIndex = (currentIndex - 1 + maxDataPoints) % maxDataPoints;
  html += "<p>Temperature: " + String(temperatureData[latestIndex]) + " F</p>";
  html += "<p>Humidity: " + String(humidityData[latestIndex]) + " %</p>";

  // Create canvas for chart
  html += "<canvas id='myChart' width='800' height='400'></canvas>";

  // JavaScript for chart creation
  html += "<script>";
  html += "var ctx = document.getElementById('myChart').getContext('2d');";
  html += "var chart = new Chart(ctx, {";
  html += "  type: 'line',";
  html += "  data: {";
  html += "    labels: Array.from({length: " + String(maxDataPoints) + "}, (_, i) => ''),"; // Labels for x-axis
  html += "    datasets: [{";
  html += "      label: 'Temperature (F)',";
  html += "      borderColor: 'rgb(255, 99, 132)',";
  html += "      borderWidth: 2,"; // Thicker lines
  html += "      data: ["; // Temperature data points
  
  // Generate temperature data points
  for (int i = 0; i < maxDataPoints; i++) {
    if (i != 0) html += ",";
    html += String(temperatureData[i]);
  }

  html += "      ],";
  html += "    }, {";
  html += "      label: 'Humidity (%)',";
  html += "      borderColor: 'rgb(54, 162, 235)',";
  html += "      borderWidth: 2,"; // Thicker lines
  html += "      data: ["; // Humidity data points
  
  // Generate humidity data points
  for (int i = 0; i < maxDataPoints; i++) {
    if (i != 0) html += ",";
    html += String(humidityData[i]);
  }

  html += "      ],";
  html += "    }]";
  html += "  },";
  html += "  options: {";
  html += "    scales: {";
  html += "      x: {";
  html += "        reverse: false,"; // Plot from left to right
  html += "      }";
  html += "    }";
  html += "  }";
  html += "});";
  html += "</script>";

  html += "</body></html>";

  server.send(200, "text/html", html);
}
