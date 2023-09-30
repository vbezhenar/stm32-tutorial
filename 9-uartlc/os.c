#include <stddef.h>
#include <sys/stat.h>
#include <unistd.h>
#include "stm32f103xb.h"

int _close(int fd)
{
    (void)fd;

    for (;;)
    {
    }
}

void _exit(int status)
{
    (void)status;

    for (;;)
    {
    }
}

int _getpid(void)
{
    for (;;)
    {
    }
}

int _kill(int pid, int sig)
{
    (void)pid;
    (void)sig;

    for (;;)
    {
    }
}

off_t _lseek(int fd, off_t pos, int whence)
{
    (void)fd;
    (void)pos;
    (void)whence;

    for (;;)
    {
    }
}

ssize_t _read(int fd, void *buf, size_t cnt)
{
    (void)fd;
    (void)buf;
    (void)cnt;

    for (;;)
    {
    }
}

int _fstat(int fd, struct stat *st)
{
    (void)fd;

    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int fd)
{
    (void)fd;

    return 1;
}

void *_sbrk(ptrdiff_t incr)
{
    extern char _ebss[];
    static char *heap_end = _ebss;
    char *base = heap_end;
    heap_end += incr;
    return base;
}

ssize_t _write(int fd, const void *buf, size_t cnt)
{
    (void)fd;

    const char *string = buf;

    for (size_t index = 0; index < cnt; index++)
    {
        char ch = string[index];

        // wait until data is transferred to the shift register
        while ((USART3->SR & USART_SR_TXE) == 0)
        {
        }

        // write data to the data register
        USART3->DR = (uint32_t)ch & 0x000000ff;
    }

    return cnt;
}
