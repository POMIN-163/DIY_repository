#ifndef _DISPLAY_EPD_W21_H_
#define _DISPLAY_EPD_W21_H_

#include "sys.h"

#define EPD_W21_MOSI_0 PAout(5) = 0
#define EPD_W21_MOSI_1 PAout(5) = 1

#define EPD_W21_CLK_0 PAout(4) = 0
#define EPD_W21_CLK_1 PAout(4) = 1

#define EPD_W21_CS_0 PAout(3) = 0
#define EPD_W21_CS_1 PAout(3) = 1

#define EPD_W21_DC_0 PAout(2) = 0
#define EPD_W21_DC_1 PAout(2) = 1

#define EPD_W21_RST_0 PAout(1) = 0
#define EPD_W21_RST_1 PAout(1) = 1

#define isEPD_W21_BUSY PAin(0)

void SPI_Write(unsigned char value);
void EPD_W21_WriteDATA(unsigned char data);
void EPD_W21_WriteCMD(unsigned char command);

#define EPD_WIDTH 400
#define EPD_HEIGHT 300

#define ALLSCREEN_GRAGHBYTES 15000
// EPD
void EPD_W21_Init(void);
void EPD_init(void);
void EPD_init_GUI(void);
void PIC_display(const unsigned char *BW_datas, const unsigned char *R_datas);
void EPD_sleep(void);
void Epaper_READBUSY(void);
void PIC_display_Clear(void);
// Display canvas function
void EPD_Display(unsigned char *BW_Image, unsigned char *R_Image);


#endif
/***********************************************************
                                                end file
***********************************************************/
