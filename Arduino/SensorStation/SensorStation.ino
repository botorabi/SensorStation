/**
 * Copyright (c) 2019 by Botorabi. All rights reserved.
 * https://github.com/botorabi/SensorStation
 *
 * License: MIT License (MIT), read the LICENSE text in
 *          main directory for more details.
 */

#include <Arduino.h>
#include "core.h"
#include "buttons.h"


static const String APP_NAME("SensorStation");
static const String APP_VERSION("0.3.0");


class App: public Buttons::Handler
{
  public:

    static const int            HTTP_PORT = 80;
    static const int            DISPLAY_OFF_TIMEOUT = 60000;

    static const int            INPUT_PIN_WPS = 19;
    static const int            INPUT_PIN_DISPLAY = 18;
    static const unsigned long  WPS_BUTTON_PRESS_TIME = 5 * 1000;

    void initialize()
    {
      core.setSplashText(APP_NAME + "\n\nVersion: " + APP_VERSION + "\n");
      core.setAppVersion(APP_VERSION);
      core.initialize(HTTP_PORT, DISPLAY_OFF_TIMEOUT);
      buttons.setHandler(this, INPUT_PIN_WPS);
      buttons.setHandler(this, INPUT_PIN_DISPLAY);
    }

    void update()
    {
      core.update();
      buttons.update();
    }

    void handleButtons();

  protected:

    Core          core;

    Buttons       buttons;

    unsigned long wpsButtonPressedTimeStamp {0};

    void          onPressed(int pin);

    void          onReleased(int pin);
};

static App app;


void setup()
{
  Serial.begin(115200);
  Serial.printf("Booting %s v%s\n", APP_NAME.c_str(), APP_VERSION.c_str());

  app.initialize();
}

void loop()
{
  app.update();

  app.handleButtons();

  delay(500);
}

void App::onPressed(int pin)
{
  core.displayTurn(true);

  switch(pin)
  {
    case INPUT_PIN_WPS:
      if (!core.isWPSActive() && (wpsButtonPressedTimeStamp == 0))
      {
        wpsButtonPressedTimeStamp = millis();
        core.displaySetStatus("Hold WPS Button!");
      }
      break;

    case INPUT_PIN_DISPLAY:
      core.displayRestore();
      break;
  }      
}

void App::onReleased(int pin)
{
  if (pin == INPUT_PIN_WPS)
  {
    if ((wpsButtonPressedTimeStamp > 0) && (millis() - wpsButtonPressedTimeStamp < WPS_BUTTON_PRESS_TIME))
      core.displayRestore();

    wpsButtonPressedTimeStamp = 0;
  }
}

void App::handleButtons()
{
  if (wpsButtonPressedTimeStamp > 0)
  {
    if (millis() - wpsButtonPressedTimeStamp > WPS_BUTTON_PRESS_TIME)
    {
      wpsButtonPressedTimeStamp = 0;
      core.startWPS();
    }
  }  
}
