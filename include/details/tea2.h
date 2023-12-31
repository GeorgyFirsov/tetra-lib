/**
 * @file tea2.h
 * @brief TEA2 algorithm functions. 
 */

#ifndef TETRALIB_TEA2_INCLUDED
#define TETRALIB_TEA2_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <inttypes.h>


/**
 * @brief TEA2 key size in bytes.
 */
#define TEA2_KEY_SIZE 10


/**
 * @brief TEA2 internal key state size in bytes.
 */
#define TEA2_KEY_STATE_SIZE 10


/**
 * @brief Key type for TEA2. The algorithm has 80 bit key.
 */
typedef struct tagTEA2_KEY
{
    uint8_t key_bytes[TEA2_KEY_SIZE];
} TEA2_KEY;


/**
 * @brief TEA2 cipher context.
 * 
 * All fields are modified and used inside API-functions.
 * Do not modify them manually.
 */
typedef struct tagTEA2_CONTEXT
{
    uint8_t key_state[TEA2_KEY_STATE_SIZE]; /**< Internal key state */
    uint64_t state;                         /**< Internal shift register state */
    uint64_t skip_steps;                    /**< Number of steps to skip before next gamma byte output */
} TEA2_CONTEXT;


/**
 * @brief Cipher context initialization function.
 * 
 * @param context cipher context
 * @param key user-supplied key
 */
void tea2_initialize(TEA2_CONTEXT* context, const TEA2_KEY* key);


/**
 * @brief Initializes internal state based on initialization vector.
 * 
 * @param context cipher context
 * @param iv initialization vector
 */
void tea2_set_iv(TEA2_CONTEXT* context, uint32_t iv);


/**
 * @brief Performs one gamma generator step.
 * 
 * Note, that TEA2 skips 50 steps in the beginning and 18 steps
 * between each pair of subsequent gamma bytes used for encryption! 
 * I.e. the following algorithm implements actual TEA2 encryption:
 * 
 * ```c
 * int skip_rounds = 18;
 * 
 * for (int i = 0; i < 50 - skip_rounds; ++i)
 *     tea2_step(&context);
 * 
 * for (int i = 0; i < plaintext_length; ++i)
 * {
 *     for (int j = 0; j < skip_rounds; ++j)
 *         tea2_step(&context);
 * 
 *     ciphertext[i] = plaintext[i] ^ tea2_step(&context);
 * }
 * ```
 * 
 * @param context cipher context
 * @return key stream byte
 */
uint8_t tea2_step(TEA2_CONTEXT* context);


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
void tea2_generate_gamma(TEA2_CONTEXT* context, uint64_t gamma_bytes, uint8_t* gamma);


#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // !TETRALIB_TEA2_INCLUDED
