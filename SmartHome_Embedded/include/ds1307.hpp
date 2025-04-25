#ifndef __DS1307_H_
#define __DS1307_H_

#include<Arduino.h>
#include <stdint.h>

void setTime(byte hr, byte min, byte sec, byte wday, byte date, int mth, int yr);
void readDS1307(void);
void digitalClockDisplay(void);
void printDigits(int digits);
int bcd2dec(byte num);
int dec2bcd(byte num);

#endif