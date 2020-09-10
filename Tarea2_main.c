/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    Tarea2_EnriqueTeran.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */
#define RTOS_STACK_SIZE 500

#define RTOS_MAX_NUMBER_OF_TASKS 3

#define STACK_FRAME_SIZE 8

#define STACK_PC_OFFSET 2

#define STACK_PSR_OFFSET 1

#define STACK_PSR_DEFAULT 0x01000000

/*PRIORITY DEFINES*/

#define PRIORITY_TASK0 0
#define PRIORITY_TASK1 2
#define PRIORITY_TASK2 1
#define PRIORITY_TASK3 3
#define PRIORITY_IDLETASK 4



typedef enum
{
  stateReady,
  stateRunning,
  stateWaiting
} rtosTaskState_t;

typedef enum
{
  from_execution,
  from_isr
} rtosContextSwitchFrom_t;

typedef struct
{
  uint32_t *sp;
  void (*task_body)();
  uint32_t stack[RTOS_STACK_SIZE];
  uint64_t local_tick;
  rtosTaskState_t state;
  uint8_t priority;
} task_t;

struct
{
  uint8_t nTask;
  int8_t current_task;
  int8_t next_task;
  task_t tasks[RTOS_MAX_NUMBER_OF_TASKS + 1];
  uint64_t global_tick;
} task_list =
{ 0 };

void task0(void);

void task1(void);

void task2(void);

void taskIdel(void);

void rtosStart(void);

void rtosReloadSysTick(void);

void rtosDelay(uint64_t ticks);
/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    /* Task 0 struct fill-in*/
    task_list.tasks[0].task_body = task0;
     task_list.tasks[0].sp = &(task_list.tasks[0].stack[RTOS_STACK_SIZE - 1])
       - STACK_FRAME_SIZE;
     task_list.tasks[0].stack[RTOS_STACK_SIZE - STACK_PC_OFFSET] =
       (uint32_t) task0;
     task_list.tasks[0].stack[RTOS_STACK_SIZE - STACK_PSR_OFFSET] =
       (STACK_PSR_DEFAULT);
     task_list.tasks[0].state = stateReady;
     task_list.tasks[0].priority = PRIORITY_TASK0;
     task_list.nTask++;

     /* Task 1 struct fill-in*/
     task_list.tasks[1].task_body = task1;
      task_list.tasks[1].sp = &(task_list.tasks[1].stack[RTOS_STACK_SIZE - 1])
        - STACK_FRAME_SIZE;
      task_list.tasks[1].stack[RTOS_STACK_SIZE - STACK_PC_OFFSET] =
        (uint32_t) task1;
      task_list.tasks[1].stack[RTOS_STACK_SIZE - STACK_PSR_OFFSET] =
        (STACK_PSR_DEFAULT);
      task_list.tasks[1].state = stateReady;
      task_list.tasks[1].priority = PRIORITY_TASK1;
      task_list.nTask++;

      /* Task 0 struct fill-in*/
      task_list.tasks[2].task_body = task2;
       task_list.tasks[2].sp = &(task_list.tasks[2].stack[RTOS_STACK_SIZE - 1])
         - STACK_FRAME_SIZE;
       task_list.tasks[2].stack[RTOS_STACK_SIZE - STACK_PC_OFFSET] =
         (uint32_t) task2;
       task_list.tasks[2].stack[RTOS_STACK_SIZE - STACK_PSR_OFFSET] =
         (STACK_PSR_DEFAULT);
       task_list.tasks[2].state = stateReady;
       task_list.tasks[2].priority = PRIORITY_TASK2;
       task_list.nTask++;

       /* Idle task struct fill-in*/
       task_list.tasks[task_list.nTask].task_body = taskIdel;
       task_list.tasks[task_list.nTask].sp =
         &(task_list.tasks[task_list.nTask].stack[RTOS_STACK_SIZE - 1])
           - STACK_FRAME_SIZE;
       task_list.tasks[task_list.nTask].stack[RTOS_STACK_SIZE - STACK_PC_OFFSET] =
         (uint32_t) taskIdel;
       task_list.tasks[task_list.nTask].stack[RTOS_STACK_SIZE - STACK_PSR_OFFSET] =
         (STACK_PSR_DEFAULT);
       task_list.tasks[task_list.nTask].state = stateReady;
       task_list.tasks[2].priority = PRIORITY_IDLETASK;
    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++ ;
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
        __asm volatile ("nop");
    }
    return 0 ;
}

void task0(void) // LED Red
{
  while (1)
  {

    PRINTF("Task_0\n\r");

    rtosDelay(10);
  }
}

void task1(void) // LED Red
{
  while (1)
  {

    PRINTF("Task_1\n\r");

    rtosDelay(30);
  }
}

void task2(void) // LED Red
{
  while (1)
  {

    PRINTF("Task_2\n\r");

    rtosDelay(20);
  }
}

void taskIdel(void)
{
  for (;;)
    ;
}
