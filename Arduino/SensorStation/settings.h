/**
 * Copyright (c) 2019 by Botorabi. All rights reserved.
 * https://github.com/botorabi/SensorStation
 *
 * License: MIT License (MIT), read the LICENSE text in
 *          main directory for more details.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <Preferences.h>


class Settings
{

  public:

    static void     saveCredentials(const String& ssid, const String& psk);

    static void     loadCredentials(String& ssid, String& psk);

    static void     saveOTAFlag(bool enable);

    static bool     loadOTAFlag();

  protected:

    static bool     open(Preferences& preferences, bool readOnly);

    static void     close(Preferences& preferences);
};

#endif /* SETTINGS_H */
