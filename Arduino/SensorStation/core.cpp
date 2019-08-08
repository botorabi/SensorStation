/**
 * Copyright (c) 2019 by Botorabi. All rights reserved.
 * https://github.com/botorabi/SensorStation
 *
 * License: MIT License (MIT), read the LICENSE text in
 *          main directory for more details.
 */

#include <Arduino.h>
#include "settings.h"
#include "core.h"


static const unsigned long SENSOR_HISTORY_INTERVAL = 1000 * 60 * 60;

static const int ANALOG_PIN_0 = 34;
static const int ANALOG_PIN_1 = 35;
static const int ANALOG_PIN_2 = 32;


Core::Core()
{
}

Core::~Core()
{
  for (int i = 0; i < countSensors; i++)
  {
    delete sensors[i];
  }
  delete[] sensors;
}

void Core::setSplashText(const String& text)
{
  splashText = text;
}

void Core::setAppVersion(const String& version)
{
  appVersion = version;
}

void Core::initialize(int httpPort, int displayOffTimeout)
{
  this->httpPort = httpPort;
  this->displayOffTimeout = displayOffTimeout;

  otaUpdate = Settings::loadOTAFlag();
  if (otaUpdate)
    Settings::saveOTAFlag(false);

  setupDisplay();
  delay(1000);
  setupSensors();
  delay(1000);
  setupWiFi();
  delay(1000);
}

void Core::update()
{
  updateSensors();
  updateHTTPServer();
  display.update();
}

void Core::enableOTAUpdate()
{
  //! NOTE: we cannot restart the web server as it causes a crash, so we really have to reboot :-/
  Settings::saveOTAFlag(true);
  Serial.println("Reboot for enabling OTA update");
  ESP.restart();
}

void Core::startWPS()
{
  displaySetStatus("Starting WPS...", false);
  displaySetFooter("");
  connectivity.startWPSConnection();
}

bool Core::isWPSActive() const
{
  return connectivity.isWPSInProgress();
}

void Core::displayTurn(bool on)
{
  display.turn(on);
}

void Core::displaySetStatus(const String& text, bool toast)
{
  display.setStatusText(text);
  sensorDisplayUpdate = toast;
}

void Core::displaySetFooter(const String& text)
{
  display.setFooterText(text);  
}

void Core::setupDisplay()
{
  display.setAutoOff(displayOffTimeout);
  display.start();
  display.setMainText(splashText);
  delay(2000);
  display.setMainText("");
  display.setStatusText("Starting...");
}

void Core::displayRestore()
{
  sensorDisplayUpdate = true;
  updateSensorDisplay();
}

void Core::setupWiFi()
{
  display.setStatusText("Establishing WiFi...");
  Serial.println("Establishing WiFi connection...");
  if (connectivity.connect() == false)
  {
    display.setStatusText("No Networking!");
    display.setFooterText("**No WiFi Connection**");
    Serial.println("WiFi connection failed, try WPS");
    connectivity.enable(false);
  }
  else
  {
    Serial.print("WiFi connection successful, local IP: ");
    Serial.println(connectivity.localIP());
  }
}

void Core::setupSensors()
{
  display.setStatusText("Initializing Sensors...");

  const int COUNT_SENSORS = 3;

  sensors = new Sensor*[COUNT_SENSORS];
  countSensors = COUNT_SENSORS;

  sensors[0] = createMoistureSensor("Moisture 1", ANALOG_PIN_0);
  sensors[1] = createMoistureSensor("Moisture 2", ANALOG_PIN_1);
  sensors[2] = createMoistureSensor("Moisture 3", ANALOG_PIN_2);

  httpServer.configureSensors(sensors, countSensors);
}

Sensor* Core::createMoistureSensor(const String& name, int pin) const
{
  Sensor* sensor = new Sensor();
  sensor->setup(name, pin);
  return sensor;
}

void Core::updateHTTPServer()
{
  if (connectivity.isConnected())
  {
    if (!httpServer.isRunning())
    {
      display.setStatusText("Networking Established!");
      display.setFooterText(String("IP: ") + connectivity.localIP().toString());

      Serial.printf("Starting HTTP Server, OTA update %s \n", (otaUpdate ? "enabled" : "disabled"));
      httpServer.setVersionInfo(appVersion);
      httpServer.start(httpPort, otaUpdate);

      if (otaUpdate)
      {
        displaySetStatus("OTA Update Mode", false);
      }

      delay(2000);
      updateSensorDisplay();
    }
    else
    {
      httpServer.update();
    }
  }
}

void Core::updateSensors()
{
  unsigned long currentTime = millis();
  if (lastSensorValueUpdate == 0)
  {
    addSensorEntry();
    lastSensorValueUpdate = currentTime;
    lastSensorDisplayUpdate = currentTime;
  }

  unsigned int diffTime = currentTime - lastSensorValueUpdate;
  if (diffTime > Sensor::SENSOR_HISTORY_INTERVAL)
  {
    addSensorEntry();
    updateSensorDisplay();
    lastSensorValueUpdate += Sensor::SENSOR_HISTORY_INTERVAL;
  }

  if (currentTime - lastSensorDisplayUpdate > sensorDisplayInterval)
  {
    lastSensorDisplayUpdate += sensorDisplayInterval;
    if (display.isOn())
      updateSensorDisplay();
  }
}

void Core::addSensorEntry()
{
  for (int i = 0; i < countSensors; i++)
  {
    sensors[i]->addHistoryEntry();
  }
}

void Core::updateSensorDisplay()
{
  if (!sensorDisplayUpdate || (countSensors < 1))
    return;

  String displayText;
  for (int i = 0; i < countSensors; i++)
  {
    const int MAX_VALUE = 4096;
    int value = sensors[i]->currentValue();
    value = ((MAX_VALUE - value) * 100) / MAX_VALUE;
    displayText += "  " + sensors[i]->name() + ": " + value + "%\n";
  }
  display.setMainText(displayText);
}
