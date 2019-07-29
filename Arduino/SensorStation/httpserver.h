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

    void        setVersionInfo(const String& versionInfo);

    void        start(int port);

    bool        isRunning() const;

    void        stop();

  protected:

    String                  extractBaseRefFromXHostHeader(AsyncWebServerRequest* request) const;

    static void             handleNotFound(AsyncWebServerRequest* request);

    void                    handleRoot(AsyncWebServerRequest* request);

    void                    handleSensorJSON(AsyncWebServerRequest* request);

    void                    handleFavIcon(AsyncWebServerRequest* request);

    AsyncWebServer*         webServer {nullptr};

    Sensor**                sensors {nullptr};

    String                  indexFileContent;

    int                     countSensors {0};

    String                  versionInfo;
};

#endif /* HTTPSERVER_H */
