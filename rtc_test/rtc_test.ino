#include <SoftI2CMaster.h>

// Some test code to talk to MCP7940M

#define RTC_DEVICE_ID 0x6F // MCP7940 I2C ID

#define CALIBRATION_VALUE 0x2C // Roughly :)
#define R_OSCTRIM 0x8

const int sdaPin = 12;
const int sclPin = 11;

SoftI2CMaster i2c = SoftI2CMaster(sclPin, sdaPin, 0);

void setup() {
  Serial.begin(38400);
  i2c.beginTransmission(RTC_DEVICE_ID);
  i2c.write(R_OSCTRIM);
  i2c.write(CALIBRATION_VALUE);
  i2c.write(0);
  i2c.write(1 << 7); // ST = Start Oscillator
  i2c.endTransmission();
  Serial.println("All ready");
}


void loop() {
  i2c.beginTransmission(RTC_DEVICE_ID);
  i2c.write(0);
  int ret = i2c.endTransmission();
  if (ret == 0) {
    
    i2c.requestFrom(RTC_DEVICE_ID);
    byte secs = i2c.read();
    byte mins = i2c.read();
    byte hours = i2c.read();
    byte day = i2c.read();
    byte date = i2c.read();
    byte month = i2c.read();
    byte year = i2c.read();
    byte control = i2c.read();
    byte calibration = i2c.readLast();
    
    byte secs_lo = secs & 0xf;
    byte secs_hi = (secs >> 4) & 7;
    int  secs_val = (secs_hi * 10) + secs_lo;
    
    byte mins_lo = mins & 0xf;
    byte mins_hi = (mins >> 4) & 7;
    int  mins_val = (mins_hi * 10) + mins_lo;
    
    byte hours_lo = hours & 0xf;
    byte hours_ten = (hours >> 4) & 1;
    byte hours_ampm = (hours >> 5) & 1;
    byte hours_24 = (hours >> 6) & 1;
    int  hours_val = (hours_ten * 10) + hours_lo;
    
    byte dow = day & 7;

    byte date_lo = date & 0xf;
    byte date_hi = (date >> 4) & 3;
    int  date_val = (date_hi * 10) + date_lo;
    
    byte month_lo = month & 0xf;
    byte month_ten = (month >> 4) & 1;
    int  month_val = (month_ten * 10) + month_lo;
    
    byte year_lo = year & 0xf;
    byte year_hi = (year >> 4) & 0xf;
    int  year_val = (year_hi * 10) + year_lo;
    
    Serial.print(year_val); 
    Serial.print("/");
    Serial.print(month_val);
    Serial.print("/");
    Serial.print(date_val);
    Serial.print("-");
    Serial.print(dow);
    Serial.print(" ");
    Serial.print(hours_val);
    Serial.print(":");
    Serial.print(mins_val);
    Serial.print(":");
    Serial.print(secs_val);
    Serial.print(" C=");
    Serial.println(calibration);
  } else {
    Serial.print("no MCP7940 detected, ret = "); Serial.println(ret, HEX);
  }
  
  delay(1000);
}
