/**
 * Copyright (c) 2019 by Botorabi. All rights reserved.
 * https://github.com/botorabi/SensorStation
 *
 * License: MIT License (MIT), read the LICENSE text in
 *          main directory for more details.
 */

#include <Arduino.h>
#include "httpserver.h"
#include "resources/html_index.h"
#include "resources/html_index_head.h"
#include "resources/html_favicon.h"


HTTPServer::HTTPServer()
{
  for (int i = 0; i < index_html_len; i++)
    indexFileContent += static_cast<char>(index_html[i]);
}

HTTPServer::~HTTPServer()
{
  if (webServer)
    delete webServer;

  removeAllSensors();
}

void HTTPServer::configureSensors(Sensor** sensors, int countSensors)
{
  removeAllSensors();

  this->sensors = new Sensor*[countSensors];
  
  for (int i = 0; i < countSensors; i++)
    this->sensors[i] = sensors[i];

  this->countSensors = countSensors;
}

void HTTPServer::removeAllSensors()
{
  if (sensors)
    delete[] sensors;

  sensors = nullptr;  
}

void HTTPServer::setVersionInfo(const String& versionInfo)
{
  this->versionInfo = versionInfo;
}

void HTTPServer::start(int port)
{
  stop();

  webServer = new AsyncWebServer(port);
  webServer->on("/", HTTP_GET, [this](AsyncWebServerRequest* request) { handleRoot(request); });
  webServer->on("/sensors.json", HTTP_GET, [this](AsyncWebServerRequest* request) { handleSensorJSON(request); });
  webServer->on("/favicon.png", HTTP_GET, [this](AsyncWebServerRequest* request) { handleFavIcon(request); });
  webServer->onNotFound(handleNotFound);
  webServer->begin();
}

bool HTTPServer::isRunning() const
{
  return webServer != nullptr;
}

void HTTPServer::stop()
{
  if (webServer)
  {
    webServer->end();
    delete webServer;
  }

  webServer = nullptr;
}

String HTTPServer::extractBaseRefFromXHostHeader(AsyncWebServerRequest* request) const
{
  AsyncWebHeader* xForwardPrefix = request->getHeader("X-Forwarded-Prefix");
  if (!xForwardPrefix)
  {
    xForwardPrefix = request->getHeader("x-forwarded-prefix");
  }

  String href("/");
  if (xForwardPrefix)
  {
    href = xForwardPrefix->value();
    //! NOTE Some reverse proxies put the forward-prefix twice into the header (Spring Gateway)
    if ((href.length() > 1) && (href.indexOf(',') > 0))
    {
      href = href.substring(0, href.indexOf(','));
      href += '/';
    }
  }

  return href;
}

void HTTPServer::handleNotFound(AsyncWebServerRequest* request)
{
  request->send(404, "text/plain", "File Not Found!");
}

void HTTPServer::handleRoot(AsyncWebServerRequest* request)
{
  String html = html_index_head;
  html.replace("@HREF@", extractBaseRefFromXHostHeader(request));
  html += indexFileContent;  
  request->send(200, "text/html", html);
}

void HTTPServer::handleSensorJSON(AsyncWebServerRequest* request)
{
  String json;
  json = "{\n";
  json += "  \"version\": \"" + versionInfo + "\",\n";
  json += "  \"sensors\": [\n";
  for (int i = 0; i < countSensors; i++)
  {
    if (i > 0)
      json += ",\n";

    json += String("    {\"name\" : \"" + sensors[i]->name() + "\", \"value\" : " + sensors[i]->currentValue() +
            ", \"history\" : [" + sensors[i]->historyAsString() + "]}");
  }
  json += "\n  ]\n}";

  AsyncWebServerResponse *response = request->beginResponse(200, "application/json", json);
  response->addHeader("Access-Control-Allow-Origin", "*");
  request->send(response);
}

void HTTPServer::handleFavIcon(AsyncWebServerRequest* request)
{
  
  request->send_P(200, "image/png", favicon_png, favicon_png_len);
}
