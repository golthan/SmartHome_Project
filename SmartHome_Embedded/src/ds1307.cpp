#include "ds1307.hpp"
#include <Wire.h>

const byte DS1307 = 0x68;
const byte NumberOfFields = 7;

int second, minute, hour, day, wday, month, year;

void readDS1307()
{
  Wire.beginTransmission(DS1307);
  Wire.write((byte)0x00);
  Wire.endTransmission();
  Wire.requestFrom(DS1307, NumberOfFields);

  second = bcd2dec(Wire.read() & 0x7f);
  minute = bcd2dec(Wire.read());
  hour = bcd2dec(Wire.read() & 0x3f); // chế độ 24h.
  wday = bcd2dec(Wire.read());
  day = bcd2dec(Wire.read());
  month = bcd2dec(Wire.read());
  year = bcd2dec(Wire.read());
}

int bcd2dec(byte num)
{
  return ((num / 16 * 10) + (num % 16));
}

int dec2bcd(byte num)
{
  return ((num / 10 * 16) + (num % 10));
}

// void digitalClockDisplay()
// {
//   lcd.setCursor(0, 0);
//   lcd.print(hour);
//   printDigits(minute);
//   //printDigits(second);
//   lcd.print(" ");
//   lcd.print(day);
//   lcd.print("/");
//   lcd.print(month);
//   lcd.print("/");
//   lcd.print(year);
//   lcd.println();
// }

// void printDigits(int digits)
// {
//   lcd.print(":");

//   if (digits < 10)
//     lcd.print('0');
//   lcd.println(digits);
// }

void setTime(byte hr, byte min, byte sec, byte wday, byte date, int mth, int yr)
{
  Wire.beginTransmission(DS1307);
  Wire.write(byte(0x00)); 
  Wire.write(dec2bcd(sec));
  Wire.write(dec2bcd(min));
  Wire.write(dec2bcd(hr));
  Wire.write(dec2bcd(wday)); 
  Wire.write(dec2bcd(date));
  Wire.write(dec2bcd(mth));
  Wire.write(dec2bcd(yr));
  Wire.endTransmission();
}