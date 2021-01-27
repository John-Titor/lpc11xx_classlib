// ETL profile
#pragma once

#include "config.h"

//#define ETL_LOG_ERRORS
#define ETL_ARRAY_VIEW_IS_MUTABLE
#define ETL_CALLBACK_TIMER_USE_ATOMIC_LOCK

// might like to use no_stl version, but that disables <atomic> ...
#include <etl/profiles/cpp17.h>
