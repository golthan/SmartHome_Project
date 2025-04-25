#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "ds1307.hpp"  // dùng thư viện bạn upload

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// Pin settings
#define RELAY_FAN 18
#define RELAY_LIGHT 19

// WiFi credentials
const char* ssid = "golthan";
const char* password = "12345678";
extern int second, minute, hour, day, wday, month, year;
// Web server
WebServer server(80);

// Sensor and DS1307
Adafruit_BME280 bme;

// Queue handle
QueueHandle_t sensorDataQueue;

// Shared variables
float temperature = 0.0, humidity = 0.0;

// auto mode settings
float fanThreshold = 26.0;   // Default: fan ON > 26°C
float lightThreshold = 20.0; // Default: light ON < 20°C

// Schedule mode time settings
int fanStartHour = -1, fanStartMinute = -1, fanEndHour = -1, fanEndMinute = -1;
int lightStartHour = -1, lightStartMinute = -1, lightEndHour = -1, lightEndMinute = -1;

// Mode flags
bool autoMode = true;    // Default: auto Mode
bool manualMode = false;
bool ScheduleMode = false;

// Manual control states
bool manualFanState = false;
bool manualLightState = false;

// Data struct
struct SensorData {
  float temperature;
  float humidity;
};

// Task to read BME280 sensor
void TaskReadSensor(void *pvParameters) {
  SensorData data;
  while (1) {
    data.temperature = bme.readTemperature();
    data.humidity = bme.readHumidity();
    xQueueOverwrite(sensorDataQueue, &data);
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

// Task to update displayed values
void TaskUpdateDisplay(void *pvParameters) {
  while (1) {
    SensorData data;
    if (xQueueReceive(sensorDataQueue, &data, portMAX_DELAY)) {
      temperature = data.temperature;
      humidity = data.humidity;
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

// Task to control relays based on mode
void TaskControlRelay(void *pvParameters) {
  while (1) {
    if (autoMode) {
      if (temperature >= fanThreshold) {
        digitalWrite(RELAY_FAN, HIGH);
        digitalWrite(RELAY_LIGHT, LOW);
      } else if (temperature <= lightThreshold) {
        digitalWrite(RELAY_FAN, LOW);
        digitalWrite(RELAY_LIGHT, HIGH);
      } else {
        digitalWrite(RELAY_FAN, LOW);
        digitalWrite(RELAY_LIGHT, LOW);
      }
    } 
    else if (manualMode) {
      digitalWrite(RELAY_FAN, manualFanState ? HIGH : LOW);
      digitalWrite(RELAY_LIGHT, manualLightState ? HIGH : LOW);
    } 
    else if (ScheduleMode) {
      uint8_t hour, minute, second;
      readDS1307();  // Lấy thời gian từ DS1307

      if (fanStartHour <= hour && hour < fanEndHour) {
        digitalWrite(RELAY_FAN, HIGH);
      } else {
        digitalWrite(RELAY_FAN, LOW);
      }

      if (lightStartHour <= hour && hour < lightEndHour) {
        digitalWrite(RELAY_LIGHT, HIGH);
      } else {
        digitalWrite(RELAY_LIGHT, LOW);
      }
    }

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

// Task to handle WebServer
void TaskWebServer(void *pvParameters) {
  server.on("/", HTTP_GET, []() {
    String html = "<html><head><style>";
    html += "body{background:#eef;text-align:center;font-family:sans-serif;}";
    html += "button{padding:10px 20px;margin:10px;font-size:18px;}";
    html += ".auto{background:#4CAF50;color:white;}";
    html += ".manual{background:#2196F3;color:white;}";
    html += ".Schedule{background:#FF9800;color:white;}";
    html += "</style></head><body>";

    html += "<h1>ESP32 Web Control</h1>";
    html += "<h3>Temperature: " + String(temperature) + " *C</h3>";
    html += "<h3>Humidity: " + String(humidity) + " %</h3>";

    html += "<br><a href='/auto'><button class='auto'>auto Mode</button></a>";
    html += "<a href='/manual'><button class='manual'>Manual Mode</button></a>";
    html += "<a href='/Schedule'><button class='Schedule'>Schedule Mode</button></a>";

    html += "</body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/auto", HTTP_GET, []() {
    autoMode = true;
    manualMode = false;
    ScheduleMode = false;

    String html = "<html><body style='text-align:center;'>";
    html += "<h2>Set thresholds for auto Mode</h2>";
    html += "<form action='/setauto' method='POST'>";
    html += "Fan ON above (*C): <input name='fan' type='number' step='0.1'><br><br>";
    html += "Light ON below (*C): <input name='light' type='number' step='0.1'><br><br>";
    html += "<input type='submit' value='Save'>";
    html += "</form><br><a href='/'>Back</a></body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/setauto", HTTP_POST, []() {
    fanThreshold = server.arg("fan").toFloat();
    lightThreshold = server.arg("light").toFloat();
    server.send(200, "text/html", "<h2>auto thresholds updated!</h2><a href='/'>Back</a>");
  });

  server.on("/manual", HTTP_GET, []() {
    autoMode = false;
    manualMode = true;
    ScheduleMode = false;

    String html = "<html><body style='text-align:center;'>";
    html += "<h2>Manual Control</h2>";
    html += "<a href='/fanon'><button>Fan ON</button></a>";
    html += "<a href='/fanoff'><button>Fan OFF</button></a><br><br>";
    html += "<a href='/lighton'><button>Light ON</button></a>";
    html += "<a href='/lightoff'><button>Light OFF</button></a><br><br>";
    html += "<a href='/'>Back</a></body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/fanon", HTTP_GET, []() {
    manualFanState = true;
    server.sendHeader("Location", "/manual");
    server.send(302);
  });

  server.on("/fanoff", HTTP_GET, []() {
    manualFanState = false;
    server.sendHeader("Location", "/manual");
    server.send(302);
  });

  server.on("/lighton", HTTP_GET, []() {
    manualLightState = true;
    server.sendHeader("Location", "/manual");
    server.send(302);
  });

  server.on("/lightoff", HTTP_GET, []() {
    manualLightState = false;
    server.sendHeader("Location", "/manual");
    server.send(302);
  });

  server.on("/Schedule", HTTP_GET, []() {
    autoMode = false;
    manualMode = false;
    ScheduleMode = true;

    String html = "<html><body style='text-align:center;'>";
    html += "<h2>Set Schedule Mode Times</h2>";
    html += "<form action='/setSchedule' method='POST'>";
    html += "Fan ON (h:m): <input name='fanStartH' type='number'>:<input name='fanStartM' type='number'><br><br>";
    html += "Fan OFF (h:m): <input name='fanEndH' type='number'>:<input name='fanEndM' type='number'><br><br>";
    html += "Light ON (h:m): <input name='lightStartH' type='number'>:<input name='lightStartM' type='number'><br><br>";
    html += "Light OFF (h:m): <input name='lightEndH' type='number'>:<input name='lightEndM' type='number'><br><br>";
    html += "<input type='submit' value='Save'>";
    html += "</form><br><a href='/'>Back</a></body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/setSchedule", HTTP_POST, []() {
    // Lấy giá trị từ form
    fanStartHour = server.arg("fanStartH").toInt();
    fanStartMinute = server.arg("fanStartM").toInt();
    fanEndHour = server.arg("fanEndH").toInt();
    fanEndMinute = server.arg("fanEndM").toInt();
    
    lightStartHour = server.arg("lightStartH").toInt();
    lightStartMinute = server.arg("lightStartM").toInt();
    lightEndHour = server.arg("lightEndH").toInt();
    lightEndMinute = server.arg("lightEndM").toInt();
    
    server.send(200, "text/html", "<h2>Schedule updated!</h2><a href='/'>Back</a>");
  });

  server.begin();

  while (1) {
    server.handleClient();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(RELAY_FAN, OUTPUT);
  pinMode(RELAY_LIGHT, OUTPUT);
  digitalWrite(RELAY_FAN, LOW);
  digitalWrite(RELAY_LIGHT, LOW);

  Wire.begin();
  if (!bme.begin(0x76)) {
    Serial.println("Lỗi khởi tạo BME280! Kiểm tra kết nối.");
    while (1);  // Dừng chương trình nếu lỗi
  }
  setTime(10, 30, 00, 2, 25, 6, 25);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected! IP: " + WiFi.localIP().toString());
  
  sensorDataQueue = xQueueCreate(1, sizeof(SensorData));
if (sensorDataQueue == NULL) {
    Serial.println("Lỗi tạo Queue! Không đủ bộ nhớ.");
    while (1);  // Dừng nếu lỗi
}

  xTaskCreatePinnedToCore(TaskReadSensor, "Sensor", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskUpdateDisplay, "Display", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskControlRelay, "Relay", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskWebServer, "WebServer", 4096, NULL, 1, NULL, 0);
}

void loop() {
  // Empty
}