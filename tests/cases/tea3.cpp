/**
 * @file tea3.cpp
 * @brief Tests for TEA3
 */

#include "test_common.hpp"


TEST(TEA3, Gamma)
{
    INIT_TEST_VECTORS(TEA3, test_vectors, {
        { 0x11111111, {{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }}, { 0x06, 0xA6, 0x58, 0x8C, 0x5D, 0x9A, 0x99, 0x6D, 0xD2, 0x5E }},
        { 0x01234567, {{ 0xA7, 0x98, 0x39, 0xE4, 0xBA, 0x88, 0xEE, 0x54, 0xA0, 0x29 }}, { 0x02, 0x49, 0x1e, 0xf5, 0x57, 0xc5, 0x1c, 0x17, 0x73, 0x0c }},
    });

    for (const auto& test_vector : test_vectors)
    {
        TEA3_CONTEXT context;

        tea3_initialize(&context, &test_vector.key);
        tea3_set_iv(&context, test_vector.iv);

        constexpr auto gamma_bytes = sizeof(test_vector.expected_gamma);
        uint8_t gamma[gamma_bytes] = {};

        tea3_generate_gamma(&context, gamma_bytes, gamma);
        
        EXPECT_PRED3(test::details::EqualBlocks, test_vector.expected_gamma, gamma, gamma_bytes);
    }
}
