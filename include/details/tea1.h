/**
 * @file tea1.h
 * @brief TEA1 algorithm functions. 
 */


#ifndef TETRALIB_TEA1_INCLUDED
#define TETRALIB_TEA1_INCLUDED

/**
 * @brief Key type for TEA1. The algorithm has 80 bit key.
 */
typedef unsigned char tea1_key_t[10];


/**
 * @brief Key state for TEA1.
 */
typedef unsigned char tea1_key_state_t[4];


/**
 * @brief Key register initialization function.
 * 
 * @param key user-supplied key
 * @param key_register output register (MUST contain all zeros)
 */
void tea1_initialize_key_register(const tea1_key_t* key, tea1_key_state_t* key_register);

#endif  // !TETRALIB_TEA1_INCLUDED
