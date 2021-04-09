#include "TIM.h"
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3__Init(u16 arr,u16 psc) {
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx
}
/*


void TIM4__Init(u16 arr,u16 psc) {
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位

	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx
}*/
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
    {
		TIM_Val++;
		if (TIM_Val == 20000) {
			TIM_Val = 0;
		}
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志
    }
}

void TIM4_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
    {
		/*TIM_flag_4++;
		if (TIM_flag_4 == 20000) {
			TIM_flag_4 = 0;
		}*/
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx更新中断标志
    }
}

void Motor_PWM_Init(u16 arr,u16 psc) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);   //使能定时器4时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //使能GPIOB的时钟

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;          //复用推挽输出
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;   //PB6 7 8 9
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;   //PB6 7 8 9
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	TIM_TimeBaseInitStruct.TIM_Period = arr;              //设定计数器自动重装值
	TIM_TimeBaseInitStruct.TIM_Prescaler  = psc;          //设定预分频器
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;         //设置时钟分割
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);       //初始化定时器

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;             //选择PWM2模式
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	//TIM_OCInitStruct.TIM_Pulse = 0;                          //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;      //设置输出极性
	TIM_OC1Init(TIM4,&TIM_OCInitStruct);                       //初始化输出比较参数

	//TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;             //选择PWM2模式
	//TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	//TIM_OCInitStruct.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	//TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;      //设置输出极性
	TIM_OC2Init(TIM4,&TIM_OCInitStruct);                         //初始化输出比较参数

	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);   //CH1使能预装载寄存器
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);   //CH2使能预装载寄存器
//	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);   //CH3使能预装载寄存器
//	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);   //CH4使能预装载寄存器

	TIM_ARRPreloadConfig(TIM4, ENABLE);                //使能TIM4在ARR上的预装载寄存器

	TIM_Cmd(TIM4,ENABLE);                              //使能定时器4
}
void Ctrl_Flip(u8 _mode){
	TIM_OCInitTypeDef TIM_OCInitStruct;
	if (_mode){
		TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;             //选择PWM2模式
		TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		//TIM_OCInitStruct.TIM_Pulse = 0;                          //设置待装入捕获比较寄存器的脉冲值
		TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;      //设置输出极性
		TIM_OC1Init(TIM4,&TIM_OCInitStruct);                       //初始化输出比较参数
		TIM_OC2Init(TIM4,&TIM_OCInitStruct);                       //初始化输出比较参数
	}else{
		TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;             //选择PWM2模式
		TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		//TIM_OCInitStruct.TIM_Pulse = 0;                          //设置待装入捕获比较寄存器的脉冲值
		TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //设置输出极性
		TIM_OC1Init(TIM4,&TIM_OCInitStruct);                       //初始化输出比较参数
		TIM_OC2Init(TIM4,&TIM_OCInitStruct);                       //初始化输出比较参数
	}
}

void Motor_Ctrl(void) {
  	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //使能GPIOB的时钟
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;          //复用推挽输出
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;   //PB6 7 8 9
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;   //PB6 7 8 9
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}



