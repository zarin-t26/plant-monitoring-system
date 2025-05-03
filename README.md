# 🌱 Smart Plant Monitoring System

An Arduino-based smart plant monitoring and watering system designed to keep your plants healthy by automatically detecting soil moisture and activating a water pump when necessary.

## 🚀 Features

- Monitors soil moisture levels using a sensor.
- Displays moisture level and pump status on a 16x2 LCD.
- Activates a water pump when the soil is too dry.
- Simple and cost-effective for home gardening automation.

## 🛠 Hardware Components

- Arduino Uno (or compatible board)
- Soil Moisture Sensor
- 16x2 LCD Display (I2C)
- Relay Module
- Water Pump
- 10kΩ Resistor (for analog reading stabilization, if needed)
- Jumper wires and breadboard
- External power supply for the pump (e.g., 5V–12V based on pump specs)

## 🧠 How It Works

1. The soil moisture sensor reads the soil condition.
2. If the soil is dry (below a threshold), the relay is activated to power the water pump.
3. The LCD shows current moisture levels and pump activity status.

## 📟 LCD Display Output

- Shows real-time moisture readings.
- Indicates whether the pump is ON or OFF.

## 🧾 Code Overview

The main logic is inside the `loop()` function:

```cpp
int moisture = analogRead(A0);
if (moisture < threshold) {
    digitalWrite(relayPin, LOW);  // Turn on the pump
    lcd.print("Pump: ON");
} else {
    digitalWrite(relayPin, HIGH); // Turn off the pump
    lcd.print("Pump: OFF");
}
