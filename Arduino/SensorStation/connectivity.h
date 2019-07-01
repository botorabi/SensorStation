/**
 * Copyright (c) 2019 by Botorabi. All rights reserved.
 * https://github.com/botorabi/SensorStation
 *
 * License: MIT License (MIT), read the LICENSE text in
 *          main directory for more details.
 */

#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H

#include <WiFi.h>
#include <esp_wps.h>


class Connectivity
{
  public:

                      Connectivity();

                      ~Connectivity();

    bool              isConnected() const;

    /**
     * Try to connect to WiFi network and return false if it fails.
     * This method call is blocking.
     */
    bool              connect();

    /**
     * If a connection failed then try to setup a WiFi network connection by WPS.
     * Starting a new WPS will erase currently stored WiFi credentials.
     */
    void              startWPSConnection();

    bool              isWPSInProgress() const;

    /**
     * Get the local IP, valid only after a successful connection.
     */
    IPAddress         localIP() const;

  protected:

    static void       storeCredentials(const String& ssid, const String& psk);

    static void       restoreCredentials(String& ssid, String& psk);

    void              setupWPS();

    static void       wifiEvent(WiFiEvent_t event, system_event_info_t info);
};

#endif /* CONNECTIVITY_H */
