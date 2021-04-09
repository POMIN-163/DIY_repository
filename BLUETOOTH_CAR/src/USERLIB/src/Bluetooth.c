#include "Bluetooth.h"

/*static u8 Compare(u8 arr1[200], u8 arr2[200], u8 length){
    u8 i;
    for(i = 0; i < length; i++){
        if(arr1[i] != arr2[i]) return 0;
    }
    return 1;
}*/
void Bluetooth_Init(void){
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
    uart_init(115200);
}
/*void Bluetooth_Test(void){
    u16 t;
    u16 len;
    u8 USART_[200] = {0xE4, 0xBD, 0xA0, 0xE5, 0xA5, 0xBD,};
     // 你好
    u8 Reply[200] = {0xE4, 0xBD, 0xA0, 0xE6, 0x9C, 0x89, 0xE9, 0x97, 0xAE, 0xE9, 0xA2, 0x98,};
     // 你有问题
    u8 (* Rec)[200];
    u8 (* Tx)[200];
     while(1) {
        if(USART_RX_STA & 0x8000) {
            len = USART_RX_STA & 0x3fff;//得到此次接收到的数据长度
            Rec = &USART_RX_BUF;
            if(Compare(*Rec, USART_, len)) Tx = &Reply;
            for(t = 0; t < 20; t++) {
                if((*Tx)[t] == 0x00) break;
                USART_SendData(USART1, (*Tx)[t]);
                while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
            }
            printf("\r\n\r\n");//插入换行
            USART_RX_STA=0;
        }else{
            printf("Waitting");_SYS_Delay_ms(1000);
        }
    }
}*/
void Bluetooth_Ctrl(void){
    u16 t;
    u16 len;
    u8 Reply[3] = {0x6F, 0x6B, };
    if(USART_RX_STA & 0x8000) {
        len = USART_RX_STA & 0x3fff;//得到此次接收到的数据长度
        switch (USART_RX_BUF[0]) {
            case 0x32:
                Run_flag = Run_Qianjin;
                break;
            case 0x38:
                Run_flag = Run_Houtui;
                break;
            case 0x34:
                Run_flag = Run_Zuo;
                break;
            case 0x36:
                Run_flag = Run_You;
                break;
            case 0x35:
                Run_flag = Run_STOP;
                break;
            case 0x33:
                mode = 2; Choose_First = 1;
                break;
            case 0x31:
                mode = 1; Choose_First = 1;
                break;
        }
        for(t = 0; t < 4; t++) {
            USART_SendData(USART1, Reply[t]);//向串口1发送数据
            while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);//等待发送结束
        }
        printf("\r\n\r\n");//插入换行
        USART_RX_STA=0;
    }
}

