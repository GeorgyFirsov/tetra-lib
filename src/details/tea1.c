/**
 * @file tea1.c
 * @brief TEA1 algorithm functions implementation 
 */


#include <inttypes.h>

#include "details/tea1.h"
#include "details/common.h"


// clang-format off
static unsigned char tea1p_sbox[256] = {
    0x9B, 0xF8, 0x3B, 0x72, 0x75, 0x62, 0x88, 0x22, 0xFF, 0xA6, 0x10, 0x4D, 0xA9, 0x97, 0xC3, 0x7B,
    0x9F, 0x78, 0xF3, 0xB6, 0xA0, 0xCC, 0x17, 0xAB, 0x4A, 0x41, 0x8D, 0x89, 0x25, 0x87, 0xD3, 0xE3,
    0xCE, 0x47, 0x35, 0x2C, 0x6D, 0xFC, 0xE7, 0x6A, 0xB8, 0xB7, 0xFA, 0x8B, 0xCD, 0x74, 0xEE, 0x11,
    0x23, 0xDE, 0x39, 0x6C, 0x1E, 0x8E, 0xED, 0x30, 0x73, 0xBE, 0xBB, 0x91, 0xCA, 0x69, 0x60, 0x49,
    0x5F, 0xB9, 0xC0, 0x06, 0x34, 0x2A, 0x63, 0x4B, 0x90, 0x28, 0xAC, 0x50, 0xE4, 0x6F, 0x36, 0xB0,
    0xA4, 0xD2, 0xD4, 0x96, 0xD5, 0xC9, 0x66, 0x45, 0xC5, 0x55, 0xDD, 0xB2, 0xA1, 0xA8, 0xBF, 0x37,
    0x32, 0x2B, 0x3E, 0xB5, 0x5C, 0x54, 0x67, 0x92, 0x56, 0x4C, 0x20, 0x6B, 0x42, 0x9D, 0xA7, 0x58,
    0x0E, 0x52, 0x68, 0x95, 0x09, 0x7F, 0x59, 0x9C, 0x65, 0xB1, 0x64, 0x5E, 0x4F, 0xBA, 0x81, 0x1C,
    0xC2, 0x0C, 0x02, 0xB4, 0x31, 0x5B, 0xFD, 0x1D, 0x0A, 0xC8, 0x19, 0x8F, 0x83, 0x8A, 0xCF, 0x33,
    0x9E, 0x3A, 0x80, 0xF2, 0xF9, 0x76, 0x26, 0x44, 0xF1, 0xE2, 0xC4, 0xF5, 0xD6, 0x51, 0x46, 0x07,
    0x14, 0x61, 0xF4, 0xC1, 0x24, 0x7A, 0x94, 0x27, 0x00, 0xFB, 0x04, 0xDF, 0x1F, 0x93, 0x71, 0x53,
    0xEA, 0xD8, 0xBD, 0x3D, 0xD0, 0x79, 0xE6, 0x7E, 0x4E, 0x9A, 0xD7, 0x98, 0x1B, 0x05, 0xAE, 0x03,
    0xC7, 0xBC, 0x86, 0xDB, 0x84, 0xE8, 0xD1, 0xF7, 0x16, 0x21, 0x6E, 0xE5, 0xCB, 0xA3, 0x1A, 0xEC,
    0xA2, 0x7D, 0x18, 0x85, 0x48, 0xDA, 0xAA, 0xF0, 0x08, 0xC6, 0x40, 0xAD, 0x57, 0x0D, 0x29, 0x82,
    0x7C, 0xE9, 0x8C, 0xFE, 0xDC, 0x0F, 0x2D, 0x3C, 0x2E, 0xF6, 0x15, 0x2F, 0xAF, 0xE1, 0xEB, 0x3F,
    0x99, 0x43, 0x13, 0x0B, 0xE0, 0xA5, 0x12, 0x77, 0x5D, 0xB3, 0x38, 0xD9, 0xEF, 0x5A, 0x01, 0x70,
};

static uint16_t tea1p_filter_table1[8] = {
    0xDA86, 0x85E9, 0x29B5, 0x2BC6, 0x8C6B, 0x974C, 0xC671, 0x93E2
};

static uint16_t tea1p_filter_table2[8] = {
    0x85D6, 0x791A, 0xE985, 0xC671, 0x2B9C, 0xEC92, 0xC62B, 0x9C47
};
// clang-format on


/**
 * @brief Initializes key state of the context.
 * 
 * @param context cipher context
 * @param key user-supplied key
 */
TETRALIB_FORCEINLINE void tea1p_initialize_key_state(TEA1_CONTEXT* context, const TEA1_KEY* key)
{
    /*
     * Key state initialization:
     *
     *           <-------------------------------------------------------
     *           +----+----+----+----+----+----+----+----+----+----+----+
     *  eck      |  0 |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 | 10 |
     *           +----+----+----+----+----+----+----+----+----+----+----+
     *              |
     *              +------------------------------------------------+
     *                                                               |
     *           <--------------------                               V
     *           +----+----+----+----+             +------+       +-----+
     *  reduced  |  0 |  1 |  2 |  3 |<------------| SBOX |<------| XOR |
     *  eck      +----+----+----+----+             +------+       +-----+
     *              |              |                                 ^
     *              |              |                                 |
     *              +--------------+---------------------------------+
     */

    for (uint_fast8_t idx = 0; idx < 10; ++idx)
    {
        context->key_state = tea1p_sbox[(uint8_t)((context->key_state >> 24) ^  // Most significant byte
                                                  key->key_bytes[idx] ^         // Key byte
                                                  context->key_state)]          // Least significant byte
                           | (context->key_state << 8);                         // Shift key register to the left
    }
}


/**
 * @brief Filter function. Its implementation is a bit magical.
 * 
 * @param bytes two input bytes
 * @param table filtering table
 * @return filtered byte
 */
TETRALIB_FORCEINLINE uint8_t tea1p_filter(uint16_t bytes, const uint16_t* table)
{
    uint8_t byte1  = (uint8_t)bytes;
    uint8_t byte2  = (uint8_t)(bytes >> 8);
    uint8_t result = 0;

    for (uint_fast8_t i = 0; i < 8; ++i)
    {
        uint8_t magic = ((byte1 >> 7) & 1) | ((byte1 << 1) & 2) | ((byte2 << 1) & 12);

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
TETRALIB_FORCEINLINE uint8_t tea1p_reorder(uint8_t byte)
{
    uint8_t result = 0;

    result |= ((byte << 6) & 0x40);
    result |= ((byte << 1) & 0x20);
    result |= ((byte << 2) & 0x08);
    result |= ((byte >> 3) & 0x14);
    result |= ((byte >> 2) & 0x01);
    result |= ((byte >> 5) & 0x02);
    result |= ((byte << 4) & 0x80);

    return result;
}


void tea1_initialize(TEA1_CONTEXT* context, const TEA1_KEY* key)
{
    //
    // All states contain zeros initially,
    // gamma generator skips 53 steps before
    // the first byte is generated
    //

    context->key_state  = 0;
    context->state      = 0;
    context->skip_steps = 53;

    //
    // Initialize key state
    //

    tea1p_initialize_key_state(context, key);
}


void tea1_set_iv(TEA1_CONTEXT* context, uint32_t iv)
{
    static const uint32_t magic = 0x96724FA1;

    uint32_t low_bytes = iv ^ magic;
    low_bytes          = (low_bytes << 8) | (low_bytes >> 24);

    uint64_t state = ((uint64_t)iv << 32) | low_bytes;
    context->state = (state >> 8) | (state << 56);
}


uint8_t tea1_step(TEA1_CONTEXT* context)
{
    //
    // Derive non-linear feedback from key state
    //

    uint8_t sbox_feedback = tea1p_sbox[(uint8_t)((context->key_state >> 24) ^ context->key_state)];
    context->key_state    = (context->key_state << 8) | sbox_feedback;

    //
    // Compute two filtered and one reordered feedback
    // bytes from state
    //

    uint8_t mix_byte           = tea1p_filter((uint16_t)(context->state >> 8), tea1p_filter_table1);
    uint8_t filter_feedback    = tea1p_filter((uint16_t)(context->state >> 40), tea1p_filter_table2);
    uint8_t reordered_feedback = tea1p_reorder((uint8_t)(context->state >> 32));
    uint8_t state_feedback     = (uint8_t)(context->state >> 56);

    //
    // Compute total feedback, that will be added to the end of the register
    //

    uint8_t feedback = (uint8_t)(filter_feedback ^ state_feedback ^ reordered_feedback ^ sbox_feedback);

    //
    // Update state with feedback and mix
    //

    context->state = ((context->state << 8) ^ ((uint64_t)mix_byte << 32)) | feedback;

    //
    // Get gamma byte from the state
    //

    return (uint8_t)(context->state >> 56);
}


void tea1_generate_gamma(TEA1_CONTEXT* context, uint64_t gamma_bytes, uint8_t* gamma)
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
            tea1_step(context);
        }

        //
        // Generate on gamma byte
        //

        gamma[i] = tea1_step(context);
        context->skip_steps = skip_steps;
    }
}
