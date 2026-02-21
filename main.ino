#define BLYNK_TEMPLATE_ID "TMPL3FEQgMCYN"
#define BLYNK_TEMPLATE_NAME "Advance Health Monitoring System"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include "MAX30105.h"

// -------- WIFI CREDENTIALS --------
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

// -------- PIN DEFINITIONS --------
#define MQ135_PIN        34
#define AD8232_OUT       35
#define AD8232_LO_PLUS    4
#define AD8232_LO_MINUS   5

// -------- OBJECTS --------
MAX30105 particleSensor;
BlynkTimer timer;

// -------- VARIABLES --------
float ecgVoltage = 0;
float spo2Value = 0;
int   scaledAQI = 0; // Changed from aqiValue to match your logic
int  life = 0;
bool  leadsOff = false;
long  irValue = 0;
long  redValue = 0;

// -------- SENSOR FUNCTION --------
void readSensors() {

  // -------- ECG --------
  leadsOff = digitalRead(AD8232_LO_PLUS) || digitalRead(AD8232_LO_MINUS);
  if (!leadsOff) {
    ecgVoltage = (analogRead(AD8232_OUT) / 4095.0) * 3300.0;
  } else {
    ecgVoltage = 0;
  }

  // -------- SpO2 --------
  irValue = particleSensor.getIR();
  redValue = particleSensor.getRed();

  if (irValue > 20000) {
    float ratio = (float)redValue / (float)irValue;
    spo2Value = 110 - (25 * ratio);
    spo2Value = constrain(spo2Value, 0, 100);
  } else {
    spo2Value = 0;
  }

  // -------- YOUR MODIFIED MQ135 CODE --------
  int rawValue = analogRead(MQ135_PIN);
  scaledAQI = rawValue/10.8 ; // Applying your x3 multiplier

  String qualityStatus = "";
  String life = "";
  if (scaledAQI < 150) {
    qualityStatus = "Excellent / Normal life (Fresh Air)";
    life="0";
  } else if (scaledAQI < 350) {
    qualityStatus = "Good / / Normal";
    life="-3 ";
  } else if (scaledAQI < 500) {
    qualityStatus = "Moderate  (Stale Air)";
    life="-5";
  } else if (scaledAQI < 700) {
    qualityStatus = "Poor /  (Ventilation Required)";
    life="-8";
  } else if (scaledAQI < 900) {
    qualityStatus = "Unhealthy / High CO2";
    life="-20";
  } else {
    qualityStatus = "HAZARDOUS / death in seconds / SMOKE!";
  }

  // -------- SERIAL OUTPUT --------
  Serial.print("ECG: ");
  Serial.print(ecgVoltage);
  Serial.print(" mV | SpO2: ");
  Serial.print(spo2Value);
  Serial.print(" % | AQI Score: "); // Removed raw value as requested
  Serial.print(scaledAQI);
  Serial.print("\t Status: ");
  Serial.println(qualityStatus);
  Serial.println(life);

  // -------- SEND TO BLYNK --------
  Blynk.virtualWrite(V0, ecgVoltage); // ECG
  Blynk.virtualWrite(V1, spo2Value);  // SpO2
  Blynk.virtualWrite(V2, scaledAQI);   // AQI Score (multiplied value)
  Blynk.virtualWrite(V3, life);
}

// -------- SETUP --------
void setup() {
  Serial.begin(115200);

  pinMode(AD8232_LO_PLUS, INPUT);
  pinMode(AD8232_LO_MINUS, INPUT);
  pinMode(MQ135_PIN, INPUT);

  Wire.begin(21, 22);

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("❌ MAX30102 not found!");
    while (1);
  }

  // Reduced brightness to prevent saturation
particleSensor.setup(60, 4, 2, 100, 118, 4096);

  // -------- BLYNK START --------
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(1000L, readSensors);

  Serial.println("✅ Blynk Connected");
}

// -------- LOOP --------
void loop() {
  Blynk.run();
  timer.run();
}
