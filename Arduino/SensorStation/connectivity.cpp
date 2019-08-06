/**
 * Copyright (c) 2019 by Botorabi. All rights reserved.
 * https://github.com/botorabi/SensorStation
 *
 * License: MIT License (MIT), read the LICENSE text in
 *          main directory for more details.
 */

#include <Arduino.h>
#include <Preferences.h>
#include "connectivity.h"

#define WIFI_CLIENT_NAME        "SensorStation"

#define PREFERENCE_NAMESPACE    "sensors-wifi"
#define MAX_CONNECTION_RETRIES  5

#define ESP_WPS_MODE            WPS_TYPE_PBC
#define ESP_MANUFACTURER        "espressif"
#define ESP_MODEL_NUMBER        "ESP32"
#define ESP_MODEL_NAME          "ESPRESSIF IOT"
#define ESP_DEVICE_NAME         WIFI_CLIENT_NAME

static esp_wps_config_t       wpsConfiguration;
static bool                   wpsStarted = false;


Connectivity::Connectivity()
{
}

Connectivity::~Connectivity()
{
}

bool Connectivity::isConnected() const
{
  return WiFi.status() == WL_CONNECTED;
}

void Connectivity::enable(bool en)
{
  WiFi.enableSTA(en);
}

bool Connectivity::connect()
{
  uint8_t status;
  String ssid, psk;
  
  restoreCredentials(ssid, psk);

  WiFi.mode(WIFI_MODE_STA);
  WiFi.onEvent(Connectivity::wifiSystemEvent);

  if (ssid.length() > 0)
  {
    Serial.printf("Using stored WiFi credentials, SSID: '%s' \n", ssid.c_str());
    int retryCount = 0;
    while (retryCount++ < MAX_CONNECTION_RETRIES)
    {
      Serial.printf("WiFi Connection Attempt: %d \n", retryCount);
      WiFi.begin(ssid.c_str(), psk.c_str());
      status = WiFi.waitForConnectResult();
      if (status == WL_CONNECTED)
      {
        break;
      }
      delay(1000);
    }
  }
  else
  {
    WiFi.begin();
    status = WiFi.waitForConnectResult();
  }

  return status == WL_CONNECTED;
}

void Connectivity::storeCredentials(const String& ssid, const String& psk)
{
  Preferences preferences;
  if (!preferences.begin(PREFERENCE_NAMESPACE, false))
  {
    Serial.println("*** Problem occurred while storing credentials!");
    return;
  }
  preferences.putString("ssid", ssid);
  preferences.putString("password", psk);
  preferences.end();
}

void Connectivity::restoreCredentials(String& ssid, String& psk)
{
  Preferences preferences;
  if (!preferences.begin(PREFERENCE_NAMESPACE, false))
  {
    Serial.println("*** Problem occurred while restoring credentials!");
    return;
  }
  ssid = preferences.getString( "ssid", "");
  psk = preferences.getString( "password", "");
  preferences.end();
}

void Connectivity::startWPSConnection()
{
  storeCredentials("", "");
  enable(true);

  WiFi.disconnect();
  WiFi.mode(WIFI_MODE_STA);
  WiFi.setAutoReconnect(true);
  WiFi.onEvent(Connectivity::wifiWPSEvent);

  Serial.println("Starting WPS");

  setupWPS();
  esp_wifi_wps_disable();

  delay(100);

  wpsStarted = true;
  esp_wifi_wps_enable(&wpsConfiguration);
  esp_wifi_wps_start(0);
}

IPAddress Connectivity::localIP() const
{
  return WiFi.localIP();
}

bool Connectivity::isWPSInProgress() const
{
  return wpsStarted;
}

void Connectivity::setupWPS()
{
  wpsConfiguration.crypto_funcs = &g_wifi_default_wps_crypto_funcs;
  wpsConfiguration.wps_type = ESP_WPS_MODE;
  strcpy(wpsConfiguration.factory_info.manufacturer, ESP_MANUFACTURER);
  strcpy(wpsConfiguration.factory_info.model_number, ESP_MODEL_NUMBER);
  strcpy(wpsConfiguration.factory_info.model_name, ESP_MODEL_NAME);
  strcpy(wpsConfiguration.factory_info.device_name, ESP_DEVICE_NAME);
}

void Connectivity::wifiWPSEvent(WiFiEvent_t event, system_event_info_t info)
{
  switch(event)
  {
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.printf("Storing credentials for SSID: %s\n", WiFi.SSID().c_str());
      storeCredentials(WiFi.SSID(), WiFi.psk());
      //! NOTE Actually we just need to remove the WPS event registry, but it is not
      //       possible while dispatching an event. So we go the easierst way and reboot.
      Serial.println("Restarting the system...");
      delay(1000);
      ESP.restart();
      break;
    case SYSTEM_EVENT_STA_STOP:
      Serial.println("WiFi client was stopped, attempting reconnection");
      delay(10000);
      WiFi.reconnect();
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("Disconnected from station, attempting reconnection");
      WiFi.reconnect();
      break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
      Serial.println("WPS Successfull, stopping WPS and connecting to: " + String(WiFi.SSID()));
      wpsStarted = false;
      esp_wifi_wps_disable();
      delay(10);
      WiFi.begin();
      break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
      Serial.println("WPS Failed, retrying");
      break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
      Serial.println("WPS Timedout, retrying");
      esp_wifi_wps_disable();
      esp_wifi_wps_enable(&wpsConfiguration);
      esp_wifi_wps_start(0);
      break;
    case SYSTEM_EVENT_STA_WPS_ER_PIN:
      Serial.println("WPS_PIN ???");
      break;
    default:
      break;
  }
}

void Connectivity::wifiSystemEvent(WiFiEvent_t event, system_event_info_t info)
{
  switch(event)
  {
    case SYSTEM_EVENT_STA_START:
      Serial.println("Station Mode Started");
      if (!WiFi.setHostname(WIFI_CLIENT_NAME)) {
        Serial.printf("*** could not set the host name to '%s'\n", WIFI_CLIENT_NAME);        
      }
      break;
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("Connected to: " + String(WiFi.SSID()));
      Serial.print("Got IP: ");
      Serial.println(WiFi.localIP());
      break;
    case SYSTEM_EVENT_STA_CONNECTED:
      Serial.println("WiFi client connected");
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi client  disconnected from station");
      break;
    case SYSTEM_EVENT_STA_LOST_IP:
      Serial.println("WiFi client lost IP, attempting reconnection...");
      delay(2000);
      WiFi.reconnect();
      break;      
    case SYSTEM_EVENT_STA_STOP:
      Serial.println("WiFi client was stopped");
      break;
    default:
      break;
  }
}
