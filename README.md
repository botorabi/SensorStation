    Copyright (c) 2019 by Botorabi. All rights reserved.
    https://github.com/botorabi/SensorStation

    License: MIT License (MIT)
    Read the LICENSE text in main directory for more details.

    First Created:     June 2019
    Author:            Botorabi (botorabi AT gmx DOT net)
                       https://vr-fun.net


# SensorStation

**SensorStation** is an Arduino project developed and tested on an ESP32 module and allows the storage and access to sensor data.
The project supports an OLED display and input buttons, and connects to a WiFi network using WPS.
An embedded webserver provides a REST API and web page for accessing the data.
It is possible to put the webserver behind a reverse-proxy, the webserver is aware of *X-Forwarded-Prefix* in HTML request header.
It is an easy task to integrate the SensorStation in  [HomieCenter](https://github.com/botorabi/HomieCenter).


# Build

Use the Arduino IDE to load the file Arduino/SensorStation/SensorStation.ino and build/upload it to your board.
The project is configured for an ESP32 NodeMCU Module with CP2102. If you want to try it on a different module,
you may need to adapt the IOs for display and pins.

The project uses a few libraries which are already setup in this repository: AsyncTCP, ESPAsyncWebServer, and OLED Driver for SSD1306.


# Customizing the Web Page

The web page source is located in folder *Arduino/SensorStation/resources/html/index.html*. You will need to run the build.sh script
in the folder *Arduino/SensorStation/resources* after changing the index.html file. It will prepare the HTML resources for the next build.


# Screenshot

![screenshot-01](https://user-images.githubusercontent.com/11502867/60448886-ca67c780-9c26-11e9-9d22-6c7ef6d6f9a2.png)

