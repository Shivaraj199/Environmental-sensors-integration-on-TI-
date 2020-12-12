# Environmental-sensors-integration-on-TI-
INTRODUCTION:
 This project aims to explore the world of normally-off/instant-on features for IoT applications. 
 We’ll be using low power modes that are shutting down most of the modules of the microcontroller to reduce the power consumption of the device so it can work with
 an irregular power supply or even without it for a long period of time.
 But it has to be efficient enough so the application to obtain desired performances. In this project we tried to implement an application that is powered during 
 the day by a solar panel and must survive the night without any power supply. 
 It’s goal is to get values from different kinds of environmental sensors. 
 Air quality, temperature, pressure, and more... This kind of application has been made possible thanks to the advent 
 of non volatile memory such as FRAM (ferroelectric RAM). 
 The fact that this memory has attained so small latencies and power consumption makes it a good choice for low power devices and is expected to be a key component 
 for future wireless products. In addition to FRAM we’ll be using modules such as real time clock (RTC) and low power modes that are implemented in this 
 microcontroller. And of course, the supercapacitor that is the only reason that the board can still be powered for some time even without power supply 
 We’ll focus on the energy consumption on the low power mode 4 (LPM4). We’ll use two kind of sensors,
 the Adafruit BME680 that is a temperature/pressure/humidity sensor and the SPG30 that is a gas sensor.
