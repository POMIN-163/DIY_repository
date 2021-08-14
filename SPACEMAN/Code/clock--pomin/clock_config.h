/**
 * @file main.h
 * @author Pomin
 * @brief
 * @date 2021-08-11
 *
 * @copyright Copyright (c) 2021
 *
**/
#ifndef __CLOCK_CONFIG_H
#define __CLOCK_CONFIG_H

#include "ntp/ntp.hpp"
#include "font/font.h"
#include "img/img.h"

#include <SPI.h>
#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>

const char ssid[] = "CMCC-WEB";  // WIFI名称
const char pass[] = "88888888";  // WIFI密码
String city_code  = "101040100"; // 天气城市代码
const int local_port = 8000;     // UDP 端口号
uint16_t bg_color = 0xFFFF;      // 背景颜色
const uint8_t is_use_border = 1; // 是否使用上下边框 (CASIO 和 NASA)

/* 动画选择 */
#define spaceman 0
#define longmao  1
#define pingpang 2

#define img_select spaceman
/* 可选项 spaceman, longmao, pingpang */

#if 1
    #define DBG_OUT(x) Serial.println(x) // 使能串口输出与否
    #define DBG_LOG(x)              \
        Serial.print("\033[34;1m"); \
        Serial.print(x);            \
        Serial.print("\033[0m\r\n")
#else
    #define DBG_OUT(x)
    #define DBG_LOG(x)
#endif

#endif

