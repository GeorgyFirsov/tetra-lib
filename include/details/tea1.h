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

#endif  // !TETRALIB_TEA1_INCLUDED
