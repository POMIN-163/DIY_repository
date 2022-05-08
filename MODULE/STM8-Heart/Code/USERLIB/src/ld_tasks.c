/*
 * @Author: Pomin
 * @Date: 2021-09-11 11:17:06
 * @Github: https://github.com/POMIN-163
 * @LastEditTime: 2021-10-19 13:42:47
 */

#include "ld_tasks.h"

#ifdef CLK_TICK

static ld_task_t* head_handle = (ld_task_t*)0;

/**
 * @brief 调度器载入一项任务 (单链表倒序插入)
 *
 * @param handle 任务指针
 * @return int
**/
uint8_t ld_tasks_load(ld_task_t* handle, uint32_t cycle_length, func_t timeout_task) {
    ld_task_t* target = head_handle;
    handle->cycle  = cycle_length;
    handle->task   = timeout_task;
    handle->lastRun= 0;
    handle->runTime= 0;
    handle->is_running = 1;
    while(target) {
        if (target == handle)
            return 1;       // 已经存在
        target = target->next;
    }
    handle->next = head_handle;
    head_handle  = handle;
    return 0;
}
/**
 * @brief 调度器删除一项任务 (二级指针遍历任务表)
 *
 * @param handle 任务指针
**/
uint8_t ld_tasks_del(ld_task_t* handle) {
    ld_task_t** curr = NULL;
    for (curr = &head_handle; *curr;) {
        ld_task_t* entry = *curr;
        if (entry == handle) {
            *curr = entry->next;
            return 0;
        }
        else
            curr = &entry->next;
    }
    return 1;
}
/**
 * @brief 调度器工作 (单链表遍历)
 *
**/
void ld_tasks_run(void) {
    ld_task_t* target;
    CLK_TYPE timeTemp = CLK_TICK;
    for (target = head_handle; target; target = target->next) {
        if (timeTemp > (target->lastRun + target->cycle) && target->is_running == 1) {
            target->lastRun = timeTemp;
            target->task(target);
            target->runTime = CLK_TICK - timeTemp;
        }
    }
}
/**
 * @brief 调度器时钟工作 (放在计时器中断)
 *
**/
void ld_tasks_tick(void) {
    CLK_TICK++;
}
#endif

