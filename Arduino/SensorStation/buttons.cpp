/**
 * Copyright (c) 2019 by Botorabi. All rights reserved.
 * https://github.com/botorabi/SensorStation
 *
 * License: MIT License (MIT), read the LICENSE text in
 *          main directory for more details.
 */

#include <Arduino.h>
#include "buttons.h"


Buttons::Buttons()
{
}

Buttons::~Buttons()
{
}

bool Buttons::setHandler(Buttons::Handler* handler, int pin)
{
  if (countButtons >= MAX_BUTTONS)
    return false;

  Button* button = getButton(pin);
  if (!button)
    button = new Button(handler, pin);

  buttons[countButtons++] = button;

  pinMode(pin, INPUT_PULLUP);

  return true;
}

void Buttons::update()
{
  for (int i = 0; i < countButtons; i++)
  {
    int state = digitalRead(buttons[i]->pin);
    if (state != buttons[i]->lastState)
    {
      if (state == LOW)
        buttons[i]->handler->onPressed(buttons[i]->pin);
      else
        buttons[i]->handler->onReleased(buttons[i]->pin);

      buttons[i]->lastState = state;
    }
  }
}

Buttons::Button* Buttons::getButton(int pin)
{
  for (int i = 0; i < countButtons; i++)
  {
    if (buttons[i]->pin == pin)
      return buttons[i];
  }

  return nullptr;
}
