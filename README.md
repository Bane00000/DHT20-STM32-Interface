# Temperature-Sensor-DHT20

This project is about reading from a temperature sensor DHT20 using STM32F407G-DISC1 (STM32F407VG MCU). DHT20 uses a standard I2C data output signal format. No hardware abstaction library was used, everything was written from scratch. Including GPIO, I2C, RCC, Timer, STM32F407VG and DHT20 drivers.

Reading the datasheet, we see how to connect with the DHT20 sensor. For SCl and SDA pins, I used PB6 and PB7:
![image](https://github.com/user-attachments/assets/17e9108c-a3c6-40c6-a098-083ebe9e4e77)

These are the steps covered in order to read the data from the sensor:
![image](https://github.com/user-attachments/assets/1e2616e1-d3a4-42d9-98ab-e42090bd0863)

This is how the data is sent and read:

![image](https://github.com/user-attachments/assets/17c6117e-bb86-42ce-b1d2-d5e97f53f55f)
![image](https://github.com/user-attachments/assets/f4cdb6dd-d338-4d3c-bbee-b7f967951812)

