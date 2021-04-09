#include "lm393.h"
static unsigned int lspeed;
static unsigned int rspeed;
static unsigned char Espeed;
static unsigned int Tim2Count;

void lm393Init() {
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化GPIO

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //使能

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn; 	 		//使能LSPEED的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;   //抢占优先级2，
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;  		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  			//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;  			//使能RSPEED的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2，
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;  		//子优先级3
	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  		//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);
	tim2IntInit(999,72);//1kHz
}

void EXTI3_IRQHandler(void) {
	lspeed++;
	EXTI_ClearITPendingBit(EXTI_Line3); //清除LINE2上的中断标志位
}

void EXTI4_IRQHandler(void) {
	rspeed++;
	EXTI_ClearITPendingBit(EXTI_Line4); //清除LINE3上的中断标志位
}

void measureVelocity() {
	if (!Espeed){
		//rspeed = 0;
		//lspeed = 0;
		Espeed = 1;//开始测速
		TIM_Cmd(TIM2,ENABLE);
	}
}

void tim2IntInit(unsigned int arr,unsigned int psc) {
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	TIM_Cmd(TIM2,ENABLE);
}

void TIM2_IRQHandler(void) {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
	{
		Tim2Count++;
		if(Tim2Count == 10000) {
			Espeed=0;
			Tim2Count=0;
			//TIM_Cmd(TIM2,DISABLE);
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源
	}
}

unsigned int getLeftSpeed(void) {
	if(Espeed == 1) {
		return lspeed;
	}
	return lspeed;
}

unsigned int getRightSpeed(void) {
	if(Espeed == 1) {
		return rspeed;
	}
	return rspeed;
}




