/**
 * @file CLOCK.ino
 * @author Pomin
 * @brief 太空人小屏幕代码
 * @date 2021-08-11
 *
 * @copyright Copyright (c) 2021
 *
**/
#include "clock_config.h"

WiFiClient wifi_client;      // web 访问操作对象
TFT_eSPI mytft = TFT_eSPI(); // tft屏幕对象
TFT_eSprite mytft_spirit_main  = TFT_eSprite(&mytft);  // tft "精灵" 缓存区主对象
TFT_eSprite mytft_spirit_scroll = TFT_eSprite(&mytft); // tft "精灵" 缓存区滚动显示对象
/**
 * @brief 启动
 *
**/
byte loading_num = 6;
void setup() {
    /* 串口初始化 */
    Serial.begin(76800);
    DBG_LOG(LOGO);
    /* 屏幕初始化 */
    mytft.init();
    /* 旋转屏幕 */
    mytft.setRotation(2);
    /* 填充屏幕 */
    mytft.fillScreen(0x0000);
    /* 设置字符颜色 */
    mytft.setTextColor(TFT_BLACK, bg_color);
    /* 连接 WiFi  */
    WiFi.begin(ssid, pass);
    DBG_LOG("正在连接 WIFI ");
    DBG_LOG(ssid);
    /* 每 500 毫秒检测一次状态 */
    while (WiFi.status() != WL_CONNECTED) {
        /* 加载动画 */
        for (byte n = 0; n < 10; n++) {
            display_loading(50);
        }
    }
    /* 连接完毕, 让动画走完 */
    while (loading_num < 194) {
        display_loading(1);
    }
    DBG_LOG("本地 IP： ");
    DBG_LOG(WiFi.localIP());

    DBG_LOG("启动 UDP");
    myudp.begin(local_port);
    DBG_LOG("端口号: ");
    DBG_LOG(local_port);
    DBG_LOG("等待同步...");
    /* 设置同步时钟为 NTP 授时 */
    setSyncProvider(getNtpTime);
    /* 同步周期 */
    setSyncInterval(300);

    /* 设置图像放大倍数 */
    TJpgDec.setJpgScale(1);
    /* 图像输出回调 */
    TJpgDec.setSwapBytes(true);
    TJpgDec.setCallback(tft_output);

    /* 绘制顶部和底部 */
    if (is_use_border) {
        TJpgDec.drawJpg(0, 0, watchtop, sizeof(watchtop));
        TJpgDec.drawJpg(0, 220, watchbottom, sizeof(watchbottom));
    }
    /* 是否显示上下边框 */
    if (is_use_border) {
        /* 绘制一个窗口 */
        mytft.setViewport(0, 20, 240, 200);
        mytft.fillScreen(0x0000);
        /* 实心圆角矩形 */
        mytft.fillRoundRect(0, 0, 240, 200, 5, bg_color);
    } else {
        mytft.setViewport(0, 0, 240, 230);
        mytft.fillScreen(0x0000);
        mytft.fillRoundRect(0, 0, 240, 230, 5, bg_color);
        mytft.drawFastHLine(0, 200,  240, TFT_BLACK);
    }
    /* 绘制线框 */
    mytft.drawFastHLine(0,   34,   240, TFT_BLACK);
    mytft.drawFastVLine(150, 0,    34,  TFT_BLACK);
    mytft.drawFastHLine(0,   166,  240, TFT_BLACK);
    mytft.drawFastVLine(60,  166,  34,  TFT_BLACK);
    mytft.drawFastVLine(160, 166,  34,  TFT_BLACK);

    /* 温度图标 */
    TJpgDec.drawJpg(161, 171, temperature, sizeof(temperature));
    /* 湿度图标 */
    TJpgDec.drawJpg(159, 130, humidity, sizeof(humidity));

    /* 获取城市代码 */
    web_get_city_code();
}
/* 伪多线程对象参数 */
typedef struct my_task {
    void(*task)(void);
    unsigned int cycle_length;
	unsigned long int last_run;
	unsigned long int is_running;
} task_s;
/* 伪多线程对象列表 */
static task_s systemTasks[] = {
    { display_time,           1000,   0, 1 },
    { display_animation,      80,     0, 1 },
    { display_scroll_banner,  3000,   0, 1 },
    { web_get_city_weater,    300000, 0, 1 },
    { display_scroll_txt,     40,     0, 0 },
};
/* 滚动效果使能位 */
#define scroll_en systemTasks[4].is_running
volatile unsigned long int systime_ms;
#define task_num 5
/**
 * @brief 主循环
 *
**/
void loop() {
    unsigned char index = 0;
    systime_ms = millis();
    for(index = 0; index < task_num; index++) {
        /* 允许执行条件   1. 时刻到达   2. 首次运行 */
        if(systemTasks[index].is_running == 1) {
            if(systime_ms - systemTasks[index].last_run > systemTasks[index].cycle_length || systemTasks[index].last_run == 0) {
                systemTasks[index].last_run = systime_ms;
                systemTasks[index].task();
            }
        } else {
            /* 避免启动线程立即执行 */
            systemTasks[index].last_run = systime_ms;
        }
    }
}
/**
 * @brief 屏幕显示图像 (回调)
 *
**/
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap) {
    if (y >= mytft.height()) return 0;
    mytft.pushImage(x, y, w, h, bitmap);
    // Return 1 to decode next block
    return 1;
}
/**
 * @brief 加载页面
 *
**/
void display_loading(byte delayTime) {
    mytft_spirit_main.setColorDepth(8);

    mytft_spirit_main.createSprite(200, 50);
    mytft_spirit_main.fillSprite(0x0000);

    mytft_spirit_main.drawRoundRect(0, 0, 200, 16, 8, 0xFFFF);
    mytft_spirit_main.fillRoundRect(3, 3, loading_num, 10, 5, 0xFFFF);
    mytft_spirit_main.setTextDatum(CC_DATUM);
    mytft_spirit_main.setTextColor(TFT_GREEN, 0x0000);
    mytft_spirit_main.drawString("Connecting to WiFi", 100, 40, 2);
    /* 缓存区推送到屏幕 */
    mytft_spirit_main.pushSprite(20, 110);
    /* 清空缓存区 */
    mytft_spirit_main.deleteSprite();
    loading_num++;
    if (loading_num >= 194) {
        loading_num = 194;
    }
    delay(delayTime);
}
/**
 * @brief 太空人旋转, 龙猫, 乒乓球等动画
 *
**/
void display_animation(void) {
    const uint16_t x = 80, y = 94, dt = 80;
    static uint16_t index = 0;
    index = (index > myimgs[img_select].arr_size - 2) ? 0 : index + 1;
    TJpgDec.drawJpg(x, y, myimgs[img_select].arr[index], myimgs[img_select].size[index]);
}
/**
 * @brief 发送 HTTP 请求获取城市代码
 *
**/
void web_get_city_code(void) {
    String url = "http://wgeo.weather.com.cn/ip/?_="+String(now());
    /* 创建 HTTPClient 对象 */
    HTTPClient http_client;

    /* 配置请求地址。此处也可以不使用端口号和 PATH 而单纯的 */
    http_client.begin(wifi_client, url);

    /* 设置请求头中的 User-Agent */
    http_client.setUserAgent("Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1");
    http_client.addHeader("Referer", "http://www.weather.com.cn/");

    /* 启动连接并发送 HTTP 请求 */
    int http_code = http_client.GET();
    DBG_LOG("Send GET request to url: ");
    DBG_LOG(url);

    /* 如果服务器响应 OK 则从服务器获取响应体信息并通过串口输出 */
    if (http_code == HTTP_CODE_OK) {
        String str = http_client.getString();

        int id = str.indexOf("id=");
        if(id != -1) {
            // city_code = str.substring(id+4,id+4+9).toInt();
            city_code = str.substring(id + 4,id + 4 + 9);
            DBG_LOG(city_code);
            web_get_city_weater();
        }else{
            DBG_LOG("获取城市代码失败");
        }
    } else {
        DBG_LOG("请求城市代码错误：");
        DBG_LOG(http_code);
    }
    /* 关闭ESP8266与服务器连接 */
    http_client.end();
}
/**
 * @brief 获取城市天气
 *
**/
void web_get_city_weater(void) {
    String url = "http://d1.weather.com.cn/weather_index/" + city_code + ".html?_=" + String(now());
    /* 创建 HTTPClient 对象 */
    HTTPClient http_client;

    http_client.begin(wifi_client, url);

    /* 设置请求头中的 User-Agent */
    http_client.setUserAgent("Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1");
    http_client.addHeader("Referer", "http://www.weather.com.cn/");

    /* 启动连接并发送 HTTP 请求 */
    int http_code = http_client.GET();
    DBG_LOG("正在获取天气数据");
    DBG_LOG(url);

    /* 如果服务器响应 OK 则从服务器获取响应体信息并通过串口输出 */
    if (http_code == HTTP_CODE_OK) {
        String str = http_client.getString();
        int indexStart = str.indexOf("weatherinfo\":");
        int indexEnd = str.indexOf("};var alarmDZ");

        String jsonCityDZ = str.substring(indexStart + 13, indexEnd);
        DBG_OUT(jsonCityDZ);

        indexStart = str.indexOf("dataSK =");
        indexEnd = str.indexOf(";var dataZS");
        String jsonDataSK = str.substring(indexStart + 8, indexEnd);
        DBG_OUT(jsonDataSK);

        indexStart = str.indexOf("\"f\":[");
        indexEnd = str.indexOf(",{\"fa");
        String jsonFC = str.substring(indexStart + 5, indexEnd);
        DBG_OUT(jsonFC);

        weaterData(&jsonCityDZ, &jsonDataSK, &jsonFC);
        DBG_LOG("获取成功");
    } else {
        DBG_LOG("请求城市天气错误：");
        DBG_LOG(http_code);
    }
    /* 关闭ESP8266与服务器连接 */
    http_client.end();
}
/**
 * @brief 天气信息写到屏幕上
 *
**/
String scroll_txt[6];
void weaterData(String *cityDZ, String *dataSK, String *dataFC) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, *dataSK);
    JsonObject sk = doc.as<JsonObject>();

    /* 绘制相关文字 */
    mytft_spirit_main.setColorDepth(8);
    mytft_spirit_main.loadFont(ZdyLwFont_20);

    /* 温度 */
    mytft_spirit_main.createSprite(54, 32);
    mytft_spirit_main.fillSprite(bg_color);
    mytft_spirit_main.setTextDatum(CC_DATUM);
    mytft_spirit_main.setTextColor(TFT_BLACK, bg_color);
    mytft_spirit_main.drawString(sk["temp"].as<String>() + "℃", 27, 16);
    mytft_spirit_main.pushSprite(185, 168);
    mytft_spirit_main.deleteSprite();

    /* 城市名称 */
    mytft_spirit_main.createSprite(88, 32);
    mytft_spirit_main.fillSprite(bg_color);
    mytft_spirit_main.setTextDatum(CC_DATUM);
    mytft_spirit_main.setTextColor(TFT_BLACK, bg_color);
    mytft_spirit_main.drawString(sk["cityname"].as<String>(), 44, 16);
    mytft_spirit_main.pushSprite(151, 1);
    mytft_spirit_main.deleteSprite();

    /* PM2.5 空气指数 */
    uint16_t pm25BgColor = mytft.color565(156, 202, 127);  // 优
    String aqiTxt = "优";
    int pm25V = sk["aqi"];
    if (pm25V > 200) {
        pm25BgColor = mytft.color565(136, 11, 32);  // 重度
        aqiTxt = "重度";
    } else if (pm25V > 150) {
        pm25BgColor = mytft.color565(186, 55, 121);  // 中度
        aqiTxt = "中度";
    } else if (pm25V > 100) {
        pm25BgColor = mytft.color565(242, 159, 57);  // 轻
        aqiTxt = "轻度";
    } else if (pm25V > 50) {
        pm25BgColor = mytft.color565(247, 219, 100);  // 良
        aqiTxt = "良";
    }
    mytft_spirit_main.createSprite(50, 24);
    mytft_spirit_main.fillSprite(bg_color);
    mytft_spirit_main.fillRoundRect(0, 0, 50, 24, 4, pm25BgColor);
    mytft_spirit_main.setTextDatum(CC_DATUM);
    mytft_spirit_main.setTextColor(0xFFFF);
    mytft_spirit_main.drawString(aqiTxt, 25, 13);
    mytft_spirit_main.pushSprite(5, 130);
    mytft_spirit_main.deleteSprite();

    /* 湿度 */
    mytft_spirit_main.createSprite(56, 24);
    mytft_spirit_main.fillSprite(bg_color);
    mytft_spirit_main.setTextDatum(CC_DATUM);
    mytft_spirit_main.setTextColor(TFT_BLACK, bg_color);
    mytft_spirit_main.drawString(sk["SD"].as<String>(), 28, 13);
    // mytft_spirit_main.drawString("100%",28,13);
    mytft_spirit_main.pushSprite(180, 130);
    mytft_spirit_main.deleteSprite();

    scroll_txt[0] = "实时天气 " + sk["weather"].as<String>();
    scroll_txt[1] = "空气质量 " + aqiTxt;
    scroll_txt[2] = "风向 " + sk["WD"].as<String>() + sk["WS"].as<String>();

    /* 左上角滚动字幕 */
    // 解析第二段JSON
    deserializeJson(doc, *cityDZ);
    JsonObject dz = doc.as<JsonObject>();
    // DBG_OUT(sk["ws"].as<String>());
    // 横向滚动方式
    // String aa = "今日天气:" + dz["weather"].as<String>() + "，温度:最低" +
    // dz["tempn"].as<String>() + "，最高" + dz["temp"].as<String>() + "
    // 空气质量:" + aqiTxt + "，风向:" + dz["wd"].as<String>() +
    // dz["ws"].as<String>(); scrollTextWidth = mytft_spirit_main.textWidth(scroll_txt);
    // DBG_OUT(aa);
    scroll_txt[3] = "今日" + dz["weather"].as<String>();

    deserializeJson(doc, *dataFC);
    JsonObject fc = doc.as<JsonObject>();

    scroll_txt[4] = "最低温度" + fc["fd"].as<String>() + "℃";
    scroll_txt[5] = "最高温度" + fc["fc"].as<String>() + "℃";

    // DBG_OUT(scroll_txt[0]);

    mytft_spirit_main.unloadFont();
}
/**
 * @brief 滚动显示天气状况
 *
**/
int current_index = 0;
void display_scroll_banner(void) {
    if (scroll_txt[current_index]) {
        mytft_spirit_scroll.setColorDepth(8);
        mytft_spirit_scroll.loadFont(ZdyLwFont_20);

        scroll_en = 1; // 开始滚动

        current_index = (current_index > 4) ? 0 : current_index + 1;
        // DBG_OUT(current_index);
    }
}
/**
 * @brief 滚动显示天气状况 (线程)
 *
**/
void display_scroll_txt(void) {
    static int pos = 24;
    if (pos > 0) {
        mytft_spirit_scroll.createSprite(148, 24);
        mytft_spirit_scroll.fillSprite(bg_color);
        mytft_spirit_scroll.setTextWrap(false);
        mytft_spirit_scroll.setTextDatum(CC_DATUM);
        mytft_spirit_scroll.setTextColor(TFT_BLACK, bg_color);
        mytft_spirit_scroll.drawString(scroll_txt[current_index], 74, pos + 12);
        mytft_spirit_scroll.pushSprite(2, 4);
        pos--;
    } else {
        pos = 24;
        mytft_spirit_scroll.deleteSprite();
        mytft_spirit_scroll.unloadFont();
        scroll_en = 0; // 关闭滚动
    }
}
/**
 * @brief 显示时间
 *
**/
void display_time(void) {
    mytft_spirit_main.setColorDepth(8);

    /*** 中间时间区 ***/
    /* 时分 */
    mytft_spirit_main.createSprite(140, 48);
    mytft_spirit_main.fillSprite(bg_color);
    // mytft_spirit_main.loadFont(FxLED_48);
    mytft_spirit_main.setTextDatum(CC_DATUM);
    mytft_spirit_main.setTextColor(TFT_BLACK, bg_color);
    mytft_spirit_main.drawString(hourMinute(), 70, 24, 7);  // 绘制时和分
    // mytft_spirit_main.unloadFont();
    mytft_spirit_main.pushSprite(28, 40);
    mytft_spirit_main.deleteSprite();

    /* 秒 */
    mytft_spirit_main.createSprite(40, 32);
    mytft_spirit_main.fillSprite(bg_color);

    mytft_spirit_main.loadFont(FxLED_32);
    mytft_spirit_main.setTextDatum(CC_DATUM);
    mytft_spirit_main.setTextColor(TFT_BLACK, bg_color);
    mytft_spirit_main.drawString(num2str(second()), 20, 16);

    mytft_spirit_main.unloadFont();
    mytft_spirit_main.pushSprite(170, 60);
    mytft_spirit_main.deleteSprite();
    /***中间时间区***/

    /***底部***/
    mytft_spirit_main.loadFont(ZdyLwFont_20);
    mytft_spirit_main.createSprite(58, 32);
    mytft_spirit_main.fillSprite(bg_color);

    /* 星期 */
    mytft_spirit_main.setTextDatum(CC_DATUM);
    mytft_spirit_main.setTextColor(TFT_BLACK, bg_color);
    mytft_spirit_main.drawString(week(), 29, 16);
    mytft_spirit_main.pushSprite(1, 168);
    mytft_spirit_main.deleteSprite();

    /* 月日 */
    mytft_spirit_main.createSprite(98, 32);
    mytft_spirit_main.fillSprite(bg_color);
    mytft_spirit_main.setTextDatum(CC_DATUM);
    mytft_spirit_main.setTextColor(TFT_BLACK, bg_color);
    mytft_spirit_main.drawString(monthDay(), 49, 16);
    mytft_spirit_main.pushSprite(61, 168);
    mytft_spirit_main.deleteSprite();
    /***底部***/
    /* 底部附加 (is_use_border 参数控制) */
    if (!is_use_border) {
        mytft_spirit_main.createSprite(230, 32); //! TODO 是否显示上下边框
        mytft_spirit_main.fillSprite(bg_color);
        mytft_spirit_main.setTextDatum(CC_DATUM);
        mytft_spirit_main.setTextColor(TFT_BLACK, bg_color);
        mytft_spirit_main.drawString("Modified by Pomin",110,16);
        mytft_spirit_main.pushSprite(10,202);
        mytft_spirit_main.deleteSprite();
    }
    /* 卸载字体 */
    mytft_spirit_main.unloadFont();
}
/* 星期 */
String week(void) {
    String wk[7] = {"日", "一", "二", "三", "四", "五", "六"};
    String s = "周" + wk[weekday() - 1];
    return s;
}
/* 月日 */
String monthDay(void) {
    String s = String(month());
    s = s + "月" + day() + "日";
    return s;
}
/* 时分 */
String hourMinute(void) {
    String s = num2str(hour());
    s = s + ":" + num2str(minute());
    return s;
}
/* 数字转字符串 */
String num2str(int digits) {
    String s = "";
    if (digits < 10) s = s + "0";
    s = s + digits;
    return s;
}
