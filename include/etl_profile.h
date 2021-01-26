// ETL profile

//#define ETL_LOG_ERRORS
#define ETL_ARRAY_VIEW_IS_MUTABLE

// might like to use no_stl version, but that disables <atomic> ...
#include <etl/profiles/cpp17.h>
