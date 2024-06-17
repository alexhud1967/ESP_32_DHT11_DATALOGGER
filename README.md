ESP32 DHT11 Data Logger

Replace this with an actual image or screenshot of your project

Overview
This project utilizes an ESP32 microcontroller along with a DHT11 temperature and humidity sensor to log environmental data over time. The data is displayed in real-time on a web server hosted by the ESP32 and can be accessed via a web browser. This repository contains the Arduino sketch for the ESP32, which logs temperature and humidity readings and displays them graphically using Chart.js.

Features
Logs temperature (in Fahrenheit) and humidity (%) every 30 seconds.
Displays real-time data on a web page hosted by the ESP32.
Uses Chart.js to graphically represent data over the last 24 hours.
Auto-refreshing web page updates every 60 seconds.
Serial monitor output for debugging and live readings.
Components Used
ESP32 microcontroller
DHT11 temperature and humidity sensor
WiFi connection for hosting the web server and uploading data
Chart.js library for data visualization
Installation and Setup
Clone the Repository:




Open the Arduino IDE.
Load the esp32_dht11_logger.ino sketch from this repository.
Update the ssid and password variables with your WiFi credentials.
Select the correct board and port for your ESP32.
Upload the sketch to your ESP32.
Access Data:

Once uploaded, open a serial monitor to view live temperature and humidity readings.
Open a web browser and navigate to the IP address of your ESP32 (http://<ESP32_IP>/) to view the graphical data representation.
Usage
Web Interface:

The web interface displays real-time temperature and humidity readings.
Data is graphed over the last 24 hours, updating every 30 seconds.
Serial Monitor:

Use the serial monitor for debugging and to monitor live sensor readings.
Contributing
Contributions to this project are welcome! Please fork the repository, make your changes, and submit a pull request with a clear description of your modifications.

License
This project is licensed under the MIT License - see the LICENSE file for details.

Acknowledgments
Chart.js: For providing the charting library used in this project.
Open-source community: For their contributions to Arduino libraries and ESP32 development.
