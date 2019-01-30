// scmRTOS OS configuration
//
#pragma once

#ifndef __ASSEMBLER__
#include <stdint.h>
typedef uint16_t      timeout_t;
typedef uint_fast32_t tick_count_t;
#endif // __ASSEMBLER__

// Options that can be overridden by the client at build time.
//
#ifndef scmRTOS_PROCESS_COUNT
#   error Must define scmRTOS_PROCESS_COUNT to use scmRTOS
#endif

#ifndef scmRTOS_IDLE_PROCESS_STACK_SIZE
#   define scmRTOS_IDLE_PROCESS_STACK_SIZE     (50 * sizeof(stack_item_t))  // min is 20*
#endif

#define scmRTOS_IDLE_HOOK_ENABLE                1
#define scmRTOS_SYSTIMER_NEST_INTS_ENABLE       1
#define scmRTOS_SYSTEM_TICKS_ENABLE             1
#define scmRTOS_SYSTIMER_HOOK_ENABLE            1
#define scmRTOS_CONTEXT_SWITCH_USER_HOOK_ENABLE 0
#define scmRTOS_DEBUG_ENABLE                    0
#define scmRTOS_PROCESS_RESTART_ENABLE          0
#define scmRTOS_CONTEXT_SWITCH_HOOK_IS_FAR      0
#define scmRTOS_SUSPENDED_PROCESS_ENABLE        0
