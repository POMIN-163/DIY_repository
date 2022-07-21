#include "Display_EPD_W21_spi.h"
#include "Display_EPD_W21.h"

void delay_xms(unsigned int xms)
{
	unsigned int i;
	while(xms--)
	{
		i=12000;
		while(i--);
	}
}

void EPD_W21_Init(void)
{
	EPD_W21_RST_0;		// Module reset
	delay_xms(10);//At least 10ms delay 
	EPD_W21_RST_1;
	delay_xms(10);//At least 10ms delay 
	
}

void EPD_Display(unsigned char *BW_Image,unsigned char *R_Image)
{
    unsigned int Width, Height,i,j;
    Width = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    Height = EPD_HEIGHT;

    EPD_W21_WriteCMD(0x24);
    for (j = 0; j < Height; j++) {
        for ( i = 0; i < Width; i++) {
           EPD_W21_WriteDATA(BW_Image[i + j * Width]);
        }
    }

    EPD_W21_WriteCMD(0x26);
    for ( j = 0; j < Height; j++) {
        for ( i = 0; i < Width; i++) {
           EPD_W21_WriteDATA(~R_Image[i + j * Width]);
        }
    }
		 //update
		EPD_W21_WriteCMD(0x22); //Display Update Control
		EPD_W21_WriteDATA(0xC7);   
		EPD_W21_WriteCMD(0x20);  //Activate Display Update Sequence
		Epaper_READBUSY(); 
}
//SSD1619
void EPD_init(void)
{	
	  EPD_W21_Init();
	
  	EPD_W21_WriteCMD(0x12);			//SWRESET
		Epaper_READBUSY(); //waiting for the electronic paper IC to release the idle signal
	
		EPD_W21_WriteCMD(0x74);
		EPD_W21_WriteDATA(0x54);
		EPD_W21_WriteCMD(0x7E);
		EPD_W21_WriteDATA(0x3B);
		EPD_W21_WriteCMD(0x2B);  // Reduce glitch under ACVCOM	
		EPD_W21_WriteDATA(0x04);           
		EPD_W21_WriteDATA(0x63);

		EPD_W21_WriteCMD(0x0C);  // Soft start setting
		EPD_W21_WriteDATA(0x8B);           
		EPD_W21_WriteDATA(0x9C);
		EPD_W21_WriteDATA(0x96);
		EPD_W21_WriteDATA(0x0F);

		EPD_W21_WriteCMD(0x01);  // Set MUX as 300
		EPD_W21_WriteDATA(0x2B);           
		EPD_W21_WriteDATA(0x01);
		EPD_W21_WriteDATA(0x00);     

		EPD_W21_WriteCMD(0x11);  // Data entry mode
		EPD_W21_WriteDATA(0x01);   
		
		EPD_W21_WriteCMD(0x44); 
		EPD_W21_WriteDATA(0x00); // RAM x address start at 0
		EPD_W21_WriteDATA(0x31); // RAM x address end at 31h(49+1)*8->400
		EPD_W21_WriteCMD(0x45); 
		EPD_W21_WriteDATA(0x2B);   // RAM y address start at 12Bh     
		EPD_W21_WriteDATA(0x01);
		EPD_W21_WriteDATA(0x00); // RAM y address end at 00h     
		EPD_W21_WriteDATA(0x00);
		EPD_W21_WriteCMD(0x3C); // board
		EPD_W21_WriteDATA(0x01); // HIZ

		EPD_W21_WriteCMD(0x18);
		EPD_W21_WriteDATA(0X80);
		EPD_W21_WriteCMD(0x22);
		EPD_W21_WriteDATA(0XB1);	//Load Temperature and waveform setting.
		EPD_W21_WriteCMD(0x20);
		Epaper_READBUSY(); //waiting for the electronic paper IC to release the idle signal
		

		EPD_W21_WriteCMD(0x4E); 
		EPD_W21_WriteDATA(0x00);
		EPD_W21_WriteCMD(0x4F); 
		EPD_W21_WriteDATA(0x2B);
		EPD_W21_WriteDATA(0x01);
}
void EPD_init_GUI(void)
{	
	  EPD_W21_Init();
	
  	EPD_W21_WriteCMD(0x12);			//SWRESET
		Epaper_READBUSY(); //waiting for the electronic paper IC to release the idle signal
	
		EPD_W21_WriteCMD(0x74);
		EPD_W21_WriteDATA(0x54);
		EPD_W21_WriteCMD(0x7E);
		EPD_W21_WriteDATA(0x3B);
		EPD_W21_WriteCMD(0x2B);  // Reduce glitch under ACVCOM	
		EPD_W21_WriteDATA(0x04);           
		EPD_W21_WriteDATA(0x63);

		EPD_W21_WriteCMD(0x0C);  // Soft start setting
		EPD_W21_WriteDATA(0x8B);           
		EPD_W21_WriteDATA(0x9C);
		EPD_W21_WriteDATA(0x96);
		EPD_W21_WriteDATA(0x0F);

		EPD_W21_WriteCMD(0x01);  // Set MUX as 300
		EPD_W21_WriteDATA(0x2B);           
		EPD_W21_WriteDATA(0x01);
		EPD_W21_WriteDATA(0x01); //Show mirror     

		EPD_W21_WriteCMD(0x11);  // Data entry mode
		EPD_W21_WriteDATA(0x01);   
		
		EPD_W21_WriteCMD(0x44); 
		EPD_W21_WriteDATA(0x00); // RAM x address start at 0
		EPD_W21_WriteDATA(0x31); // RAM x address end at 31h(49+1)*8->400
		EPD_W21_WriteCMD(0x45); 
		EPD_W21_WriteDATA(0x2B);   // RAM y address start at 12Bh     
		EPD_W21_WriteDATA(0x01);
		EPD_W21_WriteDATA(0x00); // RAM y address end at 00h     
		EPD_W21_WriteDATA(0x00);
		EPD_W21_WriteCMD(0x3C); // board
		EPD_W21_WriteDATA(0x01); // HIZ

		EPD_W21_WriteCMD(0x18);
		EPD_W21_WriteDATA(0X80);
		EPD_W21_WriteCMD(0x22);
		EPD_W21_WriteDATA(0XB1);	//Load Temperature and waveform setting.
		EPD_W21_WriteCMD(0x20);
		Epaper_READBUSY(); //waiting for the electronic paper IC to release the idle signal
		

		EPD_W21_WriteCMD(0x4E); 
		EPD_W21_WriteDATA(0x00);
		EPD_W21_WriteCMD(0x4F); 
		EPD_W21_WriteDATA(0x2B);
		EPD_W21_WriteDATA(0x01);
}	
void EPD_sleep(void)
{
  EPD_W21_WriteCMD(0x10); //enter deep sleep
  EPD_W21_WriteDATA(0x01); 
  delay_xms(100);
}

void PIC_display(const unsigned char *BW_datas,const unsigned char *R_datas)
{
   unsigned int i;	
  EPD_W21_WriteCMD(0x24);   //write RAM for black(0)/white (1)
   for(i=0;i<ALLSCREEN_GRAGHBYTES;i++)
   {               
     EPD_W21_WriteDATA(*BW_datas);
			BW_datas++;
   }
	EPD_W21_WriteCMD(0x26);   //write RAM for black(0)/white (1)
   for(i=0;i<ALLSCREEN_GRAGHBYTES;i++)
   {               
     EPD_W21_WriteDATA(~(*R_datas));
			R_datas++;
   }
	 //update
  EPD_W21_WriteCMD(0x22); //Display Update Control
  EPD_W21_WriteDATA(0xC7);   
  EPD_W21_WriteCMD(0x20);  //Activate Display Update Sequence
  Epaper_READBUSY(); 	 
}
void PIC_display_Clear(void)
{
   unsigned int i;	
  EPD_W21_WriteCMD(0x24);   //write RAM for black(0)/white (1)
   for(i=0;i<ALLSCREEN_GRAGHBYTES;i++)
   {               
     EPD_W21_WriteDATA(0xff);
   }
	EPD_W21_WriteCMD(0x26);   //write RAM for black(0)/white (1)
   for(i=0;i<ALLSCREEN_GRAGHBYTES;i++)
   {               
     EPD_W21_WriteDATA(0x00);
   }
	 //update
  EPD_W21_WriteCMD(0x22); //Display Update Control
  EPD_W21_WriteDATA(0xC7);   
  EPD_W21_WriteCMD(0x20);  //Activate Display Update Sequence
  Epaper_READBUSY(); 	 
}

void Epaper_READBUSY(void)
{ 
  while(1)
  {	 //=1 BUSY
     if(isEPD_W21_BUSY==0) break;;
  }  
}














/***********************************************************
						end file
***********************************************************/

