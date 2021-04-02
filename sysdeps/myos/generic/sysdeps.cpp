#include <stddef.h>
#include <stdint.h>
#include <mlibc/all-sysdeps.hpp>

#define __unimplemented() asm volatile ("ud2");
#define __sys_ud2(rt, name, ...) rt sys_ ## name (__VA_ARGS__) { __unimplemented(); }

namespace mlibc {

__sys_ud2(int, futex_wait, int *pointer, int expected)
__sys_ud2(int, futex_wake, int *pointer)
__sys_ud2(int, open, const char *path, int flags, int *fd)
__sys_ud2(int, vm_map, void *hint, size_t size, int prot, int flags, int fd, off_t offset, void **window)
__sys_ud2(int, close, int fd)
__sys_ud2(void, libc_log, const char *message)
__sys_ud2(void, libc_panic)
__sys_ud2(int, seek, int fd, off_t offset, int whence, off_t *new_offset)
__sys_ud2(int, read, int fd, void *data, size_t length, ssize_t *bytes_read)
__sys_ud2(int, vm_unmap, void *pointer, size_t size)
__sys_ud2(int, clock_get, int clock, time_t *secs, long *nanos)
__sys_ud2(void, exit, int status)
__sys_ud2(int, anon_allocate, size_t size, void **pointer)
__sys_ud2(int, anon_free, void *_1, unsigned long _2)
__sys_ud2(int, write, int fd, const void *buffer, size_t size, ssize_t *bytes_written)
__sys_ud2(int, tcb_set, void *pointer)

}
