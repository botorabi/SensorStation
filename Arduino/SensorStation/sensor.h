/**
 * Copyright (c) 2019 by Botorabi. All rights reserved.
 * https://github.com/botorabi/SensorStation
 *
 * License: MIT License (MIT), read the LICENSE text in
 *          main directory for more details.
 */

#ifndef SENSOR_H
#define SENSOR_H


class Sensor
{

  public:

    static const int           HISTORY_MAX_ENTRIES = 24 * 7;
    static const unsigned long SENSOR_HISTORY_INTERVAL = 1000 * 60 * 60;


                    Sensor();

                    ~Sensor();

    void            setup(const String& sensorName, int gpio);

    const String&   name() const;

    short           currentValue();

    void            addHistoryEntry();

    /**
     * Get the sensor value history. The values buffer must be at least of length HISTORY_MAX_ENTRIES.
     * Value -1 means the sensor value entry does not exist yet.
     */
    void            history(short* values);

    /**
     * Get the sensor history as a comma-separated string.
     */
    String          historyAsString();

  protected:

    String          sensorName;

    int             gpio {-1};

    short           historyEntries[HISTORY_MAX_ENTRIES];

    int             currentHistoryPosition {0};
};

#endif /* SENSOR_H */
