/**
 * @file tea1.h
 * @brief TEA1 algorithm functions. 
 */


#ifndef TETRALIB_TEA1_INCLUDED
#define TETRALIB_TEA1_INCLUDED

#include <inttypes.h>


/**
 * @brief Key type for TEA1. The algorithm has 80 bit key.
 */
typedef struct tagTEA1_KEY
{
    uint8_t key_bytes[10];
} TEA1_KEY;


/**
 * @brief TEA1 cipher context.
 */
typedef struct tagTEA1_CONTEXT
{
    uint32_t key_state; /**< */
    uint64_t state;     /**< */
} TEA1_CONTEXT;


/**
 * @brief Key register initialization function.
 * 
 * @param context cipher context
 * @param key user-supplied key
 */
void tea1_initialize(TEA1_CONTEXT* context, const TEA1_KEY* key);


/**
 * @brief Performs one gamma generator step.
 * 
 * Note, that TEA1 skips 54 steps in the beginning and 19 steps
 * between each pair of subsequent gamma bytes used for encryption! 
 * I.e. the following algorithm implements actual TEA1 encryption:
 * 
 * ```c
 * int skip_rounds = 19;
 * 
 * for (int i = 0; i < 54 - skip_rounds; ++i)
 *     tea1_step(&context);
 * 
 * for (int i = 0; i < plaintext_length; ++i)
 * {
 *     uint8_t gamma = 0;
 *     for (int j = 0; j < skip_rounds; ++j)
 *         gamma = tea1_step(&context);
 * 
 *     ciphertext[i] = plaintext[i] ^ gamma;
 * }
 * ```
 * 
 * @param context cipher context
 * @return key stream byte
 */
uint8_t tea1_step(TEA1_CONTEXT* context);

#endif  // !TETRALIB_TEA1_INCLUDED
