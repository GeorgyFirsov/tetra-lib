/**
 * @file tea1.h
 * @brief TEA1 algorithm functions. 
 */

#ifndef TETRALIB_TEA1_INCLUDED
#define TETRALIB_TEA1_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

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
 * 
 * All fields are modified and used inside API-functions.
 * Do not modify them manually.
 */
typedef struct tagTEA1_CONTEXT
{
    uint32_t key_state;  /**< Internal key state */
    uint64_t state;      /**< Internal shift register state */
    uint64_t skip_steps; /**< Number of steps to skip before next gamma byte output */
} TEA1_CONTEXT;


/**
 * @brief Cipher context initialization function.
 * 
 * @param context cipher context
 * @param key user-supplied key
 */
void tea1_initialize(TEA1_CONTEXT* context, const TEA1_KEY* key);


/**
 * @brief Initializes internal state based on initialization vector.
 * 
 * @param context cipher context
 * @param iv initialization vector
 */
void tea1_set_iv(TEA1_CONTEXT* context, uint32_t iv);


/**
 * @brief Performs one gamma generator step.
 * 
 * Note, that TEA1 skips 53 steps in the beginning and 18 steps
 * between each pair of subsequent gamma bytes used for encryption! 
 * I.e. the following algorithm implements actual TEA1 encryption:
 * 
 * ```c
 * int skip_rounds = 18;
 * 
 * for (int i = 0; i < 53 - skip_rounds; ++i)
 *     tea1_step(&context);
 * 
 * for (int i = 0; i < plaintext_length; ++i)
 * {
 *     for (int j = 0; j < skip_rounds; ++j)
 *         tea1_step(&context);
 * 
 *     ciphertext[i] = plaintext[i] ^ tea1_step(&context);
 * }
 * ```
 * 
 * @param context cipher context
 * @return key stream byte
 */
uint8_t tea1_step(TEA1_CONTEXT* context);


/**
 * @brief Generates requested number of bytes of gamma.
 * 
 * Note, that this function is low-level, i.e user MUST initialize
 * context and set IV manually.
 * 
 * @param context cipher context
 * @param gamma_bytes number of gamma bytes requested
 * @param gamma buffer for gamma
 */
void tea1_generate_gamma(TEA1_CONTEXT* context, uint64_t gamma_bytes, uint8_t* gamma);


#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // !TETRALIB_TEA1_INCLUDED
