# Temperature-Sensor-DHT20

## Overview

This project is about reading from a temperature sensor DHT20 using STM32F407G-DISC1 (STM32F407VG MCU). DHT20 uses a standard I2C data output signal format. No hardware abstaction library was used, everything was written from scratch. Including GPIO, I2C, RCC, Timer, STM32F407VG and DHT20 drivers.

## Technical Aspects

The code requires multiple stuff to be written in order to get DHT20 to work:
* The neccessary drivers in order to use the MCU peripherals,
* DHT20 drivers, which uses I2C in order to send and receive data.

In order to test the data being sent and received, I used a logic analyzer.

## DHT20 datasheet
There are a couple steps to be taken in order to get the DHT20 to work. The first thing to do is connect the DHT20 with the board, and then write the code. 

Reading the datasheet, the way to connect with the DHT20 sensor is as the image implies.
![image](https://github.com/user-attachments/assets/17e9108c-a3c6-40c6-a098-083ebe9e4e77)

These are the steps covered in order to read the data from the sensor:
![image](https://github.com/user-attachments/assets/1e2616e1-d3a4-42d9-98ab-e42090bd0863)

This is how the data looks like, sending the measuerment command and then reading the data:

![image](https://github.com/user-attachments/assets/17c6117e-bb86-42ce-b1d2-d5e97f53f55f)

![image](https://github.com/user-attachments/assets/f4cdb6dd-d338-4d3c-bbee-b7f967951812)

