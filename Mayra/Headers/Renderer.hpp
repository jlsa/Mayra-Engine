#ifndef Renderer_hpp
#define Renderer_hpp

#include <Mayra.hpp>

#ifdef _MSC_VER
    #define DEBUG_BREAK __debugbreak()
#else
    #include <signal.h>
    #define DEBUG_BREAK raise(SIGINT)
#endif

#define ASSERT(x) if (!(x)) DEBUG_BREAK

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#define BUFFER_OFFSET(x) ((const void*) (std::uintptr_t)(x))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

#endif /* Renderer_h */
