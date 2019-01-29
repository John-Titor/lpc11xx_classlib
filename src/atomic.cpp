
#include <sys/cdefs.h>

#include <stdint.h>
#include <interrupt.h>

#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"

template<typename T>
bool
__Xatomic_compare_exchange(volatile T *ptr, T *expected, T desired)
{
    BEGIN_CRITICAL_SECTION;

    if (*ptr == *expected) {
        *ptr = desired;
        return true;
    }
    *expected = *ptr;
    return false;

    END_CRITICAL_SECTION;
}

#define ATOMIC_COMPARE_EXCHANGE(_type, _size)                                                                   \
    extern "C" bool __atomic_compare_exchange_ ## _size (volatile _type *ptr, _type *expected, _type desired)   \
    {                                                                                                           \
        return __Xatomic_compare_exchange(ptr, expected, desired);                                              \
    }                                                                                                           \
    struct hack

ATOMIC_COMPARE_EXCHANGE(uint8_t, 1);
ATOMIC_COMPARE_EXCHANGE(uint16_t, 2);
ATOMIC_COMPARE_EXCHANGE(uint32_t, 4);
ATOMIC_COMPARE_EXCHANGE(uint64_t, 8);

