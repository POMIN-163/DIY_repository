/**
 * @author: Pomin
 * @date: 2022-03-04 19:07:06
 * @github: https://github.com/POMIN-163
 * @lastedit: 2022-04-05 10:54:09
 * @description: MCU 种类
 **/

#ifndef M_C_U___
#define M_C_U___

#ifdef USE_VSCODE
#ifdef stm32
    #define __CC_ARM
    #define STM32F103xB // F1
    // #define SOC_SERIES_STM32F4
    // #define STM32F40_41xxx // F4
    // #define STM32F407xx // F407
    // #define USE_STDPERIPH_DRIVER // 标准库
    #define USE_HAL_DRIVER // HAL 库
    #define __IO
#endif
#ifdef c51
    #define __C51__
    #define __VSCODE_C51__
    #define reentrant
    #define compact
    #define small
    #define large
    #define data
    #define idata
    #define pdata
    #define bdata
    #define xdata
    #define code
    #define bit char
    #define sbit char
    #define sfr char
    #define sfr16 int
    #define sfr32 int
    #define interrupt
    #define using
    #define _at_
    #define _priority_
    #define _task_
#endif

#ifdef tcxxx
    #define __TASKING__
    #define __attribute__(n)
    #define __interrupt(n)
    #define __vector_table(vectabNum)
    #define __sfrbit16 short
    #define __sfrbit32 int
    #define __fract
    #define __sfract
    #define __laccum
#endif

#ifdef rt1064
    #define CPU_MIMXRT1064DVL6A
    #define SKIP_SYSCLK_INIT
    #define XIP_EXTERNAL_FLASH 1
    #define XIP_BOOT_HEADER_DCD_ENABLE 1
    #define XIP_BOOT_HEADER_ENABLE 1
    #define PRINTF_FLOAT_ENABLE 1
    #define SCANF_FLOAT_ENABLE 1
    #define PRINTF_ADVANCED_ENABLE 1
    #define SCANF_ADVANCED_ENABLE 1
    #define FSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ 0
    #define USB_STACK_BM
#endif
#endif

#endif