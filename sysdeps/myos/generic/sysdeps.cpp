#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <mlibc/all-sysdeps.hpp>
#include <bits/ensure.h>
#include "syscall.hpp"

#define SYS_MEM_MAP		5
#define SYS_MEM_MAP_EC_MAX	100 // return values 0..100 indicate an error
// FIXME: there should be a better way of error handling.

#define SYS_SET_TLS		6
#define SYS_GET_TLS		7

#define SYS_DEBUG_PRINT_STR	9

#define __unimplemented() asm volatile ("ud2");
#define __sys_ud2(rt, name, ...) __attribute__((noreturn)) rt sys_ ## name (__VA_ARGS__) { __unimplemented(); while(true) {} }

namespace mlibc {

__sys_ud2(int, futex_wait, int *pointer, int expected)
__sys_ud2(int, futex_wake, int *pointer)
__sys_ud2(int, open, const char *path, int flags, int *fd)
__sys_ud2(int, close, int fd)
__sys_ud2(void, libc_panic)
__sys_ud2(int, seek, int fd, off_t offset, int whence, off_t *new_offset)
__sys_ud2(int, read, int fd, void *data, size_t length, ssize_t *bytes_read)
__sys_ud2(int, vm_unmap, void *pointer, size_t size)
__sys_ud2(int, clock_get, int clock, time_t *secs, long *nanos)
__sys_ud2(void, exit, int status)
__sys_ud2(int, anon_free, void *_1, unsigned long _2)
__sys_ud2(int, write, int fd, const void *buffer, size_t size, ssize_t *bytes_written)

int sys_anon_allocate(size_t size, void **pointer) {
	return sys_vm_map(
		nullptr,
		size,
		PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANONYMOUS,
		-1,
		0,
		pointer
	);
}

int sys_vm_map(
	void *hint,
	size_t size,
	int prot,
	int flags,
	int fd,
	off_t offset,
	void **window
) {
	int ret = do_syscall(SYS_MEM_MAP, hint, size, prot, flags, fd, offset);
	if (ret < SYS_MEM_MAP_EC_MAX) {
		return ret;
	}
	*window = sc_ptr_result<void>(ret);
	return 0;
}

int sys_tcb_set(void *pointer) {
	int ret = do_syscall(SYS_SET_TLS, pointer);
	__ensure(ret == 0);
	return 0;
}

int sys_tcb_get() {
	return do_syscall(SYS_GET_TLS);
}

void sys_libc_log(const char *message) {
	int ret = do_syscall(SYS_DEBUG_PRINT_STR, message, strlen(message));
	__ensure(ret == 0);
}

}
