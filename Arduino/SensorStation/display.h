/**
 * Copyright (c) 2019 by Botorabi. All rights reserved.
 * https://github.com/botorabi/SensorStation
 *
 * License: MIT License (MIT), read the LICENSE text in
 *          main directory for more details.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Wire.h>
#include <SSD1306.h>


class Display
{
  public:

                  Display();

                  ~Display();

    void          start();

    void          update();

    void          stop();

    void          clear();

    /**
     * Turn on/off the display.
     */
    void          turn(bool on);

    bool          isOn() const { return on; }

    /**
     * Enable automatic turning off display after given timeout (in milliseconds) when a text is set.
     * Set to 0 in order to disable auto-turn-off.
     */
    void          setAutoOff(int timeOut) { offTimeOut = timeOut; }

    void          setStatusText(const String& text);

    void          setMainText(const String& text);

    void          setFooterText(const String& text);

  protected:

    void          render();

    SSD1306*      display {nullptr};

    String        textStatus;

    String        textMain;

    String        textFooter;

    unsigned long lastUpdate {0ul};

    int           offTimeOut {0};

    unsigned long offTimeStamp {0};

    bool          on {true};
};

#endif /* DISPLAY_H */
