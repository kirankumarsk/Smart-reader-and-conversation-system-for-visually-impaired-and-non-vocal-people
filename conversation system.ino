#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
void setup(void)
{
Serial.begin(9600);
mySerial.begin(9600);
if(!accel.begin())
{
Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
while(1);
}
accel.setRange(ADXL345_RANGE_16_G);
}
void loop(void)
{
sensors_event_t event;
accel.getEvent(&event);
int x =event.acceleration.x;
int y=event.acceleration.y;
int z=event.acceleration.z;
Serial.print("X: "); Serial.print(x); Serial.print(",");
Serial.print("Y: "); Serial.print(y); Serial.print(",");
Serial.print("Z: "); Serial.println(z);
int fore_finger = analogRead(2);
int middle_finger = analogRead(3);
int ff = map(fore_finger, 880, 780, 0, 100);
int mf = map(middle_finger, 880, 780, 0, 100);
Serial.print("FF: "); Serial.print(ff); Serial.print(",");
Serial.print("MF: "); Serial.println(mf);
if(ff <3 && mf <22)
{
Serial.println("I need water");
}
if(y>x && y>z && ff>25)
{Serial.println("I need water");
mySerial.print("1");
delay(2000);
}
if(y>x && y>z && mf>45)
{
Serial.println("I need food");
mySerial.print("2");
delay(2000);
}
delay(1000);
}