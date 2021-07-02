#ifndef _ABIBITS_IN_H
#define _ABIBITS_IN_H

#include <bits/posix/in_addr_t.h>
#include <bits/posix/in_port_t.h>
#include <abi-bits/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

struct in_addr {
	in_addr_t s_addr;
};

struct sockaddr_in {
	sa_family_t sin_family;
	in_port_t sin_port;
	struct in_addr sin_addr;
	uint8_t pad[8];
};
#define sin_zero pad		/* for BSD Unix compatibility */

struct in6_addr {
	union {
		uint8_t __s6_addr[16];
		uint16_t __s6_addr16[8];
		uint32_t __s6_addr32[4];
	} __in6_union;
};
#define s6_addr __in6_union.__s6_addr
#define s6_addr16 __in6_union.__s6_addr16
#define s6_addr32 __in6_union.__s6_addr32

struct sockaddr_in6 {
	sa_family_t sin6_family;
	in_port_t sin6_port;
	uint32_t sin6_flowinfo;
	struct in6_addr sin6_addr;
	uint32_t sin6_scope_id;
};

extern const struct in6_addr in6addr_any;
extern const struct in6_addr in6addr_loopback;

struct ipv6_mreq {
	struct in6_addr ipv6mr_multiaddr;
	unsigned ipv6mr_interface;
};

struct ip_mreq {
	struct in_addr imr_multiaddr;
	struct in_addr imr_interface;
};

struct ip_mreq_source {
	struct in_addr imr_multiaddr;
	struct in_addr imr_interface;
	struct in_addr imr_sourceaddr;
};

struct in_pktinfo {
	unsigned int ipi_ifindex;
	struct in_addr ipi_spec_dst;
	struct in_addr ipi_addr;
};

struct group_source_req {
	uint32_t gsr_interface;
	struct sockaddr_storage gsr_group;
	struct sockaddr_storage gsr_source;
};

#ifdef __cplusplus
}
#endif

#define IN6ADDR_ANY_INIT      { { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } } }
#define IN6ADDR_LOOPBACK_INIT { { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 } } }

#define IPPROTO_IP 1
#define IPPROTO_IPV6 2
#define IPPROTO_ICMP 3
#define IPPROTO_RAW 4
#define IPPROTO_TCP 5
#define IPPROTO_UDP 6

#define INADDR_ANY ((in_addr_t)0x00000000)
#define INADDR_BROADCAST ((in_addr_t)0xffffffff)
#define INADDR_LOOPBACK ((in_addr_t)0x7f000001)
#define INADDR_NONE ((in_addr_t)0xffffffff)

#define INET_ADDRSTRLEN 1

#define INET6_ADDRSTRLEN 1

#define IPV6_JOIN_GROUP 1
#define IPV6_LEAVE_GROUP 2
#define IPV6_MULTICAST_HOPS 3
#define IPV6_MULTICAST_IF 4
#define IPV6_MULTICAST_LOOP 5
#define IPV6_UNICAST_HOPS 6
#define IPV6_V6ONLY 7

#define IP_TOS 1
#define IP_TTL 2
#define IP_OPTIONS 4
#define IP_PKTINFO	8

#define IP_MULTICAST_IF           32
#define IP_MULTICAST_TTL          33
#define IP_MULTICAST_LOOP         34
#define IP_ADD_MEMBERSHIP         35
#define IP_DROP_MEMBERSHIP        36
#define IP_ADD_SOURCE_MEMBERSHIP  39
#define IP_DROP_SOURCE_MEMBERSHIP 40
#define MCAST_JOIN_SOURCE_GROUP   46
#define MCAST_LEAVE_SOURCE_GROUP  47

#define IPV6_ADD_MEMBERSHIP  IPV6_JOIN_GROUP
#define IPV6_DROP_MEMBERSHIP IPV6_LEAVE_GROUP

#endif // _ABIBITS_IN_H
