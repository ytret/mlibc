
#include <sys/uio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <frg/vector.hpp>
#include <mlibc/allocator.hpp>
#include <mlibc/debug.hpp>
#include <bits/ensure.h>

ssize_t readv(int, const struct iovec *, int) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}

ssize_t writev(int fd, const struct iovec *iovs, int iovc) {
	__ensure(iovc);

	ssize_t written = 0;
	size_t bytes = 0;
	for(int i = 0; i < iovc; i++) {
		if(SSIZE_MAX - bytes < iovs[i].iov_len) {
			errno = EINVAL;
			return -1;
		}
		bytes += iovs[i].iov_len;
	}
	frg::vector<char, MemoryAllocator> buffer{getAllocator()};
	buffer.resize(bytes);

	size_t to_copy = bytes;
	char *bp = buffer.data();
	for(int i = 0; i < iovc; i++) {
		size_t copy = frg::min(iovs[i].iov_len, to_copy);

		bp = (char *)mempcpy((void *)bp, (void *)iovs[i].iov_base, copy);

		to_copy -= copy;
		if(to_copy == 0)
			break;
	}

	written = write(fd, buffer.data(), bytes);
	return written;
}

ssize_t preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}

ssize_t pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
