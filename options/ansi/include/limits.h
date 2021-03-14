#ifndef _LIMITS_H
#define _LIMITS_H

#define CHAR_BIT 8

#if defined (__i386__)
#ifndef LONG_MAX
#define LONG_MAX __LONG_MAX__
#endif

#ifndef LONG_MIN
#define LONG_MIN (-LONG_MAX - 1L)
#endif

#ifndef ULONG_MAX
#define ULONG_MAX (LONG_MAX * 2UL + 1UL)
#endif

#ifndef LONG_LONG_MAX
#define LONG_LONG_MAX __LONG_LONG_MAX__
#endif

#ifndef LONG_LONG_MIN
#define LONG_LONG_MIN (-LONG_LONG_MAX - 1LL)
#endif

#ifndef INT_MAX
#define INT_MAX __INT_MAX__
#endif

#ifndef SCHAR_MIN
#define SCHAR_MIN (-SCHAR_MAX - 1)
#endif

#ifndef SCHAR_MAX
#define SCHAR_MAX __SCHAR_MAX__
#endif

#ifndef UCHAR_MAX
# if __SCHAR_MAX__ == __INT_MAX__
#  define UCHAR_MAX (SCHAR_MAX * 2U + 1U)
# else
#  define UCHAR_MAX (SCHAR_MAX * 2 + 1)
# endif
#endif

#ifdef __CHAR_UNSIGNED__
# undef CHAR_MIN
# if __SCHAR_MAX__ == __INT_MAX__
#  define CHAR_MIN 0U
# else
#  define CHAR_MIN 0
# endif
# undef CHAR_MAX
# define CHAR_MAX UCHAR_MAX
#else
# undef CHAR_MIN
# define CHAR_MIN SCHAR_MIN
# undef CHAR_MAX
# define CHAR_MAX SCHAR_MAX
#endif
#endif /* __i386__ */

#ifdef LONG_MAX
# ifdef LONG_MAX == INT32_MAX
#  define LONG_BIT 32
# else
// Safe assumption
#  define LONG_BIT 64
# endif
#elif defined __LONG_MAX__
# if __LONG_MAX__ == INT32_MAX
#  define LONG_BIT 32
# else
// Safe assumption
#  define LONG_BIT 64
# endif
#else
# error "Unsupported configuration, please define either LONG_MAX or __LONG_MAX__"
#endif

#undef LLONG_MAX
#undef ULLONG_MAX
#define LLONG_MIN (-__LONG_LONG_MAX__ - 1LL)
#define LLONG_MAX __LONG_LONG_MAX__
#define ULLONG_MAX (__LONG_LONG_MAX__ * 2ULL + 1ULL)

#define NAME_MAX 255
#define PATH_MAX 4096
#define LINE_MAX 4096
#define PIPE_BUF 4096

// This value is a guaranteed minimum, get the current maximum from sysconf
#define NGROUPS_MAX 8

#if INTPTR_MAX == INT64_MAX
# define SSIZE_MAX LONG_MAX
#elif INTPTR_MAX == INT32_MAX
# define SSIZE_MAX INT_MAX
#endif

#define _POSIX_ARG_MAX 4096
#define _POSIX_OPEN_MAX 16

#endif // _LIMITS_H
