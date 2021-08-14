/**
 * @file ntp.hpp
 * @author Pomin
 * @brief NTP授时
 * @date 2021-08-11
 *
 * @copyright Copyright (c) 2021
 *
**/
#ifndef __NTP_H
#define __NTP_H

#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

extern WiFiUDP myudp;

time_t getNtpTime(void);
void sendNTPpacket(IPAddress &address);


#endif