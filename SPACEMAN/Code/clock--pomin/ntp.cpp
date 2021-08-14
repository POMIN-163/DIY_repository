/**
 * @file ntp.cpp
 * @author Pomin
 * @brief NTP 授时
 * @date 2021-08-11
 *
 * @copyright Copyright (c) 2021
 *
**/
#include "ntp/ntp.hpp"

/*-------- NTP code ----------*/

// NTP服务器
static const char ntpServerName[] = "ntp6.aliyun.com";
const int timeZone = 8;  // 东八区
WiFiUDP myudp;

const int NTP_PACKET_SIZE = 48;      // NTP时间在消息的前48字节中
byte packetBuffer[NTP_PACKET_SIZE];  // buffer to hold incoming & outgoing
/**
 * @brief 获取 NTP 授时
 *
 * @return time_t
**/
time_t getNtpTime(void) {
    IPAddress ntpServerIP;  // NTP server's ip address

    while (myudp.parsePacket() > 0);
    // discard any previously received packets
    // Serial.println("Transmit NTP Request");
    // get a random server from the pool
    WiFi.hostByName(ntpServerName, ntpServerIP);
    // Serial.print(ntpServerName);
    // Serial.print(": ");
    // Serial.println(ntpServerIP);
    sendNTPpacket(ntpServerIP);
    uint32_t beginWait = millis();
    while (millis() - beginWait < 1500) {
        int size = myudp.parsePacket();
        if (size >= NTP_PACKET_SIZE) {
            Serial.println("Receive NTP Response");
            myudp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
            unsigned long secsSince1900;
            // convert four bytes starting at location 40 to a long integer
            secsSince1900  = (unsigned long)packetBuffer[40] << 24;
            secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
            secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
            secsSince1900 |= (unsigned long)packetBuffer[43];
            // Serial.println(secsSince1900 - 2208988800UL + timeZone *
            // SECS_PER_HOUR);
            return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
        }
    }
    Serial.println("No NTP Response :-(");
    return 0;  // 无法获取时间时返回0
}
/**
 * @brief 向 NTP 服务器发送请求
 *
 * @param address
**/
void sendNTPpacket(IPAddress &address) {
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011;  // LI, Version, Mode
    packetBuffer[1] = 0;           // Stratum, or type of clock
    packetBuffer[2] = 6;           // Polling Interval
    packetBuffer[3] = 0xEC;        // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;
    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    myudp.beginPacket(address, 123);  // NTP requests are to port 123
    myudp.write(packetBuffer, NTP_PACKET_SIZE);
    myudp.endPacket();
}
