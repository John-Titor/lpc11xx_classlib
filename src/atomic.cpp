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

