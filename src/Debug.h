#include <iostream>
#include <cstdio>

#ifndef NDEBUG
	#define DEBUG_PRINTF(fmt, ...) fprintf(stdout, fmt, __VA_ARGS__)
	#define DEBUG_COUT(x) do { std::cout << x; } while (0)
#else
	#define DEBUG_PRINTF(fmt, ...) do { } while (0)
	#define DEBUG_COUT() do { } while (0)
#endif
