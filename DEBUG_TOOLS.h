#pragma once

#define DEBUG_MODE 0

//1 = in debug mode | 0 = not in debug mode
#if DEBUG_MODE == 1
#define LOG(x) std::cout << x << std::endl
#else
#define LOG(x)
#endif