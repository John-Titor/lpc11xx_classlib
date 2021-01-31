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

// Atomic primitives; should be supplied by the toolchain, but...
//
// Note that due to the way LTO is implemented by both GCC and clang,
// this file cannot be LTO'ed, as the symbol references it serves to
// satisfy don't exist until codegen and it will be eliminated before
// then as nothing in here is called by anything else.

#include <interrupt.h>

// Permute a set of functions:
//
// <type> __atomic_fetch_<operation>_<sizeof(type)>(ptr, val, model)
//
// over:
//
// type in (uint8_t, uint16_t, uint32_t, uint64_t)
// operation in (add, sub, and, or, xor, nand)

typedef unsigned char       _t1;
typedef unsigned short      _t2;
typedef unsigned int        _t4;
typedef unsigned long long  _t8;

#define FNDEF(_size, _operation_name, _prefix_operator, _operator)                  \
    extern "C"                                                                      \
    _t##_size __atomic_fetch_ ## _operation_name ## _ ## _size (volatile void *ptr, \
            _t##_size val,                                                          \
            int model)                                                              \
    {                                                                               \
        (void)model;                                                                \
        BEGIN_CRITICAL_SECTION;                                                     \
        auto temp = *(volatile _t##_size *)ptr;                                     \
        *(volatile _t##_size *)ptr = _prefix_operator (temp _operator val);         \
        return temp;                                                                \
        END_CRITICAL_SECTION;                                                       \
    }

FNDEF(1, add,, +)
FNDEF(2, add,, +)
FNDEF(4, add,, +)
FNDEF(8, add,, +)

FNDEF(1, sub,, -)
FNDEF(2, sub,, -)
FNDEF(4, sub,, -)
FNDEF(8, sub,, -)

FNDEF(1, or,, |)
FNDEF(2, or,, |)
FNDEF(4, or,, |)
FNDEF(8, or,, |)

FNDEF(1, and,, &)
FNDEF(2, and,, &)
FNDEF(4, and,, &)
FNDEF(8, and,, &)

FNDEF(1, xor,, ^)
FNDEF(2, xor,, ^)
FNDEF(4, xor,, ^)
FNDEF(8, xor,, ^)

FNDEF(1, nand, ~, &)
FNDEF(2, nand, ~, &)
FNDEF(4, nand, ~, &)
FNDEF(8, nand, ~, &)
