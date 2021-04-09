// AWTRIX Controller
// Copyright (C) 2020
// by Blueforcer & Mazze2000
#include <FS.h>
// #include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266httpUpdate.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <Fonts/TomThumb.h>
#include <LightDependentResistor.h>
#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include "SoftwareSerial.h"
#include "RTClib.h"
#include "DHT.h"
#include "ApePixelClock.h"

#include <WiFiManager.h>
#include <DoubleResetDetect.h>
#include <Wire.h>
#include <BME280_t.h>
#include "Adafruit_HTU21DF.h"
#include "DFRobotDFPlayerMini.h"


void setup()
{
  pinMode(D2, OUTPUT);
}
void loop()
{
  NS()
}
