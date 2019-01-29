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
#include <string.h>
#include <functional>

#include <interrupt.h>


#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"

////////////////////////////////////////////////////////////////////////////////

template <typename T>
__always_inline
T
__Xatomic_load(volatile T *ptr)
{
    BEGIN_CRITICAL_SECTION;

    return *ptr;

    END_CRITICAL_SECTION;
}

#define ATOMIC_LOAD(_type, _size)                                   \
    extern "C" _type __atomic_load_ ## _size (volatile _type *ptr)  \
    {                                                               \
        return __Xatomic_load(ptr);                                 \
    }                                                               \
    struct hack

ATOMIC_LOAD(uint64_t, 8);

extern "C"
void
__atomic_load(size_t size, void *ptr, void *ret)
{
    BEGIN_CRITICAL_SECTION;

    memcpy(ret, ptr, size);

    END_CRITICAL_SECTION;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
__always_inline
void
__Xatomic_store(volatile T *ptr, T val)
{
    BEGIN_CRITICAL_SECTION;

    *ptr = val;

    END_CRITICAL_SECTION;
}

#define ATOMIC_STORE(_type, _size)                                              \
    extern "C" void __atomic_store_ ## _size (volatile _type *ptr, _type val)   \
    {                                                                           \
        return __Xatomic_store(ptr, val);                                       \
    }                                                                           \
    struct hack

ATOMIC_STORE(uint64_t, 8);

extern "C"
void
__atomic_store(size_t size, void *ptr, void *val)
{
    BEGIN_CRITICAL_SECTION;

    memcpy(ptr, val, size);

    END_CRITICAL_SECTION;
}

////////////////////////////////////////////////////////////////////////////////

template<typename T>
__always_inline
void
__Xatomic_exchange(volatile T *ptr, T *val)
{
    BEGIN_CRITICAL_SECTION;

    auto tmp = *ptr;
    *ptr = *val;
    *val = tmp;


    END_CRITICAL_SECTION;
}

#define ATOMIC_EXCHANGE(_type, _size)                                               \
    extern "C" void __atomic_exchange_ ## _size (volatile _type *ptr, _type *val)   \
    {                                                                               \
        return __Xatomic_exchange(ptr, val);                                        \
    }                                                                               \
    struct hack

ATOMIC_EXCHANGE(uint8_t, 1);
ATOMIC_EXCHANGE(uint16_t, 2);
ATOMIC_EXCHANGE(uint32_t, 4);
ATOMIC_EXCHANGE(uint64_t, 8);

extern "C"
void
__atomic_exchange(size_t size, void *ptr, void *val, void *ret)
{
    BEGIN_CRITICAL_SECTION;

    memcpy(ret, ptr, size);
    memcpy(ptr, val, size);

    END_CRITICAL_SECTION;
}

////////////////////////////////////////////////////////////////////////////////

template<typename T>
__always_inline
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

extern "C"
bool
__atomic_compare_exchange(size_t size, void *ptr, void *expected, void *desired)
{
    BEGIN_CRITICAL_SECTION;

    if (memcmp(ptr, expected, size) == 0) {
        memcpy(ptr, desired, size);
        return true;
    }
    memcpy(desired, ptr, size);
    return false;

    END_CRITICAL_SECTION;
}

////////////////////////////////////////////////////////////////////////////////

template<typename T, class BinaryOperation>
__always_inline
T
__Xatomic_fetch(volatile T *ptr, T val, BinaryOperation op)
{
    BEGIN_CRITICAL_SECTION;

    auto ret = *ptr;
    *ptr = op(ret, val);
    return ret;

    END_CRITICAL_SECTION;
}

#define ATOMIC_FETCH(_type, _size, _name, _op)                                                  \
    extern "C" _type __atomic_fetch_ ## _name ## _ ## _size (volatile _type *ptr, _type val)    \
    {                                                                                           \
        return __Xatomic_fetch(ptr, val, _op<_type>());                                         \
    }                                                                                           \
    struct hack

ATOMIC_FETCH(uint8_t, 1, add, std::plus);
ATOMIC_FETCH(uint16_t, 2, add, std::plus);
ATOMIC_FETCH(uint32_t, 4, add, std::plus);
ATOMIC_FETCH(uint64_t, 8, add, std::plus);

ATOMIC_FETCH(uint8_t, 1, sub, std::minus);
ATOMIC_FETCH(uint16_t, 2, sub, std::minus);
ATOMIC_FETCH(uint32_t, 4, sub, std::minus);
ATOMIC_FETCH(uint64_t, 8, sub, std::minus);

ATOMIC_FETCH(uint8_t, 1, and, std::bit_and);
ATOMIC_FETCH(uint16_t, 2, and, std::bit_and);
ATOMIC_FETCH(uint32_t, 4, and, std::bit_and);
ATOMIC_FETCH(uint64_t, 8, and, std::bit_and);

ATOMIC_FETCH(uint8_t, 1, xor, std::bit_xor);
ATOMIC_FETCH(uint16_t, 2, xor, std::bit_xor);
ATOMIC_FETCH(uint32_t, 4, xor, std::bit_xor);
ATOMIC_FETCH(uint64_t, 8, xor, std::bit_xor);

ATOMIC_FETCH(uint8_t, 1, or, std::bit_or);
ATOMIC_FETCH(uint16_t, 2, or, std::bit_or);
ATOMIC_FETCH(uint32_t, 4, or, std::bit_or);
ATOMIC_FETCH(uint64_t, 8, or, std::bit_or);

////////////////////////////////////////////////////////////////////////////////

template<typename T>
__always_inline
T
__Xatomic_nand(volatile T *ptr, T val)
{
    BEGIN_CRITICAL_SECTION;

    auto ret = *ptr;
    *ptr = ~(ret & val);
    return ret;

    END_CRITICAL_SECTION;
}

#define ATOMIC_NAND(_type, _size)                                               \
    extern "C" _type __atomic_nand_ ## _size (volatile _type *ptr, _type val)   \
    {                                                                           \
        return __Xatomic_nand(ptr, val);                                        \
    }                                                                           \
    struct hack

ATOMIC_NAND(uint8_t, 1);
ATOMIC_NAND(uint16_t, 2);
ATOMIC_NAND(uint32_t, 4);
ATOMIC_NAND(uint64_t, 8);

////////////////////////////////////////////////////////////////////////////////
