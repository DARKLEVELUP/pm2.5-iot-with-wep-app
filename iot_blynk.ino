#define BLYNK_TEMPLATE_ID "TMPL6sW81VYex"
#define BLYNK_TEMPLATE_NAME "PM IOT"
#define BLYNK_AUTH_TOKEN "aayvLpSghbn9aiK1us1mvQqAlIFOn9FS"

// WiFi and Blynk
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// PMS Sensor & LCD
#include "PMS.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// WiFi credentials
char ssid[] = "SnowyZ";
char pass[] = "00000000";

// Blynk LED control
int ledpin = 2;

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// PMS7003 Sensor on Serial2
PMS pms(Serial2);
PMS::DATA data;

// Handle LED toggle from Blynk app (V0)
BLYNK_WRITE(V0)
{
  int value = param.asInt();
  digitalWrite(ledpin, value);
}

void setup() 
{
  Serial.begin(9600);
  Serial2.begin(9600);  // PMS7003 sensor on Serial2

  pinMode(ledpin, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting...");
  
  // Connect to WiFi and Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  lcd.clear();
}

void loop() 
{
  Blynk.run();

  // Read PM sensor data
  if (pms.read(data)) {
    // Debug print
    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_1_0);

    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(data.PM_AE_UG_2_5);

    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_10_0);

    // Display on LCD
    lcd.setCursor(0, 0);
    lcd.print("PM2.5 = ");
    lcd.setCursor(8, 0);
    lcd.print(data.PM_AE_UG_2_5);

    lcd.setCursor(0, 1);
    lcd.print("PM10 = ");
    lcd.setCursor(8, 1);
    lcd.print(data.PM_AE_UG_10_0);

    // Send PM2.5 value to Blynk (V1)
    Blynk.virtualWrite(V1, data.PM_AE_UG_2_5);

    // Send PM2.5 value to Blynk (V2)
    Blynk.virtualWrite(V2, data.PM_AE_UG_10_0);

    delay(2000); // Allow LCD to display before clearing
    lcd.clear();
  }
}
