#include <bits/ensure.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <signal.h>

#include <mlibc/debug.hpp>

char *strdup(const char *string) {
	auto num_bytes = strlen(string);

	char *new_string = (char *)malloc(num_bytes + 1);
	if(!new_string) // TODO: set errno
		return nullptr;

	memcpy(new_string, string, num_bytes);
	new_string[num_bytes] = 0;
	return new_string;
}

char *strndup(const char *string, size_t max_size) {
	auto num_bytes = strnlen(string, max_size);
	char *new_string = (char *)malloc(num_bytes + 1);
	if(!new_string) // TODO: set errno
		return nullptr;

	memcpy(new_string, string, num_bytes);
	new_string[num_bytes] = 0;
	return new_string;
}

size_t strnlen(const char *s, size_t n) {
	__ensure(n >= 0);
	size_t len = 0;
	while(len < n && s[len])
		++len;
	return len;
}

char *strsep(char **m, const char *del) {
	__ensure(m);

	auto tok = *m;
	if(!tok)
		return nullptr;

	// Replace the following delimiter by a null-terminator.
	// After this loop: *p is null iff we reached the end of the string.
	auto p = tok;
	while(*p && !strchr(del, *p))
		p++;

	if(*p) {
		*p = 0;
		*m = p + 1;
	}else{
		*m = nullptr;
	}
	return tok;
}

char *strsignal(int sig) {
	#define CASE_FOR(sigconst) case sigconst: s = #sigconst; break;
	const char *s;
	switch(sig) {
	CASE_FOR(SIGABRT)
	CASE_FOR(SIGFPE)
	CASE_FOR(SIGILL)
	CASE_FOR(SIGINT)
	CASE_FOR(SIGSEGV)
	CASE_FOR(SIGTERM)
	CASE_FOR(SIGPROF)
// TODO: Uncomment these after fixing the ABI.
//	CASE_FOR(SIGIO)
//	CASE_FOR(SIGPWR)
	CASE_FOR(SIGALRM)
	CASE_FOR(SIGBUS)
	CASE_FOR(SIGCHLD)
	CASE_FOR(SIGCONT)
	CASE_FOR(SIGHUP)
	CASE_FOR(SIGKILL)
	CASE_FOR(SIGPIPE)
	CASE_FOR(SIGQUIT)
	CASE_FOR(SIGSTOP)
	CASE_FOR(SIGTSTP)
	CASE_FOR(SIGTTIN)
	CASE_FOR(SIGTTOU)
	CASE_FOR(SIGUSR1)
	CASE_FOR(SIGUSR2)
	CASE_FOR(SIGSYS)
	CASE_FOR(SIGTRAP)
	CASE_FOR(SIGURG)
	CASE_FOR(SIGVTALRM)
	CASE_FOR(SIGXCPU)
	CASE_FOR(SIGXFSZ)
	CASE_FOR(SIGWINCH)
	default:
		mlibc::infoLogger() << "mlibc: Unknown signal number " << sig << frg::endlog;
		s = "Unknown signal number";
	}
	return const_cast<char *>(s);
}

char *strcasestr(const char *s, const char *pattern) {
	size_t plen = strlen(pattern);
	const char *p = s;
	while(*p) {
		// Need strncasecmp() to avoid checking past the end of a successful match.
		if(!strncasecmp(p, pattern, plen))
			return const_cast<char *>(p);
		++p;
	}
	return nullptr;
}
