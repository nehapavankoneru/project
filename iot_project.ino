#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

SoftwareSerial bluetooth(10, 11); // RX, TX

PulseOximeter pox;

uint32_t tsLastReport = 0;
uint8_t bpm = 0;
uint8_t spo2 = 0;

void onBeatDetected()
{
  Serial.println("Beat!");
}

void setup()
{
  Serial.begin(9600);

  if (!pox.begin())
  {
    Serial.println("MAX30100 was not found. Please check wiring/power.");
    while (1);
  }

  pox.setOnBeatDetectedCallback(onBeatDetected);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Spo2: --%");
  display.println("BPM: -- ");
  display.display();

  bluetooth.begin(9600);

  Serial.println("Initializing...");
}

void loop()
{
  pox.update();

  if (millis() - tsLastReport > 1000)
  {
    bpm = pox.getHeartRate();
    spo2 = pox.getSpO2();
    Serial.print("Heart rate:");
    Serial.print(bpm);
    Serial.print(" bpm / SpO2:");
    Serial.print(spo2);
    Serial.println("%");
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Spo2: " + String(spo2) + "%");
    display.println("BPM: " + String(bpm));
    display.display();

    bluetooth.print(String(spo2) + "," + String(bpm) + "\n");
    
    tsLastReport = millis();
  }
}
