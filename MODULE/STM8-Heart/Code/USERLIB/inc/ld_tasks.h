/*
 * @Author: Pomin
 * @Date: 2021-09-11 11:17:12
 * @Github: https://github.com/POMIN-163
 * @LastEditTime: 2021-10-08 00:57:58
 */

#ifndef _LD_TASKS_H
#define _LD_TASKS_H
#include "ld_common.h"

typedef struct TaskType {
    func_t task;        /* 任务函数 */
    uint32_t cycle;     /* 周期 (每多少 ticks执行一次) */
    uint32_t lastRun;   /* 上次运行 */
    uint32_t runTime;   /* 运行时长 */
    uint8_t is_running; /* 运行状态 */
    struct TaskType *next;
} ld_task_t;

/* 定义调度器时钟依赖的计时变量数据类型 */
#define CLK_TYPE volatile uint32_t
/* 定义调度器时钟到一个计时变量上面 */
#define CLK_TICK g_tick
#ifndef CLK_TICK
    #error "Must define CLK_TICK, eg : #define CLK_TICK gTime1"
#endif

extern CLK_TYPE CLK_TICK;

uint8_t ld_tasks_load(ld_task_t* handle, uint32_t cycle_length, func_t timeout_task);
uint8_t ld_tasks_del(ld_task_t* handle);

void ld_tasks_run (void);
void ld_tasks_tick(void);

#endif

