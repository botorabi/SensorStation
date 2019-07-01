/**
 * Copyright (c) 2019 by Botorabi. All rights reserved.
 * https://github.com/botorabi/SensorStation
 *
 * License: MIT License (MIT), read the LICENSE text in
 *          main directory for more details.
 */

#include "display.h"


Display::Display()
{
}

Display::~Display()
{
}

void Display::start()
{
  stop();
  display = new SSD1306(0x3c, 21, 22);
  display->init();
  display->resetDisplay();
  display->setBrightness(100);
  clear();
}

void Display::update()
{
  unsigned long now = millis();
  if ((offTimeStamp > 0) && (now > offTimeStamp))
  {
    turn(false);
    offTimeStamp = 0;
  }
}

void Display::stop()
{
  if (!display)
    return;

  display->end();
  delete display;
  display = nullptr;
}

void Display::clear()
{
  display->clear();
  display->setColor(WHITE);
  display->fillRect(1, 1, 126, 62);
}

void Display::turn(bool on)
{
  this->on = on;

  if (on)
  {
    display->displayOn();

    if (offTimeOut > 0)
      offTimeStamp = millis() + offTimeOut;
  }
  else
  {
    display->displayOff();
  }
}

void Display::setStatusText(const String& text)
{
  textStatus = text;
  render();

  if (offTimeOut > 0)
    offTimeStamp = millis() + offTimeOut;
}

void Display::setMainText(const String& text)
{
  textStatus = "";
  textMain = text;
  render();
}

void Display::setFooterText(const String& text)
{
  textFooter = text;
  render();
}

void Display::render()
{
  clear();
  display->setColor(BLACK);
  if (textStatus.length() > 0)
  {
    display->setFont(ArialMT_Plain_16);
    display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    display->drawStringMaxWidth(64, 16, 100, textStatus);
  }
  else if (textMain.length() > 0)
  {
    display->setFont(ArialMT_Plain_10);
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    int pos = 0, beg = 0, line = 0, strLength = textMain.length();
    while ((beg < strLength) && ((pos = textMain.indexOf("\n", beg)) > 0))
    {
      display->drawString(64, 5 + line * 12, textMain.substring(beg, pos));
      beg = pos + 1;
      line++;
    }
  }

  if (textFooter.length() > 0)
  {
    display->setFont(ArialMT_Plain_10);
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->drawString(64, 48, textFooter);
  }
  display->display();
}
