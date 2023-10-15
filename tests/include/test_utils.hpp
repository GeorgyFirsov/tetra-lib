/**
 * @file test_utils.hpp
 * @brief Some helpers for tests.
 */

#pragma once

#include <cstddef>


/**
 * @brief Initialize test vectors.
 */
#define INIT_TEST_VECTORS(cipher, name, ...) \
    struct                                   \
    {                                        \
        uint32_t iv;                         \
        cipher##_KEY key;                    \
        uint8_t expected_gamma[10];          \
    } name[] = __VA_ARGS__


namespace test::details {

/**
 * @brief Test blocks for equality.
 */
inline bool EqualBlocks(const unsigned char* lhs, const unsigned char* rhs, std::size_t block_size)
{
    for (size_t idx = 0; idx < block_size; ++idx)
    {
        if (lhs[idx] != rhs[idx])
        {
            return false;
        }
    }

    return true;
}

}  // namespace test::details