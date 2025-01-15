# MQTT Greenhouse Monitoring System

This project demonstrates a Greenhouse Monitoring System using the Quectel BC660 module, BME280 sensor, and an Arduino-compatible microcontroller. The system collects temperature, humidity, pressure, and soil moisture data and publishes it to an MQTT broker. A potentiometer simulates soil moisture levels, and an LED provides a visual indicator.

## Table of Contents

- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Circuit Diagram](#circuit-diagram)
- [Setup and Installation](#setup-and-installation)
  - [Hardware Setup](#hardware-setup)
  - [Software Setup](#software-setup)
- [Usage](#usage)
- [MQTT Topics](#mqtt-topics)
- [Code Overview](#code-overview)
  - [Initialization](#initialization)
  - [Sensor Data Collection](#sensor-data-collection)
  - [MQTT Communication](#mqtt-communication)
- [Future Improvements](#future-improvements)
- [License](#license)

## Features

- Reads temperature, humidity, and pressure using the BME280 sensor.
- Simulates soil moisture levels using a potentiometer.
- Provides visual feedback using an LED.
- Publishes data to an MQTT broker.
- Supports SSL for secure MQTT communication.
- Simple JSON format for data transfer.

## Hardware Requirements

- Arduino-compatible microcontroller (e.g., ESP32, Arduino Uno with Serial-to-TTL adapter).
- Quectel BC660 module.
- BME280 sensor.
- Potentiometer.
- LED and resistor.
- Breadboard and jumper wires.

## Software Requirements

- [Arduino IDE](https://www.arduino.cc/en/software)
- Quectel BC660 library (custom library used in this project)
- BME280I2C library

## Circuit Diagram

Connect the components as follows:

1. **BME280**:
   - SDA -> Microcontroller SDA pin
   - SCL -> Microcontroller SCL pin
   - VCC -> 3.3V
   - GND -> GND

2. **Potentiometer**:
   - Center pin -> Analog pin (e.g., GPIO34)
   - Side pins -> 3.3V and GND

3. **LED**:
   - Positive leg -> GPIO4 (via a resistor)
   - Negative leg -> GND

4. **Quectel BC660**:
   - TX/RX -> Microcontroller serial pins
   - VCC -> 3.3V
   - GND -> GND

## Setup and Installation

### Hardware Setup

1. Assemble the components on a breadboard as per the circuit diagram.
2. Ensure proper connections for power and ground to all components.

### Software Setup

1. Install the required libraries in the Arduino IDE:
   - [BME280I2C library](https://github.com/finitespace/BME280)
   - Include the custom `Quectel_BC660_custom.h` in your project.

2. Open the provided source code in the Arduino IDE.
3. Update the following placeholders in the code:
   - `MQTT_IP`: Replace with your MQTT broker's IP address.
   - `MQTT_PORT`: Replace with your MQTT broker's port.
   - `MQTT_USERNAME`: Replace with your MQTT username.
   - `MQTT_PASSWORD`: Replace with your MQTT password.
   - `MQTT_TOPIC`: Replace with your MQTT topic.
4. Upload the code to your microcontroller.

## Usage

1. Power up the microcontroller and ensure all connections are secure.
2. Open the Arduino Serial Monitor to view initialization messages.
3. Observe the LED behavior based on the potentiometer's position:
   - LED ON: Simulated soil moisture is low.
   - LED OFF: Simulated soil moisture is sufficient.
4. Check the MQTT broker for published data in the specified topic.

## MQTT Topics

- **Publish**:
  - Topic: `Robbert/Greenhouse` (or the updated `MQTT_TOPIC` from the code)
  - Payload (JSON):
    ```json
    {
      "temperature": 24.32,
      "humidity": 45.76,
      "pressure": 101325,
      "moisture": 18.5
    }
    ```

## Code Overview

### Initialization

- The `setup` function initializes the Serial interface, BME280 sensor, and Quectel BC660 module.
- It establishes a network connection and sets up MQTT communication with SSL enabled.

### Sensor Data Collection

- The `getClimateData` function reads data from the BME280 sensor and potentiometer.
- It formats the sensor readings into a JSON string. If the potentiometer's value is below a certain threshold, the LED is turned ON; otherwise, it is turned OFF.

### MQTT Communication

- Data is published to the MQTT broker every 10 seconds in the `loop` function.
- The Quectel BC660 module handles the MQTT protocol and SSL configuration.

## Future Improvements

- Add support for multiple MQTT topics for enhanced functionality.
- Implement OTA updates for remote firmware upgrades.
- Integrate a real soil moisture sensor for more accurate readings.
- Add error handling and retries for MQTT communication.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

