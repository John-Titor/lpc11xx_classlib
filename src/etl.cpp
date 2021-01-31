// Copyright (c) 2021 Michael Smith, All Rights Reserved
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

#include <interrupt.h>
#include <etl.h>

namespace ETL {

	void
	init(void)
	{
		// configure SysTick for 1ms interval tick.
		SysTick_Config(CONFIG_CPU_FREQUENCY / 1000);

#if CONFIG_ETL_NUM_CALLBACK_TIMERS > 0
		callback_timer.enable(true);
#endif

	}

	void
	millisecond_tick()
	{
#if CONFIG_ETL_NUM_CALLBACK_TIMERS > 0
		static uint32_t callback_ticks_elapsed = 0;

		if (callback_timer.tick(++callback_ticks_elapsed)) {
			callback_ticks_elapsed = 0;
		}
#endif
	}

	////////////////////////////////////////////////////////////////////////////////
	// callback timers
	//
	#if CONFIG_ETL_NUM_CALLBACK_TIMERS > 0
	etl::callback_timer<CONFIG_ETL_NUM_CALLBACK_TIMERS>	callback_timer;
	#endif // CONFIG_ETL_NUM_CALLBACK_TIMERS


};

#ifdef WITH_SCMRTOS
void
system_timer_user_hook()
#else
void
SysTick_Handler()
#endif
{
	ETL::millisecond_tick();
}
