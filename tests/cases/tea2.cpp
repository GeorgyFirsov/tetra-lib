/**
 * @file tea2.cpp
 * @brief Tests for TEA2
 */

#include "test_common.hpp"


TEST(TEA2, Gamma)
{
    INIT_TEST_VECTORS(TEA2, test_vectors, {
        { 0x12345678, {{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }}, { 0xA7, 0x98, 0x39, 0xE4, 0xBA, 0x88, 0xEE, 0x54, 0xA0, 0x29 }},
        { 0x12345678, {{ 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA }}, { 0x64, 0x70, 0x4E, 0xA9, 0xD7, 0xDC, 0x25, 0x60, 0x81, 0x39 }},
    });

    for (const auto& test_vector : test_vectors)
    {
        TEA2_CONTEXT context;

        tea2_initialize(&context, &test_vector.key);
        tea2_set_iv(&context, test_vector.iv);

        constexpr auto gamma_bytes = sizeof(test_vector.expected_gamma);
        uint8_t gamma[gamma_bytes] = {};

        tea2_generate_gamma(&context, gamma_bytes, gamma);
        
        EXPECT_PRED3(test::details::EqualBlocks, test_vector.expected_gamma, gamma, gamma_bytes);
    }
}