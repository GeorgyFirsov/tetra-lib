/**
 * @file test_utils.hpp
 * @brief Some helpers for tests.
 */

#pragma once

#include <cstddef>


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