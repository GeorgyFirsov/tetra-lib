/**
 * @file tea2.c
 * @brief TEA2 algorithm functions implementation 
 */


#include <inttypes.h>
#include <string.h>

#include "details/tea2.h"
#include "details/common.h"


// clang-format off
static unsigned char tea2p_sbox[256] = {
    0x62, 0xDA, 0xFD, 0xB6, 0xBB, 0x9C, 0xD8, 0x2A, 0xAB, 0x28, 0x6E, 0x42, 0xE7, 0x1C, 0x78, 0x9E, 
    0xFC, 0xCA, 0x81, 0x8E, 0x32, 0x3B, 0xB4, 0xEF, 0x9F, 0x8B, 0xDB, 0x94, 0x0F, 0x9A, 0xA2, 0x96, 
    0x1B, 0x7A, 0xFF, 0xAA, 0xC5, 0xD6, 0xBC, 0x24, 0xDF, 0x44, 0x03, 0x09, 0x0B, 0x57, 0x90, 0xBA, 
    0x7F, 0x1F, 0xCF, 0x71, 0x98, 0x07, 0xF8, 0xA1, 0x60, 0xF7, 0x52, 0x8D, 0xE5, 0xD7, 0x69, 0x87, 
    0x14, 0xED, 0x92, 0xEB, 0xB3, 0x2F, 0xE9, 0x3D, 0xC6, 0x50, 0x5A, 0xA7, 0x45, 0x18, 0x11, 0xC4, 
    0xCE, 0xAC, 0xF4, 0x1D, 0x82, 0x54, 0x3E, 0x49, 0xD5, 0xEE, 0x84, 0x35, 0x41, 0x3A, 0xEC, 0x34, 
    0x17, 0xE0, 0xC9, 0xFE, 0xE8, 0xCB, 0xE6, 0xAE, 0x68, 0xE2, 0x6B, 0x46, 0xC8, 0x47, 0xB2, 0xE3, 
    0x97, 0x10, 0x0E, 0xB8, 0x76, 0x5B, 0xBE, 0xF5, 0xA6, 0x3C, 0x8F, 0xF6, 0xD1, 0xAF, 0xC0, 0x5E, 
    0x7E, 0xCD, 0x7C, 0x51, 0x6D, 0x74, 0x2C, 0x16, 0xF2, 0xA5, 0x65, 0x64, 0x58, 0x72, 0x1E, 0xF1, 
    0x04, 0xA8, 0x13, 0x53, 0x31, 0xB1, 0x20, 0xD3, 0x75, 0x5F, 0xA4, 0x56, 0x06, 0x8A, 0x8C, 0xD9, 
    0x70, 0x12, 0x29, 0x61, 0x4F, 0x4C, 0x15, 0x05, 0xD2, 0xBD, 0x7D, 0x9B, 0x99, 0x83, 0x2B, 0x25, 
    0xD0, 0x23, 0x48, 0x3F, 0xB0, 0x2E, 0x0D, 0x0C, 0xC7, 0xCC, 0xB7, 0x5C, 0xF0, 0xBF, 0x2D, 0x4E, 
    0x40, 0x39, 0x9D, 0x21, 0x37, 0x77, 0x73, 0x4B, 0x4D, 0x5D, 0xFA, 0xDE, 0x00, 0x80, 0x85, 0x6F, 
    0x22, 0x91, 0xDC, 0x26, 0x38, 0xE4, 0x4A, 0x79, 0x6A, 0x67, 0x93, 0xF3, 0xFB, 0x19, 0xA0, 0x7B, 
    0xF9, 0x95, 0x89, 0x66, 0xB9, 0xD4, 0xC1, 0xDD, 0x63, 0x33, 0xE1, 0xC3, 0xB5, 0xA3, 0xC2, 0x27, 
    0x0A, 0x88, 0xA9, 0x1A, 0x6C, 0x43, 0xEA, 0xAD, 0x30, 0x86, 0x36, 0x59, 0x08, 0x55, 0x01, 0x02,
};

static uint16_t tea2p_filter_table1[8] = {
    0x2579, 0x86E5, 0xB6C8, 0x31D6, 0x7394, 0x934D, 0x638E, 0xC68B
};

static uint16_t tea2p_filter_table2[8] = {
    0xD68A, 0x97A1, 0xB2C9, 0x239E, 0x9C71, 0x36E8, 0xC9B2, 0x6CD1
};
// clang-format on


/**
 * @brief Filter function. Its implementation is a bit magical.
 * 
 * @param bytes two input bytes
 * @param table filtering table
 * @return filtered byte
 */
TETRALIB_FORCEINLINE uint8_t tea2p_filter(uint16_t bytes, const uint16_t* table)
{
    uint8_t byte1  = (uint8_t)bytes;
    uint8_t byte2  = (uint8_t)(bytes >> 8);
    uint8_t result = 0;

    for (uint_fast8_t i = 0; i < 8; ++i)
    {
        uint8_t magic = ((byte1 >> 1) & 0x1) | ((byte1 >> 1) & 0x2) |
                        ((byte2 >> 5) & 0x4) | ((byte2 << 3) & 0x8);

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
TETRALIB_FORCEINLINE uint8_t tea2p_reorder(uint8_t byte)
{
    uint8_t result = 0;

    result |= ((byte << 6) & 0x40);
    result |= ((byte << 3) & 0x10);
    result |= ((byte >> 2) & 0x01);
    result |= ((byte << 2) & 0x20);
    result |= ((byte << 3) & 0x80);
    result |= ((byte >> 4) & 0x02);
    result |= ((byte >> 3) & 0x08);
    result |= ((byte >> 5) & 0x04);

    return result;
}


void tea2_initialize(TEA2_CONTEXT* context, const TEA2_KEY* key)
{
    //
    // Internal state contains zeros initially,
    // gamma generator skips 50 steps before
    // the first byte is generated, key state
    // register is just a copy of a key
    //

    memcpy(context->key_state, key, TEA2_KEY_SIZE);
    context->state      = 0;
    context->skip_steps = 50;
}


void tea2_set_iv(TEA2_CONTEXT* context, uint32_t iv)
{
    static const uint32_t magic = 0x5A6E3278;

    uint32_t low_bytes = iv ^ magic;
    low_bytes          = (low_bytes << 8) | (low_bytes >> 24);

    uint64_t state = ((uint64_t)iv << 32) | low_bytes;
    context->state = (state >> 8) | (state << 56);
}


uint8_t tea2_step(TEA2_CONTEXT* context)
{
    //
    // Derive non-linear feedback from key state
    //

    uint8_t sbox_feedback = tea2p_sbox[(uint8_t)(context->key_state[0] ^ context->key_state[7])];
    memmove(context->key_state, context->key_state + 1, TEA2_KEY_STATE_SIZE - 1);
    context->key_state[TEA2_KEY_STATE_SIZE - 1] = sbox_feedback;

    //
    // Compute two filtered and one reordered feedback
    // bytes from state
    //

    uint8_t filter_feedback    = tea2p_filter((uint16_t)context->state, tea2p_filter_table1);
    uint8_t mix_byte           = tea2p_filter((uint16_t)(context->state >> 24), tea2p_filter_table2);
    uint8_t reordered_feedback = tea2p_reorder((uint8_t)(context->state >> 40));
    uint8_t state_feedback1    = (uint8_t)(context->state >> 16);
    uint8_t state_feedback2    = (uint8_t)(context->state >> 56);

    //
    // Compute total feedback, that will be added to the end of the register
    //

    uint8_t feedback = (uint8_t)(filter_feedback ^ state_feedback1 ^ state_feedback2 ^
                                 reordered_feedback ^ sbox_feedback);

    //
    // Update state with feedback and mix
    //

    context->state = ((context->state << 8) ^ ((uint64_t)mix_byte << 24)) | feedback;

    //
    // Get gamma byte from the state
    //

    return (uint8_t)(context->state >> 56);
}


void tea2_generate_gamma(TEA2_CONTEXT* context, uint64_t gamma_bytes, uint8_t* gamma)
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
            tea2_step(context);
        }

        //
        // Generate on gamma byte
        //

        gamma[i]            = tea2_step(context);
        context->skip_steps = skip_steps;
    }
}
