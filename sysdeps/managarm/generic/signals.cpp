#include <bits/ensure.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include <hel.h>
#include <hel-syscalls.h>

#include <mlibc/debug.hpp>
#include <mlibc/allocator.hpp>
#include <mlibc/posix-pipe.hpp>
#include <mlibc/all-sysdeps.hpp>
#include <posix.frigg_bragi.hpp>

extern "C" void __mlibc_signal_restore();

namespace mlibc {

int sys_sigprocmask(int how, const sigset_t *set, sigset_t *retrieve) {
	// This implementation is inherently signal-safe.
	uint64_t former;
	if(set) {
		HEL_CHECK(helSyscall2_1(kHelObserveSuperCall + 7, how, *set, &former));
	}else{
		HEL_CHECK(helSyscall2_1(kHelObserveSuperCall + 7, 0, 0, &former));
	}
	if(retrieve)
		*retrieve = former;
	return 0;
}

int sys_sigaction(int number, const struct sigaction *__restrict action,
		struct sigaction *__restrict saved_action) {
	SignalGuard sguard;

	// TODO: Respect restorer. __ensure(!(action->sa_flags & SA_RESTORER));

	HelAction actions[3];
	globalQueue.trim();

	managarm::posix::CntRequest<MemoryAllocator> req(getSysdepsAllocator());
	req.set_request_type(managarm::posix::CntReqType::SIG_ACTION);
	if(action) {
		req.set_mode(1);
		req.set_flags(action->sa_flags);
		req.set_sig_number(number);
		req.set_sig_mask(action->sa_mask);
		if(action->sa_flags & SA_SIGINFO) {
			req.set_sig_handler(reinterpret_cast<uintptr_t>(action->sa_sigaction));
		}else{
			req.set_sig_handler(reinterpret_cast<uintptr_t>(action->sa_handler));
		}
		req.set_sig_restorer(reinterpret_cast<uintptr_t>(&__mlibc_signal_restore));
	}

	frg::string<MemoryAllocator> ser(getSysdepsAllocator());
	req.SerializeToString(&ser);
	actions[0].type = kHelActionOffer;
	actions[0].flags = kHelItemAncillary;
	actions[1].type = kHelActionSendFromBuffer;
	actions[1].flags = kHelItemChain;
	actions[1].buffer = ser.data();
	actions[1].length = ser.size();
	actions[2].type = kHelActionRecvInline;
	actions[2].flags = 0;
	HEL_CHECK(helSubmitAsync(getPosixLane(), actions, 3,
			globalQueue.getQueue(), 0, 0));

	auto element = globalQueue.dequeueSingle();
	auto offer = parseHandle(element);
	auto send_req = parseSimple(element);
	auto recv_resp = parseInline(element);

	HEL_CHECK(offer->error);
	HEL_CHECK(send_req->error);
	HEL_CHECK(recv_resp->error);

	managarm::posix::SvrResponse<MemoryAllocator> resp(getSysdepsAllocator());
	resp.ParseFromArray(recv_resp->data, recv_resp->length);
	if(resp.error() == managarm::posix::Errors::ILLEGAL_REQUEST) {
		// This is only returned for servers, not for normal userspace.
		return ENOSYS;
	}else if(resp.error() == managarm::posix::Errors::ILLEGAL_ARGUMENTS) {
		return EINVAL;
	}
	__ensure(resp.error() == managarm::posix::Errors::SUCCESS);

	if(saved_action) {
		saved_action->sa_flags = resp.flags();
		saved_action->sa_mask = resp.sig_mask();
		if(resp.flags() & SA_SIGINFO) {
			saved_action->sa_sigaction =
					reinterpret_cast<void (*)(int, siginfo_t *, void *)>(resp.sig_handler());
		}else{
			saved_action->sa_handler = reinterpret_cast<void (*)(int)>(resp.sig_handler());
		}
		// TODO: saved_action->sa_restorer = resp.sig_restorer;
	}
	return 0;
}

int sys_kill(int pid, int number) {
	// This implementation is inherently signal-safe.
	HEL_CHECK(helSyscall2(kHelObserveSuperCall + 5, pid, number));
	return 0;
}

} //namespace mlibc
