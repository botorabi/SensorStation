/**
 * Copyright (c) 2019 by Botorabi. All rights reserved.
 * https://github.com/botorabi/SensorStation
 *
 * License: MIT License (MIT), read the LICENSE text in
 *          main directory for more details.
 */

#ifndef CORE_H
#define CORE_H

#include "sensor.h"
#include "connectivity.h"
#include "httpserver.h"
#include "display.h"


class Core
{
  public:
                      Core();

                      ~Core();

    void              setSplashText(const String& text);

    void              setAppVersion(const String& version);

    void              initialize(int httpPort = 80, int displayOffTimeout = 10000);

    void              update();

    void              enableOTAUpdate();

    void              startWPS();

    bool              isWPSActive() const;

    void              displayTurn(bool on);

    void              displaySetStatus(const String& text);

    void              displaySetFooter(const String& text);

    void              displayRestore();

  protected:

    void              setupDisplay();

    void              setupWiFi();

    void              setupSensors();

    Sensor*           createMoistureSensor(const String& name, int pin) const;

    void              updateHTTPServer();

    void              updateSensors();

    void              addSensorEntry();

    void              updateSensorDisplay();

    String            splashText;

    String            appVersion;

    Sensor**          sensors {nullptr};
    int               countSensors {0};
    unsigned long     lastSensorValueUpdate {0};
    unsigned long     lastSensorDisplayUpdate {0};
    unsigned long     sensorDisplayInterval {2000};

    Connectivity      connectivity;

    HTTPServer        httpServer;
    int               httpPort {80};
    bool              otaUpdate {false};

    Display           display;
    int               displayOffTimeout {10000};
};

#endif /* CORE_H */
