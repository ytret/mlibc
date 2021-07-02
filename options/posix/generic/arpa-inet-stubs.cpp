
#include <arpa/inet.h>
#include <bits/ensure.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <mlibc/bitutil.hpp>
#include <mlibc/debug.hpp>

const struct in6_addr in6addr_any = {{}};

uint32_t htonl(uint32_t x) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	return mlibc::bit_util<uint32_t>::byteswap(x);
#else
	return x;
#endif
}
uint16_t htons(uint16_t x) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	return mlibc::bit_util<uint16_t>::byteswap(x);
#else
	return x;
#endif
}
uint32_t ntohl(uint32_t x) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	return mlibc::bit_util<uint32_t>::byteswap(x);
#else
	return x;
#endif
}
uint16_t ntohs(uint16_t x) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	return mlibc::bit_util<uint16_t>::byteswap(x);
#else
	return x;
#endif
}

// ----------------------------------------------------------------------------
// IPv4 address manipulation.
// ----------------------------------------------------------------------------
in_addr_t inet_addr(const char *p) {
	struct in_addr a;
	if(!inet_aton(p, &a))
		return -1;
	return a.s_addr;
}
char *inet_ntoa(struct in_addr addr) {
	// string: xxx.yyy.zzz.aaa
	// 4 * 3 + 3 + 1 = 12 + 4 = 16
	thread_local static char buffer[16];
	uint32_t proper = htonl(addr.s_addr);
	snprintf(buffer, sizeof(buffer), "%d.%d.%d.%d",
		(proper >> 24) & 0xff, ((proper >> 16) & 0xff),
		(proper >> 8) & 0xff, proper & 0xff);
	return buffer;
}
int inet_aton(const char *string, struct in_addr *dest) {
	int array[4];
	int i = 0;
	char *end;

	for (; i < 4; i++) {
		array[i] = strtoul(string, &end, 0);
		if (*end && *end != '.')
			return 0;
		if (!*end)
			break;
		string = end + 1;
	}

	switch (i) {
		case 0:
			dest->s_addr = htonl(array[0]);
			break;
		case 1:
			if (array[0] > 255 || array[1] > 0xffffff)
				return 0;
			dest->s_addr = htonl((array[0] << 24) | array[1]);
			break;
		case 2:
			if (array[0] > 255 || array[1] > 255 ||
					array[2] > 0xffff)
				return 0;
			dest->s_addr = htonl((array[0] << 24) | (array[1] << 16) |
				array[2]);
			break;
		case 3:
			if (array[0] > 255 || array[1] > 255 ||
					array[2] > 255 || array[3] > 255)
				return 0;
			dest->s_addr = htonl((array[0] << 24) | (array[1] << 16) |
				(array[2] << 8) | array[3]);
			break;
	}

	return 1;
}

// ----------------------------------------------------------------------------
// Generic IP address manipulation.
// ----------------------------------------------------------------------------
const char *inet_ntop(int af, const void *__restrict src, char *__restrict dst,
		socklen_t size) {
	auto source = reinterpret_cast<const struct in_addr*>(src);
	switch (af) {
		case AF_INET:
			if (snprintf(dst, size, "%d.%d.%d.%d",
						source->s_addr & 0xff,
						(source->s_addr & 0xffff) >> 8,
						(source->s_addr & 0xffffff) >> 16,
						source->s_addr >> 24) < (int)size)
				return dst;
			break;
		case AF_INET6:
			__ensure(!"ipv6 is not implemented!");
			__builtin_unreachable();
			break;
		default:
			errno = EAFNOSUPPORT;
			return NULL;
	}

	errno = ENOSPC;
	return NULL;
}
int inet_pton(int af, const char *__restrict src, void *__restrict dst) {
	switch (af) {
		case AF_INET: {
			uint8_t array[4] = {};
			for (int i = 0; i < 4; i++) {
				char *end;
				long int value = strtol(src, &end, 10);
				if (value > 255)
					return 0;
				if (*end != '\0' && *end != '.')
					return 0;
				src = end + 1;
				array[i] = value;
			}
			auto addr = reinterpret_cast<struct in_addr*>(dst);
			memcpy(&addr->s_addr, array, 4);
			break;
		}
		case AF_INET6:
			mlibc::infoLogger() << "inet_pton: ipv6 is not implemented!" << frg::endlog;
			/* fallthrough */
		default:
			errno = EAFNOSUPPORT;
			return -1;
	}

	return 1;
}

