/**
 * Copyright (c) 2019 by Botorabi. All rights reserved.
 * https://github.com/botorabi/SensorStation
 *
 * License: MIT License (MIT), read the LICENSE text in
 *          main directory for more details.
 */

#include <Arduino.h>
#include "sensor.h"

Sensor::Sensor()
{
  for(int i = 0; i < HISTORY_MAX_ENTRIES; i++)
    historyEntries[i] = -1;
}

Sensor::~Sensor()
{
}

void Sensor::setup(const String& sensorName, int gpio)
{
  this->sensorName = sensorName;
  this->gpio = gpio;
}

const String& Sensor::name() const
{
  return sensorName;
}

short Sensor::currentValue()
{
  return analogRead(gpio);  
}

void Sensor::addHistoryEntry()
{
  historyEntries[currentHistoryPosition] = currentValue();
  currentHistoryPosition = (currentHistoryPosition + 1) % HISTORY_MAX_ENTRIES;
}

void Sensor::history(short* values)
{
  for (int i = 0; i < HISTORY_MAX_ENTRIES; i++)
  {
    values[i] = historyEntries[(i + currentHistoryPosition) % HISTORY_MAX_ENTRIES];
  }
}

String Sensor::historyAsString()
{
  String valueString;
  for (int i = 0; i < HISTORY_MAX_ENTRIES; i++)
  {
    if (i > 0)
      valueString += ",";

    valueString += historyEntries[(i + currentHistoryPosition) % HISTORY_MAX_ENTRIES];
  }

  return valueString;
}
