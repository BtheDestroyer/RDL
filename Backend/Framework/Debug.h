#ifndef __MC_DEBUG

// Code for debugging
// Only functions in Debug builds
#ifndef _MSC_VER
#include <signal.h>
#endif

// Hack to allow STRINGIFY to stringify more things
#ifndef STRINGIFY_INTERNAL
#define STRINGIFY_INTERNAL(x) #x
#endif
// Converts whatever is passed into a string literal
#ifndef STRINGIFY
#define STRINGIFY(x) STRINGIFY_INTERNAL(x)
#endif
// Combines multiple comma-separated "arguments" into a single argument for preprocessor macros
#ifndef SINGLE_ARG
#define SINGLE_ARG(...) __VA_ARGS__
#endif

// DEBUG_ENABLED is true if _DEBUG or _PROFILE is defined and DEBUG_DISABLED is undefined or false
#ifndef DEBUG_ENABLED
#if defined(_DEBUG) || defined(_PROFILE)
#if !defined(DEBUG_DISABLED) || !DEBUG_DISABLED
#define DEBUG_ENABLED true
#else
#define DEBUG_ENABLED false
#endif
#endif
#endif

// Whatever we're defining these as, we need to make sure they're undefined first
#ifdef DEBUG_BREAK
#undef DEBUG_BREAK
#endif
#ifdef DEBUG_ONLY
#undef DEBUG_ONLY
#endif
#ifdef DEBUG_REQUIRE_TRUE
#undef DEBUG_REQUIRE_TRUE
#endif
#ifdef DEBUG_REQUIRE_FALSE
#undef DEBUG_REQUIRE_FALSE
#endif
#ifdef DEBUG_NOT_YET_IMPLEMENTED
#undef DEBUG_NOT_YET_IMPLEMENTED
#endif

#if DEBUG_ENABLED
// Functions as a breakpoint in Visual Studio. Will print all logs to std::cerr if Core/Util/Log.h is included before this.
#ifdef _MSC_VER
#define DEBUG_BREAK() __debugbreak()
#else
#define DEBUG_BREAK() raise(SIGTRAP)
#endif
// Allows for certain lines to only compile in debug mode
#define DEBUG_ONLY(line) line
// Will break execution if `value` does not evaluate to true
#define DEBUG_REQUIRE_TRUE(value) if (!bool(value)) { DEBUG_BREAK(); }
// Will break execution if `value` does not evaluate to false
#define DEBUG_REQUIRE_FALSE(value) if (bool(value)) { DEBUG_BREAK(); }
// Used in the body of a function to mark that it is not implemented yet
#ifdef _MSC_VER
#define DEBUG_NOT_YET_IMPLEMENTED() __pragma(message(__FILE__ "(" STRINGIFY(__LINE__) "): warning: " __FUNCTION__ " has not been implemented yet!")) { DEBUG_BREAK(); }
#else
#define DEBUG_NOT_YET_IMPLEMENTED() DEBUG_BREAK()
#endif
#else // Not building with debugging macros
#define DEBUG_BREAK()
#define DEBUG_ONLY(line)
#define DEBUG_REQUIRE_TRUE(value)
#define DEBUG_REQUIRE_FALSE(value)
#define DEBUG_NOT_YET_IMPLEMENTED()
#endif

#define __MC_DEBUG
#endif
