#include "main.h"

void Init(void){
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    _SYS_Init(SysTick_CLKSource_HCLK_Div8, 36);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
// 灯
    _GPIO_Config(
        GPIOC,
        GPIO_Pin_13 ,
        GPIO_Mode_Out_PP, GPIO_Speed_50MHz
    );
// 轮子
    _GPIO_Config(
        GPIOB,
        GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15,
        GPIO_Mode_Out_PP, GPIO_Speed_50MHz
    );
// 蓝牙
    _GPIO_Config(
        GPIOA,
        GPIO_Pin_9 ,
        GPIO_Mode_Out_PP, GPIO_Speed_50MHz
    );
    _GPIO_Config(
        GPIOA,
        GPIO_Pin_10,
        GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz
    );
    Bluetooth_Init();
    //DianCi_Init();
    //TIM3__Init(5000,72);
    //TIM4__Init(5000,72);
    Motor_PWM_Init(2000,36);// 2ms
}
// 初始化

void Turn_2(void){
    GPIOB->ODR &= Run_STOP;
    GPIOB->ODR |= Run_flag;
}
void Test_Turn(void){
    uint time = 20000;
    Run_flag = Run_Qianjin;
    Turn_2();
    _SYS_Delay_ms(time);
    Run_flag = Run_Houtui;
    Turn_2();
    _SYS_Delay_ms(time);
    Run_flag = Run_Zuo;
    Turn_2();
    _SYS_Delay_ms(time);
    Run_flag = Run_You;
    Turn_2();
    _SYS_Delay_ms(time);
}

int main(void) {
    Init();
    mode = 1;
    GPIOB->ODR &= Run_STOP;
    while(1) {
        if (mode == 1){
            Test_Turn();
            PCout(13) = !PCout(13);
        }
        if(mode == 2){
            // 蓝牙遥控
            if(!(TIM_Val % 100)) PCout(13) = !PCout(13);
            if (Choose_First){
                Motor_Ctrl();
                Choose_First = 0;
            }
            TIM_Cmd(TIM4,DISABLE);
            Turn_2();
        }
        Bluetooth_Ctrl();
    }
}

