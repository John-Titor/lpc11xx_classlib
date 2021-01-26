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

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/times.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <stdio.h>

#include <uart.h>

extern "C"
void _exit()
{
    for (;;);
}

extern "C"
int _close(int file __unused)
{
    errno = ENOSYS;
    return -1;
}

char *__env[1] = { 0 };
char **environ = __env;

extern "C"
int _execve(char *name __unused, char **argv __unused, char **env __unused)
{
    errno = ENOMEM;
    return -1;
}

extern "C"
int _fork()
{
    errno = EAGAIN;
    return -1;
}

extern "C"
int _fstat(int file __unused, struct stat *st __unused)
{
    errno = ENOSYS;
    return -1;
}

extern "C"
int _getpid()
{
    return 1;
}

extern "C"
int _gettimeofday(struct timeval *__restrict p __unused, void *__restrict tz __unused)
{
    return 0;
}

extern "C"
int _isatty(int file)
{
    switch (file) {
    case 0:
    case 1:
    case 2:
        return 1;
    default:
        errno = EBADF;
        return 0;
    }
}

extern "C"
int _kill(int pid __unused, int sig __unused)
{
    errno = EINVAL;
    return -1;
}

extern "C"
int _link(char *old_name __unused, char *new_name __unused)
{
    errno = ENOSYS;
    return -1;
}

extern "C"
int _lseek(int file __unused, int ptr __unused, int dir __unused)
{
    errno = ENOSYS;
    return -1;
}

extern "C"
int _open(const char *name __unused, int flags __unused, ...)
{
    errno = ENOSYS;
    return -1;
}

extern "C"
int _read(int file __unused, char *ptr __unused, int len __unused)
{
    /* XXX implement UART input */
    errno = EBADF;
    return -1;
}

extern "C" char _end;

extern "C"
caddr_t _sbrk(int incr)
{
    static caddr_t brk = (caddr_t)&_end;
    caddr_t obrk = brk;

    brk += incr;
    return obrk;
}

extern "C"
int _stat(const char *file __unused, struct stat *st __unused)
{
    st->st_mode = S_IFCHR;
    return 0;
}

extern "C"
clock_t _times(struct tms *buf __unused)
{
    return -1;
}

extern "C"
int _unlink(char *name __unused)
{
    errno = ENOENT;
    return -1;
}

extern "C"
int _wait(int *status __unused)
{
    errno = ECHILD;
    return -1;
}

static void
_uputc(char c)
{
    if (c == '\n') {
        _uputc('\r');
    }
    UART << c;
}

extern "C"
int _write(int file, char *ptr, int len)
{
    int resid = len;
    while (resid--) {
        _uputc(*ptr++);
    }
    return len;
}
