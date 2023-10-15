/**
 * @file tea1.cpp
 * @brief Tests for TEA1
 */

#include "test_common.hpp"


TEST(TEA1, Gamma)
{
    INIT_TEST_VECTORS(TEA1, test_vectors, {
        { 0x11111111, {{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }}, { 0xd3, 0x3f, 0xd8, 0xa6, 0x05, 0xa0, 0xa1, 0xbb, 0x90, 0x23 }},
        { 0x01234567, {{ 0xA7, 0x98, 0x39, 0xE4, 0xBA, 0x88, 0xEE, 0x54, 0xA0, 0x29 }}, { 0x1d, 0xec, 0x9c, 0x7e, 0xc6, 0x22, 0x3d, 0x87, 0xc2, 0xcc }},
    });

    for (const auto& test_vector : test_vectors)
    {
        TEA1_CONTEXT context;

        tea1_initialize(&context, &test_vector.key);
        tea1_set_iv(&context, test_vector.iv);

        constexpr auto gamma_bytes = sizeof(test_vector.expected_gamma);
        uint8_t gamma[gamma_bytes] = {};

        tea1_generate_gamma(&context, gamma_bytes, gamma);
        
        EXPECT_PRED3(test::details::EqualBlocks, test_vector.expected_gamma, gamma, gamma_bytes);
    }
}