/**
 * Copyright (c) 2019 by Botorabi. All rights reserved.
 * https://github.com/botorabi/SensorStation
 *
 * License: MIT License (MIT), read the LICENSE text in
 *          main directory for more details.
 */

#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <ESPAsyncWebServer.h>
#include "sensor.h"

class HTTPServer
{
  public:
  
                HTTPServer();

                ~HTTPServer();

    void        configureSensors(Sensor** sensors, int countSensors);

    void        removeAllSensors();

    void        start(int port);

    bool        isRunning() const;

    void        stop();

  protected:

    AsyncWebServer*         webServer {nullptr};

    Sensor**                sensors {nullptr};

    String                  indexFileContent;

    int                     countSensors {0};

    String                  extractBaseRefFromXHostHeader(AsyncWebServerRequest* request) const;

    static void             handleNotFound(AsyncWebServerRequest* request);

    void                    handleRoot(AsyncWebServerRequest* request);

    void                    handleSensorJSON(AsyncWebServerRequest* request);

    void                    handleFavIcon(AsyncWebServerRequest* request);
};

#endif /* HTTPSERVER_H */
