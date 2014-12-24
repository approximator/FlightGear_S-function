/*!
 * @file OaMemory.c
 *
 * @brief Functions for working with memory.
 *
 * @author Oleksii Aliakin (alex@nls.la)
 * @created Nov 14, 2014
 */

#include <stdlib.h>
#include "memory/OaMemory.h"

void* oa_malloc(int size)
{
    return malloc(size);
}

void oa_free(void* pMem)
{
    free(pMem);
}
