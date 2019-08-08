/**
 * Copyright (c) 2019 by Botorabi. All rights reserved.
 * https://github.com/botorabi/SensorStation
 *
 * License: MIT License (MIT), read the LICENSE text in
 *          main directory for more details.
 */

#include <Arduino.h>
#include <Preferences.h>
#include "settings.h"


static const String PREFERENCE_NAMESPACE("sensors-wifi");


void Settings::saveCredentials(const String& ssid, const String& psk)
{
  Preferences preferences;
  if (!open(preferences, false))
    return;

  preferences.putString("ssid", ssid);
  preferences.putString("password", psk);

  close(preferences);
}

void Settings::loadCredentials(String& ssid, String& psk)
{
  Preferences preferences;
  if (!open(preferences, true))
    return;

  ssid = preferences.getString("ssid", "");
  psk = preferences.getString("password", "");

  close(preferences);
}

void Settings::saveOTAFlag(bool enable)
{
  Preferences preferences;
  if (!open(preferences, false))
    return;

  preferences.putBool("ota", enable);

  close(preferences);
}

bool Settings::loadOTAFlag()
{
  Preferences preferences;
  if (!open(preferences, true))
    return false;

  bool enable = preferences.getBool("ota", false);

  close(preferences);

  return enable;
}

bool Settings::open(Preferences& preferences, bool readOnly)
{
  if (!preferences.begin(PREFERENCE_NAMESPACE.c_str(), readOnly))
  {
    Serial.println("*** Problem occurred while opening preferences!");
    return false;
  }
  return true;  
}

void Settings::close(Preferences& preferences)
{
  preferences.end();
}
