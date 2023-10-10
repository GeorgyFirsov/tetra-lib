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
typedef struct tag_tea1_key {
    uint8_t key_bytes[10];
} tea1_key;


/**
 * @brief Key state for TEA1.
 */
typedef uint32_t tea1_key_state_t;


/**
 * @brief Key register initialization function.
 * 
 * @param key user-supplied key
 * @param key_register output register (MUST contain all zeros)
 */
void tea1_initialize_key_register(const tea1_key* key, tea1_key_state_t* key_register);

#endif  // !TETRALIB_TEA1_INCLUDED
