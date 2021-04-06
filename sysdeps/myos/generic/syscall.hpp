extern "C" {
	static int do_syscall_0(int sc) {
		int ret;
		asm volatile ("int $0x88"
			      : "=a" (ret)
			      : "a" (sc)
			      : "memory");
		return ret;
	}

	static int do_syscall_1(int sc, int arg1) {
		int ret;
		asm volatile ("int $0x88"
			      : "=a" (ret)
			      : "a" (sc),
				"b" (arg1)
			      : "memory");
		return ret;
	}

	static int do_syscall_2(int sc, int arg1, int arg2) {
		int ret;
		asm volatile ("int $0x88"
			      : "=a" (ret)
			      : "a" (sc),
				"b" (arg1),
				"c" (arg2)
			      : "memory");
		return ret;
	}

	static int do_syscall_6(
		int sc,
		int arg1,
		int arg2,
		int arg3,
		int arg4,
		int arg5,
		int arg6
	) {
		int ret;
		static int args[6] = { arg1, arg2, arg3, arg4, arg5, arg6 };
		asm volatile ("int $0x88"
			      : "=a" (ret)
			      : "a" (sc),
				"b" (reinterpret_cast<int>(args))
			      : "memory");
		return ret;
	}
}

namespace mlibc {
	inline int do_nargs_syscall(int sc) {
		return do_syscall_0(sc);
	}

	inline int do_nargs_syscall(int sc, int arg1) {
		return do_syscall_1(sc, arg1);
	}

	inline int do_nargs_syscall(int sc, int arg1, int arg2) {
		return do_syscall_2(sc, arg1, arg2);
	}

	inline int do_nargs_syscall(
		int sc,
		int arg1,
		int arg2,
		int arg3,
		int arg4,
		int arg5,
		int arg6
	) {
		return do_syscall_6(sc, arg1, arg2, arg3, arg4, arg5, arg6);
	}

	inline int sc_cast(int x) { return x; }
	inline int sc_cast(const void *x) {
		return reinterpret_cast<int>(x);
	}

	template<typename T>
	T *sc_ptr_result(int ret) {
		return reinterpret_cast<T *>(ret);
	}

	template<typename... T>
	int do_syscall(int sc, T... args) {
		return static_cast<int>(
			do_nargs_syscall(sc, sc_cast(args)...)
		);
	}
}
