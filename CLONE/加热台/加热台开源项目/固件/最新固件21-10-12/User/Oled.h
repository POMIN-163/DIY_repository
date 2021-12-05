#ifndef _OLED_H_
#define _OLED_H_

/*
  此库函数由启凡科创编写，不常用功能可能存在一定bug未遇到未修复，请谨慎使用
  Versin : 1.0.1
  Time ： 2021-9-9
*/

#include "Arduino.h"
#include "Wire.h"
#include "oledfont.h"
  
//驱动芯片选择  不用的注释掉
// #define SSD1306_128_64 
#define SSD1306_128_32 
//#define SH1106   

#define WRITE_MODE 1 //1:IIC  0:SPI

//IIC参数
#define SDA_PIN 13  
#define SCL_PIN 12
#define Oled_SLAVE  0x3c//屏幕地址

//SPI参数//根据移植平台自行修改  
//#define OLED_RES_CLR {P00=0;}
//#define OLED_RES_SET {P00=1;}
//#define OLED_CS_CLR {P01=0;}
//#define OLED_CS_SET {P01=1;}
//#define OLED_RW_CLR {P02=0;}
//#define OLED_RW_SET {P02=1;}


//屏幕参数
#define XLevelL      0x00  //驱动地址偏移
#define XLevelH      0x10  //驱动地址偏移
#define YLevel       0xB0  //驱动地址偏移
#define Brightness   127  //屏幕初始化亮度0-255
#define WIDTH        128   //宽度
#define HEIGHT       32    //高度
#define PAGE_SIZE    4  //竖直分辨率/8


//  滚动方向  数字对齐方向  显示方向
#define UP 0  
#define DOWN 1 
#define LEFT 2 
#define RIGHT 3

//屏幕显示参数
#define USUAL 0 //不反色 /正向显示
#define INVERSE 1 //反色 /反向显示


#define WHITE 1  // 白字黑底（默认）
#define BLACK 0 //  黑字白底

#define AUTO_LINE_FEED 1 //自动换行
#define NOT_LINE_FEED 0   //不换行


//字符和汉字预留大小定义
#define FONT_SIZE_8 8
#define FONT_SIZE_16 16
#define CH_SIZE_16 16
#define CH_SIZE_24 24
#define CH_SIZE_32 32




#define OLED_CMD 0
#define OLED_DATA 1



class Oled {
  public:

  /************************************/
  //以下所有函数中 参数Size 为8（6*8）或16（8*16) /mode:1白底黑字 0黑底白字
  /************************************/
  
  /************************************/
  //此类函数行高分辨率为8bit  比如64高度的屏幕y为64/8=8行（0-7）
  /******/

  //屏幕清零指定区域  指定起点xy 清零width列 height*8行 x分辨率为1，y为8
  void choose_clr(unsigned char x,unsigned char y,unsigned char width, unsigned char height);
  
  //屏幕刷新指定区域  指定起点xy 刷新width列 height*8行  x分辨率为1，y为8
  void choose_refresh(unsigned char x,unsigned char y,unsigned char width, unsigned char height);

  /*设定显示函数边界 xy为左上角,xxyy为右下角,超出部分将在显示字符串\中文\位图 显示函数中不显示
    x分辨率为1 y为8
  */
  void xy_set(unsigned char x,unsigned char y,unsigned char xx,unsigned char yy);

  /*
    指定区域起点和宽(width)高(height)  指定单次滚动格数cont(y滚动为1-8格，x滚动不限制) 
    指定滚动方向对该区域进行滚动  x分辨率为1  y和width为8  
    指定总共滚动多少次roll_cont（roll_cont*cont格） 
    roll_mode有上下左右四个方向，参数如下：
    UP 
    DOWN 
    LEFT
    RIGHT
  */
  void roll(unsigned char x,unsigned char y,unsigned char width,unsigned char height,unsigned char cont,unsigned char roll_mode,unsigned char roll_cont);
  

  
  
  /************************************/
  
  /************************************/
  //此类函数行分辨率为1行  比如64高度的屏幕0-63
  /***************/
  //写一个字符,size:8/16,mode 0:白底黑字/1：黑底白字,x分辨率为1/y为1
  void write_char(short x,short y,char chr,unsigned char Size,unsigned char mode);
  

  //写字符串,size:8/16,mode 0:白底黑字/1：黑底白字,fill:1:多余自动换行 0:多余不显示,x分辨率为1/y为1
void str(short x,short y,const void *chr,unsigned char Size,unsigned char mode,unsigned char fill);
  
  /*num最大为±2^31，显示指定位数（len)的整数,size:8/16,返回实际显示了多少位,align为对齐方式，左对齐或右对齐，对齐参数如下：
    LEFT
    RIGHT  */
  unsigned char num(short x,short y,long num,unsigned char len,unsigned char Size,unsigned char align,unsigned char mode);


  
  //显示汉字字符串，支持16/24/32大小（hsize），字库请在oledfont.h中自行添加,mode 0:白底黑字/1：黑底白字,fill：1多余自动换行，0多余不显示
  void chinese(short x,short y,const void *str,unsigned char hsize,unsigned char mode,unsigned char fill);
  //显示汉字字符串，支持16/24/32大小（hsize），字库请在oledfont.h中自行添加,mode 0:白底黑字/1：黑底白字,fill：1多余自动换行，0多余不显示

  
  //任意位置画点 mode:0黑点/1白点
  void point(unsigned char x,unsigned char y,unsigned char mode);
  
  //指定左上角起点,画任意宽(width)高(height)的实心矩形,可用作画直线, mode:0黑色/1白色
  void line(unsigned char x,unsigned char y,unsigned char width, unsigned char height,unsigned char mode);
  
  //xy为左上角位置,显示指定宽(width)高(height)的BMP位图,XY可以是负数,任意边超出部分自动去除,mode:0反色/1不反色

  void BMP(short x,short  y,short width, short height,const void *p, unsigned char mode);
  void BMP(short  y,const void *p);
  
  /*
    xy为左上角位置,自动识别BMP位图长宽显示（需在取模后数组开头分别添加宽度、高度两个数值）
    XY可以是负数,任意边超出部分自动去除,mode:0反色/1不反色
  */
  void BMP(short x,short  y,const void *p,unsigned char mode);
  /*
    xy为左上角位置,自动识别BMP位图长宽显示（需在取模后数组开头分别添加宽度、高度两个数值）
    XY可以是负数,任意边超出部分自动去除
  */
  void BMP(short x,short  y,const void *p);


  //设置自动换行显示行距 默认0
  void row_spacing(unsigned char  x);


  
  //其余函数
  /*************************************/
  //初始化驱动总线速度和引脚,具体参数配置请在Oled.h内进行宏定义配置
  void begin();
  //初始化屏幕
  void init();
  //清空缓存GRAM（清屏）
  void clr();
  //设置背光亮度0-255；需屏幕开启状态下可用，复位后需重新调用
  void light(unsigned char light);
  //刷新屏幕  数据写入到GRAM,必须调用此函数才能更新到屏幕
  void refresh();

  //显示打开
  void display_on();
  //显示关闭
  void display_off();

  
  
  private:



};

//

extern Oled oled;

#endif
