// Copyright (c) 2019 Michael Smith, All Rights Reserved
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  o Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  o Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//

#if scmRTOS_PROCESS_COUNT > 0

#include <sys/cdefs.h>
#include <scmRTOS.h>
#include <etl.h>

#include <LPC11xx.h>

#ifndef scmRTOS_DEFAULT_STACKSIZE
# define scmRTOS_DEFAULT_STACKSIZE  512
#endif

#define PROCESS_DEF(_n)                                                                 \
    typedef OS::process<OS::pr ## _n , scmRTOS_PROC ## _n ##_STACKSIZE> TProc ## _n;    \
    [[noreturn]] extern void main ## _n();                                                \
    namespace OS {                                                                      \
    template<> OS_PROCESS void TProc ## _n ::exec() { main ## _n (); }              \
    } struct hack

#ifndef scmRTOS_PROC0_STACKSIZE
# define scmRTOS_PROC0_STACKSIZE  scmRTOS_DEFAULT_STACKSIZE
#endif
PROCESS_DEF(0);

#if scmRTOS_PROCESS_COUNT > 1
# ifndef scmRTOS_PROC1_STACKSIZE
#  define scmRTOS_PROC1_STACKSIZE  scmRTOS_DEFAULT_STACKSIZE
# endif
PROCESS_DEF(1)
#endif

#if scmRTOS_PROCESS_COUNT > 2
# ifndef scmRTOS_PROC2_STACKSIZE
#  define scmRTOS_PROC2_STACKSIZE  scmRTOS_DEFAULT_STACKSIZE
# endif
PROCESS_DEF(2)
#endif

#if scmRTOS_PROCESS_COUNT > 3
# ifndef scmRTOS_PROC3_STACKSIZE
#  define scmRTOS_PROC3_STACKSIZE  scmRTOS_DEFAULT_STACKSIZE
# endif
PROCESS_DEF(3)
#endif

#if scmRTOS_PROCESS_COUNT > 4
# ifndef scmRTOS_PROC4_STACKSIZE
#  define scmRTOS_PROC4_STACKSIZE  scmRTOS_DEFAULT_STACKSIZE
# endif
PROCESS_DEF(4)
#endif

#if scmRTOS_PROCESS_COUNT > 5
# ifndef scmRTOS_PROC5_STACKSIZE
#  define scmRTOS_PROC5_STACKSIZE  scmRTOS_DEFAULT_STACKSIZE
# endif
PROCESS_DEF(5)
#endif

#if scmRTOS_PROCESS_COUNT > 6
# ifndef scmRTOS_PROC6_STACKSIZE
#  define scmRTOS_PROC6_STACKSIZE  scmRTOS_DEFAULT_STACKSIZE
# endif
PROCESS_DEF(6)
#endif

#if scmRTOS_PROCESS_COUNT > 7
# ifndef scmRTOS_PROC7_STACKSIZE
#  define scmRTOS_PROC7_STACKSIZE  scmRTOS_DEFAULT_STACKSIZE
# endif
PROCESS_DEF(7)
#endif

int
main()
{
    OS::run();
}

#endif // scmRTOS_PROCESS_COUNT > 0

#if scmRTOS_IDLE_HOOK_ENABLE

extern "C" void __idle_hook()
{
    __WFI();
}

extern "C" void __systick_hook()
{
}

namespace OS
{
void idle_process_user_hook() __attribute__((weak, alias("__idle_hook")));
void system_timer_user_hook() { ETL::millisecond_tick(); }
}
#endif
