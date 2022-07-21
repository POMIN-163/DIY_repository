#include "Display_EPD_W21_spi.h"


void EPD_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);
	
	//CS-->PA4   SCK-->PA6  SDO--->PC4
	// D/C--->PA2	   RES-->PA0
	// BUSY--->PC12

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6;		//Port configuration
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		//Port configuration
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 		
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	// D/C--->PA2	   RES-->PA0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;		//Port configuration
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	  				     		
	
	// BUSY--->PB7
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//Pull up input
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);				//Initialize GPIO

}



void SPI_Write(unsigned char value)                                    
{                                                           
    unsigned char i;

    for(i=0; i<8; i++)   
    {
        EPD_W21_CLK_0;
        if(value & 0x80)
        	EPD_W21_MOSI_1;
        else
        	EPD_W21_MOSI_0;		
        value = (value << 1); 
        EPD_W21_CLK_1; 
    }
}

void EPD_W21_WriteCMD(unsigned char command)
{
  EPD_W21_CS_0;                   
	EPD_W21_DC_0;		// command write
	SPI_Write(command);
	EPD_W21_CS_1;
}
void EPD_W21_WriteDATA(unsigned char data)
{
  EPD_W21_CS_0;                   
	EPD_W21_DC_1;		// data write
	SPI_Write(data);
	EPD_W21_CS_1;
}



/***********************************************************
						end file
***********************************************************/
