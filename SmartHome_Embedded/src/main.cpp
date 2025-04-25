// #include <WiFi.h>
// #include <WebServer.h>
// #include <WiFi.h>
// #include <NTPClient.h>
// #include <WiFiUdp.h>
// #include <Wire.h>
// #include <WiFi.h>
// #include "time.h"
// #include "ds1307.hpp"
// #include <Preferences.h>
// #include <Adafruit_Sensor.h>
// #include <Adafruit_BME280.h>
// #include <BH1750.h>
// #include<LiquidCrystal_I2C.h>

// // Cấu hình pin
// #define RELAY1 18 // Quạt
// #define RELAY2 19 // Đèn
// #define RED_PIN 27
// #define GREEN_PIN 26
// #define BLUE_PIN 25

// int redValue = 0, greenValue = 0, blueValue = 0;
// int brightness = 255;

// Preferences preferences;
// // Thông tin WiFi
// const char *ssid = "golthan";
// const char *password = "12345678";

// // Thông tin đăng nhập
// const String username = "admin";
// const String user_password = "admin123";
// int startHour = 0, startMinute = 0, endHour = 0, endMinute = 0;
// LiquidCrystal_I2C lcd(0x27, 16, 2);

// bool relay1State = false;
// bool relay2State = false;
// int rgbR = 0, rgbG = 0, rgbB = 0;

// float temperature = 0.0, humidity = 0.0, pressure = 0.0, lightLevel = 0.0;

// WiFiUDP ntpUDP;
// NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600, 60000); // UTC+7
// // BME280 Sensor
// Adafruit_BME280 bme; // Cảm biến BME280
// BH1750 lightMeter;

// // Auto Mode Variables
// // Biến thời gian cho chế độ Auto
// unsigned long fanOnTime = 0;
// unsigned long lightOnTime = 0;
// bool autoMode = false;
// extern int second, minute, hour, day, wday, month, year;
// // Smart Mode Variables
// float tempThreshold = 25.0;     // Default temperature threshold
// float humidityThreshold = 60.0; // Default humidity threshold
// bool smartMode = false;
// // Tạo server trên cổng 80
// WebServer server(80);
// bool fanAuto = false, lightAuto = false;
// int fanStartHour = 0, fanStartMinute = 0, fanEndHour = 0, fanEndMinute = 0;
// int lightStartHour = 0, lightStartMinute = 0, lightEndHour = 0, lightEndMinute = 0;
// // Trạng thái đăng nhập
// bool isAuthenticated = false;
// const char *ntpServer = "pool.ntp.org";
// const long gmtOffset_sec = 25200;
// const int daylightOffset_sec = 0;

// // CSS chung
// const char stylePage[] PROGMEM = R"rawliteral(
// <style>
//     body {
//         font-family: Arial, sans-serif;
//         margin: 0;
//         padding: 0;
//         background-color: #f4f4f9;
//         color: #333;
//     }
//     h2 {
//         color: #2c3e50;
//     }
//     .container {
//         max-width: 600px;
//         margin: auto;
//         padding: 20px;
//         background: #fff;
//         box-shadow: 0 4px 8px rgba(0,0,0,0.1);
//         border-radius: 8px;
//     }
//     form input[type="text"], form input[type="password"] {
//         width: 100%;
//         padding: 10px;
//         margin: 10px 0;
//         border: 1px solid #ccc;
//         border-radius: 4px;
//     }
//     form input[type="submit"] {
//         background-color: #3498db;
//         color: white;
//         border: none;
//         padding: 10px;
//         border-radius: 4px;
//         cursor: pointer;
//     }
//     form input[type="submit"]:hover {
//         background-color: #2980b9;
//     }
//     ul {
//         list-style: none;
//         padding: 0;
//     }
//     ul li {
//         margin: 10px 0;
//     }
//     ul li a {
//         text-decoration: none;
//         color: #3498db;
//         font-weight: bold;
//     }
//     ul li a:hover {
//         color: #2980b9;
//     }
// </style>
// )rawliteral";

// // HTML giao diện đăng nhập
// const char loginPage[] PROGMEM = R"rawliteral(
// <!DOCTYPE html>
// <html lang="vi">
// <head>
//     <meta charset="UTF-8">
//     <meta name="viewport" content="width=device-width, initial-scale=1.0">
//     <title>Đăng nhập</title>
//     %STYLE%
// </head>
// <body>
//     <div class="container">
//         <h2>Đăng nhập</h2>
//         <form action="/login" method="POST">
//             Tên đăng nhập: <input type="text" name="username"><br>
//             Mật khẩu: <input type="password" name="password"><br>
//             <input type="submit" value="Đăng nhập">
//         </form>
//     </div>
// </body>
// </html>
// )rawliteral";

// // HTML giao diện chính
// const char mainPage[] PROGMEM = R"rawliteral(
// <!DOCTYPE html>
// <html lang="vi">
// <head>
//     <meta charset="UTF-8">
//     <meta name="viewport" content="width=device-width, initial-scale=1.0">
//     <title>Menu chính</title>
//     %STYLE%
// </head>
// <body>
//     <div class="container">
//         <h2>Menu chính</h2>
//         <ul>
//             <li><a href="/control">Điều khiển thiết bị</a></li>
//             <li><a href="/settings">Thay đổi cài đặt hệ thống</a></li>
//             <li><a href="/data">Xem dữ liệu</a></li>
//         </ul>
//     </div>
// </body>
// </html>
// )rawliteral";
// String getLightStatus(float lux)
// {
//     if (lux <= 0.02)
//         return "Ban đêm";
//     if (lux <= 0.3)
//         return "Trời sáng trăng";
//     if (lux <= 50)
//         return "Trời mây trong nhà";
//     if (lux <= 500)
//         return "Trời mây ngoài trời";
//     if (lux <= 1000)
//         return "Trời nắng trong nhà";
//     return "Ánh sáng quá mạnh";
// }

// // Hàm hiển thị trang dữ liệu
// void handleDataPage()
// {
//     float temperature = bme.readTemperature();
//     float humidity = bme.readHumidity();
//     float pressure = bme.readPressure() / 100.0F;
//     uint16_t lux = lightMeter.readLightLevel(); // Đọc độ sáng từ cảm biến BH1750

//     String lightStatus = getLightStatus(lux);

//     String page = R"rawliteral(
//     <!DOCTYPE html>
//     <html lang="vi">
//     <head>
//         <meta charset="UTF-8">
//         <meta name="viewport" content="width=device-width, initial-scale=1.0">
//         <title>Dữ liệu cảm biến</title>
//         <style>
//             body { font-family: Arial, sans-serif; background-color: #f4f4f9; color: #333; }
//             h2 { color: #4CAF50; }
//             table { width: 100%; border-collapse: collapse; }
//             th, td { padding: 10px; text-align: left; border: 1px solid #ddd; }
//         </style>
//     </head>
//     <body>
//         <h2>Dữ liệu cảm biến</h2>
//         <table>
//             <tr><th>Nhiệt độ (°C)</th><td>%TEMP%</td></tr>
//             <tr><th>Độ ẩm (%)</th><td>%HUMIDITY%</td></tr>
//             <tr><th>Áp suất (hPa)</th><td>%PRESSURE%</td></tr>
//             <tr><th>Độ sáng (lx)</th><td>%LUX%</td></tr>
//             <tr><th>Trạng thái độ sáng</th><td>%LIGHT_STATUS%</td></tr>
//         </table>
//         <br>
//         <a href='/control'>Quay lại</a>
//     </body>
//     </html>
//   )rawliteral";

//     page.replace("%TEMP%", String(temperature));
//     page.replace("%HUMIDITY%", String(humidity));
//     page.replace("%PRESSURE%", String(pressure));
//     page.replace("%LUX%", String(lux));
//     page.replace("%LIGHT_STATUS%", lightStatus);

//     server.send(200, "text/html", page);
// }

// // Các route con tương tự
// const char controlPage[] PROGMEM = R"rawliteral(
// <!DOCTYPE html>
// <html lang="vi">
// <head>
//     <meta charset="UTF-8">
//     <meta name="viewport" content="width=device-width, initial-scale=1.0">
//     <title>Điều khiển thiết bị</title>
//     %STYLE%
// </head>
// <body>
//     <div class="container">
//         <h2>Điều khiển thiết bị</h2>
//         <ul>
//             <li><a href="/manual">Manual Mode</a></li>
//             <li><a href="/auto">Auto Mode</a></li>
//             <li><a href="/smart">Smart Mode</a></li>
//         </ul>
//     </div>
// </body>
// </html>
// )rawliteral";

// String injectCSS(const char *page)
// {
//     String html = page;
//     html.replace("%STYLE%", stylePage);
//     return html;
// }

// // Xử lý đăng nhập
// void handleLogin()
// {
//     if (server.method() == HTTP_POST)
//     {
//         String inputUser = server.arg("username");
//         String inputPass = server.arg("password");

//         if (inputUser == username && inputPass == user_password)
//         {
//             isAuthenticated = true;
//             server.send(200, "text/html", injectCSS(mainPage));
//         }
//         else
//         {
//             server.send(200, "text/html", injectCSS("<h2>Sai thông tin đăng nhập!</h2><a href='/'>Quay lại</a>"));
//         }
//     }
//     else
//     {
//         server.send(405, "text/plain", "Phương thức không được phép");
//     }
// }

// const char manualPage[] PROGMEM = R"rawliteral(
// <!DOCTYPE html>
// <html lang="vi">
// <head>
//     <meta charset="UTF-8">
//     <meta name="viewport" content="width=device-width, initial-scale=1.0">
//     <title>Manual Mode</title>
//     %STYLE%
// </head>
// <body>
//     <h2>Chế độ điều khiển thủ công</h2>
//     <h3>Điều khiển thiết bị</h3>
//     <button class="btn" onclick="toggleRelay('relay1')">Quạt: %RELAY1%</button>
//     <button class="btn" onclick="toggleRelay('relay2')">Đèn: %RELAY2%</button>
//     <script>
//         function toggleRelay(relay) {
//             fetch('/toggle?relay=' + relay).then(response => location.reload());
//         }
//     </script>
// </body>
// </html>
// )rawliteral";

// // Hàm thêm CSS và trạng thái vào giao diện
// String renderManualPage()
// {
//     String page = manualPage;
//     page.replace("%STYLE%", stylePage);
//     page.replace("%RELAY1%", relay1State ? "Bật" : "Tắt");
//     page.replace("%RELAY2%", relay2State ? "Bật" : "Tắt");
//     return page;
// }

// void handleManualMode()
// {
//     server.send(200, "text/html", renderManualPage());
// }

// void handleToggleRelay()
// {
//     String relay = server.arg("relay");
//     if (relay == "relay1")
//     {
//         relay1State = !relay1State;
//         digitalWrite(RELAY1, relay1State ? HIGH : LOW);
//     }
//     else if (relay == "relay2")
//     {
//         relay2State = !relay2State;
//         digitalWrite(RELAY2, relay2State ? HIGH : LOW);
//     }
//     server.send(200, "text/plain", "OK");
// }

// // Xử lý hiển thị giao diện
// void handleRoot()
// {
//     if (!isAuthenticated)
//     {
//         server.send(200, "text/html", injectCSS(loginPage));
//     }
//     else
//     {
//         server.send(200, "text/html", injectCSS(mainPage));
//     }
// }
// // Hàm điều khiển relay (quạt và đèn)
// void controlRelays()
// {
//     float currentTemp = bme.readTemperature(); // Đọc nhiệt độ từ cảm biến BME280

//     // Chế độ Smart
//     if (smartMode)
//     {
//         if (currentTemp < 20.0)
//         {
//             digitalWrite(RELAY2, HIGH); // Bật đèn
//             digitalWrite(RELAY1, LOW);  // Tắt quạt
//         }
//         else if (currentTemp >= 20.0 && currentTemp <= 25.0)
//         {
//             digitalWrite(RELAY2, LOW); // Tắt đèn
//             digitalWrite(RELAY1, LOW); // Tắt quạt
//         }
//         else if (currentTemp > 25.0)
//         {
//             digitalWrite(RELAY2, LOW);  // Tắt đèn
//             digitalWrite(RELAY1, HIGH); // Bật quạt
//         }
//     }

//     // Chế độ Auto
//     else
//     {
//         readDS1307();
//         int currentHour = hour;
//         int currentMinute = minute;

//         // Điều khiển quạt
//         if (fanAuto && (currentHour > fanStartHour || (currentHour == fanStartHour && currentMinute >= fanStartMinute)) &&
//             (currentHour < fanEndHour || (currentHour == fanEndHour && currentMinute <= fanEndMinute)))
//         {
//             digitalWrite(RELAY1, HIGH); // Bật quạt
//         }
//         else
//         {
//             digitalWrite(RELAY1, LOW); // Tắt quạt
//         }

//         // Điều khiển đèn
//         if (lightAuto && (currentHour > lightStartHour || (currentHour == lightStartHour && currentMinute >= lightStartMinute)) &&
//             (currentHour < lightEndHour || (currentHour == lightEndHour && currentMinute <= lightEndMinute)))
//         {
//             digitalWrite(RELAY2, HIGH); // Bật đèn
//         }
//         else
//         {
//             digitalWrite(RELAY2, LOW); // Tắt đèn
//         }
//     }
// }

// // Các route khác
// void handleControl()
// {
//     server.send(200, "text/html", injectCSS(controlPage));
// }
// void handleSmartPage()
// {
//     float currentTemp = bme.readTemperature(); // Đọc nhiệt độ từ cảm biến BME280
//     String page = R"rawliteral(
//     <!DOCTYPE html>
//     <html lang="vi">
//     <head>
//         <meta charset="UTF-8">
//         <meta name="viewport" content="width=device-width, initial-scale=1.0">
//         <title>Điều khiển Smart</title>
//         <style>
//             body { font-family: Arial, sans-serif; background-color: #f4f4f9; color: #333; }
//             h2 { color: #4CAF50; }
//             input[type="submit"] { background-color: #4CAF50; color: white; border: none; padding: 10px 20px; cursor: pointer; }
//             input[type="submit"]:hover { background-color: #45a049; }
//         </style>
//     </head>
//     <body>
//         <h2>Điều khiển Smart</h2>
//         <form action="/setSmart" method="POST">
//             <label for="enable">Kích hoạt chế độ Smart:</label>
//             <input type="checkbox" id="enable" name="enable" value="1" %CHECK%><br>
//             <input type="submit" value="Cập nhật Smart">
//         </form>
//         <br>
//         <h3>Nhiệt độ hiện tại: %TEMP% °C</h3>
//     </body>
//     </html>
//   )rawliteral";

//     page.replace("%TEMP%", String(currentTemp));
//     page.replace("%CHECK%", smartMode ? "checked" : "");
//     server.send(200, "text/html", page);
// }

// // Cập nhật chế độ Smart
// void handleSetSmart()
// {
//     if (server.method() == HTTP_POST)
//     {
//         bool enable = server.arg("enable") == "1";
//         smartMode = enable;
//         server.send(200, "text/html", "<h2>Đã cập nhật chế độ Smart</h2><a href='/control'>Quay lại</a>");
//         Serial.printf("Smart Mode updated: Enabled = %d\n", enable);
//     }
// }

// // Hàm hiển thị trang Auto
// void handleAutoPage()
// {
//     String page = R"rawliteral(
//     <!DOCTYPE html>
//     <html lang="vi">
//     <head>
//         <meta charset="UTF-8">
//         <meta name="viewport" content="width=device-width, initial-scale=1.0">
//         <title>Điều khiển Auto</title>
//         <style>
//             body { font-family: Arial, sans-serif; background-color: #f4f4f9; color: #333; }
//             h2 { color: #4CAF50; }
//             input[type="submit"] { background-color: #4CAF50; color: white; border: none; padding: 10px 20px; cursor: pointer; }
//             input[type="submit"]:hover { background-color: #45a049; }
//         </style>
//     </head>
//     <body>
//         <h2>Điều khiển Auto</h2>
//         <form action="/setAuto" method="POST">
//             <label for="fanStartHour">Giờ bắt đầu quạt:</label>
//             <input type="number" id="fanStartHour" name="fanStartHour" min="0" max="23" value="%FAN_START_HOUR%"><br>
//             <label for="fanStartMinute">Phút bắt đầu quạt:</label>
//             <input type="number" id="fanStartMinute" name="fanStartMinute" min="0" max="59" value="%FAN_START_MINUTE%"><br>
//             <label for="fanEndHour">Giờ kết thúc quạt:</label>
//             <input type="number" id="fanEndHour" name="fanEndHour" min="0" max="23" value="%FAN_END_HOUR%"><br>
//             <label for="fanEndMinute">Phút kết thúc quạt:</label>
//             <input type="number" id="fanEndMinute" name="fanEndMinute" min="0" max="59" value="%FAN_END_MINUTE%"><br>

//             <label for="lightStartHour">Giờ bắt đầu đèn:</label>
//             <input type="number" id="lightStartHour" name="lightStartHour" min="0" max="23" value="%LIGHT_START_HOUR%"><br>
//             <label for="lightStartMinute">Phút bắt đầu đèn:</label>
//             <input type="number" id="lightStartMinute" name="lightStartMinute" min="0" max="59" value="%LIGHT_START_MINUTE%"><br>
//             <label for="lightEndHour">Giờ kết thúc đèn:</label>
//             <input type="number" id="lightEndHour" name="lightEndHour" min="0" max="23" value="%LIGHT_END_HOUR%"><br>
//             <label for="lightEndMinute">Phút kết thúc đèn:</label>
//             <input type="number" id="lightEndMinute" name="lightEndMinute" min="0" max="59" value="%LIGHT_END_MINUTE%"><br>
            
//             <input type="submit" value="Cập nhật Auto">
//         </form>
//         <br>
//     </body>
//     </html>
//   )rawliteral";

//     page.replace("%FAN_START_HOUR%", String(fanStartHour));
//     page.replace("%FAN_START_MINUTE%", String(fanStartMinute));
//     page.replace("%FAN_END_HOUR%", String(fanEndHour));
//     page.replace("%FAN_END_MINUTE%", String(fanEndMinute));
//     page.replace("%LIGHT_START_HOUR%", String(lightStartHour));
//     page.replace("%LIGHT_START_MINUTE%", String(lightStartMinute));
//     page.replace("%LIGHT_END_HOUR%", String(lightEndHour));
//     page.replace("%LIGHT_END_MINUTE%", String(lightEndMinute));

//     server.send(200, "text/html", page);
// }

// // Cập nhật chế độ Auto
// void handleSetAuto()
// {
//     if (server.method() == HTTP_POST)
//     {
//         fanStartHour = server.arg("fanStartHour").toInt();
//         fanStartMinute = server.arg("fanStartMinute").toInt();
//         fanEndHour = server.arg("fanEndHour").toInt();
//         fanEndMinute = server.arg("fanEndMinute").toInt();
//         lightStartHour = server.arg("lightStartHour").toInt();
//         lightStartMinute = server.arg("lightStartMinute").toInt();
//         lightEndHour = server.arg("lightEndHour").toInt();
//         lightEndMinute = server.arg("lightEndMinute").toInt();

//         fanAuto = (fanStartHour >= 0 && fanEndHour >= 0);
//         lightAuto = (lightStartHour >= 0 && lightEndHour >= 0);

//         server.send(200, "text/html", "<h2>Đã cập nhật chế độ Auto</h2><a href='/control'>Quay lại</a>");
//     }
// }

// // Cài đặt hệ thống
// void handleSystemSettingsPage()
// {
//     String html = R"====(
//   <html>
//   <head>
//     <meta charset='UTF-8'>
//     <meta name='viewport' content='width=device-width, initial-scale=1.0'>
//     <title>Cài đặt Hệ thống</title>
//     <style>
//       body { font-family: Arial, sans-serif; background-color: #f5f5f5; color: #333; }
//       .container { max-width: 500px; margin: 0 auto; padding: 20px; background-color: #fff; border-radius: 10px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2); }
//       .input-group { margin: 10px 0; }
//       label { display: block; margin-bottom: 5px; font-weight: bold; color: #00796b; }
//       input, select { width: 100%; padding: 8px; border-radius: 5px; border: 1px solid #ddd; }
//       .btn { margin-top: 10px; background-color: #00796b; color: white; padding: 10px; border: none; border-radius: 5px; cursor: pointer; }
//       .btn:hover { background-color: #004d40; }
//     </style>
//   </head>
//   <body>
//     <div class="container">
//       <h2>Cài đặt Hệ thống</h2>
//       <form action='/save-settings' method='POST'>
//         <div class="input-group">
//           <label>Đổi tên WiFi:</label>
//           <input type="text" name="wifiName" placeholder="Tên WiFi">
//         </div>
//         <div class="input-group">
//           <label>Nhập mật khẩu WiFi:</label>
//           <input type="password" name="wifiPassword" placeholder="Mật khẩu WiFi">
//         </div>
//         <div class="input-group">
//           <label>Time Server:</label>
//           <select name="timeServer">
//             <option value="pool">Pool</option>
//             <option value="windows">Windows</option>
//             <option value="google">Google</option>
//           </select>
//         </div>
//         <div class="input-group">
//           <label>Múi giờ:</label>
//           <input type="number" name="timezone" placeholder="Múi giờ (GMT)">
//         </div>
//         <button type="submit" class="btn">Lưu cài đặt</button>
//       </form>
//     </div>
//   </body>
//   </html>
//   )====";
//     server.send(200, "text/html", html);
// }

// // Hàm thiết lập màu RGB
// void setRGBColor(int red, int green, int blue, int brightness)
// {
//     analogWrite(RED_PIN, (red * brightness) / 255);
//     analogWrite(GREEN_PIN, (green * brightness) / 255);
//     analogWrite(BLUE_PIN, (blue * brightness) / 255);
// }

// // Hàm chế độ thông minh RGB
// void smartRGBMode(float lux)
// {
//     if (lux <= 50)
//     {
//         redValue = 255;
//         greenValue = 255;
//         blueValue = 255;
//         brightness = 255;
//     }
//     else if (lux > 50 && lux <= 500)
//     {
//         redValue = 100;
//         greenValue = 100;
//         blueValue = 255;
//         brightness = 150;
//     }
//     else
//     {
//         redValue = 0;
//         greenValue = 100;
//         blueValue = 255;
//         brightness = 100;
//     }
//     setRGBColor(redValue, greenValue, blueValue, brightness);
// }

// // Giao diện chế độ thủ công
// void handleManualRGBPage()
// {
//     String page = R"rawliteral(
//     <!DOCTYPE html>
//     <html lang="vi">
//     <head><meta charset="UTF-8"><title>RGB Thủ công</title></head>
//     <body>
//         <h2>Chọn màu sắc và độ sáng</h2>
//         <form action="/setManualRGB" method="POST">
//             R: <input type="range" name="red" min="0" max="255" value="%RED%"> <br>
//             G: <input type="range" name="green" min="0" max="255" value="%GREEN%"> <br>
//             B: <input type="range" name="blue" min="0" max="255" value="%BLUE%"> <br>
//             Brightness: <input type="range" name="brightness" min="0" max="255" value="%BRIGHTNESS%"> <br>
//             <input type="submit" value="Cập nhật">
//         </form>
//     </body>
//     </html>
//   )rawliteral";

//     page.replace("%RED%", String(redValue));
//     page.replace("%GREEN%", String(greenValue));
//     page.replace("%BLUE%", String(blueValue));
//     page.replace("%BRIGHTNESS%", String(brightness));
//     server.send(200, "text/html", page);
// }

// // Cập nhật RGB thủ công
// void handleSetManualRGB()
// {
//     if (server.hasArg("red"))
//         redValue = server.arg("red").toInt();
//     if (server.hasArg("green"))
//         greenValue = server.arg("green").toInt();
//     if (server.hasArg("blue"))
//         blueValue = server.arg("blue").toInt();
//     if (server.hasArg("brightness"))
//         brightness = server.arg("brightness").toInt();

//     setRGBColor(redValue, greenValue, blueValue, brightness);
//     server.send(200, "text/plain", "Đã cập nhật!");
// }

// // Giao diện chế độ thông minh
// void handleSmartRGBPage()
// {
//     float lux = lightMeter.readLightLevel();
//     smartRGBMode(lux);
//     server.send(200, "text/html", "<h2>Chế độ thông minh</h2><p>Lux: " + String(lux) + "</p>");
// }

// void setup()
// {
//     Serial.begin(9600);
//     Wire.begin();
//     WiFi.begin(ssid, password);
//     while (WiFi.status() != WL_CONNECTED)
//     {
//         delay(1000);
//         Serial.println("Đang kết nối WiFi...");
//     }
//     if (!bme.begin(0x76))
//     {
//         Serial.println("Không tìm thấy cảm biến BME280.");
//     }

//     if (!lightMeter.begin())
//     {
//         Serial.println("Không thể kết nối với cảm biến BH1750!");
//         while (1)
//             ;
//     }
//     lcd.init();
//     lcd.backlight();
//     Serial.println("Kết nối thành công!");
//     Serial.println(WiFi.localIP());
//     lcd.setCursor(0,0);
//     lcd.println("   WiFi Connected   ");
//     lcd.setCursor(0,1);
//     lcd.println(WiFi.localIP());
//     configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
//     struct tm timeinfo;
//     if (!getLocalTime(&timeinfo))
//     {
//         Serial.println("Failed to obtain time from NTP");
//         return;
//     }

//     setTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_wday, timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900 - 2000);

//     pinMode(RELAY1, OUTPUT);
//     pinMode(RELAY2, OUTPUT);
//     pinMode(RED_PIN, OUTPUT);
//     pinMode(GREEN_PIN, OUTPUT);
//     pinMode(BLUE_PIN, OUTPUT);
//     // Thiết lập các route
//     server.on("/", handleRoot);
//     server.on("/settings", handleSystemSettingsPage);
//     server.on("/login", handleLogin);
//     server.on("/control", handleControl);
//     server.on("/manual", handleManualMode); // Route cho Manual Mode
//     server.on("/toggle", handleToggleRelay);
//     // Cấu hình server
//     server.on("/smart", HTTP_GET, handleSmartPage);    // Trang chế độ Smart
//     server.on("/setSmart", HTTP_POST, handleSetSmart); // Cập nhật chế độ Smart
//     server.on("/auto", HTTP_GET, handleAutoPage);      // Trang chế độ Auto
//     server.on("/setAuto", HTTP_POST, handleSetAuto);   // Cập nhật chế độ Auto
//     server.on("/data", HTTP_GET, handleDataPage);
//     server.on("/manualRGB", handleManualRGBPage);
//     server.on("/setManualRGB", HTTP_POST, handleSetManualRGB);
//     server.on("/smartRGB", handleSmartRGBPage);
//     server.begin();
//     Serial.println("Server đã sẵn sàng.");
// }

// void loop()
// {
//     server.handleClient();
//     float currentTemp = bme.readTemperature(); // Ví dụ nhiệt độ là 28°C
//     controlRelays();
// }
