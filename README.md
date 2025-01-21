# DHT20 - Temperature and humidity sensor

## Overview

This project involves interfacing the DHT20 temperature and humidity sensor with the STM32F407G-DISC1 development board, which features the STM32F407VG microcontroller. The DHT20 sensor communicates via the I²C protocol, and this project implements the entire communication process from scratch without relying on any hardware abstraction libraries (HAL).

The project includes:
* **GPIO Configuration:** Manual configuration of the General-Purpose Input/Output pins for sensor communication.
* **I²C Communication:** Full implementation of the I²C protocol to communicate with the DHT20, including signal timing, reading sensor data, and error handling.
* **RCC (Reset and Clock Control):** Direct configuration of the microcontroller's clock system to ensure correct timing for sensor communication and system stability.
* **Timer Implementation:** Use of timers for precise delays and synchronization needed for the sensor data collection process.
* **Drivers:** Custom drivers written for both the STM32F407VG microcontroller and the DHT20 sensor, focusing on low-level control and efficiency.

By avoiding HAL libraries, this project provides a deeper understanding of microcontroller programming and low-level hardware control, focusing on bare-metal programming principles. It also demonstrates debugging techniques using a logic analyzer to ensure accurate data transmission over the I²C bus.

## Technical Aspects

To successfully interface with the DHT20 temperature and humidity sensor using the STM32F407G-DISC1 (STM32F407VG), several technical components were developed and configured. These include low-level drivers and direct management of microcontroller peripherals:

1. **MCU Peripheral Drivers:**
Custom drivers were written to interface with the STM32F407VG's peripherals. This included configuring the General-Purpose Input/Output (GPIO) pins for communication with the DHT20 sensor and setting up the I²C bus to ensure reliable data transfer. The code directly controls the microcontroller's Reset and Clock Control (RCC) to manage the clock system for the correct timing and synchronization of operations.

2. **DHT20 Driver:**
A dedicated driver was created for the DHT20 sensor, which communicates over the I²C protocol. The driver implements the full I²C communication protocol, including:
* **Start/Stop Conditions:** Handling I²C bus conditions to initiate and terminate data transfers.
* **Data Transmission:** Sending read/write requests to the DHT20 and retrieving temperature and humidity data.
* **Error Handling:** Ensuring reliable communication by managing potential transmission errors, such as timeouts or corrupted data.

3. **Timer and Delay Management:**
Timers were used to introduce necessary delays for proper sensor data sampling and communication timing, particularly in line with the sensor's required read interval. This allows for accurate data readings and ensures synchronization with the I²C protocol.

4. **Debugging and Testing with Logic Analyzer:**
To verify the I²C communication and ensure proper data transmission between the microcontroller and the sensor, a logic analyzer was used. This tool allowed real-time monitoring of the data sent and received over the I²C bus, providing valuable insights into the signal integrity and data consistency. The analyzer helped identify and troubleshoot timing issues and communication errors during development.

## Interfacing with DHT20 Sensor
To get the DHT20 working, the first step is to connect the sensor to the development board, followed by writing the necessary code to interface with it.

Referring to the datasheet, the DHT20 sensor should be connected as shown in the following image.
![image](https://github.com/user-attachments/assets/17e9108c-a3c6-40c6-a098-083ebe9e4e77)

The following steps outline the process for reading data from the DHT20 sensor:
![image](https://github.com/user-attachments/assets/1e2616e1-d3a4-42d9-98ab-e42090bd0863)

This is the data flow: first, the measurement command is sent, followed by reading the data:
![image](https://github.com/user-attachments/assets/17c6117e-bb86-42ce-b1d2-d5e97f53f55f)

![image](https://github.com/user-attachments/assets/f4cdb6dd-d338-4d3c-bbee-b7f967951812)

## Acknowledgments
The drivers used in this project (EXCEPT DRIVERS FOR TIMER AND DHT20) were developed as part of the course I followed on STM32 programming. These files provided the foundation for configuring the STM32F407VG microcontroller's peripherals and implementing the communication protocols.
**Course link:** https://www.udemy.com/course/mastering-microcontroller-with-peripheral-driver-development/

## References
**DHT20 product and datasheet:** https://www.sparkfun.com/humidity-and-temperature-sensor-dht20.html
