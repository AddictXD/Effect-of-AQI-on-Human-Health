🌫️ IoT-Based Health & AQI Monitoring System

📖 Project Overview

The objective of this project is to develop a comprehensive, real-time health monitoring system that correlates environmental air quality data with vital human physiological parameters. By using IoT technology, the system enables early detection of health risks associated with heart conditions and poor air quality.

Key Monitoring Outputs:

🌫️ AQI Score: Real-time air quality index tracking.


❤️ ECG (mV): Cardiac cycle visualization and lead-off detection.


🫁 SpO2 (%): Blood oxygen saturation levels.


⏳ Life Impact: Estimated life expectancy reduction based on real-time pollution exposure.

🛠️ System Architecture

Hardware Components

- Microcontroller: ESP32 (Dual-core, integrated WiFi/Bluetooth).

- Air Quality Sensor: MQ135 (Detects $NH_3$, $NO_x$, Alcohol, Benzene, Smoke, and $CO_2$).

- ECG Monitor: AD8232 Module (Biopotential signal conditioning block for noise filtering).

- Power Regulation: Regulated 5V and 3.3V DC supply for stable sensor reference voltages.

Software & Cloud

- Framework: Arduino IDE for ESP32.
  
- Dashboard: Blynk IoT Platform (Mobile and Web visualization).
  
- Communication: 2.4GHz WiFi using secure Auth Tokens to push data to Blynk virtual pins.
  
- Pulse Oximeter: MAX30102 (High-sensitivity SpO2 and Heart-Rate sensor).

📊 Analytical Models & Logic

AQI and Life Expectancy

The system maps analog sensor readings to health impact categories and calculates estimated life reduction based on continuous exposure:

| AQI Range | Condition | Life Reduction   |
| --------- | --------- | ---------------- |
| <150      | Normal    | 0 Years          |
| 150–350   | Good      | -3 Years         |
| 350–500   | Moderate  | -5 Years         |
| 500–700   | Poor      | -8 Years         |
| 700–900   | Unhealthy | -20 Years        |
| >900      | Hazardous | Immediate Danger |

🫁 SpO2 Calculation

Blood oxygen is calculated using the ratio ($ratio$) of the AC components of Red and Infrared light signals emitted into the fingertip:

$$SpO2 = 110 - (25 \times ratio)$$

📝 Configuration & Use

1. Pin Mapping: Connect sensors to the following ESP32 GPIOs:
   - MQ135: GPIO 34 (ADC).
   - AD8232: GPIO 35 (Output), GPIO 4/5 (LO+/-).
   - MAX30102: GPIO 21 (SDA), GPIO 22 (SCL).
     
2. Calibration: The MQ135 sensor requires a 24-48 hour "burn-in" time for pre-heating and stable readings.

3. Deployment: Enter your BLYNK_AUTH_TOKEN, ssid, and pass in the provided system code overview to establish a cloud connection.

📱 Blynk Dashboard

- Heart Rate graph

- SpO₂ value

- AQI index

- Life Reduction estimate

- Real-time cloud updates

 🔬 Features

✔ Real-time monitoring
✔ Cloud visualization
✔ AQI-based life impact estimation
✔ Low-cost IoT architecture
✔ Modular and scalable 
