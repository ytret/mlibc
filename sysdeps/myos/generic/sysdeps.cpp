#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <mlibc/all-sysdeps.hpp>
#include <bits/ensure.h>
#include "syscall.hpp"

#define SYS_MEM_MAP		5
#define SYS_MEM_MAP_EC_MAX	100 // return values 0..100 indicate an error
// FIXME: there should be a better way of error handling.

#define SYS_SET_TLS		6

#define SYS_DEBUG_PRINT_STR	9

#define SYS_WRITE		1
#define SYS_READ		2

#define SYS_SEEK_ABS		3
#define SYS_SEEK_REL		4

#define SYS_EXIT		10

#define SYS_IS_TTY		11

#define SYS_GET_PID		12
#define SYS_FORK		13
#define SYS_REPLACE_PROCESS	14

#define __unimplemented() asm volatile ("ud2");
#define __sys_ud2(rt, name, ...) __attribute__((noreturn)) rt sys_ ## name (__VA_ARGS__) { __unimplemented(); while(true) {} }

namespace mlibc {

__sys_ud2(int, futex_wait, int *pointer, int expected)
__sys_ud2(int, futex_wake, int *pointer)
__sys_ud2(int, open, const char *path, int flags, int *fd)
__sys_ud2(int, close, int fd)
__sys_ud2(int, vm_unmap, void *pointer, size_t size)
__sys_ud2(int, clock_get, int clock, time_t *secs, long *nanos)
__sys_ud2(int, anon_free, void *_1, unsigned long _2)

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

void sys_libc_log(const char *message) {
	int ret = do_syscall(SYS_DEBUG_PRINT_STR, message, strlen(message));
	__ensure(ret == 0);
}

int sys_write(int fd, const void *buf, size_t len, ssize_t *bytes_written) {
	int ret = do_syscall(SYS_WRITE, fd, buf, len);
	if (ret < 0)
		return -ret;
	*bytes_written = ret;
	return 0;
}

int sys_read(int fd, void *buf, size_t len, ssize_t *bytes_read) {
	int ret = do_syscall(SYS_READ, fd, buf, len);
	if (ret < 0)
		return -ret;
	*bytes_read = ret;
	return 0;
}

int sys_seek(int fd, off_t offset, int whence, off_t *new_offset) {
	int ret;
	switch (whence) {
	case SEEK_SET:
		ret = do_syscall(SYS_SEEK_ABS, fd, offset);
		break;
	case SEEK_CUR:
		ret = do_syscall(SYS_SEEK_REL, fd, offset);
		break;
	case SEEK_END:
		MLIBC_UNIMPLEMENTED();
		break;
	default:
		return EINVAL;
	}
	if (ret < 0)
		return -ret;
	*new_offset = ret;
	return 0;
}

void sys_exit(int status) {
	do_syscall(SYS_EXIT, status);
	__builtin_trap();
}

int sys_isatty(int fd) {
	int ret = do_syscall(SYS_IS_TTY, fd);
	if (ret < 0)
		return -ret;
	if (ret == 1)
		return 0;
	return 1;
}

void sys_libc_panic(void) {
	sys_libc_log("libc panic");
	sys_exit(-1);
}

pid_t sys_getpid() {
	int ret = do_syscall(SYS_GET_PID);
	return ret;
}

int sys_fork(pid_t *child) {
	int ret = do_syscall(SYS_FORK);
	if (ret < 0)
		return -ret;
	*child = ret;
	return 0;
}

int sys_execve(const char *path, char *const argv[], char *const envp[]) {
	int ret = do_syscall(SYS_REPLACE_PROCESS, path, argv, envp);
	if (ret < 0)
		return -ret;
	sys_libc_log("SYS_REPLACE_PROCESS returned with a return value >= 0");
	sys_libc_panic();
	return 0;
}

}
