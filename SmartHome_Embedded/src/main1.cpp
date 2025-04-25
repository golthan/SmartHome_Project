// #include <Arduino.h>
// #include <WiFi.h>
// #include <Wire.h>
// #include <Adafruit_BME280.h>
// #include <BH1750.h>
// #include<LiquidCrystal_I2C.h>
// #include <BH1750.h>
// #include <WebServer.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/queue.h"
// #include "freertos/semphr.h"

// #define RELAY1 18
// #define RELAY2 19

// const char *ssid = "golthan";
// const char *password = "12345678";
// const char *adminUser = "admin";
// const char *adminPass = "123456";

// Adafruit_BME280 bme;
// BH1750 lightMeter;
// LiquidCrystal_I2C lcd(0x27, 16, 2);
// WebServer server(80);
// bool isAuthenticated = false;

// float currentTemp;
// QueueHandle_t tempQueue;
// SemaphoreHandle_t lcdSemaphore;
// SemaphoreHandle_t relayMutex;
// TaskHandle_t serverTaskHandle;

// enum Mode { NONE, MANUAL, TIMER, SMART };
// Mode currentMode = NONE;

// bool manualRelay1 = false;
// bool manualRelay2 = false;

// int timerOnHour = 8;
// int timerOffHour = 18;

// void taskReadSensor(void *pvParameters);
// void taskControlRelay(void *pvParameters);
// void taskDisplayLCD(void *pvParameters);
// void taskServer(void *pvParameters);
// void taskManager(void *pvParameters);

// void setup() {
//     Serial.begin(9600);
//     WiFi.begin(ssid, password);
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(1000);
//         Serial.println("Đang kết nối WiFi...");
//     }
//     Serial.println("Kết nối thành công! IP: " + WiFi.localIP().toString());
    
//     if (!bme.begin(0x76)) Serial.println("Không tìm thấy BME280");
//     if (!lightMeter.begin()) Serial.println("Không thể kết nối BH1750");
    
//     lcd.init();
//     lcd.backlight();
//     lcd.setCursor(0, 0);
//     lcd.print("WiFi Connected");
//     lcd.setCursor(0, 1);
//     lcd.print(WiFi.localIP().toString());
    
//     pinMode(RELAY1, OUTPUT);
//     pinMode(RELAY2, OUTPUT);
    
//     tempQueue = xQueueCreate(5, sizeof(float));
//     lcdSemaphore = xSemaphoreCreateMutex();
//     relayMutex = xSemaphoreCreateMutex();
    
//     xTaskCreate(taskReadSensor, "ReadSensor", 2048, NULL, 2, NULL);
//     xTaskCreate(taskControlRelay, "ControlRelay", 2048, NULL, 3, NULL);
//     xTaskCreate(taskDisplayLCD, "DisplayLCD", 2048, NULL, 1, NULL);
//     xTaskCreate(taskServer, "WebServerTask", 4096, NULL, 1, &serverTaskHandle);
//     xTaskCreate(taskManager, "TaskManager", 2048, NULL, 1, NULL);
    
//     server.on("/", HTTP_GET, []() {
//         if (!isAuthenticated) {
//             server.send(200, "text/html", "<html><body><h2>Đăng nhập</h2><form action='/login' method='POST'><input name='user'><input type='password' name='pass'><input type='submit' value='Đăng nhập'></form></body></html>");
//         } else {
//             server.send(200, "text/html", "<html><body><h2>Trang chủ</h2><button onclick='location.href=\"/manual\"'>Chế độ thủ công</button><button onclick='location.href=\"/timer\"'>Chế độ hẹn giờ</button><button onclick='location.href=\"/smart\"'>Chế độ thông minh</button></body></html>");
//         }
//     });
    
//     server.on("/login", HTTP_POST, []() {
//         if (server.hasArg("user") && server.hasArg("pass")) {
//             if (server.arg("user") == adminUser && server.arg("pass") == adminPass) {
//                 isAuthenticated = true;
//                 server.sendHeader("Location", "/");
//                 server.send(302, "text/plain", "");
//                 return;
//             }
//         }
//         server.send(401, "text/plain", "Đăng nhập thất bại!");
//     });
    
//     server.on("/manual", HTTP_GET, []() { currentMode = MANUAL; server.send(200, "text/plain", "Chế độ thủ công đã chọn"); });
//     server.on("/timer", HTTP_GET, []() { currentMode = TIMER; server.send(200, "text/plain", "Chế độ hẹn giờ đã chọn"); });
//     server.on("/smart", HTTP_GET, []() { currentMode = SMART; server.send(200, "text/plain", "Chế độ thông minh đã chọn"); });
//     server.on("/toggleRelay1", HTTP_GET, []() { manualRelay1 = !manualRelay1; server.send(200, "text/plain", "Relay 1 toggled"); });
//     server.on("/toggleRelay2", HTTP_GET, []() { manualRelay2 = !manualRelay2; server.send(200, "text/plain", "Relay 2 toggled"); });
    
//     server.begin();
//     Serial.println("Server đã sẵn sàng.");
// }

// void loop() 
// {
  
// }

// void taskReadSensor(void *pvParameters) {
//     while (1) {
//         float temp = bme.readTemperature();
//         xQueueOverwrite(tempQueue, &temp);
//         vTaskDelay(pdMS_TO_TICKS(5000));
//     }
// }

// void taskControlRelay(void *pvParameters) {
//     while (1) {
//         xSemaphoreTake(relayMutex, portMAX_DELAY);
//         if (currentMode == SMART) {
//             xQueuePeek(tempQueue, &currentTemp, portMAX_DELAY);
//             digitalWrite(RELAY1, (currentTemp > 28) ? HIGH : LOW);
//         }
//         xSemaphoreGive(relayMutex);
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
// }

// void taskDisplayLCD(void *pvParameters) {
//     while (1) {
//         xSemaphoreTake(lcdSemaphore, portMAX_DELAY);
//         lcd.setCursor(0, 0);
//         lcd.print("Temp: ");
//         lcd.print(currentTemp);
//         lcd.print("C");
//         xSemaphoreGive(lcdSemaphore);
//         vTaskDelay(pdMS_TO_TICKS(2000));
//     }
// }

// void taskServer(void *pvParameters) {
//     while (1) {
//         server.handleClient();
//         vTaskDelay(pdMS_TO_TICKS(100));
//     }
// }

// void taskManager(void *pvParameters) {
//     while (1) {
//         switch (currentMode) {
//             case MANUAL:
//                 digitalWrite(RELAY1, manualRelay1 ? HIGH : LOW);
//                 digitalWrite(RELAY2, manualRelay2 ? HIGH : LOW);
//                 break;
//             case TIMER:
//                 // if (hour() >= timerOnHour && hour() < timerOffHour) {
//                 //     digitalWrite(RELAY1, HIGH);
//                 //     digitalWrite(RELAY2, HIGH);
//                 // } else {
//                 //     digitalWrite(RELAY1, LOW);
//                 //     digitalWrite(RELAY2, LOW);
//                 // }
//                 // break;
//             case SMART:
//                 if (currentTemp > 28) {
//                     digitalWrite(RELAY1, HIGH);
//                 } else {
//                     digitalWrite(RELAY1, LOW);
//                 }
//                 break;
//             default:
//                 Serial.println("Chưa chọn chế độ.");
//                 break;
//         }
//         vTaskDelay(pdMS_TO_TICKS(2000));
//     }
// }
