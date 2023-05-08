#ifndef TASTYLIB_LEAKCHECK_H_
#define TASTYLIB_LEAKCHECK_H_

#define LEAK_CHECK_ENABLED

#if defined(LEAK_CHECK_ENABLED) && defined(_MSC_VER) && defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

/*
The function is available only when compiled with Visual Studio under
Debug model. Memory dump messages will be printed to the output window.
*/
inline void checkMemoryLeaks() {
#ifdef _CRTDBG_MAP_ALLOC
    _CrtDumpMemoryLeaks();
#endif
}

#endif
