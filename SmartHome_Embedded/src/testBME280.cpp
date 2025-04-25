// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>
// #include <Adafruit_Sensor.h>
// #include <Adafruit_BME280.h>
// #include <SPI.h>
// #include <SD.h>
// #include "ds1307.hpp"

// LiquidCrystal_I2C lcd(0x27, 20, 4); // LCD I2C address (update if needed)
// Adafruit_BME280 bme;
// extern int second, minute, hour, day, wday, month, year;

// void setup()
// {
//     Serial.begin(9600);
//     lcd.init();
//     lcd.backlight();

//     // Initialize I2C
//     Wire.begin();

//     // Initialize BME280
//     if (!bme.begin(0x76))
//     {
//         Serial.println("Không tìm thấy cảm biến BME280.");
//     }

//     setTime(10, 12, 0, 5, 5, 12, 24);
// }

// void loop()
// {
//     // Read time from DS1307
//     readDS1307();

//     // Format time for display
//     char timeBuffer[20];
//     snprintf(timeBuffer, sizeof(timeBuffer), "%02d/%02d/%04d %02d:%02d:%02d",
//              day, month, year + 2000, hour, minute, second);

//     // Get BME280 sensor readings
//     float temperature = bme.readTemperature();
//     float humidity = bme.readHumidity();
//     float pressure = bme.readPressure() / 100.0F;

//     // Display time and sensor data on the LCD
//     // lcd.setCursor(0, 0);
//     Serial.print(timeBuffer); // Date and time on the first line

//     //lcd.setCursor(0, 1);
//     Serial.printf("Temp: %.2f C", temperature); // Temperature on the second line

//     //lcd.setCursor(0, 2);
//     Serial.printf("Humidity: %.2f %%", humidity); // Humidity on the third line

//     //lcd.setCursor(0, 3);
//     Serial.printf("Pressure:%.2fhPa", pressure); // Pressure on the fourth line

//     delay(1000); // Update every second
// }
