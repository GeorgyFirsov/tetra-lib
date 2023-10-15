/**
 * @file tea3.h
 * @brief TEA3 algorithm functions. 
 */

#ifndef TETRALIB_TEA3_INCLUDED
#define TETRALIB_TEA3_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <inttypes.h>


/**
 * @brief TEA3 key size in bytes.
 */
#define TEA3_KEY_SIZE 10


/**
 * @brief TEA3 internal key state size in bytes.
 */
#define TEA3_KEY_STATE_SIZE 10


/**
 * @brief Key type for TEA3. The algorithm has 80 bit key.
 */
typedef struct tagTEA3_KEY
{
    uint8_t key_bytes[TEA3_KEY_SIZE];
} TEA3_KEY;


/**
 * @brief TEA3 cipher context.
 * 
 * All fields are modified and used inside API-functions.
 * Do not modify them manually.
 */
typedef struct tagTEA3_CONTEXT
{
    uint8_t key_state[TEA3_KEY_STATE_SIZE]; /**< Internal key state */
    uint64_t state;                         /**< Internal shift register state */
    uint64_t skip_steps;                    /**< Number of steps to skip before next gamma byte output */
} TEA3_CONTEXT;


/**
 * @brief Cipher context initialization function.
 * 
 * @param context cipher context
 * @param key user-supplied key
 */
void tea3_initialize(TEA3_CONTEXT* context, const TEA3_KEY* key);


/**
 * @brief Initializes internal state based on initialization vector.
 * 
 * @param context cipher context
 * @param iv initialization vector
 */
void tea3_set_iv(TEA3_CONTEXT* context, uint32_t iv);


/**
 * @brief Performs one gamma generator step.
 * 
 * Note, that TEA3 skips 50 steps in the beginning and 18 steps
 * between each pair of subsequent gamma bytes used for encryption! 
 * I.e. the following algorithm implements actual TEA3 encryption:
 * 
 * ```c
 * int skip_rounds = 18;
 * 
 * for (int i = 0; i < 50 - skip_rounds; ++i)
 *     tea3_step(&context);
 * 
 * for (int i = 0; i < plaintext_length; ++i)
 * {
 *     for (int j = 0; j < skip_rounds; ++j)
 *         tea3_step(&context);
 * 
 *     ciphertext[i] = plaintext[i] ^ tea3_step(&context);
 * }
 * ```
 * 
 * @param context cipher context
 * @return key stream byte
 */
uint8_t tea3_step(TEA3_CONTEXT* context);


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
void tea3_generate_gamma(TEA3_CONTEXT* context, uint64_t gamma_bytes, uint8_t* gamma);


#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // !TETRALIB_TEA3_INCLUDED
