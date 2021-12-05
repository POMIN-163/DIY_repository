#include "WiFiManager.h"

WiFiManager wifima;

void handleRoot(); //主页回调函数
void Handledata();


DNSServer dnsServer;//创建dnsServer实例
ESP8266WebServer server(80);//创建WebServer
const byte DNS_PORT = 53;//DNS端口号

String txet_button_buf = "";
String id_function_buf = "";
String id_function_buf1 = "";
String text_tips_buf = "";

bool data_get_flg = false;

const char*  html_url_head0 = "\r\n\
  <!DOCTYPE html>\r\n\
  <html>\r\n\
  <head>\r\n\
  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\r\n\
  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n\
  <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">\r\n\
  <title>ESP8266网页配网</title>\r\n\
  <script type=\"text/javascript\">\r\n\
  function wifi(){\r\n\
  ";
const char*  html_url_head1 = "var xmlhttp=new XMLHttpRequest();xmlhttp.open(\"GET\",\"/Handledata?";
const char*  html_url_head2 = ",true);\r\n\
  xmlhttp.send();\r\n\
  xmlhttp.onload = function(e){\r\n\
  alert(this.responseText);}\r\n\
  }\r\n\
  </script></head>\r\n\
 ";
const char*  html_url_end = "</form></html>";

const char*  html_url_tips = "<br><br><body>点击保存后请耐心等待，如配置后物联网连接失败请重新配置更正密匙";




/* Tips为输入框前面的内容 name为输入框提示的内容 id为代号  代号对应到function 取结果时名字为function*/
void creat_input_object(String Tips,String name,String id,String function)
{
  id_function_buf += "var " + function + "=" + id + ".value;";
  if(id_function_buf1 != "")
    id_function_buf1 += "+\"&";
  id_function_buf1 += function + "=\"+" + function;
  if(txet_button_buf != "")
    txet_button_buf += "<br>";
  txet_button_buf += Tips + "<input type=\"text\"lenth=32 placeholder=\"" + name + "\"id=\"" + id + "\">";
}

//创建按钮
void creat_button_object(String name)
{
  txet_button_buf += "<br><br>";
  txet_button_buf += "<input type=\"button\"value=\"" + name + "\"onclick=\"wifi()\">";
}

//创建顶部信息提示内容
void creat_text_tips(String ch)
{
  text_tips_buf += "<body>";
  text_tips_buf += "<div style= 'text-align:center;'>";
  text_tips_buf += ch;
}

//创建好控件后经由此函数打包生成最终URL
String pack_url()
{
  String url = FPSTR(html_url_head0);
  url += id_function_buf;
  url += FPSTR(html_url_head1);
  url += id_function_buf1;
  url += FPSTR(html_url_head2);
  url += text_tips_buf;
  url += "<form>";
  url += txet_button_buf;
  url += FPSTR(html_url_tips);
  url += FPSTR(html_url_end);
  id_function_buf = "";
  id_function_buf1 = "";
  txet_button_buf = "";
  text_tips_buf = "";
  return url;
}


bool WiFiManager::startConfigPortal(char const *apName) {
  
  // WiFi.setAutoReconnect(false);
  // if(!WiFi.isConnected())
  // {
  //     WiFi.disconnect(); //  this alone is not enough to stop the autoconnecter
  // }
  // WiFi.persistent(true);
  WiFi.mode(WIFI_AP_STA);

  
  data_get_flg = false;
  wifima_flg = 1;
  back_flg = 0;

  
  WiFi.softAP(apName);//生成wifi的名称
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());//初始化DNS服务器dnsServer.start(DNS_PORT, "*", apIP)

  server.on("/", HTTP_GET,  handleRoot);//主页回调函数(发送HTML到客户端)
  server.onNotFound(handleRoot);//设置无法响应的http请求的回调函数(失败需要跳到主页否则强制门户不能实现)
  server.on("/Handledata", HTTP_GET, Handledata);//获取用户输入的数据进行处理
  server.begin();//开启服务器
 
  for (;;)
  {
    server.handleClient(); //HTTP请求处理
    dnsServer.processNextRequest();//DNS请求处理

    if(back_flg)
    {
      back_flg = 0;
      server.close();//关闭网络服务
      WiFi.mode(WIFI_STA);
      wifima_flg = 0;
      return 0;
    }
    if (data_get_flg) {
      server.close();//关闭网络服务
      WiFi.mode(WIFI_STA);
      wifima_flg = 0;
      return 1;
    }
    yield();
  }
}


void handleRoot() //主页回调函数
{
  creat_text_tips("启凡科创配置服务<br><br>");

  creat_text_tips("请输入点灯科技注册的设备密匙<br>");
  creat_input_object("<br>密匙:","请正确输入密匙","aa","bkey");

  creat_button_object("保存");
  server.send(200, "text/html", pack_url());
}

void Handledata() //处理输入得到的数据
{

  wifima.blinker_id = server.arg("bkey");//密匙
  data_get_flg = 1;
}
