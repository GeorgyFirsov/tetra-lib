/**
 * @file common.h
 * @brief Some useful helper functions, macros, etc.
 */

#ifndef TETRALIB_UTILS_INCLUDED
#define TETRALIB_UTILS_INCLUDED


/**
 * @brief Alignment specifier (alignas is supported since C11).
 */
#if defined(_MSC_VER)
#   define TETRALIB_ALIGN16 __declspec(align(16))
#elif defined(__GNUC__)
#   define TETRALIB_ALIGN16 __attribute__((aligned(16)))
#else
#   error Unsupported target for now
#endif


/**
 * @brief Force inlining specifier.
 */
#if defined(_MSC_VER)
#   define TETRALIB_FORCEINLINE __forceinline
#elif defined(__GNUC__)
#   define TETRALIB_FORCEINLINE __attribute__((always_inline))
#else
#   error Unsupported target for now
#endif


/**
 * @brief Static assertion for C language (prior to C11).
 */
#define TETRALIB_STATIC_ASSERT(cond, msg) \
    typedef char static_assertion_failed_##msg[(cond) ? 1 : -1]

#endif  // !TETRALIB_UTILS_INCLUDED