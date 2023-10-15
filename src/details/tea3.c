/**
 * @file tea3.c
 * @brief TEA3 algorithm functions implementation 
 */


#include <inttypes.h>
#include <string.h>

#include "details/tea3.h"
#include "details/common.h"


// clang-format off
static unsigned char tea3p_sbox[256] = {
    0x7D, 0xBF, 0x7B, 0x92, 0xAE, 0x7C, 0xF2, 0x10, 0x5A, 0x0F, 0x61, 0x7A, 0x98, 0x76, 0x07, 0x64,
    0xEE, 0x89, 0xF7, 0xBA, 0xC2, 0x02, 0x0D, 0xE8, 0x56, 0x2E, 0xCA, 0x58, 0xC0, 0xFA, 0x2A, 0x01,
    0x57, 0x6E, 0x3F, 0x4B, 0x9C, 0xDA, 0xA6, 0x5B, 0x41, 0x26, 0x50, 0x24, 0x3E, 0xF8, 0x0A, 0x86,
    0xB6, 0x5C, 0x34, 0xE9, 0x06, 0x88, 0x1F, 0x39, 0x33, 0xDF, 0xD9, 0x78, 0xD8, 0xA8, 0x51, 0xB2,
    0x09, 0xCD, 0xA1, 0xDD, 0x8E, 0x62, 0x69, 0x4D, 0x23, 0x2B, 0xA9, 0xE1, 0x53, 0x94, 0x90, 0x1E,
    0xB4, 0x3B, 0xF9, 0x4E, 0x36, 0xFE, 0xB5, 0xD1, 0xA2, 0x8D, 0x66, 0xCE, 0xB7, 0xC4, 0x60, 0xED,
    0x96, 0x4F, 0x31, 0x79, 0x35, 0xEB, 0x8F, 0xBB, 0x54, 0x14, 0xCB, 0xDE, 0x6B, 0x2D, 0x19, 0x82,
    0x80, 0xAC, 0x17, 0x05, 0xFF, 0xA4, 0xCF, 0xC6, 0x6F, 0x65, 0xE6, 0x74, 0xC8, 0x93, 0xF4, 0x7E,
    0xF3, 0x43, 0x9F, 0x71, 0xAB, 0x9A, 0x0B, 0x87, 0x55, 0x70, 0x0C, 0xAD, 0xCC, 0xA5, 0x44, 0xE7,
    0x46, 0x45, 0x03, 0x30, 0x1A, 0xEA, 0x67, 0x99, 0xDB, 0x4A, 0x42, 0xD7, 0xAA, 0xE4, 0xC2, 0xD5,
    0xF0, 0x77, 0x20, 0xC3, 0x3C, 0x16, 0xB9, 0xE2, 0xEF, 0x6C, 0x3D, 0x1B, 0x22, 0x84, 0x2F, 0x81,
    0x1D, 0xB1, 0x3A, 0xE5, 0x73, 0x40, 0xD0, 0x18, 0xC7, 0x6A, 0x9E, 0x91, 0x48, 0x27, 0x95, 0x72,
    0x68, 0x0E, 0x00, 0xFC, 0xC5, 0x5F, 0xF1, 0xF5, 0x38, 0x11, 0x7F, 0xE3, 0x5E, 0x13, 0xAF, 0x37,
    0xE0, 0x8A, 0x49, 0x1C, 0x21, 0x47, 0xD4, 0xDC, 0xB0, 0xEC, 0x83, 0x28, 0xB8, 0xF6, 0xA7, 0xC9,
    0x63, 0x59, 0xBD, 0x32, 0x85, 0x08, 0xBE, 0xD3, 0xFD, 0x4C, 0x2C, 0xFB, 0xA0, 0xC1, 0x9D, 0xB3,
    0x52, 0x8C, 0x5D, 0x29, 0x6D, 0x04, 0xBC, 0x25, 0x15, 0x8B, 0x12, 0x9B, 0xD6, 0x75, 0xA3, 0x97,
};

static uint16_t tea3p_filter_table1[8] = {
    0x92A7, 0xA761, 0x974C, 0x6B8C, 0x29CE, 0x176C, 0x39D4, 0x7463
};

static uint16_t tea3p_filter_table2[8] = {
    0x9D58, 0xA46D, 0x176C, 0x79C4, 0xC62B, 0xB2C9, 0x4D93, 0x2E93
};
// clang-format on


/**
 * @brief Filter function. Its implementation is a bit magical.
 * 
 * @param bytes two input bytes
 * @param table filtering table
 * @return filtered byte
 */
TETRALIB_FORCEINLINE uint8_t tea3p_filter(uint16_t bytes, const uint16_t* table)
{
    uint8_t byte1  = (uint8_t)bytes;
    uint8_t byte2  = (uint8_t)(bytes >> 8);
    uint8_t result = 0;

    for (uint_fast8_t i = 0; i < 8; ++i)
    {
        uint8_t magic = ((byte1 >> 5) & 3) | ((byte2 >> 3) & 12);;

        if (table[i] & (1 << magic))
        {
            result |= 1 << i;
        }

        byte1 = ((byte1 >> 1) | (byte1 << 7));
        byte2 = ((byte2 >> 1) | (byte2 << 7));
    }

    return result;
}


/**
 * @brief Reorders bits in an input byte.
 * 
 * @param byte input byte
 * @return byte with reordered bits 
 */
TETRALIB_FORCEINLINE uint8_t tea3p_reorder(uint8_t byte)
{
    uint8_t result = 0;

    result |= ((byte << 6) & 0x40);
    result |= ((byte << 1) & 0x20);
    result |= ((byte << 2) & 0x98);
    result |= ((byte >> 4) & 0x04);
    result |= ((byte >> 3) & 0x01);
    result |= ((byte >> 6) & 0x02);

    return result;
}


void tea3_initialize(TEA3_CONTEXT* context, const TEA3_KEY* key)
{
    //
    // Internal state contains zeros initially,
    // gamma generator skips 50 steps before
    // the first byte is generated, key state
    // register is just a copy of a key
    //

    memcpy(context->key_state, key, TEA3_KEY_SIZE);
    context->state      = 0;
    context->skip_steps = 50;
}


void tea3_set_iv(TEA3_CONTEXT* context, uint32_t iv)
{
    static const uint32_t magic = 0xC43A7D51;

    uint32_t low_bytes = iv ^ magic;
    low_bytes          = (low_bytes << 8) | (low_bytes >> 24);

    uint64_t state = ((uint64_t)iv << 32) | low_bytes;
    context->state = (state >> 8) | (state << 56);
}


uint8_t tea3_step(TEA3_CONTEXT* context)
{
    //
    // Derive non-linear feedback from key state
    //

    uint8_t sbox_feedback = tea3p_sbox[context->key_state[2] ^ context->key_state[7]] ^ context->key_state[0];
    memmove(context->key_state, context->key_state + 1, TEA3_KEY_STATE_SIZE - 1);
    context->key_state[TEA3_KEY_STATE_SIZE - 1] = sbox_feedback;

    //
    // Compute two filtered and one reordered feedback
    // bytes from state
    //

    uint8_t filter_feedback    = tea3p_filter((uint16_t)(context->state >> 8), tea3p_filter_table1);
    uint8_t mix_byte           = tea3p_filter((uint16_t)(context->state >> 40), tea3p_filter_table2);
    uint8_t reordered_feedback = tea3p_reorder((uint8_t)(context->state >> 32));
    uint8_t state_feedback    = (uint8_t)(context->state >> 56);

    //
    // Compute total feedback, that will be added to the end of the register
    //

    uint8_t feedback = (uint8_t)(filter_feedback ^ state_feedback ^ reordered_feedback ^ sbox_feedback);

    //
    // Update state with feedback and mix
    //

    context->state = ((context->state << 8) ^ ((uint64_t)mix_byte << 40)) | feedback;

    //
    // Get gamma byte from the state
    //

    return (uint8_t)(context->state >> 56);
}


void tea3_generate_gamma(TEA3_CONTEXT* context, uint64_t gamma_bytes, uint8_t* gamma)
{
    //
    // Between each pair of gamma bytes, generator skips 18 steps
    //

    static const uint64_t skip_steps = 18;

    for (uint64_t i = 0; i < gamma_bytes; ++i)
    {
        //
        // Skip required number of steps
        //

        for (uint64_t j = 0; j < context->skip_steps; ++j)
        {
            tea3_step(context);
        }

        //
        // Generate on gamma byte
        //

        gamma[i]            = tea3_step(context);
        context->skip_steps = skip_steps;
    }
}
